#ifndef RECCHECK
#include <algorithm>
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int getHeight(Node * root)
{
    // Base case
    if (root == nullptr) {
      return -1;
    }

    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);

    return 1 + std::max(leftHeight, rightHeight);
}


bool equalPaths(Node * root)
{
    // Base case
    if (root == nullptr) {
      return true;
    }

    // Check for leaf node
    if (root->left == nullptr && root->right == nullptr) {
      return true;
    }

    // Only one child exists, check if its a leaf
    if (root->left == nullptr) {
      return equalPaths(root->right);
    }
    else if (root->right == nullptr) {
      return equalPaths(root->left);
    }
    
    // Two children exist
    if (getHeight(root->left) == getHeight(root->right)) {
      if (equalPaths(root->left) && equalPaths(root->right)) {
        return true;
      }
    }
    return false;
}

