//
// Created by Diego Camilo Pinto Sarmiento on 17/3/2025.
//

#include <iostream>
using namespace std;

#include "services/database.h"
#include  "services/encrypt.h"
#include "services/file.h"


int main() {
    cout << "👀 Validating installation of Libraries...\n";

    Database db;
    db.load_SQL_From_File("data.sql");

    Encrypt encrypt;
    encrypt.check();

    File file;
    file.check();

    cout << "🎉 All Libraries Working Successfully.\n";
    return 0;
}