/*

	GALILEI Research Project

	GDocOptions.cpp

	Options for the document Analysis - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocoptions.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GDocOptions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDocOptions::GDocOptions(void)
	: StaticLang(false), MinStopWords(0.1), MinWordSize(4), MinStemSize(3),
	  MinOccur(1), MinDocs(5), MaxDocs(300), MinOccurCluster(2), NbIteration(2),
	  NonLetterWords(false), UseLink(false), UseExternalLink(false), LimitLink(false), NbLinks(200)
{
}



//-----------------------------------------------------------------------------
GALILEI::GDocOptions::GDocOptions(const GDocOptions& opt)
	: StaticLang(opt.StaticLang), MinStopWords(opt.MinStopWords), MinWordSize(opt.MinWordSize),
	  MinStemSize(opt.MinStemSize), MinOccur(opt.MinOccur), MinDocs(opt.MinDocs),
	  MaxDocs(opt.MaxDocs),MinOccurCluster(opt.MinOccurCluster), NbIteration(opt.NbIteration),
	  NonLetterWords(opt.NonLetterWords), UseLink(opt.UseLink), UseExternalLink(opt.UseExternalLink),
	  LimitLink(opt.LimitLink), NbLinks(opt.NbLinks)
{
}


//-----------------------------------------------------------------------------
GALILEI::GDocOptions::GDocOptions(const GDocOptions* opt)
	: StaticLang(opt->StaticLang), MinStopWords(opt->MinStopWords), MinWordSize(opt->MinWordSize),
	  MinStemSize(opt->MinStemSize), MinOccur(opt->MinOccur), MinDocs(opt->MinDocs),
	  MaxDocs(opt->MaxDocs),MinOccurCluster(opt->MinOccurCluster), NbIteration(opt->NbIteration),
	  NonLetterWords(opt->NonLetterWords), UseLink(opt->UseLink), UseExternalLink(opt->UseExternalLink),
	  LimitLink(opt->LimitLink), NbLinks(opt->NbLinks)
{
}


//-----------------------------------------------------------------------------
GDocOptions& GALILEI::GDocOptions::operator=(const GDocOptions& opt)
{
	StaticLang=opt.StaticLang;
	MinStopWords=opt.MinStopWords;
	MinWordSize=opt.MinWordSize;
	MinStemSize=opt.MinStemSize;
	MinOccur=opt.MinOccur;
	MinDocs=opt.MinDocs;
	MaxDocs=opt.MaxDocs;
	MinOccurCluster=opt.MinOccurCluster;
	NbIteration=opt.NbIteration;
	NonLetterWords=opt.NonLetterWords;
	UseLink=opt.UseLink;
	UseExternalLink=opt.UseExternalLink;
	LimitLink=opt.LimitLink;
	NbLinks=opt.NbLinks;
	return(*this);
}
