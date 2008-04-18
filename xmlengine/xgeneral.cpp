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

#include "xgeneral.h"
#include "xnode.h"

#include "xstuff.h"

XConst::tOper XConst::GetOper(const RString &s)
{
	RString sup(s.ToUpper());
	if (sup == "OR")
		return XConst::OR;
	if (sup == "AND")
		return XConst::AND;
	if (sup == "INC")
		return XConst::INC;
	if (sup == "SIB")
		return XConst::SIB;
	if (sup == "NAND" || sup == "AND_NOT" || sup == "ANDNOT")
		return XConst::NAND;
	if (sup == "INC_NOT")
		return XConst::NINC;
	if (sup == "SIB_NOT")
		return XConst::NSIB;
	return XConst::NOP;
}

bool __ISOPEN__ = false;
int __IDREFS__ = 0;
int __XQRREFS__ = 0;
int __XNSREFS__ = 0;
int __XNODEREFS__ = 0;
int __INTREFS__ = 0;
int __IINTREFS__ = 0;
int __XQNREFS__ = 0;
int *__VAR__ = 0;
RTextFile debugfile("/home/fgaultier/prj/DEBUG.LOG");
void __OPEN__()
{
	if (!__ISOPEN__)
		debugfile.Open(RIO::Create);
	__ISOPEN__ = true;
}
void __VARREF__(RString a)
{
	__OPEN__();
	if (a == XQR)
		__VAR__ = &__XQRREFS__;
	else if (a == XNS)
		__VAR__ = &__XNSREFS__;
	else if (a == XND)
		__VAR__ = &__XNODEREFS__;
	else if (a == INT)
		__VAR__ = &__INTREFS__;
	else if (a == IIT)
		__VAR__ = &__IINTREFS__;
	else if (a == XQN)
		__VAR__ = &__XQNREFS__;
}
void __NEW__(RString a, int b, int c)
{
	__OPEN__();
	__VARREF__(a);
	if (!*__VAR__)
		debugfile<<">>>>>>>>>>>>>>>>>";
	if (c == -500)
		debugfile<< "NEW :"<<a << b <<"ref"<<*__VAR__;
	else
		debugfile<< "NEW :"<<a << b << c <<"ref"<<*__VAR__;
	debugfile.WriteLine();
	(*__VAR__)++;
}
void __DEL__(RString a, int b, int c)
{
	__OPEN__();
	__VARREF__(a);
	(*__VAR__)--;
	if (!*__VAR__)
		debugfile<<"<<<<<<<<<<<<<<<<<";
	if (c == -500)
		debugfile<< " DEL:"<<a << b <<"ref"<<*__VAR__;
	else
		debugfile<< " DEL:"<<a << b << c <<"ref"<<*__VAR__;
	debugfile.WriteLine();
}
void __NEW__(RString a, RString b)
{
	__OPEN__();
	__VARREF__(a);
	if (!*__VAR__)
		debugfile<<">>>>>>>>>>>>>>>>>";
	debugfile<< "NEW :"<<a << b <<"ref"<<*__VAR__;
	debugfile.WriteLine();
	(*__VAR__)++;
}
void __DEL__(RString a, RString b)
{
	__OPEN__();
	__VARREF__(a);
	(*__VAR__)--;
	if (!*__VAR__)
		debugfile<<"<<<<<<<<<<<<<<<<<";
	debugfile<< " DEL:"<<a << b <<"ref"<<*__VAR__;
	debugfile.WriteLine();
}
void __PUT__(RString a)
{
	__OPEN__();
	debugfile<<a;
	debugfile.WriteLine();
}
void prn(RContainer<Int, true, true> r)
{
	RCursor<Int> c(r);
	for (c.Start(); !c.End(); c.Next())
		cout << c()->Get() << "-";
	cout << endl;
}
void prn(RContainer<Int, true, false> r)
{
	RCursor<Int> c(r);
	for (c.Start(); !c.End(); c.Next())
		cout << c()->Get() << "-";
	cout << endl;
}
