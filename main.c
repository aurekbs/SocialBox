#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "geometry.h"
#include "algo.h"

void display_help(char * msg) {
	ShowMessage("", 0);
	fprintf(stderr, "%s\n", msg);
	ShowMessage("", 0);
	ShowMessage("make run in=filename out=filename algo={0,1}\n", 0);
	ShowMessage("algo:", 0);
	ShowMessage("\t0 - AllPairs (glouton)", 0);
	ShowMessage("\t1 - Bentley-Ottmann", 0);
	ShowMessage("", 1);
}

int main(int argc, char * argv[]) {
	if (argc != 4)
		display_help("Error parameter's number");
	char * input_filename = argv[1];
	char * output_filename = argv[2];
	int algo = (int) strtol(argv[3], NULL, 10);
	if (algo != 0 && algo != 1)
		display_help("Error value for parameter algo");

	struct list_t * segments = load_segments(input_filename);
	struct list_t * intersections;

	if (algo == 0) {
		intersections = all_pairs(segments);
	}
	else {
		intersections = BentleyOttmann(segments);
	}

	save_intersections(output_filename, intersections);
	delete_list(intersections, free);
	delete_list(segments, free_segment);

	return EXIT_SUCCESS;
}