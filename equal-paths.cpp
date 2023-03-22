#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int equalPathshelper(Node * root);

bool equalPaths(Node * root)
{
 if(equalPathshelper(root)!=-10){
        return true;
    }
    else{
        return false;
    }
}
int equalPathshelper(Node * root){
    if(root==NULL){
        return 1;
    }
    int left_recurse = equalPathshelper(root->left);
    int right_recurse = equalPathshelper(root->right);
    if(root->right||root->left != NULL){
        right_recurse += 10;
        left_recurse += 10;
      
    }
    if(left_recurse == right_recurse){
        int recurse = right_recurse+left_recurse/2;
        return recurse;
    }
    else{
        return -10;
    }
    
}

