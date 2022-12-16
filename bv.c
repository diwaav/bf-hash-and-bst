// CITE: Professor's code comments bit vectors used as base/pseudocode
// I also used code.c from my assignment 5 for all
// of the functions
#include "bv.h"
#include <stdio.h>
#include <stdlib.h>

// Structure for Bit Vector
// length = length of bit vector
// vector = the array containing the bit vector

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// The bv_create() function creates a bit vector
// Inputs: the length of the bit vector
// Outputs: a pointer to the bit vector

BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) calloc(1, sizeof(BitVector));
    if (bv) {
        // set the length and make the vector
        bv->length = length;
        bv->vector = (uint8_t *) calloc(length, sizeof(uint8_t));
    }
    return bv;
}

// The bv_print() function prints each bit in the bit vector
// Inputs: a pointer to the bit vector
// Outputs: void

void bv_print(BitVector *bv) {
    for (uint64_t i = 0; i < bv->length; i += 1) {
        printf("%u\n", bv_get_bit(bv, i));
    }
    return;
}

// The bv_delete() function destructs the bit vector
// Inputs: a pointer to a pointer to the bit vector
// Outputs: void

void bv_delete(BitVector **bv) {
    if (*bv) {
        free((*bv)->vector);
        free(*bv);
        *bv = NULL;
    }
    return;
}

// The bv_length() function returns the length of the bit vector
// Inputs: a pointer to the bit vector
// Outputs: the length of the bit vector

uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

// The bv_set_bit() function sets a bit at the given index
// Inputs: a pointer to the bit vector, index number
// Outputs: true or false depending on if setting was successful

bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (bv && (i < bv->length)) {
        bv->vector[i / 8] |= (0x1 << i % 8);
        return true;
    } else {
        return false;
    }
}

// The bv_clr_bit() function clears a bit at the given index
// Inputs: a pointer to the bit vector, index number
// Outputs: true or false depending on if clearing was successful

bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (bv && (i < bv->length)) {
        bv->vector[i / 8] &= ~((uint8_t) 0x1 << i % 8);
        return true;
    } else {
        return false;
    }
}

// The bv_get_bit() function finds the value of the bit at the
// given index
// Inputs: a pointer to the bit vector, index number
// Outputs: true or false depending on if the bit was 1 or 0
// at that given index

bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (bv && (i < bv->length)) {
        return ((bv->vector[i / 8] >> i % 8) & 0x1);
    } else {
        return false;
    }
}
