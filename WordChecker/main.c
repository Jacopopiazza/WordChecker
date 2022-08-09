//
//  main.c
//  PREFIX TRIE
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

char isWordInTree(t_node *tree,const char *word){
    
    if(tree == NULL){
        return 0;
    }
    
    if(tree->son == NULL){
        return 0;
    }
    
    t_node *curr = tree->son;
    int chkCmp = strcmp(curr->prefix, word);
    
    if(chkCmp == 0){
        return 1;
    }
    else if(chkCmp > 0){
        return 0;
    }
    int chkPrefix = checkPrefixes(curr->prefix, word);
    if(chkPrefix != 0){
        //Hanno prefisso comune, altrimenti cerco tra i fratelli
        if(abs(chkPrefix) == strlen(curr->prefix)){
            //Corrisponde il prefisso
            return isWordInTree(curr, word+strlen(curr->prefix));
        }
        return 0;
    }
    
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
    
    //Sei in fondo, o all'ultimo con prefisso precedente o al tuo prefisso
    if(chkCmp == 0){
        return 1;
    }
    else if(chkCmp > 0 && chkPrefix == 0){
        return 0;
    }
    //chkPrefix = checkPrefixes(curr->brother->prefix, word);
    if(chkPrefix != 0){
        //Hanno prefisso comune, altrimenti cerco tra i fratelli
        if(abs(chkPrefix) == strlen(curr->brother->prefix)){
            //Corrisponde il prefisso
            return isWordInTree(curr->brother, word+strlen(curr->brother->prefix));
        }
        return 0;
    }
    
    
    return 0;
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


void iterativeInsertWordFiltered(t_node *tree,const char *word,int wordLen,u_short charOccurrencies[N],char charExact[N],char charMatches[N][wordLen]){

    t_node *new;
    t_node *last=NULL;
    
    if(tree->son == NULL){//Albero Vuoto
        new = malloc(sizeof(t_node));
        new->brother = NULL;
        new->son = NULL;
        new->matches = 1;
        new->prefix = calloc(strlen(word)+1, 1);
        snprintf(new->prefix, strlen(word)+1, "%s",word);
        tree->son = new;
        
        for(size_t i=0;i<wordLen;i++){
            if(!charMatches[charToIndex(word[i])][i]){
                new->matches = 0;
                break;
            }
        }
        
        last = new;
    }
    else{
        int step = 0;
        t_node *curr = tree;
        char *buffer = calloc(wordLen+1, 1);
        strcpy(buffer, word);

        
        while(step < wordLen){
            int chkCmp = strcmp(curr->son->prefix, buffer);
            int chkPrefix = checkPrefixes(curr->son->prefix, buffer);
            
            if(chkCmp > 0 && chkPrefix == 0){
                new = malloc(sizeof(t_node));
                new->brother = curr->son;
                new->son = NULL;
                new->matches = 1;
                new->prefix = calloc(strlen(buffer)+1, 1);
                snprintf(new->prefix, strlen(buffer)+1, "%s",buffer);
                curr->son = new;
                
                last = new;
                
                for(size_t i=step;i<wordLen;i++){
                    if(!charMatches[charToIndex(word[i-step])][i]){
                        new->matches = 0;
                        break;
                    }
                }
                
                break;
            }
            else if(chkPrefix != 0 && abs(chkPrefix) != strlen(curr->son->prefix)){
                t_node *toChange = curr->son;
                
                size_t prefixLen = chkPrefix < 0 ? chkPrefix * -1 : chkPrefix;
                char *prefix = calloc(prefixLen+1, 1);
                strncat(prefix, toChange->prefix, prefixLen);
                prefix[prefixLen] = '\0';
                
                size_t suffixLen = strlen(buffer)-prefixLen;
                char *suffix1;
                char *suffix2;
                
                if(chkPrefix > 0){
                    suffix1 = calloc(suffixLen+1, 1);
                    suffix2 = calloc(suffixLen+1, 1);
                    strncat(suffix1, buffer+prefixLen, suffixLen);
                    strncat(suffix2, toChange->prefix+prefixLen, suffixLen);
        
                }
                else{
                    suffix1 = calloc(suffixLen+1, 1);
                    suffix2 = calloc(suffixLen+1, 1);
                    strncat(suffix1, toChange->prefix+prefixLen, suffixLen);
                    strncat(suffix2, buffer+prefixLen, suffixLen);
                }
                
                free(toChange->prefix);
                toChange->prefix = prefix;
                
                for(size_t i=0;i<prefixLen;i++){
                    if(!charMatches[charToIndex(toChange->prefix[i])][i+step]){
                        toChange->matches = 0;
                        break;
                    }
                }
                
                t_node *new1 = malloc(sizeof(t_node));
                t_node *new2 = malloc(sizeof(t_node));
                
                if(chkPrefix > 0){
                    last = new1;
                }
                else{
                    last = new2;
                }
                
                new1->prefix = suffix1;
                new1->matches = 1;
                new1->brother = new2;
                new1->son = NULL;
                
                new2->prefix = suffix2;
                new2->matches = 1;
                new2->brother = NULL;
                new2->son = NULL;
                
                if(chkPrefix > 0){
                    if(toChange->son != NULL){
                        new1->son = toChange->son;
                    }
                }
                else{
                    if(toChange->son != NULL){
                        new2->son = toChange->son;
                    }
                }

                toChange->son = new1;

                for(size_t i=0;i<suffixLen;i++){
                    if(!charMatches[charToIndex(new1->prefix[i])][i+step+prefixLen]){
                        new2->matches = 0;
                    }
                    if(!charMatches[charToIndex(new2->prefix[i])][i+step+prefixLen]){
                        new1->matches = 0;
                    }
                }
                break;
            }
            else if(chkPrefix != 0 && abs(chkPrefix) == strlen(curr->son->prefix)){
                curr = curr->son;
                buffer[0] = '\0';
                step += strlen(curr->prefix);
                strcpy(buffer, (word+step));
                continue;
            }
            
            curr = curr->son;
            if(curr->brother != NULL){
                chkCmp = strcmp(curr->brother->prefix, buffer);
                chkPrefix = checkPrefixes(curr->brother->prefix, buffer);
            }
            
            while(curr->brother != NULL && chkCmp < 0 && chkPrefix == 0){
                curr = curr->brother;
                if(curr->brother != NULL){
                    chkCmp = strcmp(curr->brother->prefix, buffer);
                    chkPrefix = checkPrefixes(curr->brother->prefix, buffer);
                }
            }
            
            if(curr->brother == NULL){
                new = malloc(sizeof(t_node));
                new->brother = NULL;
                new->son = NULL;
                new->matches = 1;
                new->prefix = calloc(strlen(buffer)+1, 1);
                snprintf(new->prefix, strlen(buffer)+1, "%s",buffer);
                curr->brother = new;
                
                last = new;
                
                for(size_t i=step;i<wordLen;i++){
                    if(!charMatches[charToIndex(word[i-step])][i]){
                        new->matches = 0;
                        break;
                    }
                }
                
                break;
            }
            else if(chkCmp > 0 && chkPrefix == 0){
                
                new = malloc(sizeof(t_node));
                new->son = NULL;
                new->matches = 1;
                new->prefix = calloc(strlen(buffer)+1, 1);
                snprintf(new->prefix, strlen(buffer)+1, "%s",buffer);
                new->brother = curr->brother;
                curr->brother = new;
                
                last = new;
                
                for(size_t i=step;i<wordLen;i++){
                    if(!charMatches[charToIndex(word[i-step])][i]){
                        new->matches = 0;
                        break;
                    }
                }
                break;
                
            }
            else if(abs(chkPrefix) == strlen(curr->brother->prefix)){
                curr = curr->brother;
                buffer[0] = '\0';
                step += strlen(curr->prefix);
                strcpy(buffer, word+step);

            }
            else{
                
                t_node *toChange = curr->brother;
                
                size_t prefixLen = chkPrefix < 0 ? chkPrefix * -1 : chkPrefix;
                char *prefix = calloc(prefixLen+1, 1);
                strncat(prefix, toChange->prefix, prefixLen);
                prefix[prefixLen] = '\0';
                
                size_t suffixLen = strlen(buffer)+1-prefixLen;
                char *suffix1;
                char *suffix2;
                
                if(chkPrefix > 0){
                    suffix1 = calloc(suffixLen, 1);
                    suffix2 = calloc(suffixLen, 1);
                    snprintf(suffix1, suffixLen, "%s",buffer+prefixLen);
                    snprintf(suffix2, suffixLen, "%s",toChange->prefix+prefixLen);
                }
                else{
                    suffix1 = calloc(suffixLen, 1);
                    suffix2 = calloc(suffixLen, 1);
                    snprintf(suffix1, suffixLen, "%s",toChange->prefix+prefixLen);
                    snprintf(suffix2, suffixLen, "%s",buffer+prefixLen);
                }
                
                free(toChange->prefix);
                toChange->prefix = prefix;
                
                for(size_t i=0;i<prefixLen;i++){
                    if(!charMatches[charToIndex(toChange->prefix[i])][i+step]){
                        toChange->matches = 0;
                        break;
                    }
                }
                
                t_node *new1 = malloc(sizeof(t_node));
                t_node *new2 = malloc(sizeof(t_node));
                
                if(chkPrefix > 0){
                    last = new1;
                }
                else{
                    last = new2;
                }
                
                new1->prefix = suffix1;
                new1->matches = 1;
                new1->brother = new2;
                new1->son = NULL;
                
                
                new2->prefix = suffix2;
                new2->matches = 1;
                new2->brother = NULL;
                new2->son = NULL;
                
                if(chkPrefix > 0){
                    if(toChange->son != NULL){
                        new1->son = toChange->son;
                    }
                }
                else{
                    if(toChange->son != NULL){
                        new2->son = toChange->son;
                    }
                }
                
                toChange->son = new1;
                
                for(size_t i=0;i<suffixLen;i++){
                    if(!charMatches[charToIndex(new1->prefix[i])][i+step+prefixLen]){
                        new2->matches = 0;
                    }
                    if(!charMatches[charToIndex(new2->prefix[i])][i+step+prefixLen]){
                        new1->matches = 0;
                    }
                }

                break;
            }
        }
        
        free(buffer);
    }
    
    u_short currentWord[N] = {0};
    for(size_t i=0;i<wordLen;i++){
        currentWord[charToIndex(word[i])]++;
    }
    
    for(size_t j=0;j<N;j++){
        if(currentWord[j] < charOccurrencies[j]){
            last->matches = 0;
        }
    }
}

void iterativeInsertWord(t_node *tree, const char *word){
        
    t_node *new;
    int wordLen = strlen(word);

    
    if(tree->son == NULL){//Albero Vuoto
        new = malloc(sizeof(t_node));
        new->brother = NULL;
        new->son = NULL;
        new->matches = 1;
        new->prefix = calloc(strlen(word)+1, 1);
        snprintf(new->prefix, strlen(word)+1, "%s",word);
        tree->son = new;
        
    }
    else{
        int step = 0;
        t_node *curr = tree;
        char *buffer = calloc(wordLen+1, 1);
        strcpy(buffer, word);

        
        while(step < wordLen){
            int chkCmp = strcmp(curr->son->prefix, buffer);
            int chkPrefix = checkPrefixes(curr->son->prefix, buffer);
            
            if(chkCmp > 0 && chkPrefix == 0){
                new = malloc(sizeof(t_node));
                new->brother = curr->son;
                new->son = NULL;
                new->matches = 1;
                new->prefix = calloc(strlen(buffer)+1, 1);
                snprintf(new->prefix, strlen(buffer)+1, "%s",buffer);
                curr->son = new;

                break;
            }
            else if(chkPrefix != 0 && abs(chkPrefix) != strlen(curr->son->prefix)){
                t_node *toChange = curr->son;
                
                size_t prefixLen = chkPrefix < 0 ? chkPrefix * -1 : chkPrefix;
                char *prefix = calloc(prefixLen+1, 1);
                strncat(prefix, toChange->prefix, prefixLen);
                prefix[prefixLen] = '\0';
                
                size_t suffixLen = strlen(buffer)-prefixLen;
                char *suffix1;
                char *suffix2;
                
                if(chkPrefix > 0){
                    suffix1 = calloc(suffixLen+1, 1);
                    suffix2 = calloc(suffixLen+1, 1);
                    strncat(suffix1, buffer+prefixLen, suffixLen);
                    strncat(suffix2, toChange->prefix+prefixLen, suffixLen);
        
                }
                else{
                    suffix1 = calloc(suffixLen+1, 1);
                    suffix2 = calloc(suffixLen+1, 1);
                    strncat(suffix1, toChange->prefix+prefixLen, suffixLen);
                    strncat(suffix2, buffer+prefixLen, suffixLen);
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
                
                if(chkPrefix > 0){
                    if(toChange->son != NULL){
                        new2->son = toChange->son;
                    }
                }
                else{
                    if(toChange->son != NULL){
                        new1->son = toChange->son;
                    }
                }

                toChange->son = new1;

                break;
            }
            else if(chkPrefix != 0 && abs(chkPrefix) == strlen(curr->son->prefix)){
                curr = curr->son;
                buffer[0] = '\0';
                step += strlen(curr->prefix);
                strcpy(buffer, (word+step));
                continue;
            }
            
            curr = curr->son;
            if(curr->brother != NULL){
                chkCmp = strcmp(curr->brother->prefix, buffer);
                chkPrefix = checkPrefixes(curr->brother->prefix, buffer);
            }
            
            while(curr->brother != NULL && chkCmp < 0 && chkPrefix == 0){
                curr = curr->brother;
                if(curr->brother != NULL){
                    chkCmp = strcmp(curr->brother->prefix, buffer);
                    chkPrefix = checkPrefixes(curr->brother->prefix, buffer);
                }
            }
            
            if(curr->brother == NULL){
                new = malloc(sizeof(t_node));
                new->brother = NULL;
                new->son = NULL;
                new->matches = 1;
                new->prefix = calloc(strlen(buffer)+1, 1);
                snprintf(new->prefix, strlen(buffer)+1, "%s",buffer);
                curr->brother = new;
                
                break;
            }
            else if(chkCmp > 0 && chkPrefix == 0){
                
                new = malloc(sizeof(t_node));
                new->son = NULL;
                new->matches = 1;
                new->prefix = calloc(strlen(buffer)+1, 1);
                snprintf(new->prefix, strlen(buffer)+1, "%s",buffer);
                new->brother = curr->brother;
                curr->brother = new;
                break;
            }
            else if(abs(chkPrefix) == strlen(curr->brother->prefix)){
                curr = curr->brother;
                buffer[0] = '\0';
                step += strlen(curr->prefix);
                strcpy(buffer, word+step);

            }
            else{
                
                t_node *toChange = curr->brother;
                
                size_t prefixLen = chkPrefix < 0 ? chkPrefix * -1 : chkPrefix;
                char *prefix = calloc(prefixLen+1, 1);
                strncat(prefix, toChange->prefix, prefixLen);
                prefix[prefixLen] = '\0';
                
                size_t suffixLen = strlen(buffer)+1-prefixLen;
                char *suffix1;
                char *suffix2;
                
                if(chkPrefix > 0){
                    suffix1 = calloc(suffixLen, 1);
                    suffix2 = calloc(suffixLen, 1);
                    snprintf(suffix1, suffixLen, "%s",buffer+prefixLen);
                    snprintf(suffix2, suffixLen, "%s",toChange->prefix+prefixLen);
                }
                else{
                    suffix1 = calloc(suffixLen, 1);
                    suffix2 = calloc(suffixLen, 1);
                    snprintf(suffix1, suffixLen, "%s",toChange->prefix+prefixLen);
                    snprintf(suffix2, suffixLen, "%s",buffer+prefixLen);
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
                
                if(chkPrefix > 0){
                    if(toChange->son != NULL){
                        new2->son = toChange->son;
                    }
                }
                else{
                    if(toChange->son != NULL){
                        new1->son = toChange->son;
                    }
                }
                
                toChange->son = new1;

                break;
            }
        }
        
        free(buffer);
    }
}







void filterTreeRecCall(t_node *tree,int wordLen,u_short charOccurrencies[N],char charExact[N],char charMatches[N][wordLen],int step,u_short currentWord[N],int *validWords){


    if(tree == NULL || !tree->matches){
        return;
    }
    
    if(step == wordLen && tree->son == NULL){
        
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
        
    t_node *curr = tree->son;
    char filter = 1;
    
    while(curr != NULL){
        
        filter = 1;
        u_short currentWord[N] = {0};
        
        for(size_t i=0;i<strlen(curr->prefix);i++){
            int index = charToIndex(curr->prefix[i]);
            if(!charMatches[index][step+i] || (currentWord[index]+1 > charOccurrencies[index] && charExact[index]) ){
                curr->matches = 0;
                filter = 0;
                break;
            }
            else{
                currentWord[index]++;
            }
        }
        if(filter){
            filterTreeRecCall(curr, wordLen, charOccurrencies, charExact, charMatches, step+strlen(curr->prefix), currentWord,validWords);
        }

        curr = curr->brother;
    }
    
}

void filterTree(t_node *tree,int wordLen,u_short charOccurrencies[N],char charExact[N],char charMatches[N][wordLen]){
    if(tree == NULL || tree->son == NULL){
        return;
    }
    
    int step = 0;
    int *validWords = calloc(1, sizeof(int));
    char filter = 1;
    
    t_node *curr = tree->son;
    
    
    while(curr != NULL){
        filter = 1;
        u_short currentWord[N] = {0};
        
        for(size_t i=0;i<strlen(curr->prefix);i++){
            int index = charToIndex(curr->prefix[i]);
            if(!charMatches[index][step+i] || (currentWord[index]+1 > charOccurrencies[index] && charExact[index]) ){
                curr->matches = 0;
                filter = 0;
                break;
            }
            else{
                currentWord[index]++;
            }
        }
        if(filter){
            filterTreeRecCall(curr, wordLen, charOccurrencies, charExact, charMatches, step+strlen(curr->prefix), currentWord,validWords);
        }

        curr = curr->brother;
    }
    
    
    printf("%d\n",*validWords);
    free(validWords);
    
}

int checkWord(const char *refWord,const char* toBeCheckedWord,int wordLen,t_node* tree, u_short charOccurrencies[N], char charExact[N], char charMatches[N][wordLen]){
    
    
    if(!isWordInTree(tree, toBeCheckedWord)){
        printf("not_exists\n");
        return -1;
    }
    
    char *buffer = calloc(wordLen+1, 1);
    char won = 1;
    
    u_short freeOccurrencies[N] = {0};
    u_short currentWord[N] = {0};
    t_node *curr = tree;
    
    
    for(size_t i = 0;i<wordLen;i++){
        
        int index = charToIndex(toBeCheckedWord[i]);
        
        if(refWord[i] == toBeCheckedWord[i]){
            buffer[i] = '+';
            currentWord[index]++;
        }
        else{
            buffer[i] = '/';
            won = 0;
            freeOccurrencies[charToIndex(refWord[i])]++;
        }
        
    }
    
    if(won){
        free(buffer);
        printf("ok\n");
        return 1;
    }
    
    for(size_t i=0;i<wordLen;i++){
        switch(buffer[i]){
            case '+':
                for(size_t j=0;j<N;j++){
                    charMatches[j][i] = 0;
                }
                charMatches[charToIndex(toBeCheckedWord[i])][i]=1;
                break;
            default:
                charMatches[charToIndex(toBeCheckedWord[i])][i]=0;
                if(freeOccurrencies[charToIndex(toBeCheckedWord[i])] > 0){
                    buffer[i] = '|';
                    freeOccurrencies[charToIndex(toBeCheckedWord[i])]--;
                    charOccurrencies[charToIndex(toBeCheckedWord[i])]++;
                }
                else{
                    charExact[charToIndex(toBeCheckedWord[i])] = 1;
                }
                break;
        }
    }
    
    printf("%s\n",buffer);
    //FILTER
    filterTree(tree, wordLen, charOccurrencies, charExact, charMatches);
    
    free(buffer);
    
    return 0;
}

void cleanTreeMatches(t_node *tree, int wordLen,int step){
    if(tree == NULL){
        return;
    }
    
    t_node *curr = tree->son;
    
    while(curr != NULL){
        curr->matches = 1;
        cleanTreeMatches(curr, wordLen, step+1);
        curr = curr->brother;
    }
    
    
}

void newGame(t_node *tree, int wordLen,char charMatches[N][wordLen],u_short charOccurrencies[N],char charExact[N]){
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
    
    char nullScanfValue;
    int wordLen = -1;
    nullScanfValue=scanf("%d", &wordLen);
    
    //Creo e inizializzo strutture di controllo
    char charMatches[N][wordLen];
    u_short charOccurrencies[N] = {0};
    char charExact[N] = {0};
    
    for(size_t i=0;i<N;i++){
        for(size_t j=0;j<wordLen;j++){
            charMatches[i][j] = 1;
        }
    }
    
    //Alloco albero
    t_node *tree = malloc(sizeof(t_node));
    tree->matches = 1;
    tree->prefix = NULL;
    tree->son = NULL;
    tree->brother = NULL;
    
    char *buffer;
    if(wordLen > COMMAND_LEN){
        buffer = calloc(wordLen+1, 1);
    }
    else{
        buffer = calloc(COMMAND_LEN+1, 1);
    }

    while(1){
        nullScanfValue=scanf("%s",buffer);
        if(strcmp(buffer, COMMAND_NG) == 0){
            break;
        }
        
        //insertWord(tree, buffer);
        iterativeInsertWord(tree, buffer);
        //addWordToTree(tree, buffer);
    }
    
   
    
    char new_game = 1;
    
    int tries = - 1;
    char won = 0;
    char *refWord = calloc(wordLen+1, 1);
    while(new_game){
        new_game = 0;
        won = 0;
        
        nullScanfValue=scanf("%s",refWord);
        nullScanfValue=scanf("%d",&tries);
        
        while(!won && tries > 0){
            nullScanfValue=scanf("%s",buffer);
            if(strcmp(buffer, COMMAND_IS) == 0){
                
                while(1){
                    nullScanfValue=scanf("%s",buffer);
                    if(strcmp(buffer, COMMAND_IE) == 0){
                        break;
                    }
                    //addWordToTreeFiltered(tree, buffer, wordLen, charMatches, charOccurrencies, charExact);
                    //insertWordFiltered(tree, buffer, wordLen, charOccurrencies, charExact, charMatches);
                    iterativeInsertWordFiltered(tree, buffer, wordLen, charOccurrencies, charExact, charMatches);
                }
            }
            else if(strcmp(buffer, COMMAND_FP) == 0){
                printWords(tree, wordLen);
            }
            else{
                size_t res = checkWord(refWord, buffer, wordLen, tree, charOccurrencies, charExact, charMatches);
                //size_t res = checkWord(refWord, buffer, wordLen, charMatches, charOccurrencies, charExact, tree);
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
            nullScanfValue=scanf("%s",buffer);
            if(strcmp(buffer, COMMAND_NG) == 0){
                new_game = 1;
                //Clean tree and filters
                newGame(tree, wordLen, charMatches, charOccurrencies, charExact);
                break;
            }
            else if(strcmp(buffer, COMMAND_IS) == 0){
                while(1){
                    nullScanfValue=scanf("%s",buffer);
                    if(strcmp(buffer, COMMAND_IE) == 0){
                        break;
                    }
                    //addWordToTree(tree, buffer);
                    //insertWord(tree, buffer);
                    iterativeInsertWord(tree, buffer);
                }
            }
            else{
                break;
            }
        }
        
        
    }
    
    free(refWord);
    free(buffer);
    
    if(nullScanfValue == 0){
        nullScanfValue=0;
    }
    
    return 0;
}
