/*

	GALILEI Research Project

	LangDE.cpp

	German Language - Implementation.

	Copyright 2001 by David Wartel.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for ANSI C/C++
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


//-----------------------------------------------------------------------------
// include files for R
#include <rstring.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langde.h>
using namespace GALILEI;
using namespace std;


//-----------------------------------------------------------------------------
// include files for GALILEI
using namespace stemming;



//-----------------------------------------------------------------------------
//
// class GLangDE
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangDE::GLangDE(GFactoryLang* fac)
	: GLang(fac,"German","de")
{
}


//-----------------------------------------------------------------------------
void GLangDE::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("aber"));
	stop.InsertPtr(new RString("alle"));
	stop.InsertPtr(new RString("allem"));
	stop.InsertPtr(new RString("allen"));
	stop.InsertPtr(new RString("aller"));
	stop.InsertPtr(new RString("alles"));
	stop.InsertPtr(new RString("als"));
	stop.InsertPtr(new RString("also"));
	stop.InsertPtr(new RString("am"));
	stop.InsertPtr(new RString("an"));
	stop.InsertPtr(new RString("ander"));
	stop.InsertPtr(new RString("andere"));
	stop.InsertPtr(new RString("anderem"));
	stop.InsertPtr(new RString("anderen"));
	stop.InsertPtr(new RString("anderer"));
	stop.InsertPtr(new RString("anderes"));
	stop.InsertPtr(new RString("anderm"));
	stop.InsertPtr(new RString("andern"));
	stop.InsertPtr(new RString("anderr"));
	stop.InsertPtr(new RString("anders"));
	stop.InsertPtr(new RString("auch"));
	stop.InsertPtr(new RString("auf"));
	stop.InsertPtr(new RString("aus"));
	stop.InsertPtr(new RString("über"));
	stop.InsertPtr(new RString("bei"));
	stop.InsertPtr(new RString("bin"));
	stop.InsertPtr(new RString("bis"));
	stop.InsertPtr(new RString("bist"));
	stop.InsertPtr(new RString("da"));
	stop.InsertPtr(new RString("daß"));
	stop.InsertPtr(new RString("damit"));
	stop.InsertPtr(new RString("dann"));
	stop.InsertPtr(new RString("das"));
	stop.InsertPtr(new RString("dasselbe"));
	stop.InsertPtr(new RString("dazu"));
	stop.InsertPtr(new RString("dein"));
	stop.InsertPtr(new RString("deine"));
	stop.InsertPtr(new RString("deinem"));
	stop.InsertPtr(new RString("deinen"));
	stop.InsertPtr(new RString("deiner"));
	stop.InsertPtr(new RString("deines"));
	stop.InsertPtr(new RString("dem"));
	stop.InsertPtr(new RString("demselben"));
	stop.InsertPtr(new RString("den"));
	stop.InsertPtr(new RString("denn"));
	stop.InsertPtr(new RString("denselben"));
	stop.InsertPtr(new RString("der"));
	stop.InsertPtr(new RString("derer"));
	stop.InsertPtr(new RString("derselbe"));
	stop.InsertPtr(new RString("derselben"));
	stop.InsertPtr(new RString("des"));
	stop.InsertPtr(new RString("desselben"));
	stop.InsertPtr(new RString("dessen"));
	stop.InsertPtr(new RString("dich"));
	stop.InsertPtr(new RString("die"));
	stop.InsertPtr(new RString("dies"));
	stop.InsertPtr(new RString("diese"));
	stop.InsertPtr(new RString("dieselbe"));
	stop.InsertPtr(new RString("dieselben"));
	stop.InsertPtr(new RString("diesem"));
	stop.InsertPtr(new RString("diesen"));
	stop.InsertPtr(new RString("dieser"));
	stop.InsertPtr(new RString("dieses"));
	stop.InsertPtr(new RString("dir"));
	stop.InsertPtr(new RString("doch"));
	stop.InsertPtr(new RString("dort"));
	stop.InsertPtr(new RString("du"));
	stop.InsertPtr(new RString("durch"));
	stop.InsertPtr(new RString("ein"));
	stop.InsertPtr(new RString("eine"));
	stop.InsertPtr(new RString("einem"));
	stop.InsertPtr(new RString("einen"));
	stop.InsertPtr(new RString("einer"));
	stop.InsertPtr(new RString("eines"));
	stop.InsertPtr(new RString("einig"));
	stop.InsertPtr(new RString("einige"));
	stop.InsertPtr(new RString("einigem"));
	stop.InsertPtr(new RString("einigen"));
	stop.InsertPtr(new RString("einiger"));
	stop.InsertPtr(new RString("einiges"));
	stop.InsertPtr(new RString("einmal"));
	stop.InsertPtr(new RString("er"));
	stop.InsertPtr(new RString("es"));
	stop.InsertPtr(new RString("etwas"));
	stop.InsertPtr(new RString("euch"));
	stop.InsertPtr(new RString("euer"));
	stop.InsertPtr(new RString("eure"));
	stop.InsertPtr(new RString("eurem"));
	stop.InsertPtr(new RString("euren"));
	stop.InsertPtr(new RString("eurer"));
	stop.InsertPtr(new RString("eures"));
	stop.InsertPtr(new RString("für"));
	stop.InsertPtr(new RString("gegen"));
	stop.InsertPtr(new RString("gewesen"));
	stop.InsertPtr(new RString("hab"));
	stop.InsertPtr(new RString("habe"));
	stop.InsertPtr(new RString("haben"));
	stop.InsertPtr(new RString("hat"));
	stop.InsertPtr(new RString("hatte"));
	stop.InsertPtr(new RString("hatten"));
	stop.InsertPtr(new RString("hier"));
	stop.InsertPtr(new RString("hin"));
	stop.InsertPtr(new RString("hinter"));
	stop.InsertPtr(new RString("ich"));
	stop.InsertPtr(new RString("ihm"));
	stop.InsertPtr(new RString("ihn"));
	stop.InsertPtr(new RString("ihnen"));
	stop.InsertPtr(new RString("ihr"));
	stop.InsertPtr(new RString("ihre"));
	stop.InsertPtr(new RString("ihrem"));
	stop.InsertPtr(new RString("ihren"));
	stop.InsertPtr(new RString("ihrer"));
	stop.InsertPtr(new RString("ihres"));
	stop.InsertPtr(new RString("im"));
	stop.InsertPtr(new RString("in"));
	stop.InsertPtr(new RString("indem"));
	stop.InsertPtr(new RString("ins"));
	stop.InsertPtr(new RString("ist"));
	stop.InsertPtr(new RString("jede"));
	stop.InsertPtr(new RString("jedem"));
	stop.InsertPtr(new RString("jeden"));
	stop.InsertPtr(new RString("jeder"));
	stop.InsertPtr(new RString("jedes"));
	stop.InsertPtr(new RString("jene"));
	stop.InsertPtr(new RString("jenem"));
	stop.InsertPtr(new RString("jenen"));
	stop.InsertPtr(new RString("jener"));
	stop.InsertPtr(new RString("jenes"));
	stop.InsertPtr(new RString("jetzt"));
	stop.InsertPtr(new RString("kann"));
	stop.InsertPtr(new RString("können"));
	stop.InsertPtr(new RString("könnte"));
	stop.InsertPtr(new RString("kein"));
	stop.InsertPtr(new RString("keine"));
	stop.InsertPtr(new RString("keinem"));
	stop.InsertPtr(new RString("keinen"));
	stop.InsertPtr(new RString("keiner"));
	stop.InsertPtr(new RString("keines"));
	stop.InsertPtr(new RString("machen"));
	stop.InsertPtr(new RString("man"));
	stop.InsertPtr(new RString("manche"));
	stop.InsertPtr(new RString("manchem"));
	stop.InsertPtr(new RString("manchen"));
	stop.InsertPtr(new RString("mancher"));
	stop.InsertPtr(new RString("manches"));
	stop.InsertPtr(new RString("mein"));
	stop.InsertPtr(new RString("meine"));
	stop.InsertPtr(new RString("meinem"));
	stop.InsertPtr(new RString("meinen"));
	stop.InsertPtr(new RString("meiner"));
	stop.InsertPtr(new RString("meines"));
	stop.InsertPtr(new RString("mich"));
	stop.InsertPtr(new RString("mir"));
	stop.InsertPtr(new RString("mit"));
	stop.InsertPtr(new RString("muss"));
	stop.InsertPtr(new RString("musste"));
	stop.InsertPtr(new RString("nach"));
	stop.InsertPtr(new RString("nicht"));
	stop.InsertPtr(new RString("nichts"));
	stop.InsertPtr(new RString("noch"));
	stop.InsertPtr(new RString("nun"));
	stop.InsertPtr(new RString("nur"));
	stop.InsertPtr(new RString("ob"));
	stop.InsertPtr(new RString("oder"));
	stop.InsertPtr(new RString("ohne"));
	stop.InsertPtr(new RString("sehr"));
	stop.InsertPtr(new RString("sein"));
	stop.InsertPtr(new RString("seine"));
	stop.InsertPtr(new RString("seinem"));
	stop.InsertPtr(new RString("seinen"));
	stop.InsertPtr(new RString("seiner"));
	stop.InsertPtr(new RString("seines"));
	stop.InsertPtr(new RString("selbst"));
	stop.InsertPtr(new RString("sich"));
	stop.InsertPtr(new RString("sie"));
	stop.InsertPtr(new RString("sind"));
	stop.InsertPtr(new RString("so"));
	stop.InsertPtr(new RString("solche"));
	stop.InsertPtr(new RString("solchem"));
	stop.InsertPtr(new RString("solchen"));
	stop.InsertPtr(new RString("solcher"));
	stop.InsertPtr(new RString("solches"));
	stop.InsertPtr(new RString("soll"));
	stop.InsertPtr(new RString("sollte"));
	stop.InsertPtr(new RString("sondern"));
	stop.InsertPtr(new RString("sonst"));
	stop.InsertPtr(new RString("um"));
	stop.InsertPtr(new RString("und"));
	stop.InsertPtr(new RString("uns"));
	stop.InsertPtr(new RString("unse"));
	stop.InsertPtr(new RString("unsem"));
	stop.InsertPtr(new RString("unsen"));
	stop.InsertPtr(new RString("unser"));
	stop.InsertPtr(new RString("unses"));
	stop.InsertPtr(new RString("unter"));
	stop.InsertPtr(new RString("viel"));
	stop.InsertPtr(new RString("vom"));
	stop.InsertPtr(new RString("von"));
	stop.InsertPtr(new RString("vor"));
	stop.InsertPtr(new RString("war"));
	stop.InsertPtr(new RString("waren"));
	stop.InsertPtr(new RString("warst"));
	stop.InsertPtr(new RString("was"));
	stop.InsertPtr(new RString("während"));
	stop.InsertPtr(new RString("würde"));
	stop.InsertPtr(new RString("würden"));
	stop.InsertPtr(new RString("weg"));
	stop.InsertPtr(new RString("weil"));
	stop.InsertPtr(new RString("weiter"));
	stop.InsertPtr(new RString("welche"));
	stop.InsertPtr(new RString("welchem"));
	stop.InsertPtr(new RString("welchen"));
	stop.InsertPtr(new RString("welcher"));
	stop.InsertPtr(new RString("welches"));
	stop.InsertPtr(new RString("wenn"));
	stop.InsertPtr(new RString("werde"));
	stop.InsertPtr(new RString("werden"));
	stop.InsertPtr(new RString("wie"));
	stop.InsertPtr(new RString("wieder"));
	stop.InsertPtr(new RString("will"));
	stop.InsertPtr(new RString("wir"));
	stop.InsertPtr(new RString("wird"));
	stop.InsertPtr(new RString("wirst"));
	stop.InsertPtr(new RString("wo"));
	stop.InsertPtr(new RString("wollen"));
	stop.InsertPtr(new RString("wollte"));
	stop.InsertPtr(new RString("zu"));
	stop.InsertPtr(new RString("zum"));
	stop.InsertPtr(new RString("zur"));
	stop.InsertPtr(new RString("zwar"));
}


//-----------------------------------------------------------------------------
RString GLangDE::GetStemming(const RString& _kwd)
{
	string word;
	 word=_kwd.ToString();
	(*this)(word);
	return RString(word);
}


//------------------------------------------------------------------------------
void GLangDE::CreateParams(RConfig*)
{
}


//-----------------------------------------------------------------------------
GLangDE::~GLangDE(void)
{
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("German",GLangDE,"de")
