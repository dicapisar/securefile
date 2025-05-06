#include <iostream>
#include <vector>
#include <variant>
#include <core/file_management/file_management.h>

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
#endif

using namespace std;

#include "services/database.h"
#include  "services/file.h"
#include  "services/encrypt.h"
#include "core/auth/auth.h"
#include <string>
#include "ui/ui.h"

class Dependencies {
    public:
        DatabaseService* database;
        EncryptService* encrypt;
        FileService* file;

    Dependencies(DatabaseService* database, EncryptService* encrypt, FileService* file) : database(database), encrypt(encrypt), file(file) {}

    ~Dependencies() {
        delete database;
        delete encrypt;
        delete file;
    }
};

Dependencies loadDependenciesTest() {

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(hOut, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, mode);
    }
#endif

    cout << "👀 Validating installation of Libraries...\n";

    auto* database_service = new DatabaseService();
    database_service->load_SQL_file();
    /**vector<User> users = database_service.getUsers();
    for (const auto& user : users) {
        cout << "ID: " << user.id << " | Nombre: " << user.name
                << " | Email: " << user.email << " | Password: " << user.password
                << " | Admin: " << (user.is_admin ? "Yes" : "No")
                << " | Crated at: " << user.created_at << std::endl;
    }

    auto encryptedFileOpt = database_service.getEncryptedFilesByOwnerID(1);
    if (encryptedFileOpt.has_value()) {
        vector<EncryptedFile> encryptedFiles = encryptedFileOpt.value();
        for (const auto& encryptedFile : encryptedFiles) {
            cout << "\n📂 Encrypted File Found:\n";
            cout << "ID: " << encryptedFile.id << " | Name: " << encryptedFile.file_name
                    << " | Path: " << encryptedFile.file_path << endl;
            cout << "👤 Owner: " << encryptedFile.owner.name << " | Owner Email: " << encryptedFile.owner.email << endl;
        }
    }

    auto sharedEncryptedFileOpt = database_service.getSharedEncryptedFilesByUserID(2);
    if (sharedEncryptedFileOpt.has_value()) {
        vector<EncryptedFile> encryptedFiles = sharedEncryptedFileOpt.value();
        for (const auto& encryptedFile : encryptedFiles) {
            cout << "\n📂 Shared Encrypted File Found:\n";
            cout << "ID: " << encryptedFile.id << " | Name: " << encryptedFile.file_name
                    << " | Path: " << encryptedFile.file_path << endl;
            cout << "👤 Owner: " << encryptedFile.owner.name << " | Owner Email: " << encryptedFile.owner.email << endl;
        }
    } **/

    /*
     * Encrypt
     */

    auto* file_service = new FileService();
    auto* encrypt_service = new EncryptService();

    /**
    const string file_to_encrypt = "test.txt";
    const string password = "password";

    vector<unsigned char> file = file_service.readFile(file_to_encrypt);

    if (file.empty()) {
        cerr << "❌ Error reading file. \n" << endl;
    }

    vector<unsigned char> encrypted_file = encrypt_service.encryptAES(file, password);

    file_service.createDirectory("./encrypted_files/");

    const string output_file_name = "./encrypted_files/" + file_to_encrypt + ".enc";

    file_service.writeFile(output_file_name, encrypted_file);

    cout << "📁 File Encrypted Successfully at: " << output_file_name << endl;

    **/

    /*
     * Decrypt
     */

    /**

    string path_encrypted_file = "./encrypted_files/" + file_to_encrypt + ".enc";
    vector<unsigned char> encrypted_file_to_decrypt = file_service.readFile(path_encrypted_file);

    vector<unsigned char> decrypted_file = encrypt_service.decryptAES(encrypted_file_to_decrypt, password);

    file_service.createDirectory("./decrypted_files/");

    const string output_decrypted_file = "./decrypted_files/" + file_to_encrypt;

    file_service.writeFile(output_decrypted_file, decrypted_file);

    cout << "📂 File Decrypted Successfully at: " << output_file_name << endl;

    auto user_test = database_service.getModelByID(UserModel, 1);

    if (user_test && holds_alternative<User>(*user_test)) {
        User u = get<User>(*user_test);
        cout << "ID: " << u.id << "\n";
    }

    auto encrypted_file_test = database_service.getModelByID(EncryptedFileModel, 1);

    if (encrypted_file_test && holds_alternative<EncryptedFile>(*encrypted_file_test)) {
        EncryptedFile ef = get<EncryptedFile>(*encrypted_file_test);
        cout << "ID Encrypted File: " << ef.id << "\n";
    }

    auto share_file_test = database_service.getModelByID(SharedFileModel, 1);

    if (share_file_test && holds_alternative<SharedFile>(*share_file_test)) {
        SharedFile sf = get<SharedFile>(*share_file_test);
        cout << "ID Shared File: " << sf.id << "\n";
    }

    auto metadata_file_test = database_service.getModelByID(MetadataFileModel, 1);

    if (metadata_file_test && holds_alternative<MetadataFile>(*metadata_file_test)) {
        MetadataFile mf = get<MetadataFile>(*metadata_file_test);
        cout << "ID Metadata File: " << mf.id << "\n";
    }


    map<string, string> attributes = {{"name", "Test of update  "}};
    bool isUptaded = database_service.alterAttributeFromModelByID(UserModel, 2, attributes);

    if (!isUptaded) cout << "❌ Error updating";

    **/


    cout << "🎉 All Libraries Working Successfully.\n";

    Dependencies dependencies = Dependencies(database_service, encrypt_service, file_service);

    return dependencies;
}

void start() {
    Dependencies dependencies = loadDependenciesTest();

    Auth auth{ dependencies.database, dependencies.encrypt};
    FileManagement file_management{ dependencies.database, dependencies.encrypt, dependencies.file };
    Session session;
    bool isLoggedIn = false;

    UI::showWelcomeMessage();

    while (true) {
        // Check if the user is logged in
        if (!isLoggedIn) {
            std::vector<std::string> loginOptions = {
                "Login",
                "Quit",
            };

            int optionSelected = UI::showMenu(loginOptions);
            UI::showMessage("You selected option " + std::to_string(optionSelected), MessageType::Info);


            if (optionSelected == 1) {
                UI::showMessage("Please enter your credentials", MessageType::Info);

                UI::showMessage("Student ID: ", MessageType::Info);
                string student_id;
                cin >> student_id;

                UI::showMessage("Password: ", MessageType::Info);
                string password;
                cin >> password;

                session = auth.login(student_id, password);

                if (session.is_logged) {
                    isLoggedIn = true;
                    UI::showMessage("Welcome " + session.user_name, MessageType::Success);
                } else {
                    UI::showMessage("Login failed", MessageType::Error);
                    UI::showMessage("Please try again", MessageType::Info);
                    continue;
                }
            } else {
                break;
            }
        }

        // Show the main menu if the user is logged in
        std::vector<std::string> mainMenuOptions = {
            "Encrypt File",
            "Decrypt File",
            "Delete File",
            "List Files",
            "Share File",
            "See Report",
            "Logout"
        };

        int optionSelected = UI::showMenu(mainMenuOptions);
        UI::showMessage("You selected option " + std::to_string(optionSelected), MessageType::Info);

        switch (optionSelected) {
            case 1: {
                UI::showMessage("Encrypting file...", MessageType::Info);

                // Request the file name
                UI::showMessage("Please enter the file name to encrypt: ", MessageType::Info);
                string file_name;
                cin >> file_name;

                // Request the password
                UI::showMessage("Please enter the password: ", MessageType::Info);
                string password;
                cin >> password;

                bool is_ok = file_management.encryptFile(session, file_name, password);

                if (is_ok) {
                    UI::showMessage("File encrypted successfully", MessageType::Success);
                } else {
                    UI::showMessage("Error encrypting file", MessageType::Error);
                }
                break;
            }
            case 2: {
                UI::showMessage("Decrypting file...", MessageType::Info);

                // Get all encrypted files allowed to the user
                optional<vector<EncryptedFile>> encrypted_files = file_management.getListEncryptedFiles(session);

                if (!encrypted_files.has_value()) {
                    UI::showMessage("No encrypted files found", MessageType::Warning);
                    break;
                }

                // Show the list of encrypted files
                vector<string> headers = {"ID", "File Name", "Owner"};
                vector<map<string,string>> rows;

                int index = 1;
                for (const auto& file : *encrypted_files) {
                    map<string,string> row;
                    row["ID"] = to_string(index);
                    row["File Name"] = file.file_name;
                    row["Owner"] = file.owner.name;
                    rows.push_back(row);
                    index++;
                }
                UI::showTableWithInformation(headers, rows);

                UI::showMessage("Please select the file ID to decrypt: ", MessageType::Info);
                int file_id;
                cin >> file_id;

                if (file_id < 1 || file_id > rows.size()) {
                    UI::showMessage("Invalid file ID", MessageType::Error);
                    break;
                }

                EncryptedFile fileSelected = encrypted_files.value()[file_id - 1];

                if ( fileSelected.password != "" && fileSelected.owner.id != session.user_id) {
                    UI::showMessage("Please enter the password: ", MessageType::Info);
                    string password;
                    cin >> password;

                    if (fileSelected.password != password) {
                        UI::showMessage("Invalid password", MessageType::Error);
                        break;
                    }
                }

                bool isOK = file_management.decryptFile(session, fileSelected.id, fileSelected.password);

                if (!isOK) {
                    UI::showMessage("Error decrypting file", MessageType::Error);
                } else {
                    UI::showMessage("File decrypted successfully", MessageType::Success);
                }
                break;
            }
            case 3: {
                UI::showMessage("Deleting file...", MessageType::Info);

                // 1. Get all encrypted files allowed to the user
                optional<vector<EncryptedFile>> encrypted_files = file_management.getListEncryptedFiles(session);

                if (!encrypted_files.has_value()) {
                    UI::showMessage("No encrypted files found", MessageType::Warning);
                    break;
                }
                // 2. remove the encrypted files which the owner is not the user of the session
                vector<string> headers = {"ID", "File Name"};
                vector<map<string,string>> rows;

                int index = 1;
                for (const auto& file : *encrypted_files) {
                    if (file.owner.id == session.user_id) {
                        map<string,string> row;
                        row["ID"] = to_string(index);
                        row["File Name"] = file.file_name;
                        rows.push_back(row);
                    }
                    index++;
                }

                // 3. Show the list of encrypted files
                UI::showTableWithInformation(headers, rows);

                // 4. Request the file ID to delete
                UI::showMessage("Please select the file ID to delete: ", MessageType::Info);
                int file_id;
                cin >> file_id;

                if (file_id < 1 || file_id > rows.size()) {
                    UI::showMessage("Invalid file ID", MessageType::Error);
                    break;
                }

                // 5. Get the path of the file selected
                EncryptedFile fileSelected = encrypted_files.value()[file_id - 1];

                // 6. Call the delete function of file management
				bool isOK = file_management.deleteFile(session, fileSelected.id, fileSelected.file_name);

                // 7. show the message of success or error
                if (!isOK) {
                    UI::showMessage("Error deleting file", MessageType::Error);
                } else {
                    UI::showMessage("File deleted successfully", MessageType::Success);
                }
                break;
            }
            case 4: {
                UI::showMessage("Listing files...", MessageType::Info);
                // Get all encrypted files allowed to the user
                optional<vector<EncryptedFile>> encrypted_files = file_management.getListEncryptedFiles(session);

                if (!encrypted_files.has_value()) {
                    UI::showMessage("No encrypted files found", MessageType::Warning);
                    break;
                }

                // Show the list of encrypted files
                vector<string> headers = {"ID", "File Name", "Owner"};
                vector<map<string,string>> rows;

                int index = 1;
                for (const auto& file : *encrypted_files) {
                    map<string,string> row;
                    row["ID"] = to_string(index);
                    row["File Name"] = file.file_name;
                    row["Owner"] = file.owner.name;
                    rows.push_back(row);
                    index++;
                }
                UI::showTableWithInformation(headers, rows);
                break;
            }
            case 5: {
                UI::showMessage("Sharing file...", MessageType::Info);

                // 1. Get all encrypted files allowed to the user
                optional<vector<EncryptedFile>> encrypted_files = file_management.getListEncryptedFiles(session);

                if (!encrypted_files.has_value()) {
                    UI::showMessage("No encrypted files found", MessageType::Warning);
                    break;
                }

                // 2. remove the encrypted files which the owner is not the user of the session
                vector<string> headers = {"ID", "File Name"};
                vector<map<string,string>> rows;

                int index = 1;
                for (const auto& file : *encrypted_files) {
                    if (file.owner.id == session.user_id) {
                        map<string,string> row;
                        row["ID"] = to_string(index);
                        row["File Name"] = file.file_name;
                        rows.push_back(row);
                    }
                    index++;
                }

                // 3. Show the list of encrypted files
                UI::showTableWithInformation(headers, rows);

                // 4. Request the file ID to share
                UI::showMessage("Please select the file ID to share: ", MessageType::Info);
                int file_id;
                cin >> file_id;

                if (file_id < 1 || file_id > rows.size()) {
                    UI::showMessage("Invalid file ID", MessageType::Error);
                    break;
                }

                // 5. Get the encrypted file selected
                EncryptedFile fileSelected = encrypted_files.value()[file_id - 1];

                // 6. Validate if the encrypted file has password
                if (fileSelected.password == "") {;
                    UI::showMessage("The file does not have a password, please enter a password: ", MessageType::Info);
                    string password;
                    cin >> password;
                }

                // 7. If the file has password, request the password
                if (fileSelected.password != "") {
                    UI::showMessage("The file has a password, please enter the password: ", MessageType::Info);
                    string password;
                    cin >> password;

                    if (fileSelected.password != password) {
                        UI::showMessage("Invalid password", MessageType::Error);
                        break;
                    }
                }

                // 8. Request the student ID to share
                UI::showMessage("Please enter the student ID to share: ", MessageType::Info);
                string student_id;
                cin >> student_id;

                // 9. Get the path of the file selected
				// ??????

                // 10. Call the share function of file management
                bool isOK = file_management.shareFile(session, fileSelected.id , fileSelected.file_name, student_id, fileSelected.password);

                // 11. show the message of success or error
                if (!isOK) {
                    UI::showMessage("Error sharing file", MessageType::Error);
                } else {
                    UI::showMessage("File shared successfully", MessageType::Success);
                }
                break;
            }
            case 6: {
                UI::showMessage("Generating report...", MessageType::Info);

                // 1. Get all reports allowed to the user

                // 2. Show the list of reports

                break;
            }
            case 7: {
                isLoggedIn = false;
                UI::showMessage("Logging out...", MessageType::Success);
                break;
            }
            default: {
                UI::showMessage("Invalid option", MessageType::Error);
            }
        }

        if (!isLoggedIn) {
            break;
        }
    }

    UI::showExitMessage();
}

int main() {
    start();
    return 0;
}