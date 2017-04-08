/**
 * Histogram sort
 *
 * This sort is interesting in that it is a "comparison-free" sort. That is to
 * say that it doesn't ever directly compare two items against each other to
 * determine the larger one, much like conventional sorting algorithms does.
 *
 * It basically counts the frequency of each input value into a histogram.
 * So it'd take an input set
 *
 *     {5,4,0,5,0,1,4,2,1,1,3}
 *
 * and compress it into the histogram
 *
 *     {2,3,1,1,2,2}
 *
 * by counting the number of occurences of 0, 1, … 5. Then it just builds the
 * sorted set by walking the histogram from left to right.
 *
 *     {0,0,1,1,1,2,3,4,4,5,5}
 *
 * Essentially, a histogram is built by counting the frequency of each value in
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

int
is_sorted(unsigned int *data, size_t length) {
	size_t i = 0;

	for (i = 0; i < length-1; i++)
		if (data[i] > data[i+1])
			return 0;

	return 1;
}

void
fill_random(unsigned int *data, size_t length, unsigned int max) {
	size_t i = 0;

	for (i = 0; i < length; i++)
		data[i] = rand()%max+1;
}

void
sort(unsigned int *data, size_t length, unsigned int max) {
	size_t *hist = calloc(max+1, sizeof(size_t));
	size_t i = 0;
	size_t j = 0;

	if (!hist) {
		perror("calloc");
		return;
	}

	/* build the histogram */
	for (i = 0; i < length; i++)
		hist[data[i]]++;

	/* expand histogram to form sorted set */
	j = 0;
	for (i = 0; i < max+1; i++)
		for (; hist[i]; hist[i]--, j++)
			data[j] = i;
}

void
show_usage(const char *argv0) {
	fprintf(stderr, "usage: %s <item_count> <max_value>\n", argv0);
}

int
main(int argc, char **argv) {
	clock_t start, end;
	unsigned int *data = NULL;
	int range = 0;
	int count = 0;

	if (argc != 3) {
		show_usage(argv[0]);
		return 1;
	}

	count = atoi(argv[1]);
	range = atoi(argv[2]);

	if (count <= 0 || range <= 0) {
		show_usage(argv[0]);
		return 1;
	}

	data = calloc(count, sizeof(unsigned int));
	if (!data) {
		perror("calloc");
		return 1;
	}

	srand(time(NULL));

	fill_random(data, count, range);

	start = clock();
	sort(data, count, range);
	end = clock();

	printf("Time taken to sort: %.4f\n",
			((double)(end-start))/CLOCKS_PER_SEC);

	if (!is_sorted(data, count)) {
		fprintf(stderr, "Failed: out of order\n");
	} else {
		printf("Success.\n");
	}
}
