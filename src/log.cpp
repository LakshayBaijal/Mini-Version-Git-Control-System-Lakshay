#include "log.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace fs = filesystem;
using namespace std;

void Log::log() 
{
    fs::path head_path = ".mygit/HEAD";
    if (!fs::exists(head_path)) 
    {
        cerr << "Error: HEAD file does not exist. No commits yet.\n";
        exit(1);
    }

    ifstream head_file(head_path);
    string current_sha1;
    getline(head_file, current_sha1);
    head_file.close();

    if (current_sha1.empty()) 
    {
        cerr << "Error: HEAD is empty. No commits yet.\n";
        exit(1);
    }

    while (!current_sha1.empty()) {
        string dir = current_sha1.substr(0, 2);
        string file = current_sha1.substr(2);
        fs::path object_path = fs::path(".mygit") / "objects" / dir / file;

        if (!fs::exists(object_path)) {
            cerr << "Error: Commit object " << current_sha1 << " does not exist.\n";
            exit(1);
        }

        ifstream commit_file(object_path, ios::binary);
        if (!commit_file) {
            cerr << "Error: Cannot open commit object " << current_sha1 << ".\n";
            exit(1);
        }

        string header;
        getline(commit_file, header, '\0'); 

        size_t space_pos = header.find(' ');
        if (space_pos == string::npos) {
            cerr << "Error: Malformed commit object header.\n";
            exit(1);
        }

        string type = header.substr(0, space_pos);
        string size_str = header.substr(space_pos + 1);
        size_t size = stoul(size_str);

        if (type != "commit") {
            cerr << "Error: Object " << current_sha1 << " is not a commit.\n";
            exit(1);
        }

        string body((istreambuf_iterator<char>(commit_file)), istreambuf_iterator<char>());
        commit_file.close();

        if (body.size() != size) {
            cerr << "Error: Commit object size mismatch.\n";
            exit(1);
        }

        istringstream iss(body);
        string line;
        string tree_sha1;
        string parent_sha1;
        string committer_line;
        string commit_message;

        while (getline(iss, line) && !line.empty())
         {
            if (line.rfind("tree ", 0) == 0) {
                tree_sha1 = line.substr(5);
            }
            else if (line.rfind("parent ", 0) == 0) {
                parent_sha1 = line.substr(7);
            }
            else if (line.rfind("committer ", 0) == 0) {
                committer_line = line.substr(10);
            }
        }

        ostringstream message_oss;
        while (getline(iss, line)) {
            message_oss << line << "\n";
        }
        commit_message = message_oss.str();

        size_t last_space = committer_line.find_last_of(' ');
        if (last_space == string::npos) {
            cerr << "Error: Malformed committer line.\n";
            exit(1);
        }
        string timestamp_str = committer_line.substr(last_space + 1);
        long timestamp = stol(timestamp_str);

        time_t t = timestamp;
        tm* tm_info = localtime(&t); 
        char buffer[30];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

        cout << "commit " << current_sha1 << "\n";
        if (!parent_sha1.empty()) {
            cout << "Parent: " << parent_sha1 << "\n";
        }
        cout << "Date:   " << buffer << "\n\n";
        cout << "    " << commit_message << "\n";

        current_sha1 = parent_sha1;
    }
}
