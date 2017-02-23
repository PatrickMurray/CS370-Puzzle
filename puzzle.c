#ifndef _puzzle_c_
#define _puzzle_c_


#include "headers.h"


struct Puzzle*
puzzle_parse(char* filepath)
{
	struct Puzzle* puzzle;
	int            fd;
	short          piece;
	char           buffer[12]; /* one line */
	ssize_t        bytes_read;

	if ((puzzle = malloc(sizeof(struct Puzzle))) == NULL)
	{
		fprintf(
			stderr,
			"%s: unable to allocate memory: %s\n",
			getprogname(),
			strerror(errno)
		);
		exit(EXIT_FAILURE);
	}

	if ((fd = open(filepath, O_RDONLY)) == -1)
	{
		fprintf(
			stderr,
			"%s: failed to open '%s': %s\n",
			getprogname(),
			filepath,
			strerror(errno)
		);
		exit(EXIT_FAILURE);
	}

	for (piece = 0; piece < 9; piece++)
	{
		if ((bytes_read = read(fd, buffer, sizeof(buffer))) == -1)
		{
			fprintf(
				stderr,
				"%s: unable to read from '%s': %s\n",
				getprogname(),
				filepath,
				strerror(errno)
			);
			exit(EXIT_FAILURE);
		}
		puzzle->pieces[piece].identifier = piece;
		
		/* top */
		puzzle->pieces[piece].edges[TOP].color = buffer[0];
		puzzle->pieces[piece].edges[TOP].type = 0;
		if (buffer[1] == '1')
		{
			puzzle->pieces[piece].edges[TOP].type = 1;
		}

		/* right */
		puzzle->pieces[piece].edges[RIGHT].color = buffer[3];
		puzzle->pieces[piece].edges[RIGHT].type = 0;
		if (buffer[4] == '1')
		{
			puzzle->pieces[piece].edges[RIGHT].type = 1;
		}

		/* bottom */
		puzzle->pieces[piece].edges[BOTTOM].color = buffer[6];
		puzzle->pieces[piece].edges[BOTTOM].type = 0;
		if (buffer[7] == '1')
		{
			puzzle->pieces[piece].edges[BOTTOM].type = 1;
		}

		/* left */
		puzzle->pieces[piece].edges[LEFT].color = buffer[9];
		puzzle->pieces[piece].edges[LEFT].type = 0;
		if (buffer[10] == '1')
		{
			puzzle->pieces[piece].edges[LEFT].type = 1;
		}
	}

	if (close(fd) == -1)
	{
		fprintf(
			stderr,
			"%s: unable to close file: %s\n",
			getprogname(),
			strerror(errno)
		);
		exit(EXIT_FAILURE);
	}

	return puzzle;
}


void
puzzle_print(struct Puzzle* puzzle) /* TODO add solution array pointer */
{
	short piece;

	printf("Input tiles:\n");

	/*  */
	for (piece = 0; piece < 9; piece++)
	{
		printf(
			"%i. <%c%hu, %c%hu, %c%hu, %c%hu>\n",
			piece + 1,
			puzzle->pieces[piece].edges[TOP].color,
			puzzle->pieces[piece].edges[TOP].type,
			puzzle->pieces[piece].edges[RIGHT].color,
			puzzle->pieces[piece].edges[RIGHT].type,
			puzzle->pieces[piece].edges[BOTTOM].color,
			puzzle->pieces[piece].edges[BOTTOM].type,
			puzzle->pieces[piece].edges[LEFT].color,
			puzzle->pieces[piece].edges[LEFT].type
		);
	}

	puzzle_print_matrix(puzzle);

	return;
}


void
puzzle_print_matrix(struct Puzzle* puzzle)
{
	short piece;

	/* print the solutions - TODO for-loop here*/
	for (piece = 0; piece < 9; piece += 3)
	{
		printf("+--------+--------+--------+\n");

		/* print the identifier and top edge information */
		printf(
			"|%hu  %c%hu   |%hu  %c%hu   |%hu  %c%hu   |\n",
			puzzle->pieces[piece].identifier,
			puzzle->pieces[piece].edges[TOP].color,
			puzzle->pieces[piece].edges[TOP].type,

			puzzle->pieces[piece+1].identifier,
			puzzle->pieces[piece+1].edges[TOP].color,
			puzzle->pieces[piece+1].edges[TOP].type,

			puzzle->pieces[piece+2].identifier,
			puzzle->pieces[piece+2].edges[TOP].color,
			puzzle->pieces[piece+2].edges[TOP].type
		);

		/* print the left and right edge */
		printf(
			"|%c%hu    %c%hu|%c%hu    %c%hu|%c%hu    %c%hu|\n",
			puzzle->pieces[piece].edges[LEFT].color,
			puzzle->pieces[piece].edges[LEFT].type,
			puzzle->pieces[piece].edges[RIGHT].color,
			puzzle->pieces[piece].edges[RIGHT].type,

			puzzle->pieces[piece+1].edges[LEFT].color,
			puzzle->pieces[piece+1].edges[LEFT].type,
			puzzle->pieces[piece+1].edges[RIGHT].color,
			puzzle->pieces[piece+1].edges[RIGHT].type,

			puzzle->pieces[piece+2].edges[LEFT].color,
			puzzle->pieces[piece+2].edges[LEFT].type,
			puzzle->pieces[piece+2].edges[RIGHT].color,
			puzzle->pieces[piece+2].edges[RIGHT].type
		);

		/* print the bottom edge */
		printf(
			"|   %c%hu   |   %c%hu   |   %c%hu   |\n",
			puzzle->pieces[piece].edges[BOTTOM].color,
			puzzle->pieces[piece].edges[BOTTOM].type,

			puzzle->pieces[piece+1].edges[BOTTOM].color,
			puzzle->pieces[piece+1].edges[BOTTOM].type,

			puzzle->pieces[piece+2].edges[BOTTOM].color,
			puzzle->pieces[piece+2].edges[BOTTOM].type
		);
	}
	printf("+--------+--------+--------+\n");

	return;
}


#endif
