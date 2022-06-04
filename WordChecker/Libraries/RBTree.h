//
//  RBTree.h
//  WordChecker
//
//  Created by Japo on 29/05/22.
//

#ifndef RBTree_h
#define RBTree_h

#include <stdio.h>

enum treeColor{
    RED = 0,
    BLAKC = 1
};

typedef struct rbnode{
    
    char key;
    int count;
    enum treeColor color;
    
    struct rbnode *parent,*left,*right;
    
}*t_rbnode;

void leftRoatate(t_rbnode *tree,t_rbnode x);
void rightRoatate(t_rbnode *tree,t_rbnode y);
void insert(t_rbnode *tree,t_rbnode newNode);
void insertFixUp(t_rbnode *tree, t_rbnode newNode);



#endif /* RBTree_h */
