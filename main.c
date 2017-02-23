#ifndef _main_c_
#define _main_c_


#include "headers.h"


int
main(int argc, char** argv)
{
	struct Puzzle* initial_state;
	/*size_t         num_solutions;
	struct Puzzle* solution_states;*/

	setprogname(argv[0]);

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s [input.txt]\n", getprogname());
		exit(EXIT_FAILURE);
	}

	/* disable output buffering - costs */
	if (setvbuf(stdout, NULL, _IONBF, 0) != 0)
	{
		fprintf(
			stderr,
			"%s: unable to disable output buffering: %s\n",
			getprogname(),
			strerror(errno)
		);
		exit(EXIT_FAILURE);
	}

	initial_state = puzzle_parse(argv[1]);

	/* memcpy() */

	puzzle_print(initial_state);

	free(initial_state);

	return EXIT_SUCCESS;
}


#endif
