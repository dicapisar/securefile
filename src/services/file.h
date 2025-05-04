

#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;


class FileService {
  public:
    FileService();
    ~FileService();

    vector<unsigned char> readFile(const string &file_name);
    void writeFile(const string &file_name, const vector<unsigned char> &data);
    void createDirectory(const string &file_path);
    string removeExtension(const string &file_name, const string &extension);
    void removeFile(const string &file_name);
};



#endif //FILE_H
