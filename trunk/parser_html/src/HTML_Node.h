/*
**	HTML Tree
**	HTML_Node.h
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

#ifndef HTML_Node_H
#define HTML_Node_H

// standard
#include <iostream>
#include <string>

// local
#ifndef	PJL_NO_NAMESPACES
namespace HTML_Tree {
#endif

class Content_Node;

//*****************************************************************************
//
// SYNOPSIS
//
	class HTML_Node
//
// DESCRIPTION
//
//	An HTML_Node is an abstract base class for all nodes in a tree.
//
//*****************************************************************************
{
public:
	class visitor {
		//
		// A visitor is an abstract base class for objects that "visit"
		// nodes in the tree during a traversal.
		//
	public:
		virtual ~visitor() { }
		virtual bool operator()(
			HTML_Node*, int depth, bool is_end_tag
		) = 0;
	};

	virtual ~HTML_Node() = 0;

	class iterator;
	iterator	begin();
	iterator	end();

	class const_iterator;
	const_iterator	begin() const;
	const_iterator	end() const;

	std::string	as_string( int pretty_print = -1 ) const;
	Content_Node*	parent() const { return parent_; }
	void		parent( Content_Node *new_parent );
	virtual void	visit( visitor&, int depth = 0 );
	std::ostream&	write( std::ostream&, int pretty_print = -1 ) const;
	virtual bool	write_node(
				std::ostream&, int spaces, bool is_end_tag
			) const = 0;

	class manip {
		//
		// manip is an ostream manipulator that is used to be able to
		// insert the result of a call to a member function taking
		// arguments of (ostream&,int).
		//
	public:
		typedef
		std::ostream& (HTML_Node::*function)(std::ostream&, int) const;

		manip( HTML_Node const &node, function f, int a ) :
			node_( node ), function_( f ), arg_( a ) { }

		friend
		std::ostream& operator<<( std::ostream &o, manip const &m ) {
			return (m.node_.*m.function_)( o, m.arg_ );
		}
	private:
		HTML_Node const &node_;
		function const function_;
		int const arg_;
	};

	manip write( int pretty_print = -1 ) const {
		//
		// This member function allows you to do this:
		//
		//	some_ostream << node->write();
		//
		// i.e., write the HTML text representation to an ostream using
		// "insertion style," but without specifying the ostream
		// directly.
		//
		return manip( *this, &HTML_Node::write, pretty_print );
	}

	friend bool operator==( HTML_Node const &i, HTML_Node const &j ) {
		return i.similar_to( j );
	}
	friend bool operator!=( HTML_Node const &i, HTML_Node const &j ) {
		return !( i == j );
	}
protected:
	HTML_Node( Content_Node* = 0 );
	virtual bool	similar_to( HTML_Node const& ) const;
private:
	Content_Node	*parent_;
	friend class	Content_Node;
};

#ifndef	PJL_NO_NAMESPACES
}
#endif

#endif	/* HTML_Node_H */
