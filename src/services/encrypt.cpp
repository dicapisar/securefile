//
// Created by Diego Camilo Pinto Sarmiento on 18/3/2025.
//

#include "encrypt.h"

#include <iostream>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace std;

#define AES_BLOCK_SIZE 16

Encrypt::Encrypt() {
  cout << "✅ Encrypt service started correctly 🚀\n";
}

Encrypt::~Encrypt() {
  cout << "✅ Encrypt service closed correctly\n";
}

vector<unsigned char> Encrypt::encryptAES(const vector<unsigned char> &data, const string &password) {
  unsigned char key[32], iv[16];
  deriveKeyAndIV(password, key, iv);

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);

  vector<unsigned char> encrypted(data.size() + AES_BLOCK_SIZE);
  int len = 0, encrypted_len = 0;

  EVP_EncryptUpdate(ctx, encrypted.data(), &len, data.data(), data.size());
  encrypted_len += len;

  EVP_EncryptFinal_ex(ctx, encrypted.data() + encrypted_len, &len);
  encrypted_len += len;
  encrypted.resize(encrypted_len);

  EVP_CIPHER_CTX_free(ctx);
  return encrypted;
}

vector<unsigned char> Encrypt::decryptAES(const vector<unsigned char> &encryptedData, const string &password) {
  unsigned char key[32], iv[16];
  deriveKeyAndIV(password, key, iv);

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);

  std::vector<unsigned char> decrypted(encryptedData.size());
  int len = 0, decrypted_len = 0;

  EVP_DecryptUpdate(ctx, decrypted.data(), &len, encryptedData.data(), encryptedData.size());
  decrypted_len += len;

  EVP_DecryptFinal_ex(ctx, decrypted.data() + decrypted_len, &len);
  decrypted_len += len;
  decrypted.resize(decrypted_len);

  EVP_CIPHER_CTX_free(ctx);
  return decrypted;
}



void Encrypt::deriveKeyAndIV(const string &password, unsigned char *key, unsigned char *iv) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256(reinterpret_cast<const unsigned char *>(password.c_str()), password.size(), hash);
  memcpy(key, hash, 32);
  memcpy(iv, hash + 16, 16);
}
