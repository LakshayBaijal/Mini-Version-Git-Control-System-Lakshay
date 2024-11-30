#include "cat_file.h"
#include "sha1.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace fs = filesystem;

void CatFile::catFile(const string& flag, const string& sha1) {

    if (sha1.length() != 40) 
    {
        cerr << "Error: Invalid SHA-1 hash.\n";
        exit(1);
    }

    string dir = sha1.substr(0, 2);
    string file = sha1.substr(2);
    fs::path object_path = fs::path(".mygit") / "objects" / dir / file;

    if (!fs::exists(object_path)) 
    {
        cerr << "Error: Object " << sha1 << " does not exist.\n";
        exit(1);
    }

    ifstream object_file(object_path, ios::binary);
    if (!object_file) 
    {
        cerr << "Error: Cannot open object file " << object_path << ".\n";
        exit(1);
    }

    string header;
    getline(object_file, header, '\0'); 

    size_t space_pos = header.find(' ');
    if (space_pos == string::npos) 
    {
        cerr << "Error: Malformed object header.\n";
        exit(1);
    }

    string type = header.substr(0, space_pos);
    string size_str = header.substr(space_pos + 1);
    size_t size = stoul(size_str);

    string body((istreambuf_iterator<char>(object_file)), istreambuf_iterator<char>());
    object_file.close();

    if (body.size() != size) 
    {
        cerr << "Error: Object size mismatch.\n";
        exit(1);
    }

    if (flag == "-p") 
    {
        cout << body <<"\n";
    }
    else if (flag == "-s") 
    {
        cout << size << "\n";
    }
    else if (flag == "-t") 
    {
        cout << type << "\n";
    }
    else 
    {
        cerr << "Available flags: -p, -s, -t\n";
        exit(1);
    }
}
