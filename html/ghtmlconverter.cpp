/*

	GALILEI Research Project

	GHTMLConverter.cpp

	HTML file Converter to DocXML - Implementation.

	Copyright 2004 by the Universit�Libre de Bruxelles.

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



//------------------------------------------------------------------------------
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
const RCharCode FromXML[]={
{"#13",'\n'},
{"#32",' '},
{"#33",'!'},
{"#34",'"'},
{"#35",'#'},
{"#36",'$'},
{"#37",'%'},
{"#38",'&'},
{"#39",'\''},
{"#40",'('},
{"#41",')'},
{"#42",'*'},
{"#43",'+'},
{"#44",','},
{"#45",'-'},
{"#46",'.'},
{"#47",'/'},
{"#58",','},
{"#59",';'},
{"#60",'<'},
{"#61",'='},
{"#62",'>'},
{"#63",'?'},
{"#64",'@'},
{"#91",'['},
{"#92",'\\'},
{"#93",']'},
{"#94",'^'},
{"#95",'_'},
{"#96",'`'},
{"#123",'{'},
{"#124",'|'},
{"#125",'}'},
{"#126",'~'},
{"#128",'?'},
{"#130",','},
{"#131",'?'},
{"#132",'"'},
{"#133",'?'},
{"#134",'?'},
{"#135",'?'},
{"#136",'?'},
{"#137",'?'},
{"#138",'?'},
{"#139",'<'},
{"#140",'?'},
{"#142",'?'},
{"#145",'\''},
{"#146",'\''},
{"#147",'"'},
{"#148",'"'},
{"#149",'*'},
{"#150",'-'},
{"#151",'-'},
{"#152",'~'},
{"#153",'?'},
{"#154",'?'},
{"#155",'>'},
{"#156",'?'},
{"#158",'?'},
{"#159",'?'},
{"#160",' '},
{"#161",'¡'},
{"#162",'¢'},
{"#163",'£'},
{"#164",'¤'},
{"#165",'¥'},
{"#166",'¦'},
{"#167",'§'},
{"#168",'¨'},
{"#169",'©'},
{"#170",'ª'},
{"#171",'«'},
{"#172",'¬'},
{"#173",'­'},
{"#174",'®'},
{"#175",'¯'},
{"#176",'°'},
{"#177",'±'},
{"#178",'²'},
{"#179",'³'},
{"#180",'´'},
{"#181",'µ'},
{"#182",'¶'},
{"#183",'·'},
{"#184",'¸'},
{"#185",'¹'},
{"#186",'º'},
{"#187",'»'},
{"#188",'¼'},
{"#189",'½'},
{"#190",'¾'},
{"#191",'¿'},
{"#192",'À'},
{"#193",'Á'},
{"#194",'Â'},
{"#195",'Ã'},
{"#196",'Ä'},
{"#197",'Å'},
{"#198",'Æ'},
{"#199",'Ç'},
{"#200",'È'},
{"#201",'É'},
{"#202",'Ê'},
{"#203",'Ë'},
{"#204",'Ì'},
{"#205",'Í'},
{"#206",'Î'},
{"#207",'Ï'},
{"#208",'Ð'},
{"#209",'Ñ'},
{"#210",'Ò'},
{"#211",'Ó'},
{"#212",'Ô'},
{"#213",'Õ'},
{"#214",'Ö'},
{"#215",'×'},
{"#216",'Ø'},
{"#217",'Ù'},
{"#218",'Ú'},
{"#219",'Û'},
{"#220",'Ü'},
{"#221",'Ý'},
{"#222",'Þ'},
{"#223",'ß'},
{"#224",'à'},
{"#225",'á'},
{"#226",'â'},
{"#227",'ã'},
{"#228",'ä'},
{"#229",'å'},
{"#230",'æ'},
{"#231",'ç'},
{"#232",'è'},
{"#233",'é'},
{"#234",'ê'},
{"#235",'ë'},
{"#236",'ì'},
{"#237",'í'},
{"#238",'î'},
{"#239",'ï'},
{"#240",'ð'},
{"#241",'ñ'},
{"#242",'ò'},
{"#243",'ó'},
{"#244",'ô'},
{"#245",'õ'},
{"#246",'ö'},
{"#247",'÷'},
{"#248",'ø'},
{"#249",'ù'},
{"#250",'ú'},
{"#251",'û'},
{"#252",'ü'},
{"#253",'ý'},
{"#254",'þ'},
{"#255",'ÿ'},
{"#338",'?'},
{"#339",'?'},
{"#352",'?'},
{"#353",'?'},
{"#376",'?'},
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
{"AElig",'Æ'},
{"Aacute",'Á'},
{"Acirc",'Â'},
{"Agrave",'À'},
{"Alpha",0x0391},
{"Aring",'Å'},
{"Atilde",'Ã'},
{"Auml",'Ä'},
{"Beta",0x0392},
{"Ccedil",'Ç'},
{"Chi",0x03A7},
{"Dagger",0x2021},
{"Delta",0x0394},
{"ETH",'Ð'},
{"Eacute",'É'},
{"Ecirc",'Ê'},
{"Egrave",'È'},
{"Epsilon",0x0395},
{"Eta",0x0397},
{"Euml",'Ë'},
{"Gamma",0x0393},
{"Iacute",'Í'},
{"Icirc",'Î'},
{"Igrave",'Ì'},
{"Iota",0x0399},
{"Iuml",'Ï'},
{"Kappa",0x039A},
{"Lambda",0x039B},
{"Mu",0x039C},
{"Ntilde",'Ñ'},
{"Nu",0x039D},
{"OElig",'?'},
{"Oacute",'Ó'},
{"Ocirc",'Ô'},
{"Ograve",'Ò'},
{"Omega",0x03A9},
{"Omicron",0x039F},
{"Oslash",'Ø'},
{"Otilde",'Õ'},
{"Ouml",'Ö'},
{"Phi",0x03A6},
{"Pi",0x03A0},
{"Prime",0x2033},
{"Psi",0x03A8},
{"Rho",0x03A1},
{"Scaron",'?'},
{"Sigma",0x03A3},
{"THORN",'Þ'},
{"Tau",0x03A4},
{"Theta",0x0398},
{"Uacute",'Ú'},
{"Ucirc",'Û'},
{"Ugrave",'Ù'},
{"Upsilon",0x03A5},
{"Uuml",'Ü'},
{"Xi",0x039E},
{"Yacute",'Ý'},
{"Yuml",'?'},
{"Zeta",0x0396},
{"aacute",'á'},
{"acirc",'â'},
{"acute",'´'},
{"aelig",'æ'},
{"agrave",'à'},
{"alefsym",0x2135},
{"alpha",0x03B1},
{"amp",'&'},
{"and",0x2227},
{"ang",0x2220},
{"aring",'å'},
{"asymp",0x2248},
{"atilde",'ã'},
{"auml",'ä'},
{"bdquo",0x201E},
{"beta",0x03B2},
{"brvbar",'¦'},
{"bull",0x2022},
{"cap",0x2229},
{"ccedil",'ç'},
{"cedil",'¸'},
{"cent",'¢'},
{"chi",0x03C7},
{"circ",0x02C6},
{"clubs",0x2663},
{"cong",0x2245},
{"copy",'©'},
{"crarr",0x21B5},
{"cup",0x222A},
{"curren",'¤'},
{"dArr",0x21D3},
{"dagger",0x2020},
{"darr",0x2193},
{"deg",'°'},
{"delta",0x03B4},
{"diams",0x2666},
{"divide",'÷'},
{"eacute",'é'},
{"ecirc",'ê'},
{"egrave",'è'},
{"empty",0x2205},
{"emsp",0x2003},
{"ensp",0x2002},
{"epsilon",0x03B5},
{"equiv",0x2261},
{"eta",0x03B7},
{"eth",'ð'},
{"euml",'ë'},
{"euro",0x20AC},
{"exist",0x2203},
{"forall",0x2200},
{"frac12",'½'},
{"frac14",'¼'},
{"frac34",'¾'},
{"frasl",'/'},
{"gamma",0x03B3},
{"ge",0x2265},
{"gt",'>'},
{"hArr",0x21D4},
{"harr",0x2194},
{"hearts",0x2665},
{"hellip",0x2026},
{"iacute",'í'},
{"icirc",'î'},
{"iexcl",'¡'},
{"igrave",'ì'},
{"image",0x2111},
{"infin",0x221E},
{"int",0x222B},
{"iota",0x03B9},
{"iquest",'¿'},
{"isin",0x2208},
{"iuml",'ï'},
{"kappa",0x03BA},
{"lArr",0x21D0},
{"lang",0x2329},
{"lambda",0x03BB},
{"laquo",'«'},
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
{"lt",'<'},
{"macr",'¯'},
{"mdash",0x2014},
{"micro",'µ'},
{"middot",'·'},
{"minus",0x2212},
{"mu",0x03BC},
{"nabla",0x2207},
{"nbsp",' '},
{"ndash",0x2013},
{"ne",0x2260},
{"ni",0x220B},
{"not",'¬'},
{"notin",0x2209},
{"nsub",0x2284},
{"ntilde",'ñ'},
{"nu",0x03BD},
{"oacute",'ó'},
{"ocirc",'ô'},
{"oelig",'?'},
{"ograve",'ò'},
{"oline",0x203E},
{"omega",0x03C9},
{"omicron",0x03BF},
{"oplus",0x2295},
{"or",0x2228},
{"ordf",'ª'},
{"ordm",'º'},
{"oslash",'ø'},
{"otilde",'õ'},
{"otimes",0x2297},
{"ouml",'ö'},
{"para",'¶'},
{"part",0x2202},
{"permil",0x2030},
{"perp",0x22A5},
{"phi",0x03C6},
{"pi",0x03C0},
{"piv",0x03D6},
{"plusmn",'±'},
{"pound",'£'},
{"prime",0x2032},
{"prod",0x220F},
{"prop",0x221D},
{"psi",0x03C8},
{"quot",'¨'},
{"rArr",0x21D2},
{"radic",0x221A},
{"rang",0x232A},
{"raquo",'»'},
{"rarr",0x2192},
{"rceil",0x2309},
{"rdquo",0x201D},
{"real",0x211C},
{"reg",'®'},
{"rfloor",0x230B},
{"rho",0x03C1},
{"rlm",0x200F},
{"rsaquo",0x203A},
{"rsquo",0x2019},
{"sbquo",0x201A},
{"scaron",'?'},
{"sdot",0x22C5},
{"sect",'§'},
{"shy",'­'},
{"sigma",0x03C3},
{"sigmaf",0x03C2},
{"sim",0x223C},
{"spades",0x2660},
{"sub",0x2282},
{"sube",0x2286},
{"sum",0x2211},
{"sup",0x2283},
{"sup1",'¹'},
{"sup2",'²'},
{"sup3",'³'},
{"supe",0x2287},
{"szlig",'ß'},
{"tau",0x03C4},
{"there4",0x2234},
{"theta",0x03B8},
{"thetasym",0x03D1},
{"thinsp",0x2009},
{"thorn",'þ'},
{"tilde",'~'},
{"times",'×'},
{"trade",0x2122},
{"uArr",0x21D1},
{"uacute",'ú'},
{"uarr",0x2191},
{"ucirc",'û'},
{"ugrave",'ù'},
{"uml",'¨'},
{"upsih",0x03D2},
{"upsilon",0x03C5},
{"uuml",'ü'},
{"weierp",0x2118},
{"xi",0x03BE},
{"yacute",'ý'},
{"yen",'¥'},
{"yuml",'ÿ'},
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
 : RXMLFile(name,xmlstruct,R::Read,encoding), Filter(filter),Doc(xmlstruct),Tags(200,10), FoundClosingHTML(false),Base(""),ParTag(0)
{
	InitValidTags();
}


//------------------------------------------------------------------------------
GHTMLConverter::GHTMLConverter(GFilter* filter,RString name,GDocXML& xmlstruct,const RString& encoding) throw(std::bad_alloc,RIOException)
 : RXMLFile(name,xmlstruct,R::Read,encoding), Filter(filter),Doc(&xmlstruct), Tags(200,10), FoundClosingHTML(false),Base(""),ParTag(0)
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
			Doc->AddTag(CurTag,ParTag=new RXMLTag("docxml:p"));

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
		if(!xmlAttrCur()->GetName().Compare("href"))
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
		id= urlTmp.FindStr("/",-1);
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
