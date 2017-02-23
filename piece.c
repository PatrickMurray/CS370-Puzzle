#ifndef _piece_c_
#define _piece_c_


#include "headers.h"


void
piece_rotate(struct Piece* piece, int shift)
{
	short       idx;
	short       swp;
	struct Edge temp;
	if (shift == 0 || (shift %= 4) == 0)
	{
		return;
	}

	for (idx = 0; idx < 4; idx++)
	{
		swp = (idx + shift) % 4;

		temp.color = piece->edges[idx].color;
		temp.type  = piece->edges[idx].type;

		piece->edges[idx].color = piece->edges[swp].color;
		piece->edges[idx].type  = piece->edges[swp].type;

		piece->edges[swp].color = temp.color;
		piece->edges[swp].type  = temp.type;
	}

	return;
}


#endif
