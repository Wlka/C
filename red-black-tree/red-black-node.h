#pragma once

/****************************************************************************************************
 * red-black-tree is a binary sreach tree with color property
 * for any valid red-black-tree,it has some additional requirements as follows:
 * 1. the nodes' color is red or black.
 * 2. the root node's color is black.
 * 3. all the leaf nodes' color is black.
 * 4. each node with red color should has two child nodes with black color.
 *    (the path from leaf to root cannot has two continuous nodes with red color)
 * 5. all the simple path from any node to its leaf includes the same quantity nodes with black color.
 * 
****************************************************************************************************/

const int RED=0;
const int BLACK=1;


template<typename T>
class red_black_node
{
private:
    int color;
    T value;
    red_black_node* left;
    red_black_node* right;
    red_black_node* parent;
public:
    red_black_node(T value,int color):color(color),value(value),left(nullptr),right(nullptr),parent(nullptr)
    {
        
    }

    int Color() const
    {
        return color;
    }

    T Value() const
    {
        return value;
    }

    red_black_node* Left() const
    {
        return left;
    }

    red_black_node* Right() const
    {
        return right;
    }

    red_black_node* Parent() const
    {
        return parent;
    }

    red_black_node* insert(red_black_node* newNode)
    {
        if(this->value > newNode->value)
        {
            if(this->left)
                return this->left->insert(newNode);
            else
            {
                this->left=newNode;
                newNode->parent=this;
                return fixup(newNode);  //return the new root
            }
        }
        else if(this->value < newNode->value)
        {
            if(this->right)
                return this->right->insert(newNode);
            else
            {
                this->right=newNode;
                newNode->parent=this;
                return fixup(newNode);  //return the new root
            }
        }
        return this;
    }

    red_black_node* left_rotate()
    {
        /******************************
         *           左旋
         *      P           P
         *      A     ->    R
         *    L   R       A 
         *              L 
        ******************************/
        red_black_node* parent=this->parent;
        if(parent && parent->left==this)
            parent->left=this->right;
        if(parent && parent->right==this)
            parent->right=this->right;
        
        this->parent=this->right;
        this->right->parent=parent;

        red_black_node* tmp=this->right->left;
        this->right->left=this;
        this->right=tmp;
        return this;
    }

    red_black_node* right_rotate()
    {
        /******************************
         *           右旋
         *      P           P
         *      A     ->    L
         *    L   R          A
         *                     R
        ******************************/
        red_black_node* parent=this->parent;
        if(parent && parent->left)
            parent->left=this->left;
        if(parent && parent->right)
            parent->right=this->right;
        
        this->parent=this->left;
        this->left->parent=parent;
        
        red_black_node* tmp=this->left->right;
        this->left->right=this;
        this->left=tmp;
        return this;
    }

    //rotate and adjust color
    red_black_node* fixup(red_black_node* newNode)
    {
        red_black_node* current=newNode;
        //the current's color is red(we set the insert node's color is red)
        while(current->parent && current->parent->color==RED)
        {
            //parent is not null and color is red -> grandparent is not null
            red_black_node* parent=current->parent;
            red_black_node* grandparent=parent->parent;

            if(grandparent->left==parent)
            {
                if(grandparent->right && grandparent->right->color==RED)
                {
                    //case 1
                    grandparent->right->color=BLACK;
                    parent->color=BLACK;
                    grandparent->color=RED;
                    current=grandparent;
                }
                else
                {
                    //case 2
                    if(this==parent->right)
                    {
                        parent->left_rotate();
                        current=parent;
                    }
                    else
                    {
                        grandparent->right_rotate();
                        grandparent->color=RED;
                        parent->color=BLACK;
                    }
                }
            }
            else
            {
                if(grandparent->left && grandparent->left->color==RED)
                {
                    //case 3
                    grandparent->left->color=BLACK;
                    parent->color=BLACK;
                    grandparent->color=RED;
                    current=grandparent;
                }
                else
                {
                    //case 4 - reverse case 2
                    if(this==parent->left)
                    {
                        parent->right_rotate();
                        current=parent;
                    }
                    else
                    {
                        grandparent->left_rotate();
                        grandparent->color=RED;
                        parent->color=BLACK;
                    }
                }
            }
        }
        red_black_node* tmp=current;
        while(tmp->parent)
            tmp=tmp->parent;
        tmp->color=BLACK;

        return tmp; //the new root
    }

};