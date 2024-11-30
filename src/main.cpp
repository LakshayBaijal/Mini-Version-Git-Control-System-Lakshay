
#include <iostream>
#include <string>
#include <vector>
#include "init.h"
#include "hash_object.h"
#include "cat_file.h"
#include "write_tree.h"
#include "ls_tree.h"
#include "add.h"
#include "commit.h"
#include "log.h" 

using namespace std;
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./mygit <command> [options]\n";
        return 1;
    }

    std::string command = argv[1];

    if (command == "init") {
        InitRepository::init(".");
    }
    else if (command == "hash-object") {
        if (argc < 3) {
            std::cerr << "Usage: ./mygit hash-object [-w] <file>\n";
            return 1;
        }

        bool write_flag = false;
        std::string filepath;

        if (std::string(argv[2]) == "-w") {
            write_flag = true;
            if (argc < 4) {
                std::cerr << "Usage: ./mygit hash-object [-w] <file>\n";
                return 1;
            }
            filepath = argv[3];
        }
        else {
            filepath = argv[2];
        }

        std::string sha1 = HashObject::hash_object(filepath, write_flag);
        if (!sha1.empty()) {
            std::cout << sha1 << "\n";
        }
    }
    else if (command == "cat-file") {
        if (argc < 4) {
            std::cerr << "Usage: ./mygit cat-file <flag> <object_sha>\n";
            std::cerr << "Available flags: -p, -s, -t\n";
            return 1;
        }

        std::string flag = argv[2];
        std::string sha1 = argv[3];
        CatFile::catFile(flag, sha1);
    }
    else if (command == "write-tree") {
        std::string tree_sha1 = WriteTree::writeTree();
        if (!tree_sha1.empty()) {
            std::cout << tree_sha1 << "\n";
        }
    }
    else if (command == "ls-tree") {
        if (argc < 3) {
            std::cerr << "Usage: ./mygit ls-tree [--name-only] <tree_sha>\n";
            return 1;
        }

        bool name_only = false;
        std::string tree_sha1;

        if (std::string(argv[2]) == "--name-only") {
            if (argc < 4) {
                std::cerr << "Usage: ./mygit ls-tree [--name-only] <tree_sha>\n";
                return 1;
            }
            name_only = true;
            tree_sha1 = argv[3];
        }
        else {
            tree_sha1 = argv[2];
        }

        LsTree::listTree(tree_sha1, name_only);
    }
    else if (command == "add") {
        if (argc < 3) {
            std::cerr << "Usage: ./mygit add [file1 file2 ... | .]\n";
            return 1;
        }

        std::vector<std::string> paths;
        for (int i = 2; i < argc; ++i) {
            paths.emplace_back(argv[i]);
        }

        Add::add(paths);
    }
    else if (command == "commit") {
        std::string message = "No commit message";
        if (argc >= 3 && std::string(argv[2]) == "-m") {
            if (argc >= 4) {
                message = argv[3];
            } else {
                std::cerr << "Usage: ./mygit commit -m \"Commit message\"\n";
                return 1;
            }
        }
        Commit::commit(message);
    }
    else if (command == "log") {
        Log::log(); // Handle log command
    }
    else {
        std::cerr << "Unknown command: " << command << "\n";
        std::cerr << "Available commands: init, hash-object, cat-file, write-tree, ls-tree, add, commit, log\n";
        return 1;
    }

    return 0;
}
