#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


struct node {
    char letter;
    //Array of Children - pointer
    struct node* children[127];
    struct node* parent;
};

void addToTrie(char* word, struct node* node) {

    unsigned int size = strlen(word);

    if (size > 0) {

        char newWord[strlen(word) - 1];

        for (int i = 1; i < strlen(word); i++) {
            newWord[i - 1] = word[i];
        }

        const char currentLetter = word[0];
        const int NUMBER_OF_CHILDREN = 127;
        const unsigned int CHILDREN_ALLOCATION_SIZE = NUMBER_OF_CHILDREN * sizeof(struct node *);

        //*node->children = (struct node*) malloc(sizeof(CHILDREN_ALLOCATION_SIZE));

        // If any children nodes don't exist
        if (node->children[0] == NULL || node->children[0]->letter == '\0') {
        //if (node->children[0] == NULL) {
            //Allocating memory to make an array of pointers of child nodes - NEW
            //*node->children = (struct node*) malloc(sizeof(CHILDREN_ALLOCATION_SIZE));
            struct node* childPtr = (struct node*) malloc(sizeof(struct node*));
            // Unnecessary?
            struct node child = {currentLetter, (struct node*) malloc(sizeof(CHILDREN_ALLOCATION_SIZE))};
            childPtr = &child;
            childPtr->letter = currentLetter;
            //CHANGED
            *childPtr->children = (struct node*) malloc(sizeof(CHILDREN_ALLOCATION_SIZE));
            childPtr->parent = node;
            node->children[0] = childPtr;

            if (size > 1)
                addToTrie(newWord, childPtr);

        }
        // If there are children nodes that exist
        else {
            // Iterate over each child node
            for (int i = 0; i < NUMBER_OF_CHILDREN; i++) {

                // If the node has a null pointer (a node with that letter doesn't exist)
                if (node->children[i] == NULL) {
                    //CHANGED
                    //*node->children = (struct node*) malloc(CHILDREN_ALLOCATION_SIZE);
                    struct node *childPtr = (struct node*) malloc(sizeof(struct node*));
                    //CHANGED
                    *childPtr->children = (struct node*) malloc(sizeof(CHILDREN_ALLOCATION_SIZE));
                    childPtr->letter = currentLetter;
                    childPtr->parent = node;
                    node->children[i] = childPtr;
                    if (size > 1) {
                        addToTrie(newWord, childPtr);
                        break;
                    }
                }

                // If the child node at i's letter is the same as the current letter in the word
                struct node* child = node->children[i];
                if (child != NULL) {
                    if (node->children[i]->letter == currentLetter) {
                        if (size > 1) {
                            addToTrie(newWord, node->children[i]);
                            break;
                        }
                    }
                }
            }
        }
    }

}

int main() {

    // Grabs Directory - DONE
    char input[100];
    printf("Enter a directory: >");
    scanf("%s", input);
    DIR *d;
    struct dirent *dir;
    d = opendir(input);
    const int MAX_FILES = 100;
    // Gets the list of files in a directory
    char* files[MAX_FILES] = {};
    if (d) {
        int counter = 0;
        while ((dir = readdir(d)) != NULL) {
            if (strchr(dir->d_name, '.'))
                continue;
            files[counter] = dir->d_name;
            if (files[counter] != NULL) {
                for (int i = 0; i < strlen(files[counter]); i++)
                    files[counter][i] = (char) tolower(files[counter][i]);
            }
            counter++;
        }
        closedir(d);
    }
    const int CHILDREN_ALLOCATION_SIZE = 127 * sizeof(struct node*);
    struct node head = {' ', malloc(CHILDREN_ALLOCATION_SIZE)};
    struct node* headPtr = &head;
    for (int i = 0; i < MAX_FILES - 1; i++) {
        char* file = files[i];
        if (file != NULL)
            addToTrie(file, headPtr);
        else
            break;
    }


    //Asks user prefix and makes every letter lowercase- DONE
    char prefix[100];
    printf(">");
    //scanf("%s", prefix);
    for (int i = 0; i < strlen(prefix); i++) {
        prefix[i] = (char) tolower(prefix[i]);
    }

    return 0;
}
