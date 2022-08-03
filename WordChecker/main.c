//
//  main.c
//  WordChecker
//
//  Created by Japo on 29/05/22.
//

#include <stdio.h>
#include "Dictionary.h"

#define COMMAND_NG "+nuova_partita"
#define COMMAND_FP "+stampa_filtrate"
#define COMMAND_IS "+inserisci_inizio"
#define COMMAND_IE "+inserisci_fine"


#define COMMAND_LEN 18

int checkWord(const char* refWord, char* toBeCheckedWord,int len,t_node* tree,t_occurrence limits[N]);


void filter_plus(char c,int pos,t_node *tree);
void filter_plusRecCall(char c,int pos,t_node *tree,int step);

void filter_slash(char c,int pos,t_node *tree);
void filter_slashRecCall(char c,int pos,t_node *tree,int step);


int main(int argc, const char * argv[]) {
    // insert code here...
        
    t_node* tree = (t_node*)malloc(sizeof(t_node));
    t_occurrence limits[N] = {0,0};

    tree->hasChild = 0;
    tree->matches = 1;
    for(int i=0;i<N;i++){
        tree->pointers[i] = NULL;
    }
    
    int wordLen = -1;
    char *buffer = NULL;
    scanf("%d",&wordLen);
    
    //Dimensiono il buffer
    if(wordLen < COMMAND_LEN){
        buffer = calloc(COMMAND_LEN, 1);
    }
    else{
        buffer = calloc(wordLen+1, 1);
    }
    
    //Riempio il dizionario iniziale
    while(1){
        scanf("%s",buffer);
        if(strcmp(COMMAND_NG, buffer) == 0){
            break;
        }
        addWordToTree(tree, buffer);
        
    }
    
    filter_plus('1', 1, tree);
    filter_slash('5', 2, tree);
    printWords(tree, wordLen);
    return 1;
    
    //Nuova partita
    char *refWord = calloc(wordLen+1, 1);
    int tries = -1;
    int won = 0;
    scanf("%s",refWord);
    scanf("%d",&tries);
    
    while(tries > 0 && !won){
        
        scanf("%s",buffer);
        if(strcmp(buffer, COMMAND_FP) == 0){ //+stampa_filtrate
            printWords(tree, wordLen);
            
        }
        else if(strcmp(buffer, COMMAND_IS) == 0){ //+inserisci_inizio
            //Riempio il dizionario iniziale
            while(1){
                scanf("%s",buffer);
                if(strcmp(COMMAND_IE, buffer)){ //+inserisci_fine
                    break;
                }
                addWordToTree(tree, buffer);
                
            }
        }
        else{ //tentativo: voglio stampare stringa e conteggio parole rimanenti
            if(checkWord(refWord, buffer, wordLen, tree, &limits) == 1){
                break;
            }
            //printf("%d",countWords(tree, wordLen));
        }
        tries--;
    }
    
    if(won){
        //Ha vinto
    }
    else{
        //Non ha vinto
    }
    
    
    
    
    return 0;

}


//Qua invece che usare la funzione ricorsiva di conteggio ogni volta si potrebbe provare a sfruttare il fatto che tutti i nodi con segno di match '+' comportano una sola possibile scelta, perciò posso ignorare le scelte

int checkWord(const char* refWord, char* toBeCheckedWord,int len,t_node* tree,t_occurrence limits[N]){
    
    //char *buffer = malloc(sizeof(char) * (len+1));
    char *buffer = calloc(len+1,sizeof(char));
    int right_guess = 0, wrong_pos=0;
    int checked[len];
    int retValue = 0;
    int currentWord[N] = {0};
    
    t_node *curr = tree;
    //check posizione singola e presenza nel dizionario
    for(int i = 0;i<len;i++){
        if(curr->pointers[charToIndex(toBeCheckedWord[i])] == NULL){//Mentre scorro controllo che la parola sia nel dizionario
            printf("not_exists\n");
            free(buffer);
            return -1;
        }
        
        if(refWord[i] == toBeCheckedWord[i]){
            buffer[i] = '+';
            right_guess++;
            checked[i] = 1;
            
        }
        else{
            buffer[i] = '\\';
            checked[i]=0;
        }
        
        curr = curr->pointers[charToIndex(toBeCheckedWord[i])];
    }
    //se mancano dei check verifico se ho lettere fuori posizione
    if(right_guess < len){
        
        //Non considero eventuali caratteri iniziali che già combaciano nella ricerca di fuori posizione. Cerco dove finiscono eventuali caratteri che già matchano
        int base = 0;
        
        while(right_guess < len && base < len ){
            if(buffer[base] != '+'){
                break;
            }
            base++;
        }
        
        //Cerco fuori posizione da base in poi.
        int i = base;
        while(i < len && right_guess < len){
            
            if(buffer[i] == '\\'){
                int j = base;
                while(j < len){
                    if(toBeCheckedWord[i] == refWord[j] && checked[j] == 0){
                        checked[j] = 1;
                        buffer[i] = '|';
                        
                        size_t index = charToIndex(toBeCheckedWord[i]);
                        currentWord[index]++;
                        if(currentWord[index] > limits[index].count){
                            limits[index].count = currentWord[index];
                        }
                        
                        wrong_pos++;
                        break;
                    }
                    j++;
                }
                if(j==len){
                    limits[charToIndex(toBeCheckedWord[i])].exact = 1;
                }
                
            }
            i++;
            
        }
        
        //Se ci sono caratteri sbagliati rendo not_maching ramo dell'albero.
        
        i = 0;
        t_node *curr = tree;
        while(i<len){
            int currIndex = charToIndex(toBeCheckedWord[i]);
            
            if(buffer[i] == '+'){
                
                for(int j=0;j<N;j++){
                    if(curr->pointers[j] != NULL){
                        curr->pointers[j]->matches=0;
                    }
                }
                curr->pointers[currIndex]->matches = 1;
                
                curr = curr->pointers[currIndex];
            }
            else if(buffer[i] == '\\'){ // char -> '\'
                curr->pointers[currIndex]->matches = 0;
                break;
            }
            
            i++;
        }
        
        //visito in profondità e check occorrenze se non rispetto minimo quando exact = 0, esatto quando exact = 1 metto match = 0
        //N.B. se ho vincolo esatto e supero occorrenze posso mettere SUBITO match=false, altrimenti se ho solo vincolo minimo devo arrivare in fondo alla parola.
        
    }
    //filterTree(tree, len, limits);
    
    if (right_guess+wrong_pos == len){
        printf("ok\n");
        retValue = 1;
    }
    else{
        printf("%s\n",buffer);
    }
    
    
    free(buffer);
    return retValue;
    
}


void filter_plus(char c,int pos,t_node *tree){
    
    //visito in profondita fino a pos e verifico.
    if(tree == NULL || tree->hasChild == 0){
        return;
    }
    
    if(pos == 0){
        for(size_t i = 0;i<N;i++){
            if(tree->pointers[i] != NULL && c != indexToChar(i)){
                tree->pointers[i]->matches = 0;
            }
        }
        return;
    }

    for(size_t i=0;i<N;i++){
        //Rec call
        if(tree->pointers[i] != NULL && tree->matches == 1 && tree->hasChild == 1){
            filter_plusRecCall(c,pos,tree->pointers[i],0);
        }
    }
    
}

void filter_plusRecCall(char c,int pos,t_node *tree,int step){
    if(tree == NULL || tree->hasChild == 0){
        return;
    }
    
    if(step+1 == pos){
        for(size_t i=0;i<N;i++){
            if(tree->pointers[i] != NULL && c!=indexToChar(i)){
                tree->pointers[i]->matches = 0;
            }
        }
        return;
    }
    
    
    for(size_t i=0;i<N;i++){
        //Rec call
        if(tree->pointers[i] != NULL && tree->matches == 1 && tree->hasChild == 1){
            filter_plusRecCall(c,pos,tree->pointers[i],step+1);
        }
    }
}

void filter_slash(char c,int pos,t_node *tree){
    //visito in profondita fino a pos e verifico.
    if(tree == NULL || tree->hasChild == 0){
        return;
    }
    
    if(pos == 0){
        for(size_t i = 0;i<N;i++){
            if(tree->pointers[i] != NULL && c == indexToChar(i)){
                tree->pointers[i]->matches = 0;
            }
        }
        return;
    }

    for(size_t i=0;i<N;i++){
        //Rec call
        if(tree->pointers[i] != NULL && tree->matches == 1 && tree->hasChild == 1){
            filter_slashRecCall(c,pos,tree->pointers[i],0);
        }
    }
}

void filter_slashRecCall(char c,int pos,t_node *tree,int step){
    if(tree == NULL || tree->hasChild == 0){
        return;
    }
    
    if(step+1 == pos){
        for(size_t i=0;i<N;i++){
            if(tree->pointers[i] != NULL && c==indexToChar(i)){
                tree->pointers[i]->matches = 0;
            }
        }
        return;
    }
    
    
    for(size_t i=0;i<N;i++){
        //Rec call
        if(tree->pointers[i] != NULL && tree->matches == 1 && tree->hasChild == 1){
            filter_slashRecCall(c,pos,tree->pointers[i],step+1);
        }
    }
}

void filter_sign(char *word,char* buffer,int wordLen,t_node* tree,int step){
    if(tree == NULL || tree->hasChild == 0){
        return;
    }
    
    for(size_t pos=0;pos<wordLen;pos++){
        
        for(size_t i = 0;i<N;i++){
            if(tree->pointers[i] != NULL){
                if((buffer[pos] == '+' && word[pos] != indexToChar(i)) || (buffer[pos] == '\\' && word[pos] == indexToChar(i))){
                    tree->pointers[i]->matches = 0;
                }
            }
        }
        
        
    }
    
}
