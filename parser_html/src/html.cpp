/*
**	HTML Tree
**	html.c
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
#include <cctype>
#include <cstring>
#include <string>

// local
#include "Comment_Node.h"
#include "Content_Node.h"
#include "Element_Node.h"
#include "html.h"
#include "Text_Node.h"
#include "util.h"

#ifndef	PJL_NO_NAMESPACES
using namespace std;
namespace HTML_Tree {
#endif

int const	Tag_Name_Max_Size = 10;
//		The maximum size of an HTML tag name, e.g., "BLOCKQUOTE".  You
//		might need to increase this if you are indexing HTML documents
//		that contain non-standard tags and at least one of them is
//		longer than the above.

void	parse_html_comment( char const *&pos, char const *end );
void	parse_html_tag( char const *&pos, char const *end, Content_Node*& );
void	skip_html_tag( char const *&pos, char const *end );

//*****************************************************************************
//
// SYNOPSIS
//
	Root_Node* html_parse(
		register char const *c, char const *end,
		bool include_comments
	)
//
// DESCRIPTION
//
//	Parse the HTML file to build a tree.
//
// PARAMETERS
//
//	c	The pointer to the beginning of the buffer.
//
//	end	The pointer to one past the end of the buffer.
//
// RETURN VALUE
//
//	Returns a pointer to the root node of the tree.
//
//*****************************************************************************
{
	Root_Node *const root_node = new Root_Node;
	Content_Node *cur_node = root_node;

	while ( c != end ) {
		char const *const b = c;
		if ( *c++ == '<' ) {
			if ( c == end )
				break;
			if ( *c != '!' ) {
				//
				// It must be an HTML tag.
				//
				parse_html_tag( c, end, cur_node );
				continue;
			}

			if ( ++c == end )
				break;
			if ( *c == '-' && c + 1 != end && c[1] == '-' ) {
				//
				// It's a comment.
				//
				parse_html_comment( c += 2, end );
				if ( include_comments )
					new Comment_Node( b, c, cur_node );
				continue;
			}

			//
			// This is something like <!DOCUMENT
			//
			skip_html_tag( c, end );
			goto new_text_node;
		}

		//
		// Collect a run of text into a Text_Node.
		//
		for ( ; c != end; ++c )
			if ( *c == '<' ) {
				//
				// We've encountered the potential beginning of
				// an HTML tag: stop collecting text and create
				// a new Text_Node containing what we've
				// collected so far.
				//
				break;
			}
new_text_node:
		new Text_Node( b, c, cur_node );
	}

	if ( root_node->empty() ) {
		delete root_node;
		return 0;
	}
	return root_node;
}

//*****************************************************************************
//
// SYNOPSIS
//
	void parse_attributes(
		register char const *c, char const *end,
		Element_Node::attribute_map &attributes
	)
//
// DESCRIPTION
//
//	Parse out all the attributes and their values of an HTML element.
//
// PARAMETERS
//
//	c		The iterator marking the beginning of where to start
//			parsing.
//
//	end		The iterator marking the end of where to stop parsing
//			(usually positioned at the closing '>' character of the
//			HTML tag).
//
//	attributes	Where to deposit the attributes.
//
// SEE ALSO
//
//	Dave Raggett, Arnaud Le Hors, and Ian Jacobs.  "On SGML and HTML: SGML
//	constructs used in HTML: Attributes," HTML 4.0 Specification, section
//	3.2.2, World Wide Web Consortium, April 1998.
//		http://www.w3.org/TR/PR-html40/intro/sgmltut.html#h-3.2.2
//
//*****************************************************************************
{
	while ( c != end && !is_space( *c++ ) ) ;	// skip element name

	while ( c != end ) {
		if ( !isalpha( *c ) ) {
			++c;
			continue;
		}
		//
		// We just found the start of a potential attribute name: now
		// try to find its end by stopping at one of 'end', whitespace,
		// or '='.  To be robust, we allow whitespace around the '='.
		//
		char const *const name_begin = c;
		while ( c != end && !isspace( *c ) && *c != '=' )
			++c;
		char const *const name_end = c;
		while ( c != end && isspace( *c ) )
			++c;
		string const name( to_lower( name_begin, name_end ) );
		if ( c == end || *c != '=' ) {
			//
			// It's a Boolean attribute: set its value to be its
			// own name (per the HTML 4.0 spec).
			//
			attributes[ name ] = name;
			continue;
		}
		if ( c == end )
			break;
		// Here, 'c' is positioned at the '='.
		while ( ++c != end && isspace( *c ) )
			;
		if ( c == end )
			break;
		//
		// Determine the span of the attribute's value: if it started
		// with a quote, it's terminated only by the matching closing
		// quote; if not, it's terminated by a whitespace character (or
		// running into 'end').
		//
		// This is more lenient than the HTML 4.0 specification in that
		// it allows non-quoted values to contain characters other than
		// the set [A-Za-z0-9.-], i.e., any non-whitespace character.
		//
		char const quote = ( *c == '"' || *c == '\'' ) ? *c : 0;
		if ( quote && ++c == end )
			break;
		char const *const b = c;
		for ( ; c != end; ++c )
			if ( quote ) {		// stop at matching quote only
				if ( *c == quote )
					break;
			} else if ( isspace( *c ) )
				break;		// stop at whitespace

		attributes[ name ] = string( b, c );

		if ( c == end )
			break;
		++c;
	}
}

//*****************************************************************************
//
// SYNOPSIS
//
	void parse_html_comment( register char const *&c, char const *end )
//
// DESCRIPTION
//
//	Skip past an HTML comment by scanning for the closing "-->" character
//	sequence.  Unlike skipping an ordinary HTML tag, quotes are not
//	significant and no attempt must be made either to "balance" them or to
//	ignore what is in between them.  The HTML specification permits
//	whitespace between the "--" and the ">" (for some strange reason).
//
//	This function is more lenient than the HTML 4.0 specification in that
//	it allows for a string of hyphens within a comment since this is common
//	in practice; the specification considers this to be an error.
//
// PARAMETERS
//
//	c	The iterator to use.  It is presumed to start anywhere after
//		the initial "<!--"; it is left positioned after the "-->".
//
//	end	The pointer to the end of the file.
//
// SEE ALSO
//
//	Dave Raggett, Arnaud Le Hors, and Ian Jacobs.  "On SGML and HTML: SGML
//	constructs used in HTML: Comments," HTML 4.0 Specification, section
//	3.2.4, World Wide Web Consortium, April 1998.
//		http://www.w3.org/TR/PR-html40/intro/sgmltut.html#h-3.2.4
//
//*****************************************************************************
{
	while ( c != end )
		if ( *c++ == '-' && c != end && *c == '-' ) {
			char const *const d = c;
			while ( ++c != end && is_space( *c ) )
				;
			if ( c == end || *c++ == '>' )
				break;
			c = d;
		}
}

//*****************************************************************************
//
// SYNOPSIS
//
	void parse_html_tag(
		register char const *&c, register char const *end,
		Content_Node *&cur_node
	)
//
// DESCRIPTION
//
//	This function does everything skip_html_tag() does but additionally
//	builds a DOM-like (Document Object Model) tree of nodes.  It does this
//	by knowing when to end elements.
//
// PARAMETERS
//
//	c		The iterator to use.  It must be positioned at the
//			character after the '<'; it is repositioned at the
//			first character after the '>'.
//
//	end		The iterator marking the end of the file.
//
//	cur_node	The current node.  If a new XML node is parsed, this is
//			set to the new node; if an end tag is parsed, this is
//			"popped" to the previous node.
//
//*****************************************************************************
{
	if ( c == end )
		return;
	char const *const tag_begin = c;
	skip_html_tag( c, end );
	char const *const tag_end = c - 1;
	bool const is_end_tag = *tag_begin == '/';

	////////// Determine tag name /////////////////////////////////////////

	char name[ Tag_Name_Max_Size + 2 ];	// 1 for '/', 1 for null
	{ // local scope
	//
	// Copy only the tag name by stopping at a whitespace character (or
	// running into tag_end); also convert it to lower case.  (We don't
	// call to_lower() in util.c so as not to waste time copying the entire
	// tag with its attributes since we only want the tag name.)
	//
	register char *to = name;
	register char const *from = tag_begin;
	while ( from != tag_end && !isspace( *from ) ) {
		//
		// Check to see if the name is too long to be a valid one for an
		// HTML element: if it is, invalidate it by writing "garbage"
		// into the name so something like "BLOCKQUOTED" (an invalid
		// element) won't match "BLOCKQUOTE" (a valid element) when one
		// letter shorter and throw off element closures.
		//
		if ( to - name >= Tag_Name_Max_Size + is_end_tag ) {
			to = name;
			*to++ = '\1';
			break;
		}
		*to++ = to_lower( *from++ );
	}
	*to = '\0';
	} // local scope

	////////// Close open element(s) //////////////////////////////////////

	while (
		cur_node->element_ &&
		cur_node->element_->close_tags.contains( name )
	) {
		//
		// The element we're parsing closes the currently open element,
		// so "pop" the current node.
		//
		Content_Node const *const old_node = cur_node;
		cur_node = cur_node->parent();

		//
		// We have to stop closing elements if we encounter the start
		// tag matching the end tag.
		//
		if ( !::strcmp( name + 1, old_node->name ) )
			break;
	}

	if ( is_end_tag ) {
		//
		// The tag is an end tag: stop here.
		//
		return;
	}

	////////// Look up the name ///////////////////////////////////////////

	static element_map const &elements = element_map::instance();
	element_map::const_iterator const element_found = elements.find( name );
	if ( element_found == elements.end() ) {
		//
		// We didn't find the element in our internal table: ignore it.
		// We really should do something better because this could
		// potentially mess up the proper closing of elements, but,
		// since we know nothing about this element, there's nothing
		// better that can be done.
		//
		return;
	}

	//
	// Found the element in our internal table: add it to the tree.
	//
	if ( element_found->second.end_tag == element::forbidden ) {
		//
		// This element has no end tag, so create an Element_Node for
		// it.
		//
		new Element_Node(
			element_found->first, element_found->second,
			tag_begin, tag_end, cur_node
		);
	} else {
		//
		// The element's end tag isn't forbidden, so create a
		// Content_Node for it and make it the new current node.
		//
		cur_node = new Content_Node(
			element_found->first, element_found->second,
			tag_begin, tag_end, cur_node
		);
	}
}

//*****************************************************************************
//
// SYNOPSIS
//
	void skip_html_tag( register char const *&c, register char const *end )
//
// DESCRIPTION
//
//	Scan for the closing '>' of an HTML tag skipping all characters until
//	it's found.  It takes care to ignore any '>' inside either single or
//	double quotation marks.
//
// PARAMETERS
//
//	c	The iterator to use.  It is presumed to start at any position
//		after the '<' and before the '>'; it is left at the first
//		character after the '>'.
//
//	end	The iterator marking the end of the file.
//
//*****************************************************************************
{
	register char quote = '\0';
	while ( c != end ) {
		if ( quote ) {			// ignore everything...
			if ( *c++ == quote )	// ...until matching quote
				quote = '\0';
			continue;
		}
		if ( *c == '\"' || *c == '\'' ) {
			quote = *c++;		// start ignoring stuff
			continue;
		}
		if ( *c++ == '>' )		// found it  :)
			break;
	}
}

} // namespace HTML_Tree
