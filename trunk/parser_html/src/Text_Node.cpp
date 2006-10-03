/*
**	HTML Tree
**	Text_Node.c
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

// local
#include "Text_Node.h"
#include "util.h"

#ifndef	PJL_NO_NAMESPACES
using namespace std;
namespace HTML_Tree {
#endif

static char const whitespace[] = " \f\n\r\t\v";

//*****************************************************************************
//
// SYNOPSIS
//
	/* virtual */ Text_Node::~Text_Node()
//
// DESCRIPTION
//
//	Destroy a Text_Node.  This is declared out-of-line because (a) it's
//	virtual and (b) the destructor is non-trivial because of implicit data
//	member destructors.
//
//*****************************************************************************
{
	// do nothing explicitly
}

//*****************************************************************************
//
// SYNOPSIS
//
	/* virtual */ bool Text_Node::similar_to( HTML_Node const &node ) const
//
// DESCRIPTION
//
//	Compare this node to another to see if they are "similar" meaning they
//	have the same text.
//
// PARAMETERS
//
//	node	A reference to another node to compare to.
//
// RETURN VALUE
//
//	Returns true only if the two nodes have the same text; otherwise,
//	false.
//
//*****************************************************************************
{
	if ( Text_Node const *const
		t = dynamic_cast< Text_Node const* >( &node )
	)
		return t->text == text;
	return false;
}

//*****************************************************************************
//
// SYNOPSIS
//
	/* virtual */ bool
	Text_Node::write_node( ostream &o, int spaces, bool is_end_tag ) const
//
// DESCRIPTION
//
//	Write the HTML text for this node to an ostream.
//
// PARAMETERS
//
//	o		The ostream to write to.
//
//	spaces		The number of spaces to emit before the node when
//			pretty-printing.  If -1, do no pretty-printing.
//
//	is_end_tag	True only when doing an end tag.
//
//*****************************************************************************
{
	if ( is_end_tag )
		return false;
	if ( spaces >= 0 ) {
		//
		// Since we are emitting our own newlines in order to
		// pretty-print, we have to strip leading and trailing
		// whitespace from the string.
		//
		string s = text;
		string::size_type i;

		i = s.find_first_not_of( whitespace );
		if ( i == string::npos )
			return false;
		if ( i > 0 )
			s.erase( 0, i );

		i = s.find_last_not_of( whitespace );
		if ( i != string::npos && i < s.size() - 1 )
			s.erase( i + 1 );

		o << space( spaces ) << s;
	} else
		o << text;
	return true;
}

#ifndef	PJL_NO_NAMESPACES
}
#endif
