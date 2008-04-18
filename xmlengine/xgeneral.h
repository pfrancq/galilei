/***************************************************************************
 *   Copyright (C) 2005 by Jean-Baptiste Valsamis                          *
 *   jvalsami@ulb.ac.be                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __XGENERALH__			// Avoids multiple declarations
#define __XGENERALH__

#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
using namespace std;

#include <rstring.h>
#include <rcontainer.h>
#include <rcursor.h>
using namespace R;

class XAnalyseXMLFile;			// Forwarding declarations
class XAnalyser;
template<class elem> class XFifo;
template<class elem> class XFifoNode;
class XNodeSQL;
class XNodeXML;
class XNodeSet;
class XNode;
class XQuery;
class XQueryNode;
class XQueryRes;
class XWordSQL;
class XXMLAttr;
class XXMLElem;
class XXMLFile;
class XXMLStruct;
class XXMLTag;
class Int;
class IInt;

namespace XConst
{
	enum tOper
	{
		NOP,
		NOT,
		OR,
		AND,
		INC,
		SIB,
		NAND,
		NINC,
		NSIB
	};
	enum tElem
	{
		TAGNAM = 8,
		ATTNAM = 4,
		ATTVAL = 2,
		TAGVAL = 1,
		NOVAL = 0
	};
	tOper GetOper(const RString &s);
}

#include <rtextfile.h>
#define XND "XNode"
#define XNS "XNS"
#define XQR "XQR"
#define INT "int"
#define IIT "iint"
#define XQN "xqno"

extern bool __ISOPEN__;
extern int __IDREFS__;
extern int __XQRREFS__;
extern int __XNSREFS__;
extern int __XNODEREFS__;
extern int __INTREFS__;
extern int __IINTREFS__;
extern int __XQNREFS__;
extern int *__VAR__;

extern RTextFile debugfile;
void __OPEN__();
//void __NEW__(RString a, int b);
//void __DEL__(RString a, int b);
void __VARREF__(RString a);
void __NEW__(RString a, int b, int c = -500);
void __DEL__(RString a, int b, int c = -500);
void __NEW__(RString a, RString b);
void __DEL__(RString a, RString b);
void __PUT__(RString a);

void prn(RContainer<Int, true, true> r);
void prn(RContainer<Int, true, false> r);
template<bool a, bool b> void prn(RContainer<RString, a, b> r)
{
	RCursor<RString> c(r);
	c.Start();
	if (!c.End())
		cout << c()->Latin1();
	for (c.Next(); !c.End(); c.Next())
		cout << "-" << c()->Latin1();
	cout << endl;
}

#endif
