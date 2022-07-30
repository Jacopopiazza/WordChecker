//
//  main.c
//  WordChecker
//
//  Created by Japo on 29/05/22.
//

#include <stdio.h>
#include "Dictionary.h"

void checkWord(const char* refWord, char* toBeCheckedWord,int len,t_node* tree);

int main(int argc, const char * argv[]) {
    // insert code here...

    t_node* tree = (t_node*)malloc(sizeof(t_node));

    tree->hasChild = 0;
    tree->matches = 1;
    for(int i=0;i<N;i++){
        tree->pointers[i] = NULL;
    }
    
    addWordToTree(tree, "c_a5");
    addWordToTree(tree, "CASA");
    addWordToTree(tree, "pipo");
    addWordToTree(tree, "CASO");
    addWordToTree(tree, "NOIA");
    
    checkWord("CASA", "CASO", 4, tree);
    checkWord("CASA", "NOIA", 4, tree);
    checkWord("CASA", "pipo", 4, tree);
    
    printWords(tree, 4);
    printf("%d\n",countWords(tree, 4));
    
    
    
    return 0;
}


void checkWord(const char* refWord, char* toBeCheckedWord,int len,t_node* tree){
    
    char *buffer = malloc(sizeof(char) * (len+1));
    int right_guess = 0;
    int checked[len];
    
    
    //check posizione singola
    buffer[len] = '\0';
    for(int i = 0;i<len;i++){
        if(refWord[i] == toBeCheckedWord[i]){
            buffer[i] = '+';
            right_guess++;
            checked[i] = 1;
        }
        else{
            buffer[i] = '\\';
            checked[i]=0;
        }
    }
    
    if(right_guess < len){
        
        //Non considero eventuali caratteri iniziali che già combaciano nella ricerca di fuori posizione
        //Cerco dove finiscono eventuali caratteri che già matchano
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
                        break;
                    }
                    j++;
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
            else{ // char -> '\' o '|'
                curr->pointers[currIndex]->matches = 0;
                break;
            }
            
            i++;
        }
    
    }
    
    
    printf("%s\n",buffer);
    free(buffer);
    
}
