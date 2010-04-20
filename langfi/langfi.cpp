/*

	GALILEI Research Project

	LangFI.cpp

	Finnish Language - Implementation.

	Copyright 2001-2010 by the Snowball Project.
	Copyright 2001-2010 by Pascal Francq.
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
	stop.InsertPtr(new RString("älä"));
	stop.InsertPtr(new RString("älköön"));
	stop.InsertPtr(new RString("edellä"));
	stop.InsertPtr(new RString("edelle"));
	stop.InsertPtr(new RString("edelleen"));
	stop.InsertPtr(new RString("edeltä"));
	stop.InsertPtr(new RString("edemmäs"));
	stop.InsertPtr(new RString("edes"));
	stop.InsertPtr(new RString("edessä"));
	stop.InsertPtr(new RString("edestä"));
	stop.InsertPtr(new RString("ehkä"));
	stop.InsertPtr(new RString("ei"));
	stop.InsertPtr(new RString("eikä"));
	stop.InsertPtr(new RString("eilen"));
	stop.InsertPtr(new RString("eivät"));
	stop.InsertPtr(new RString("eli"));
	stop.InsertPtr(new RString("ellei"));
	stop.InsertPtr(new RString("elleivät"));
	stop.InsertPtr(new RString("ellemme"));
	stop.InsertPtr(new RString("ellen"));
	stop.InsertPtr(new RString("ellet"));
	stop.InsertPtr(new RString("ellette"));
	stop.InsertPtr(new RString("emme"));
	stop.InsertPtr(new RString("en"));
	stop.InsertPtr(new RString("enää"));
	stop.InsertPtr(new RString("enemmän"));
	stop.InsertPtr(new RString("eniten"));
	stop.InsertPtr(new RString("ennen"));
	stop.InsertPtr(new RString("ensi"));
	stop.InsertPtr(new RString("ensimmäinen"));
	stop.InsertPtr(new RString("ensimmäiseksi"));
	stop.InsertPtr(new RString("ensimmäisen"));
	stop.InsertPtr(new RString("ensimmäisenä"));
	stop.InsertPtr(new RString("ensimmäiset"));
	stop.InsertPtr(new RString("ensimmäisiä"));
	stop.InsertPtr(new RString("ensimmäisiksi"));
	stop.InsertPtr(new RString("ensimmäisinä"));
	stop.InsertPtr(new RString("ensimmäistä"));
	stop.InsertPtr(new RString("ensin"));
	stop.InsertPtr(new RString("entinen"));
	stop.InsertPtr(new RString("entisen"));
	stop.InsertPtr(new RString("entisiä"));
	stop.InsertPtr(new RString("entistä"));
	stop.InsertPtr(new RString("entisten"));
	stop.InsertPtr(new RString("eräät"));
	stop.InsertPtr(new RString("eräiden"));
	stop.InsertPtr(new RString("eräs"));
	stop.InsertPtr(new RString("eri"));
	stop.InsertPtr(new RString("erittäin"));
	stop.InsertPtr(new RString("erityisesti"));
	stop.InsertPtr(new RString("esi"));
	stop.InsertPtr(new RString("esiin"));
	stop.InsertPtr(new RString("esillä"));
	stop.InsertPtr(new RString("esimerkiksi"));
	stop.InsertPtr(new RString("et"));
	stop.InsertPtr(new RString("eteen"));
	stop.InsertPtr(new RString("etenkin"));
	stop.InsertPtr(new RString("että"));
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
	stop.InsertPtr(new RString("hän"));
	stop.InsertPtr(new RString("häneen"));
	stop.InsertPtr(new RString("hänellä"));
	stop.InsertPtr(new RString("hänelle"));
	stop.InsertPtr(new RString("häneltä"));
	stop.InsertPtr(new RString("hänen"));
	stop.InsertPtr(new RString("hänessä"));
	stop.InsertPtr(new RString("hänestä"));
	stop.InsertPtr(new RString("hänet"));
	stop.InsertPtr(new RString("he"));
	stop.InsertPtr(new RString("hei"));
	stop.InsertPtr(new RString("heidän"));
	stop.InsertPtr(new RString("heihin"));
	stop.InsertPtr(new RString("heille"));
	stop.InsertPtr(new RString("heiltä"));
	stop.InsertPtr(new RString("heissä"));
	stop.InsertPtr(new RString("heistä"));
	stop.InsertPtr(new RString("heitä"));
	stop.InsertPtr(new RString("helposti"));
	stop.InsertPtr(new RString("heti"));
	stop.InsertPtr(new RString("hetkellä"));
	stop.InsertPtr(new RString("hieman"));
	stop.InsertPtr(new RString("huolimatta"));
	stop.InsertPtr(new RString("huomenna"));
	stop.InsertPtr(new RString("hyvä"));
	stop.InsertPtr(new RString("hyvää"));
	stop.InsertPtr(new RString("hyvät"));
	stop.InsertPtr(new RString("hyviä"));
	stop.InsertPtr(new RString("hyvien"));
	stop.InsertPtr(new RString("hyviin"));
	stop.InsertPtr(new RString("hyviksi"));
	stop.InsertPtr(new RString("hyville"));
	stop.InsertPtr(new RString("hyviltä"));
	stop.InsertPtr(new RString("hyvin"));
	stop.InsertPtr(new RString("hyvinä"));
	stop.InsertPtr(new RString("hyvissä"));
	stop.InsertPtr(new RString("hyvistä"));
	stop.InsertPtr(new RString("ihan"));
	stop.InsertPtr(new RString("ilman"));
	stop.InsertPtr(new RString("ilmeisesti"));
	stop.InsertPtr(new RString("itse"));
	stop.InsertPtr(new RString("itseään"));
	stop.InsertPtr(new RString("itsensä"));
	stop.InsertPtr(new RString("ja"));
	stop.InsertPtr(new RString("jää"));
	stop.InsertPtr(new RString("jälkeen"));
	stop.InsertPtr(new RString("jälleen"));
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
	stop.InsertPtr(new RString("keillä"));
	stop.InsertPtr(new RString("keille"));
	stop.InsertPtr(new RString("keiltä"));
	stop.InsertPtr(new RString("keinä"));
	stop.InsertPtr(new RString("keissä"));
	stop.InsertPtr(new RString("keistä"));
	stop.InsertPtr(new RString("keitä"));
	stop.InsertPtr(new RString("keittä"));
	stop.InsertPtr(new RString("keitten"));
	stop.InsertPtr(new RString("keneen"));
	stop.InsertPtr(new RString("keneksi"));
	stop.InsertPtr(new RString("kenellä"));
	stop.InsertPtr(new RString("kenelle"));
	stop.InsertPtr(new RString("keneltä"));
	stop.InsertPtr(new RString("kenen"));
	stop.InsertPtr(new RString("kenenä"));
	stop.InsertPtr(new RString("kenessä"));
	stop.InsertPtr(new RString("kenestä"));
	stop.InsertPtr(new RString("kenet"));
	stop.InsertPtr(new RString("kenettä"));
	stop.InsertPtr(new RString("kennessästä"));
	stop.InsertPtr(new RString("kerran"));
	stop.InsertPtr(new RString("kerta"));
	stop.InsertPtr(new RString("kertaa"));
	stop.InsertPtr(new RString("kesken"));
	stop.InsertPtr(new RString("keskimäärin"));
	stop.InsertPtr(new RString("ketä"));
	stop.InsertPtr(new RString("ketkä"));
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
	stop.InsertPtr(new RString("kyllä"));
	stop.InsertPtr(new RString("kymmenen"));
	stop.InsertPtr(new RString("kyse"));
	stop.InsertPtr(new RString("lähekkäin"));
	stop.InsertPtr(new RString("lähellä"));
	stop.InsertPtr(new RString("lähelle"));
	stop.InsertPtr(new RString("läheltä"));
	stop.InsertPtr(new RString("lähemmäs"));
	stop.InsertPtr(new RString("lähes"));
	stop.InsertPtr(new RString("lähinnä"));
	stop.InsertPtr(new RString("lähtien"));
	stop.InsertPtr(new RString("läpi"));
	stop.InsertPtr(new RString("liian"));
	stop.InsertPtr(new RString("liki"));
	stop.InsertPtr(new RString("lisää"));
	stop.InsertPtr(new RString("lisäksi"));
	stop.InsertPtr(new RString("luo"));
	stop.InsertPtr(new RString("mahdollisimman"));
	stop.InsertPtr(new RString("mahdollista"));
	stop.InsertPtr(new RString("me"));
	stop.InsertPtr(new RString("meidän"));
	stop.InsertPtr(new RString("meillä"));
	stop.InsertPtr(new RString("meille"));
	stop.InsertPtr(new RString("melkein"));
	stop.InsertPtr(new RString("melko"));
	stop.InsertPtr(new RString("menee"));
	stop.InsertPtr(new RString("meneet"));
	stop.InsertPtr(new RString("menemme"));
	stop.InsertPtr(new RString("menen"));
	stop.InsertPtr(new RString("menet"));
	stop.InsertPtr(new RString("menette"));
	stop.InsertPtr(new RString("menevät"));
	stop.InsertPtr(new RString("meni"));
	stop.InsertPtr(new RString("menimme"));
	stop.InsertPtr(new RString("menin"));
	stop.InsertPtr(new RString("menit"));
	stop.InsertPtr(new RString("menivät"));
	stop.InsertPtr(new RString("mennessä"));
	stop.InsertPtr(new RString("mennyt"));
	stop.InsertPtr(new RString("menossa"));
	stop.InsertPtr(new RString("mihin"));
	stop.InsertPtr(new RString("mikä"));
	stop.InsertPtr(new RString("mikään"));
	stop.InsertPtr(new RString("mikäli"));
	stop.InsertPtr(new RString("mikin"));
	stop.InsertPtr(new RString("miksi"));
	stop.InsertPtr(new RString("milloin"));
	stop.InsertPtr(new RString("minä"));
	stop.InsertPtr(new RString("minne"));
	stop.InsertPtr(new RString("minun"));
	stop.InsertPtr(new RString("minut"));
	stop.InsertPtr(new RString("missä"));
	stop.InsertPtr(new RString("mistä"));
	stop.InsertPtr(new RString("mitä"));
	stop.InsertPtr(new RString("mitään"));
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
	stop.InsertPtr(new RString("myöhemmin"));
	stop.InsertPtr(new RString("myös"));
	stop.InsertPtr(new RString("myöskään"));
	stop.InsertPtr(new RString("myöskin"));
	stop.InsertPtr(new RString("myötä"));
	stop.InsertPtr(new RString("näiden"));
	stop.InsertPtr(new RString("näin"));
	stop.InsertPtr(new RString("näissä"));
	stop.InsertPtr(new RString("näissähin"));
	stop.InsertPtr(new RString("näissälle"));
	stop.InsertPtr(new RString("näissältä"));
	stop.InsertPtr(new RString("näissästä"));
	stop.InsertPtr(new RString("näitä"));
	stop.InsertPtr(new RString("nämä"));
	stop.InsertPtr(new RString("ne"));
	stop.InsertPtr(new RString("neljä"));
	stop.InsertPtr(new RString("neljää"));
	stop.InsertPtr(new RString("neljän"));
	stop.InsertPtr(new RString("niiden"));
	stop.InsertPtr(new RString("niin"));
	stop.InsertPtr(new RString("niistä"));
	stop.InsertPtr(new RString("niitä"));
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
	stop.InsertPtr(new RString("päälle"));
	stop.InsertPtr(new RString("peräti"));
	stop.InsertPtr(new RString("perusteella"));
	stop.InsertPtr(new RString("pian"));
	stop.InsertPtr(new RString("pieneen"));
	stop.InsertPtr(new RString("pieneksi"));
	stop.InsertPtr(new RString("pienellä"));
	stop.InsertPtr(new RString("pienelle"));
	stop.InsertPtr(new RString("pieneltä"));
	stop.InsertPtr(new RString("pienempi"));
	stop.InsertPtr(new RString("pienestä"));
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
	stop.InsertPtr(new RString("seitsemän"));
	stop.InsertPtr(new RString("sekä"));
	stop.InsertPtr(new RString("sen"));
	stop.InsertPtr(new RString("seuraavat"));
	stop.InsertPtr(new RString("siellä"));
	stop.InsertPtr(new RString("sieltä"));
	stop.InsertPtr(new RString("siihen"));
	stop.InsertPtr(new RString("siinä"));
	stop.InsertPtr(new RString("siis"));
	stop.InsertPtr(new RString("siitä"));
	stop.InsertPtr(new RString("sijaan"));
	stop.InsertPtr(new RString("siksi"));
	stop.InsertPtr(new RString("sillä"));
	stop.InsertPtr(new RString("silloin"));
	stop.InsertPtr(new RString("silti"));
	stop.InsertPtr(new RString("sinä"));
	stop.InsertPtr(new RString("sinne"));
	stop.InsertPtr(new RString("sinua"));
	stop.InsertPtr(new RString("sinulle"));
	stop.InsertPtr(new RString("sinulta"));
	stop.InsertPtr(new RString("sinun"));
	stop.InsertPtr(new RString("sinussa"));
	stop.InsertPtr(new RString("sinusta"));
	stop.InsertPtr(new RString("sinut"));
	stop.InsertPtr(new RString("sisäkkäin"));
	stop.InsertPtr(new RString("sisällä"));
	stop.InsertPtr(new RString("sitä"));
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
	stop.InsertPtr(new RString("täällä"));
	stop.InsertPtr(new RString("täältä"));
	stop.InsertPtr(new RString("tähän"));
	stop.InsertPtr(new RString("tällä"));
	stop.InsertPtr(new RString("tällöin"));
	stop.InsertPtr(new RString("tämä"));
	stop.InsertPtr(new RString("tämän"));
	stop.InsertPtr(new RString("tänä"));
	stop.InsertPtr(new RString("tänään"));
	stop.InsertPtr(new RString("tänne"));
	stop.InsertPtr(new RString("tässä"));
	stop.InsertPtr(new RString("tästä"));
	stop.InsertPtr(new RString("tätä"));
	stop.InsertPtr(new RString("täten"));
	stop.InsertPtr(new RString("täysin"));
	stop.InsertPtr(new RString("täytyvät"));
	stop.InsertPtr(new RString("täytyy"));
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
	stop.InsertPtr(new RString("tykö"));
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
	stop.InsertPtr(new RString("vähän"));
	stop.InsertPtr(new RString("vähemmän"));
	stop.InsertPtr(new RString("vähintään"));
	stop.InsertPtr(new RString("vähiten"));
	stop.InsertPtr(new RString("välillä"));
	stop.InsertPtr(new RString("verran"));
	stop.InsertPtr(new RString("vielä"));
	stop.InsertPtr(new RString("vierekkäin"));
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
	stop.InsertPtr(new RString("yhä"));
	stop.InsertPtr(new RString("yhdeksän"));
	stop.InsertPtr(new RString("yhden"));
	stop.InsertPtr(new RString("yhdessä"));
	stop.InsertPtr(new RString("yhtä"));
	stop.InsertPtr(new RString("yhtäällä"));
	stop.InsertPtr(new RString("yhtäälle"));
	stop.InsertPtr(new RString("yhtäältä"));
	stop.InsertPtr(new RString("yhtään"));
	stop.InsertPtr(new RString("yhteen"));
	stop.InsertPtr(new RString("yhteensä"));
	stop.InsertPtr(new RString("yhteydessä"));
	stop.InsertPtr(new RString("yhteyteen"));
	stop.InsertPtr(new RString("yksi"));
	stop.InsertPtr(new RString("yksin"));
	stop.InsertPtr(new RString("yksittäin"));
	stop.InsertPtr(new RString("ylös"));
	stop.InsertPtr(new RString("yleensä"));
	stop.InsertPtr(new RString("ylemmäs"));
	stop.InsertPtr(new RString("yli"));
}


//-----------------------------------------------------------------------------
RString GLangFI::GetStemming(const RString& kwd)
{
	RCString Str(StdCodec->FromUnicode(kwd));
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
