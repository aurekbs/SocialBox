#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "util.h"
#include "geometry.h"
#include "algo.h"

int main(int argc, char *argv[]) {
	if (argc != 2)
		ShowMessage("expe:main: invalid number of arguments", 1);

	char * input_filename = argv[1];

	clock_t start, end;

	struct list_t * segments = load_segments(input_filename);
	struct list_t * intersections;

	start = clock();
	intersections = all_pairs(segments);
	end = clock();
	double cpu_time_used0 = ((double) (end - start)) / CLOCKS_PER_SEC;
	delete_list(intersections, free);

	start = clock();
	intersections = BentleyOttmann(segments);
	end = clock();
	double cpu_time_used1 = ((double) (end - start)) / CLOCKS_PER_SEC;
	delete_list(intersections, free);

	delete_list(segments, free_segment);

	printf("%lf %lf	", cpu_time_used0, cpu_time_used1);

	return EXIT_SUCCESS;
}