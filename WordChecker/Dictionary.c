//
//  Dictionary.c
//  WordChecker
//
//  Created by Japo on 04/07/22.
//

#include "Dictionary.h"

char indexToChar(int i){
    
    if(i==0){
        return '-';
    }
    
    if(i >= 1 && i <= 10){
        return '0' + i - 1;
    }
    
    if(i>= 11 && i <= 36){
        return 'A' + i - 11;
    }
    
    if(i == 37){
        return '_';
    }
    
    if (i>= 38 && i <= 63){
        return 'a'+i -38;
    }
    return '-';
}

int charToIndex(char c){
    
    if(c == '-'){
        return 0;
    }
    
    if(c >= '0' && c <= '9'){
        return c - '0' + 1;
    }
    
    if(c >= 'A' && c <= 'Z'){
        return c - 'A' + 11;
    }
    
    if(c == '_'){
        return c - '_'+37;
    }
    
    if(c >= 'a' && c <= 'z'){
        return 38 + c - 'a';
    }
    
    return 0;
}



void printWords(t_node* tree,int wordLen){
    
    if(tree == NULL || tree->hasChild == 0){
        return;
    }

    char* buffer = calloc(wordLen, sizeof(char));
    
    
    for(size_t i=0;i<N;i++){
        //Rec call
        buffer[0] = indexToChar(i);
        if(tree->pointers[i] != NULL && tree->matches == 1 && tree->hasChild == 1){
            printWordsRecCall(tree->pointers[i],wordLen,1,buffer);
        }
        
        
    }
    
}

void printWordsRecCall(t_node *tree,int wordLen,int step,char *buffer){
    
    if(step == wordLen){
        if(tree->matches){
            printf("%s\n",buffer);
        }
        return;
    }
    
    
    for(size_t i=0;i<N;i++){
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
        for(size_t i=0;i<N;i++){
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
            for(size_t i=0;i<N;i++){
                curr->pointers[index]->pointers[i] = NULL;
            }
        }
                
        curr = curr->pointers[index];
        i++;
    }

    return 1;
    
}

int addWordToTreeFiltered(t_node* tree, const char* word,int wordLen,char charMatches[N][wordLen],int charOccurrencies[N],char charExact[N]){
    
    
    if(tree == NULL){
        tree = (t_node*)malloc(sizeof(t_node));
        if(tree == NULL){
            return 0;
        }
        tree->hasChild = 0;
        tree->matches = 1;
        for(size_t i=0;i<N;i++){
            tree->pointers[i] = NULL;
        }
    }
    
    int len = (int)strlen(word);
    int i = 0;
    int currentWord[N] = {0};
    t_node* curr = tree;
    
    while(i<len){
        
        char c = word[i];
        int index = charToIndex(c);
        currentWord[index]++;
        
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
            
            if(!charMatches[index][i] || (currentWord[index] > charOccurrencies[index] && charExact[index])){
                curr->pointers[index]->matches = 0;
            }
                        
            for(size_t i=0;i<N;i++){
                curr->pointers[index]->pointers[i] = NULL;
            }
        }
                
        curr = curr->pointers[index];
        i++;
    }

    for(size_t j = 0;j<N;j++){
        if(currentWord[j] < charOccurrencies[j]){
            curr->matches = 0;
            break;
        }
    }
    
    return 1;
    
}
