/*
**	HTML Tree
**	HTML_Node_iterator.h
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

#ifndef HTML_Node_iterator_H
#define HTML_Node_iterator_H

// standard
#include <iterator>
#include <stack>

// local
#include "Content_Node.h"
#include "HTML_Node.h"

#ifndef	PJL_NO_NAMESPACES
namespace HTML_Tree {
#endif

//*****************************************************************************
//
// SYNOPSIS
//
	class HTML_Node::iterator :
		public std::iterator< std::forward_iterator_tag, HTML_Node >
//
// DESCRIPTION
//
//	An HTML_Node::iterator is-a forward_iterator for an HTML tree.
//
//*****************************************************************************
{
public:
	iterator() { }

	iterator&	operator++();
	iterator	operator++(int) {
				iterator tmp = *this;
				++*this;
				return tmp;
			}

	reference	operator* () const { return *node_; }
	pointer		operator->() const { return  node_; }

	friend bool	operator==( iterator const &i, iterator const &j ) {
				return i.node_ == j.node_;
			}
	friend bool	operator!=( iterator const &i, iterator const &j ) {
				return !( i == j );
			}
private:
	pointer node_;
	Content_Node *parent_;
	std::stack< Content_Node::iterator > stack_;

	iterator( pointer node ) : node_( node ) { push_if_content(); }
	void push_if_content();

	friend class HTML_Node;		// for access to private constructor
};

////////// Inlines ////////////////////////////////////////////////////////////

inline HTML_Node::iterator HTML_Node::begin() {
	return iterator( this );
}
inline HTML_Node::iterator HTML_Node::end() {
	return iterator( 0 );
}

//*****************************************************************************
//
// SYNOPSIS
//
	class HTML_Node::const_iterator :
		public std::iterator<std::forward_iterator_tag, HTML_Node const>
//
// DESCRIPTION
//
//	An HTML_Node::iterator is-a forward_iterator for an HTML tree.
//
//*****************************************************************************
{
public:
	const_iterator() { }

	const_iterator&	operator++();
	const_iterator	operator++(int) {
				const_iterator tmp = *this;
				++*this;
				return tmp;
			}

	reference	operator* () const { return *node_; }
	pointer		operator->() const { return  node_; }

	friend
	bool operator==( const_iterator const &i, const_iterator const &j ) {
		return i.node_ == j.node_;
	}
	friend
	bool operator!=( const_iterator const &i, const_iterator const &j ) {
		return !( i == j );
	}
private:
	pointer node_;
	Content_Node const *parent_;
	std::stack< Content_Node::const_iterator > stack_;

	const_iterator( pointer node ) : node_( node ) { push_if_content(); }
	void push_if_content();

	friend class HTML_Node;		// for access to private constructor
};

////////// Inlines ////////////////////////////////////////////////////////////

inline HTML_Node::const_iterator HTML_Node::begin() const {
	return const_iterator( this );
}
inline HTML_Node::const_iterator HTML_Node::end() const {
	return const_iterator( 0 );
}

#ifndef	PJL_NO_NAMESPACES
}
#endif

#endif	/* HTML_Node_iterator_H */
