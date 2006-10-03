/*
**	HTML Tree
**	Text_Node.h
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

#ifndef Text_Node_H
#define Text_Node_H

// standard
#include <string>

// local
#include "HTML_Node.h"

#ifndef	PJL_NO_NAMESPACES
namespace HTML_Tree {
#endif

//*****************************************************************************
//
// SYNOPSIS
//
	class Text_Node : public HTML_Node
//
// DESCRIPTION
//
//	A Text_Node is-an HTML_Node that simply contains plain text.
//
//*****************************************************************************
{
public:
	Text_Node( char const *new_text, Content_Node *parent = 0 );
	Text_Node(
		char const *begin, char const *end, Content_Node *parent = 0
	);
	virtual ~Text_Node();

	std::string	text;
protected:
	virtual bool	similar_to( HTML_Node const& ) const;
	virtual bool	write_node( std::ostream&, int, bool ) const;
};

////////// Inlines ////////////////////////////////////////////////////////////

inline Text_Node::Text_Node( char const *new_text, Content_Node *parent ) :
	HTML_Node( parent ), text( new_text )
{
	// do nothing else
}

inline Text_Node::Text_Node(
	char const *begin, char const *end, Content_Node *parent
) :
	HTML_Node( parent ), text( begin, end )
{
	// do nothing else
}

#ifndef	PJL_NO_NAMESPACES
}
#endif

#endif	/* Text_Node_H */
