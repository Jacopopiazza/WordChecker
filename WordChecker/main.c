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
    
    if(strlen(word) == 0){
        return 1;
    }
    
    if(tree == NULL){
        return 0;
    }
    
    if(abs(checkPrefixes(tree->son->prefix, word)) == strlen(tree->son->prefix)){
        return isWordInTree(tree->son, word+abs(checkPrefixes(tree->son->prefix, word)));
    }
    
    t_node *curr = tree->son;
    
    int chkCmp;
    int chkPref;
    
    if(curr->brother != NULL){
        chkCmp = strcmp(curr->brother->prefix, word);
        chkPref = checkPrefixes(curr->brother->prefix, word);
    }
    
    while(curr->brother != NULL && chkCmp< 0 && chkPref == 0){
        curr = curr->brother;
        if(curr->brother != NULL){
            chkCmp = strcmp(curr->brother->prefix, word);
            chkPref = checkPrefixes(curr->brother->prefix, word);
        }
        
    }
    
    if(curr->brother == NULL){
        return 0;
    }
    if(chkCmp > 0 && chkPref == 0){
        return 0;
    }
    if(abs(chkPref) == strlen(curr->brother->prefix)){
        return isWordInTree(curr->brother, word+abs(chkPref));
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
    
    free(buffer);
}

void filterTreeRecCall(t_node *tree,int wordLen,int charOccurrencies[N],char charExact[N],char charMatches[N][wordLen], char print, int step,int currentWord[N],int *validWords){


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
        if(curr->matches){
            filter = 1;
            
            char charInPrefix[strlen(curr->prefix)];
            int reached = 0;
           
            for(size_t i=0;i<strlen(curr->prefix);i++){
                int index = charToIndex(curr->prefix[i]);
                
                if(!charMatches[index][step+i] || (currentWord[index]+1 > charOccurrencies[index] && charExact[index]) ){
                    curr->matches = 0;
                    filter = 0;
                    break;
                }
                else{
                    currentWord[index]++;
                    charInPrefix[i] = indexToChar(index);
                    reached++;
                }
            }
            if(filter){
                filterTreeRecCall(curr, wordLen, charOccurrencies, charExact, charMatches, print,step+strlen(curr->prefix), currentWord,validWords);
                
                
            }
            for(size_t i=0;i<reached;i++){
                currentWord[charToIndex(charInPrefix[i])]--;
            }
        
        
        
        }
        curr = curr->brother;
    }
    
}

void filterTree(t_node *tree,int wordLen,int charOccurrencies[N],char charExact[N],char charMatches[N][wordLen],char print){
    if(tree == NULL || tree->son == NULL){
        return;
    }
    
    int step = 0;
    int *validWords = calloc(1, sizeof(int));
    char filter = 1;
    
    t_node *curr = tree->son;
    
    
    while(curr != NULL){
        
        if(curr->matches){
            filter = 1;
            int currentWord[N] = {0};
        
        
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
                filterTreeRecCall(curr, wordLen, charOccurrencies, charExact, charMatches, print, step+strlen(curr->prefix), currentWord,validWords);
            }
        }

        curr = curr->brother;
    }
    if(print){
        printf("%d\n",*validWords);
    }
    free(validWords);
    
}





int checkWord(const char *refWord,const char* toBeCheckedWord,int wordLen,t_node* tree, int charOccurrencies[N], char charExact[N], char charMatches[N][wordLen]){
    
    
    if(!isWordInTree(tree, toBeCheckedWord)){
        printf("not_exists\n");
        return -1;
    }
    
    char *buffer = calloc(wordLen+1, 1);
    char won = 1;
    
    int freeOccurrencies[N] = {0};
    int currentWord[N] = {0};
    //t_node *curr = tree;
    
    
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
                    currentWord[charToIndex(toBeCheckedWord[i])]++;
                    if(currentWord[charToIndex(toBeCheckedWord[i])] > charOccurrencies[charToIndex(toBeCheckedWord[i])]){
                        charOccurrencies[charToIndex(toBeCheckedWord[i])]=currentWord[charToIndex(toBeCheckedWord[i])];
                    }
                    freeOccurrencies[charToIndex(toBeCheckedWord[i])]--;
                    
                    /*freeOccurrencies[charToIndex(toBeCheckedWord[i])]--;
                    charOccurrencies[charToIndex(toBeCheckedWord[i])]++;*/
                }
                else{
                    charExact[charToIndex(toBeCheckedWord[i])] = 1;
                    charOccurrencies[charToIndex(toBeCheckedWord[i])]=currentWord[charToIndex(toBeCheckedWord[i])];
                }
                break;
        }
    }
    
    
    
    printf("%s\n",buffer);
    //FILTER
    
    //printWords(tree, wordLen);
    
    filterTree(tree, wordLen, charOccurrencies, charExact, charMatches, 1);
    
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

void freeTree(t_node *tree){
    
    if(tree==NULL){
        return;
    }
    
    if(tree->son){
        freeTree(tree->son);
    }
    if(tree->brother){
        freeTree(tree->brother);
    }
    if(tree->prefix){
        free(tree->prefix);
    }
    free(tree);
}

void addWordToTree(t_node *tree, const char* word){
    
    size_t wordLen = strlen(word);
    
    if(tree->son == NULL){
        
        tree->son = malloc(sizeof(t_node));
        tree->son->son = NULL;
        tree->son->brother = NULL;
        tree->son->matches = 1;
        tree->son->prefix = calloc(wordLen+1, 1);
        strcpy(tree->son->prefix, word);
        
    }
    else{
        
        t_node *curr = tree->son;
        t_node *parent = tree;
        
        char *buffer = calloc(wordLen+1, 1);
        strcpy(buffer, word);
        size_t step = 0;
        
        while (step < wordLen) {
            int chkCmp = strcmp(curr->prefix, buffer);
            int chkPrefix = checkPrefixes(curr->prefix, buffer);
            
            //1: condividono tutto il prefisso->vado avanti con il figlio
            //2: condividono solo una parte di prefisso, -> splitto e creo nuovo figlio
            //3: non condividono nulla -> dipende da ordine
            
            if(abs(chkPrefix) == strlen(curr->prefix)){
                buffer[0] = '\0';
                step += abs(chkPrefix);
                strcpy(buffer, word+step);
                parent = curr;
                curr = curr->son;

                continue;
            }
            else if(chkPrefix != 0){
                
                char *newPrefix = calloc(abs(chkPrefix)+1, 1);
                snprintf(newPrefix, abs(chkPrefix)+1, "%s",buffer);
                
                t_node *new1 = malloc(sizeof(t_node));
                t_node *new2 = malloc(sizeof(t_node));
                
                new1->brother = new2;
                new2->brother = NULL;
                
                if(chkPrefix < 0){
                    new1->prefix = calloc(strlen(curr->prefix)-abs(chkPrefix)+1, 1);
                    new2->prefix = calloc(wordLen-step-abs(chkPrefix)+1, 1);
                    snprintf(new1->prefix, strlen(curr->prefix)-abs(chkPrefix)+1, "%s", curr->prefix+abs(chkPrefix));
                    snprintf(new2->prefix, wordLen-step-abs(chkPrefix)+1, "%s", buffer+abs(chkPrefix));
                    
                    new1->son = curr->son;
                    new2->son = NULL;
                }
                else{
                    new1->prefix = calloc(wordLen-step-abs(chkPrefix)+1, 1);
                    new2->prefix = calloc(strlen(curr->prefix)-abs(chkPrefix)+1, 1);
                    snprintf(new1->prefix, wordLen-step-abs(chkPrefix)+1, "%s", buffer+abs(chkPrefix));
                    snprintf(new2->prefix, strlen(curr->prefix)-abs(chkPrefix)+1, "%s", curr->prefix+abs(chkPrefix));
                    
                    
                    new2->son = curr->son;
                    new1->son = NULL;
                }
                
                curr->son = new1;
                free(curr->prefix);
                curr->prefix = newPrefix;
                curr->matches=1;
                new1->matches = 1;
                new2->matches = 1;
                
                step += strlen(buffer);
                buffer[0] = '\0';
                continue;
                
            }
            else if(chkCmp > 0 && chkPrefix == 0){
                t_node *new = malloc(sizeof(t_node));
                new->brother = curr;
                new->matches = 1;
                new->son = NULL;
                new->prefix = calloc(strlen(buffer)+1, 1);
                snprintf(new->prefix, strlen(buffer)+1, "%s",buffer);
                
                parent->son = new;
                step += strlen(buffer);
                buffer[0] = '\0';
                
                continue;
            }
            
            //Se sei arrivato qui dopo il primo inizio allora non ha nulla a che fare con il primo figlio, bisogna cercare tra i fratelli
            
            if(curr->brother){
                chkCmp = strcmp(curr->brother->prefix, buffer);
                chkPrefix = checkPrefixes(curr->brother->prefix, buffer);
                
                if(chkCmp > 0 && chkPrefix == 0){
                    t_node *new = malloc(sizeof(t_node));
                    new->brother = curr->brother;
                    new->matches = 1;
                    new->son = NULL;
                    new->prefix = calloc(strlen(buffer)+1, 1);
                    snprintf(new->prefix, strlen(buffer)+1, "%s",buffer);
                    
                    curr->brother = new;
                    step += strlen(buffer);
                    buffer[0] = '\0';
                    
                    continue;
                }
                else{
                    curr = curr->brother;
                    continue;
                }
                
            }
            else{
                curr->brother = malloc(sizeof(t_node));
                curr->brother->brother = NULL;
                curr->brother->son = NULL;
                curr->brother->matches = 1;
                curr->brother->prefix = calloc(strlen(buffer)+1, 1);
                strcpy(curr->brother->prefix, buffer);
                
                step += strlen(buffer);
                buffer[0] = '\0';
                continue;
            }
            
        }
        
        free(buffer);
        
    }
    
    
}

void addWordToTreeFiltered(t_node *tree, const char* word, int wordLen,char charMatches[N][wordLen],int charOccurrencies[N],char charExact[N]){
    
    
    if(tree->son == NULL){
        
        tree->son = malloc(sizeof(t_node));
        tree->son->son = NULL;
        tree->son->brother = NULL;
        tree->son->matches = 1;
        tree->son->prefix = calloc(wordLen+1, 1);
        strcpy(tree->son->prefix, word);
        
    }
    else{
        
        t_node *curr = tree->son;
        t_node *parent = tree;
        t_node *last = NULL;
        
        char *buffer = calloc(wordLen+1, 1);
        strcpy(buffer, word);
        size_t step = 0;
        
        while (step < wordLen) {
            int chkCmp = strcmp(curr->prefix, buffer);
            int chkPrefix = checkPrefixes(curr->prefix, buffer);
            
            //1: condividono tutto il prefisso->vado avanti con il figlio
            //2: condividono solo una parte di prefisso, -> splitto e creo nuovo figlio
            //3: non condividono nulla -> dipende da ordine
            
            if(abs(chkPrefix) == strlen(curr->prefix)){
                buffer[0] = '\0';
                step += abs(chkPrefix);
                strcpy(buffer, word+step);
                parent = curr;
                curr = curr->son;

                continue;
            }
            else if(chkPrefix != 0){
                
                char *newPrefix = calloc(abs(chkPrefix)+1, 1);
                snprintf(newPrefix, abs(chkPrefix)+1, "%s",buffer);
                
                t_node *new1 = malloc(sizeof(t_node));
                t_node *new2 = malloc(sizeof(t_node));
                
                new1->brother = new2;
                new2->brother = NULL;
                
                if(chkPrefix < 0){
                    new1->prefix = calloc(strlen(curr->prefix)-abs(chkPrefix)+1, 1);
                    new2->prefix = calloc(wordLen-step-abs(chkPrefix)+1, 1);
                    snprintf(new1->prefix, strlen(curr->prefix)-abs(chkPrefix)+1, "%s", curr->prefix+abs(chkPrefix));
                    snprintf(new2->prefix, wordLen-step-abs(chkPrefix)+1, "%s", buffer+abs(chkPrefix));
                    
                    new1->son = curr->son;
                    new2->son = NULL;
                    
                    last = new2;
                    
                    
                    new1->matches = curr->matches;
                }
                else{
                    new1->prefix = calloc(wordLen-step-abs(chkPrefix)+1, 1);
                    new2->prefix = calloc(strlen(curr->prefix)-abs(chkPrefix)+1, 1);
                    snprintf(new1->prefix, wordLen-step-abs(chkPrefix)+1, "%s", buffer+abs(chkPrefix));
                    snprintf(new2->prefix, strlen(curr->prefix)-abs(chkPrefix)+1, "%s", curr->prefix+abs(chkPrefix));
                    
                    
                    new2->son = curr->son;
                    new1->son = NULL;
                    
                    last = new1;
                    
                    
                    new2->matches = curr->matches;
                }
                
                curr->son = new1;
                free(curr->prefix);
                curr->prefix = newPrefix;
                curr->matches=1;
                
                for(size_t j=0;j<abs(chkPrefix);j++){
                    if(!charMatches[charToIndex(curr->prefix[j])][j+step]){
                        curr->matches = 0;
                        break;
                    }
                }
                
                if(curr->matches){
                    for(size_t j=0;j<strlen(last->prefix);j++){
                        if(!charMatches[charToIndex(last->prefix[j])][j+step+abs(chkPrefix)]){
                            last->matches=0;
                            break;
                        }
                    }
                }
                
                
                step += strlen(buffer);
                buffer[0] = '\0';
                continue;
                
            }
            else if(chkCmp > 0 && chkPrefix == 0){
                t_node *new = malloc(sizeof(t_node));
                new->brother = curr;
                new->matches = 1;
                new->son = NULL;
                new->prefix = calloc(strlen(buffer)+1, 1);
                snprintf(new->prefix, strlen(buffer)+1, "%s",buffer);
                
                for(size_t j=0;j<strlen(buffer);j++){
                    if(!charMatches[charToIndex(buffer[j])][step+j]){
                        new->matches = 0;
                        break;
                    }
                }
                
                parent->son = new;
                step += strlen(buffer);
                buffer[0] = '\0';
                
                last = new;
                
                continue;
            }
            
            //Se sei arrivato qui dopo il primo inizio allora non ha nulla a che fare con il primo figlio, bisogna cercare tra i fratelli
            
            if(curr->brother){
                chkCmp = strcmp(curr->brother->prefix, buffer);
                chkPrefix = checkPrefixes(curr->brother->prefix, buffer);
                
                if(chkCmp > 0 && chkPrefix == 0){
                    t_node *new = malloc(sizeof(t_node));
                    new->brother = curr->brother;
                    new->matches = 1;
                    new->son = NULL;
                    new->prefix = calloc(strlen(buffer)+1, 1);
                    snprintf(new->prefix, strlen(buffer)+1, "%s",buffer);
                    
                    for(size_t j=0;j<strlen(buffer);j++){
                        if(!charMatches[charToIndex(buffer[j])][step+j]){
                            new->matches = 0;
                            break;
                        }
                    }
                    
                    curr->brother = new;
                    step += strlen(buffer);
                    buffer[0] = '\0';
                    
                    last = new;
                    
                    continue;
                }
                else{
                    curr = curr->brother;
                    continue;
                }
                
            }
            else{
                curr->brother = malloc(sizeof(t_node));
                curr->brother->brother = NULL;
                curr->brother->son = NULL;
                curr->brother->matches = 1;
                curr->brother->prefix = calloc(strlen(buffer)+1, 1);
                strcpy(curr->brother->prefix, buffer);
                
                for(size_t j=0;j<strlen(buffer);j++){
                    if(!charMatches[charToIndex(buffer[j])][step+j]){
                        curr->brother->matches = 0;
                        break;
                    }
                }
                
                step += strlen(buffer);
                buffer[0] = '\0';
                
                last = curr->brother;
                continue;
            }
            
        }
        
        free(buffer);
        
        if(last->matches){
            size_t currentWord[N] = {0};
            for(size_t j=0;j<wordLen;j++){
                currentWord[charToIndex(word[j])]++;
            }
            
            for(size_t j=0;j<N;j++){
                if(currentWord[j] < charOccurrencies[j]){
                    last->matches = 0;
                    break;
                }
            }
        }
        
        
        
        
    }
    
    
}


int main(int argc, const char * argv[]) {
    // insert code here...
    
    char nullScanfValue;
    int wordLen = -1;
    nullScanfValue=scanf("%d", &wordLen);

    
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
        //iterativeInsertWord(tree, buffer);
        addWordToTree(tree, buffer);
        //addWordToTree(tree, buffer);
    }
    
    /*FILE *file = fopen("/Users/japo/Downloads/open_testcases(1)/test3_words.txt", "r");
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    
    while ((read = getline(&line, &len, file)) != -1) {
        if( line[5] == '\r' ) {
                line[5] = '\0';
            }
        iterativeInsertWord(tree, line);
    }
    
    fclose(file);*/
     
   
    
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
                        //filterTree(tree, wordLen, charOccurrencies, charExact, charMatches, 0);
                        break;
                    }
                    //addWordToTreeFiltered(tree, buffer, wordLen, charMatches, charOccurrencies, charExact);
                    //insertWordFiltered(tree, buffer, wordLen, charOccurrencies, charExact, charMatches);
                    //iterativeInsertWordFiltered(tree, buffer, wordLen, charOccurrencies, charExact, charMatches);
                    //addWordToTree(tree, buffer);
                    addWordToTreeFiltered(tree, buffer, wordLen, charMatches, charOccurrencies, charExact);
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
                    addWordToTree(tree, buffer);
                    //iterativeInsertWord(tree, buffer);
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
    
    //freeTree(tree);

    
    
    return 0;
}

