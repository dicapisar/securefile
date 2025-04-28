//
// Created by Diego Camilo Pinto Sarmiento on 18/3/2025.
//

#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <iostream>
#include <vector>
using namespace std;

class EncryptService {
  private:
    void deriveKeyAndIV(const string &password, unsigned char *key, unsigned char *iv);
  public:
    EncryptService();
    ~EncryptService();

    vector<unsigned char> encryptAES(const vector<unsigned char> &data, const string &password);
    vector<unsigned char> decryptAES(const vector<unsigned char> &encryptedData, const string &password);



};



#endif //ENCRYPT_H
