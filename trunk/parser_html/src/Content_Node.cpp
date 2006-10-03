/*
**	HTML Tree
**	Content_Node.c
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
#include "Content_Node.h"
#include "util.h"

#ifndef	PJL_NO_NAMESPACES
using namespace std;
namespace HTML_Tree {
#endif

//*****************************************************************************
//
// SYNOPSIS
//
	Content_Node::Content_Node()
//
// DESCRIPTION
//
//	Construct (initialize) a Content_Node.  This is used as a root node of
//	an HTML_Tree.  Such a node has no name.
//
//*****************************************************************************
	: destructing_( false )
{
	// do nothing else
}

//*****************************************************************************
//
// SYNOPSIS
//
	Content_Node::Content_Node(
		char const *name, element const &e, Content_Node *parent
	)
//
// DESCRIPTION
//
//	Construct (initialize) a Content_Node.
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
	: Element_Node( name, e, parent ), destructing_( false )
{
	// do nothing else
}

//*****************************************************************************
//
// SYNOPSIS
//
	Content_Node::Content_Node(
		char const *name, element const &e,
		char const *att_begin, char const *att_end,
		Content_Node *parent
	)
//
// DESCRIPTION
//
//	Construct (initialize) a Content_Node.
//
// PARAMETERS
//
//	name		The name of this node.
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
	: Element_Node( name, e, att_begin, att_end, parent ),
	  destructing_( false )
{
	// do nothing else
}

//*****************************************************************************
//
// SYNOPSIS
//
	/* virtual */ Content_Node::~Content_Node()
//
// DESCRIPTION
//
//	Destroy a Content_Node and destroy all of its child nodes recursively.
//	Set our destructing_ flag so our children don't bother erasing
//	themselves from our list of children since we're destroying ourselves
//	anyway.
//
//*****************************************************************************
{
	destructing_ = true;
	for ( iterator child = begin(); child != end(); ++child )
		delete *child;
}

//*****************************************************************************
//
// SYNOPSIS
//
	HTML_Node* Content_Node::erase( HTML_Node *orphan )
//
// DESCRIPTION
//
//	Erase a given child node from our list of children.  Note that the node
//	itself is not deleted.
//
// PARAMETERS
//
//	orphan	A pointer to the child node to be erased.
//
// RETURN VALUE
//
//	Returns a pointer to the erased node if it was actually erased; null
//	otherwise.
//
//*****************************************************************************
{
	if ( orphan )
		for ( iterator child = begin(); child != end(); ++child )
			if ( *child == orphan )
				return erase( child );
	return 0;
}

//*****************************************************************************
//
// SYNOPSIS
//
	HTML_Node* Content_Node::insert(
		iterator const &i, HTML_Node *new_child
	)
//
// DESCRIPTION
//
//	Insert a given child node to our list of children taking care to erase
//	it from its current parent's list of children, if any.
//
// PARAMETERS
//
//	i		The iterator marking the position of the child to
//			insert before.
//
//	new_child	A pointer to the child node to be inserted.
//
// RETURN VALUE
//
//	Returns new_child if it was actually inserted; null otherwise.
//
//*****************************************************************************
{
	if ( new_child && new_child->parent() != this ) {
		if ( new_child->parent() )
			new_child->parent()->erase( new_child );
		new_child->parent_ = this;
		children_.insert( i, new_child );
		return new_child;
	}
	return 0;
}

//*****************************************************************************
//
// SYNOPSIS
//
	HTML_Node* Content_Node::push_back( HTML_Node *new_child )
//
// DESCRIPTION
//
//	Push a given child node onto the back of out list of children taking
//	care to erase it from its current parent's list of children, if any.
//
// PARAMETERS
//
//	new_child	A pointer to the child node to be pushed.
//
// RETURN VALUE
//
//	Returns new_child if it was actually pushed; null otherwise.
//
//*****************************************************************************
{
	if ( new_child && new_child->parent() != this ) {
		if ( new_child->parent() )
			new_child->parent()->erase( new_child );
		new_child->parent_ = this;
		children_.push_back( new_child );
		return new_child;
	}
	return 0;
}

//*****************************************************************************
//
// SYNOPSIS
//
	HTML_Node* Content_Node::push_front( HTML_Node *new_child )
//
// DESCRIPTION
//
//	Push a given child node onto the front of our list of children taking
//	care to erase it from its current parent's list of children, if any.
//
// PARAMETERS
//
//	new_child	A pointer to the child node to be pushed.
//
// RETURN VALUE
//
//	Returns new_child if it was actually pushed; null otherwise.
//
//*****************************************************************************
{
	if ( new_child && new_child->parent() != this ) {
		if ( new_child->parent() )
			new_child->parent()->erase( new_child );
		new_child->parent_ = this;
		children_.push_front( new_child );
		return new_child;
	}
	return 0;
}

//*****************************************************************************
//
// SYNOPSIS
//
	/* virtual */ void Content_Node::visit( visitor &v, int depth )
//
// DESCRIPTION
//
//	Have the visitor visit us, all of our children, and us again.  If the
//	visitor returns false the first time, don't visit our children and
//	don't have the visitor visit us a second time; if the visitor returns
//	true the second time, repeat the entire visit cycle.
//	
//	An exception is made if this node is the root node: in this case, it
//	is not visited itself.
//	
// PARAMETERS
//
//	v	A reference to a visitor.
//
//*****************************************************************************
{
	bool const not_root_node = name && *name;
	do {
		if ( not_root_node && !v( this, depth, false ) )
			break;
		for ( iterator child = begin(); child != end(); ++child )
			(*child)->visit( v, depth + not_root_node );
	} while ( not_root_node && v( this, depth, true ) );
}

//*****************************************************************************
//
// SYNOPSIS
//
	/* virtual */ bool
	Content_Node::write_node( ostream &o, int spaces, bool is_end_tag )
		const
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
	if ( !Element_Node::write_node( o, spaces, is_end_tag ) )
		return false;
	if ( is_end_tag )
		o << space( spaces ) << "</" << name << '>';
	return true;
}

#ifndef	PJL_NO_NAMESPACES
}
#endif
