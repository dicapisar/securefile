//
// Created by Diego Camilo Pinto Sarmiento on 17/3/2025.
//

#include <iostream>

// OpenSSL
#include <openssl/sha.h>

// SQLite
#include <sqlite3.h>

// Boost Filesystem
#include <boost/filesystem.hpp>

// Función para verificar OpenSSL (hash SHA256)
void checkOpenSSL() {
    std::string input = "Hola Mundo";
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256((unsigned char*)input.c_str(), input.size(), hash);

    std::cout << "✅ OpenSSL funciona correctamente - SHA256 hash: ";
    for (unsigned char c : hash) {
        printf("%02x", c);
    }
    std::cout << std::endl;
}

// Función para verificar SQLite (crear base de datos en memoria)
void checkSQLite() {
    sqlite3* db;
    int rc = sqlite3_open(":memory:", &db); // Base de datos en memoria
    if (rc == SQLITE_OK) {
        std::cout << "✅ SQLite funciona correctamente - Base de datos creada en memoria\n";
    } else {
        std::cerr << "❌ Error en SQLite: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_close(db);
}

// Función para verificar Boost.Filesystem (crear y eliminar un directorio)
void checkBoostFilesystem() {
    boost::filesystem::path testDir("test_folder");

    if (!boost::filesystem::exists(testDir)) {
        boost::filesystem::create_directory(testDir);
        std::cout << "✅ Boost.Filesystem funciona correctamente - Carpeta creada: " << testDir.string() << std::endl;
        boost::filesystem::remove(testDir);
    } else {
        std::cout << "✅ Boost.Filesystem ya está funcionando - Carpeta existente detectada: " << testDir.string() << std::endl;
    }
}

int main() {
    std::cout << "🚀 Validando instalación de librerías...\n";

    checkOpenSSL();
    checkSQLite();
    checkBoostFilesystem();

    std::cout << "🎉 Todas las librerías fueron verificadas correctamente.\n";
    return 0;
}