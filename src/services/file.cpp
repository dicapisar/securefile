

#include "file.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <string>

using namespace std;
using namespace boost::filesystem;

FileService::FileService() {
    cout << "✅ File service started correctly 🚀\n";
}

FileService::~FileService() {
    cout << "✅ File service closed correctly\n";
}

vector<unsigned char> FileService::readFile(const string &file_name) {
    ifstream file(file_name, ios::binary);
    return {istreambuf_iterator<char>(file), istreambuf_iterator<char>()};
}

void FileService::writeFile(const string &file_name, const vector<unsigned char> &data) {
    ofstream file(file_name, ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

void FileService::createDirectory(const string &file_path) {
    path outDir(file_path);
    if (!exists(outDir)) {
        create_directory(outDir);
    }
}

string FileService::removeExtension(const string &file_name, const string &extension) {
    std::string f = path(file_name).filename().string();
    if (f.find(extension) != string::npos) {
        return f.substr(0, f.find(extension));
    }
    return "";
}
