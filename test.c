#include <stdio.h>
#include "ring.h"

#define SIZE 100

int pool[SIZE];

int main(int argc, char *argv[])
{
	struct ring ring;
	int i;

	for (i = 0; i < SIZE; ++i) {
		pool[i] = i;
	}

	ring_init(&ring, 2);

	for (i = 0; i < SIZE; ++i) {
		if (!ring_push(&ring, &pool[i]))
			break;
	}

	for (i = 0; i < SIZE; ++i) {
		printf("%d %d\n", i, *(int*)ring.slot[i]);
	}

	while (1) {
		int *ptr = (int*)ring_pop(&ring);
		if (!ptr)
			break;
		printf("pop %d\n", *ptr);
	}

	ring_destroy(&ring);

	return 0;
}
