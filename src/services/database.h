//
// Created by Diego Camilo Pinto Sarmiento on 18/3/2025.
//

#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <iostream>
#include "models/user.h"
#include "models/encrypted_file.h"

using namespace std;


class DatabaseService {
    private:
      sqlite3* db;
      bool is_there_backup;
      bool loadDatabaseFromFile(const string &file_name);
      bool loadDatabaseFromBackup(const string &file_name);
    public:
      DatabaseService();
      ~DatabaseService();

      bool load_SQL_file();
      vector<User> getUsers();
      optional<vector<EncryptedFile>> getEncryptedFilesByOwnerID(int owner_iD);
      optional<vector<EncryptedFile>> getSharedEncryptedFilesByUserID(int user_id);
};



#endif //DATABASE_H
