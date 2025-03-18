//
// Created by Diego Camilo Pinto Sarmiento on 18/3/2025.
//

#include "file.h"
#include <iostream>
#include <boost/filesystem.hpp>

using namespace std;

File::File() {
    cout << "✅ File service started correctly 🚀\n";
}

File::~File() {
    cout << "✅ File service closed correctly\n";
}

void File::check() {
    boost::filesystem::path testDir("test_folder");

    if (!boost::filesystem::exists(testDir)) {
        boost::filesystem::create_directory(testDir);
        cout << "✅File Service: Boost.Filesystem funciona correctamente - Carpeta creada: " << testDir.string() << endl;
        boost::filesystem::remove(testDir);
    } else {
        cout << "✅File Service: Boost.Filesystem ya está funcionando - Carpeta existente detectada: " << testDir.string() << endl;
    }
}