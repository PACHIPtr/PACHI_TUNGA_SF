#include "stdafx.h"
#include "vid.h"
#include <boost/functional/hash.hpp>

std::size_t hash_value(VID const& v) {
	boost::hash<DWORD> hasher;
	return hasher(v.getID());
}