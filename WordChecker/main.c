//
//  main.c
//  WordChecker
//
//  Created by Japo on 29/05/22.
//

#include <stdio.h>
#include "Dictionary.h"


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
    addWordToTree(tree, "CASO");
    addWordToTree(tree, "NOIA");
    
    printWords(tree, 4);
    
    return 0;
}
