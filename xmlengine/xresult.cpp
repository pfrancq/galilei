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

#include "xresult.h"

XResult::XResult(const int &_idfile, const RString &_title, const RString &_url, const RString &_snippet, const int &_score)
{
	idfile = _idfile;
	title = _title;
	url = _url;
	snippet = _snippet;
	score = _score;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XResult::GetIdfile() const
{
	return idfile;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
RString XResult::GetTitle() const
{
	return title;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
RString XResult::GetUrl() const
{
	return url;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
RString XResult::GetSnippet() const
{
	return snippet;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XResult::GetScore() const
{
	return score;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XResult::Compare(const XResult &xr) const
{
	return score - xr.score;
}
