
#include "commit.h"
#include "write_tree.h"
#include "sha1.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>

namespace fs = std::filesystem;
using namespace std;
void Commit::commit(const std::string& message) {
    if (!fs::exists(".mygit")) {
        cerr << "Error: Repository not initialized. Run ./mygit init first.\n";
        exit(1);
    }

    fs::path index_path = ".mygit/index";
    if (!fs::exists(index_path)) {
        cerr << "Error: No changes added to commit. Use './mygit add' to stage changes.\n";
        exit(1);
    }

    string tree_sha1 = WriteTree::writeTree();
    if (tree_sha1.empty()) {
        cerr << "Error: Failed to write tree object.\n";
        exit(1);
    }

    fs::path head_path = ".mygit/HEAD";
    string parent_sha1;
    if (fs::exists(head_path)) {
        ifstream head_file(head_path);
        getline(head_file, parent_sha1);
        head_file.close();
        if (parent_sha1.empty()) {
            parent_sha1 = "";
        }
    }

    ostringstream commit_content;
    commit_content << "tree " << tree_sha1 << "\n";
    if (!parent_sha1.empty()) 
    {
        commit_content << "parent " << parent_sha1 << "\n";
    }

    time_t now = time(nullptr);
    commit_content << "committer Your Name <you@example.com> " << now << " +0000\n";
    commit_content << "\n";
    commit_content << message << "\n";

    string content = commit_content.str();

    string header = "commit " + to_string(content.size()) + '\0';
    string full_content = header + content;

    string commit_sha1 = SHA1::hash(full_content);
    if (commit_sha1.empty()) 
    {
        cerr << "Error: Failed to compute SHA-1 hash for commit.\n";
        exit(1);
    }

    fs::path object_dir = fs::path(".mygit") / "objects" / commit_sha1.substr(0, 2);
    fs::path object_path = object_dir / commit_sha1.substr(2);

    if (!fs::exists(object_dir)) {
        fs::create_directories(object_dir);
    }

    if (!fs::exists(object_path)) {
        ofstream object_file(object_path, std::ios::binary);
        if (!object_file) {
            cerr << "Error: Cannot write commit object to " << object_path << "\n";
            exit(1);
        }
        object_file.write(full_content.data(), full_content.size());
        object_file.close();
    }

    ofstream head_file(head_path, std::ios::trunc);
    if (!head_file) 
    {
        cerr << "Error: Cannot update HEAD.\n";
        exit(1);
    }
    head_file << commit_sha1 << "\n";
    head_file.close();

    fs::remove(index_path);

    cout << commit_sha1 << "\n";
}
