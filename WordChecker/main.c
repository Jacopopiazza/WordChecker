//
//  main.c
//  TestBase
//
//  Created by Japo on 01/08/22.
//

#include <stdio.h>
#include <stdlib.h>
#include "Dictionary.h"

#define COMMAND_FP "+stampa_filtrate"
#define COMMAND_IS "+inserisci_inizio"
#define COMMAND_IE "+inserisci_fine"
#define COMMAND_NG "+nuova_partita"
#define COMMAND_LEN 17


void print(const char* str){
    if(DEBUG){
        printf("%s",str);
    }
}



void filterTreeCompletelyRecCall(t_node *tree,const char* toBeCheckedWord,const char* buffer,int wordLen,char charMatches[N][wordLen],int charOccurrencies[N],char charExact[N],int step,int currentWord[N],int *validWords){
    
    if(tree == NULL || !tree->matches){
        return;
    }
    
    if(step == wordLen && !tree->hasChild){
        
        for(size_t i=0;i<N;i++){
            if(currentWord[i] < charOccurrencies[i]){
                
                tree->matches = 0;
                return;
            }
        }
        
        if(tree->matches){
            (*validWords)++;
            return;
        }
    }
    
    for(size_t i = 0;i<N;i++){
        if(tree->pointers[i] != NULL && (!charMatches[i][step] || (currentWord[i]+1>charOccurrencies[i] && charExact[i]) )){
            tree->pointers[i]->matches = 0;
            continue;
        }
        else{
            currentWord[i]++;
            filterTreeCompletelyRecCall(tree->pointers[i],toBeCheckedWord,buffer,wordLen,charMatches,charOccurrencies,charExact, step+1,currentWord,validWords);
            currentWord[i]--;
        }
    }
}

void filterTreeCompletely(t_node *tree,const char* toBeCheckedWord,const char* buffer,int wordLen,char charMatches[N][wordLen],int charOccurrencies[N],char charExact[N]){
    

    
    int step = 0;
    
    int *validWords = calloc(1, sizeof(int));

    if(tree == NULL || !tree->matches || tree->hasChild == 0){
        return;
    }

    for(size_t i = 0;i<N;i++){
        int currentWord[N] = {0};
        if(tree->pointers[i] != NULL && (!charMatches[i][step] || (currentWord[i]+1>charOccurrencies[i] && charExact[i]) )){
            tree->pointers[i]->matches = 0;
            
            continue;
        }
        else{
            currentWord[i]++;
            filterTreeCompletelyRecCall(tree->pointers[i],toBeCheckedWord,buffer,wordLen,charMatches,charOccurrencies,charExact, step+1,currentWord, validWords);
        }
    }
    
    printf("%d\n",*validWords);
    
}


int checkWord(const char* refWord,const char* toBeCheckedWord,int wordLen,char charMatches[N][wordLen],int charOccurrencies[N],char charExact[N],t_node *tree){
    
    if(tree == NULL){
        return -2;
    }
    
    char *buffer = calloc(1,wordLen+1);
    char *used = calloc(1, wordLen);
    int currentOccurrencies[N] = {0};
    char currentExact[N] = {0};
    
    char won = 1;
    
    t_node *curr = tree;
    for(size_t i = 0;i<wordLen;i++){
        
        if(curr->hasChild && curr->pointers[charToIndex(toBeCheckedWord[i])] == NULL){
            free(buffer);
            free(used);
            printf("not_exists\n");
            return -1;
        }
        
        buffer[i] = '/';
        if(refWord[i] == toBeCheckedWord[i]){
            buffer[i] = '+';
            used[i] = 1;
            currentOccurrencies[charToIndex(toBeCheckedWord[i])]++;
            
            for(size_t j=0;j<N;j++){
                charMatches[j][i] = 0;
            }
            charMatches[charToIndex(toBeCheckedWord[i])][i] = 1;
        }
        else{
            won = 0;
            charMatches[charToIndex(toBeCheckedWord[i])][i] = 0;
        }
        
        curr = curr->pointers[charToIndex(toBeCheckedWord[i])];
    }
    
    if(won){
        //Ha vinto
        free(buffer);
        free(used);
        printf("ok\n");
        return 1;
    }
    
    for(size_t i=0;i<wordLen;i++){
        size_t index = charToIndex(toBeCheckedWord[i]);
        if(buffer[i] != '+'){ //Non corretto
            size_t j = 0;
            for(j=0;j<wordLen;j++){
                if(toBeCheckedWord[i] == refWord[j] && !used[j]){//Ho un fuori posizione
                    buffer[i] = '|';
                    used[j] = 1;
                    currentOccurrencies[index]++;
                    break;
                }
            }
            
            if(j == wordLen ){///Non ho *altre* occorrenze libere della lettera che sto cercando => '\'
                currentExact[index] = 1;
            }
            
        }
    }
    
    for(size_t i=0;i<N;i++){
        if(currentExact[i]){
            charOccurrencies[i] = currentOccurrencies[i];
            charExact[i] = 1;
        }
        else if(currentOccurrencies[i] > charOccurrencies[i] && !charExact[i]){
            charOccurrencies[i] = currentOccurrencies[i];
        }
    }
    
    
    printf("%s\n",buffer);
    
    //Filtro in un passo unico
    filterTreeCompletely(tree, toBeCheckedWord, buffer, wordLen, charMatches, charOccurrencies, charExact);
    
    
    free(buffer);
    free(used);
    
    return 0;
}

void cleanTreeMatches(t_node *tree, int wordLen,int step){
    if(tree == NULL){
        return;
    }
    
    tree->matches = 1;
    for(size_t i=0;i<N;i++){
        if(tree->pointers[i] != NULL){
            cleanTreeMatches(tree->pointers[i], wordLen, step+1);
        }
    }
    
}

void newGame(t_node *tree, int wordLen,char charMatches[N][wordLen],int charOccurrencies[N],char charExact[N]){
    for(size_t i=0;i<N;i++){
        charOccurrencies[i] = 0;
        charExact[i] = 0;
        for(size_t j=0;j<wordLen;j++){
            charMatches[i][j] = 1;
        }
    }
    
    cleanTreeMatches(tree,wordLen,0);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    
    int wordLen = -1;
    scanf("%d", &wordLen);
    
    //Creo e inizializzo strutture di controllo
    char charMatches[N][wordLen];
    int charOccurrencies[N] = {0};
    char charExact[N] = {0};
    
    for(size_t i=0;i<N;i++){
        for(size_t j=0;j<wordLen;j++){
            charMatches[i][j] = 1;
        }
    }
    
    //Alloco albero
    t_node *tree = (t_node *)malloc(sizeof(t_node));
    tree->matches = 1;
    tree->hasChild = 0;
    for(size_t i=0;i<N;i++){
        tree->pointers[i] = NULL;
    }
    
    char *buffer;
    if(wordLen > COMMAND_LEN){
        buffer = calloc(wordLen+1, 1);
    }
    else{
        buffer = calloc(COMMAND_LEN+1, 1);
    }
    
    while(1){
        scanf("%s",buffer);
        if(strcmp(buffer, COMMAND_NG) == 0){
            break;
        }
        addWordToTree(tree, buffer);
    }
    
    char new_game = 1;
    int tries = - 1;
    char won = 0;
    char *refWord = calloc(wordLen+1, 1);
    while(new_game){
        new_game = 0;
        won = 0;
        
        scanf("%s",refWord);
        scanf("%d",&tries);
        
        while(!won && tries > 0){
            scanf("%s",buffer);
            if(strcmp(buffer, COMMAND_IS) == 0){
                
                while(1){
                    scanf("%s",buffer);
                    if(strcmp(buffer, COMMAND_IE) == 0){
                        break;
                    }
                    addWordToTreeFiltered(tree, buffer, wordLen, charMatches, charOccurrencies, charExact);
                }
            }
            else if(strcmp(buffer, COMMAND_FP) == 0){
                printWords(tree, wordLen);
            }
            else{
                size_t res = checkWord(refWord, buffer, wordLen, charMatches, charOccurrencies, charExact, tree);
                if(res == 1){
                    won=1;
                    break;
                }
                else if(res == 0){
                    tries--;
                }
                
            }
        }
        
        if(!won && tries == 0){
            printf("ko\n");
        }
        
        while(1){
            scanf("%s",buffer);
            if(strcmp(buffer, COMMAND_NG) == 0){
                new_game = 1;
                //Clean tree and filters
                newGame(tree, wordLen, charMatches, charOccurrencies, charExact);
                break;
            }
            else if(strcmp(buffer, COMMAND_IS) == 0){
                while(1){
                    scanf("%s",buffer);
                    if(strcmp(buffer, COMMAND_IE) == 0){
                        break;
                    }
                    addWordToTree(tree, buffer);
                }
            }
            else{
                break;
            }
        }
        
        
    }
    
    free(refWord);
    free(buffer);
    
    return 0;
    
    /*
    int wordLen = 5;
    int tries = 4;
    
    char charMatches[N][wordLen];
    int charOccurrencies[N] = {0};
    char charExact[N] = {0};
    
    for(size_t i=0;i<N;i++){
        for(size_t j=0;j<wordLen;j++){
            charMatches[i][j] = 1;
        }
    }
    
    
    t_node *tree = (t_node *)malloc(sizeof(t_node));
    tree->matches=1;
    for(size_t i=0;i<N;i++){
        tree->pointers[i] = NULL;
    }
    char* refWord = "5sjaH";
    
    addWordToTree(tree, "8adfs");
    addWordToTree(tree, "5sjaH");
    addWordToTree(tree, "KS06l");
    addWordToTree(tree, "Hi23a");
    addWordToTree(tree, "laj74");
    addWordToTree(tree, "-s9k0");
    addWordToTree(tree, "sm_ks");
    addWordToTree(tree, "okauE");
    
    //printWords(tree, wordLen);
    
    checkWord(refWord, "KS06l", wordLen, charMatches, charOccurrencies, charExact, tree);
    checkWord(refWord, "had7s", wordLen, charMatches, charOccurrencies, charExact, tree);
    checkWord(refWord, "okauE", wordLen, charMatches, charOccurrencies, charExact, tree);
    
    printWords(tree, wordLen);
 
    
    addWordToTreeFiltered(tree, "PsjW5", wordLen, charMatches, charOccurrencies, charExact);
    addWordToTreeFiltered(tree, "asHdd", wordLen, charMatches, charOccurrencies, charExact);
    addWordToTreeFiltered(tree, "paF7s", wordLen, charMatches, charOccurrencies, charExact);
    
    checkWord(refWord, "-s9k0", wordLen, charMatches, charOccurrencies, charExact, tree);
    checkWord(refWord, "sghks", wordLen, charMatches, charOccurrencies, charExact, tree);

    printWords(tree, wordLen);
    
    checkWord(refWord, "sm_ks", wordLen, charMatches, charOccurrencies, charExact, tree);
    
    printWords(tree, wordLen);
     */

}


