#ifndef _piece_c_
#define _piece_c_


#include "headers.h"


void
piece_rotate(struct Piece* piece, int shift)
{
	short idx;
	short swp;

	if (shift == 0 || (shift %= 4) == 0)
	{
		return;
	}

	for (idx = EDGE_TOP; idx <= EDGE_LEFT; idx++)
	{
		swp = (idx + shift) % 4;

		piece->edges[idx].color ^= piece->edges[swp].color;
		piece->edges[swp].color ^= piece->edges[idx].color;
		piece->edges[idx].color ^= piece->edges[swp].color;

		piece->edges[idx].type ^= piece->edges[swp].type;
		piece->edges[swp].type ^= piece->edges[idx].type;
		piece->edges[idx].type ^= piece->edges[swp].type;
	}

	return;
}


#endif
