#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>

// Contains the properties of a node
struct node {
    char letter;
    //Array of Children - pointer
    struct node* children[127];
    struct node* parent;
    int numberOfChildren;
};

// Function to recursively add files to the trie
void addToTrie(char* word, struct node* node, unsigned int size) {
    if (word[0] == '\\') {
        return;
    }
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
            childPtr->letter = currentLetter;
            //CHANGED
            *childPtr->children = (struct node*) malloc(sizeof(CHILDREN_ALLOCATION_SIZE));
            childPtr->parent = node;
            node->children[0] = childPtr;
            node->children[1] = NULL;
            node->numberOfChildren ++;

            if (size > 1) {
                addToTrie(newWord, childPtr, size - 1);
                return;
            }

        }
        // If there are children nodes that exist
        else {
            // Iterate over each child node
            for (int i = 0; i < node->numberOfChildren + 1; i++) {

                // If the node has a null pointer (a node with that letter doesn't exist)
                if (node->children[i] == NULL) {
                    //CHANGED
                    struct node *childPtr = (struct node*) malloc(sizeof(struct node*));
                    //CHANGED
                    *childPtr->children = (struct node*) malloc(sizeof(CHILDREN_ALLOCATION_SIZE));
                    childPtr->letter = currentLetter;
                    childPtr->parent = node;
                    node->children[i] = childPtr;
                    if (i < NUMBER_OF_CHILDREN - 1)
                        node->children[i+1] = NULL;
                    node->numberOfChildren ++;

                    if (size > 1) {
                        addToTrie(newWord, childPtr, size - 1);
                        return;
                    }
                }

                // If the child node at i's letter is the same as the current letter in the word
                struct node* child = node->children[i];
                if (child != NULL || child != 0x00) {
                    if (node->children[i]->letter == currentLetter) {
                        if (size > 1) {
                            addToTrie(newWord, node->children[i], size - 1);
                            return;
                        }
                    }
                }
            }
            return;
        }
    }

}

void TrieToString(struct node* node, int currentDepth) {

    if (node == NULL || node == 0x00 || node->letter == '\0')
        return;
    else {
        char indentation[currentDepth];
        if (currentDepth > 0) {
            for (int i = 0; i < currentDepth; i++) {
                indentation[i] = '-';
                if (i == currentDepth - 1)
                    indentation[i] = node->letter;
            }
        }

        printf("%s\n", indentation);

        for (int i = 0; i < node->numberOfChildren; i++) {
            if (node->children[i] != NULL) {
                if (node->children[i]->letter != '\0') {
                    TrieToString(node->children[i], currentDepth + 1);
                }
                else
                    break;
            }
            else
                break;
        }
    }
}

void backTrackTrie(struct node* node) {
    if(node->letter == ' ') {
        printf("%c", node->letter);
        return;
    }
    backTrackTrie(node->parent);
    printf("%c", node->letter);
}

void findTerminatorNode(struct node* node) {
    if (node == NULL)
        return;
    if (node->letter == '\0') {
        backTrackTrie(node);
    }
    else {
        for (int i = 0; i < node->numberOfChildren+1; i++) {
            if (node->children[i] == NULL)
                break;
            if (node->children[i]->letter == '\0')
                node->children[i]->parent = node;
            findTerminatorNode(node->children[i]);
        }
    }

}

void searchTrie(struct node* node, char* name, int size) {

    if (size > 0) {

        char newName[strlen(name) - 1];
        char letter = name[0];

        for (int i = 1; i < strlen(name); i++) {
            newName[i - 1] = name[i];
        }

        for (int i = 0; i < node->numberOfChildren; i++) {
            if (node->children[i]->letter == letter) {
                searchTrie(node->children[i], newName, size - 1);
                return;
            }
            if (i == node->numberOfChildren - 1) {
                printf("No files found\n");
            }
        }

    }
    else {
        findTerminatorNode(node);
        printf("\n");
    }
}

int main() {

    int exit = 0;

    while (exit == 0) {

        // Grabs Directory - DONE
        char input[100];
        printf("Enter a directory: >");
        scanf("%s", input);

        if (strcmp(input, "exit") == 0) {
            exit = 1;
            continue;
        }

        //Variables for getting the directory
        DIR *d;
        struct dirent *dir;
        struct stat filestat;

        //Grabs directory
        d = opendir(input);

        const int MAX_FILES = 100;
        // Counter for the indices in the array of file names
        int counter = 0;
        // Gets the list of files in a directory
        char *files[MAX_FILES];

        if (d) {
            // Loops through while there is still a valid directory
            while ((dir = readdir(d)) != NULL) {

                // Checks if the name is a file or sub-directory
                stat(dir->d_name, &filestat);
                // If sub directory, skip over
                if (S_ISDIR(filestat.st_mode) || dir->d_name[0] == '.')
                    continue;
                    // If file, then add name to the list of file names
                else
                    files[counter] = dir->d_name;

                counter++;
            }
            //Closes the directory
            closedir(d);
        }

        const int CHILDREN_ALLOCATION_SIZE = 127 * sizeof(struct node *);
        struct node head = {' ', malloc(CHILDREN_ALLOCATION_SIZE)};
        struct node *headPtr = &head;
        for (int i = 0; i < counter; i++) {
            char *file = files[i];
            if (file != NULL)
                addToTrie(file, headPtr, strlen(file));
            else
                break;
        }

        int cd = 0;

        while (cd == 0) {
            char prefix[100];
            printf(">");
            scanf("%s", prefix);

            if (strcmp(prefix, "cd") == 0)
                cd = 1;
            else if (strcmp(prefix, "ls") == 0) {
                for (int i = 0; i < counter; i++)
                    printf("%s\n", files[i]);
            }
            else
                searchTrie(headPtr, prefix, strlen(prefix));
        }
    }
    printf("Good Night");

    return 0;
}
