
#include "init.h"
#include <filesystem>
#include <fstream>
#include <iostream>
using namespace std;

namespace fs = filesystem;

void InitRepository::init(const string& directory) {
    fs::path repo_path = fs::absolute(directory) / ".mygit";

    if (fs::exists(repo_path)) {
        cerr << "Reinitialized existing mygit repository in " << repo_path << "\n";
        return;
    }

    fs::create_directory(repo_path);
    cout << "Created directory: " << repo_path << "\n";

    fs::create_directory(repo_path / "objects");
    cout << "Created directory: " << repo_path / "objects" << "\n";

    fs::create_directory(repo_path / "hooks");
    cout << "Created directory: " << repo_path / "hooks" << "\n";

    fs::create_directory(repo_path / "info");
    cout << "Created directory: " << repo_path / "info" << "\n";

    // Create HEAD file (empty since no commits yet)
    fs::path head_path = repo_path / "HEAD";
    ofstream head_file(head_path);
    if (!head_file) 
    {
        cerr << "Error: Cannot create HEAD file.\n";
        exit(1);
    }
    head_file.close();
    cout << "Created file: " << head_path << "\n";

    fs::path config_path = repo_path / "config";
    ofstream config_file(config_path);
    if (!config_file) 
    {
        cerr << "Error: Cannot create config file.\n";
        exit(1);
    }
    config_file.close();
    cout << "Created file: " << config_path << "\n";

    fs::path description_path = repo_path / "description";
    ofstream description_file(description_path);
    if (!description_file) {
        cerr << "Error: Cannot create description file.\n";
        exit(1);
    }
    description_file.close();
    cout << "Created file: " << description_path << "\n";

    cout << "Initialized empty mygit repository in " << repo_path << "\n";
}
