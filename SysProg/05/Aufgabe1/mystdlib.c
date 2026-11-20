#include "mystdlib.h"

#include <stdint.h>
#include <stddef.h>

#define ARENA_SIZE (1024u * 1024u) /* Fixed 1 MiB arena requested by the exercise. */

/* The union gives the arena the alignment required by any standard C object. */
static union {
    max_align_t alignment;
    unsigned char bytes[ARENA_SIZE];
} arena;

typedef struct Block Block;
struct Block {
    size_t size;       /* Number of usable bytes immediately following this header. */
    int is_free;       /* Nonzero means this block is available for allocation. */
    Block *next;       /* Next block in address order inside the arena. */
};

/* The union rounds each header size up to max_align_t alignment as well. */
typedef union {
    Block block;
    max_align_t alignment;
} AlignedBlock;

static Block *first_block; /* NULL until the arena is initialized on first request. */

static size_t aligned_header_size(void)
{
    return sizeof(AlignedBlock); /* Payload starts at a correctly aligned address. */
}

/* Round a request up to max_align_t alignment; return zero if it would overflow. */
static size_t align_size(size_t size)
{
    const size_t alignment = _Alignof(max_align_t);
    const size_t remainder = size % alignment;
    if (remainder == 0) {
        return size; /* Already aligned; no extra padding is needed. */
    }
    if (size > SIZE_MAX - (alignment - remainder)) {
        return 0; /* Adding the padding would wrap size_t. */
    }
    return size + alignment - remainder;
}

/* Create one initial free block spanning the whole arena, including no header twice. */
static void initialize_arena(void)
{
    if (first_block == NULL) {
        first_block = (Block *)arena.bytes; /* The first header occupies arena start. */
        first_block->size = ARENA_SIZE - aligned_header_size();
        first_block->is_free = 1;
        first_block->next = NULL; /* There is no block after the initial free region. */
    }
}

/* Split a larger block when enough room remains for a header and a payload. */
static void split_block(Block *block, size_t wanted)
{
    const size_t header = aligned_header_size();
    if (block->size < wanted || block->size - wanted <= header) {
        return; /* Keep a tiny remainder in this allocation instead of losing it. */
    }

    /* Place the new block header directly after the payload being allocated. */
    Block *remainder = (Block *)((unsigned char *)block + header + wanted);
    remainder->size = block->size - wanted - header;
    remainder->is_free = 1;
    remainder->next = block->next; /* Preserve the rest of the free-list chain. */

    block->size = wanted; /* The original block now describes only this allocation. */
    block->next = remainder;
}

/* Merge adjacent free blocks; neighboring payloads can then satisfy larger requests. */
static void merge_free_blocks(void)
{
    for (Block *block = first_block; block != NULL && block->next != NULL;) {
        Block *next = block->next;
        unsigned char *end = (unsigned char *)block + aligned_header_size() + block->size;
        if (block->is_free && next->is_free && end == (unsigned char *)next) {
            block->size += aligned_header_size() + next->size; /* Absorb next header too. */
            block->next = next->next; /* Remove the absorbed header from the list. */
            /* Stay on block: it may also merge with its new next neighbor. */
        } else {
            block = next; /* No merge here; inspect the following pair. */
        }
    }
}

void *mymalloc(size_t size)
{
    if (size == 0) {
        return NULL; /* The exercise specifies NULL for a zero-byte request. */
    }

    const size_t wanted = align_size(size);
    if (wanted == 0) {
        return NULL; /* Alignment overflow means this request cannot be represented. */
    }

    initialize_arena();
    /* First-fit search: take the first free block large enough for this request. */
    for (Block *block = first_block; block != NULL; block = block->next) {
        if (block->is_free && block->size >= wanted) {
            split_block(block, wanted); /* Keep any sufficiently large remainder free. */
            block->is_free = 0; /* Mark the chosen block used before returning it. */
            return (unsigned char *)block + aligned_header_size(); /* Skip its header. */
        }
    }
    return NULL; /* No free block can satisfy the aligned size. */
}

void myfree(void *ptr)
{
    if (ptr == NULL || first_block == NULL) {
        return; /* Freeing NULL is a no-op; an uninitialized arena has nothing to free. */
    }

    /* Every returned payload begins immediately after its allocation header. */
    Block *released = (Block *)((unsigned char *)ptr - aligned_header_size());
    released->is_free = 1; /* Make this region available to later mymalloc calls. */
    merge_free_blocks(); /* Combine it with any free blocks directly beside it. */
}

void *mycalloc(size_t count, size_t size)
{
    if (count == 0 || size == 0 || count > SIZE_MAX / size) {
        return NULL; /* Reject empty requests and multiplication overflow. */
    }

    const size_t total = count * size; /* Safe because the overflow check passed. */
    unsigned char *memory = mymalloc(total);
    if (memory == NULL) {
        return NULL; /* Preserve malloc behavior when the arena has no space. */
    }
    for (size_t i = 0; i < total; ++i) {
        memory[i] = 0; /* calloc guarantees every allocated byte starts as zero. */
    }
    return memory;
}

void *myrealloc(void *ptr, size_t size)
{
    if (ptr == NULL) {
        return mymalloc(size); /* A null old pointer is the same as a fresh allocation. */
    }
    if (size == 0) {
        myfree(ptr); /* A zero-size request releases the old block and returns NULL. */
        return NULL;
    }

    const size_t wanted = align_size(size);
    if (wanted == 0) {
        return NULL; /* Overflow/failure leaves the original allocation untouched. */
    }

    Block *block = (Block *)((unsigned char *)ptr - aligned_header_size());
    if (wanted <= block->size) {
        split_block(block, wanted); /* A smaller request can keep the same address. */
        block->is_free = 0; /* The original allocation remains in use after splitting. */
        merge_free_blocks(); /* The newly split tail may join a following free block. */
        return ptr;
    }

    /* Grow in place only if the immediately following free block is large enough. */
    Block *next = block->next;
    unsigned char *block_end = (unsigned char *)block + aligned_header_size() + block->size;
    if (next != NULL && next->is_free && block_end == (unsigned char *)next) {
        const size_t combined = block->size + aligned_header_size() + next->size;
        if (combined >= wanted) {
            block->size = combined; /* Consume the neighboring free block and its header. */
            block->next = next->next;
            split_block(block, wanted); /* Return any unused part to the free list. */
            block->is_free = 0;
            merge_free_blocks();
            return ptr; /* This resize needed no copy, so the address stays unchanged. */
        }
    }

    /* Otherwise reserve a new block before releasing the old one. */
    void *replacement = mymalloc(size);
    if (replacement == NULL) {
        return NULL; /* Keep ptr allocated and its contents unchanged on failure. */
    }

    /* Preserve exactly the bytes that are valid in both the old and requested sizes. */
    const size_t bytes_to_copy = block->size < size ? block->size : size;
    unsigned char *source = (unsigned char *)ptr;
    unsigned char *destination = (unsigned char *)replacement;
    for (size_t i = 0; i < bytes_to_copy; ++i) {
        destination[i] = source[i];
    }

    myfree(ptr); /* Release old storage only after its contents have been copied. */
    return replacement;
}
