

#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <map>
#include <variant>
#include <models/metadata_file.h>
#include <models/shared_file.h>
#include <vector>
#include <string>
#include <optional>
#include <models/report.h>

#include "models/user.h"
#include "models/encrypted_file.h"

using namespace std;

enum Models {
    UserModel,
    EncryptedFileModel,
    SharedFileModel,
    MetadataFileModel,
    ReportModel
};


class DatabaseService {
    private:
      sqlite3* db;
      bool is_there_backup;
      bool loadDatabaseFromFile(const string &file_name);
      bool loadDatabaseFromBackup(const string &file_name);
      string getTableNameFromModelName(Models model_name);
    public:
      DatabaseService();
      ~DatabaseService();

      bool load_SQL_file();
      vector<User> getUsers();
      optional<vector<EncryptedFile>> getEncryptedFilesByOwnerID(int owner_iD);
      optional<vector<EncryptedFile>> getSharedEncryptedFilesByUserID(int user_id);
      optional<vector<Report>> getReportsByOwnerID(int owner_ID);
      optional<variant<User, EncryptedFile, SharedFile, MetadataFile, Report>> getModelByID(Models model_name, int model_id);
      bool alterAttributeFromModelByID(Models model_name, int id, const map<string, string> &attributes);
      bool deleteRecordByID(Models model_name, int id);
      bool saveEncryptedFile(const EncryptedFile &encryptedFile);
};



#endif //DATABASE_H
