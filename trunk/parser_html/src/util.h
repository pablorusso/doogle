/*
**	HTML Tree
**	util.h
**
**	Copyright (C) 1999  Paul J. Lucas
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

#ifndef	util_H
#define	util_H

// standard
#include <cctype>
#include <iostream>

//*****************************************************************************
//
// SYNOPSIS
//
	template< class Arg > class my_omanip
//
// DESCRIPTION
//
//	A my_omanip is a class for assisting in the creating of ostream
//	manipulators by storing a pointer to a function and its argument to be
//	called later via operator<<().
//
// SEE ALSO
//
//	Angelika Langer and Klaus Kreft.  "Standard C++ IOStreams and Locales."
//	Addison-Wesley, Reading, MA., pp. 179-191.
//
//*****************************************************************************
{
public:
	typedef void (*func_type)( std::ostream&, Arg );
	my_omanip( func_type f, Arg const &arg ) : f_( f ), arg_( arg ) { }

	friend
	std::ostream& operator<<( std::ostream &o, my_omanip<Arg> const &om ) {
		(*om.f_)( o, om.arg_ );
		return o;
	}
private:
	func_type const f_;
	Arg const arg_;
};

///////////////////////////////////////////////////////////////////////////////

inline void		space( std::ostream &o, int n ) {
				while ( n-- > 0 )
					o << ' ';
			}
inline my_omanip<int>	space( int n ) { return my_omanip<int>( space, n ); }

			// ensure function semantics: 'c' is expanded once
inline bool		is_space( char c )	{ return isspace( c ); }
inline char		to_lower( char c )	{ return tolower( c ); }
extern char*		to_lower( char const *begin, char const *end );

#endif	/* util_H */
