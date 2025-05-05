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
        DatabaseService *databaseService;
        EncryptService *encryptService;
        FileService *fileService;
        static const string PATH_ENCRYPT_FILE;

    public:
        bool encryptFile(const Session& session, const string& file_name, const string& password);
        bool decryptFile(const Session& session, int fileID, const string& password);
        bool deleteFile(const Session& session, const string& file_name);
        optional<vector<EncryptedFile>> getListEncryptedFiles(const Session& session);
        bool shareFile(const Session& session, const string& file_name, const string& student_id, const string& password);

        FileManagement(DatabaseService* databaseService, EncryptService* encryptService, FileService* fileService);
        ~FileManagement();
};



#endif //FILE_MANAGEMENT_H
