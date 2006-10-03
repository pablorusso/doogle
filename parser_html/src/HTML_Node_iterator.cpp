/*
**	HTML Tree
**	HTML_Node_iterator.c
**
**	Copyright (C) 2002  Paul J. Lucas
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
#include "HTML_Node_iterator.h"

#ifndef	PJL_NO_NAMESPACES
using namespace std;
namespace HTML_Tree {
#endif

//*****************************************************************************
//
// SYNOPSIS
//
	void HTML_Node::iterator::push_if_content()
//
// DESCRIPTION
//
//	If the current node is-a Content_Node and it has child nodes, update
//	parent_ and push an iterator onto the stack.
//
//*****************************************************************************
{
	if ( Content_Node *const c = dynamic_cast< Content_Node* >( node_ ) )
		if ( !c->empty() )
			stack_.push( (parent_ = c)->begin() );
}

//*****************************************************************************
//
// SYNOPSIS
//
	HTML_Node::iterator& HTML_Node::iterator::operator++()
//
// DESCRIPTION
//
//	Increment an HTML_Node::iterator.
//
// RETURN VALUE
//
//	Return a reference to *this.
//
//*****************************************************************************
{
	while ( !stack_.empty() ) {
		if ( stack_.top() == parent_->end() ) {
			stack_.pop();
			parent_ = parent_->parent();
			continue;
		}
		node_ = *stack_.top()++;
		push_if_content();
		return *this;
	}
	node_ = 0;
	return *this;
}

//*****************************************************************************
//
// SYNOPSIS
//
	void HTML_Node::const_iterator::push_if_content()
//
// DESCRIPTION
//
//	If the current node is-a Content_Node and it has child nodes, update
//	parent_ and push an iterator onto the stack.
//
//*****************************************************************************
{
	if ( Content_Node const *const
		c = dynamic_cast< Content_Node const* >( node_ )
	)
		if ( !c->empty() )
			stack_.push( (parent_ = c)->begin() );
}

//*****************************************************************************
//
// SYNOPSIS
//
	HTML_Node::const_iterator& HTML_Node::const_iterator::operator++()
//
// DESCRIPTION
//
//	Increment an HTML_Node::const_iterator.
//
// RETURN VALUE
//
//	Return a reference to *this.
//
//*****************************************************************************
{
	while ( !stack_.empty() ) {
		if ( stack_.top() == parent_->end() ) {
			stack_.pop();
			parent_ = parent_->parent();
			continue;
		}
		node_ = *stack_.top()++;
		push_if_content();
		return *this;
	}
	node_ = 0;
	return *this;
}

#ifndef	PJL_NO_NAMESPACES
}
#endif
