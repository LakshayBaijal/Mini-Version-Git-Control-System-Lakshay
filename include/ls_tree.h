
#ifndef LS_TREE_H
#define LS_TREE_H

#include <string>
using namespace std;

class LsTree {
public:
  static void listTree(const string& tree_sha1, bool name_only = false);
};

#endif 
