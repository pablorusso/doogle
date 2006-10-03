/*
**	HTML Tree
**	Element_Node.h
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

#ifndef Element_Node_H
#define Element_Node_H

// standard
#include <map>
#include <string>

// local
#include "elements.h"
#include "HTML_Node.h"

#ifndef	PJL_NO_NAMESPACES
namespace HTML_Tree {
#endif

class Content_Node;

//*****************************************************************************
//
// SYNOPSIS
//
	class Element_Node : public HTML_Node
//
// DESCRIPTION
//
//	An Element_Node is-an HTML_Node for HTML elements that possibly have
//	attributes.
//
//*****************************************************************************
{
public:
	typedef std::map< std::string, std::string > attribute_map;

	Element_Node(
		char const *name, element const&, Content_Node *parent = 0
	);
	Element_Node(
		char const *name, element const&,
		char const *att_begin, char const *att_end,
		Content_Node *parent = 0
	);
	virtual ~Element_Node();

	attribute_map	attributes;
	char const	*const name;
	//
	// It's OK for name to be just a pointer rather than a string since it
	// points to the key in the element_map.
protected:
	Element_Node();

	virtual bool	similar_to( HTML_Node const& ) const;
	virtual bool	write_node( std::ostream&, int, bool ) const;
private:
	element const	*const element_;	// null for root node

	friend void	parse_html_tag(
				char const*&, char const*, Content_Node*&
			);
};

#ifndef	PJL_NO_NAMESPACES
}
#endif

#endif	/* Element_Node_H */
