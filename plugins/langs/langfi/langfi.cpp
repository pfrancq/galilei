/*

	GALILEI Research Project

	LangFI.cpp

	Finnish Language - Implementation.

	Copyright 2001-2011 by the Snowball Project.
	Copyright 2001-2014 by Pascal Francq.
	Copyright 2001 by David Wartel.
	Copyright 2001-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#include <langfi.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GLangFI
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangFI::GLangFI(GSession* session,GPlugInFactory* fac)
	: GLang(session,fac,"Finnish","fi"), Stemmer(sb_stemmer_new("finnish",0)), StdCodec(RTextEncoding::GetTextEncoding("utf-8"))
{
	if(!Stemmer)
		throw GException("GLangSE : Swedish is not available for stemming");
}


//-----------------------------------------------------------------------------
void GLangFI::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("aiemmin"));
	stop.InsertPtr(new RString("aika"));
	stop.InsertPtr(new RString("aikaa"));
	stop.InsertPtr(new RString("aikaan"));
	stop.InsertPtr(new RString("aikaisemmin"));
	stop.InsertPtr(new RString("aikaisin"));
	stop.InsertPtr(new RString("aikajen"));
	stop.InsertPtr(new RString("aikana"));
	stop.InsertPtr(new RString("aikoina"));
	stop.InsertPtr(new RString("aikoo"));
	stop.InsertPtr(new RString("aikovat"));
	stop.InsertPtr(new RString("aina"));
	stop.InsertPtr(new RString("ainakaan"));
	stop.InsertPtr(new RString("ainakin"));
	stop.InsertPtr(new RString("ainoa"));
	stop.InsertPtr(new RString("ainoat"));
	stop.InsertPtr(new RString("aiomme"));
	stop.InsertPtr(new RString("aion"));
	stop.InsertPtr(new RString("aiotte"));
	stop.InsertPtr(new RString("aist"));
	stop.InsertPtr(new RString("aivan"));
	stop.InsertPtr(new RString("ajan"));
	stop.InsertPtr(new RString("alas"));
	stop.InsertPtr(new RString("alemmas"));
	stop.InsertPtr(new RString("alkuisin"));
	stop.InsertPtr(new RString("alkuun"));
	stop.InsertPtr(new RString("alla"));
	stop.InsertPtr(new RString("alle"));
	stop.InsertPtr(new RString("aloitamme"));
	stop.InsertPtr(new RString("aloitan"));
	stop.InsertPtr(new RString("aloitat"));
	stop.InsertPtr(new RString("aloitatte"));
	stop.InsertPtr(new RString("aloitattivat"));
	stop.InsertPtr(new RString("aloitettava"));
	stop.InsertPtr(new RString("aloitettevaksi"));
	stop.InsertPtr(new RString("aloitettu"));
	stop.InsertPtr(new RString("aloitimme"));
	stop.InsertPtr(new RString("aloitin"));
	stop.InsertPtr(new RString("aloitit"));
	stop.InsertPtr(new RString("aloititte"));
	stop.InsertPtr(new RString("aloittaa"));
	stop.InsertPtr(new RString("aloittamatta"));
	stop.InsertPtr(new RString("aloitti"));
	stop.InsertPtr(new RString("aloittivat"));
	stop.InsertPtr(new RString("alta"));
	stop.InsertPtr(new RString("aluksi"));
	stop.InsertPtr(new RString("alussa"));
	stop.InsertPtr(new RString("alusta"));
	stop.InsertPtr(new RString("annettavaksi"));
	stop.InsertPtr(new RString("annetteva"));
	stop.InsertPtr(new RString("annettu"));
	stop.InsertPtr(new RString("antaa"));
	stop.InsertPtr(new RString("antamatta"));
	stop.InsertPtr(new RString("antoi"));
	stop.InsertPtr(new RString("aoua"));
	stop.InsertPtr(new RString("apu"));
	stop.InsertPtr(new RString("asia"));
	stop.InsertPtr(new RString("asiaa"));
	stop.InsertPtr(new RString("asian"));
	stop.InsertPtr(new RString("asiasta"));
	stop.InsertPtr(new RString("asiat"));
	stop.InsertPtr(new RString("asioiden"));
	stop.InsertPtr(new RString("asioihin"));
	stop.InsertPtr(new RString("asioita"));
	stop.InsertPtr(new RString("asti"));
	stop.InsertPtr(new RString("avuksi"));
	stop.InsertPtr(new RString("avulla"));
	stop.InsertPtr(new RString("avun"));
	stop.InsertPtr(new RString("avutta"));
	stop.InsertPtr(new RString("√§l√§"));
	stop.InsertPtr(new RString("√§lk√∂√∂n"));
	stop.InsertPtr(new RString("edell√§"));
	stop.InsertPtr(new RString("edelle"));
	stop.InsertPtr(new RString("edelleen"));
	stop.InsertPtr(new RString("edelt√§"));
	stop.InsertPtr(new RString("edemm√§s"));
	stop.InsertPtr(new RString("edes"));
	stop.InsertPtr(new RString("edess√§"));
	stop.InsertPtr(new RString("edest√§"));
	stop.InsertPtr(new RString("ehk√§"));
	stop.InsertPtr(new RString("ei"));
	stop.InsertPtr(new RString("eik√§"));
	stop.InsertPtr(new RString("eilen"));
	stop.InsertPtr(new RString("eiv√§t"));
	stop.InsertPtr(new RString("eli"));
	stop.InsertPtr(new RString("ellei"));
	stop.InsertPtr(new RString("elleiv√§t"));
	stop.InsertPtr(new RString("ellemme"));
	stop.InsertPtr(new RString("ellen"));
	stop.InsertPtr(new RString("ellet"));
	stop.InsertPtr(new RString("ellette"));
	stop.InsertPtr(new RString("emme"));
	stop.InsertPtr(new RString("en"));
	stop.InsertPtr(new RString("en√§√§"));
	stop.InsertPtr(new RString("enemm√§n"));
	stop.InsertPtr(new RString("eniten"));
	stop.InsertPtr(new RString("ennen"));
	stop.InsertPtr(new RString("ensi"));
	stop.InsertPtr(new RString("ensimm√§inen"));
	stop.InsertPtr(new RString("ensimm√§iseksi"));
	stop.InsertPtr(new RString("ensimm√§isen"));
	stop.InsertPtr(new RString("ensimm√§isen√§"));
	stop.InsertPtr(new RString("ensimm√§iset"));
	stop.InsertPtr(new RString("ensimm√§isi√§"));
	stop.InsertPtr(new RString("ensimm√§isiksi"));
	stop.InsertPtr(new RString("ensimm√§isin√§"));
	stop.InsertPtr(new RString("ensimm√§ist√§"));
	stop.InsertPtr(new RString("ensin"));
	stop.InsertPtr(new RString("entinen"));
	stop.InsertPtr(new RString("entisen"));
	stop.InsertPtr(new RString("entisi√§"));
	stop.InsertPtr(new RString("entist√§"));
	stop.InsertPtr(new RString("entisten"));
	stop.InsertPtr(new RString("er√§√§t"));
	stop.InsertPtr(new RString("er√§iden"));
	stop.InsertPtr(new RString("er√§s"));
	stop.InsertPtr(new RString("eri"));
	stop.InsertPtr(new RString("eritt√§in"));
	stop.InsertPtr(new RString("erityisesti"));
	stop.InsertPtr(new RString("esi"));
	stop.InsertPtr(new RString("esiin"));
	stop.InsertPtr(new RString("esill√§"));
	stop.InsertPtr(new RString("esimerkiksi"));
	stop.InsertPtr(new RString("et"));
	stop.InsertPtr(new RString("eteen"));
	stop.InsertPtr(new RString("etenkin"));
	stop.InsertPtr(new RString("ett√§"));
	stop.InsertPtr(new RString("ette"));
	stop.InsertPtr(new RString("ettei"));
	stop.InsertPtr(new RString("halua"));
	stop.InsertPtr(new RString("haluaa"));
	stop.InsertPtr(new RString("haluamatta"));
	stop.InsertPtr(new RString("haluamme"));
	stop.InsertPtr(new RString("haluan"));
	stop.InsertPtr(new RString("haluat"));
	stop.InsertPtr(new RString("haluatte"));
	stop.InsertPtr(new RString("haluavat"));
	stop.InsertPtr(new RString("halunnut"));
	stop.InsertPtr(new RString("halusi"));
	stop.InsertPtr(new RString("halusimme"));
	stop.InsertPtr(new RString("halusin"));
	stop.InsertPtr(new RString("halusit"));
	stop.InsertPtr(new RString("halusitte"));
	stop.InsertPtr(new RString("halusivat"));
	stop.InsertPtr(new RString("halutessa"));
	stop.InsertPtr(new RString("haluton"));
	stop.InsertPtr(new RString("h√§n"));
	stop.InsertPtr(new RString("h√§neen"));
	stop.InsertPtr(new RString("h√§nell√§"));
	stop.InsertPtr(new RString("h√§nelle"));
	stop.InsertPtr(new RString("h√§nelt√§"));
	stop.InsertPtr(new RString("h√§nen"));
	stop.InsertPtr(new RString("h√§ness√§"));
	stop.InsertPtr(new RString("h√§nest√§"));
	stop.InsertPtr(new RString("h√§net"));
	stop.InsertPtr(new RString("he"));
	stop.InsertPtr(new RString("hei"));
	stop.InsertPtr(new RString("heid√§n"));
	stop.InsertPtr(new RString("heihin"));
	stop.InsertPtr(new RString("heille"));
	stop.InsertPtr(new RString("heilt√§"));
	stop.InsertPtr(new RString("heiss√§"));
	stop.InsertPtr(new RString("heist√§"));
	stop.InsertPtr(new RString("heit√§"));
	stop.InsertPtr(new RString("helposti"));
	stop.InsertPtr(new RString("heti"));
	stop.InsertPtr(new RString("hetkell√§"));
	stop.InsertPtr(new RString("hieman"));
	stop.InsertPtr(new RString("huolimatta"));
	stop.InsertPtr(new RString("huomenna"));
	stop.InsertPtr(new RString("hyv√§"));
	stop.InsertPtr(new RString("hyv√§√§"));
	stop.InsertPtr(new RString("hyv√§t"));
	stop.InsertPtr(new RString("hyvi√§"));
	stop.InsertPtr(new RString("hyvien"));
	stop.InsertPtr(new RString("hyviin"));
	stop.InsertPtr(new RString("hyviksi"));
	stop.InsertPtr(new RString("hyville"));
	stop.InsertPtr(new RString("hyvilt√§"));
	stop.InsertPtr(new RString("hyvin"));
	stop.InsertPtr(new RString("hyvin√§"));
	stop.InsertPtr(new RString("hyviss√§"));
	stop.InsertPtr(new RString("hyvist√§"));
	stop.InsertPtr(new RString("ihan"));
	stop.InsertPtr(new RString("ilman"));
	stop.InsertPtr(new RString("ilmeisesti"));
	stop.InsertPtr(new RString("itse"));
	stop.InsertPtr(new RString("itse√§√§n"));
	stop.InsertPtr(new RString("itsens√§"));
	stop.InsertPtr(new RString("ja"));
	stop.InsertPtr(new RString("j√§√§"));
	stop.InsertPtr(new RString("j√§lkeen"));
	stop.InsertPtr(new RString("j√§lleen"));
	stop.InsertPtr(new RString("jo"));
	stop.InsertPtr(new RString("johon"));
	stop.InsertPtr(new RString("joiden"));
	stop.InsertPtr(new RString("joihin"));
	stop.InsertPtr(new RString("joiksi"));
	stop.InsertPtr(new RString("joilla"));
	stop.InsertPtr(new RString("joille"));
	stop.InsertPtr(new RString("joilta"));
	stop.InsertPtr(new RString("joissa"));
	stop.InsertPtr(new RString("joista"));
	stop.InsertPtr(new RString("joita"));
	stop.InsertPtr(new RString("joka"));
	stop.InsertPtr(new RString("jokainen"));
	stop.InsertPtr(new RString("jokin"));
	stop.InsertPtr(new RString("joko"));
	stop.InsertPtr(new RString("joku"));
	stop.InsertPtr(new RString("jolla"));
	stop.InsertPtr(new RString("jolle"));
	stop.InsertPtr(new RString("jolloin"));
	stop.InsertPtr(new RString("jolta"));
	stop.InsertPtr(new RString("jompikumpi"));
	stop.InsertPtr(new RString("jonka"));
	stop.InsertPtr(new RString("jonkin"));
	stop.InsertPtr(new RString("jonne"));
	stop.InsertPtr(new RString("joo"));
	stop.InsertPtr(new RString("jopa"));
	stop.InsertPtr(new RString("jos"));
	stop.InsertPtr(new RString("joskus"));
	stop.InsertPtr(new RString("jossa"));
	stop.InsertPtr(new RString("josta"));
	stop.InsertPtr(new RString("jota"));
	stop.InsertPtr(new RString("jotain"));
	stop.InsertPtr(new RString("joten"));
	stop.InsertPtr(new RString("jotenkin"));
	stop.InsertPtr(new RString("jotenkuten"));
	stop.InsertPtr(new RString("jotka"));
	stop.InsertPtr(new RString("jotta"));
	stop.InsertPtr(new RString("jouduimme"));
	stop.InsertPtr(new RString("jouduin"));
	stop.InsertPtr(new RString("jouduit"));
	stop.InsertPtr(new RString("jouduitte"));
	stop.InsertPtr(new RString("joudumme"));
	stop.InsertPtr(new RString("joudun"));
	stop.InsertPtr(new RString("joudutte"));
	stop.InsertPtr(new RString("joukkoon"));
	stop.InsertPtr(new RString("joukossa"));
	stop.InsertPtr(new RString("joukosta"));
	stop.InsertPtr(new RString("joutua"));
	stop.InsertPtr(new RString("joutui"));
	stop.InsertPtr(new RString("joutuivat"));
	stop.InsertPtr(new RString("joutumaan"));
	stop.InsertPtr(new RString("joutuu"));
	stop.InsertPtr(new RString("joutuvat"));
	stop.InsertPtr(new RString("juuri"));
	stop.InsertPtr(new RString("kahdeksan"));
	stop.InsertPtr(new RString("kahdeksannen"));
	stop.InsertPtr(new RString("kahdella"));
	stop.InsertPtr(new RString("kahdelle"));
	stop.InsertPtr(new RString("kahdelta"));
	stop.InsertPtr(new RString("kahden"));
	stop.InsertPtr(new RString("kahdessa"));
	stop.InsertPtr(new RString("kahdesta"));
	stop.InsertPtr(new RString("kahta"));
	stop.InsertPtr(new RString("kahteen"));
	stop.InsertPtr(new RString("kai"));
	stop.InsertPtr(new RString("kaiken"));
	stop.InsertPtr(new RString("kaikille"));
	stop.InsertPtr(new RString("kaikilta"));
	stop.InsertPtr(new RString("kaikkea"));
	stop.InsertPtr(new RString("kaikki"));
	stop.InsertPtr(new RString("kaikkia"));
	stop.InsertPtr(new RString("kaikkiaan"));
	stop.InsertPtr(new RString("kaikkialla"));
	stop.InsertPtr(new RString("kaikkialle"));
	stop.InsertPtr(new RString("kaikkialta"));
	stop.InsertPtr(new RString("kaikkien"));
	stop.InsertPtr(new RString("kaikkin"));
	stop.InsertPtr(new RString("kaksi"));
	stop.InsertPtr(new RString("kannalta"));
	stop.InsertPtr(new RString("kannattaa"));
	stop.InsertPtr(new RString("kanssa"));
	stop.InsertPtr(new RString("kanssaan"));
	stop.InsertPtr(new RString("kanssamme"));
	stop.InsertPtr(new RString("kanssani"));
	stop.InsertPtr(new RString("kanssanne"));
	stop.InsertPtr(new RString("kanssasi"));
	stop.InsertPtr(new RString("kauan"));
	stop.InsertPtr(new RString("kauemmas"));
	stop.InsertPtr(new RString("kautta"));
	stop.InsertPtr(new RString("kehen"));
	stop.InsertPtr(new RString("keiden"));
	stop.InsertPtr(new RString("keihin"));
	stop.InsertPtr(new RString("keiksi"));
	stop.InsertPtr(new RString("keill√§"));
	stop.InsertPtr(new RString("keille"));
	stop.InsertPtr(new RString("keilt√§"));
	stop.InsertPtr(new RString("kein√§"));
	stop.InsertPtr(new RString("keiss√§"));
	stop.InsertPtr(new RString("keist√§"));
	stop.InsertPtr(new RString("keit√§"));
	stop.InsertPtr(new RString("keitt√§"));
	stop.InsertPtr(new RString("keitten"));
	stop.InsertPtr(new RString("keneen"));
	stop.InsertPtr(new RString("keneksi"));
	stop.InsertPtr(new RString("kenell√§"));
	stop.InsertPtr(new RString("kenelle"));
	stop.InsertPtr(new RString("kenelt√§"));
	stop.InsertPtr(new RString("kenen"));
	stop.InsertPtr(new RString("kenen√§"));
	stop.InsertPtr(new RString("keness√§"));
	stop.InsertPtr(new RString("kenest√§"));
	stop.InsertPtr(new RString("kenet"));
	stop.InsertPtr(new RString("kenett√§"));
	stop.InsertPtr(new RString("kenness√§st√§"));
	stop.InsertPtr(new RString("kerran"));
	stop.InsertPtr(new RString("kerta"));
	stop.InsertPtr(new RString("kertaa"));
	stop.InsertPtr(new RString("kesken"));
	stop.InsertPtr(new RString("keskim√§√§rin"));
	stop.InsertPtr(new RString("ket√§"));
	stop.InsertPtr(new RString("ketk√§"));
	stop.InsertPtr(new RString("kiitos"));
	stop.InsertPtr(new RString("kohti"));
	stop.InsertPtr(new RString("koko"));
	stop.InsertPtr(new RString("kokonaan"));
	stop.InsertPtr(new RString("kolmas"));
	stop.InsertPtr(new RString("kolme"));
	stop.InsertPtr(new RString("kolmen"));
	stop.InsertPtr(new RString("kolmesti"));
	stop.InsertPtr(new RString("koska"));
	stop.InsertPtr(new RString("koskaan"));
	stop.InsertPtr(new RString("kovin"));
	stop.InsertPtr(new RString("kuin"));
	stop.InsertPtr(new RString("kuinka"));
	stop.InsertPtr(new RString("kuitenkaan"));
	stop.InsertPtr(new RString("kuitenkin"));
	stop.InsertPtr(new RString("kuka"));
	stop.InsertPtr(new RString("kukaan"));
	stop.InsertPtr(new RString("kukin"));
	stop.InsertPtr(new RString("kumpainen"));
	stop.InsertPtr(new RString("kumpainenkaan"));
	stop.InsertPtr(new RString("kumpi"));
	stop.InsertPtr(new RString("kumpikaan"));
	stop.InsertPtr(new RString("kumpikin"));
	stop.InsertPtr(new RString("kun"));
	stop.InsertPtr(new RString("kuten"));
	stop.InsertPtr(new RString("kuuden"));
	stop.InsertPtr(new RString("kuusi"));
	stop.InsertPtr(new RString("kuutta"));
	stop.InsertPtr(new RString("kyll√§"));
	stop.InsertPtr(new RString("kymmenen"));
	stop.InsertPtr(new RString("kyse"));
	stop.InsertPtr(new RString("l√§hekk√§in"));
	stop.InsertPtr(new RString("l√§hell√§"));
	stop.InsertPtr(new RString("l√§helle"));
	stop.InsertPtr(new RString("l√§helt√§"));
	stop.InsertPtr(new RString("l√§hemm√§s"));
	stop.InsertPtr(new RString("l√§hes"));
	stop.InsertPtr(new RString("l√§hinn√§"));
	stop.InsertPtr(new RString("l√§htien"));
	stop.InsertPtr(new RString("l√§pi"));
	stop.InsertPtr(new RString("liian"));
	stop.InsertPtr(new RString("liki"));
	stop.InsertPtr(new RString("lis√§√§"));
	stop.InsertPtr(new RString("lis√§ksi"));
	stop.InsertPtr(new RString("luo"));
	stop.InsertPtr(new RString("mahdollisimman"));
	stop.InsertPtr(new RString("mahdollista"));
	stop.InsertPtr(new RString("me"));
	stop.InsertPtr(new RString("meid√§n"));
	stop.InsertPtr(new RString("meill√§"));
	stop.InsertPtr(new RString("meille"));
	stop.InsertPtr(new RString("melkein"));
	stop.InsertPtr(new RString("melko"));
	stop.InsertPtr(new RString("menee"));
	stop.InsertPtr(new RString("meneet"));
	stop.InsertPtr(new RString("menemme"));
	stop.InsertPtr(new RString("menen"));
	stop.InsertPtr(new RString("menet"));
	stop.InsertPtr(new RString("menette"));
	stop.InsertPtr(new RString("menev√§t"));
	stop.InsertPtr(new RString("meni"));
	stop.InsertPtr(new RString("menimme"));
	stop.InsertPtr(new RString("menin"));
	stop.InsertPtr(new RString("menit"));
	stop.InsertPtr(new RString("meniv√§t"));
	stop.InsertPtr(new RString("menness√§"));
	stop.InsertPtr(new RString("mennyt"));
	stop.InsertPtr(new RString("menossa"));
	stop.InsertPtr(new RString("mihin"));
	stop.InsertPtr(new RString("mik√§"));
	stop.InsertPtr(new RString("mik√§√§n"));
	stop.InsertPtr(new RString("mik√§li"));
	stop.InsertPtr(new RString("mikin"));
	stop.InsertPtr(new RString("miksi"));
	stop.InsertPtr(new RString("milloin"));
	stop.InsertPtr(new RString("min√§"));
	stop.InsertPtr(new RString("minne"));
	stop.InsertPtr(new RString("minun"));
	stop.InsertPtr(new RString("minut"));
	stop.InsertPtr(new RString("miss√§"));
	stop.InsertPtr(new RString("mist√§"));
	stop.InsertPtr(new RString("mit√§"));
	stop.InsertPtr(new RString("mit√§√§n"));
	stop.InsertPtr(new RString("miten"));
	stop.InsertPtr(new RString("moi"));
	stop.InsertPtr(new RString("molemmat"));
	stop.InsertPtr(new RString("mones"));
	stop.InsertPtr(new RString("monesti"));
	stop.InsertPtr(new RString("monet"));
	stop.InsertPtr(new RString("moni"));
	stop.InsertPtr(new RString("moniaalla"));
	stop.InsertPtr(new RString("moniaalle"));
	stop.InsertPtr(new RString("moniaalta"));
	stop.InsertPtr(new RString("monta"));
	stop.InsertPtr(new RString("muassa"));
	stop.InsertPtr(new RString("muiden"));
	stop.InsertPtr(new RString("muita"));
	stop.InsertPtr(new RString("muka"));
	stop.InsertPtr(new RString("mukaan"));
	stop.InsertPtr(new RString("mukaansa"));
	stop.InsertPtr(new RString("mukana"));
	stop.InsertPtr(new RString("mutta"));
	stop.InsertPtr(new RString("muu"));
	stop.InsertPtr(new RString("muualla"));
	stop.InsertPtr(new RString("muualle"));
	stop.InsertPtr(new RString("muualta"));
	stop.InsertPtr(new RString("muuanne"));
	stop.InsertPtr(new RString("muulloin"));
	stop.InsertPtr(new RString("muun"));
	stop.InsertPtr(new RString("muut"));
	stop.InsertPtr(new RString("muuta"));
	stop.InsertPtr(new RString("muutama"));
	stop.InsertPtr(new RString("muutaman"));
	stop.InsertPtr(new RString("muuten"));
	stop.InsertPtr(new RString("my√∂hemmin"));
	stop.InsertPtr(new RString("my√∂s"));
	stop.InsertPtr(new RString("my√∂sk√§√§n"));
	stop.InsertPtr(new RString("my√∂skin"));
	stop.InsertPtr(new RString("my√∂t√§"));
	stop.InsertPtr(new RString("n√§iden"));
	stop.InsertPtr(new RString("n√§in"));
	stop.InsertPtr(new RString("n√§iss√§"));
	stop.InsertPtr(new RString("n√§iss√§hin"));
	stop.InsertPtr(new RString("n√§iss√§lle"));
	stop.InsertPtr(new RString("n√§iss√§lt√§"));
	stop.InsertPtr(new RString("n√§iss√§st√§"));
	stop.InsertPtr(new RString("n√§it√§"));
	stop.InsertPtr(new RString("n√§m√§"));
	stop.InsertPtr(new RString("ne"));
	stop.InsertPtr(new RString("nelj√§"));
	stop.InsertPtr(new RString("nelj√§√§"));
	stop.InsertPtr(new RString("nelj√§n"));
	stop.InsertPtr(new RString("niiden"));
	stop.InsertPtr(new RString("niin"));
	stop.InsertPtr(new RString("niist√§"));
	stop.InsertPtr(new RString("niit√§"));
	stop.InsertPtr(new RString("noin"));
	stop.InsertPtr(new RString("nopeammin"));
	stop.InsertPtr(new RString("nopeasti"));
	stop.InsertPtr(new RString("nopeiten"));
	stop.InsertPtr(new RString("nro"));
	stop.InsertPtr(new RString("nuo"));
	stop.InsertPtr(new RString("nyt"));
	stop.InsertPtr(new RString("ohi"));
	stop.InsertPtr(new RString("oikein"));
	stop.InsertPtr(new RString("ole"));
	stop.InsertPtr(new RString("olemme"));
	stop.InsertPtr(new RString("olen"));
	stop.InsertPtr(new RString("olet"));
	stop.InsertPtr(new RString("olette"));
	stop.InsertPtr(new RString("oleva"));
	stop.InsertPtr(new RString("olevan"));
	stop.InsertPtr(new RString("olevat"));
	stop.InsertPtr(new RString("oli"));
	stop.InsertPtr(new RString("olimme"));
	stop.InsertPtr(new RString("olin"));
	stop.InsertPtr(new RString("olisi"));
	stop.InsertPtr(new RString("olisimme"));
	stop.InsertPtr(new RString("olisin"));
	stop.InsertPtr(new RString("olisit"));
	stop.InsertPtr(new RString("olisitte"));
	stop.InsertPtr(new RString("olisivat"));
	stop.InsertPtr(new RString("olit"));
	stop.InsertPtr(new RString("olitte"));
	stop.InsertPtr(new RString("olivat"));
	stop.InsertPtr(new RString("olla"));
	stop.InsertPtr(new RString("olleet"));
	stop.InsertPtr(new RString("olli"));
	stop.InsertPtr(new RString("ollut"));
	stop.InsertPtr(new RString("oma"));
	stop.InsertPtr(new RString("omaa"));
	stop.InsertPtr(new RString("omaan"));
	stop.InsertPtr(new RString("omaksi"));
	stop.InsertPtr(new RString("omalle"));
	stop.InsertPtr(new RString("omalta"));
	stop.InsertPtr(new RString("oman"));
	stop.InsertPtr(new RString("omassa"));
	stop.InsertPtr(new RString("omat"));
	stop.InsertPtr(new RString("omia"));
	stop.InsertPtr(new RString("omien"));
	stop.InsertPtr(new RString("omiin"));
	stop.InsertPtr(new RString("omiksi"));
	stop.InsertPtr(new RString("omille"));
	stop.InsertPtr(new RString("omilta"));
	stop.InsertPtr(new RString("omissa"));
	stop.InsertPtr(new RString("omista"));
	stop.InsertPtr(new RString("on"));
	stop.InsertPtr(new RString("onkin"));
	stop.InsertPtr(new RString("onko"));
	stop.InsertPtr(new RString("ovat"));
	stop.InsertPtr(new RString("paikoittain"));
	stop.InsertPtr(new RString("paitsi"));
	stop.InsertPtr(new RString("pakosti"));
	stop.InsertPtr(new RString("paljon"));
	stop.InsertPtr(new RString("paremmin"));
	stop.InsertPtr(new RString("parempi"));
	stop.InsertPtr(new RString("parhaillaan"));
	stop.InsertPtr(new RString("parhaiten"));
	stop.InsertPtr(new RString("p√§√§lle"));
	stop.InsertPtr(new RString("per√§ti"));
	stop.InsertPtr(new RString("perusteella"));
	stop.InsertPtr(new RString("pian"));
	stop.InsertPtr(new RString("pieneen"));
	stop.InsertPtr(new RString("pieneksi"));
	stop.InsertPtr(new RString("pienell√§"));
	stop.InsertPtr(new RString("pienelle"));
	stop.InsertPtr(new RString("pienelt√§"));
	stop.InsertPtr(new RString("pienempi"));
	stop.InsertPtr(new RString("pienest√§"));
	stop.InsertPtr(new RString("pieni"));
	stop.InsertPtr(new RString("pienin"));
	stop.InsertPtr(new RString("puolesta"));
	stop.InsertPtr(new RString("puolestaan"));
	stop.InsertPtr(new RString("runsaasti"));
	stop.InsertPtr(new RString("saakka"));
	stop.InsertPtr(new RString("sadam"));
	stop.InsertPtr(new RString("sama"));
	stop.InsertPtr(new RString("samaa"));
	stop.InsertPtr(new RString("samaan"));
	stop.InsertPtr(new RString("samalla"));
	stop.InsertPtr(new RString("samallalta"));
	stop.InsertPtr(new RString("samallassa"));
	stop.InsertPtr(new RString("samallasta"));
	stop.InsertPtr(new RString("saman"));
	stop.InsertPtr(new RString("samat"));
	stop.InsertPtr(new RString("samoin"));
	stop.InsertPtr(new RString("sata"));
	stop.InsertPtr(new RString("sataa"));
	stop.InsertPtr(new RString("satojen"));
	stop.InsertPtr(new RString("se"));
	stop.InsertPtr(new RString("seitsem√§n"));
	stop.InsertPtr(new RString("sek√§"));
	stop.InsertPtr(new RString("sen"));
	stop.InsertPtr(new RString("seuraavat"));
	stop.InsertPtr(new RString("siell√§"));
	stop.InsertPtr(new RString("sielt√§"));
	stop.InsertPtr(new RString("siihen"));
	stop.InsertPtr(new RString("siin√§"));
	stop.InsertPtr(new RString("siis"));
	stop.InsertPtr(new RString("siit√§"));
	stop.InsertPtr(new RString("sijaan"));
	stop.InsertPtr(new RString("siksi"));
	stop.InsertPtr(new RString("sill√§"));
	stop.InsertPtr(new RString("silloin"));
	stop.InsertPtr(new RString("silti"));
	stop.InsertPtr(new RString("sin√§"));
	stop.InsertPtr(new RString("sinne"));
	stop.InsertPtr(new RString("sinua"));
	stop.InsertPtr(new RString("sinulle"));
	stop.InsertPtr(new RString("sinulta"));
	stop.InsertPtr(new RString("sinun"));
	stop.InsertPtr(new RString("sinussa"));
	stop.InsertPtr(new RString("sinusta"));
	stop.InsertPtr(new RString("sinut"));
	stop.InsertPtr(new RString("sis√§kk√§in"));
	stop.InsertPtr(new RString("sis√§ll√§"));
	stop.InsertPtr(new RString("sit√§"));
	stop.InsertPtr(new RString("siten"));
	stop.InsertPtr(new RString("sitten"));
	stop.InsertPtr(new RString("suoraan"));
	stop.InsertPtr(new RString("suuntaan"));
	stop.InsertPtr(new RString("suuren"));
	stop.InsertPtr(new RString("suuret"));
	stop.InsertPtr(new RString("suuri"));
	stop.InsertPtr(new RString("suuria"));
	stop.InsertPtr(new RString("suurin"));
	stop.InsertPtr(new RString("suurten"));
	stop.InsertPtr(new RString("taa"));
	stop.InsertPtr(new RString("taas"));
	stop.InsertPtr(new RString("taemmas"));
	stop.InsertPtr(new RString("tahansa"));
	stop.InsertPtr(new RString("tai"));
	stop.InsertPtr(new RString("takaa"));
	stop.InsertPtr(new RString("takaisin"));
	stop.InsertPtr(new RString("takana"));
	stop.InsertPtr(new RString("takia"));
	stop.InsertPtr(new RString("tapauksessa"));
	stop.InsertPtr(new RString("tavalla"));
	stop.InsertPtr(new RString("tavoitteena"));
	stop.InsertPtr(new RString("t√§√§ll√§"));
	stop.InsertPtr(new RString("t√§√§lt√§"));
	stop.InsertPtr(new RString("t√§h√§n"));
	stop.InsertPtr(new RString("t√§ll√§"));
	stop.InsertPtr(new RString("t√§ll√∂in"));
	stop.InsertPtr(new RString("t√§m√§"));
	stop.InsertPtr(new RString("t√§m√§n"));
	stop.InsertPtr(new RString("t√§n√§"));
	stop.InsertPtr(new RString("t√§n√§√§n"));
	stop.InsertPtr(new RString("t√§nne"));
	stop.InsertPtr(new RString("t√§ss√§"));
	stop.InsertPtr(new RString("t√§st√§"));
	stop.InsertPtr(new RString("t√§t√§"));
	stop.InsertPtr(new RString("t√§ten"));
	stop.InsertPtr(new RString("t√§ysin"));
	stop.InsertPtr(new RString("t√§ytyv√§t"));
	stop.InsertPtr(new RString("t√§ytyy"));
	stop.InsertPtr(new RString("te"));
	stop.InsertPtr(new RString("tietysti"));
	stop.InsertPtr(new RString("todella"));
	stop.InsertPtr(new RString("toinen"));
	stop.InsertPtr(new RString("toisaalla"));
	stop.InsertPtr(new RString("toisaalle"));
	stop.InsertPtr(new RString("toisaalta"));
	stop.InsertPtr(new RString("toiseen"));
	stop.InsertPtr(new RString("toiseksi"));
	stop.InsertPtr(new RString("toisella"));
	stop.InsertPtr(new RString("toiselle"));
	stop.InsertPtr(new RString("toiselta"));
	stop.InsertPtr(new RString("toisemme"));
	stop.InsertPtr(new RString("toisen"));
	stop.InsertPtr(new RString("toisensa"));
	stop.InsertPtr(new RString("toisessa"));
	stop.InsertPtr(new RString("toisesta"));
	stop.InsertPtr(new RString("toista"));
	stop.InsertPtr(new RString("toistaiseksi"));
	stop.InsertPtr(new RString("toki"));
	stop.InsertPtr(new RString("tosin"));
	stop.InsertPtr(new RString("tuhannen"));
	stop.InsertPtr(new RString("tuhat"));
	stop.InsertPtr(new RString("tule"));
	stop.InsertPtr(new RString("tulee"));
	stop.InsertPtr(new RString("tulemme"));
	stop.InsertPtr(new RString("tulen"));
	stop.InsertPtr(new RString("tulet"));
	stop.InsertPtr(new RString("tulette"));
	stop.InsertPtr(new RString("tulevat"));
	stop.InsertPtr(new RString("tulimme"));
	stop.InsertPtr(new RString("tulin"));
	stop.InsertPtr(new RString("tulisi"));
	stop.InsertPtr(new RString("tulisimme"));
	stop.InsertPtr(new RString("tulisin"));
	stop.InsertPtr(new RString("tulisit"));
	stop.InsertPtr(new RString("tulisitte"));
	stop.InsertPtr(new RString("tulisivat"));
	stop.InsertPtr(new RString("tulit"));
	stop.InsertPtr(new RString("tulitte"));
	stop.InsertPtr(new RString("tulivat"));
	stop.InsertPtr(new RString("tulla"));
	stop.InsertPtr(new RString("tulleet"));
	stop.InsertPtr(new RString("tullut"));
	stop.InsertPtr(new RString("tuntuu"));
	stop.InsertPtr(new RString("tuo"));
	stop.InsertPtr(new RString("tuolla"));
	stop.InsertPtr(new RString("tuolloin"));
	stop.InsertPtr(new RString("tuolta"));
	stop.InsertPtr(new RString("tuonne"));
	stop.InsertPtr(new RString("tuskin"));
	stop.InsertPtr(new RString("tyk√∂"));
	stop.InsertPtr(new RString("usea"));
	stop.InsertPtr(new RString("useasti"));
	stop.InsertPtr(new RString("useimmiten"));
	stop.InsertPtr(new RString("usein"));
	stop.InsertPtr(new RString("useita"));
	stop.InsertPtr(new RString("uudeksi"));
	stop.InsertPtr(new RString("uudelleen"));
	stop.InsertPtr(new RString("uuden"));
	stop.InsertPtr(new RString("uudet"));
	stop.InsertPtr(new RString("uusi"));
	stop.InsertPtr(new RString("uusia"));
	stop.InsertPtr(new RString("uusien"));
	stop.InsertPtr(new RString("uusinta"));
	stop.InsertPtr(new RString("uuteen"));
	stop.InsertPtr(new RString("uutta"));
	stop.InsertPtr(new RString("vaan"));
	stop.InsertPtr(new RString("vai"));
	stop.InsertPtr(new RString("vaiheessa"));
	stop.InsertPtr(new RString("vaikea"));
	stop.InsertPtr(new RString("vaikean"));
	stop.InsertPtr(new RString("vaikeat"));
	stop.InsertPtr(new RString("vaikeilla"));
	stop.InsertPtr(new RString("vaikeille"));
	stop.InsertPtr(new RString("vaikeilta"));
	stop.InsertPtr(new RString("vaikeissa"));
	stop.InsertPtr(new RString("vaikeista"));
	stop.InsertPtr(new RString("vaikka"));
	stop.InsertPtr(new RString("vain"));
	stop.InsertPtr(new RString("varmasti"));
	stop.InsertPtr(new RString("varsin"));
	stop.InsertPtr(new RString("varsinkin"));
	stop.InsertPtr(new RString("varten"));
	stop.InsertPtr(new RString("vasta"));
	stop.InsertPtr(new RString("vastaan"));
	stop.InsertPtr(new RString("vastakkain"));
	stop.InsertPtr(new RString("v√§h√§n"));
	stop.InsertPtr(new RString("v√§hemm√§n"));
	stop.InsertPtr(new RString("v√§hint√§√§n"));
	stop.InsertPtr(new RString("v√§hiten"));
	stop.InsertPtr(new RString("v√§lill√§"));
	stop.InsertPtr(new RString("verran"));
	stop.InsertPtr(new RString("viel√§"));
	stop.InsertPtr(new RString("vierekk√§in"));
	stop.InsertPtr(new RString("vieri"));
	stop.InsertPtr(new RString("viiden"));
	stop.InsertPtr(new RString("viime"));
	stop.InsertPtr(new RString("viimeinen"));
	stop.InsertPtr(new RString("viimeisen"));
	stop.InsertPtr(new RString("viimeksi"));
	stop.InsertPtr(new RString("viisi"));
	stop.InsertPtr(new RString("voi"));
	stop.InsertPtr(new RString("voidaan"));
	stop.InsertPtr(new RString("voimme"));
	stop.InsertPtr(new RString("voin"));
	stop.InsertPtr(new RString("voisi"));
	stop.InsertPtr(new RString("voit"));
	stop.InsertPtr(new RString("voitte"));
	stop.InsertPtr(new RString("voivat"));
	stop.InsertPtr(new RString("vuoden"));
	stop.InsertPtr(new RString("vuoksi"));
	stop.InsertPtr(new RString("vuosi"));
	stop.InsertPtr(new RString("vuosien"));
	stop.InsertPtr(new RString("vuosina"));
	stop.InsertPtr(new RString("vuotta"));
	stop.InsertPtr(new RString("yh√§"));
	stop.InsertPtr(new RString("yhdeks√§n"));
	stop.InsertPtr(new RString("yhden"));
	stop.InsertPtr(new RString("yhdess√§"));
	stop.InsertPtr(new RString("yht√§"));
	stop.InsertPtr(new RString("yht√§√§ll√§"));
	stop.InsertPtr(new RString("yht√§√§lle"));
	stop.InsertPtr(new RString("yht√§√§lt√§"));
	stop.InsertPtr(new RString("yht√§√§n"));
	stop.InsertPtr(new RString("yhteen"));
	stop.InsertPtr(new RString("yhteens√§"));
	stop.InsertPtr(new RString("yhteydess√§"));
	stop.InsertPtr(new RString("yhteyteen"));
	stop.InsertPtr(new RString("yksi"));
	stop.InsertPtr(new RString("yksin"));
	stop.InsertPtr(new RString("yksitt√§in"));
	stop.InsertPtr(new RString("yl√∂s"));
	stop.InsertPtr(new RString("yleens√§"));
	stop.InsertPtr(new RString("ylemm√§s"));
	stop.InsertPtr(new RString("yli"));
}


//-----------------------------------------------------------------------------
RString GLangFI::GetStemming(const RString& kwd)
{
	RCString Str(StdCodec->FromUnicode(kwd,false));
	const sb_symbol * stemmed = sb_stemmer_stem(Stemmer,(const sb_symbol*)Str(),Str.GetLen());
	return RString((char *)stemmed);
}


//------------------------------------------------------------------------------
void GLangFI::CreateParams(GPlugInFactory*)
{
}


//-----------------------------------------------------------------------------
GLangFI::~GLangFI(void)
{
	if(Stemmer)
		sb_stemmer_delete(Stemmer);
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("fi","Finnish",GLangFI)
