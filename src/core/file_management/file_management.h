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
        bool encryptFile(Session& session, const string& file_name, const string& password);
        bool decryptFile(Session& session, const string& file_name, const string& password);
        bool deleteFile(Session& session, const string& file_name);
        bool listFiles(Session& session);
        bool shareFile(Session& session, const string& file_name, const string& student_id);

        FileManagement(const DatabaseService& database, const EncryptService& encrypt, const FileService& file);
        ~FileManagement();
};



#endif //FILE_MANAGEMENT_H
