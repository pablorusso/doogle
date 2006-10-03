/*
**	HTML Tree
**	prettyhtml.c
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
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>				/* for getopt(3) */

// local
#include "HTML_Tree.h"
#include "mmap_file.h"
#include "version.h"

#ifndef	PJL_NO_NAMESPACES
using namespace HTML_Tree;
using namespace PJL;
using namespace std;
#endif

//
// POSIX.1 is, IMHO, brain-damaged in the way it makes you determine the
// maximum file-name length, so we'll simply pick a sufficiently large
// constant.  See also: W. Richard Stevens.  "Advanced Programming in the Unix
// Environment," Addison-Wesley, Reading, MA, 1993.  pp. 34-42.
//
#ifdef	NAME_MAX
#undef	NAME_MAX
#endif
int const	NAME_MAX = 255;

extern "C" {
	extern char*	optarg;
	extern int	optind, opterr;
}
char const*		me;			// executable name
struct stat		stat_buf;		// somplace to do a stat(2) in

//
// File test functions.  Those that do not take an argument operate on the last
// file stat'ed.
//
inline bool		file_exists( char const *path ) {
				return ::stat( path, &stat_buf ) != -1;
			}
inline bool		is_plain_file() {
				return S_ISREG( stat_buf.st_mode );
			}

void			usage();

#define ERROR		cerr << me << ": error: "

//*****************************************************************************
//
// SYNOPSIS
//
	int main( int argc, char *argv[] )
//
// DESCRIPTION
//
//	Parse the command line, initialize, call other functions ... the usual
//	things that are done in main().
//
// PARAMETERS
//
//	argc	The number of arguments.
//
//	argv	A vector of the arguments; argv[argc] is null.  Aside from the
//		options below, the arguments are the names of the files and
//		directories to be indexed.
//
// SEE ALSO
//
//	Bjarne Stroustrup.  "The C++ Programming Language, 3rd ed."
//	Addison-Wesley, Reading, MA.  pp. 116-118.
//
//*****************************************************************************
{
	me = ::strrchr( argv[0], '/' );		// determine base name...
	me = me ? me + 1 : argv[0];		// ...of executable

	/////////// Process command-line options //////////////////////////////

	::opterr = 1;
	char const opts[] = "v";
	for ( int opt; (opt = ::getopt( argc, argv, opts )) != EOF; )
		switch ( opt ) {

			case 'v': // Display version and exit.
				cout << "prettyhtml " << version << endl;
				::exit( 0 );

			case '?': // Bad option.
				usage();
		}
	argc -= ::optind, argv += ::optind;
	if ( argc > 1 )
		usage();

	/////////// Get file file to parse ////////////////////////////////////

	char const *file_name;
	if ( !argc ) {
		//
		// Read file name from standard input.
		//
		static char file_name_buf[ NAME_MAX + 1 ];
		if ( !cin.getline( file_name_buf, NAME_MAX ) )
			::exit( 2 );
		file_name = file_name_buf;
	} else {
		//
		// Read file name from command line.
		//
		file_name = *argv;
	}

	if ( !file_exists( file_name ) ) {
		ERROR << '"' << file_name << "\" does not exist\n";
		::exit( 2 );
	}
	if ( !is_plain_file() ) {
		ERROR << '"' << file_name << "\" is not a plain file\n";
		::exit( 2 );
	}

	mmap_file file( file_name );
	if ( !file ) {
		ERROR << "can not open \"" << file_name << "\"\n";
		::exit( 2 );
	}

	/////////// Parse specified file //////////////////////////////////////

	Root_Node *const root = html_parse( file.begin(), file.end(), true );
	if ( root ) {
		cout << root->write( 0 );
		delete root;
	}
	::exit( 0 );
}

//*****************************************************************************
//
//	Miscellaneous function(s)
//
//*****************************************************************************

void usage() {
	cerr <<	"usage: " << me << " [options] [file]\n"
	"options:\n"
	"========\n"
	"-v : Print version number and exit\n";
	::exit( 1 );
}
