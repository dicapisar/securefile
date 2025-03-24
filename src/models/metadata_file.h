//
// Created by Diego Camilo Pinto Sarmiento on 20/3/2025.
//

#ifndef METADATA_FILE_H
#define METADATA_FILE_H

#include <iostream>

#include "encrypted_file.h"

using namespace std;

class MetadataFile {
public:
    int id;
    EncryptedFile encrypted_file;
    string last_update;

    MetadataFile();
    ~MetadataFile();

    MetadataFile(int id, EncryptedFile encrypted_file, string& last_update);
};



#endif //METADATA_FILE_H
