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
		puzzle->pieces[piece].offset      = 0;
		
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
	struct Piece piece_top_left;
	struct Piece piece_top_center;
	struct Piece piece_top_right;
	struct Piece piece_middle_left;
	struct Piece piece_middle_center;
	struct Piece piece_middle_right;
	struct Piece piece_bottom_left;
	struct Piece piece_bottom_center;
	struct Piece piece_bottom_right;

	piece_top_left      = puzzle->pieces[PIECE_TOP_LEFT];
	piece_top_center    = puzzle->pieces[PIECE_TOP_CENTER];
	piece_top_right     = puzzle->pieces[PIECE_TOP_RIGHT];
	piece_middle_left   = puzzle->pieces[PIECE_MIDDLE_LEFT];
	piece_middle_center = puzzle->pieces[PIECE_MIDDLE_CENTER];
	piece_middle_right  = puzzle->pieces[PIECE_MIDDLE_RIGHT];
	piece_bottom_left   = puzzle->pieces[PIECE_BOTTOM_LEFT];
	piece_bottom_center = puzzle->pieces[PIECE_BOTTOM_CENTER];
	piece_bottom_right  = puzzle->pieces[PIECE_BOTTOM_RIGHT];

	/* Top Left Corner Piece */
	if (edge_valid(&piece_top_left.edges[(EDGE_RIGHT  + piece_top_left.offset) % 4], &piece_top_center.edges[(EDGE_LEFT + piece_top_center.offset) % 4])  == 0 ||
	    edge_valid(&piece_top_left.edges[(EDGE_BOTTOM + piece_top_left.offset) % 4], &piece_middle_left.edges[(EDGE_TOP + piece_middle_left.offset) % 4]) == 0)
	{
		return 0;
	}

	/* Top Right Corner Piece */
	if (edge_valid(&piece_top_right.edges[(EDGE_BOTTOM + piece_top_right.offset) % 4], &piece_middle_right.edges[(EDGE_TOP + piece_middle_right.offset) % 4]) == 0 ||
	    edge_valid(&piece_top_right.edges[(EDGE_LEFT   + piece_top_right.offset) % 4], &piece_top_center.edges[(EDGE_RIGHT + piece_top_center.offset) % 4])   == 0)
	{
		return 0;
	}

	/* Center */
	if (edge_valid(&piece_middle_center.edges[(EDGE_TOP    + piece_middle_center.offset)], &piece_top_center.edges[(EDGE_BOTTOM + piece_top_center.offset) % 4])    == 0 ||
	    edge_valid(&piece_middle_center.edges[(EDGE_RIGHT  + piece_middle_center.offset)], &piece_middle_right.edges[(EDGE_LEFT + piece_middle_right.offset) % 4])  == 0 ||
	    edge_valid(&piece_middle_center.edges[(EDGE_BOTTOM + piece_middle_center.offset)], &piece_bottom_center.edges[(EDGE_TOP + piece_bottom_center.offset) % 4]) == 0 ||
	    edge_valid(&piece_middle_center.edges[(EDGE_LEFT   + piece_middle_center.offset)], &piece_middle_left.edges[(EDGE_RIGHT + piece_middle_left.offset) % 4])   == 0)
	{
		return 0;
	}

	/* Bottom Left Corner Piece */
	if (edge_valid(&piece_bottom_left.edges[(EDGE_TOP   + piece_bottom_left.offset) % 4], &piece_middle_left.edges[(EDGE_BOTTOM + piece_middle_left.offset) % 4])   == 0 ||
	    edge_valid(&piece_bottom_left.edges[(EDGE_RIGHT + piece_bottom_left.offset) % 4], &piece_bottom_center.edges[(EDGE_LEFT + piece_bottom_center.offset) % 4]) == 0)
	{
		return 0;
	}

	/* Bottom Right Corner Piece */
	if (edge_valid(&piece_bottom_right.edges[(EDGE_TOP  + piece_bottom_right.offset) % 4], &piece_middle_right.edges[(EDGE_BOTTOM + piece_middle_right.offset) % 4])  == 0 ||
	    edge_valid(&piece_bottom_right.edges[(EDGE_LEFT + piece_bottom_right.offset) % 4], &piece_middle_center.edges[(EDGE_RIGHT + piece_middle_center.offset) % 4]) == 0)
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
	short        row_idx;
	struct Piece piece_left;
	struct Piece piece_center;
	struct Piece piece_right;

	/* print the solutions - TODO for-loop here*/
	for (row_idx = PIECE_TOP_LEFT; row_idx <= PIECE_BOTTOM_RIGHT; row_idx += 3)
	{
		piece_left   = puzzle->pieces[row_idx];
		piece_center = puzzle->pieces[row_idx + 1];
		piece_right  = puzzle->pieces[row_idx + 2];

		printf("+--------+--------+--------+\n");

		/* print the identifier and top edge information */
		printf(
			"|%hu  %c%hu   |%hu  %c%hu   |%hu  %c%hu   |\n",
			piece_left.identifier,
			piece_left.edges[(EDGE_TOP + piece_left.offset) % 4].color,
			piece_left.edges[(EDGE_TOP + piece_left.offset) % 4].type,

			piece_center.identifier,
			piece_center.edges[(EDGE_TOP + piece_center.offset) % 4].color,
			piece_center.edges[(EDGE_TOP + piece_center.offset) % 4].type,

			piece_right.identifier,
			piece_right.edges[(EDGE_TOP + piece_right.offset) % 4].color,
			piece_right.edges[(EDGE_TOP + piece_right.offset) % 4].type
		);

		/* print the left and right edge */
		printf(
			"|%c%hu    %c%hu|%c%hu    %c%hu|%c%hu    %c%hu|\n",
			piece_left.edges[(EDGE_LEFT  + piece_left.offset) % 4].color,
			piece_left.edges[(EDGE_LEFT  + piece_left.offset) % 4].type,
			piece_left.edges[(EDGE_RIGHT + piece_left.offset) % 4].color,
			piece_left.edges[(EDGE_RIGHT + piece_left.offset) % 4].type,

			piece_center.edges[(EDGE_LEFT  + piece_center.offset) % 4].color,
			piece_center.edges[(EDGE_LEFT  + piece_center.offset) % 4].type,
			piece_center.edges[(EDGE_RIGHT + piece_center.offset) % 4].color,
			piece_center.edges[(EDGE_RIGHT + piece_center.offset) % 4].type,

			piece_right.edges[(EDGE_LEFT  + piece_right.offset) % 4].color,
			piece_right.edges[(EDGE_LEFT  + piece_right.offset) % 4].type,
			piece_right.edges[(EDGE_RIGHT + piece_right.offset) % 4].color,
			piece_right.edges[(EDGE_RIGHT + piece_right.offset) % 4].type
		);

		/* print the bottom edge */
		printf(
			"|   %c%hu   |   %c%hu   |   %c%hu   |\n",
			piece_left.edges[(EDGE_BOTTOM + piece_left.offset) % 4].color,
			piece_left.edges[(EDGE_BOTTOM + piece_left.offset) % 4].type,

			piece_center.edges[(EDGE_BOTTOM + piece_center.offset) % 4].color,
			piece_center.edges[(EDGE_BOTTOM + piece_center.offset) % 4].type,

			piece_right.edges[(EDGE_BOTTOM + piece_right.offset) % 4].color,
			piece_right.edges[(EDGE_BOTTOM + piece_right.offset) % 4].type
		);
	}
	printf("+--------+--------+--------+\n");

	return;
}


#endif
