//
// Created by Diego Camilo Pinto Sarmiento on 20/3/2025.
//

#include "encrypted_file.h"


EncryptedFile::EncryptedFile() : id(0), file_name(""), file_path(""), last_modified(0), password("") {}

EncryptedFile::EncryptedFile(int id, string file_name, string file_path, string last_modified, string password, User owner)
    : id(id), file_name(file_name), file_path(file_path), last_modified(last_modified), password(password), owner(owner) {}

EncryptedFile::~EncryptedFile() = default;
