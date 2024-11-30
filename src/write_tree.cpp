#include "write_tree.h"
#include "hash_object.h"
#include "sha1.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <tuple>

namespace fs = filesystem;
using namespace std;

namespace 
{
    string buildTree(const fs::path& directory);
    void writeObject(const string& sha1, const string& content);
}

string WriteTree::writeTree(const string& directory) 
{

    return buildTree(directory);
}

namespace {
string buildTree(const fs::path& directory) {
    vector<tuple<string, string, string>> entries; 

    for (const auto& entry : fs::directory_iterator(directory)) {
        fs::path path = entry.path();
        string name = path.filename().string();

        if (name == ".mygit" || name == "src" || name == "include" || name == "Makefile") 
        {
            continue;
        }

        if (entry.is_regular_file()) 
        {
            string sha1 = HashObject::hash_object(path.string(), true);
            entries.emplace_back("100644", name, sha1);
        } 
        else if (entry.is_directory()) 
        {
            string sha1 = buildTree(path);
            entries.emplace_back("040000", name, sha1);
        }
    }

    string tree_content;
    for (const auto& [mode, name, sha1_hex] : entries) 
    {
        string sha1_binary;
        for (size_t i = 0; i < sha1_hex.size(); i += 2) 
        {
            string byte_str = sha1_hex.substr(i, 2);
            char byte = static_cast<char>(stoi(byte_str, nullptr, 16));
            sha1_binary.push_back(byte);
        }

        ostringstream oss;
        oss << mode << " " << name << '\0';
        string entry_content = oss.str() + sha1_binary;
        tree_content += entry_content;
    }

    string header = "tree " + to_string(tree_content.size()) + '\0';
    string full_content = header + tree_content;

    string tree_sha1 = SHA1::hash(full_content);
    writeObject(tree_sha1, full_content);

    return tree_sha1;
}

void writeObject(const string& sha1, const string& content) {
    fs::path objects_dir = ".mygit/objects";
    if (!fs::exists(objects_dir)) {
        cerr << "Error: Repository not initialized. Run ./mygit init first.\n";
        exit(1);
    }

    string dir_name = sha1.substr(0, 2);
    string file_name = sha1.substr(2);
    fs::path object_dir = objects_dir / dir_name;

    if (!fs::exists(object_dir)) 
    {
        fs::create_directory(object_dir);
    }

    fs::path object_path = object_dir / file_name;

    if (fs::exists(object_path)) 
    {
        return;
    }

    ofstream object_file(object_path, ios::binary);
    if (!object_file) 
    {
        cerr << "Error: Cannot write object to " << object_path << "\n";
        exit(1);
    }

    object_file.write(content.data(), content.size());
    object_file.close();
}
}
