//
// Created by Diego Camilo Pinto Sarmiento on 18/3/2025.
//

#include "encrypt.h"

#include <iostream>
#include <openssl/sha.h>

using namespace std;

Encrypt::Encrypt() {
  cout << "✅ Encrypt service started correctly 🚀\n";
}

Encrypt::~Encrypt() {
  cout << "✅ Encrypt service closed correctly\n";
}

void Encrypt::check() {
  const string input = "Check OpenSSL";
  unsigned char hash[SHA256_DIGEST_LENGTH];

  SHA256((unsigned char*)input.c_str(), input.size(), hash);

  cout << "✅ OpenSSL working correctly - SHA256 hash: ";
  for (unsigned char c : hash) {
    printf("%02x", c);
  }
  cout << endl;
}