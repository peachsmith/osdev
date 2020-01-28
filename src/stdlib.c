#include <stdint.h>

#include "libc/stdio.h"
#include "libc/stdlib.h"

#include "kernel/memory.h"

// number of bytes in a page frame
#define FRAME_LEN 4096

// a single page
void* page = NULL;

// a bitmap to keep track of which bytes have been allocated
uint8_t bitmap[512] = { 0 };

#define check(b)  (!(bitmap[(b / 8)] &  (1 << (b % 8))))
#define mark(b)   (bitmap[(b / 8)] |= (1 << (b % 8)))
#define unmark(b) (bitmap[(b / 8)] ^= (1 << (b % 8)))

void* malloc(size_t n)
{
	size_t i, j, k;
	uint32_t v;

	// Ensure that we have a page allocated
	if (page == NULL)
		page = k_palloc();

	// If we failed to allocate a page, return NULL
	if (page == NULL)
		return NULL;

	// Find a contiguous sequence of bytes in the page
	// that are currently available.
	for (i = 0; i < FRAME_LEN; i++)
	{
		if (check(i))
		{
			// Check if n consecutive bytes are available
			for (j = i; j < FRAME_LEN && j - i < n && check(j); j++);

			if (j - i == n)
			{
				// Mark the bytes as reserved
				for (k = i; k <= j; k++)
					mark(k);

				// Determine the offset
				v = ((uint32_t)page) | i;
				return (void*)v;
			}
		}
	}

	return NULL;
}
