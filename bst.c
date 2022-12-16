// CITE: Professor's binary search lecture used as pseudocode for delete (is same),
// height, find, and insert (all similar in concept but different in implementation)
#include "bst.h"
#include "node.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// branches counts the number of links traversed while finding
// and inserting with BST
uint64_t branches;

// The bst_create() function contructs a binary search tree
// Inputs: void
// Outputs: a pointer to a null node

Node *bst_create(void) {
    return NULL;
}

// The bst_delete() function destructs the binary search tree
// Inputs: a pointer to a pointer to a root node
// Outputs: void

void bst_delete(Node **root) {
    if (*root) {
        bst_delete(&(*root)->left);
        bst_delete(&(*root)->right);
        node_delete(root);
    }
    return;
}

// The bst_height() function finds the height of the binary search tree
// Inputs: a pointer to a root node
// Outputs: height of the binary search tree

uint32_t bst_height(Node *root) {
    if (root) {
        if (bst_height(root->left) > bst_height(root->right)) {
            return bst_height(root->left) + 1; // add 1 for root itself·
        } else {
            return bst_height(root->right) + 1; // add 1 for root itself
        }
    } else {
        return 0; // root is null
    }
}

// The bst_size() function finds the size of the binary search tree
// Inputs: a pointer to a root node
// Outputs: size of the binary search tree

uint32_t bst_size(Node *root) {
    if (root) {
        // add the sizes of the left side, right side, and 1 for the root itself
        return bst_size(root->left) + bst_size(root->right) + 1;
    } else {
        return 0;
    }
}

// The bst_find() finds the root containing the given oldspeak
// in the binary search tree
// Inputs: a pointer to a root node, oldspeak that we finding
// Outputs: node containing the given oldspeak

Node *bst_find(Node *root, char *oldspeak) {
    if (root) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            // the string is larger, look to the left
            branches += 1;
            return bst_find(root->left, oldspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            // the string is smaller, look to the right
            branches += 1;
            return bst_find(root->right, oldspeak);
        }
    }
    return root; // returns root if null, and itself if root->oldspeak == oldspeak
}

// The bst_insert() function inserts a given oldspeak and newspeak
// to the binary search tree
// Inputs: a pointer to a root node, oldspeak and newspeak
// Outputs: the node that was inserted into

Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    if (root && oldspeak) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            // string is larger so go left
            branches += 1; // going down a branch, so add 1
            root->left = bst_insert(root->left, oldspeak, newspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            // string is larger so go right
            branches += 1; // going down a branch, so add 1
            root->right = bst_insert(root->right, oldspeak, newspeak);
        }
        return root;
    } else if (oldspeak == NULL) {
        return NULL;
    } else {
        return node_create(oldspeak, newspeak);
    }
}

// The bst_print() function prints out the binary search tree
// Inputs: a pointer to a root node
// Outputs: void

void bst_print(Node *root) {
    if (root) {
        bst_print(root->left);
        node_print(root);
        bst_print(root->right);
    }
    return;
}
