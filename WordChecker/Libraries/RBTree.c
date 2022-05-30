//
//  RBTree.c
//  WordChecker
//
//  Created by Japo on 29/05/22.
//

#include "RBTree.h"

void leftRoatate(t_rbnode *tree,t_rbnode x){
    t_rbnode y = x->right;
    x->right = y->left;
    
    if(y->left != NULL){
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    
    if(x->parent == NULL){
        *tree = y;
    }
    else if(x == x->parent->left){
        x->parent->left = y;
    }
    else{
        x->parent->right = y;
    }
    
    y->left = x;
    
    x->parent = y;
}

void rightRotation(t_rbnode *tree,t_rbnode y){
    t_rbnode x = y->left;
    y->left = x->right;
    
    if(x->right != NULL){
        x->right->parent = y;
    }
    
    x->parent = y->parent;
    
    if(y->parent == NULL){
        *tree = x;
    }
    else if(y == y->parent->right){
        y->parent->right = x;
    }
    else{
        y->parent->left = x;
    }
    
    x->right = y;
    y->parent = x;
    
}

void insert(t_rbnode *tree,t_rbnode newNode){
    
    if(*tree == NULL){
        *tree = newNode;
        return;
    }
    
    t_rbnode *temp = *tree;
    t_rbnode *parent = NULL;
    
    while(temp != NULL){
        parent= temp;
        if(newNode->key > temp->key){
            temp = temp->right;
        }
        else{
            temp = temp->left;
        }
    }
    
    newNode->parent = parent;
    
    if(newNode->key > parent->key){
        parent->right = newNode;
    }
    else{
        parent->left = newNode;
    }
    
    newNode->color = RED;
    newNode->left = NULL;
    newNode->right = NULL;
    
    insertFixUp(tree,newNode)

}


void insertFixUp(t_rbnode *tree, t_rbnode newNode){
    
}
