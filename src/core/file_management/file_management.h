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
        static constexpr string PATH_ENCRYPT_FILE = "./encrypted_files/";

    public:
        bool encryptFile(const Session& session, const string& file_name, const string& password);
        bool decryptFile(const Session& session, int fileID, const string& password);
        bool deleteFile(const Session& session, const string& file_name);
        optional<vector<EncryptedFile>> getListEncryptedFiles(const Session& session);
        bool shareFile(const Session& session, const string& file_name, const string& student_id);

        FileManagement(DatabaseService* databaseService, EncryptService* encryptService, FileService* fileService);
        ~FileManagement();
};



#endif //FILE_MANAGEMENT_H
