/*
**	HTML Tree
**	Content_Node.h
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

#ifndef Content_Node_H
#define Content_Node_H

// standard
#include <list>

// local
#include "Element_Node.h"

#ifndef	PJL_NO_NAMESPACES
namespace HTML_Tree {
#endif

//*****************************************************************************
//
// SYNOPSIS
//
	class Content_Node : public Element_Node
//
// DESCRIPTION
//
//	A Content_Node is-an HTML_Node for all nodes that have child nodes.  An
//	instance of it is used for the root node of an HTML tree.
//
//*****************************************************************************
{
	typedef std::list< HTML_Node* > child_list;
public:
	typedef child_list::iterator iterator;
	typedef child_list::const_iterator const_iterator;
	typedef child_list::reverse_iterator reverse_iterator;
	typedef child_list::const_reverse_iterator const_reverse_iterator;
	typedef child_list::size_type size_type;

	Content_Node();		// used to create a Root_Node
	Content_Node(
		char const *name, element const&, Content_Node *parent = 0
	);
	Content_Node(
		char const *name, element const&,
		char const *att_begin, char const *att_end,
		Content_Node *parent = 0
	);
	virtual ~Content_Node();

	HTML_Node*		back()		{ return children_.back(); }
	HTML_Node const*	back() const	{ return children_.back(); }
	iterator		begin()		{ return children_.begin(); }
	const_iterator		begin() const	{ return children_.begin(); }
	iterator		end()		{ return children_.end(); }
	const_iterator		end() const	{ return children_.end(); }
	HTML_Node*		front()		{ return children_.front(); }
	HTML_Node const*	front() const	{ return children_.front(); }
	bool			empty() const	{ return children_.empty(); }
	HTML_Node*		erase( iterator const& );
	HTML_Node*		erase( HTML_Node* );
	HTML_Node*		insert( iterator const&, HTML_Node* );
	HTML_Node*		pop_back();
	HTML_Node*		pop_front();
	HTML_Node*		push_back( HTML_Node* );
	HTML_Node*		push_front( HTML_Node* );
	reverse_iterator	rbegin()	{ return children_.rbegin(); }
	const_reverse_iterator	rbegin() const	{ return children_.rbegin(); }
	reverse_iterator	rend()		{ return children_.rend(); }
	const_reverse_iterator	rend() const	{ return children_.rend(); }
	size_type		size() const	{ return children_.size(); }
	virtual void		visit( visitor&, int depth = 0 );
protected:
	virtual bool		write_node( std::ostream&, int, bool ) const;
private:
	child_list		children_;
	bool			destructing_;

	friend class		HTML_Node;
};

typedef Content_Node Root_Node;

////////// Inline functions ///////////////////////////////////////////////////

inline HTML_Node* Content_Node::erase( iterator const &i ) {
	(*i)->parent_ = 0;
	children_.erase( i );
	return *i;
}

inline HTML_Node* Content_Node::pop_back() {
	HTML_Node *const b = back();
	b->parent_ = 0;
	children_.pop_back();
	return b;
}

inline HTML_Node* Content_Node::pop_front() {
	HTML_Node *const f = front();
	f->parent_ = 0;
	children_.pop_front();
	return f;
}

#ifndef	PJL_NO_NAMESPACES
}
#endif

#endif	/* Content_Node_H */
