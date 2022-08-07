/**************************************************************

 ATOMATRIX

 The Atomatrix software is a multithreaded execution system
 of atomic matrices with a dynamic compiler and network eviron.

 The Atomatrix software is an open source.

 Please distribute for public domain but do not modify this file
 without consent from the author.

 Authorization on usage of any executable instruction datasets
 that acquires this source may may require license procurement.
 Use of this file and these contents is subject to the terms of
 this copyright notice. Every inclusion of this file or these
 contents are subject to the terms of this copyright notice.

 Copyright (c) 2003 Ballard

 Any use of this file is subject to the terms of the foregoing
 copyright notice and public license.  

 Copyright (C)

 The Atomatrix software is free software; it is redistributable
 or modifiable under the terms of the GNU General Public License
 version two, or any later version, as published by the Free
 Software Foundation.

 Ballard, and any contributor to the Atomatrix software,
 distrubutes the Atomatrix software in hope that the software
 will be useful but without any warranty or without even the
 implied warranty of merchatability or fitness for a particular
 purpose. See the GNU General Public License for more details.

 A copy of the GNU General Public License should accompany this
 software; if not, write to the Free Software Foundation, Inc.,
 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.  

 Author:

   Ballard, Jonathan H.
   1907 Belcot Rd
   California 95825+1106

   CaliforniaKidd@users.SourceForge.NET
   atomatrix.sourceforge.net

*********************************************************/

#include "The GNU General Public License Version Two"

#ifdef __cplusplus
extern "C" {
#endif

struct _atom
	{
	struct _atom *		precursor ;
	struct _atom *		intratom ;
	struct _atom *		intratoms ;
	struct _atom *		orbit ;
	struct _atom *		interatom ;
	struct _atom *		interatoms ;
	unsigned long		allusions ;
	void * 			datum ;
	} ;

typedef struct _atom *		atom ;


#define atominul		(atom)0

typedef unsigned char	byte;
typedef char *		string ; 
typedef char **		strings ;

#include "include.h"

#ifdef __cplusplus
} // extern "C"
#endif

static string AtomatrixSoftwareCopyrightNoticed
	= " Copyright (C) 2003 Ballard " ;