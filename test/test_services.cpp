// FILE: tests/test_services.cpp

#include <cassert>
#include <iostream>
#include <../src/services/encrypt.h>
#include <../src/services/file.h>
#include <../src/services/database.h>


int main() {
    std::cout << "🔧 Inicializing tests...\n\n";

    // 1) EncryptService: roundtrip AES
    {
        EncryptService enc;
        std::vector<unsigned char> data = {'H','o','l','a','!','\n'};
        std::string password = "P4$$w0rd!";
        auto encrypted = enc.encryptAES(data, password);
        auto decrypted = enc.decryptAES(encrypted, password);
        assert(decrypted == data && "Encrypt/Decrypt AES roundtrip falló");
        std::cout << "✅ EncryptService AES roundtrip OK\n";
    }

    // 2) FileService: removeExtension
    {
        FileService fs;
        std::string f1 = fs.removeExtension("documento.txt.enc", ".enc");
        std::string f2 = fs.removeExtension("archivo.enc", ".enc");
        assert(f1 == "documento.txt" && "removeExtension faii with 'documento.txt.enc'");
        assert(f2 == "archivo"        && "removeExtension fail with 'archivo.enc'");
        std::cout << "✅ FileService removeExtension OK\n";
    }

    // 3) DatabaseService: carga SQL y getUsers
    {
        DatabaseService dbs;
        bool loaded = dbs.load_SQL_file();
        assert(loaded && "load_SQL_file failed");
        auto users = dbs.getUsers();
        assert(users.size() >= 2 && "getUsers must return at least 2 users");
        std::cout << "✅ DatabaseService load_SQL_file & getUsers OK\n";
    }

    std::cout << "\n🎉 All test passed successfully !\n";
    return 0;
}
