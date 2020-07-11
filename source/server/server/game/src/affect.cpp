#include "stdafx.h"
#include "affect.h"

#include <boost/pool/object_pool.hpp>

boost::object_pool<CAffect> affect_pool;

CAffect* CAffect::Acquire()
{
	return affect_pool.malloc();
}

void CAffect::Release(CAffect* p)
{
	affect_pool.free(p);
}