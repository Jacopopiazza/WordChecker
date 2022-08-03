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

typedef struct occ{
    short count,
        exact;
}t_occurrence;

int charToIndex(char c);
char indexToChar(int i);
int addWordToTree(t_node* tree, const char* word);
void printWords(t_node* tree,int wordLen);
void printWordsRecCall(t_node *tree,int wordLen,int step,char *buffer);
void filterTree(t_node* tree,int wordLen,t_occurrence occurrencies[N]);
void filterTreeRecCall(t_node* tree,int wordLen,int step,t_occurrence occurrencies[N],int currentWord[N]);

int countWords(t_node* tree,int wordLen);
void countWordsRecCall(t_node *tree,int wordLen,int step,char *buffer,int *count);



#endif /* Dictionary_h */
