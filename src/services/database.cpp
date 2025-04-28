//
// Created by Diego Camilo Pinto Sarmiento on 18/3/2025.
//

#include "database.h"

#include <vector>
#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include "models/user.h"
#include "models/encrypted_file.h"
#include "models/shared_file.h"
#include "models/metadata_file.h"


using namespace std;

map<Models, map<string, string>> modelColumnTypes = {
    {UserModel, {
                {"student_id", "TEXT"},
                {"name", "TEXT"},
                {"email", "TEXT"},
                {"password", "TEXT"},
                {"age", "INT"},
                {"is_admin", "INT"},
                {"created_at", "DATETIME"}
    }},
    {EncryptedFileModel, {
                {"file_name", "TEXT"},
                {"file_path", "TEXT"},
                {"last_modified", "DATETIME"},
                {"password", "TEXT"},
                {"owner_id", "INT"}
    }},
    {SharedFileModel, {
                {"encrypted_file_id", "INT"},
                {"shared_user_id", "INT"},
                {"date", "DATETIME"}
    }},
    {MetadataFileModel, {
                {"encrypted_file_id", "INT"},
                {"last_update", "DATETIME"}
    }}
};


DatabaseService::DatabaseService() {
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        cerr << "❌ Error starting SQLite: " << sqlite3_errmsg(db) << endl;
        db = nullptr;
    } else {
        cout << "✅ Database started on memory correctly 🚀\n";
    }

    ifstream file_backup("data_backup.sql");
    is_there_backup = file_backup ? true : false;
}

DatabaseService::~DatabaseService() {
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

bool DatabaseService::load_SQL_file() {
    string const filename = "data.sql";
    string const filename_backup = "data_backup.sql";

    if (is_there_backup) {
        return loadDatabaseFromBackup(filename_backup);
    }

    return loadDatabaseFromFile(filename);
}

vector<User> DatabaseService::getUsers() {  // TODO: DELETE
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

optional<vector<EncryptedFile>> DatabaseService::getEncryptedFilesByOwnerID(int owner_iD) {
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

optional<vector<EncryptedFile>> DatabaseService::getSharedEncryptedFilesByUserID(int user_id) {
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

    optional result = vector<EncryptedFile>(); ;

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

optional<variant<User, EncryptedFile, SharedFile, MetadataFile>> DatabaseService::getModelByID(Models model_name, int model_id) {
    string table_name = getTableNameFromModelName(model_name);

    string sql = "SELECT * FROM " + table_name + " WHERE id = " + to_string(model_id);

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "❌ Error making consult: " << sqlite3_errmsg(db) << endl;
        return nullopt;
    }

    sqlite3_bind_int(stmt, 1, model_id);

    optional<variant<User, EncryptedFile, SharedFile, MetadataFile>> result;

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        switch (model_name) {
            case UserModel: {
                const int u_id = sqlite3_column_int(stmt, 0);
                const string& u_student_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                const string& u_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                const string& u_email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                const string& u_password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                const bool u_is_admin = sqlite3_column_int(stmt, 5) == 1;
                const string u_created_at = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

                result = User(u_id, u_student_id, u_name, u_email, u_password, u_is_admin, u_created_at);
                break;
            }
            case EncryptedFileModel: {
                const int ef_id = sqlite3_column_int(stmt, 0);
                const int ef_owner_id = sqlite3_column_int(stmt, 1);
                const string& ef_file_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                const string& ef_file_path = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                const string& ef_last_modified = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                const string& ef_password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

                optional<variant<User, EncryptedFile, SharedFile, MetadataFile>> ef_owner = getModelByID(UserModel, ef_owner_id);

                auto user = std::get_if<User>(&(*ef_owner));

                result = EncryptedFile(ef_id, ef_file_name, ef_file_path, ef_last_modified, ef_password, *user);
                break;
            }
            case SharedFileModel: {
                const int sf_id = sqlite3_column_int(stmt, 0);
                const int sf_encrypted_file_id = sqlite3_column_int(stmt, 1);
                const int sf_shared_user_id = sqlite3_column_int(stmt, 2);
                string date = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

                optional<variant<User, EncryptedFile, SharedFile, MetadataFile>> sf_encrypted_file_opt = getModelByID(
                    EncryptedFileModel, sf_encrypted_file_id);

                EncryptedFile sf_encrypted_file = get<EncryptedFile>(*sf_encrypted_file_opt);

                optional<variant<User, EncryptedFile, SharedFile, MetadataFile>> sf_shared_user_opt = getModelByID(
                    UserModel, sf_shared_user_id);

                User sf_shared_user = get<User>(*sf_shared_user_opt);

                result = SharedFile(sf_id, sf_encrypted_file, sf_shared_user, date);
                break;
            }
            case MetadataFileModel: {
                const int mf_id = sqlite3_column_int(stmt, 0);
                const int mf_encrypted_file_id = sqlite3_column_int(stmt, 1);
                string mf_last_update = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));

                optional<variant<User, EncryptedFile, SharedFile, MetadataFile>> mf_encrypted_file_opt = getModelByID(
                EncryptedFileModel, mf_encrypted_file_id);

                EncryptedFile mf_encrypted_file = get<EncryptedFile>(*mf_encrypted_file_opt);

                result = MetadataFile(mf_id, mf_encrypted_file, mf_last_update);
                break;
            }
            default:
                result = nullopt;

        }
    }

    sqlite3_finalize(stmt);

    return result;

}

bool DatabaseService::alterAttributeFromModelByID(Models model_name, int id, const map<string, string>& attributes) {
    string table_name = getTableNameFromModelName(model_name);

    ostringstream sql_query;
    sql_query << "UPDATE " << table_name << " SET ";
    bool first = true;
    for (const auto& [column, value] : attributes) {
        if (!first) {
            sql_query << ", ";
        }
        sql_query << column << " = ?";
        first = false;
    }
    sql_query << " WHERE id = ?;";
    string sql = sql_query.str();

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "❌ Error making consult: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    int bind_index = 1;

    auto colTypeIt = modelColumnTypes.find(model_name);
    if (colTypeIt == modelColumnTypes.end()) {
        cerr << "❌ Not was possible to find the map of colums for the model." << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    const auto& colTypes = colTypeIt->second;

    for (const auto& [column, value] : attributes) {
        auto typeIt = colTypes.find(column);
        if (typeIt == colTypes.end()) {
            cerr << "❌ Colum " << column << " didn't find on the model." << endl;
            sqlite3_finalize(stmt);
            return false;
        }
        string type = typeIt->second;

        if (type == "INT") {
            try {
                int int_val = stoi(value);
                sqlite3_bind_int(stmt, bind_index, int_val);
            } catch (const invalid_argument& e) {
                cerr << "❌ Error parsing value " << column << " to int." << endl;
                sqlite3_finalize(stmt);
                return false;
            }
        } else {
            sqlite3_bind_text(stmt, bind_index, value.c_str(), -1, SQLITE_TRANSIENT);
        }
        bind_index++;
    }

    sqlite3_bind_int(stmt, bind_index, id);

    int step_res = sqlite3_step(stmt);
    if (step_res != SQLITE_DONE) {
        cerr << "❌ Error running the query: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool DatabaseService::deleteRecordByID(Models model_name, int id) {
    string table_name = getTableNameFromModelName(model_name);

    ostringstream sql_query;
    sql_query << "DELETE FROM " << table_name << " WHERE id = ?;";
    string sql = sql_query.str();

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "❌ Error making consult: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    int step_result = sqlite3_step(stmt);
    if (step_result != SQLITE_DONE) {
        cerr << "❌ Error deleting record: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

string DatabaseService:: getTableNameFromModelName(Models model_name) {
    map<Models, string> table_name_map = {
        {UserModel, "users"},
        {EncryptedFileModel, "encrypted_files"},
        {SharedFileModel, "shared_files"},
        {MetadataFileModel, "metadata_files"}
    };

    auto it = table_name_map.find(model_name);
    return (it != table_name_map.end()) ? it->second : "";
}



















bool DatabaseService::loadDatabaseFromFile(const string &file_name) {
    ifstream file(file_name);

    if (!file) {
        cerr << "❌ Error opening sql file " << file_name << endl;
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

    cout << "✅ Data loaded successfully from: " << file_name << endl;
    return true;
}

bool DatabaseService::loadDatabaseFromBackup(const string &file_name) {
    sqlite3* fileDB;
    if (sqlite3_open(file_name.c_str(), &fileDB) != SQLITE_OK) {
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

    cout << "✅ Database restored from: " << file_name << endl;
    return true;
}

