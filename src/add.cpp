#include "add.h"
#include "hash_object.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <algorithm>

namespace fs = filesystem;

vector<tuple<string, string, string, string>> Add::readIndex() 
{
    vector<tuple<string, string, string, string>> entries;
    fs::path index_path = ".mygit/index";

    if (!fs::exists(index_path)) 
    {
        return entries; 
    }

    ifstream index_file(index_path);
    string line;
    while (getline(index_file, line)) 
    {
        istringstream iss(line);
        string mode, type, sha1, path;
        if (!(iss >> mode >> type >> sha1 >> path)) 
        {
            continue;
        }
        entries.emplace_back(mode, type, sha1, path);
    }

    index_file.close();
    return entries;
}

void Add::writeIndex(const vector<tuple<string, string, string, string>>& entries) 
{
    fs::path index_path = ".mygit/index";
    ofstream index_file(index_path, ios::trunc);

    for (const auto& entry : entries) 
    {
        index_file << get<0>(entry) << " "
                   << get<1>(entry) << " "
                   << get<2>(entry) << " "
                   << get<3>(entry) << "\n";
    }

    index_file.close();
}

void Add::updateIndex(const string& mode, const string& type, const string& sha1, const string& path) 
{
    auto entries = readIndex();
    entries.erase(
        remove_if(entries.begin(), entries.end(),
                       [&](const tuple<string, string, string, string>& entry) {
                           return get<3>(entry) == path;
                       }),
        entries.end()
    );

    entries.emplace_back(mode, type, sha1, path);

    writeIndex(entries);
}

void Add::addFile(const string& filepath) 
{
    if (!fs::exists(filepath)) 
    {
        cerr << "Error: File " << filepath << " does not exist.\n";
        return;
    }

    if (fs::is_directory(filepath)) 
    {
        cerr << "Error: " << filepath << " is a directory. Use './mygit add .' to add directories.\n";
        return;
    }

    string sha1 = HashObject::hash_object(filepath, true);
    if (sha1.empty()) 
    {
        cerr << "Error: Failed to hash file " << filepath << ".\n";
        return;
    }

    string mode = "100644";

    fs::path repo_root = fs::current_path();
    fs::path file_path = fs::absolute(filepath);
    if (file_path.string().find(fs::absolute(".mygit").string()) == 0) 
    {
        cerr << "Error: Attempting to add files from .mygit directory.\n";
        return;
    }
    fs::path relative_path = fs::relative(file_path, repo_root);

    updateIndex(mode, "blob", sha1, relative_path.string());

    cout << "Added " << relative_path.string() << "\n";
}

void Add::addAll(const string& directory) {
    fs::path dir_path = fs::absolute(directory);
    fs::path repo_root = fs::current_path();

    for (const auto& entry : fs::recursive_directory_iterator(dir_path)) {
        fs::path path = entry.path();

        fs::path relative_path;
        try 
        {
            relative_path = fs::relative(path, repo_root);
        }
        catch (const fs::filesystem_error& e) {
            cerr << "Error computing relative path for " << path << ": " << e.what() << "\n";
            continue;
        }

        if (relative_path.string().find(".mygit") == 0) 
        {
            continue;
        }

        if (relative_path.string().find("src/") == 0 || relative_path.string().find("include/") == 0) 
        {
            continue;
        }

        string filename = path.filename().string();
        transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
        if (filename == "makefile") 
        {
            continue;
        }

        if (entry.is_regular_file()) 
        {
            addFile(relative_path.string());
        } 
    }
}

// Adds a list of paths to the staging area
void Add::add(const vector<string>& paths) {
    if (paths.empty()) 
    {
        cerr << "Error: No paths provided to add.\n";
        return;
    }

    for (const auto& path : paths) 
    {
        if (path == ".") 
        {
            addAll(".");
        }
        else {
            addFile(path);
        }
    }
}
