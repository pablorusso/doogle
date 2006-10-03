/*
**	HTML Tree
**	Element_Node.c
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

// standard
#include <cstring>

// local
#include "Element_Node.h"
#include "html.h"
#include "util.h"

#ifndef	PJL_NO_NAMESPACES
using namespace std;
namespace HTML_Tree {
#endif

void	parse_attributes(
		char const *begin, char const *end, Element_Node::attribute_map&
	);

//*****************************************************************************
//
// SYNOPSIS
//
	Element_Node::Element_Node()
//
// DESCRIPTION
//
//	Construct (initialize) an Element_Node.  This is used as a root node of
//	an HTML_Tree.  Such a node has no name or element.
//
//*****************************************************************************
	: name( "" ), element_( 0 )
{
	// do nothing else
}

//*****************************************************************************
//
// SYNOPSIS
//
	Element_Node::Element_Node(
		char const *s, element const &e, Content_Node *parent
	)
//
// DESCRIPTION
//
//	Construct (initialize) an Element_Node.
//
// PARAMETERS
//
//	name	The name of this node.
//
//	e	The element information for this node.
//
//	parent	The parent node of this node, if any.
//
//*****************************************************************************
	: HTML_Node( parent ), name( s ), element_( &e )
{
	// do nothing else
}

//*****************************************************************************
//
// SYNOPSIS
//
	Element_Node::Element_Node(
		char const *s, element const &e,
		char const *att_begin, char const *att_end,
		Content_Node *parent
	)
//
// DESCRIPTION
//
//	Construct (initialize) an Element_Node.
//
// PARAMETERS
//
//	s		The name of this node.
//
//	e		The element information for this node.
//
//	att_begin	An iterator pointing to the first character of the text
//			to be parsed for attributes.
//
//	att_end		An iterator pointing to one past the last character of
//			the text to be parsed for attributes.
//
//	parent		The parent node of this node, if any.
//
//*****************************************************************************
	: HTML_Node( parent ), name( s ), element_( &e )
{
	parse_attributes( att_begin, att_end, attributes );
}

//*****************************************************************************
//
// SYNOPSIS
//
	/* virtual */ Element_Node::~Element_Node()
//
// DESCRIPTION
//
//	Destroy an Element_Node.  This is declared out-of-line because (a) it's
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
	/* virtual */
	bool Element_Node::similar_to( HTML_Node const &node ) const
//
// DESCRIPTION
//
//	Compare this node to another to see if they are "similar" meaning they
//	have the same name.
//
// PARAMETERS
//
//	node	A reference to another node to compare to.
//
// RETURN VALUE
//
//	Returns true only if the two nodes have the same name; otherwise,
//	false.
//
//*****************************************************************************
{
	if ( Element_Node const *const
		e = dynamic_cast< Element_Node const* >( &node )
	)
		return !::strcmp( e->name, name );
	return false;
}

//*****************************************************************************
//
// SYNOPSIS
//
	/* virtual */ bool
	Element_Node::write_node( ostream &o, int spaces, bool is_end_tag )
		const
//
// DESCRIPTION
//
//	Write the XML text for this node to an ostream.
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
	if ( !is_end_tag ) {
		o << space( spaces ) << '<' << name;
		for ( attribute_map::const_iterator
			att = attributes.begin(); att != attributes.end(); ++att
		)
			o << ' ' << att->first << "=\"" << att->second << '"';
		o << '>';
	}
	return true;
}

#ifndef	PJL_NO_NAMESPACES
}
#endif
