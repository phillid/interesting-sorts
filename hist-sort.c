/**
 * Histogram sort
 *
 * This sort is interesting in that it is a "comparison-free" sort. That is to
 * say that it doesn't ever directly compare two items against each other to
 * determine the larger one, much like conventional sorting algorithms does.
 *
 * Instead, a histogram is built by counting the frequency of each value in
 * the input set. Then, a sorted output set is constructed from this histogram
 * since it is implicitly in order.
 *
 * This has the disadvantage of becoming heavy on memory usage when the input
 * values increase in range. Theoretically, if the range was high, but sparse,
 * some simple compression could be applied. There's an area for
 * experimentation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RANGE 10000
#define COUNT 10000000

int is_sorted(unsigned int *data, size_t length)
{
	size_t i = 0;

	for (i = 0; i < length-1; i++)
		if (data[i] > data[i+1])
			return 0;

	return 1;
}

void fill_random(unsigned int *data, size_t length, unsigned int max)
{
	size_t i = 0;

	for (i = 0; i < length; i++)
		data[i] = rand()%max+1;
}

void sort(unsigned int *data, size_t length, unsigned int max)
{
	size_t *hist = calloc(max+1, sizeof(size_t));
	size_t i = 0;
	size_t j = 0;

	for (i = 0; i < length; i++) {
		hist[data[i]]++;
	}

	j = 0;
	for (i = 0; i < max+1; i++) {
		for (; hist[i]; hist[i]--, j++)
			data[j] = i;
	}
}

void dump_data(unsigned int *data, size_t length)
{
	size_t i = 0;

	for (i = 0; i < length; i++)
		printf("%d, ", data[i]);

	fputc('\n', stdout);
}

int main(int argc, char **argv)
{
	unsigned int *data = calloc(COUNT, sizeof(unsigned int));

	if (!data) {
		perror("calloc");
		return 1;
	}

	srand(time(NULL));

	fill_random(data, COUNT, RANGE);

	//dump_data(data, COUNT);
	sort(data, COUNT, RANGE);
	//dump_data(data, COUNT);

	if (!is_sorted(data, COUNT)) {
		fprintf(stderr, "Failed: out of order\n");
	} else {
		printf("Success.\n");
	}
}
