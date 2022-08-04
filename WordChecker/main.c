//
//  main.c
//  WordChecker
//
//  Created by Japo on 03/08/22.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 64
#define COMMAND_FP "+stampa_filtrate"
#define COMMAND_IS "+inserisci_inizio"
#define COMMAND_IE "+inserisci_fine"
#define COMMAND_NG "+nuova_partita"
#define COMMAND_LEN 17

typedef struct nodo{
    int matches;
    int hasChild;
    int hasBrother;
    char letter;
    struct nodo* son;
    struct nodo* brother;
    
}t_node;

t_node* lookForSuccessorInTree(t_node* node,char letter){
    if(!node->hasChild){
        return NULL;
    }
    
    if(node->son == NULL){
        return NULL;
    }
    
    t_node *son = node->son;
    
    while(son != NULL){
        if(son->letter == letter){
            return son;
        }
        
        son = son->brother;
    }
    return NULL;
    
}



void insertWord(t_node* tree, const char* word){
    if(tree == NULL){
        return;
    }
    
    t_node *curr = tree;
    
    size_t wordLen = strlen(word);
    for(size_t i=0;i<wordLen;i++){
        t_node *new = (t_node *)malloc(sizeof(t_node));
        new->hasChild=0;
        new->hasBrother = 0;
        new->matches = 1;
        new->son = NULL;
        new->brother = NULL;
        new->letter = word[i];
        
        
        //Il nodo non ha ancora figli
        if(curr->son == NULL){
            curr->hasChild = 1;
            curr->son = new;
            curr = new;
            continue;
        }
        
        //Il nodo ha figli,controllo se la nuova lettera è precedente al primo figlio
        if(word[i] < curr->son->letter){
            new->hasBrother = 1;
            new->brother = curr->son;
            curr->son = new;
            curr = new;

        }
        else if(word[i] == curr->son->letter){ //la nuova lettera è già presente ed è la prima
            free(new);
            curr = curr->son;
            continue;
        }
        else{ //La nuova lettera va accodata perchè non è la prima in ordine alfabetico
            t_node *appoggio = curr->son;
            
            //Mi fermo quando sono arrivato all'ultimo figlio o il successivo è dopo in ordine alfabetico
            while(appoggio->hasBrother && appoggio->brother->letter < word[i]){
                appoggio = appoggio->brother;
            }
            
            if(appoggio->hasBrother){ //Il successivo è >= in ordine alfabetico alla nuova lettera
                
                if(appoggio->brother->letter == word[i]){ //La nuova lettera c'è già ed è il successivo
                    free(new);
                    curr = appoggio->brother;
                    continue;
                }
                
                //Il nodo successivo segue il carattere da aggiungere
                new->hasBrother = 1;
                new->brother = appoggio->brother;
                
            }
            else{
                //Il nodo successivo segue il carattere da aggiungere
                appoggio->hasBrother = 1;
            }
            appoggio->brother = new;
            curr=new;
            
        }
    }
}

void printWordsRecCall(t_node *tree, int wordLen, int step, char *buffer){
    
    if(tree == NULL && step == wordLen){
        
        printf("%s\n",buffer);
        return;
    }
    
    t_node *curr = tree;
    
    while(curr != NULL){
        buffer[step] = curr->letter;
        if(curr->matches){
            printWordsRecCall(curr->son,wordLen,step+1,buffer);
        }
        
        curr = curr->brother;
    }
    
}

void printWords(t_node* tree, int wordLen){
    
    if(tree == NULL || tree->hasChild == 0){
        return;
    }
    
    char *buffer = calloc(wordLen+1, 1);
    t_node *curr = tree->son;
    
    while(curr != NULL){
        buffer[0] = curr->letter;
        if(curr->matches ){
            printWordsRecCall(curr->son,wordLen,1,buffer);
        }
        
        curr = curr->brother;
    }

}

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

void filterTreeRecCall(t_node *tree,int wordLen,u_short charOccurrencies[N],char charExact[N],char charMatches[N][wordLen],int step,u_short currentWord[N],int *validWords){


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
        
    t_node *curr = tree->son;
    
    while(curr != NULL){
        
        int index = charToIndex(curr->letter);
        if(!charMatches[index][step] || (currentWord[index]+1 > charOccurrencies[index] && charExact[index]) ){
            curr->matches = 0;
        }
        else{
            currentWord[index]++;
            filterTreeRecCall(curr, wordLen, charOccurrencies, charExact, charMatches, step+1, currentWord,validWords);
            currentWord[index]--;
        }
    
        curr = curr->brother;
    }
    
}

void filterTree(t_node *tree,int wordLen,u_short charOccurrencies[N],char charExact[N],char charMatches[N][wordLen]){
    if(tree == NULL || tree->hasChild == 0){
        return;
    }
    
    int step = 0;
    int *validWords = calloc(1, sizeof(int));
    
    t_node *curr = tree->son;
    
    while(curr != NULL){
        u_short currentWord[N] = {0};
        int index = charToIndex(curr->letter);
        if(!charMatches[index][step] || (currentWord[index]+1 > charOccurrencies[index] && charExact[index]) ){
            curr->matches = 0;
        }
        else{
            currentWord[index]++;
            filterTreeRecCall(curr, wordLen, charOccurrencies, charExact, charMatches, step+1, currentWord,validWords);
        }
    
        curr = curr->brother;
    }
    
    printf("%d\n",*validWords);
    free(validWords);
    
}

int checkWord(const char *refWord,const char* toBeCheckedWord,int wordLen,t_node* tree, u_short charOccurrencies[N], char charExact[N], char charMatches[N][wordLen]){
    
    u_short freeOccurrencies[N] = {0};
    u_short currentWord[N] = {0};
    char *buffer = calloc(wordLen+1, 1);
    
    char won = 1;
    
    t_node *curr = tree;
    
    for(size_t i = 0;i<wordLen;i++){
        
        if((curr = lookForSuccessorInTree(curr, toBeCheckedWord[i])) == NULL){
            free(buffer);
            printf("not_exists\n");
            return -1;
        }
        int index = charToIndex(toBeCheckedWord[i]);
        
        if(refWord[i] == toBeCheckedWord[i]){
            buffer[i] = '+';
            
            currentWord[index]++;
            /*for(size_t j=0;j<N;j++){
                charMatches[j][i] = 0;
            }
            charMatches[index][i]=1;*/
        }
        else{
            buffer[i] = '/';
            won = 0;
            freeOccurrencies[charToIndex(refWord[i])]++;
            //charMatches[index][i]=0;
        }
        
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
                break;
        }
    }
    
    if(won){
        free(buffer);
        printf("ok\n");
        return 1;
    }
    
    for(size_t i=0;i<wordLen;i++){
        if(buffer[i] == '/' && freeOccurrencies[charToIndex(toBeCheckedWord[i])] > 0){
            buffer[i] = '|';
            currentWord[charToIndex(toBeCheckedWord[i])]++;
            if(currentWord[charToIndex(toBeCheckedWord[i])] > charOccurrencies[charToIndex(toBeCheckedWord[i])]){
                charOccurrencies[charToIndex(toBeCheckedWord[i])]=currentWord[charToIndex(toBeCheckedWord[i])];
            }
            freeOccurrencies[charToIndex(toBeCheckedWord[i])]--;
        }
        else if(buffer[i] == '/'){
            charExact[charToIndex(toBeCheckedWord[i])] = 1;
            charOccurrencies[charToIndex(toBeCheckedWord[i])]=currentWord[charToIndex(toBeCheckedWord[i])];
        }
            
    }
    
    printf("%s\n",buffer);
    
    filterTree(tree, wordLen, charOccurrencies, charExact, charMatches);
    
    free(buffer);
    return 0;
    
}

void insertWordFiltered(t_node *tree, const char* word,int wordLen,u_short charOccurrencies[N],char charExact[N],char charMatches[N][wordLen]){
    if(tree == NULL){
        return;
    }
    
    t_node *curr = tree;
    int currentWord[N] = {0};
    
    for(size_t i=0;i<wordLen;i++){
        
        char c = word[i];
        size_t index = charToIndex(c);
        currentWord[index]++;
        
        t_node *new = (t_node *)malloc(sizeof(t_node));
        new->hasChild=0;
        new->hasBrother = 0;
        new->matches = 1;
        if(!charMatches[index][i] || (currentWord[index] > charOccurrencies[index] && charExact[index])){
            new->matches = 0;
        }
        
        new->son = NULL;
        new->brother = NULL;
        new->letter = word[i];
        
        
        //Il nodo non ha ancora figli
        if(curr->son == NULL){
            curr->hasChild = 1;
            curr->son = new;
            curr = new;
            continue;
        }
        
        //Il nodo ha figli,controllo se la nuova lettera è precedente al primo figlio
        if(word[i] < curr->son->letter){
            new->hasBrother = 1;
            new->brother = curr->son;
            curr->son = new;
            curr = new;

        }
        else if(word[i] == curr->son->letter){ //la nuova lettera è già presente ed è la prima
            free(new);
            curr = curr->son;
            continue;
        }
        else{ //La nuova lettera va accodata perchè non è la prima in ordine alfabetico
            t_node *appoggio = curr->son;
            
            //Mi fermo quando sono arrivato all'ultimo figlio o il successivo è dopo in ordine alfabetico
            while(appoggio->hasBrother && appoggio->brother->letter < word[i]){
                appoggio = appoggio->brother;
            }
            
            if(appoggio->hasBrother){ //Il successivo è >= in ordine alfabetico alla nuova lettera
                
                if(appoggio->brother->letter == word[i]){ //La nuova lettera c'è già ed è il successivo
                    free(new);
                    curr = appoggio->brother;
                    continue;
                }
                
                //Il nodo successivo segue il carattere da aggiungere
                new->hasBrother = 1;
                new->brother = appoggio->brother;
                
            }
            else{
                //Il nodo successivo segue il carattere da aggiungere
                appoggio->hasBrother = 1;
            }
            appoggio->brother = new;
            curr=new;
            
        }
    }
    
    for(size_t j = 0;j<N;j++){
        if(currentWord[j] < charOccurrencies[j]){
            curr->matches = 0;
            break;
        }
    }
}

void cleanTreeMatches(t_node *tree, int wordLen,int step){
    if(tree == NULL || !tree->hasChild){
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
    t_node* tree = (t_node *)malloc(sizeof(t_node));
    tree->hasChild = 0;
    tree->hasBrother = 0;
    tree->matches = 1;
    tree->son = NULL;
    tree->brother = NULL;
    
    char *buffer;
    if(wordLen > COMMAND_LEN){
        buffer = calloc(wordLen+1, 1);
    }
    else{
        buffer = calloc(COMMAND_LEN+1, 1);
    }
    
    /*ADD HERE
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("/Users/japo/Desktop/Università/API/TEST/words.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        insertWord(tree, line);
    }

    fclose(fp);
    if (line)
        free(line);
    printf("finito\n");
    FINISH HERE*/
    
    while(1){
        nullScanfValue=scanf("%s",buffer);
        if(strcmp(buffer, COMMAND_NG) == 0){
            break;
        }
        insertWord(tree, buffer);
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
                    insertWordFiltered(tree, buffer, wordLen, charOccurrencies, charExact, charMatches);
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
                    insertWord(tree, buffer);
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
    
    
    
    
    
    /*int wordLen = 5;
    u_short charOccurrencies[N] = {0};
    char charExact[N] = {0};
    char charMatches[N][wordLen];
    
    for(size_t i=0;i<N;i++){
        for(size_t j=0;j<wordLen;j++){
            charMatches[i][j] = 1;
        }
    }
    
    t_node* tree = (t_node *)malloc(sizeof(t_node));
    tree->hasChild = 0;
    tree->hasBrother = 0;
    tree->matches = 1;
    tree->son = NULL;
    tree->brother = NULL;
    
    
    
    insertWord(tree, "8adfs");
    insertWord(tree, "5sjaH");
    insertWord(tree, "KS06l");
    insertWord(tree, "Hi23a");
    insertWord(tree, "laj74");
    insertWord(tree, "-s9k0");
    insertWord(tree, "sm_ks");
    insertWord(tree, "okauE");

    char *refWord = "5sjaH";
    
    
    checkWord(refWord, "KS06l", wordLen, tree, charOccurrencies, charExact, charMatches);
    checkWord(refWord, "had7s", wordLen, tree, charOccurrencies, charExact, charMatches);
    checkWord(refWord, "okauE", wordLen, tree, charOccurrencies, charExact, charMatches);

    printWords(tree, wordLen);
    
    insertWordFiltered(tree, "PsjW5", wordLen, charOccurrencies, charExact, charMatches);
    insertWordFiltered(tree, "asHdd", wordLen, charOccurrencies, charExact, charMatches);
    insertWordFiltered(tree, "paF7s", wordLen, charOccurrencies, charExact, charMatches);
    
    checkWord(refWord, "-s9k0", wordLen, tree, charOccurrencies, charExact, charMatches);
    checkWord(refWord, "sghks", wordLen, tree, charOccurrencies, charExact, charMatches);

    printWords(tree, wordLen);
    
    checkWord(refWord, "sm_ks", wordLen, tree, charOccurrencies, charExact, charMatches);
    */
    
    
    return 0;
}
