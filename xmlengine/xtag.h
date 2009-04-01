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

#ifndef __XTAGH__
#define __XTAGH__

#include <rxmltag.h>
#include <rxmlattr.h>
#include <rstring.h>
using namespace R;

#include "xgeneral.h"

class XXMLAttr;
class XXMLTag;

class XXMLElem
{
public :

	enum tElem
	{
		TAG,
		ATT
	};

protected :

	unsigned int pos, len;
	tElem type;

public :

	void SetByte(unsigned int _pos, unsigned int _len);
	unsigned int GetPos();
	unsigned int GetLen();
	tElem GetType();
	virtual RString Getname() = 0;
	virtual RString Getvalue() = 0;
	virtual int Getnb() = 0;
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

class XXMLTag : public RXMLTag, public XXMLElem
{
public :

//	XXMLTag();
	XXMLTag(RString _name);
	virtual RString Getname();
	virtual RString Getvalue();
	virtual int Getnb();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

class XXMLAttr : public RXMLAttr, public XXMLElem
{
public :

	XXMLAttr(const RString &_name,const RString &_value);
	XXMLAttr(const char *_name,const double _value);
	virtual RString Getname();
	virtual RString Getvalue();
	virtual int Getnb();
};


#endif
