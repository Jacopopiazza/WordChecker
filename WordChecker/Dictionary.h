//
//  Dictionary.h
//  WordChecker
//
//  Created by Japo on 04/07/22.
//

#ifndef Dictionary_h
#define Dictionary_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 64

typedef struct nodo{
    int matches;
    int hasChild;
    struct nodo* pointers[N];
    
}t_node;

int charToIndex(char c);
char indexToChar(int i);
int addWordToTree(t_node* tree, const char* word);
void printWords(t_node* tree,int wordLen);
void printWordsRecCall(t_node *tree,int wordLen,int step,char *buffer);

#endif /* Dictionary_h */
