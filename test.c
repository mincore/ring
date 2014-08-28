#include <stdio.h>
#include "ring.h"

#define SIZE 100

int pool[SIZE];

int main(int argc, char *argv[])
{
	struct ring ring;
	int i;

	for (i = 0; i < SIZE; ++i)
	{
		pool[i] = i;
	}

	ring_init(&ring, 8);

	for (i = 0; i < SIZE; ++i)
	{
		if (!ring_push(&ring, &pool[i]))
			break;
		printf("ring %d:%d\n", i, *(int*)ring.slot[i%ring.cap]);
	}

	return 0;
}
