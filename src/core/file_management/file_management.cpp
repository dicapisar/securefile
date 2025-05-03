#include "file_management.h"

#include "./services/encrypt.h"
#include "./services/file.h"
#include "./services/database.h"
#include "./models/session.h"

#include <string>
using namespace std;

FileManagement::FileManagement(const DatabaseService& database, const EncryptService& encrypt, const FileService& file)
    : database(database), encrypt(encrypt), file(file) {}

FileManagement::~FileManagement() = default;


bool FileManagement::encryptFile(const Session& session, const string& file_name, const string& password) {
    return true;
}

bool FileManagement::decryptFile(const Session& session, const string& file_name, const string& password) {
    return true;
}

bool FileManagement::deleteFile(const Session& session, const string& file_name) {
    return true;
}

optional<vector<EncryptedFile>> FileManagement::getListFiles(const Session& session) {
    return nullopt;
}

bool FileManagement::shareFile(const Session& session, const string& file_name, const string& student_id) {
    return true;
}