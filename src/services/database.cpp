//
// Created by Diego Camilo Pinto Sarmiento on 18/3/2025.
//

#include "database.h"

#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


Database::Database() {
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        cerr << "❌ Error starting SQLite: " << sqlite3_errmsg(db) << endl;
        db = nullptr;
    } else {
        cout << "✅ Database started on memory correctly 🚀\n";
    }
}

Database::~Database() {
    if (db) {
        sqlite3_close(db);
        cout << "🔒 Database closed successfully since destructor method.\n";
    }
}

bool Database::load_SQL_From_File(const string& filename) {
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

