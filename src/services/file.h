//
// Created by Diego Camilo Pinto Sarmiento on 18/3/2025.
//

#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <vector>

using namespace std;


class FileService {
  public:
    FileService();
    ~FileService();

    vector<unsigned char> readFile(const string &file_name);
    void writeFile(const string &file_name, const vector<unsigned char> &data);
    void createDirectory(const string &file_path);
    string removeExtension(const string &file_name, const string &extension);
};



#endif //FILE_H
