//
// Created by Diego Camilo Pinto Sarmiento on 20/3/2025.
//

#include "metadata_file.h"
#include <string>

MetadataFile::MetadataFile() : id(0), encrypted_file(EncryptedFile()), last_update("") {}

MetadataFile::MetadataFile(int id, EncryptedFile encrypted_file, string &last_update)
    : id(id), encrypted_file(encrypted_file), last_update(last_update) {}

MetadataFile::~MetadataFile() = default;
