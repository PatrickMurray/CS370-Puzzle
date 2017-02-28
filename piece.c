#ifndef _piece_c_
#define _piece_c_


#include "headers.h"


void
piece_rotate(struct Piece* piece, int shift)
{
	if (shift == 0)
	{
		return;
	}

	if ((shift %= 4) == 0)
	{
		return;
	}

	piece->offset += shift;
	piece->offset %= 4;

	return;
}


#endif
