
#include "hash_object.h"
#include "sha1.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

namespace fs = filesystem;

string HashObject::hash_object(const string& filepath, bool write) {
    ifstream file(filepath, ios::binary);
    if (!file) 
    {
        cerr << "Error: Cannot open file " << filepath << "\n";
        return "";
    }

    ostringstream oss;
    oss << file.rdbuf();
    string content = oss.str();
    file.close();

    string header = "blob " + to_string(content.size()) + '\0';
    string store_content = header + content;

    string sha1 = SHA1::hash(store_content);

    if (write) 
    {
        fs::path object_dir = fs::path(".mygit") / "objects" / sha1.substr(0, 2);
        fs::path object_path = object_dir / sha1.substr(2);

        if (!fs::exists(object_dir)) {
            fs::create_directories(object_dir);
        }

        if (!fs::exists(object_path)) {
            ofstream object_file(object_path, ios::binary);
            if (!object_file) {
                cerr << "Error: Cannot write object to " << object_path << "\n";
                return "";
            }
            object_file.write(store_content.data(), store_content.size());
            object_file.close();
        }
    }

    return sha1;
}
