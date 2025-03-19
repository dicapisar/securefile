//
// Created by Diego Camilo Pinto Sarmiento on 20/3/2025.
//

#ifndef ENCRYPTED_FILE_H
#define ENCRYPTED_FILE_H

#include <iostream>

#include "user.h"
using namespace std;

class EncryptedFile {
    public:
      int id;
      string file_name;
      string file_path;
      string last_modified;
      string password;
      User owner;

      EncryptedFile();
      ~EncryptedFile();

      EncryptedFile(int id, string file_name, string file_path, string last_modified, string password, User owner);
};



#endif //ENCRYPTED_FILE_H
