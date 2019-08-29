#include <fstream>
#include <iostream>
#include <string>
#include "red-black-node.h"

using namespace std;

template<typename T>
void pre_progress(ofstream &out,red_black_node<T>* root)
{
    out<<root->Value()<<"[color="<<(root->Color()==BLACK?"black":"red")<<"]"<<endl;
    if(root->Left())
        pre_progress(out,root->Left());
    if(root->Right())
        pre_progress(out,root->Right());
}

template<typename T>
void visitNode(ofstream &out,red_black_node<T>* root)
{
    if(root->Left())
    {
        out<<root->Value()<<"->"<<root->Left()->Value()<<endl;
        visitNode(out,root->Left());
    }
    if(root->Right())
    {
        out<<root->Value()<<"->"<<root->Right()->Value()<<endl;
        visitNode(out,root->Right());
    }
}


template<typename T>
void createDotFile(string fileName,red_black_node<T>* root)
{
    ofstream out;
    out.open(fileName);
    out<<"digraph myGraph{"<<endl;

    pre_progress(out,root);
    visitNode(out,root);
    
    out<<"}"<<endl;
    out.close();
}