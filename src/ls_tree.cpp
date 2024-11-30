
#include "ls_tree.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace fs = filesystem;
using namespace std;

void LsTree::listTree(const string& tree_sha1, bool name_only) {

    if (tree_sha1.length() != 40) {
        cerr << "Error: Invalid SHA-1 hash.\n";
        exit(1);
    }

    string dir = tree_sha1.substr(0, 2);
    string file = tree_sha1.substr(2);
    fs::path object_path = fs::path(".mygit") / "objects" / dir / file;

    if (!fs::exists(object_path)) {
        cerr << "Error: Tree object " << tree_sha1 << " does not exist.\n";
        exit(1);
    }

    ifstream object_file(object_path, ios::binary);
    if (!object_file) {
        cerr << "Error: Cannot open tree object file " << object_path << ".\n";
        exit(1);
    }

    // Read the header
    string header;
    getline(object_file, header, '\0'); // Read until null character

    size_t space_pos = header.find(' ');
    if (space_pos == string::npos) 
    {
        cerr << "Error: Malformed tree object header.\n";
        exit(1);
    }

    string type = header.substr(0, space_pos);
    string size_str = header.substr(space_pos + 1);
    size_t size = stoul(size_str);

    if (type != "tree") 
    {
        cerr << "Error: Object " << tree_sha1 << " is not a tree.\n";
        exit(1);
    }

    string body((istreambuf_iterator<char>(object_file)), istreambuf_iterator<char>());
    object_file.close();

    if (body.size() != size) 
    {
        cerr << "Error: Tree object size mismatch.\n";
        exit(1);
    }

    size_t pos = 0;
    while (pos < body.size()) {
        size_t space_pos = body.find(' ', pos);
        if (space_pos == string::npos) break;
        string mode = body.substr(pos, space_pos - pos);
        pos = space_pos + 1;

        size_t null_pos = body.find('\0', pos);
        if (null_pos == string::npos) break;
        string name = body.substr(pos, null_pos - pos);
        pos = null_pos + 1;

        if (pos + 20 > body.size()) 
        {
            cerr << "Error: Invalid tree object format.\n";
            exit(1);
        }
        string sha1_binary = body.substr(pos, 20);
        pos += 20;

        ostringstream sha1_hex;
        for (unsigned char c : sha1_binary) {
            sha1_hex << hex << setw(2) << setfill('0') << (int)c;
        }

        string entry_type = (mode == "040000") ? "tree" : "blob";

        if (name_only) 
        {
            cout << name << "\n";
        } else 
        {
            cout << mode << " " << entry_type << " " << sha1_hex.str() << " " << name << "\n";
        }
    }
}
