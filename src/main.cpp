//
// Created by Diego Camilo Pinto Sarmiento on 17/3/2025.
//

#include <iostream>
using namespace std;

#include "services/database.h"
#include  "services/file.h"
#include  "services/encrypt.h"

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

    /*
     * Encrypt
     */

    const string file_to_encrypt = "test.txt";
    const string password = "password";

    File file_service;
    Encrypt encrypt_service;

    vector<unsigned char> file = file_service.readFile(file_to_encrypt);

    if (file.empty()) {
        cerr << "❌ Error reading file. \n" << endl;
    }

    vector<unsigned char> encrypted_file = encrypt_service.encryptAES(file, password);

    file_service.createDirectory("./encrypted_files/");

    const string output_file_name = "./encrypted_files/" + file_to_encrypt + ".enc";

    file_service.writeFile(output_file_name, encrypted_file);

    cout << "📁 File Encrypted Successfully at: " << output_file_name << endl;

    /*
     * Decrypt
     */

    string path_encrypted_file = "./encrypted_files/" + file_to_encrypt + ".enc";
    vector<unsigned char> encrypted_file_to_decrypt = file_service.readFile(path_encrypted_file);

    vector<unsigned char> decrypted_file = encrypt_service.decryptAES(encrypted_file_to_decrypt, password);

    file_service.createDirectory("./decrypted_files/");

    const string output_decrypted_file = "./decrypted_files/" + file_to_encrypt;

    file_service.writeFile(output_decrypted_file, decrypted_file);

    cout << "📂 File Decrypted Successfully at: " << output_file_name << endl;

    cout << "🎉 All Libraries Working Successfully.\n";
    return 0;
}