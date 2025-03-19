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
    db.load_SQL_file();
    vector<User> users = db.getUsers();
    for (const auto& user : users) {
        cout << "ID: " << user.id << " | Nombre: " << user.name
                  << " | Email: " << user.email << " | Password: " << user.password
                  << " | Admin: " << (user.is_admin ? "Yes" : "No")
                  << " | Crated at: " << user.created_at << std::endl;
    }

    auto encryptedFileOpt = db.getEncryptedFilesByOwnerID(1);
    if (encryptedFileOpt.has_value()) {
        vector<EncryptedFile> encryptedFiles = encryptedFileOpt.value();
        for (const auto& encryptedFile : encryptedFiles) {
            cout << "\n📂 Encrypted File Found:\n";
            cout << "ID: " << encryptedFile.id << " | Name: " << encryptedFile.file_name
                 << " | Path: " << encryptedFile.file_path << endl;
            cout << "👤 Owner: " << encryptedFile.owner.name << " | Owner Email: " << encryptedFile.owner.email << endl;
        }
    }

    auto sharedEncryptedFileOpt = db.getSharedEncryptedFilesByUserID(2);
    if (sharedEncryptedFileOpt.has_value()) {
        vector<EncryptedFile> encryptedFiles = sharedEncryptedFileOpt.value();
        for (const auto& encryptedFile : encryptedFiles) {
            cout << "\n📂 Shared Encrypted File Found:\n";
            cout << "ID: " << encryptedFile.id << " | Name: " << encryptedFile.file_name
                 << " | Path: " << encryptedFile.file_path << endl;
            cout << "👤 Owner: " << encryptedFile.owner.name << " | Owner Email: " << encryptedFile.owner.email << endl;
        }
    }

    Encrypt encrypt;
    encrypt.check();

    File file;
    file.check();

    cout << "🎉 All Libraries Working Successfully.\n";
    return 0;
}