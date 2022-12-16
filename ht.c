// CITE: HashTable structure given to us by instructions, and Hashing
// lecture is used as pseudocode for find, insert, delete. Used instructions
// for hash() % size structure for index
#include "ht.h"
#include "node.h"
#include "bst.h"
#include "salts.h"
#include "speck.h"

#include <stdio.h>
#include <stdlib.h>

// lookups counts the number of times lookups and insert is called for
// a hash table
uint64_t lookups;

// Stucture for a Hash Table
// salt = salt array for the hash table
// size = size of the hash table
// trees = array of nodes

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

// The ht_create() function constructs the hash table
// Inputs: size of the hash table
// Outputs: a pointer to a hash table

HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) calloc(size, sizeof(HashTable));
    if (ht) {
        // set salts, size, and create trees
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
        ht->size = size;
        ht->trees = (Node **) calloc(size, sizeof(Node *));
        // start with null nodes
        for (uint32_t i = 0; i < size; i += 1) {
            ht->trees[i] = NULL;
        }
    }
    return ht;
}

// The ht_print() function prints the hash table
// Inputs: a pointer to a hash table
// Outputs: void

void ht_print(HashTable *ht) {
    if (ht) {
        for (uint32_t i = 0; i < ht->size; i += 1) {
            bst_print(ht->trees[i]);
        }
    }
    return;
}

// The ht_delete() function destructs the hash table
// Inputs: a pointer to a pointer to a hash table
// Outputs: void

void ht_delete(HashTable **ht) {
    if ((*ht) && (*ht)->trees) {
        // delete each tree
        for (uint32_t i = 0; i < (*ht)->size; i += 1) {
            if ((*ht)->trees[i]) {
                bst_delete(&((*ht)->trees[i]));
            }
        }
        free((*ht)->trees);
        free(*ht);
        *ht = NULL;
    }
    return;
}

// The ht_size() function finds the size of the hash table
// Inputs: a pointer to a hash table
// Outputs: the size of the hash table

uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

// The ht_lookup() function finds the node that contains the given
// oldspeak
// Inputs: a pointer to a hash table, the oldspeak to lookup
// Outputs: the node with the oldspeak

Node *ht_lookup(HashTable *ht, char *oldspeak) {
    if (ht && oldspeak) {
        lookups += 1;
        uint32_t index = hash(ht->salt, oldspeak) % ht_size(ht);
        // returns the node with that oldspeak
        return bst_find(ht->trees[index], oldspeak);
    } else {
        return NULL;
    }
}

// The ht_insert() function inserts an oldspeak into the hash table
// Inputs: a pointer to a hash table, the oldspeak and newspeak
// Outputs: void

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    if (ht && oldspeak) {
        lookups += 1;
        uint32_t index = hash(ht->salt, oldspeak) % ht_size(ht);
        // if it does not exist, it makes a new node there
        // if it does exist, new value replaces it
        ht->trees[index] = bst_insert(ht->trees[index], oldspeak, newspeak);
    }
    return;
}

// The ht_count() function counts the number of non-null BSTs in
// the hash table
// Inputs: a pointer to a hash table
// Outputs: the count of non-null BSTs

uint32_t ht_count(HashTable *ht) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht->size; i += 1) {
        if (ht->trees[i]) {
            // add to count if node is valid
            count += 1;
        }
    }
    return count;
}

// The ht_avg_bst_size() function finds the average size of the
// binary search tree (BST)
// Inputs: a pointer to a hash table
// Outputs: the average size of the BST

double ht_avg_bst_size(HashTable *ht) {
    double sum = 0;
    // find the total size
    for (uint32_t i = 0; i < ht_size(ht); i += 1) {
        sum += bst_size(ht->trees[i]);
    }
    // divide the total size by count
    if (ht_count(ht)) {
        return (sum / ht_count(ht));
    } else {
        return 0;
    }
}

// The ht_avg_bst_height() function finds the average height of the
// binary search tree (BST)
// Inputs: a pointer to a hash table
// Outputs: the average height of the BST

double ht_avg_bst_height(HashTable *ht) {
    double sum = 0;
    // find the total height
    for (uint32_t i = 0; i < ht_size(ht); i += 1) {
        sum += bst_height(ht->trees[i]);
    }
    // divide the total height by count
    if (ht_count(ht)) {
        return (sum / ht_count(ht));
    } else {
        return 0;
    }
}
