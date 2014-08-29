#define _POSIX_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "ring.h"

#define SIZE 100

int pool[SIZE];

void* push_func(void *p)
{
	struct ring *r = (struct ring *)p;
	unsigned int seed;

	while(1) {
		ring_push(r, &pool[rand_r(&seed)%SIZE]);
		usleep(rand_r(&seed)%100000);
	}

	return NULL;
}

void* pop_func(void *p)
{
	struct ring *r = (struct ring *)p;
	unsigned int seed;
	void *ptr;

	while(1) {
		ptr = ring_pop(r);
		if (ptr)
			printf("poped %d\n", *(int*)ptr);
		else
			printf("poped NULL\n");
		usleep(rand_r(&seed)%100000);
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t thr_push;
	pthread_t thr_pop;

	struct ring ring;
	int i;

	for (i = 0; i < SIZE; ++i) {
		pool[i] = i;
	}

	srand(time(NULL));
	ring_init(&ring, 2);

	pthread_create(&thr_push, NULL, push_func, &ring);
	pthread_create(&thr_pop, NULL, pop_func, &ring);

	pthread_join(thr_push, NULL);
	pthread_join(thr_pop, NULL);

	ring_destroy(&ring);

	return 0;
}
