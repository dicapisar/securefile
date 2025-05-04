

#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class EncryptService {
  private:
    void deriveKeyAndIV(const string &password, unsigned char *key, unsigned char *iv);
    static constexpr int PASSWORD_SALT_LEN = 16;
    static constexpr int PASSWORD_HASH_LEN = 32;
    static constexpr int PBKDF2_ITERS = 100000;
  public:
    EncryptService();
    ~EncryptService();

    vector<unsigned char> encryptAES(const vector<unsigned char> &data, const string &password);
    vector<unsigned char> decryptAES(const vector<unsigned char> &encryptedData, const string &password);

    string hashPassword(const string &password);
    bool verifyPassword(const string &password, const string &hashedPassword);



};



#endif //ENCRYPT_H
