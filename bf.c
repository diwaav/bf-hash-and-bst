// CITE: structure for BloomFilter is given by instructions, index format
// for hash() % size and pseudocode for how bloomfilters work
// is from the professor's lecture slides
#include "bf.h"
#include "bv.h"
#include "salts.h"
#include "speck.h"

#include <stdio.h>
#include <stdlib.h>

// Structure for Bloom Filter
// primary = primary hash function salt
// secondary = secondary hash function salt
// tertiary = tertiary hash function salt

struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

// The bf_create() function constructs a bloom filter
// Inputs: the size of the bloom filter
// Outputs: a pointer to the bloom filter

BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) calloc(size, sizeof(BloomFilter));
    if (bf) {
        // set salts and create filter
        bf->primary[0] = (uint64_t) SALT_PRIMARY_LO;
        bf->primary[1] = (uint64_t) SALT_PRIMARY_HI;
        bf->secondary[0] = (uint64_t) SALT_SECONDARY_LO;
        bf->secondary[1] = (uint64_t) SALT_SECONDARY_HI;
        bf->tertiary[0] = (uint64_t) SALT_TERTIARY_LO;
        bf->tertiary[1] = (uint64_t) SALT_TERTIARY_HI;
        bf->filter = bv_create(size);
        // if something goes wrong creating the bit vector
        if (!bf->filter) {
            bv_delete(&bf->filter);
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

// The bf_delete() function destructs the bloom filter
// Inputs: pointer to the pointer to a bloom filter
// Outputs: void

void bf_delete(BloomFilter **bf) {
    if (*bf && (*bf)->filter) {
        bv_delete(&(*bf)->filter);
        free(*bf);
        *bf = NULL;
    }
    return;
}

// The bf_size() function finds the length of the bit vector in
// the bloom filter
// Inputs: a pointer to the bloom filter
// Outputs: the length of the bit vector in the bloom filter

uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

// The bf_insert() function inserts an oldspeak into the bloom filter
// Inputs: a pointer to the bloom filter, the oldspeak to be inserted
// Outputs: void

void bf_insert(BloomFilter *bf, char *oldspeak) {
    // get the indices to use
    uint32_t pri_index = hash(bf->primary, oldspeak) % bf_size(bf);
    uint32_t sec_index = hash(bf->secondary, oldspeak) % bf_size(bf);
    uint32_t ter_index = hash(bf->tertiary, oldspeak) % bf_size(bf);
    // set the bit at given indices
    bv_set_bit(bf->filter, pri_index);
    bv_set_bit(bf->filter, sec_index);
    bv_set_bit(bf->filter, ter_index);
    return;
}

// The bf_probe() function checks if a given oldspeak is likely to
// be in the bloom filter
// Inputs: a pointer to the bloom filter, the oldspeak we are looking for
// Outputs: true or false depending on if the oldspeak is probably in the
// bloom filter

bool bf_probe(BloomFilter *bf, char *oldspeak) {
    // get the indices to use
    uint32_t pri_index = hash(bf->primary, oldspeak) % bf_size(bf);
    uint32_t sec_index = hash(bf->secondary, oldspeak) % bf_size(bf);
    uint32_t ter_index = hash(bf->tertiary, oldspeak) % bf_size(bf);
    // if all three are set, then return true
    if (bv_get_bit(bf->filter, pri_index) && bv_get_bit(bf->filter, sec_index)
        && bv_get_bit(bf->filter, ter_index)) {
        return true;
    } else {
        return false;
    }
}

// The bf_count() function counts the number of set bits in the bloom filter
// Inputs: a pointer to the bloom filter
// Outputs: the count of set bits in the bloom filter

uint32_t bf_count(BloomFilter *bf) {
    uint64_t count = 0;
    for (uint32_t i = 0; i < bv_length(bf->filter); i += 1) {
        if (bv_get_bit(bf->filter, i)) {
            // add to the count only if the bit is set
            count += 1;
        }
    }
    return count;
}

// The bf_print() function prints out the bit vector in the bloom filter
// Inputs: a pointer to the bloom filter
// Outputs: void

void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
