//
//  Dictionary.c
//  WordChecker
//
//  Created by Japo on 04/07/22.
//

#include "Dictionary.h"

char indexToChar(int i){
    if(i >= 0 && i<= 25){
        return 'a' + i;
    }
    
    if(i>=26 && i <= 51){
        return 'A' + i - 26;
    }
    
    if(i>= 52 && i<= 61){
        return '0' + i - 52;
    }
    
    if(i == 62){
        return '-';
    }
    
    return '_';
}

int charToIndex(char c){
    if (c >= 'a' && c <= 'z'){
        return c-'a';
    }
    
    if(c >= 'A' && c <= 'Z'){
        return 26 + c - 'A';
    }
    
    if(c >= '0' && c <= '9'){
        return 52 + c - '0';
    }
    
    if (c == '-'){
        return 62;
    }
    
    return 63;
}



void printWords(t_node* tree,int wordLen){
    
    if(tree == NULL || tree->hasChild == 0){
        return;
    }
    
    char* buffer = malloc(sizeof(char) * wordLen);
    for(int i=0;i<wordLen;i++){
        buffer[i] = '\0';
    }
    
    for(int i=0;i<N;i++){
        //Rec call
        buffer[0] = indexToChar(i);
        if(tree->pointers[i] != NULL && tree->matches == 1 && tree->hasChild == 1){
            printWordsRecCall(tree->pointers[i],wordLen,1,buffer);
        }
        
        
    }
    
}

void printWordsRecCall(t_node *tree,int wordLen,int step,char *buffer){
    if(step >= wordLen){
        printf("%s\n",buffer);
        return;
    }
    
    for(int i=0;i<N;i++){
        //Rec call
        buffer[step] = indexToChar(i);
        if(tree->pointers[i] != NULL && tree->matches == 1 && tree->hasChild == 1){
            printWordsRecCall(tree->pointers[i], wordLen, step+1, buffer);
        }
    }
    
    
}






int addWordToTree(t_node* tree, const char* word){
    
    
    if(tree == NULL){
        tree = (t_node*)malloc(sizeof(t_node));
        if(tree == NULL){
            return 0;
        }
        tree->hasChild = 0;
        tree->matches = 1;
        for(int i=0;i<N;i++){
            tree->pointers[i] = NULL;
        }
    }
    
    int len = (int)strlen(word);
    int i = 0;
    t_node* curr = tree;
    
    while(i<len){
        
        char c = word[i];
        int index = charToIndex(c);
        if (curr->pointers[index] == NULL){
            if(curr->hasChild == 0){
                curr->hasChild = 1;
            }
            curr->pointers[index] = (t_node*)malloc(sizeof(t_node));
            if( curr->pointers[index] == NULL){
                return 0;
            }
            curr->pointers[index]->hasChild = 0;
            curr->pointers[index]->matches = 1;
            for(int i=0;i<N;i++){
                curr->pointers[index]->pointers[i] = NULL;
            }
        }
                
        curr = curr->pointers[index];
        i++;
    }

    return 1;
    
}
