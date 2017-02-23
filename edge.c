#ifndef _edge_c_
#define _edge_c_


#include "headers.h"


int
edge_valid(struct Edge* a, struct Edge* b)
{
	if ((a->color != b->color) || (a->type == b->type))
	{
		return 0;
	}
	return 1;
}


#endif
