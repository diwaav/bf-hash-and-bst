// CITE: How to compile regex & use the parser module, specifically,
// (word = next_word(stdin , &re)) is from the instructions, main function made
// using basic pseudocode given in instructions
#include "set.h"
#include "salts.h"
#include "messages.h"
#include "bf.h"
#include "bst.h"
#include "bv.h"
#include "ht.h"
#include "node.h"
#include "parser.h"
#include "speck.h"

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <getopt.h>
#include <errno.h>
#include <regex.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// The message() function prints out information about how to properly use the file
// Inputs: void
// Outputs: void

void message(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "  A word filtering program for the GPRSC.\n"
                    "  Filters out and reports bad words parsed from stdin.\n"
                    "\n"
                    "USAGE\n"
                    "  ./banhammer [-hs] [-t size] [-f size]\n"
                    "\n"
                    "OPTIONS\n"
                    "  -h           Program usage and help.\n"
                    "  -s           Print program statistics\n"
                    "  -t size      Specify hash table size (default: 2^16).\n"
                    "  -f size      Specify Bloom filter size (default: 2^20).\n");
    return;
}

// The lower() function finds the lowercase of a given string
// Inputs: string (character array)
// Outputs: string (character array) after lowecase applied

char *lower(char *word) {
    uint32_t i = 0;
    // keep looping until null character found
    while (word[i]) {
        // make the character lowercase
        word[i] = tolower(word[i]);
        i += 1;
    }
    return word;
}

typedef enum { VERBOSE, THOUGHTCRIME, RIGHTSPEAK } Banhammer;
#define OPTIONS "hst:f:"

int main(int argc, char **argv) {
    // Declare default values and set
    Set chosen = empty_set();
    Set punishment = empty_set();
    int option = 0;
    uint64_t table_size = pow(2, 16);
    uint64_t filter_size = pow(2, 20);

    // Parse command line inputs
    // If help was chosen or something went wrong, send user to
    // the usage message and end the program
    while ((option = getopt(argc, argv, OPTIONS)) != -1) {
        switch (option) {
        case 'h': message(); return 0;
        case 's':
            // verbose printing was chosen
            chosen = insert_set(VERBOSE, chosen);
            break;
        case 'f':
            // bloom filter size chosen
            filter_size = (uint64_t) strtoul(optarg, NULL, 10);
            break;
        case 't':
            // hash table size chosen
            if (strtol(optarg, NULL, 10) < 0 || strtol(optarg, NULL, 10) > 598048253) {
                // the table size given is less than 0 (negative) or too large
                printf("Failed to create hash table.\n");
                return 1;
            }
            table_size = (uint64_t) strtoul(optarg, NULL, 10);
            break;
        default: message(); return 1;
        }
    }

    // if the values are 0
    if (filter_size == 0) {
        printf("Invalid Bloom filter size.\n");
        return 1;
    }
    if (table_size == 0) {
        printf("Invalid hash table size.\n");
        return 1;
    }

    // opening files
    FILE *bad = fopen("badspeak.txt", "r");
    FILE *new = fopen("newspeak.txt", "r");

    // create a bloom filter
    BloomFilter *bf = bf_create(filter_size);
    HashTable *ht = ht_create(table_size);

    char oldspeak[1024] = "";
    char newspeak[1024] = "";
    // read in a list of badspeak words and add it to the bloom filter
    while (fscanf(bad, "%s\n", oldspeak) != -1) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, NULL);
    }

    while (fscanf(new, "%s %s\n", oldspeak, newspeak) != -1) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, newspeak);
    }

    // regex compile, made like in instructions
    regex_t re;
    if (regcomp(&re, "[a-zA-Z0-9_'-]+", REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        // close files and clear memory
        bf_delete(&bf);
        ht_delete(&ht);
        fclose(new);
        fclose(bad);
        return 1;
    }

    // making two binary search trees to hold the bad words
    char *word = "";
    Node *badwords_list = bst_create();
    Node *badwords_list_with_newspeak = bst_create();
    // reading and filtering words
    while ((word = next_word(stdin, &re)) != NULL) {
        // make the word lowercase
        word = lower(word);
        if (bf_probe(bf, word)) {
            // word is probably in bloom filter
            Node *n = ht_lookup(ht, word);
            if (n && !n->newspeak) {
                // there is no newspeak, thoughtcrime
                punishment = insert_set(THOUGHTCRIME, punishment);
                badwords_list = bst_insert(badwords_list, word, NULL);
            }
            if (n && n->newspeak) {
                // contains word and newspeak, needs counseling on Rightspeak
                punishment = insert_set(RIGHTSPEAK, punishment);
                badwords_list_with_newspeak
                    = bst_insert(badwords_list_with_newspeak, word, n->newspeak);
            }
        }
    }

    // print statistics OR print the crime message
    if (member_set(VERBOSE, chosen)) {
        // average BST SIZE
        printf("Average BST size: %.6lf\n", (double) ht_avg_bst_size(ht));
        // average BST height
        printf("Average BST height: %.6lf\n", (double) ht_avg_bst_height(ht));
        // average branches traversed
        printf("Average branches traversed: %.6lf\n", (double) branches / lookups);
        // hash table load
        printf("Hash table load: %.6lf%%\n", 100 * ((double) ht_count(ht) / (double) ht_size(ht)));
        // bloom filter load
        printf(
            "Bloom filter load: %.6lf%%\n", 100 * ((double) bf_count(bf) / (double) bf_size(bf)));
    } else {
        // if there are "bad words" indicated
        // thoughtcrime and rightspeak counselling
        if (member_set(THOUGHTCRIME, punishment) && member_set(RIGHTSPEAK, punishment)) {
            printf("%s", mixspeak_message);
            bst_print(badwords_list);
            bst_print(badwords_list_with_newspeak);
        }
        // thoughtcrime and not rightspeak counselling
        if (member_set(THOUGHTCRIME, punishment) && !member_set(RIGHTSPEAK, punishment)) {
            printf("%s", badspeak_message);
            bst_print(badwords_list);
            bst_print(badwords_list_with_newspeak);
        }
        // rightspeak counselling and not thoughtcrime
        if (!member_set(THOUGHTCRIME, punishment) && member_set(RIGHTSPEAK, punishment)) {
            printf("%s", goodspeak_message);
            bst_print(badwords_list);
            bst_print(badwords_list_with_newspeak);
        }
    }

    // clear memory allocated, and close files
    bf_delete(&bf);
    ht_delete(&ht);
    bst_delete(&badwords_list);
    bst_delete(&badwords_list_with_newspeak);
    clear_words();
    regfree(&re);
    fclose(new);
    fclose(bad);
    return 0;
}
