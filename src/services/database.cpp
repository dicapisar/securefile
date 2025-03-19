//
// Created by Diego Camilo Pinto Sarmiento on 18/3/2025.
//

#include "database.h"

#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <models/encrypted_file.h>

using namespace std;


Database::Database() {
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        cerr << "❌ Error starting SQLite: " << sqlite3_errmsg(db) << endl;
        db = nullptr;
    } else {
        cout << "✅ Database started on memory correctly 🚀\n";
    }

    ifstream file_backup("data_backup.sql");
    is_there_backup = file_backup ? true : false;
}

Database::~Database() {
    if (db) {
        string filename_backup = "data_backup.sql";

        sqlite3* fileDB;
        if (sqlite3_open(filename_backup.c_str(), &fileDB) != SQLITE_OK) {
            cerr << "❌ It wasn't possible open backup file for saving.\n";
        }

        sqlite3_backup* backup = sqlite3_backup_init(fileDB, "main", db, "main");
        if (!backup) {
            cerr << "❌ Error initializing backup: " << sqlite3_errmsg(fileDB) << endl;
            sqlite3_close(fileDB);
        }

        sqlite3_backup_step(backup, -1);
        sqlite3_backup_finish(backup);
        sqlite3_close(fileDB);

        cout << "✅ Database saved in: " << filename_backup << endl;
        cout << "🔒 Database closed successfully since destructor method.\n";
    }
}

bool Database::load_SQL_file() {
    string const filename = "data.sql";
    string const filename_backup = "data_backup.sql";

    if (is_there_backup) {
        return loadDatabaseFromBackup(filename_backup);
    }

    return loadDatabaseFromFile(filename);
}

vector<User> Database::getUsers() {  // TODO: DELETE
    vector<User> users;
    string sql = "SELECT * FROM users;";
    sqlite3_stmt* stmt;


    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "❌ Error making consult: " << sqlite3_errmsg(db) << endl;
        return users;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        string student_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        string email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        bool isAdmin = sqlite3_column_int(stmt, 5) == 1;
        string created_at = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

        users.emplace_back(id, student_id, name, email, password, isAdmin, created_at);
    }

    sqlite3_finalize(stmt);
    return users;
}


optional<vector<EncryptedFile>> Database::getEncryptedFilesByOwnerID(int owner_iD) {
    string sql = R"(
        SELECT e.id, e.owner_id, e.file_name, e.file_path, e.last_modified, e.password, u.id, u.student_id, u.name, u.email, u.password, u.is_admin, u.created_at
        FROM encrypted_files e
        JOIN users u ON e.owner_id = u.id
        WHERE e.owner_id = ?;
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "❌ Error making consult: " << sqlite3_errmsg(db) << endl;
        return nullopt;
    }

    sqlite3_bind_int(stmt, 1, owner_iD);

    optional<vector<EncryptedFile>> result = vector<EncryptedFile>(); ;

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        int e_id = sqlite3_column_int(stmt, 0);
        string e_file_name  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        string e_file_path  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        string e_last_modified = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        string e_password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

        int u_id = sqlite3_column_int(stmt, 6);
        string u_student_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        string u_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
        string u_email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
        string u_password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
        bool u_is_admin = sqlite3_column_int(stmt, 11) == 1;
        string u_created_at = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));

        auto owner = User(u_id, u_student_id, u_name, u_email, u_password, u_is_admin, u_created_at);

        result->emplace_back(e_id, e_file_name, e_file_path, e_last_modified, e_password, owner);
    }

    sqlite3_finalize(stmt);

    if (result->empty()) return nullopt;

    return result;

}

optional<vector<EncryptedFile>> Database::getSharedEncryptedFilesByUserID(int user_id) {
    string sql = R"(
        SELECT e.id, e.owner_id, e.file_name, e.file_path, e.last_modified, e.password, u.id, u.student_id, u.name, u.email, u.password, u.is_admin, u.created_at
        FROM shared_files
        JOIN encrypted_files E on E.id = shared_files.encrypted_file_id
        JOIN users u on shared_files.shared_user_id = u.id
        WHERE shared_user_id = ?;
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "❌ Error making consult: " << sqlite3_errmsg(db) << endl;
        return nullopt;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    optional<vector<EncryptedFile>> result = vector<EncryptedFile>(); ;

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        int e_id = sqlite3_column_int(stmt, 0);
        string e_file_name  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        string e_file_path  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        string e_last_modified = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        string e_password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

        int u_id = sqlite3_column_int(stmt, 6);
        string u_student_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        string u_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
        string u_email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
        string u_password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
        bool u_is_admin = sqlite3_column_int(stmt, 11) == 1;
        string u_created_at = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));

        auto owner = User(u_id, u_student_id, u_name, u_email, u_password, u_is_admin, u_created_at);

        result->emplace_back(e_id, e_file_name, e_file_path, e_last_modified, e_password, owner);
    }

    sqlite3_finalize(stmt);

    if (result->empty()) return nullopt;

    return result;

}












bool Database::loadDatabaseFromFile(const string &filename) {
    ifstream file(filename);

    if (!file) {
        cerr << "❌ Error opening sql file " << filename << endl;
        return false;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string sql = buffer.str();

    char* errorMsg;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMsg);
    if (rc != SQLITE_OK) {
        cerr << "❌ Error charging data: " << errorMsg << endl;
        sqlite3_free(errorMsg);
        return false;
    }

    cout << "✅ Data loaded successfully from: " << filename << endl;
    return true;
}

bool Database::loadDatabaseFromBackup(const string &filename) {
    sqlite3* fileDB;
    if (sqlite3_open(filename.c_str(), &fileDB) != SQLITE_OK) {
        cerr << "❌ Error opening sql file for restoration of Database.\n";
        return false;
    }

    sqlite3_backup* backup = sqlite3_backup_init(db, "main", fileDB, "main");
    if (!backup) {
        cerr << "❌ Error initializing restoration: " << sqlite3_errmsg(fileDB) << endl;
        sqlite3_close(fileDB);
        return false;
    }

    sqlite3_backup_step(backup, -1);
    sqlite3_backup_finish(backup);
    sqlite3_close(fileDB);

    cout << "✅ Database restored from: " << filename << endl;
    return true;
}

