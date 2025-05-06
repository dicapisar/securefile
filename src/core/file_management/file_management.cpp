#include "file_management.h"

#include "./services/encrypt.h"
#include "./services/file.h"
#include "./services/database.h"
#include "./models/session.h"

#include <string>
using namespace std;

const string FileManagement::PATH_ENCRYPT_FILE =  "./encrypted_files/";

FileManagement::FileManagement(DatabaseService* databaseService, EncryptService* encryptService, FileService* fileService)
    : databaseService(databaseService), encryptService(encryptService), fileService(fileService) {}

FileManagement::~FileManagement() = default;

string getCurrentTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

bool FileManagement::encryptFile(const Session& session, const string& file_name, const string& password) {
    // Check if the session is valid
    if (!session.is_logged) {
        return false;
    }

    // Check if the file exists
    vector<unsigned char> file = fileService->readFile("./" + file_name);
    if (file.empty()) {
        return false;
    }

    // Encrypt the file
    vector<unsigned char> encrypted_file = encryptService->encryptAES(file, password);

    // Create the directory if it doesn't exist
    fileService->createDirectory(PATH_ENCRYPT_FILE);

    // Subfolder for the user
    const string user_subfolder = PATH_ENCRYPT_FILE + session.user_student_id + "/";

    // Create the directory if it doesn't exist
    fileService->createDirectory(user_subfolder);

    // Create the user subfolder if it doesn't exist
    const string output_file_name = user_subfolder + file_name + ".enc";

    // Write the encrypted file
    fileService->writeFile(output_file_name, encrypted_file);

    fileService->removeFile(file_name);

    // Save the file information to the database
    User owner_user;
    optional<variant<User, EncryptedFile, SharedFile, MetadataFile, Report>> owner = databaseService->getModelByID(UserModel, session.user_id);
    if (!owner.has_value() || !holds_alternative<User>(*owner)) {
        return false;
    }

    owner_user = get<User>(*owner);

    EncryptedFile encryptedFile;
    encryptedFile.file_name = file_name;
    encryptedFile.file_path = output_file_name;
    encryptedFile.last_modified = getCurrentTime();
    encryptedFile.password = password;
    encryptedFile.owner = owner_user;

    bool isSaved = databaseService->saveEncryptedFile(encryptedFile);

    // TODO: SAVE REPORT

    return isSaved;
}

bool FileManagement::decryptFile(const Session& session, int fileID, const string& password) {
    // Check if the session is valid
    if (!session.is_logged) {
        return false;
    }

    // Get the encrypted file from the database
    optional<variant<User, EncryptedFile, SharedFile, MetadataFile, Report>> encrypted_file = databaseService->getModelByID(EncryptedFileModel, fileID);
    if (!encrypted_file.has_value() || !holds_alternative<EncryptedFile>(*encrypted_file)) {
        return false;
    }

    EncryptedFile encrypted_file_row = get<EncryptedFile>(*encrypted_file);

    if (encrypted_file_row.password != password) {
        return false;
    }

    // Read the encrypted file
    vector<unsigned char> encrypted_file_data = fileService->readFile(encrypted_file_row.file_path);
    if (encrypted_file_data.empty()) {
        return false;
    }

    // Decrypt the file
    vector<unsigned char> decrypted_file = encryptService->decryptAES(encrypted_file_data, password);
    if (decrypted_file.empty()) {
        return false;
    }

    // Create the directory for decrypted files
    const string decrypted_file_path = "./";

    // Write the decrypted file
    const string output_decrypted_file = decrypted_file_path + encrypted_file_row.file_name;

    fileService->writeFile(output_decrypted_file, decrypted_file);

    // TODO: SAVE REPORT

    return true;
}

bool FileManagement::deleteFile(const Session& session, int fileID, const string& file_name) {

    // 1. Check if the session is valid
    if (!session.is_logged) {
        return false;
    }

    // 2. Get the encrypted file from the database
    optional<variant<User, EncryptedFile, SharedFile, MetadataFile, Report>> encrypted_file = databaseService->getModelByID(EncryptedFileModel, fileID);
    if (!encrypted_file.has_value() || !holds_alternative<EncryptedFile>(*encrypted_file)) {
        return false;
    }

    EncryptedFile encrypted_file_row = get<EncryptedFile>(*encrypted_file);

    // 3. Check if user of the session is the owner of the file
    if (encrypted_file_row.owner.id != session.user_id) {
        return false;
    }

    // 4. Delete encrypted file using file service -> remove: you must use the file_path of the encrypted file
    fileService->removeFile(encrypted_file_row.file_path);

    // 5. Delete the file from the database
    databaseService->deleteRecordByID(EncryptedFileModel, fileID);

    // 6. Generate a report on database
    // TODO: SAVE REPORT

    return true;
}

optional<vector<EncryptedFile>> FileManagement::getListEncryptedFiles(const Session& session) {
    // Check if the session is valid
    if (!session.is_logged) {
        return nullopt;
    }

    // Get the list of shared encrypted files from the database
    optional<vector<EncryptedFile>> shared_encrypted_files = databaseService->getSharedEncryptedFilesByUserID(session.user_id);

    // Get the list of encrypted files from the database
    optional<vector<EncryptedFile>> encrypted_files = databaseService->getEncryptedFilesByOwnerID(session.user_id);

    // Check if the user has any encrypted files
    if (!encrypted_files.has_value() && !shared_encrypted_files.has_value()) {
        return nullopt;
    }

    // Generate the list of all encrypted files
    vector<EncryptedFile> all_encrypted_files;

    if (encrypted_files.has_value()) {
        for (const auto& file : *encrypted_files) {
            all_encrypted_files.push_back(file);
        }
    }

    if (shared_encrypted_files.has_value()) {
        for (const auto& file : *shared_encrypted_files) {
            all_encrypted_files.push_back(file);
        }
    }
    cout << typeid(all_encrypted_files).name() << endl;

    return all_encrypted_files;
}

bool FileManagement::shareFile(const Session& session, int fileID, const string& file_name, const string& student_id, const string& password) {

    // 1. Check if the session is valid
    if (!session.is_logged) {
        return false;
    }

    // 2. Get the encrypted file from the database
    optional<variant<User, EncryptedFile, SharedFile, MetadataFile, Report>> encrypted_file = databaseService->getModelByID(EncryptedFileModel, fileID);
    if (!encrypted_file.has_value() || !holds_alternative<EncryptedFile>(*encrypted_file)) {
        return false;
    }

    EncryptedFile encrypted_file_row = get<EncryptedFile>(*encrypted_file);

    // 3. Check if user of the session is the owner of the file
    if (session.user_id != encrypted_file_row.owner.id) {
        return false;
    }

    // 4. Check if the file has password
    bool isOK = encrypted_file_row.password != "";

    // 5. If the file has no password update the encrypted file with the password
    if (!isOK) {
      encrypted_file_row.password = password;
      }

    // 5.1. Generate the encrypted file map
    map<string, string> encrypted_file_map;
    encrypted_file_map["file_name"] = encrypted_file_row.file_name;
    encrypted_file_map["file_path"] = encrypted_file_row.file_path;
    encrypted_file_map["password"] = encrypted_file_row.password;
    encrypted_file_map["last_modified"] = getCurrentTime();
    encrypted_file_map["owner_id"] = to_string(encrypted_file_row.owner.id);

    // 5.2. Update the encrypted file in the database (use the alterAttributeFromModelByID function of database service)
    bool isUpdated = databaseService->alterAttributeFromModelByID(EncryptedFileModel, encrypted_file_row.id, encrypted_file_map);

    if (!isUpdated) {
        return false;
    }

    // 6. Generate the SharedFile object
	User shared_user;

    for (auto& user : databaseService->getUsers()) {
        if (user.student_id == student_id) {
            shared_user = user;
            break;
        }
    }

    SharedFile shared_file;

    shared_file.encrypted_file = encrypted_file_row;
    shared_file.shared_user = shared_user;
    shared_file.date = getCurrentTime();

    // 7. Save the shared file in the database
    bool isSaved = databaseService->saveSharedFile(shared_file);

    // 8. Generate a report on database
    // TODO: SAVE REPORT

    return true;
}