//
// Created by Diego Camilo Pinto Sarmiento on 18/3/2025.
//

#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <iostream>
using namespace std;


class Database {
    private:
      sqlite3* db;
    public:
      Database();
      ~Database();

      bool load_SQL_From_File(const string& filename);
};



#endif //DATABASE_H
