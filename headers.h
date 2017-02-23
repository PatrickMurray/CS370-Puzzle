#ifndef _header_h_
#define _header_h_


#include <stdio.h>
#include <stdlib.h>

#ifndef __NetBSD__
#include <bsd/stdlib.h>
#endif

#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

/* function benchmarking */
#include <time.h>


#define TOP    0
#define RIGHT  1
#define BOTTOM 2
#define LEFT   3


// depth first search - halt at no solution, continue on next lexicographical
//     permutation (generate each permutation of 1-9 - it's not as big as you'd
//     think)
// pruning


/*
 * type = 0, solid block - does not point to the current piece
 * type = 1, arrow - points to the current piece
 *
 */
struct Edge {
	char  color;
	short type;
};


/*
 * identifier = 1
 * edges = {top, right, bottom, left}
 */
struct Piece {
	short       identifier;
	struct Edge edges[4];
};


/*
 *
 *
 *
 *
 */
struct Puzzle {
	struct Piece pieces[9];
};


/* main.c */
int main(int, char**);


/* puzzle.c */
struct Puzzle* puzzle_parse(char*);
void           puzzle_print(struct Puzzle*);
void           puzzle_print_matrix(struct Puzzle*);


/* piece.c */
void piece_rotate(struct Piece*, int);


/* edge.c */
int edge_valid(struct Edge*, struct Edge*);


#endif
