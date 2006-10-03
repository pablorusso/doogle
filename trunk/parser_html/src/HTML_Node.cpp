/*
**	HTML Tree
**	HTML_Node.c
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
#ifndef	PJL_NO_SSTREAM
#include <sstream>
#else
#include <strstream.h>
#define	ostringstream ostrstream
#endif

// local
#include "Content_Node.h"
#include "Element_Node.h"
#include "HTML_Node.h"
#include "Text_Node.h"
#include "util.h"

#ifndef	PJL_NO_NAMESPACES
using namespace std;
namespace HTML_Tree {
#endif

//*****************************************************************************
//
// SYNOPSIS
//
	class write_visitor : public HTML_Node::visitor
//
// DESCRIPTION
//
//	A write_visitor is-a visitor for writing out the text representing an
//	HTML node to an ostream.
//
//*****************************************************************************
{
public:
	write_visitor( ostream &o, int pretty_print = -1 ) :
		out_( o ), pretty_print_( pretty_print ), pre_count_( 0 ) { }

	virtual bool	operator()( HTML_Node*, int, bool );
private:
	ostream&	out_;
	int const	pretty_print_;
	int		pre_count_;
};

//*****************************************************************************
//
// SYNOPSIS
//
	/* virtual */ bool write_visitor::operator()(
		HTML_Node *node, int depth, bool is_end_tag
	)
//
// DESCRIPTION
//
//	Write the HTML text for a given node to an ostream.
//
// PARAMETERS
//
//	node		The HTML_Node to be visited.
//
//	depth		How many nodes "deep" in the tree we are.
//
//	is_end_tag	True when called at the end of a visit.
//
// RETURN VALUE
//
//	Returns !is_end_tag.
//	
//*****************************************************************************
{
	//
	// We have to see if the node is a <PRE> element to suspend pretty-
	// printing inside it.
	//
	Element_Node *const e = dynamic_cast<Element_Node*>( node );
	bool const is_pre = e && !::strcmp( e->name, "pre" );
	if ( !is_end_tag && is_pre )
		++pre_count_;

	int const spaces = pretty_print_ >= 0 && !pre_count_ ?
		(pretty_print_ + depth) * 2 : -1;
	if ( node->write_node( out_, spaces, is_end_tag ) && spaces >= 0 )
		out_ << '\n';

	if ( is_end_tag && is_pre )
		--pre_count_;

	return !is_end_tag;
}

//*****************************************************************************
//
// SYNOPSIS
//
	class as_string_visitor : public write_visitor
//
// DESCRIPTION
//
//	A as_string_visitor is-a visitor for concatenating the text
//	representing an HTML tree into a string.
//
//*****************************************************************************
{
public:
	as_string_visitor( int pretty_print = -1 ) :
		write_visitor( out_, pretty_print ) { }

#ifndef	PJL_NO_SSTREAM
	string as_string() const { return out_.str(); }
#else
	string as_string() const {
		as_string_visitor *const p =
			const_cast<as_string_visitor*>( this );
		p->out_ << ends;
		string const temp = p->out_.str();
		p->out_.freeze( false );
		return temp;
	}
#endif	/* PJL_NO_SSTREAM */
private:
	ostringstream	out_;
};

//*****************************************************************************
//
// SYNOPSIS
//
	HTML_Node::HTML_Node( Content_Node *parent )
//
// DESCRIPTION
//
//	Construct (initialize) an HTML_Node by adding ourselves to our parent's
//	list of children.
//
// PARAMETERS
//
//	parent	A pointer to the node to be the parent of this node, if any.
//
//*****************************************************************************
	: parent_( parent )
{
	if ( parent )
		parent->children_.push_back( this );
}

//*****************************************************************************
//
// SYNOPSIS
//
	/* virtual */ HTML_Node::~HTML_Node()
//
// DESCRIPTION
//
//	Destroy an HTML_Node.  If we have a parent node and it isn't already in
//	the process of destroying itself (its destructor isn't executing),
//	remove ourselves from our parent's list of children.
//
//*****************************************************************************
{
	if ( parent_ && !parent_->destructing_ )
		parent_->erase( this );
}

//*****************************************************************************
//
// SYNOPSIS
//
	string HTML_Node::as_string( int pretty_print ) const
//
// DESCRIPTION
//
//	Write the HTML text representing an HTML node to a given ostream,
//	optionally pretty-printing it.
//
// PARAMETERS
//
//	pretty_print	The initial indent to pretty-print.  If < 0, no pretty-
//			printing is done.
//
// RETURN VALUE
//
//	Returns the string.
//
//*****************************************************************************
{
	as_string_visitor v( pretty_print );
	const_cast< HTML_Node* >( this )->visit( v );
	return v.as_string();
}

//*****************************************************************************
//
// SYNOPSIS
//
	void HTML_Node::parent( Content_Node *new_parent )
//
// DESCRIPTION
//
//	Set the current node's parent to a new parent removing it from the old
//	parent, if any.
//
// PARAMETERS
//
//	new_parent	A pointer to a new parent.
//	
//*****************************************************************************
{
	if ( new_parent != parent_ ) {
		if ( parent_ )
			parent_->erase( this );
		if ( new_parent )
			new_parent->push_back( this );
	}
}

//*****************************************************************************
//
// SYNOPSIS
//
	/* virtual */ bool HTML_Node::similar_to( HTML_Node const &node ) const
//
// DESCRIPTION
//
//	Compare this node to another to see if they are "similar" meaning they
//	have the same address.  This rather strict "similar" test (must be the
//	exact same HTML_Node) is merely the default and is expected to be
//	overridden.
//
// PARAMETERS
//
//	node	A reference to another node to compare to.
//
// RETURN VALUE
//
//	Returns true only if the two nodes have the same address; otherwise,
//	false.
//
//*****************************************************************************
{
	return &node == this;
}

//*****************************************************************************
//
// SYNOPSIS
//
	/* virtual */ void HTML_Node::visit( visitor &v, int depth )
//
// DESCRIPTION
//
//	Call the visitor function passing ourselves as the node to visit.
//
// PARAMETERS
//
//	v	A reference to a visitor.
//
//	depth	The depth (nesting level) of this node.
//	
//*****************************************************************************
{
	v( this, depth, false );
}

//*****************************************************************************
//
// SYNOPSIS
//
	ostream& HTML_Node::write( ostream& o, int pretty_print ) const
//
// DESCRIPTION
//
//	Write the HTML text representing an HTML node to a given ostream,
//	optionally pretty-printing it.
//
// PARAMETERS
//
//	o		The ostream to write to.
//
//	pretty_print	The initial indent to pretty-print.  If < 0, no pretty-
//			printing is done.
//
// RETURN VALUE
//
//	Returns the given ostream.
//
//*****************************************************************************
{
	write_visitor v( o, pretty_print );
	const_cast< HTML_Node* >( this )->visit( v );
	return o;
}

#ifndef	PJL_NO_NAMESPACES
}
#endif
