#ifndef _puzzle_c_
#define _puzzle_c_


#include "headers.h"


void
puzzle_parse(char* filepath, struct Puzzle* puzzle)
{
	int     fd;
	short   piece;
	char    buffer[12]; /* one line */
	ssize_t bytes_read;

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

	for (piece = PIECE_TOP_LEFT; piece <= PIECE_BOTTOM_RIGHT; piece++)
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
		puzzle->pieces[piece].edges[EDGE_TOP].color = buffer[0];
		puzzle->pieces[piece].edges[EDGE_TOP].type = 0;
		if (buffer[1] == '1')
		{
			puzzle->pieces[piece].edges[EDGE_TOP].type = 1;
		}

		/* right */
		puzzle->pieces[piece].edges[EDGE_RIGHT].color = buffer[3];
		puzzle->pieces[piece].edges[EDGE_RIGHT].type = 0;
		if (buffer[4] == '1')
		{
			puzzle->pieces[piece].edges[EDGE_RIGHT].type = 1;
		}

		/* bottom */
		puzzle->pieces[piece].edges[EDGE_BOTTOM].color = buffer[6];
		puzzle->pieces[piece].edges[EDGE_BOTTOM].type = 0;
		if (buffer[7] == '1')
		{
			puzzle->pieces[piece].edges[EDGE_BOTTOM].type = 1;
		}

		/* left */
		puzzle->pieces[piece].edges[EDGE_LEFT].color = buffer[9];
		puzzle->pieces[piece].edges[EDGE_LEFT].type = 0;
		if (buffer[10] == '1')
		{
			puzzle->pieces[piece].edges[EDGE_LEFT].type = 1;
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

	return;
}


int
puzzle_valid(struct Puzzle* puzzle)
{
	/* Top Left Corner Piece */
	if (edge_valid(&puzzle->pieces[PIECE_TOP_LEFT].edges[EDGE_RIGHT],  &puzzle->pieces[PIECE_TOP_CENTER].edges[EDGE_LEFT]) == 0 ||
	    edge_valid(&puzzle->pieces[PIECE_TOP_LEFT].edges[EDGE_BOTTOM], &puzzle->pieces[PIECE_MIDDLE_LEFT].edges[EDGE_TOP]) == 0)
	{
		return 0;
	}

	/* Top Right Corner Piece */
	if (edge_valid(&puzzle->pieces[PIECE_TOP_RIGHT].edges[EDGE_BOTTOM], &puzzle->pieces[PIECE_MIDDLE_RIGHT].edges[EDGE_TOP]) == 0 ||
	    edge_valid(&puzzle->pieces[PIECE_TOP_RIGHT].edges[EDGE_LEFT],   &puzzle->pieces[PIECE_TOP_CENTER].edges[EDGE_RIGHT]) == 0)
	{
		return 0;
	}

	/* Center */
	if (edge_valid(&puzzle->pieces[PIECE_MIDDLE_CENTER].edges[EDGE_TOP],    &puzzle->pieces[PIECE_TOP_CENTER].edges[EDGE_BOTTOM]) == 0 ||
	    edge_valid(&puzzle->pieces[PIECE_MIDDLE_CENTER].edges[EDGE_RIGHT],  &puzzle->pieces[PIECE_MIDDLE_RIGHT].edges[EDGE_LEFT]) == 0 ||
	    edge_valid(&puzzle->pieces[PIECE_MIDDLE_CENTER].edges[EDGE_BOTTOM], &puzzle->pieces[PIECE_BOTTOM_CENTER].edges[EDGE_TOP]) == 0 ||
	    edge_valid(&puzzle->pieces[PIECE_MIDDLE_CENTER].edges[EDGE_LEFT],   &puzzle->pieces[PIECE_MIDDLE_LEFT].edges[EDGE_RIGHT]) == 0)
	{
		return 0;
	}

	/* Bottom Left Corner Piece */
	if (edge_valid(&puzzle->pieces[PIECE_BOTTOM_LEFT].edges[EDGE_TOP],   &puzzle->pieces[PIECE_MIDDLE_LEFT].edges[EDGE_BOTTOM]) == 0 ||
	    edge_valid(&puzzle->pieces[PIECE_BOTTOM_LEFT].edges[EDGE_RIGHT], &puzzle->pieces[PIECE_BOTTOM_CENTER].edges[EDGE_LEFT]) == 0)
	{
		return 0;
	}

	/* Bottom Right Corner Piece */
	if (edge_valid(&puzzle->pieces[PIECE_BOTTOM_RIGHT].edges[EDGE_TOP],  &puzzle->pieces[PIECE_MIDDLE_RIGHT].edges[EDGE_BOTTOM]) == 0 ||
	    edge_valid(&puzzle->pieces[PIECE_BOTTOM_RIGHT].edges[EDGE_LEFT], &puzzle->pieces[PIECE_BOTTOM_CENTER].edges[EDGE_RIGHT]) == 0)
	{
		return 0;
	}

	return 1;
}


void
puzzle_print(struct Puzzle* puzzle) /* TODO add solution array pointer */
{
	short piece;

	printf("Input tiles:\n");

	/*  */
	for (piece = PIECE_TOP_LEFT; piece <= PIECE_BOTTOM_RIGHT; piece++)
	{
		printf(
			"%i. <%c%hu, %c%hu, %c%hu, %c%hu>\n",
			piece + 1,
			puzzle->pieces[piece].edges[EDGE_TOP].color,
			puzzle->pieces[piece].edges[EDGE_TOP].type,
			puzzle->pieces[piece].edges[EDGE_RIGHT].color,
			puzzle->pieces[piece].edges[EDGE_RIGHT].type,
			puzzle->pieces[piece].edges[EDGE_BOTTOM].color,
			puzzle->pieces[piece].edges[EDGE_BOTTOM].type,
			puzzle->pieces[piece].edges[EDGE_LEFT].color,
			puzzle->pieces[piece].edges[EDGE_LEFT].type
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
	for (piece = PIECE_TOP_LEFT; piece <= PIECE_BOTTOM_RIGHT; piece += 3)
	{
		printf("+--------+--------+--------+\n");

		/* print the identifier and top edge information */
		printf(
			"|%hu  %c%hu   |%hu  %c%hu   |%hu  %c%hu   |\n",
			puzzle->pieces[piece].identifier,
			puzzle->pieces[piece].edges[EDGE_TOP].color,
			puzzle->pieces[piece].edges[EDGE_TOP].type,

			puzzle->pieces[piece+1].identifier,
			puzzle->pieces[piece+1].edges[EDGE_TOP].color,
			puzzle->pieces[piece+1].edges[EDGE_TOP].type,

			puzzle->pieces[piece+2].identifier,
			puzzle->pieces[piece+2].edges[EDGE_TOP].color,
			puzzle->pieces[piece+2].edges[EDGE_TOP].type
		);

		/* print the left and right edge */
		printf(
			"|%c%hu    %c%hu|%c%hu    %c%hu|%c%hu    %c%hu|\n",
			puzzle->pieces[piece].edges[EDGE_LEFT].color,
			puzzle->pieces[piece].edges[EDGE_LEFT].type,
			puzzle->pieces[piece].edges[EDGE_RIGHT].color,
			puzzle->pieces[piece].edges[EDGE_RIGHT].type,

			puzzle->pieces[piece+1].edges[EDGE_LEFT].color,
			puzzle->pieces[piece+1].edges[EDGE_LEFT].type,
			puzzle->pieces[piece+1].edges[EDGE_RIGHT].color,
			puzzle->pieces[piece+1].edges[EDGE_RIGHT].type,

			puzzle->pieces[piece+2].edges[EDGE_LEFT].color,
			puzzle->pieces[piece+2].edges[EDGE_LEFT].type,
			puzzle->pieces[piece+2].edges[EDGE_RIGHT].color,
			puzzle->pieces[piece+2].edges[EDGE_RIGHT].type
		);

		/* print the bottom edge */
		printf(
			"|   %c%hu   |   %c%hu   |   %c%hu   |\n",
			puzzle->pieces[piece].edges[EDGE_BOTTOM].color,
			puzzle->pieces[piece].edges[EDGE_BOTTOM].type,

			puzzle->pieces[piece+1].edges[EDGE_BOTTOM].color,
			puzzle->pieces[piece+1].edges[EDGE_BOTTOM].type,

			puzzle->pieces[piece+2].edges[EDGE_BOTTOM].color,
			puzzle->pieces[piece+2].edges[EDGE_BOTTOM].type
		);
	}
	printf("+--------+--------+--------+\n");

	return;
}


#endif
