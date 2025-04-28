//
// Created by Diego Camilo Pinto Sarmiento on 20/3/2025.
//

#ifndef SHARED_FILE_H
#define SHARED_FILE_H

#include <iostream>
#include <string>

#include "encrypted_file.h"

using namespace std;

class SharedFile {
public:
    int id;
    EncryptedFile encrypted_file;
    User shared_user;
    string date;

    SharedFile();
    ~SharedFile();

    SharedFile(int id, EncryptedFile encrypted_file, User shared_user, string &date);

};



#endif //SHARED_FILE_H
