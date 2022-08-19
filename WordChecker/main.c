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
    
    
    if(c >= 'A' && c <= 'Z'){
        return c - 'A' + 11;
    }
    else if(c >= 'a' && c <= 'z'){
        return 38 + c - 'a';
    }
    else if(c >= '0' && c <= '9'){
        return c - '0' + 1;
    }
    if(c == '-'){
        return 0;
    }
    if(c == '_'){
        return c - '_'+37;
    }
    
    
    
    return 0;
}


int getMatchedNode(t_node *tree, const char* word, t_node **whereToInsert){
    if(!tree || !tree->son){
        return 0;
        //return 0 -> tree is empty
    }
    
    if(tree->son->prefix[0] == word[0]){
        *whereToInsert = tree->son;
        return 1;
        //return 1 -> first children is the node we are looking for
        
    }
    if(tree->son->prefix[0] > word[0]){
        *whereToInsert = tree;
        return 2;
        //return 2 -> first children is successor of what we are looking for
    }
    
    tree = tree->son;
    while(tree->brother && tree->brother->prefix[0] < word[0]){
        tree = tree->brother;
    }
    
    if(!tree->brother){
        *whereToInsert = tree;
        return 3;
        //return 3: what we are looking for will be the last
    }
    
    if(tree->brother->prefix[0] == word[0]){
        *whereToInsert = tree->brother;
        return 4;
        //return 4: node we are looking for is in variable
    }
    
    if(tree->brother->prefix[0] > word[0]){
        
        *whereToInsert = tree;
        return 5;
        //return 5: new node will be in the middle of the node in whereToInsert and his current brother
    }
    
    return -1;
    
}

int matchingCharacters(const char* word1, const char *word2){
    int matches = 0;
    int len = 0;
    
    if(strlen(word1) <= strlen(word2)){
        len = strlen(word1);
    }
    else{
        len = strlen(word2);
    }
    
    for(int i = 0;i<len;i++){
        if(word1[i] != word2[i]){
            return matches;
        }
        matches++;
    }
    
    return matches;
}

void insertWordIterative(t_node *tree, const char* word){
    t_node *toInsert = NULL;
    t_node *curr = tree;
    char done=0;
    
    while(!done){
        int resGetMatched = getMatchedNode(curr, word, &toInsert);
        
        switch (resGetMatched){
            case 0:
            {
                curr->son = malloc(sizeof(t_node));
                curr->son->brother = NULL;
                curr->son->son = NULL;
                curr->son->prefix = calloc(strlen(word)+2, 1);
                strcpy(curr->son->prefix, word);
                curr->son->prefix[strlen(word)] = 0;
                curr->son->prefix[strlen(word)+1] = 1;
                
                done=1;
                break;
            }
            
            case 1:
            case 4:
            {
                int matching = matchingCharacters(word, toInsert->prefix);
                
                if(matching == strlen(word)){
                    return;
                }
                
                if(matching == strlen(toInsert->prefix)){
                    curr = toInsert;
                    word = word+strlen(toInsert->prefix);
                    continue;
                }
                
                char *commonRadix = calloc(matching+2, 1);
                strncpy(commonRadix, word, matching);
                commonRadix[matching] = 0;
                commonRadix[matching+1] = 1;
                
                char *newPartOldNode = calloc(strlen(toInsert->prefix)-matching+2, 1);
                strncpy(newPartOldNode, toInsert->prefix+matching, strlen(toInsert->prefix)-matching);
                newPartOldNode[strlen(toInsert->prefix)-matching] = 0;
                newPartOldNode[strlen(toInsert->prefix)-matching+1] = 1;
                
                char *newPartNewWord = calloc(strlen(word)-matching+2, 1);
                strncpy(newPartNewWord, word+matching, strlen(word)-matching);
                newPartNewWord[strlen(word)-matching] = 0;
                newPartNewWord[strlen(word)-matching+1] = 1;
                
                free(toInsert->prefix);
                toInsert->prefix = commonRadix;
                
                t_node *newPartNewWordNode = malloc(sizeof(t_node));
                newPartNewWordNode->prefix = newPartNewWord;
                newPartNewWordNode->son = NULL;
                newPartNewWordNode->brother = NULL;
                
                t_node *newPartOldNodeNode = malloc(sizeof(t_node));
                newPartOldNodeNode->prefix = newPartOldNode;
                newPartOldNodeNode->son = toInsert->son;
                newPartOldNodeNode->brother = NULL;
                
                
                if(newPartNewWord[0] < newPartOldNode[0]){
                    toInsert->son = newPartNewWordNode;
                    newPartNewWordNode->brother = newPartOldNodeNode;
                }
                else{
                    toInsert->son = newPartOldNodeNode;
                    newPartOldNodeNode->brother = newPartNewWordNode;
                }
                
                done=1;
                break;
            }
                
            case 2:
            {
                t_node *new = malloc(sizeof(t_node));
                new->son = NULL;
                new->prefix = calloc(strlen(word)+2, 1);
                strcpy(new->prefix, word);
                new->prefix[strlen(word)] = 0;
                new->prefix[strlen(word)+1] = 1;
                new->brother = toInsert->son;
                toInsert->son = new;
                
                done=1;
                break;
            }
                
            case 3:
            case 5:
            {
                t_node *new = malloc(sizeof(t_node));
                new->son = NULL;
                new->brother = NULL;
                new->prefix = calloc(strlen(word)+2, 1);
                strcpy(new->prefix, word);
                new->prefix[strlen(word)] = 0;
                new->prefix[strlen(word)+1] = 1;
                
                if(resGetMatched == 5){
                    new->brother = toInsert->brother;
                }
                
                toInsert->brother = new;
                
                done=1;
                break;
            }
                
        }
    }
}

char strCmpPersonal(const char* word1, const char* word2){
    
    if(strlen(word1) != strlen(word2)){
        return 1;
    }
    
    return memcmp(word1, word2, strlen(word1));
}

void insertWordFilteredIterative(t_node *tree, const char* word,int wordLen,int charOccurrencies[N],char charExact[N], char charMatches[N][wordLen]){
    t_node *toInsert = NULL;
    t_node *curr = tree;
    int step = 0;
    char done=0;
    int currentWord[N] = { 0 };
    
    while(!done){
        int resGetMatched = getMatchedNode(curr, word, &toInsert);
        
        switch (resGetMatched){
            case 0:
            {
                curr->son = malloc(sizeof(t_node));
                curr->son->brother = NULL;
                curr->son->son = NULL;
                curr->son->prefix = calloc(strlen(word)+2, 1);
                curr->son->prefix[strlen(word)+1] = 1;
                
                for(int i = 0;i<strlen(word);i++){
                    curr->son->prefix[i] = word[i];
                    currentWord[charToIndex(word[i])]++;
                    
                    if(!charMatches[charToIndex(word[i])][i+step] || ( charExact[charToIndex(word[i])] && currentWord[charToIndex(word[i])] > charOccurrencies[charToIndex(word[i])] )){
                        curr->son->prefix[strlen(word)+1]=0;
                    }
                }
                
                curr->son->prefix[strlen(word)] = 0;
                step += strlen(word);
                
                if(curr->son->prefix[strlen(word)+1]){
                    for(int i=0; i<N; i++){
                        if(currentWord[i] < charOccurrencies[i]){
                            curr->son->prefix[strlen(word)+1]=0;
                            break;
                        }
                    }
                }
               
                
                done=1;
                break;
            }
            
            case 1:
            case 4:
            {
                int matching = matchingCharacters(word, toInsert->prefix);
                
                if(matching == strlen(word)){
                    return;
                }
                
                if(matching == strlen(toInsert->prefix)){
                    curr = toInsert;
                    word = word+strlen(toInsert->prefix);
                    step += strlen(toInsert->prefix);
                    
                    for(int i = 0;i<strlen(toInsert->prefix);i++){
                        currentWord[charToIndex(toInsert->prefix[i])]++;
                    }
                    
                    continue;
                }
                
                char *commonRadix = calloc(matching+2, 1);
                commonRadix[matching+1] = 1;
                
                for(int i = 0; i<matching ;i++){
                    commonRadix[i] = word[i];
                    currentWord[charToIndex(word[i])]++;
                    
                    if(!charMatches[charToIndex(commonRadix[i])][i+step] || ( charExact[charToIndex(commonRadix[i])] && currentWord[charToIndex(commonRadix[i])] > charOccurrencies[charToIndex(commonRadix[i])] )){
                        commonRadix[matching+1]=0;
                    }
                }
                
                commonRadix[matching] = 0;
                
                
                char *newPartOldNode = calloc(strlen(toInsert->prefix)-matching+2, 1);
                newPartOldNode[strlen(toInsert->prefix)-matching+1] = toInsert->prefix[strlen(toInsert->prefix)+1];
                for(int i = 0;i<strlen(toInsert->prefix)-matching;i++){
                    newPartOldNode[i] = toInsert->prefix[i+matching];
                    currentWord[charToIndex(toInsert->prefix[i+matching])]++;
                    
                    if(!charMatches[charToIndex(newPartOldNode[i])][i+step+strlen(commonRadix)] || ( charExact[charToIndex(newPartOldNode[i])] && currentWord[charToIndex(newPartOldNode[i])] > charOccurrencies[charToIndex(newPartOldNode[i])] )){
                        newPartOldNode[strlen(toInsert->prefix)-matching+1]=0;
                    }
                    
                }
                
                
                newPartOldNode[strlen(toInsert->prefix)-matching] = 0;
                
                for(int i = 0;i<strlen(toInsert->prefix)-matching;i++){
                    currentWord[charToIndex(toInsert->prefix[i+matching])]--;
                }
                
                char *newPartNewWord = calloc(strlen(word)-matching+2, 1);
                newPartNewWord[strlen(word)-matching+1] = 1;
                
                for(int i=0; i < strlen(word)-matching; i++){
                    newPartNewWord[i] = word[i+matching];
                    currentWord[charToIndex(word[i+matching])]++;
                    
                    if(!charMatches[charToIndex(newPartNewWord[i])][i+step+strlen(commonRadix)] || (
                       charExact[charToIndex(newPartNewWord[i])] && currentWord[charToIndex(newPartNewWord[i])] > charOccurrencies[charToIndex(newPartNewWord[i])] )){
                        newPartNewWord[strlen(word)-matching+1] = 0;
                    }
                }
                
                newPartNewWord[strlen(word)-matching] = 0;
                
                if( newPartNewWord[strlen(word)-matching+1] ){
                    for(int i = 0;i<N;i++){
                        if(currentWord[i] < charOccurrencies[i]){
                            newPartNewWord[strlen(word)-matching+1] = 0;
                        }
                    }
                }
                
                free(toInsert->prefix);
                toInsert->prefix = commonRadix;
                
                t_node *newPartNewWordNode = malloc(sizeof(t_node));
                newPartNewWordNode->prefix = newPartNewWord;
                newPartNewWordNode->son = NULL;
                newPartNewWordNode->brother = NULL;
                
                t_node *newPartOldNodeNode = malloc(sizeof(t_node));
                newPartOldNodeNode->prefix = newPartOldNode;
                newPartOldNodeNode->son = toInsert->son;
                newPartOldNodeNode->brother = NULL;
                
                
                if(newPartNewWord[0] < newPartOldNode[0]){
                    toInsert->son = newPartNewWordNode;
                    newPartNewWordNode->brother = newPartOldNodeNode;
                }
                else{
                    toInsert->son = newPartOldNodeNode;
                    newPartOldNodeNode->brother = newPartNewWordNode;
                }
                
                done=1;
                step += strlen(word);
                break;
            }
                
            case 2:
            {
                t_node *new = malloc(sizeof(t_node));
                new->son = NULL;
                new->prefix = calloc(strlen(word)+2, 1);
                new->prefix[strlen(word)+1] = 1;
                
                for(int i = 0;i<strlen(word);i++){
                    new->prefix[i] = word[i];
                    currentWord[charToIndex(word[i])]++;
                    
                    if(!charMatches[charToIndex(word[i])][i+step] || ( charExact[charToIndex(word[i])] && currentWord[charToIndex(word[i])] > charOccurrencies[charToIndex(word[i])] )){
                        new->prefix[strlen(word)+1] = 0;
                    }
                }
                
                new->prefix[strlen(word)] = 0;
                
                new->brother = toInsert->son;
                toInsert->son = new;
                
                if(new->prefix[strlen(new->prefix)+1]){
                    for(int i = 0;i<N;i++){
                        if(currentWord[i] < charOccurrencies[i]){
                            new->prefix[strlen(new->prefix)+1] = 0;
                            break;
                        }
                    }
                }
                
                done=1;
                break;
            }
                
            case 3:
            case 5:
            {
                t_node *new = malloc(sizeof(t_node));
                new->son = NULL;
                new->brother = NULL;
                new->prefix = calloc(strlen(word)+2, 1);
                new->prefix[strlen(word)+1] = 1;
                
                for(int i = 0;i<strlen(word);i++){
                    new->prefix[i] = word[i];
                    currentWord[charToIndex(word[i])]++;
                    
                    if(!charMatches[charToIndex(word[i])][i+step] || ( charExact[charToIndex(word[i])] && currentWord[charToIndex(word[i])] > charOccurrencies[charToIndex(word[i])] )){
                        new->prefix[strlen(word)+1] = 0;
                    }
                }
                
                //strcpy(new->prefix, word);
                new->prefix[strlen(word)] = 0;
                
                if(resGetMatched == 5){
                    new->brother = toInsert->brother;
                }
                
                toInsert->brother = new;
                
                if(new->prefix[strlen(new->prefix)+1]){
                    for(int i = 0;i<N;i++){
                        if(currentWord[i] < charOccurrencies[i]){
                            new->prefix[strlen(new->prefix)+1] = 0;
                            break;
                        }
                    }
                }
                
                done=1;
                break;
            }
                
        }
    }
}

void printTreeWordsRecCall(t_node *tree,int wordLen, int step, char *buffer){
    if(step == wordLen){
        
        printf("%s\n",buffer);
        return;
    }
    
    t_node *curr = tree;
    
    while (curr) {
                
        if(curr->prefix[strlen(curr->prefix)+1]){ //Match
            
            strcpy(buffer+step, curr->prefix);
            printTreeWordsRecCall(curr->son, wordLen, step + strlen(curr->prefix), buffer);
            
            buffer[step] = '\0';
            
            
        }
        curr=curr->brother;
    }
}

void printTreeWords(t_node *tree,int wordLen){
    
    if(!tree || !tree->son){
        return;
    }
    
    int step = 0;
    char *buffer = calloc(wordLen+1, 1);
    
    t_node *curr = tree->son;
    
    while (curr) {
                
        if(curr->prefix[strlen(curr->prefix)+1]){ //Match
            
            strcpy(buffer+step, curr->prefix);
            printTreeWordsRecCall(curr->son, wordLen, step + strlen(curr->prefix), buffer);
            buffer[0] = '\0';
            
            
        }
        curr=curr->brother;
    }
    
    
    
    free(buffer);
}

void filterTreeRecCall(t_node *tree, int wordLen, int charOccurrencies[N], char charExact[N], char charMatches[N][wordLen], int step, int currentWord[N], char *buffer, int *validWords){
    
    
    if(step == wordLen){
        
        for(int i=0;i<N;i++){
            if(currentWord[i] < charOccurrencies[i]){
                tree->prefix[strlen(tree->prefix)+1] = 0;
                return;
            }
        }
        
        (*validWords)++;
        return;
    }
    
    t_node *curr = tree->son;
    
    while (curr) {
        int len = strlen(curr->prefix);
                
        if(curr->prefix[len+1]){ //Match
            
            buffer[step] = '\0';
            //char charInPrefix[strlen(curr->prefix)];
            int i = 0;
            char proceed = 1;
            for(i = 0;i<len;i++){
                
                if(!charMatches[charToIndex(curr->prefix[i])][step+i] || (charExact[charToIndex(curr->prefix[i])] && currentWord[charToIndex(curr->prefix[i])]+1 > charOccurrencies[charToIndex(curr->prefix[i])])){
                    curr->prefix[len+1] = 0;
                    proceed=0;
                    break;
                }
                currentWord[charToIndex(curr->prefix[i])]++;
                //charInPrefix[i] = curr->prefix[i];
                buffer[step+i] = curr->prefix[i];
            }
            
            if(proceed){
                //strcpy(buffer+step, curr->prefix);
                filterTreeRecCall(curr, wordLen, charOccurrencies, charExact, charMatches, step + len, currentWord, buffer, validWords);
                
            }
            buffer[step] = '\0';
            for(int j=0;j<i;j++){
                //currentWord[charToIndex(charInPrefix[j])]--;
                currentWord[charToIndex(curr->prefix[j])]--;

            }
            
            
        }
        curr=curr->brother;
    }
}

void filterTree(t_node *tree, int wordLen, int charOccurrencies[N], char charExact[N], char charMatches[N][wordLen],char print){
    if(!tree || !tree->son){
        return;
    }
    
    int step = 0;
    int *validWords = calloc(1, sizeof(int));
    char *buffer = calloc(wordLen+1, 1);
    
    t_node *curr = tree->son;
    
    while (curr) {
                
        int len = strlen(curr->prefix);
        if(curr->prefix[len+1]){ //Match
            
            
            buffer[0] = '\0';
            int currentWord[N] = {0};
            int i = 0;
            char proceed = 1;
            for(i = 0;i<len;i++){
            
                if(!charMatches[charToIndex(curr->prefix[i])][i] || (charExact[charToIndex(curr->prefix[i])] && currentWord[charToIndex(curr->prefix[i])]+1 > charOccurrencies[charToIndex(curr->prefix[i])])){
                    curr->prefix[len+1] = 0;
                    proceed=0;
                    break;
                }
                currentWord[charToIndex(curr->prefix[i])]++;
                buffer[i] = curr->prefix[i];
            }
            
            if(proceed){
                //strcpy(buffer, curr->prefix);
                filterTreeRecCall(curr, wordLen, charOccurrencies, charExact, charMatches, step + len, currentWord, buffer, validWords);
            }
            buffer[0] = '\0';
        }
        curr=curr->brother;
    }
    
    if(print){
        printf("%d\n",*validWords);
    }
    free(buffer);
    free(validWords);
}

char isWordInTree(t_node *tree, const char *word){
    
    if(strlen(word) == 0){
        return 1;
    }
    
    if(!tree || !tree->son){
        return 0;
    }
    
    if(tree->son->prefix[0] == word[0]){
        
        if(strlen(tree->son->prefix) == matchingCharacters(tree->son->prefix, word)){
            return isWordInTree(tree->son, word+strlen(tree->son->prefix));
        }
        else{
            return 0;
        }
        
    }
    else if(tree->son->prefix[0] > word[0]){
        return 0;
    }
    else{
        
        t_node *curr = tree->son;
        
        while (curr->brother && curr->brother->prefix[0] < word[0]) {
            curr = curr->brother;
        }
        
        if(!curr->brother){
            return 0;
        }
        else if(curr->brother->prefix[0] == word[0]){
            if(strlen(curr->brother->prefix) == matchingCharacters(curr->brother->prefix, word)){
                return isWordInTree(curr->brother, word+strlen(curr->brother->prefix));
            }
            else{
                return 0;
            }
        }
        else{
            return 0;
        }
    }
    
    
    return 0;
}

int checkWord(t_node *tree, const char* word, const char* refWord, int wordLen, int charOccurrencies[N], char charExact[N], char charMatches[N][wordLen]){
    
    if(!isWordInTree(tree,word)){
        printf("not_exists\n");
        return -1;
    }
    
    char *buffer = calloc(wordLen+1, 1);
    int freeOccurencies[N] = {0};
    int currentWord[N] = {0};
    char won = 1;
    
    for(int i = 0;i<wordLen;i++){
        if(word[i] == refWord[i]){
            buffer[i] = '+';
            
            for(int j=0;j<N;j++){
                charMatches[j][i] = 0;
            }
            charMatches[charToIndex(word[i])][i] = 1;
            currentWord[charToIndex(word[i])]++;
        }
        else{
            won = 0;
            buffer[i] = '/';
            charMatches[charToIndex(word[i])][i] = 0;
            freeOccurencies[charToIndex(refWord[i])]++;
        }
    }
    
    if(won){
        printf("ok\n");
        free(buffer);
        return 1;
    }
    
    for(int i = 0;i<wordLen;i++){
        if(buffer[i] == '/'){
            if(freeOccurencies[charToIndex(word[i])] > 0){
                buffer[i] = '|';
                freeOccurencies[charToIndex(word[i])]--;
                currentWord[charToIndex(word[i])]++;
                
                if(currentWord[charToIndex(word[i])] > charOccurrencies[charToIndex(word[i])]){
                    charOccurrencies[charToIndex(word[i])]=currentWord[charToIndex(word[i])];
                }
            }
            else{
                charExact[charToIndex(word[i])] = 1;
                charOccurrencies[charToIndex(word[i])]=currentWord[charToIndex(word[i])];
            }
        }
    }
    
    printf("%s\n",buffer);
    
    filterTree(tree, wordLen, charOccurrencies, charExact, charMatches,1);
    
    
    free(buffer);
    
    return 0;
}

void cleanTreeMatches(t_node *tree, int wordLen,int step){
    if(tree == NULL){
        return;
    }
    
    t_node *curr = tree->son;
    
    while(curr != NULL){
        curr->prefix[strlen(curr->prefix)+1] = 1;
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

int main(int argc, const char * argv[]) {
    // insert code here...
    
    char nullScanfValue;
    int wordLen = 5;
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
    t_node *tree = malloc(sizeof(tree));
    tree->prefix = calloc(2, 1);
    tree->prefix[0] = '~';
    tree->brother = NULL;
    tree->son = NULL;
    
    
    char *buffer;
    if(wordLen > COMMAND_LEN){
        buffer = calloc(wordLen+1, 1);
    }
    else{
        buffer = calloc(COMMAND_LEN+1, 1);
    }

    while(1){
        nullScanfValue=scanf("%s",buffer);
        if(buffer[0] == '+'){
            if(buffer[0] == '+' && strCmpPersonal(buffer, COMMAND_NG) == 0){
                break;
            }
            continue;
        }
    
        insertWordIterative(tree, buffer);
        //insertWord(tree, buffer);
    }

    char new_game = 1;
    tree = tree;
    
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
            if(buffer[0] == '+' && strCmpPersonal(buffer, COMMAND_IS) == 0){
                
                while(1){
                    nullScanfValue=scanf("%s",buffer);
                    if(buffer[0] == '+' && strCmpPersonal(buffer, COMMAND_IE) == 0){
                        //filterTree(tree, wordLen, charOccurrencies, charExact, charMatches,0);
                        break;
                    }
                    //insertWordIterative(tree, buffer);
                    insertWordFilteredIterative(tree, buffer, wordLen, charOccurrencies, charExact, charMatches);
                }
            }
            else if(buffer[0] == '+' && strCmpPersonal(buffer, COMMAND_FP) == 0){
                printTreeWords(tree, wordLen);
            }
            else{
                size_t res = checkWord(tree, buffer, refWord, wordLen, charOccurrencies, charExact, charMatches);
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
            if(buffer[0] == '+' && strCmpPersonal(buffer, COMMAND_NG) == 0){
                new_game = 1;
                //Clean tree and filters
                newGame(tree, wordLen, charMatches, charOccurrencies, charExact);
                break;
            }
            else if(buffer[0] == '+' && strCmpPersonal(buffer, COMMAND_IS) == 0){
                while(1){
                    nullScanfValue=scanf("%s",buffer);
                    if(buffer[0] == '+' && strCmpPersonal(buffer, COMMAND_IE) == 0){
                        break;
                    }
                    insertWordIterative(tree, buffer);
                    //insertWord(tree, buffer);
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
