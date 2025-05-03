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


bool FileManagement::encryptFile(Session& session, const string& file_name, const string& password) {
    return true;
}

bool FileManagement::decryptFile(Session& session, const string& file_name, const string& password) {
    return true;
}

bool FileManagement::deleteFile(Session& session, const string& file_name) {
    return true;
}

bool FileManagement::listFiles(Session& session) {
    return true;
}

bool FileManagement::shareFile(Session& session, const string& file_name, const string& student_id) {
    return true;
}