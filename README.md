# WordChecker

# Data Structures and Algorithms 2021/22 - Polimi
Prova finale Algoritmi e Strutture Dati

## Grade
30 / 30 with honors

## Warning

This code is not meant to be a final product. Through this code, it is possible to easily implement a game such as Wordle, but:

1. there is no input check;
2. the user is supposed to enter the word to guess and then guess the word.
3. the 4 instructions to be entered are coded in the Italian language.
4. there is no GUI.
   
Although, a few adjustments are enough to make it a playable game.

## Game Rules

A word consists of $k$ characters from $a$ to $z$, $A$ to $Z$, $0$ to $9$, "-", or "_".
The system will read information and instructions from stdin and print strings as follows:

1. The value of $k$, which is the length for the current game;
2. A sequence of words, composing the dictionary of allowed words (Note: no duplicates allowed);
3. The instruction "+nuova_partita".
    
When this happens, the game begins. The system will now read:

1. A reference word (Note: it must be an allowed word);
2. The value of $n$, which is the maximum number of attempts;
3. A sequence of words to compare to the reference or instructions to execute.

For each word read, the system prints a string of $k$ characters according to the following rules:
1. If the $i$-th character in the read word matches the $i$-th character in the reference word, the $i$-th printed character will be "+", indicating the correct position;
2. If the $i$-th character in the read word does not match any character in the reference word, the $i$-th printed character will be "/";
3. If the $i$-th character in the read word does not match the $i$-th character in the reference word, but matches another character in the reference word, and let $a$ be the number of characters in the read word that match the $i$-th character and are in the right position, and $b$ be the number of characters in the reference word that match the $i$-th character, if there are fewer than $a-b$ characters in lower positions than $i$ in the read word, then the $i$-th output character will be "|"; otherwise, it will be "/".
4. If the system reads the instruction "+stampa_filtrate", it prints, in lexicographic order, the set of allowed words in the dictionary that are still suitable.
5. If the system reads the instruction "+inserisci_inizio", it will start reading a new sequence of words to add to the allowed words in the dictionary, until it reads "+inserisci_fine" (Note: no duplicates allowed).
6. If in the sequence of words to compare, if the system reads an attempted word that is not allowed, it prints "not_exists". If the system reads the reference word, it prints "ok". If the system reads $n$ allowed words that were not the reference word, it prints "ko".
When the game ends, it can only read "+nuova_partita" to start a new game or "+inserisci_inizio" to insert more words into the dictionary.

# Data Structure

I chose to implement the dictionary using a Radix Tree, as this structure allows efficient storage and retrieval of words.
This choice allows me to:
1. Retrieve a word, or check its presence, in the dictionary in $O(k)$ time;
2. Add a new word to the dictionary in $O(k)$ time;
3. Apply filters efficiently during a game. Thanks to this structure, it is possible to filter entire sub-tries as soon as the first mismatched letter is encountered.
