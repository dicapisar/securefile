#ifndef FILE_MANAGEMENT_H
#define FILE_MANAGEMENT_H

#include "./services/encrypt.h"
#include "./services/file.h"
#include "./services/database.h"
#include "./models/session.h"

#include <string>
using namespace std;


class FileManagement {
    private:
        DatabaseService database;
        EncryptService encrypt;
        FileService file;

    public:
        bool encryptFile(const Session& session, const string& file_name, const string& password);
        bool decryptFile(const Session& session, const string& file_name, const string& password);
        bool deleteFile(const Session& session, const string& file_name);
        optional<vector<EncryptedFile>> getListFiles(const Session& session);
        bool shareFile(const Session& session, const string& file_name, const string& student_id);

        FileManagement(const DatabaseService& database, const EncryptService& encrypt, const FileService& file);
        ~FileManagement();
};



#endif //FILE_MANAGEMENT_H
