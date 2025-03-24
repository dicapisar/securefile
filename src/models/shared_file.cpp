//
// Created by Diego Camilo Pinto Sarmiento on 20/3/2025.
//

#include "shared_file.h"

SharedFile::SharedFile(): id(0), encrypted_file(EncryptedFile()), shared_user(User()), date("") {}

SharedFile::SharedFile(int id, EncryptedFile encrypted_file, User shared_user, string &date)
    : id(id), encrypted_file(encrypted_file), shared_user(shared_user), date(date) {}

SharedFile::~SharedFile() = default;