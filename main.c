#ifndef _main_c_
#define _main_c_


#include "headers.h"


int
main(int argc, char** argv)
{
	struct Puzzle* initial_state;
#if BENCHMARK_MODE
	clock_t        benchmark_overall_start;
	clock_t        benchmark_overall_end;
	clock_t        benchmark_start;
	clock_t        benchmark_end;

	benchmark_overall_start = clock();
#endif
	/*size_t         num_solutions;
	struct Puzzle* solution_states;*/

	setprogname(argv[0]);

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s [input.txt]\n", getprogname());
		exit(EXIT_FAILURE);
	}

#if BENCHMARK_MODE
	benchmark_start = clock();
#endif

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

#if BENCHMARK_MODE
	benchmark_end = clock();
	printf(
		"BENCHMARK : setvbuf() = %li cycles\n",
		benchmark_end - benchmark_start
	);
#endif

#if BENCHMARK_MODE
	benchmark_start = clock();
#endif

	initial_state = puzzle_parse(argv[1]);

#if BENCHMARK_MODE
	benchmark_end = clock();
	printf(
		"BENCHMARK : puzzle_parse() = %li cycles\n",
		benchmark_end - benchmark_start
	);
#endif

	/* memcpy() */

	#if BENCHMARK_MODE
	benchmark_start = clock();
	#endif

	puzzle_print(initial_state);

	#if BENCHMARK_MODE
	benchmark_end = clock();
	printf(
		"BENCHMARK : puzzle_print() = %li cycles\n",
		benchmark_end - benchmark_start
	);
	#endif

	free(initial_state);

#if BENCHMARK_MODE
	benchmark_overall_end = clock();
	printf(
		"BENCHMARK : main() = %li cycles\n",
		benchmark_overall_end - benchmark_overall_start
	);
#endif
	return EXIT_SUCCESS;
}


#endif
