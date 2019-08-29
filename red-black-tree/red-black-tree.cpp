#include <iostream>
#include "red-black-node.h"
#include "dot.h"

using namespace std;

int main()
{
    red_black_node<int>* root=nullptr;
    int array[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    for(int i:array)
    {
        /************************************************************
         * the root node' color should be black
         * 
         * set the leaf node'color as red,
         * it may cause conflicts between two consecutive red nodes,
         * and we can call fixup function to adjust
        ************************************************************/
        if(!root)
            root=new red_black_node<int>(i,BLACK);
        else
            root=root->insert(new red_black_node<int>(i,RED));
        createDotFile(to_string(i)+".dot",root);
    }

    //print the tree
    createDotFile("myDot.dot",root);
    cout<<"done!"<<endl;
    return 0;
}