//
//  main.c
//  WordChecker
//
//  Created by Japo on 04/08/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 64
#define COMMAND_FP "+stampa_filtrate"
#define COMMAND_IS "+inserisci_inizio"
#define COMMAND_IE "+inserisci_fine"
#define COMMAND_NG "+nuova_partita"
#define COMMAND_LEN 17

typedef struct nodo{
    char matches;
    char *prefix;
    struct nodo* son;
    struct nodo* brother;
    
}t_node;

int checkPrefixes(const char* word1, const char* word2){
    int lenEqual = 0;
    for(lenEqual=0;lenEqual<strlen(word1);lenEqual++){
        if(word1[lenEqual] == word2[lenEqual]){
            continue;
        }else if(word1[lenEqual] < word2[lenEqual]){
            return -1 * lenEqual;
        }
        else{
            return lenEqual;
        }
    }
    return lenEqual;
}


void printWordsRecCall(t_node *tree, int wordLen, int step, char *buffer){
    
    if(tree == NULL && step == wordLen){
        
        printf("%s\n",buffer);
        return;
    }
    
    t_node *curr = tree;
    
    while(curr != NULL){
        buffer[step] = '\0';
        strcat(buffer+step, curr->prefix);
        if(curr->matches){
            printWordsRecCall(curr->son,wordLen,strlen(buffer),buffer);
        }
        
        curr = curr->brother;
    }
    
}

void printWords(t_node* tree, int wordLen){
    
    if(tree == NULL || tree->son == NULL){
        return;
    }
    
    char *buffer = calloc(wordLen+1, 1);
    t_node *curr = tree->son;
    
    while(curr != NULL){
        buffer[0] = '\0';
        strcat(buffer, curr->prefix);
        
        if(curr->matches ){
            printWordsRecCall(curr->son,wordLen,strlen(buffer),buffer);
        }
        
        curr = curr->brother;
    }

}

void inserisciInVuoto(t_node *tree, const char *word){
    t_node *new = malloc(sizeof(tree));
    new->brother = NULL;
    new->son = NULL;
    new->matches = 1;
    new->prefix = calloc(strlen(word)+1, 1);
    snprintf(new->prefix, strlen(word)+1, "%s",word);
    tree->son = new;
    return;
}

void inserisciUltimo(t_node *tree, const char *word){
    t_node *new = malloc(sizeof(tree));
    new->brother = NULL;
    new->son = NULL;
    new->matches = 1;
    new->prefix = calloc(strlen(word)+1, 1);
    snprintf(new->prefix, strlen(word)+1, "%s",word);
    tree->brother = new;
    return;
}

void inserisciFratello(t_node *tree,const char *word){
    t_node *new = malloc(sizeof(tree));
    new->son = NULL;
    new->matches = 1;
    new->prefix = calloc(strlen(word)+1, 1);
    snprintf(new->prefix, strlen(word)+1, "%s",word);
    new->brother = tree->brother;
    tree->brother = new;
    return;
}

void inserisciConPrefissoComune(t_node *tree, const char *word, int prefixEnd){
    
    t_node *toChange = tree->brother;
    
    size_t prefixLen = prefixEnd < 0 ? prefixEnd * -1 : prefixEnd;
    char *prefix = calloc(prefixLen+1, 1);
    strncat(prefix, toChange->prefix, prefixLen);
    prefix[prefixLen] = '\0';
    
    size_t suffixLen = strlen(word)+1-prefixLen;
    char *suffix1;
    char *suffix2;
    
    if(prefixEnd > 0){
        suffix1 = calloc(suffixLen, 1);
        suffix2 = calloc(suffixLen, 1);
        snprintf(suffix1, suffixLen, "%s",word+prefixLen);
        snprintf(suffix2, suffixLen, "%s",toChange->prefix+prefixLen);
    }
    else{
        suffix1 = calloc(suffixLen, 1);
        suffix2 = calloc(suffixLen, 1);
        snprintf(suffix1, suffixLen, "%s",toChange->prefix+prefixLen);
        snprintf(suffix2, suffixLen, "%s",word+prefixLen);
    }
    
    free(toChange->prefix);
    toChange->prefix = prefix;
    
    t_node *new1 = malloc(sizeof(t_node));
    t_node *new2 = malloc(sizeof(t_node));
    
    new1->prefix = suffix1;
    new1->matches = 1;
    new1->brother = new2;
    new1->son = NULL;
    
    
    new2->prefix = suffix2;
    new2->matches = 1;
    new2->brother = NULL;
    new2->son = NULL;
    
    toChange->son = new1;
    
    
    return;
    
}

void inserisciPrimoEShifta(t_node *tree,const char* word){
    t_node *new = malloc(sizeof(t_node));
    new->brother = tree->son;
    new->son = NULL;
    new->matches = 1;
    new->prefix = calloc(strlen(word)+1, 1);
    snprintf(new->prefix, strlen(word)+1, "%s",word);
    tree->son = new;
    return;
}

void inserisciESplittaPrimo(t_node *tree,const char* word,int prefixEnd){
    t_node *toChange = tree->son;
    
    size_t prefixLen = prefixEnd < 0 ? prefixEnd * -1 : prefixEnd;
    char *prefix = calloc(prefixLen+1, 1);
    strncat(prefix, toChange->prefix, prefixLen);
    prefix[prefixLen] = '\0';
    
    size_t suffixLen = strlen(word)-prefixLen;
    char *suffix1;
    char *suffix2;
    
    if(prefixEnd > 0){
        suffix1 = calloc(suffixLen+1, 1);
        suffix2 = calloc(suffixLen+1, 1);
        strncat(suffix1, word+prefixLen, suffixLen);
        strncat(suffix2, toChange->prefix+prefixLen, suffixLen);
        
        //snprintf(suffix1, suffixLen, "%s",word+prefixEnd);
        //snprintf(suffix2, suffixLen, "%s",toChange->prefix+prefixEnd);
    }
    else{
        suffix1 = calloc(suffixLen+1, 1);
        suffix2 = calloc(suffixLen+1, 1);
        //snprintf(suffix1, suffixLen, "%s",toChange->prefix+prefixEnd);
        //snprintf(suffix2, suffixLen, "%s",word+prefixEnd);
        
        strncat(suffix1, toChange->prefix+prefixLen, suffixLen);
        strncat(suffix2, word+prefixLen, suffixLen);
    }
    
    free(toChange->prefix);
    toChange->prefix = prefix;
    
    t_node *new1 = malloc(sizeof(t_node));
    t_node *new2 = malloc(sizeof(t_node));
    
    new1->prefix = suffix1;
    new1->matches = 1;
    new1->brother = new2;
    new1->son = NULL;
    
    
    new2->prefix = suffix2;
    new2->matches = 1;
    new2->brother = NULL;
    new2->son = NULL;
    
    toChange->son = new1;
    
    
    return;
}

void insertNextPart(t_node *tree, const char *word){
    
    /*if(tree == NULL){
        inserisciInVuoto(tree, word);
        return;
    }*/
    
    int chkCmp = strcmp(tree->son->prefix, word);
    int chkPrefix = checkPrefixes(tree->son->prefix, word);

    if(chkCmp > 0 && chkPrefix == 0){
        inserisciPrimoEShifta(tree,word);
        return;
    }
    else if(chkPrefix != 0){
        inserisciESplittaPrimo(tree,word,chkPrefix);
        return;
    }
    
    t_node *curr = tree->son;
    
    while(curr->brother != NULL && strcmp(curr->brother->prefix, word) < 0 && checkPrefixes(curr->brother->prefix, word) == 0){
        curr = curr->brother;
    }
    
    if(curr->brother == NULL){
        inserisciUltimo(curr,word);
    }
    else if(strcmp(curr->brother->prefix, word) > 0 && checkPrefixes(curr->brother->prefix, word) == 0){
        inserisciFratello(curr, word);
    }
    else if(checkPrefixes(curr->brother->prefix, word) == strlen(curr->brother->prefix)){
        insertNextPart(curr->brother, word+strlen(curr->brother->prefix));
    }
    else{
        inserisciConPrefissoComune(curr,word, checkPrefixes(curr->brother->prefix, word));
    }
    
}

void insertWord(t_node *tree, const char *word){
    
    if(tree->son == NULL){
        inserisciInVuoto(tree, word);
        return;
    }
    
    int chkCmp = strcmp(tree->son->prefix, word);
    int chkPrefix = checkPrefixes(tree->son->prefix, word);

    if(chkCmp > 0 && chkPrefix == 0){
        inserisciPrimoEShifta(tree,word);
        return;
    }
    else if(chkPrefix != 0){
        inserisciESplittaPrimo(tree,word,chkPrefix);
        return;
    }
    
    t_node *curr = tree->son;
    if(curr->brother != NULL){
        chkCmp = strcmp(curr->brother->prefix, word);
        chkPrefix = checkPrefixes(curr->brother->prefix, word);
    }
    
    
    while(curr->brother != NULL && chkCmp < 0 && chkPrefix == 0){
        curr = curr->brother;
        if(curr->brother != NULL){
            chkCmp = strcmp(curr->brother->prefix, word);
            chkPrefix = checkPrefixes(curr->brother->prefix, word);
        }
    }
    
    if(curr->brother == NULL){
        inserisciUltimo(curr,word);
    }
    else if(chkCmp > 0 && chkPrefix == 0){
        inserisciFratello(curr, word);
    }
    else if(chkPrefix == strlen(curr->brother->prefix)){
        insertNextPart(curr->brother, word+strlen(curr->brother->prefix));
    }
    else{
        inserisciConPrefissoComune(curr,word, chkPrefix);
    }
    
}



int main(int argc, const char * argv[]) {
    // insert code here...
    int wordLen = 6;
    
    
    
    t_node *tree = malloc(sizeof(t_node));
    tree->matches = 1;
    tree->prefix = NULL;
    tree->son = NULL;
    tree->brother = NULL;
    
    insertWord(tree, "aurora");
    insertWord(tree, "bevuta");
    insertWord(tree, "divano");
    insertWord(tree, "davide");
    insertWord(tree, "banana");
    insertWord(tree, "botola");
    
    printWords(tree, wordLen);
    
    
    return 0;
}
