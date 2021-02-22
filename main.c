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
void addToTrie(char* name, struct node* node, unsigned int size) {
    // Terminator
    if (name[0] == '\\') {
        return;
    }
    // Checks if the size of the word is 0 and then finishes the recursion
    if (size > 0) {
        // A new word is the old word minus the first index in the word. This reduces the word by one letter each time
        /// The recursive function is called
        char newName[strlen(name) - 1];
        // Subtracts the first letter from the original word and adds the rest to new word
        for (int i = 1; i < strlen(name); i++)
            newName[i - 1] = name[i];

        //First letter from the name
        const char currentLetter = name[0];
        // Max number of children a node can have - 127 ASCII values
        const int NUMBER_OF_CHILDREN = 127;
        //Integer value for the amount of space to allocate for a node's children
        const unsigned int CHILDREN_ALLOCATION_SIZE = NUMBER_OF_CHILDREN * sizeof(struct node *);

        // If any children nodes don't exist
        if (node->children[0] == NULL || node->children[0]->letter == '\0') {
            // Makes memory for a new child node
            struct node* childPtr = (struct node*) malloc(sizeof(struct node*));
            // Sets the current letter to the node's letter value
            childPtr->letter = currentLetter;
            // Makes memory for the child node to have children nodes
            *childPtr->children = (struct node*) malloc(sizeof(CHILDREN_ALLOCATION_SIZE));
            // Points the child nodes parent pointer to the current node
            childPtr->parent = node;
            // Sets the current node's child at index 0 to the child node
            node->children[0] = childPtr;
            // Sets the next child indice to NULL for easy access
            node->children[1] = NULL;
            // Increases the count size for children nodes
            node->numberOfChildren ++;

            // If the size of the name is greater than 1 than recursively call the function
            if (size > 1) {
                addToTrie(newName, childPtr, size - 1);
                return;
            }

        }
        // If there are children nodes that exist
        else {
            // Iterate over each child node
            for (int i = 0; i < node->numberOfChildren + 1; i++) {

                // If the node has a null pointer (a node with that letter doesn't exist)
                if (node->children[i] == NULL) {
                    // Makes new child node
                    struct node *childPtr = (struct node*) malloc(sizeof(struct node*));
                    *childPtr->children = (struct node*) malloc(sizeof(CHILDREN_ALLOCATION_SIZE));
                    childPtr->letter = currentLetter;
                    childPtr->parent = node;
                    node->children[i] = childPtr;
                    // Makes the next child node index NULL for easy access
                    if (i < NUMBER_OF_CHILDREN - 1)
                        node->children[i+1] = NULL;
                    node->numberOfChildren ++;
                    // Recursively call function on child node
                    if (size > 1) {
                        addToTrie(newName, childPtr, size - 1);
                        return;
                    }
                }

                // If the child node at i's letter is the same as the current letter in the word
                struct node* child = node->children[i];
                if (child != NULL) {
                    // Checks if a child node has the same letter as current Letter
                    if (node->children[i]->letter == currentLetter) {
                        if (size > 1) {
                            // If so, then instead of making a new node, recursively call the function on that child node
                            addToTrie(newName, node->children[i], size - 1);
                            return;
                        }
                    }
                }
            }
            return;
        }
    }

}

//void trieToString(struct node* node, int currentDepth) {
//
//    if (node == NULL || node == 0x00 || node->letter == '\0')
//        return;
//    else {
//        char indentation[currentDepth];
//        if (currentDepth > 0) {
//            for (int i = 0; i < currentDepth; i++) {
//                indentation[i] = '-';
//                if (i == currentDepth - 1)
//                    indentation[i] = node->letter;
//            }
//        }
//
//        printf("%s\n", indentation);
//
//        for (int i = 0; i < node->numberOfChildren; i++) {
//            if (node->children[i] != NULL) {
//                if (node->children[i]->letter != '\0') {
//                    trieToString(node->children[i], currentDepth + 1);
//                }
//                else
//                    break;
//            }
//            else
//                break;
//        }
//    }
//}

//Recursive Function for a terminating node to backtrack through the trie and find each parent node and return the letter
void backTrackTrie(struct node* node) {
    // If the node is the head node
    if(node->letter == ' ') {
        // stop the recursion
        return;
    }
    // If the node is not the head node, recursively call the function on the parent node
    backTrackTrie(node->parent);
    // Prints out the letter of the node. Since the function is backtracking, printing the letter has to be done after
    // The recursive call to make the string appear to be in the right order
    printf("%c", node->letter);
}

// Finds all terminating nodes branching from a single node
void findTerminatorNodes(struct node* node) {
    // Makes sure there are no bad memory accesses
    if (node == NULL)
        return;
    // If the node is a terminating node, then call the backtracking algorithm on that node
    if (node->letter == '\0') {
        backTrackTrie(node);
    }
    // If the node is not a terminating node
    else {
        // Iterate over each child node that the current node has
        for (int i = 0; i < node->numberOfChildren+1; i++) {
            // Makes sure there are no bad memory accesses
            if (node->children[i] == NULL)
                break;
            // Checks ahead of the trie to see if the next node coming up is a terminating node
            if (node->children[i]->letter == '\0')
                // Sets the child nodes parent to the current node to be able to backtrack trough the trie
                node->children[i]->parent = node;
            // Recursively calls the function to find the terminating node
            findTerminatorNodes(node->children[i]);
        }
    }

}

// Searching through the trie with a given word
void searchTrie(struct node* node, char* name, int size) {

    if (size > 0) {
        // Reduces the word by one letter each time the recursive function is called
        char newName[strlen(name) - 1];
        // Grabs the first letter from the word
        char letter = name[0];
        // Transfers over each letter to the new word except for the first letter in the word
        for (int i = 1; i < strlen(name); i++)
            newName[i - 1] = name[i];

        // Iterates over each child node to find if a letter matches up with the first letter of the word
        for (int i = 0; i < node->numberOfChildren; i++) {
            // If a child node has the same letter as the first node letter
            if (node->children[i]->letter == letter) {
                // recursively call the function to find the next matching node
                searchTrie(node->children[i], newName, size - 1);
                return;
            }
            // If the loop reached the last child node and found no match then return no files found (DEBUGGING)
            if (i == node->numberOfChildren - 1) {
                printf("No files found\n");
            }
        }
    }
    // If a complete match of nodes are found, call findTerminatorNode function to find all the terminating nodes to be able to
    // back track through the trie
    else {
        findTerminatorNodes(node);
        printf("\n");
    }
}

int main() {

    //When to break the loop
    int exit = 0;

    while (exit == 0) {

        // Grabs Directory
        char input[100];
        printf("Enter a directory: >");
        scanf("%s", input);

        //Checks to see if the user wants to exit out
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
        // creates the start of the trie
        struct node head = {' ', malloc(CHILDREN_ALLOCATION_SIZE)};
        struct node *headPtr = &head;

        //Loop through each file in the list
        for (int i = 0; i < counter; i++) {
            char *file = files[i];
            if (file != NULL)
                // Places each file name in the trie
                addToTrie(file, headPtr, strlen(file));
            else
                break;
        }

        // Checks to see if the user wants to switch directories
        int cd = 0;

        while (cd == 0) {
            // Prefix for the file lookup
            char prefix[100];
            printf(">");
            scanf("%s", prefix);

            // Checks to see if the user wants to switch directories
            if (strcmp(prefix, "cd") == 0)
                cd = 1;
            // Lists all the files in the directory
            else if (strcmp(prefix, "ls") == 0) {
                //trieToString(headPtr, 0);
                // Prints each file
                for (int i = 0; i < counter; i++)
                    printf("%s\n", files[i]);
            }
            // What ever else try to autocomplete
            else
                searchTrie(headPtr, prefix, strlen(prefix));
        }
    }
    printf("...");

    return 0;
}
