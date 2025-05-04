

#include "encrypt.h"

#include <iostream>
#include <vector>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdexcept>

using namespace std;

#define AES_BLOCK_SIZE 16

EncryptService::EncryptService() {
  cout << "✅ Encrypt service started correctly 🚀\n";
}

EncryptService::~EncryptService() {
  cout << "✅ Encrypt service closed correctly\n";
}

vector<unsigned char> EncryptService::encryptAES(const vector<unsigned char> &data, const string &password) {
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

vector<unsigned char> EncryptService::decryptAES(const vector<unsigned char> &encryptedData, const string &password) {
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

void EncryptService::deriveKeyAndIV(const string &password, unsigned char *key, unsigned char *iv) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256(reinterpret_cast<const unsigned char *>(password.c_str()), password.size(), hash);
  memcpy(key, hash, 32);
  memcpy(iv, hash + 16, 16);
}

string EncryptService::hashPassword(const string &password) {
  unsigned char salt[PASSWORD_SALT_LEN];
  if (RAND_bytes(salt, sizeof(salt)) != 1) {
    throw runtime_error("Failed to generate salt");
  }

  unsigned char hash[PASSWORD_HASH_LEN];
  if (PKCS5_PBKDF2_HMAC(
          password.c_str(), password.size(),
          salt, sizeof(salt),
          PBKDF2_ITERS,
          EVP_sha256(),
          sizeof(hash),
          hash) != 1) {
    throw runtime_error("PBKDF2 hashing failed");
          }

  ostringstream oss;
  oss << PBKDF2_ITERS << '$';
  oss << hex << setfill('0');
  for (int i = 0; i < (int)sizeof(salt); ++i)
    oss << setw(2) << (int)salt[i];
  oss << '$';
  for (int i = 0; i < (int)sizeof(hash); ++i)
    oss << setw(2) << (int)hash[i];

  return oss.str();
}

bool EncryptService::verifyPassword(const string &password, const string &hashedPassword) {
  istringstream iss(hashedPassword);
  string iterStr, saltHex, hashHex;
  if (!getline(iss, iterStr, '$') ||
      !getline(iss, saltHex, '$') ||
      !getline(iss, hashHex)) {
    return false;
      }

  int iters = stoi(iterStr);
  if ((int)saltHex.size() != PASSWORD_SALT_LEN * 2 ||
      (int)hashHex.size() != PASSWORD_HASH_LEN * 2) {
    return false;
      }

  // 2) Convertir hex a bytes
  vector<unsigned char> salt(PASSWORD_SALT_LEN), hashStored(PASSWORD_HASH_LEN);
  for (int i = 0; i < PASSWORD_SALT_LEN; ++i)
    salt[i] = stoi(saltHex.substr(i*2,2), nullptr, 16);
  for (int i = 0; i < PASSWORD_HASH_LEN; ++i)
    hashStored[i] = stoi(hashHex.substr(i*2,2), nullptr, 16);

  // 3) Recalcular PBKDF2 con la misma salt e iteraciones
  vector<unsigned char> hashCalc(PASSWORD_HASH_LEN);
  if (PKCS5_PBKDF2_HMAC(
          password.c_str(), password.size(),
          salt.data(), salt.size(),
          iters,
          EVP_sha256(),
          hashCalc.size(),
          hashCalc.data()) != 1) {
    return false;
          }

  // 4) Comparar en tiempo constante
  return CRYPTO_memcmp(hashStored.data(), hashCalc.data(), PASSWORD_HASH_LEN) == 0;
}
