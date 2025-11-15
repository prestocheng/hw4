#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    BinarySearchTree<int,double> bt;

    bt.insert(std::make_pair(5, 1.0));
    bt.print();
	  bt.insert(std::make_pair(2, 1.0));
    bt.print();
	  bt.insert(std::make_pair(6, 1.0));
    bt.print();
    bt.insert(std::make_pair(4, 1.0));
    bt.print();
    bt.remove(5);
    bt.print();
    
	  //bt.remove(5);
    //bt.print();

    // AVL Tree Tests
    /*
    AVLTree<char,int> at;
    at.insert(std::make_pair('a',1));
    at.insert(std::make_pair('b',2));

    cout << "\nAVLTree contents:" << endl;
    for(AVLTree<char,int>::iterator it = at.begin(); it != at.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(at.find('b') != at.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    at.remove('b');
    */
    return 0;
}
