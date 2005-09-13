/*

	GALILEI Research Project

	GHTMLConverter.cpp

	HTML file Converter to DocXML - Implementation.

	Copyright 2004 by the UniversitLibre de Bruxelles.

	Authors:
		Vandaele Valery
		Pascal Francq (pfrancq@ulb.ac.be).

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//------------------------------------------------------
// include files for R Project
#include <ghtmlconverter.h>
using namespace R;
using namespace std;
using namespace GALILEI;


//------------------------------------------------------------------------------
//
// class GHTMLConverter::Tag
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
class GHTMLConverter::Tag
{
public:
	RString Name;      // Name of the HTML Tag
	RString XMLName;   //DocXML Name
	RString XMLParent;   //DocXML Name
	bool Single;       // A single Tag (ex: br)?

	Tag(RString n,RString x,RString p,bool s)
		: Name(n), XMLName(x),XMLParent(p),Single(s) {}
	int Compare(const Tag* t) const {return(Name.Compare(t->Name));}
	int Compare(const Tag& t) const {return(Name.Compare(t.Name));}
	int Compare(RString t) const {return(Name.Compare(t));}
};


//------------------------------------------------------------------------------
//
// struct CodeToChar
//
//------------------------------------------------------------------------------
struct RCharCode
{
	RString Code;
	RChar Car;
};


//------------------------------------------------------------------------------
// All the supported codes from HTML or XML to normal.
#warning Automatic code extraction must be possible
const RCharCode FromXML[]={
{"#13",0x000D},
{"#32",0x0020},
{"#33",0x0021},
{"#34",0x0022},
{"#35",0x0023},
{"#36",0x0024},
{"#37",0x0025},
{"#38",0x0026},
{"#39",0x0027},
{"#40",0x0028},
{"#41",0x0029},
{"#42",0x002A},
{"#43",0x002B},
{"#44",0x002C},
{"#45",0x002D},
{"#46",0x002E},
{"#47",0x002F},
{"#58",0x003A},
{"#59",0x003B},
{"#60",0x003C},
{"#61",0x003D},
{"#62",0x003E},
{"#63",0x003F},
{"#64",0x0040},
{"#91",0x005B},
{"#92",0x005C},
{"#93",0x005D},
{"#94",0x005E},
{"#95",0x005F},
{"#96",0x0060},
{"#123",0x007B},
{"#124",0x007C},
{"#125",0x007D},
{"#126",0x007E},
{"#128",0x007F},
{"#130",0x0082},
{"#131",0x0083},
{"#132",0x0084},
{"#133",0x0085},
{"#134",0x0086},
{"#135",0x0087},
{"#136",0x0088},
{"#137",0x0089},
{"#138",0x008A},
{"#139",0x008B},
{"#140",0x008C},
{"#142",0x008E},
{"#145",0x0091},
{"#146",0x0092},
{"#147",0x0093},
{"#148",0x0094},
{"#149",0x0095},
{"#150",0x0096},
{"#151",0x0097},
{"#152",0x0098},
{"#153",0x0099},
{"#154",0x009A},
{"#155",0x009B},
{"#156",0x009C},
{"#158",0x009E},
{"#159",0x009F},
{"#160",0x00A0},
{"#161",0x00A1},
{"#162",0x00A2},
{"#163",0x00A3},
{"#164",0x00A4},
{"#165",0x00A5},
{"#166",0x00A6},
{"#167",0x00A7},
{"#168",0x00A8},
{"#169",0x00A9},
{"#170",0x00AA},
{"#171",0x00AB},
{"#172",0x00AC},
{"#173",0x00AD},
{"#174",0x00AE},
{"#175",0x00AF},
{"#176",0x00B0},
{"#177",0x00B1},
{"#178",0x00B2},
{"#179",0x00B3},
{"#180",0x00B4},
{"#181",0x00B5},
{"#182",0x00B6},
{"#183",0x00B7},
{"#184",0x00B8},
{"#185",0x00B9},
{"#186",0x00BA},
{"#187",0x00BB},
{"#188",0x00BC},
{"#189",0x00BD},
{"#190",0x00BE},
{"#191",0x00BF},
{"#192",0x00C0},
{"#193",0x00C1},
{"#194",0x00C2},
{"#195",0x00C3},
{"#196",0x00C4},
{"#197",0x00C5},
{"#198",0x00C6},
{"#199",0x00C7},
{"#200",0x00C8},
{"#201",0x00C9},
{"#202",0x00CA},
{"#203",0x00CB},
{"#204",0x00CC},
{"#205",0x00CD},
{"#206",0x00CE},
{"#207",0x00CF},
{"#208",0x00D0},
{"#209",0x00D1},
{"#210",0x00D2},
{"#211",0x00D3},
{"#212",0x00D4},
{"#213",0x00D5},
{"#214",0x00D6},
{"#215",0x00D7},
{"#216",0x00D8},
{"#217",0x00D9},
{"#218",0x00DA},
{"#219",0x00DB},
{"#220",0x00DC},
{"#221",0x00DD},
{"#222",0x00DE},
{"#223",0x00DF},
{"#224",0x00E0},
{"#225",0x00E1},
{"#226",0x00E2},
{"#227",0x00E3},
{"#228",0x00E4},
{"#229",0x00E5},
{"#230",0x00E6},
{"#231",0x00E7},
{"#232",0x00E8},
{"#233",0x00E9},
{"#234",0x00EA},
{"#235",0x00EB},
{"#236",0x00EC},
{"#237",0x00ED},
{"#238",0x00EE},
{"#239",0x00EF},
{"#240",0x00F0},
{"#241",0x00F1},
{"#242",0x00F2},
{"#243",0x00F3},
{"#244",0x00F4},
{"#245",0x00F5},
{"#246",0x00F6},
{"#247",0x00F7},
{"#248",0x00F8},
{"#249",0x00F9},
{"#250",0x00FA},
{"#251",0x00FB},
{"#252",0x00FC},
{"#253",0x00FD},
{"#254",0x00FE},
{"#255",0x00FF},
{"#338",0x0152},
{"#339",0x0153},
{"#352",0x0160},
{"#353",0x0161},
{"#376",0x0178},
{"#710",0x02C6},
{"#732",0x02DC},
{"#913",0x0391},
{"#914",0x0392},
{"#915",0x0393},
{"#916",0x0394},
{"#917",0x0395},
{"#918",0x0396},
{"#919",0x0397},
{"#920",0x0398},
{"#921",0x0399},
{"#922",0x039A},
{"#923",0x039B},
{"#924",0x039C},
{"#925",0x039D},
{"#926",0x039E},
{"#927",0x039F},
{"#928",0x03A0},
{"#929",0x03A1},
{"#931",0x03A3},
{"#932",0x03A4},
{"#933",0x03A5},
{"#934",0x03A6},
{"#935",0x03A7},
{"#936",0x03A8},
{"#937",0x03A9},
{"#945",0x03B1},
{"#946",0x03B2},
{"#947",0x03B3},
{"#948",0x03B4},
{"#949",0x03B5},
{"#950",0x03B6},
{"#951",0x03B7},
{"#952",0x03B8},
{"#953",0x03B9},
{"#954",0x03BA},
{"#955",0x03BB},
{"#956",0x03BC},
{"#957",0x03BD},
{"#958",0x03BE},
{"#959",0x03BF},
{"#959",0x03BF},
{"#960",0x03C0},
{"#961",0x03C1},
{"#962",0x03C2},
{"#963",0x03C3},
{"#964",0x03C4},
{"#965",0x03C5},
{"#966",0x03C6},
{"#967",0x03C7},
{"#968",0x03C8},
{"#969",0x03C9},
{"#977",0x03D1},
{"#978",0x03D2},
{"#982",0x03D6},
{"#8194",0x2002},
{"#8195",0x2003},
{"#8201",0x2009},
{"#8204",0x200C},
{"#8205",0x200D},
{"#8206",0x200E},
{"#8207",0x200F},
{"#8211",0x2013},
{"#8212",0x2014},
{"#8216",0x2018},
{"#8217",0x2019},
{"#8218",0x201A},
{"#8220",0x201C},
{"#8221",0x201D},
{"#8222",0x201E},
{"#8224",0x2020},
{"#8225",0x2021},
{"#8226",0x2022},
{"#8230",0x2026},
{"#8240",0x2030},
{"#8242",0x2032},
{"#8243",0x2033},
{"#8249",0x2039},
{"#8250",0x203A},
{"#8254",0x203E},
{"#8260",0x2044},
{"#8364",0x20AC},
{"#8472",0x2118},
{"#8465",0x2111},
{"#8476",0x211C},
{"#8482",0x2122},
{"#8501",0x2135},
{"#8592",0x2190},
{"#8593",0x2191},
{"#8594",0x2192},
{"#8595",0x2193},
{"#8596",0x2194},
{"#8629",0x21B5},
{"#8656",0x21D0},
{"#8657",0x21D1},
{"#8658",0x21D2},
{"#8659",0x21D3},
{"#8660",0x21D4},
{"#8704",0x2200},
{"#8706",0x2202},
{"#8707",0x2203},
{"#8709",0x2205},
{"#8711",0x2207},
{"#8712",0x2208},
{"#8713",0x2209},
{"#8715",0x220B},
{"#8719",0x220F},
{"#8721",0x2211},
{"#8722",0x2212},
{"#8727",0x2217},
{"#8730",0x221A},
{"#8733",0x221D},
{"#8734",0x221E},
{"#8736",0x2220},
{"#8743",0x2227},
{"#8744",0x2228},
{"#8745",0x2229},
{"#8746",0x222A},
{"#8747",0x222B},
{"#8756",0x2234},
{"#8764",0x223C},
{"#8773",0x2245},
{"#8776",0x2248},
{"#8800",0x2260},
{"#8801",0x2261},
{"#8804",0x2264},
{"#8805",0x2265},
{"#8834",0x2282},
{"#8835",0x2283},
{"#8836",0x2284},
{"#8838",0x2286},
{"#8839",0x2287},
{"#8853",0x2295},
{"#8855",0x2297},
{"#8869",0x22A5},
{"#8901",0x22C5},
{"#8968",0x2308},
{"#8969",0x2309},
{"#8970",0x230A},
{"#8971",0x230B},
{"#9001",0x2329},
{"#9002",0x232A},
{"#9674",0x25CA},
{"#9824",0x2660},
{"#9827",0x2663},
{"#9829",0x2665},
{"#9830",0x2666},
{"AElig",0x00C6},
{"Aacute",0x00C1},
{"Acirc",0x00C2},
{"Agrave",0x00C4},
{"Alpha",0x0391},
{"Aring",0x00C5},
{"Atilde",0x00C3},
{"Auml",0x00C4},
{"Beta",0x0392},
{"Ccedil",0x00C7},
{"Chi",0x03A7},
{"Dagger",0x2021},
{"Delta",0x0394},
{"ETH",0x00D0},
{"Eacute",0x00C9},
{"Ecirc",0x00CA},
{"Egrave",0x00C8},
{"Epsilon",0x0395},
{"Eta",0x0397},
{"Euml",0x00CB},
{"Gamma",0x0393},
{"Iacute",0x00CD},
{"Icirc",0x00CE},
{"Igrave",0x00CC},
{"Iota",0x0399},
{"Iuml",0x00CF},
{"Kappa",0x039A},
{"Lambda",0x039B},
{"Mu",0x039C},
{"Ntilde",0x00D1},
{"Nu",0x039D},
{"OElig",0x0152},
{"Oacute",0x00D3},
{"Ocirc",0x00D4},
{"Ograve",0x00D2},
{"Omega",0x03A9},
{"Omicron",0x039F},
{"Oslash",0x00D8},
{"Otilde",0x00D5},
{"Ouml",0x00D6},
{"Phi",0x03A6},
{"Pi",0x03A0},
{"Prime",0x2033},
{"Psi",0x03A8},
{"Rho",0x03A1},
{"Scaron",'?'},
{"Sigma",0x03A3},
{"THORN",0x00FE},
{"Tau",0x03A4},
{"Theta",0x0398},
{"Uacute",0x00DA},
{"Ucirc",0x00DB},
{"Ugrave",0x00D9},
{"Upsilon",0x03A5},
{"Uuml",0x00DC},
{"Xi",0x039E},
{"Yacute",0x00DD},
{"Yuml",0x0178},
{"Zeta",0x0396},
{"aacute",0x00E1},
{"acirc",0x00E2},
{"acute",0x00B4},
{"aelig",0x00E6},
{"agrave",0x00E0},
{"alefsym",0x2135},
{"alpha",0x03B1},
{"amp",0x0026},
{"and",0x2227},
{"ang",0x2220},
{"aring",0x00E5},
{"asymp",0x2248},
{"atilde",0x00E3},
{"auml",0x00E4},
{"bdquo",0x201E},
{"beta",0x03B2},
{"brvbar",0x00a6},
{"bull",0x2022},
{"cap",0x2229},
{"ccedil",0x00E7},
{"cedil",0x00B8},
{"cent",0x00A2},
{"chi",0x03C7},
{"circ",0x02C6},
{"clubs",0x2663},
{"cong",0x2245},
{"copy",0x00A9},
{"crarr",0x21B5},
{"cup",0x222A},
{"curren",0x00A4},
{"dArr",0x21D3},
{"dagger",0x2020},
{"darr",0x2193},
{"deg",0x00B0},
{"delta",0x03B4},
{"diams",0x2666},
{"divide",0x00F7},
{"eacute",0x00E9},
{"ecirc",0x00EA},
{"egrave",0x0E8},
{"empty",0x2205},
{"emsp",0x2003},
{"ensp",0x2002},
{"epsilon",0x03B5},
{"equiv",0x2261},
{"eta",0x03B7},
{"eth",0x00F0},
{"euml",0x00EB},
{"euro",0x20AC},
{"exist",0x2203},
{"forall",0x2200},
{"frac12",0x00BD},
{"frac14",0x00BC},
{"frac34",0x00BE},
{"frasl",0x002F},
{"gamma",0x03B3},
{"ge",0x2265},
{"gt",0x003E},
{"hArr",0x21D4},
{"harr",0x2194},
{"hearts",0x2665},
{"hellip",0x2026},
{"iacute",0x00ED},
{"icirc",0x00EE},
{"iexcl",0x00A1},
{"igrave",0x00EC},
{"image",0x2111},
{"infin",0x221E},
{"int",0x222B},
{"iota",0x03B9},
{"iquest",0x00BF},
{"isin",0x2208},
{"iuml",0x00ef},
{"kappa",0x03BA},
{"lArr",0x21D0},
{"lang",0x2329},
{"lambda",0x03BB},
{"laquo",0x00AB},
{"larr",0x2190},
{"lceil",0x2308},
{"ldquo",0x201C},
{"le",0x2264},
{"lfloor",0x230A},
{"lowast",0x2217},
{"loz",0x25CA},
{"lrm",0x200E},
{"lsaquo",0x2039},
{"lsquo",0x2018},
{"lt",0x003C},
{"macr",0x00AF},
{"mdash",0x2014},
{"micro",0x00B5},
{"middot",0x00B6},
{"minus",0x2212},
{"mu",0x03BC},
{"nabla",0x2207},
{"nbsp",0x0020},
{"ndash",0x2013},
{"ne",0x2260},
{"ni",0x220B},
{"not",0x00AC},
{"notin",0x2209},
{"nsub",0x2284},
{"ntilde",0x00F1},
{"nu",0x03BD},
{"oacute",0x00F6},
{"ocirc",0x00F4},
{"oelig",0x0153},
{"ograve",0x00F2},
{"oline",0x203E},
{"omega",0x03C9},
{"omicron",0x03BF},
{"oplus",0x2295},
{"or",0x2228},
{"ordf",0x00AA},
{"ordm",0x00BA},
{"oslash",0x00F8},
{"otilde",0x00F5},
{"otimes",0x2297},
{"ouml",0x00F6},
{"para",0x00B6},
{"part",0x2202},
{"permil",0x2030},
{"perp",0x22A5},
{"phi",0x03C6},
{"pi",0x03C0},
{"piv",0x03D6},
{"plusmn",0x00B1},
{"pound",0x00A3},
{"prime",0x2032},
{"prod",0x220F},
{"prop",0x221D},
{"psi",0x03C8},
{"quot",0x00B4},
{"rArr",0x21D2},
{"radic",0x221A},
{"rang",0x232A},
{"raquo",0x00BB},
{"rarr",0x2192},
{"rceil",0x2309},
{"rdquo",0x201D},
{"real",0x211C},
{"reg",0x00AE},
{"rfloor",0x230B},
{"rho",0x03C1},
{"rlm",0x200F},
{"rsaquo",0x203A},
{"rsquo",0x2019},
{"sbquo",0x201A},
{"scaron",'?'},
{"sdot",0x22C5},
{"sect",0x00A7},
{"shy",0x002D},
{"sigma",0x03C3},
{"sigmaf",0x03C2},
{"sim",0x223C},
{"spades",0x2660},
{"sub",0x2282},
{"sube",0x2286},
{"sum",0x2211},
{"sup",0x2283},
{"sup1",0x00B9},
{"sup2",0x00B2},
{"sup3",0x00B3},
{"supe",0x2287},
{"szlig",0x00DF},
{"tau",0x03C4},
{"there4",0x2234},
{"theta",0x03B8},
{"thetasym",0x03D1},
{"thinsp",0x2009},
{"thorn",0x00DE},
{"tilde",0x007E},
{"times",0x00D7},
{"trade",0x2122},
{"uArr",0x21D1},
{"uacute",0x00FA},
{"uarr",0x2191},
{"ucirc",0x00FB},
{"ugrave",0x00F9},
{"uml",0x00A8},
{"upsih",0x03D2},
{"upsilon",0x03C5},
{"uuml",0x00FC},
{"weierp",0x2118},
{"xi",0x03BE},
{"yacute",0x00FD},
{"yen",0x00A5},
{"yuml",0x00FF},
{"zeta",0x03B6},
{"zwj",0x200D},
{"zwnj",0x200C}
};



//------------------------------------------------------------------------------
//
// class GHTMLConverter
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GHTMLConverter::GHTMLConverter(GFilter* filter,RString name,GDocXML* xmlstruct,const RString& encoding) throw(std::bad_alloc,RIOException)
 : RXMLFile(name,xmlstruct,encoding), Filter(filter),Doc(xmlstruct),Tags(200,10), FoundClosingHTML(false),Base(""),ParTag(0)
{
	InitValidTags();
}


//------------------------------------------------------------------------------
GHTMLConverter::GHTMLConverter(GFilter* filter,RString name,GDocXML& xmlstruct,const RString& encoding) throw(std::bad_alloc,RIOException)
 : RXMLFile(name,xmlstruct,encoding), Filter(filter),Doc(&xmlstruct), Tags(200,10), FoundClosingHTML(false),Base(""),ParTag(0)
{
	InitValidTags();
}


//------------------------------------------------------------------------------
void GHTMLConverter::SetDocType(const RString& docType) throw(RIOException)
{
	RString name;

	name=docType.ToLower();
	if((name=="xhtml")||(name=="html"))
		RXMLFile::SetDocType("html");
	else
		throw RIOException(this,"DOCTYPE of a HTML file must be either HTML or XHTML");
}


//------------------------------------------------------------------------------
void GHTMLConverter::InitValidTags(void) throw(std::bad_alloc)
{
	//Tags for Meta Datas
	Tags.InsertPtr(new Tag("head","","docxml:metaData",false));
	Tags.InsertPtr(new Tag("meta","","",true));
	Tags.InsertPtr(new Tag("title","dc:title","",false));

	//Tags for Content
	Tags.InsertPtr(new Tag("body","","docxml:content",false));
	Tags.InsertPtr(new Tag("h1","docxml:h1","",false));
	Tags.InsertPtr(new Tag("h2","docxml:h2","",false));
	Tags.InsertPtr(new Tag("h3","docxml:h3","",false));
	Tags.InsertPtr(new Tag("h4","docxml:h4","",false));
	Tags.InsertPtr(new Tag("h5","docxml:h5","",false));
	Tags.InsertPtr(new Tag("h6","docxml:h6","",false));
	Tags.InsertPtr(new Tag("p","docxml:p","",false));
	Tags.InsertPtr(new Tag("td","docxml:p","",false));
	Tags.InsertPtr(new Tag("li","docxml:p","",false));
	Tags.InsertPtr(new Tag("div","docxml:p","",false));
	Tags.InsertPtr(new Tag("script","","",false));

	//Tags for Links
	Tags.InsertPtr(new Tag("a","docxml:p","",false));
}


//------------------------------------------------------------------------------
void GHTMLConverter::BeginTag(const RString& namespaceURI, const RString& lName, const RString& name,RContainer<RXMLAttr,true,true>& attrs) throw(RIOException)
{
	RString htmlName;
	Tag* tag;
	// if HTML closing tag found -> Nothing to do
	if(FoundClosingHTML)
		return;

	// HTML is not case sensitive
	htmlName=name.ToLower();
	//Test if it is a link
	if(!htmlName.Compare("a"))
	{
		InsertLink(attrs);
		return;
	}

	if(!htmlName.Compare("base"))
	{
		StoreBase(attrs);
		return;
	}

	if(!htmlName.Compare("meta"))
	{
		InsertMetaData(attrs);
		return;
	}


	// Valid HTML Tag?
	tag=Tags.GetPtr<const RString>(htmlName);
	if(!tag)
		return;

	//Check for docxml parent
	if(!tag->XMLParent.IsEmpty())
	{
		CurTag=Doc->GetTag(tag->XMLParent);
	}
	else
	{
		//If Tag to insert is the same that the current one close the  change parent
		if((CurTag)&&(!CurTag->GetName().Compare(tag->XMLName)))
		{
			CurTag=CurTag->GetParent();
		}
		//drop attributes
		attrs.Clear();
		// Treat the beginning tag
		RXMLFile::BeginTag(namespaceURI,lName,tag->XMLName,attrs);
	}

	//Skip "SCRIPT" tag content
	if(!htmlName.Compare("script"))
	{
		SkipTagContent(htmlName);
		CurTagClosing=true;
	}

	// Verify that is not a single tag in HTML
	if(tag->Single)
		CurTagClosing=true;
}


//------------------------------------------------------------------------------
void GHTMLConverter::EndTag(const RString& namespaceURI, const RString& lName, const RString& name) throw(RIOException)
{
	RString htmlName;
	Tag* tag;

	// if HTML closing tag found -> Nothing to do
	if(htmlName=="html")
		FoundClosingHTML=true;
		//RXMLFile::EndTag(namespaceURI,lName,tag->XMLName);

	if(FoundClosingHTML)
		return;

	// A Current tag must exist
	if(!CurTag)
		throw RIOException(this,"No current tag");

	// HTML is not case sensitive
	htmlName=name.ToLower();

	// Valid HTML Tag?
	tag=Tags.GetPtr<const RString>(htmlName);
	if(!tag)
	{
		return;

	}

	if(htmlName=="html")
		FoundClosingHTML=true;

	// Treat the end tag if needed
	if((tag->XMLParent.IsEmpty())&&(tag->Name.Compare("a"))&&(!CurTag->GetName().Compare(tag->XMLName)))
		RXMLFile::EndTag(namespaceURI,lName,tag->XMLName);
}


//------------------------------------------------------------------------------
void GHTMLConverter::Text(const RString& text) throw(RIOException)
{
	// if HTML closing tag found -> Nothing to do
	if(FoundClosingHTML)
		return;

	// A Current tag must exist
	if(!CurTag)
		throw RIOException(this,"No current tag");

	//Test if parent of text is "docxml:content"
	if(!CurTag->GetName().Compare("docxml:content"))
	{
		//If No paragraph opened -> create it
		if(!ParTag)
			Doc->AddTag(CurTag,ParTag=new RXMLTag(Doc,"docxml:p"));

		Filter->AnalyzeBlock(text,ParTag);
	}
	else
	{
		//Set ParTag to Null
		ParTag=0;
		Filter->AnalyzeBlock(text,CurTag);
	}
}

//------------------------------------------------------------------------------
void GHTMLConverter::SkipTagContent(const RString& tag)throw(RIOException)
{
	RString tagToFind("</");

	tagToFind+=tag;
	tagToFind+=">";
	while((!Cur.IsNull()&&(!CurString(tagToFind,false))))
	{
		Next();
	}

	//The End Tag is found ->skip it ( /tag> )
	unsigned int skiplen=tag.GetLen()+2;
	while((!Cur.IsNull())&&(skiplen--))
	{
		Next();
	}
}


//------------------------------------------------------------------------------
void GHTMLConverter::InsertLink(RContainer<RXMLAttr,true,true>& attrs) throw(RIOException)
{
	RXMLTag* link;
	R::RCursor<RXMLAttr> xmlAttrCur;

	link=0;
	//If no attributes -> not a correct link
	if(!attrs.GetNb())
		return;

	//Parse all attributes and insert their value
	xmlAttrCur.Set(attrs);
	for(xmlAttrCur.Start();!xmlAttrCur.End();xmlAttrCur.Next())
	{
		if(!xmlAttrCur()->GetName().ToLower().Compare("href"))
		{
			RString urlG;
			RString url=xmlAttrCur()->GetValue();

			//Add Base to url and skip anchor
			ConstructURL(urlG,url);
			//If both url are the same -> point to this page ->skip ((Name =name of the read file)
			if((urlG.IsEmpty())||(!urlG.Compare(Name)))
				return;

			//Create a node for the current link if not exist
			if(!link)
				link=Doc->AddLink();

			//Insert link informations
			Doc->AddIdentifier(urlG,link);
		}
	}
}


//------------------------------------------------------------------------------
void GHTMLConverter::ConstructURL(RString& urlG,RString link)
{
	RString urlTmp;
	RString base;
	int id;

	urlG="";
	if(link.IsEmpty())
		return;

	//Drop the anchor if url contains one
	int idAnchor=link.Find('#',-1);
	if(idAnchor>=0)
		link=link.Mid(0,idAnchor);

	//If url is just an anchor -> point to this doc -> skip
	if(!link.GetLen())
		return;

	// if complete url -> no change
	if(link.FindStr("http://")==0)
	{
		urlG=link;
		return;
	}
	if (link.FindStr("ftp://")==0)
	{
		urlG=link;
		return;
	}

	//Find Base for the current link
	if(Base.IsEmpty())
	{
		//The name of the file read ->to get base url
		urlTmp = Doc->GetURL();
		id= urlTmp.FindStr("/",-1)+1;
		base=urlTmp.Mid(0,id);
	}
	else
	{
		base=Base;
	}

	// if url begins with a './' or '../' -->change the base url
	while(link.Find('.')==0)
	{
		if(link.FindStr("/")==1)
		{
			link=link.Mid(2);
		}
		else if(link.FindStr("./")==1)
		{
			link=link.Mid(3);
			id=base.Find('/',-1);
			base=base.Mid(0,id);
		}
		else
		{
			//skip '.'
			link=link.Mid(1);
		}
	}

	//Construct the complete url
	urlG=base;
	urlG+="/";
	urlG+=link;
}


//------------------------------------------------------------------------------
void GHTMLConverter::StoreBase(RContainer<RXMLAttr,true,true>& attrs)
{
	R::RCursor<RXMLAttr> xmlAttrCur;

	xmlAttrCur.Set(attrs);
	for(xmlAttrCur.Start();!xmlAttrCur.End();xmlAttrCur.Next())
	{
		if(!xmlAttrCur()->GetName().Compare("href"))
		{
			Base=xmlAttrCur()->GetValue();
		}
	}
}


//------------------------------------------------------------------------------
void GHTMLConverter::InsertMetaData(RContainer<RXMLAttr,true,true>& attrs)
{
	R::RCursor<RXMLAttr> xmlAttrCur;
	RXMLTag *metaTag;
	RXMLTag* oldCurTag;
	RString type;
	RString content;

	//Remember CurTag
	oldCurTag=CurTag;
	//Get metaData tag
	metaTag=Doc->GetMetaData();

	//HTML is not case sensitive
	xmlAttrCur.Set(attrs);
	for(xmlAttrCur.Start();!xmlAttrCur.End();xmlAttrCur.Next())
	{
		if(!xmlAttrCur()->GetName().Compare("name"))
		{
			type=xmlAttrCur()->GetValue().ToLower();
		}
		if(!xmlAttrCur()->GetName().Compare("http-equiv"))
		{
			type=xmlAttrCur()->GetValue().ToLower();
		}
		if(!xmlAttrCur()->GetName().Compare("content"))
		{
			content=xmlAttrCur()->GetValue().ToLower();
		}
	}


	if((!type.IsEmpty())&&(!content.IsEmpty()))
	{
		//Test differents value of 'NAME' attribute
		if(!type.Compare("author"))
		{
			Doc->AddCreator(content);
		}
		if(!type.Compare("description"))
		{
			Filter->AnalyzeBlock(content,Doc->AddDescription());
		}
		if(!type.Compare("keywords"))
		{
			Filter->AnalyzeKeywords(content,',',Doc->AddSubject());
		}
		if(!type.Compare("title"))
		{
			Doc->AddTitle(content);
		}
		if(!type.Compare("generator"))
		{
			Doc->AddContributor(content);
		}
		if(!type.Compare("copyright"))
		{
			Doc->AddRights(content);
		}
		//Test different values for 'HTTP-EQUIV' attribute
		//if content-type -> Content="text/html charset=..."
		if(!type.Compare("content-type"))
		{
			//Look for charset
			int id=content.FindStr("charset=")+8;
			int len=0;
			if(id>7)
			{	//charset found ->find encoding
				RCharCursor Cur(content);
				bool end=false;
				for(Cur.GoTo(id);!Cur.End(),!end;Cur.Next())
				{
					if((Cur()==' ')||(Cur()==';')||(Cur()=='"'))
						end=true;
					len++;
				}
				content=content.Mid(id,len);
				Doc->SetEncoding(content);
			}
		}
		//if refresh ->Content="10;URL='http://....'""
		if(!type.Compare("refresh"))
		{
			//find URL
			int id=content.FindStr("url='")+5;
			RString url=content.Mid(id,content.Find('\'',id)-id);
			RString urlG;

			ConstructURL(urlG,url);
			if(!urlG.IsEmpty())
				Doc->AddIdentifier(urlG,Doc->AddLink());
		}
	}
 	CurTag=oldCurTag;
}


//------------------------------------------------------------------------------
RChar GHTMLConverter::CodeToChar(RString& str)
{
	RChar res;
	//Get CodeToChar from RXMLFile
	res=RXMLFile::CodeToChar(str);
	if(!res.IsNull())
		return res;

	// Else try to get char from RCarCode
	unsigned int NbMin,NbMax,i=0;
	int Comp=0;
	bool Cont=true,NotLast=true;
	const RCharCode* ptr;

	NbMax=sizeof(FromXML)/sizeof(RCharCode)-1;
	NbMin=0;
	while(Cont)
	{
		i=(NbMax+NbMin)/2;
		ptr=&FromXML[i];
		Comp=ptr->Code.Compare(str);
		if(!Comp)
		{
			return(ptr->Car);
		}
		if(Comp>0)
		{
			NbMax = i;
			if(i) NbMax--;
		}
		else
			NbMin = i+1;
		Cont = NotLast;
		if(NbMin>=NbMax) NotLast=false;
	}
	return(0);
}


//------------------------------------------------------------------------------
bool GHTMLConverter::OnlyQuote(void)
{
	return(false);
}


//------------------------------------------------------------------------------
bool GHTMLConverter::InvalidXMLCodeAccept(void)
{
	return(true);
}


//------------------------------------------------------------------------------
GHTMLConverter::~GHTMLConverter(void)
{
}
