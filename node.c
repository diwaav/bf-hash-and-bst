// CITE: used my node.c from assignment 5 as base for these functions
#include "node.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// The node_create() function constructs the node
// Inputs: oldspeak and newspeak
// Outputs: a pointer to the node

Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) calloc(1, sizeof(Node));
    if (oldspeak) {
        // oldspeak is not null, we can do strdup
        n->oldspeak = strdup(oldspeak);
        if (!(n->oldspeak)) { // just in case something went wrong with strdup
            free(n->oldspeak);
            n->oldspeak = NULL;
        }
    } else {
        // oldspeak is null
        n->oldspeak = NULL;
    }

    if (newspeak) {
        // newspeak is not null, we can do strdup
        n->newspeak = strdup(newspeak);
        if (!(n->newspeak)) { // just in case something went wrong with strdup
            free(n->newspeak);
            n->newspeak = NULL;
        }
    } else {
        // newspeak is null
        n->newspeak = NULL;
    }

    return n;
}

// The node_delete() function destructs a given node
// Inputs: a pointer to a pointer to the node
// Outputs: void

void node_delete(Node **n) {
    if ((*n)->oldspeak) {
        free((*n)->oldspeak);
    }
    if ((*n)->newspeak) {
        free((*n)->newspeak);
    }
    free(*n);
    *n = NULL;
    return;
}

// The node_print() function prints the given node
// Inputs: a pointer to a node
// Outputs: void

void node_print(Node *n) {
    if (n->oldspeak && n->newspeak) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    } else if (n->oldspeak) {
        printf("%s\n", n->oldspeak);
    }
    return;
}
