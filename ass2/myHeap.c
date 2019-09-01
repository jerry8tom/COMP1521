// COMP1521 19t2 ... Assignment 2: heap management system
// Author	:	Mukul Raj Sharma
// zID		: 	z5220980	

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myHeap.h"

/** minimum total space for heap */
#define MIN_HEAP 4096
/** minimum amount of space for a free Chunk (excludes Header) */
#define MIN_CHUNK 32


#define ALLOC 0x55555555
#define FREE  0xAAAAAAAA

/// Types:

typedef unsigned int  uint;
typedef unsigned char byte;

//typedef uintptr_t     addr; // an address as a numeric type
typedef uintptr_t addr;
/** The header for a chunk. */
typedef struct header {
	uint status;    /**< the chunk's status -- ALLOC or FREE */
	uint size;      /**< number of bytes, including header */
	byte data[];    /**< the chunk's data -- not interesting to us */
} header;

/** The heap's state */
struct heap {
	void  *heapMem;     /**< space allocated for Heap */
	uint   heapSize;    /**< number of bytes in heapMem */
	void **freeList;    /**< array of pointers to free chunks */
	uint   freeElems;   /**< number of elements in freeList[] */
	uint   nFree;       /**< number of free chunks */
};


/// Variables:

/** The heap proper. */
static struct heap Heap;


/// Functions:
static addr heapMaxAddr (void);
static void delete(header *p, int pos);
static void merge();

/** Initialise the Heap. */
int initHeap (int size)
{
	Heap.nFree = 0;
	Heap.freeElems = 0;
	
	// size < min size, size = min size(4096)
	if (size < MIN_HEAP)
		size = MIN_HEAP;
	
	// round off to nearest multiple of 4
	if ((size % 4) != 0)
		size = 4*((size/4) + 1);
	
	// point Heap.heapMem to first byte of allocated region
	Heap.heapMem = malloc(size);
	if (!Heap.heapMem)
		return -1;

	// zero out the entire region
	Heap.heapMem = calloc(1, size);
	if (!Heap.heapMem)
		return -1;
	
	// initialise the region to be a single large free-space chunk
	header *head = (header *) Heap.heapMem;
	head->status = FREE;
	head->size = size;
	
	// allocate freeList array with pointers to free chunks in heapMMem
	Heap.freeElems = size/MIN_CHUNK;
	Heap.freeList = calloc(Heap.freeElems, sizeof(int));
	if (!Heap.freeList)
		return -1;
	
	// set first item to single free-space chunk
	Heap.freeList[0] = Heap.heapMem;
	Heap.heapSize = size;
	Heap.nFree = 1;
	return 0; // this just keeps the compiler quiet
}

/** Release resources associated with the heap. */
void freeHeap (void)
{
	free (Heap.heapMem);
	free (Heap.freeList);
}

/** Allocate a chunk of memory large enough to store `size' bytes. */
void *myMalloc (int size)
{
	// printf("size is:%d\n",size);
	// printf("enter myalloc\n");
	if (size%4 != 0)
		size = 4*((size/4) + 1);
	if (size < 1)
	{
		//printf("enter size < 1");
		return NULL;
	}
	addr *curr_address;
	uint chunk_size = size + sizeof(header);
	header *chunk;

	int min = Heap.heapSize + 1;
	int pos = -1;
	//printf("nFree: %d\n",Heap.nFree);
	// search for smallest free chunk i.e. larger than size+HeaderSize
	for (int i = 0; i < Heap.nFree; i++)
	{
		//printf("enter for");
		curr_address = (addr *) Heap.freeList[i];
		chunk = (header*) curr_address;
		if ((chunk->size >= (chunk_size)) && chunk->size < min)
		{
			pos = i;
			min = chunk->size;
		}
	}
	// chunk not found, return NULL
	if (pos == -1)
		return NULL;
	chunk = (header*) Heap.freeList[pos];

	addr *return_addr;
	uint split_size = chunk_size + MIN_CHUNK;
	// if chunk < size + HeaderSize + MIN_CHUNK, allocate entire chunk
	if (chunk->size < split_size)
	{
		chunk->status = ALLOC;
		return_addr = (addr *)((char *) chunk + sizeof(header));
		for (int j = pos; j < Heap.nFree; j++)
		{
			Heap.freeList[j] = Heap.freeList[j + 1];
		}
		Heap.nFree--;
	}
	// if chunk > size + HeaderSize + MIN_CHUNK, split the chunk
	else
	{
		int size_free = chunk->size;
		// first part of the split
		chunk->status = ALLOC;
		chunk->size = chunk_size;

		int size_alloc = chunk->size;
		return_addr = (addr *)((char *) chunk + sizeof(header));
		curr_address = (addr *)((char *) Heap.freeList[pos] + chunk->size);
		chunk = (header*) curr_address;
		
		// for second part of split
		chunk->status = FREE;
		chunk->size = size_free - size_alloc;
		// move ptr to second chunk (ie free)
		Heap.freeList[pos] = curr_address;
	}
	
	return (void *) return_addr; // this just keeps the compiler quiet
}

void myFree (void *obj)
{
	// check for already FREE / NULL obj
	if (obj == NULL)
	{
		fprintf(stderr, "Attempt to free unallocated chunk\n");
		exit(1);
	}

	//printf("before\n");
	addr temp = (addr) obj - sizeof(header);
	header *headerPtr = (header *)(temp);
	// check status of obj
	if (headerPtr->status != ALLOC)
	{
		fprintf(stderr, "Attempt to free unallocated chunk\n");
		exit(1);
	}
	//printf("here\n");
	headerPtr->status = FREE;
	
	// shift elements in freeList to accomodate the new obj
	int i = Heap.nFree - 1;
	void *point = headerPtr;
	while (i >= 0 && Heap.freeList[i] > point)
	{
		Heap.freeList[i+1] = Heap.freeList[i];
		i--;
	}
	Heap.freeList[i+1] = (void*) headerPtr;
	Heap.nFree++;
	// merge the adjacent FREE chunks into a single FREE chunk
	merge();
}

/** Return the first address beyond the range of the heap. */
static addr heapMaxAddr (void)
{
	return (addr) Heap.heapMem + Heap.heapSize;
}

/** Convert a pointer to an offset in the heap. */
int heapOffset (void *obj)
{
	addr objAddr = (addr) obj;
	addr heapMin = (addr) Heap.heapMem;
	addr heapMax = heapMaxAddr ();
	if (obj == NULL || !(heapMin <= objAddr && objAddr < heapMax))
		return -1;
	else
		return (int) (objAddr - heapMin);
}

/** Dump the contents of the heap (for testing/debugging). */
void dumpHeap (void)
{
	int onRow = 0;

	// We iterate over the heap, chunk by chunk; we assume that the
	// first chunk is at the first location in the heap, and move along
	// by the size the chunk claims to be.
	addr curr = (addr) Heap.heapMem;
	while (curr < heapMaxAddr ()) {
		header *chunk = (header *) curr;

		char stat;
		switch (chunk->status) {
		case FREE:  stat = 'F'; break;
		case ALLOC: stat = 'A'; break;
		default:
			fprintf (
				stderr,
				"myHeap: corrupted heap: chunk status %08x\n",
				chunk->status
			);
			exit (1);
		}

		printf (
			"+%05d (%c,%5d)%c",
			heapOffset ((void *) curr),
			stat, chunk->size,
			(++onRow % 5 == 0) ? '\n' : ' '
		);

		curr += chunk->size;
	}

	if (onRow % 5 > 0)
		printf ("\n");
}

// delete the element in freeList at a particular position
static void delete(header *p, int pos)
{
	// shift the elements in freeList to the left
	for (int i = pos; i < (Heap.nFree - 1); i++)
	{
		Heap.freeList[i] = Heap.freeList[i+1];
	}
	Heap.nFree--;
}

// merge adjacent FREE chunks into a single FREE chunk
static void merge()
{
	int i = 0;
	while (i < Heap.nFree)
	{
		header *curr = (header *) Heap.freeList[i];
		// check if the status of adjacent chunk is a FREE one
		while (((addr) curr + curr->size) == (addr) Heap.freeList[i + 1])
		{
			header *nextFree = (header *) Heap.freeList[i+1];
			// the single chunk would have the size of sum of smaller chunks
			curr->size += nextFree->size;
			int pos = i+1;
			delete(Heap.freeList[i + 1], pos);
		}
		i++;
	}
}