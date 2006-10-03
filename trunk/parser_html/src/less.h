/*
**	PJL C++ Library
**	less.h
**
**	Copyright (C) 1998  Paul J. Lucas
**
**	This program is free software; you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation; either version 2 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program; if not, write to the Free Software
**	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef	less_H
#define	less_H

// standard
#include <cstring>
#include <functional>

// local

#ifndef	PJL_NO_NAMESPACES
namespace std {
#endif

//*****************************************************************************
//
// SYNOPSIS
//
	template<> struct less< char const* > :
		binary_function< char const*, char const*, bool >
//
// DESCRIPTION
//
//	Specialize the binary_function "less" so that C-style strings (char
//	const*) will work properly with STL containers.
//
// SEE ALSO
//
//	binary_function(3), less(3), strcmp(3)
//
//	Bjarne Stroustrup.  "The C++ Programming Language, 3rd ed."
//	Addison-Wesley, Reading, MA, 1997.  p. 468.
//
//*****************************************************************************
{
	less() { }
	// This default constructor doesn't need to be defined, but g++ 2.8.0
	// complains if it isn't and you try to define a "const less" object.

	result_type
	operator()( first_argument_type a, second_argument_type b ) const {
		return std::strcmp( a, b ) < 0;
	}
};

#ifndef	PJL_NO_NAMESPACES
}
#endif

#endif	/* less_H */
