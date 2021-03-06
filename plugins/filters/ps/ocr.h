const char *ocr[] = {
  "% Copyright (C) 1995, Digital Equipment Corporation.\n",
  "% All rights reserved.\n",
  "% See the file pstotext.txt for a full description.\n",
  "\n",
  "% ocr.ps, part of BuildLectern\n",
  "%\n",
  "%   This is a PostScript library to send the characters rendered by a\n",
  "%   PostScript job back on stdout.  The output is intended to allow\n",
  "%   reconstruction of the document's words and an approximation of the words'\n",
  "%   bounding rectangles.\n",
  "\n",
  "% Last modified on Sat Feb  5 21:00:00 AEST 2000 by rjl\n",
  "%      modified on Fri Oct  2 17:13:53 PDT 1998 by mcjones\n",
  "%      modified on Thu Jan 25 15:24:37 PST 1996 by deutsch\n",
  "%      modified on Wed May  3 15:41:30 PDT 1995 by birrell\n",
  "\n",
  "% Restrictions:\n",
  "%\n",
  "%   This library redefines some names that are originally defined as\n",
  "%   operators, and of course the new definitions are procedures. Some\n",
  "%   jobs might be sensitive to this distinction.  In this case, you'd\n",
  "%   need to make \"redef\" create a new operator.  For example, \"odef\".\n",
  "%\n",
  "%   This library reports characters rendered by show, ashow, showwidth,\n",
  "%   ashowwidth and kshow.  The characters are reported even if they would\n",
  "%   be invisible in final hardcopy.  This could happen, for example, if the\n",
  "%   characters get clipped, or if they get overprinted, or if they are the\n",
  "%   same color as their background, or if the font's glyph make no marks.\n",
  "%\n",
  "%   PostScript doesn't specify a standard character code for the characters\n",
  "%   being rendered; rather, the font maps small integers into glyphs.  It's\n",
  "%   not possible in general to determine a translation back into a standard\n",
  "%   character code.  This library approximates this by giving mappings from\n",
  "%   the job's characters to indexes into a table of known glyph names.  This\n",
  "%   handles most PostScript jobs that we've encountered, but it's not an\n",
  "%   absolute solution to the problem.\n",
  "\n",
  "% Output format:\n",
  "%\n",
  "%    The \"reporting coordinate system\" is the device coordinate system.  The\n",
  "%    application interpreting this library's output needs to understand the\n",
  "%    device coordinate system.\n",
  "%\n",
  "%    Positions are reported in the reporting coordinate system, but with (x,y) \n",
  "%    values multiplied by 100 and rounded to integers.  Note that\n",
  "%    positions might involve negative integers.\n",
  "%\n",
  "%    The directives, whitespace and numbers are themselves encoded in ASCII.\n",
  "%    The strings in the \"S\" directives are in currentfont's encoding: they\n",
  "%    should be treated as 8-bit binary data.\n",
  "%\n",
  "%    QI 6-integers\n",
  "%                       Specifies the inverse of the currentmatrix for\n",
  "%                       the current output device, after rotXXX.ps and\n",
  "%                       before the document is processed.\n",
  "%\n",
  "%    QM m blx bly trx try 256-pairs\n",
  "%			Introduces a new character metrics table used by\n",
  "%			some font, where \"m\" is an integer that identifies\n",
  "%			the metrics table in subsequent \"F\" directives.  \"m\"\n",
  "%			is small.  (blx,bly) is the bottom left corner of the\n",
  "%			font's bounding box, and (trx,try) is its top right. \n",
  "%			The pairs are the stringwidth of each character in the\n",
  "%			font's encoding.  All of these are reported in the\n",
  "%			font's character coordinate system, multiplied by 100\n",
  "%			and rounded to integers.  The metrics\n",
  "%			table will be referenced in a subsequent \"F\"\n",
  "%			directive, which includes information mapping the\n",
  "%			font's metrics to the initial user coordinate system. \n",
  "%			Note that values of \"m\" might get reused, after a\n",
  "%			\"restore\".\n",
  "%\n",
  "% QE e n n-integers	Introduces a new encoding, where \"e\" is an integer\n",
  "%			that identifies the encoding in subsequent \"F\"\n",
  "%			directives.  \"e\" is small.  \"e\" is followed by\n",
  "%			an integer \"n\" and then by a sequence of\n",
  "%			exactly n integers.  The i'th integer\n",
  "%			specifies the glyph for the i'th entry of the\n",
  "%			font's encoding vector.  The value of the\n",
  "%			integer is usually an index in the array\n",
  "%			\"StandardGlyphs\", defined below.  Note that\n",
  "%			the first 256 entries of that array equal the\n",
  "%			corresponding entries of ISOLatin1Encoding.\n",
  "%			The value 9999 indicates that this entry in\n",
  "%			the font's encoding specifies a glyph not\n",
  "%			named in StandardGlyphs.  Note that values of\n",
  "%			\"e\" might get reused, after a \"restore\".\n",
  "%\n",
  "%    QF n x y x' y' e m	Introduces a new font, where \"n\" is an integer\n",
  "%			that identifies the font in subsequent \"S\"\n",
  "%			directives.  \"n\" is small.  (x,y) is the\n",
  "%			position corresponding to (1000,0) in the font's\n",
  "%			character coordinate system, and (x',y') is the\n",
  "%			position corresponding to (0,1000), both as they would\n",
  "%			be if the character were drawn with its origin at\n",
  "%			the origin of the reporting coordinate system. \"e\" is\n",
  "%			an integer specifying a previously defined\n",
  "%			encoding vector.  \"m\" is an integer specifying a\n",
  "%			previously defined metrics table.  Note that values\n",
  "%			of \"n\" might get reused, after a \"restore\".\n",
  "%\n",
  "%    QS n x y l s x' y'	Reports rendering of string \"s\" in font \"n\".\n",
  "%			(x,y) is the position corresponding to the origin of\n",
  "%			the first character. \"l\" is length of the string,\n",
  "%			followed immediately by a single space then the string.\n",
  "%			(x',y') is the position that would correspond to the\n",
  "%			origin of a subsequent character. The\n",
  "%			string reported by this directive is never empty.\n",
  "%			The string also never contains a \"space\": strings\n",
  "%			that would have contained a \"space\" are split up\n",
  "%			into multiple directives, with the \"space\"\n",
  "%			omitted.  Here \"space\" means the first character\n",
  "%			in the font's encoding that maps to the glyph\n",
  "%			named \"/space\", if there is such a character.\n",
  "%\n",
  "%    QC			copypage was invoked\n",
  "%\n",
  "%    QZ			erasepage was invoked\n",
  "%\n",
  "%    QP			showpage was invoked\n",
  "\n",
  "%\n",
  "% globals and subroutines\n",
  "%\n",
  "\n",
  "%/setglobal where\n",
  "% { pop currentglobal /setglobal load true setglobal }\n",
  "% { { } }\n",
  "%ifelse\n",
  "revision 353 ge {\n",
  "NOBIND /DELAYBIND where { pop DELAYBIND or } if\n",
  " { systemdict begin\n",
  "                /bind /.bind load\n",
  "                  /.forcedef where { pop .forcedef } { def } ifelse\n",
  "              end\n",
  " }\n",
  "if\n",
  "} if\n",
  "\n",
  "% put our private stuff in a local dictionary,\n",
  "% but place a reference to it in systemdict\n",
  "systemdict begin\n",
  "/pstotextLocalDict 30 dict /.forcedef where { pop .forcedef } { def } ifelse\n",
  "end\n",
  "\n",
  "pstotextLocalDict begin		% following stuff is private\n",
  "/redef { systemdict begin 1 index exch .makeoperator def end } bind def\n",
  "/privateDict currentdict def	% for lastFontNum and lastEncoding\n",
  "/fonts 200 dict def		% maps font to integer \"n\"\n",
  "/fontsUnit 200 dict def 	% (1000,0) and (0,1000) transformed, per font\n",
  "/encodings 200 dict def		% maps encoding array to integer \"e\"\n",
  "/encodingSpace 200 dict def	% space char for each encoding\n",
  "/metrics 200 dict def		% maps font UniqueID to integer  \"m\"\n",
  "/lastFontNum 0 def		% last integer used for a font\n",
  "/lastEncoding 0 def		% last integer used for an encoding\n",
  "/lastMetrics 0 def		% last integer used for a metrics table\n",
  "/tempString 20 string def	% scratch for printing integers\n",
  "/reportMatrix matrix identmatrix def % maps device coords to reporting coords\n",
  "/inUse false def		% prevents recursive invokcation of \"report\"\n",
  "\n",
  "/TimesRomanGlyphs [\n",
  "  % ISOLatin1Encoding ...\n",
  "  /.notdef /.notdef /.notdef /.notdef /.notdef\n",
  "  /.notdef /.notdef /.notdef /.notdef /.notdef\n",
  "  /.notdef /.notdef /.notdef /.notdef /.notdef\n",
  "  /.notdef /.notdef /.notdef /.notdef /.notdef\n",
  "  /.notdef /.notdef /.notdef /.notdef /.notdef\n",
  "  /.notdef /.notdef /.notdef /.notdef /.notdef\n",
  "  /.notdef /.notdef /space /exclam /quotedbl\n",
  "  /numbersign /dollar /percent /ampersand /quoteright\n",
  "  /parenleft /parenright /asterisk /plus /comma\n",
  "  /minus /period /slash /zero /one\n",
  "  % 50\n",
  "  /two /three /four /five /six\n",
  "  /seven /eight /nine /colon /semicolon\n",
  "  /less /equal /greater /question /at\n",
  "  /A /B /C /D /E\n",
  "  /F /G /H /I /J\n",
  "  /K /L /M /N /O\n",
  "  /P /Q /R /S /T\n",
  "  /U /V /W /X /Y\n",
  "  /Z /bracketleft /backslash /bracketright /asciicircum\n",
  "  /underscore /quoteleft /a /b /c\n",
  "  % 100\n",
  "  /d /e /f /g /h\n",
  "  /i /j /k /l /m\n",
  "  /n /o /p /q /r\n",
  "  /s /t /u /v /w\n",
  "  /x /y /z /braceleft /bar\n",
  "  /braceright /asciitilde /.notdef /.notdef /.notdef\n",
  "  /.notdef /.notdef /.notdef /.notdef /.notdef\n",
  "  /.notdef /.notdef /.notdef /.notdef /.notdef\n",
  "  /.notdef /.notdef /.notdef /.notdef /dotlessi\n",
  "  /grave /acute /circumflex /tilde /macron\n",
  "  % 150\n",
  "  /breve /dotaccent /dieresis /.notdef /ring\n",
  "  /cedilla /.notdef /hungarumlaut /ogonek /caron\n",
  "  /space /exclamdown /cent /sterling /currency\n",
  "  /yen /brokenbar /section /dieresis /copyright\n",
  "  /ordfeminine /guillemotleft /logicalnot /hyphen /registered\n",
  "  /macron /degree /plusminus /twosuperior /threesuperior\n",
  "  /acute /mu /paragraph /periodcentered /cedilla\n",
  "  /onesuperior /ordmasculine /guillemotright /onequarter /onehalf\n",
  "  /threequarters /questiondown /Agrave /Aacute /Acircumflex\n",
  "  /Atilde /Adieresis /Aring /AE /Ccedilla\n",
  "  % 200\n",
  "  /Egrave /Eacute /Ecircumflex /Edieresis /Igrave\n",
  "  /Iacute /Icircumflex /Idieresis /Eth /Ntilde\n",
  "  /Ograve /Oacute /Ocircumflex /Otilde /Odieresis\n",
  "  /multiply /Oslash /Ugrave /Uacute /Ucircumflex\n",
  "  /Udieresis /Yacute /Thorn /germandbls /agrave\n",
  "  /aacute /acircumflex /atilde /adieresis /aring\n",
  "  /ae /ccedilla /egrave /eacute /ecircumflex\n",
  "  /edieresis /igrave /iacute /icircumflex /idieresis\n",
  "  /eth /ntilde /ograve /oacute /ocircumflex\n",
  "  /otilde /odieresis /divide /oslash /ugrave\n",
  "  % 250\n",
  "  /uacute /ucircumflex /udieresis /yacute /thorn\n",
  "  /ydieresis \n",
  "  % Other glyphs from /Times-Roman ...\n",
  "  % 256\n",
  "  /quotedblright /Scaron /dagger /guilsinglleft\n",
  "  /Zcaron /daggerdbl /Lslash /ellipsis /guilsinglright\n",
  "  /oe /fi /bullet /perthousand /quotedblbase\n",
  "  /endash /emdash /trademark /florin /lslash\n",
  "  /scaron /Ydieresis /fl /fraction /quotedblleft\n",
  "  /quotesinglbase /quotesingle /zcaron /OE\n",
  "  % 284\n",
  "  ] def\n",
  "\n",
  "/dvipsGlyphs [\n",
  "  % Self-named glyphs for dvitops type 3 fonts ...\n",
  "  % 284\n",
  "  0 1 255 { 1 string dup 0 4 -1 roll put cvn } for\n",
  "  % 540\n",
  "  ] def\n",
  "\n",
  "/ttypeGlyphs1 [\n",
  "  % Glyph names used by MS TrueType encodings ...\n",
  "  % 540\n",
  "  /G00 /G01 /G02 /G03 /G04 /G05 /G06 /G07 /G08 /G09 /G0a /G0b /G0c /G0d /G0e\n",
  "  /G0f /G10 /G11 /G12 /G13 /G14 /G15 /G16 /G17 /G18 /G19 /G1a /G1b /G1c /G1d\n",
  "  /G1e /G1f /G20 /G21 /G22 /G23 /G24 /G25 /G26 /G27 /G28 /G29 /G2a /G2b /G2c\n",
  "  /G2d /G2e /G2f /G30 /G31 /G32 /G33 /G34 /G35 /G36 /G37 /G38 /G39 /G3a /G3b\n",
  "  % 600\n",
  "  /G3c /G3d /G3e /G3f /G40 /G41 /G42 /G43 /G44 /G45 /G46 /G47 /G48 /G49 /G4a\n",
  "  /G4b /G4c /G4d /G4e /G4f /G50 /G51 /G52 /G53 /G54 /G55 /G56 /G57 /G58 /G59\n",
  "  /G5a /G5b /G5c /G5d /G5e /G5f /G60 /G61 /G62 /G63 /G64 /G65 /G66 /G67 /G68\n",
  "  /G69 /G6a /G6b /G6c /G6d /G6e /G6f /G70 /G71 /G72 /G73 /G74 /G75 /G76 /G77\n",
  "  /G78 /G79 /G7a /G7b /G7c /G7d /G7e /G7f /G80 /G81 /G82 /G83 /G84 /G85 /G86\n",
  "  /G87 /G88 /G89 /G8a /G8b /G8c /G8d /G8e /G8f /G90 /G91 /G92 /G93 /G94 /G95\n",
  "  /G96 /G97 /G98 /G99 /G9a /G9b /G9c /G9d /G9e /G9f /Ga0 /Ga1 /Ga2 /Ga3 /Ga4\n",
  "  /Ga5 /Ga6 /Ga7 /Ga8 /Ga9 /Gaa /Gab /Gac /Gad /Gae /Gaf /Gb0 /Gb1 /Gb2 /Gb3\n",
  "  /Gb4 /Gb5 /Gb6 /Gb7 /Gb8 /Gb9 /Gba /Gbb /Gbc /Gbd /Gbe /Gbf /Gc0 /Gc1 /Gc2\n",
  "  /Gc3 /Gc4 /Gc5 /Gc6 /Gc7 /Gc8 /Gc9 /Gca /Gcb /Gcc /Gcd /Gce /Gcf /Gd0 /Gd1\n",
  "  % 750\n",
  "  /Gd2 /Gd3 /Gd4 /Gd5 /Gd6 /Gd7 /Gd8 /Gd9 /Gda /Gdb /Gdc /Gdd /Gde /Gdf /Ge0\n",
  "  /Ge1 /Ge2 /Ge3 /Ge4 /Ge5 /Ge6 /Ge7 /Ge8 /Ge9 /Gea /Geb /Gec /Ged /Gee /Gef\n",
  "  /Gf0 /Gf1 /Gf2 /Gf3 /Gf4 /Gf5 /Gf6 /Gf7 /Gf8 /Gf9 /Gfa /Gfb /Gfc /Gfd /Gfe\n",
  "  /Gff \n",
  "  % 796\n",
  "  ] def\n",
  "\n",
  "/ttypeGlyphs2 [\n",
  "  % 796\n",
  "  /G00 /G01 /G02 /G03 /G04 /G05 /G06 /G07 /G08 /G09 /G0A /G0B /G0C /G0D\n",
  "  /G0E /G0F /G10 /G11 /G12 /G13 /G14 /G15 /G16 /G17 /G18 /G19 /G1A /G1B /G1C\n",
  "  /G1D /G1E /G1F /G20 /G21 /G22 /G23 /G24 /G25 /G26 /G27 /G28 /G29 /G2A /G2B\n",
  "  /G2C /G2D /G2E /G2F /G30 /G31 /G32 /G33 /G34 /G35 /G36 /G37 /G38 /G39 /G3A\n",
  "  /G3B /G3C /G3D /G3E /G3F /G40 /G41 /G42 /G43 /G44 /G45 /G46 /G47 /G48 /G49\n",
  "  /G4A /G4B /G4C /G4D /G4E /G4F /G50 /G51 /G52 /G53 /G54 /G55 /G56 /G57 /G58\n",
  "  /G59 /G5A /G5B /G5C /G5D /G5E /G5F /G60 /G61 /G62 /G63 /G64 /G65 /G66 /G67\n",
  "  % 900\n",
  "  /G68 /G69 /G6A /G6B /G6C /G6D /G6E /G6F /G70 /G71 /G72 /G73 /G74 /G75 /G76\n",
  "  /G77 /G78 /G79 /G7A /G7B /G7C /G7D /G7E /G7F /G80 /G81 /G82 /G83 /G84 /G85\n",
  "  /G86 /G87 /G88 /G89 /G8A /G8B /G8C /G8D /G8E /G8F /G90 /G91 /G92 /G93 /G94\n",
  "  /G95 /G96 /G97 /G98 /G99 /G9A /G9B /G9C /G9D /G9E /G9F /GA0 /GA1 /GA2 /GA3\n",
  "  /GA4 /GA5 /GA6 /GA7 /GA8 /GA9 /GAA /GAB /GAC /GAD /GAE /GAF /GB0 /GB1 /GB2\n",
  "  /GB3 /GB4 /GB5 /GB6 /GB7 /GB8 /GB9 /GBA /GBB /GBC /GBD /GBE /GBF /GC0 /GC1\n",
  "  /GC2 /GC3 /GC4 /GC5 /GC6 /GC7 /GC8 /GC9 /GCA /GCB /GCC /GCD /GCE /GCF /GD0\n",
  "  /GD1 /GD2 /GD3 /GD4 /GD5 /GD6 /GD7 /GD8 /GD9 /GDA /GDB /GDC /GDD /GDE /GDF\n",
  "  /GE0 /GE1 /GE2 /GE3 /GE4 /GE5 /GE6 /GE7 /GE8 /GE9 /GEA /GEB /GEC /GED /GEE\n",
  "  /GEF /GF0 /GF1 /GF2 /GF3 /GF4 /GF5 /GF6 /GF7 /GF8 /GF9 /GFA /GFB /GFC /GFD\n",
  "  % 1050\n",
  "  /GFE /GFF \n",
  "  % 1052\n",
  "  ] def\n",
  "\n",
  "/oldDviGlyphs [\n",
  "  % More self-named glyphs for old dvitops type 3 fonts ...\n",
  "  % 1052\n",
  "  0 1 127 { 10 3 string cvrs cvn } for\n",
  "  % 1180\n",
  "  ] def\n",
  "\n",
  "/StandardGlyphs //TimesRomanGlyphs length 896 add array def\n",
  "//StandardGlyphs 0 //TimesRomanGlyphs putinterval\n",
  "//StandardGlyphs //TimesRomanGlyphs length //dvipsGlyphs putinterval\n",
  "//StandardGlyphs //TimesRomanGlyphs length 256 add //ttypeGlyphs1 putinterval\n",
  "//StandardGlyphs //TimesRomanGlyphs length 512 add //ttypeGlyphs2 putinterval\n",
  "//StandardGlyphs //TimesRomanGlyphs length 768 add //oldDviGlyphs putinterval\n",
  "\n",
  "/standardMap StandardGlyphs length dict\n",
  "  % Maps names to indices in StandardGlyphs.\n",
  "  0 StandardGlyphs {\n",
  "    2 index exch\n",
  "    2 copy known { pop pop } { 2 index put } ifelse\n",
  "    1 add\n",
  "    } forall\n",
  "  pop def\n",
  "\n",
  "/printInt {			% stack: n\n",
  "  % Prints an integer followed by a space on stdout\n",
  "  //tempString cvs print ( ) print\n",
  "  } bind def\n",
  "\n",
  "/showxy {			%stack:  x y\n",
  "  % prints a pair of integers on stdout, converting to 1/100th's\n",
  "  exch\n",
  "  100 mul round cvi //printInt exec\n",
  "  100 mul round cvi //printInt exec\n",
  "  } bind def\n",
  "\n",
  "/characterToReporting {		% stack: x y -> x' y'\n",
  "  % Transforms a vector in currentfont's character coordinate\n",
  "  % system to the reporting coordinate system\n",
  "  currentfont /FontMatrix get\n",
  "  dtransform			% to current user coordinates\n",
  "  dtransform			% to device coordinates\n",
  "  //reportMatrix idtransform	% to reporting coordinates\n",
  "  } def\n",
  "\n",
  "/printCharacterOrigin {\n",
  "  % Prints the position in the reporting coordinate system at which a\n",
  "  % character origin would be painted using currentfont\n",
  "  0 0 currentfont /FontMatrix get transform\n",
  "  currentpoint exch 4 -1 roll add 3 1 roll add\n",
  "  transform //reportMatrix itransform\n",
  "  //showxy exec\n",
  "  } bind def\n",
  "\n",
  "/printMap {\n",
  "  % Print map from indices in currentfont/Encoding to StandardEncoding indices\n",
  "  16 currentfont /Encoding get dup length //printInt exec {\n",
  "    exch dup 16 eq { () = pop 1 } { 1 add } ifelse exch\n",
  "    //standardMap exch\n",
  "    2 copy known {\n",
  "      get //printInt exec\n",
  "      } {\n",
  "      pop pop (9999 ) print\n",
  "      } ifelse\n",
  "    } forall\n",
  "  pop () =\n",
  "  } bind def\n",
  "\n",
  "/metricsString (X) def\n",
  "\n",
  "/printMetrics {\n",
  "  % Print bounding box and character metrics for currentfont\n",
  "  % Sadly, dvitops produces illegal type 3 fonts with no /.notdef entry. The\n",
  "  % use of \"stopped\" deals with that and any other silliness.\n",
  "  currentfont /FontBBox get aload pop 4 2 roll\n",
  "  //showxy exec\n",
  "  //showxy exec\n",
  "  currentfont /FontMatrix get\n",
  "  currentfont /FontType get 3 eq {\n",
  "    //privateDict /pm.dictCount countdictstack put\n",
  "    count //privateDict exch /pm.count exch put\n",
  "    //privateDict /pm.save save put\n",
  "    0 1 255 {\n",
  "      dup 8 mod 0 eq { () = } if\n",
  "      //metricsString exch 0 exch put\n",
  "      { //metricsString //stringwidth exec\n",
  "        2 index idtransform		% to font's character coordinates\n",
  "        } stopped {\n",
  "        count //privateDict /pm.count get sub { pop } repeat\n",
  "        countdictstack //privateDict /pm.dictCount get sub { end } repeat\n",
  "        //privateDict /pm.save get restore\n",
  "        //privateDict /pm.save save put\n",
  "        0 0\n",
  "        } if\n",
  "      //showxy exec\n",
  "      } for\n",
  "    //privateDict /pm.save get restore % necessary to balance the \"save\"\n",
  "    }{\n",
  "    currentfont /CharStrings get\n",
  "    currentfont /Encoding get\n",
  "    0 1 255 {\n",
  "      dup 8 mod 0 eq { () = } if\n",
  "      2 copy get 3 index exch known {\n",
  "        //metricsString exch 0 exch put\n",
  "        //metricsString //stringwidth exec\n",
  "        4 index idtransform		% to font's character coordinates\n",
  "        } {\n",
  "        pop 0 0\n",
  "        } ifelse\n",
  "      //showxy exec\n",
  "      } for\n",
  "      pop pop				% encoding, charstrings\n",
  "    } ifelse\n",
  "  pop				% fontmatrix\n",
  "  } bind def\n",
  "\n",
  "\n",
  "%\n",
  "% The main work: reportMetrics, reportEncoding, reportFont and report\n",
  "%\n",
  "\n",
  "/reportMetrics {		% stack: -> m\n",
  "				% privateDict is open\n",
  "  % Print new \"m\" directive for currentfont\n",
  "  (QM ) print\n",
  "  lastMetrics 1 add /lastMetrics 1 index def\n",
  "  dup //printInt exec\n",
  "  //printMetrics exec\n",
  "  () =\n",
  "  } bind def\n",
  "\n",
  "/reportEncoding {		% stack: -> e\n",
  "				% privateDict is open\n",
  "  % Print new \"e\" directive for currentfont\n",
  "  (QE ) print\n",
  "  lastEncoding 1 add /lastEncoding 1 index def\n",
  "  dup //printInt exec\n",
  "  //printMap exec\n",
  "  } bind def\n",
  "\n",
  "/spaceString (X) def		% for space char from current encoding\n",
  "\n",
  "/reportFont {			% stack: any -> unchanged\n",
  "  % Report currentfont to the user and record its number\n",
  "  % //privateDict is open\n",
  "  % Sets //privateDict/spaceString to font's encoding's space string, if any,\n",
  "  % and sets //privateDict/hasSpace to indicate whether there's a space char.\n",
  "  currentfont /UniqueID known not {\n",
  "    //reportMetrics exec\n",
  "    } {\n",
  "    //metrics currentfont /UniqueID get\n",
  "    2 copy known {\n",
  "      get\n",
  "      } {\n",
  "      //reportMetrics exec\n",
  "      dup 4 1 roll put		% define in /metrics\n",
  "      } ifelse\n",
  "    } ifelse			% stack: m\n",
  "  //encodings currentfont /Encoding get\n",
  "  2 copy known {\n",
  "    get\n",
  "    } {\n",
  "    //reportEncoding exec	% leaves e on stack\n",
  "    dup 4 1 roll put		% define in /encodings\n",
  "    % Find space character number for the encoding.\n",
  "    % Biased towards number 32, so avoid problems with weird encodings\n",
  "    //encodingSpace 1 index	% dict and key for putting the result\n",
  "    currentfont /Encoding get\n",
  "    dup length 32 le {\n",
  "      false\n",
  "      } {\n",
  "      dup 32 get dup /space eq exch /G20 eq or\n",
  "      } ifelse {\n",
  "      pop 32\n",
  "      } {\n",
  "      % It's not character number 32: search from 0\n",
  "      0 exch {\n",
  "        dup /space eq exch dup /G20 eq exch /suppress eq or or { exit } if\n",
  "        1 add\n",
  "        } forall\n",
  "      } ifelse\n",
  "    put				% put in /encodingSpace\n",
  "    } ifelse			% stack: m e\n",
  "  //encodingSpace 1 index get	% font's space character (256 if none)\n",
  "  /hasSpace 1 index 256 lt def\n",
  "  hasSpace { //spaceString exch 0 exch put } { pop } ifelse\n",
  "  (QF ) print\n",
  "  lastFontNum 1 add /lastFontNum 1 index def\n",
  "  //fonts currentfont 2 index put % record fontNum in /fonts\n",
  "  dup //printInt exec		% print fontNum\n",
  "				% stack: m e f\n",
  "  1000 0 //characterToReporting exec 2 copy //showxy exec\n",
  "  0 1000 //characterToReporting exec 2 copy //showxy exec\n",
  "  4 array astore		% stack: m e f array\n",
  "  //fontsUnit 3 1 roll put	% stack: m e\n",
  "  //printInt exec		% print encoding number\n",
  "  //printInt exec		% print metrics number\n",
  "  () =\n",
  "  } bind def\n",
  "\n",
  "/reportFontCreation {		% stack: font\n",
  "  % Report a newly created font. Called now so that if later uses are\n",
  "  % inside a save/restore we don't forget it.\n",
  "  % NOTE: this is currently not used, since it actually slows things down\n",
  "  //privateDict begin\n",
  "  inUse not {\n",
  "    /inUse true def\n",
  "    dup currentfont exch setfont //reportFont exec setfont\n",
  "    /inUse false def\n",
  "    } if\n",
  "  end\n",
  "  } bind def\n",
  "\n",
  "/reportSubString {		% stack: args string -> args\n",
  "  % Report the rendering of a string, assumed to be a single word.\n",
  "  % privateDict is open.  /n is font number, /p is call-back\n",
  "  dup length 0 eq {\n",
  "    p\n",
  "    } {\n",
  "    (QS ) print\n",
  "    n //printInt exec\n",
  "    //printCharacterOrigin exec\n",
  "    dup length //printInt exec\n",
  "    dup print ( ) print\n",
  "    /p load end			% close privateDict during the call-back\n",
  "    exec			% render the string; leaves args on stack\n",
  "    //privateDict begin\n",
  "    //printCharacterOrigin exec\n",
  "    () =\n",
  "    } ifelse\n",
  "  } bind def\n",
  "\n",
  "/report {			% stack: args string proc -> args\n",
  "  % Report the rendering of a string.\n",
  "  % Calls proc for each word and space.  The call-back should expect \"args\"\n",
  "  % on the stack followed by a string, and should leave \"args\" on the stack.\n",
  "  //privateDict begin\n",
  "  inUse {\n",
  "    end exec\n",
  "    } {\n",
  "    /inUse true def\n",
  "    //fonts currentfont\n",
  "    2 copy known not { //reportFont exec } if\n",
  "    get				% stack: probable font-number\n",
  "    % check if points transform as before ...\n",
  "    //fontsUnit 1 index get\n",
  "    1000 0 //characterToReporting exec\n",
  "    0 1000 //characterToReporting exec\n",
  "    4 index 3 get ne 4 1 roll\n",
  "    4 index 2 get ne 4 1 roll\n",
  "    4 index 1 get ne 4 1 roll\n",
  "    4 index 0 get ne 5 -1 roll pop\n",
  "    or or or {			% if transformed points differ\n",
  "      //reportFont exec\n",
  "      pop //fonts currentfont get\n",
  "      } if			% stack: args string proc n\n",
  "    /n exch def\n",
  "    /p exch def		% stack: args string\n",
  "    hasSpace {\n",
  "      {			% begin loop\n",
  "        //spaceString search {\n",
  "          exch pop exch /s exch def\n",
  "          //reportSubString exec\n",
  "          //spaceString p\n",
  "          s\n",
  "          } {\n",
  "          //reportSubString exec\n",
  "          exit\n",
  "          } ifelse\n",
  "        } loop\n",
  "      } {\n",
  "      //reportSubString exec\n",
  "      } ifelse\n",
  "    /inUse false def\n",
  "    end\n",
  "    } ifelse\n",
  "  } bind def\n",
  "\n",
  "/dontReport {			% stack: proc\n",
  "  % Call \"proc\" with //privateDict/inUse set to true\n",
  "  //privateDict /inUse get {\n",
  "    exec\n",
  "    } {\n",
  "    //privateDict /inUse true put\n",
  "    exec\n",
  "    //privateDict /inUse false put\n",
  "    } ifelse\n",
  "  } bind def\n",
  "\n",
  "/kshow.temp (X) def		% scratch space for kshow\n",
  "\n",
  "% Output inverse of initial currentmatrix, for possible use by postprocessor.\n",
  "(QI ) print\n",
  "matrix currentmatrix matrix invertmatrix\n",
  "{ 100 mul round cvi //printInt exec } forall\n",
  "() =\n",
  "\n",
  "userdict begin			% subsequent definitions are publicly visible\n",
  "\n",
  "% Objects placed in systemdict must be in global memory,\n",
  "% and must not reference local objects.\n",
  "/setglobal where\n",
  " { pop currentglobal true setglobal }\n",
  " { }\n",
  "ifelse\n",
  "\n",
  "\n",
  "%\n",
  "% Redefine the character rendering operations to call \"report\"\n",
  "%\n",
  "\n",
  "/show { { show }\n",
  "  //systemdict /pstotextLocalDict get /report get\n",
  "  exec } bind redef\n",
  "\n",
  "/ashow { {3 copy ashow pop}\n",
  "  //systemdict /pstotextLocalDict get /report get\n",
  "  exec pop pop } bind redef\n",
  "\n",
  "/widthshow { {4 copy widthshow pop}\n",
  "  //systemdict /pstotextLocalDict get /report get\n",
  "  exec pop pop pop } bind redef\n",
  "\n",
  "/awidthshow { {6 copy awidthshow pop}\n",
  "  //systemdict /pstotextLocalDict get /report get \n",
  "  exec 5 {pop} repeat } bind redef\n",
  "\n",
  "/kshow {			% stack: proc string\n",
  "  exch //systemdict /pstotextLocalDict get exch /kshow.proc exch put\n",
  "  false exch			% stack: false string\n",
  "  {				% stack: false next | prev true next\n",
  "    //systemdict /pstotextLocaldict get /kshow.temp get 0 2 index put\n",
  "    exch { //systemdict /pstotextLocalDict get\n",
  "           /kshow.proc get exec\n",
  "         } { pop } ifelse\n",
  "    //systemdict /pstotextLocaldict get\n",
  "    /kshow.temp get //show exec\n",
  "    //systemdict /pstotextLocaldict get\n",
  "    /kshow.temp get 0 get true	% stack: this true\n",
  "    } forall\n",
  "				% stack: false | last true\n",
  "  { pop } if\n",
  "  } bind redef\n",
  "\n",
  "\n",
  "%\n",
  "% Redefine non-rendering operations so that they don't report\n",
  "%\n",
  "\n",
  "/stringwidth { {stringwidth}\n",
  "  //systemdict /pstotextLocalDict get /dontReport get\n",
  "  exec } bind redef\n",
  "\n",
  "/charpath { {charpath}\n",
  "  //systemdict /pstotextLocalDict get /dontReport get\n",
  "  exec } bind redef\n",
  "\n",
  "\n",
  "%\n",
  "% Intercept and report the page operations\n",
  "%\n",
  "\n",
  "/copypage { (QC) = flush copypage } bind redef\n",
  "\n",
  "/erasepage { (QZ) = flush erasepage } bind redef\n",
  "\n",
  "/showpage { (QP) = flush showpage } bind redef\n",
  "\n",
  "\n",
  "%\n",
  "% Intercept font creation so as to record the font inside less save/restore's\n",
  "% NOTE: disabled, because it actually slows things down\n",
  "%\n",
  "\n",
  "% /definefont { definefont //reportFontCreation exec } bind redef\n",
  "\n",
  "% /makefont { makefont //reportFontCreation exec } bind redef\n",
  "\n",
  "% /scalefont { scalefont //reportFontCreation exec } bind redef\n",
  "\n",
  "%\n",
  "% Clean-up\n",
  "%\n",
  "\n",
  "% restore local/global state\n",
  "/setglobal where\n",
  " { pop setglobal }\n",
  " { }\n",
  "ifelse\n",
  "\n",
  "end				% close nested userdict begin\n",
  "end				% close private dictionary\n",
  "\n",
  "% Bind the operators we just defined, and all the others if we didn't\n",
  "% do it before.  Also reenable 'bind' for future files.\n",
  "\n",
  "revision 353 ge {\n",
  ".bindoperators\n",
  "NOBIND currentdict systemdict ne and\n",
  " { systemdict begin .bindoperators end }\n",
  "if\n",
  "/DELAYBIND where { pop DELAYBIND { .bindnow } if } if\n",
  "} if\n",
  "\n",
  "systemdict readonly pop\n",
  "% Restore the current local/global VM mode.\n",
  "% exec\n",
  "\n",
  "\n",
  "%\n",
  "% Testing\n",
  "%\n",
  "\n",
  "false {\n",
  "  100 dict begin\n",
  "  (Times 12, two strings; second one sloping up with ashow:)=\n",
  "  /Times-Roman findfont 12 scalefont dup /t12 exch def setfont\n",
  "  72 300 moveto (Hello world) show\n",
  "  72 280 moveto 10 1 (Hello world once more) ashow\n",
  "  (Times 10 two strings:)=\n",
  "  /Times-Roman findfont 10 scalefont setfont\n",
  "  72 260 moveto (Third) show\n",
  "  72 240 moveto (Fourth) show\n",
  "  (Symbol 12, one string:)=\n",
  "  /Symbol findfont 12 scalefont setfont\n",
  "  72 220 moveto (symbol string) show\n",
  "  (Helvetica 12, two strings:)=\n",
  "  /Helvetica findfont 12 scalefont setfont\n",
  "  72 200 moveto (Fifth) show\n",
  "  72 180 moveto (Sixth) show\n",
  "  (Times 12 again, two strings; second one with kshow:)=\n",
  "  t12 setfont\n",
  "  72 160 moveto (Seventh) show\n",
  "  end\n",
  "  72 140 moveto\n",
  "  gsave\n",
  "    /dx 1.0 def\n",
  "    { pop pop\n",
  "      dx 1 add /dx 1 index def\n",
  "      0 rmoveto } (Accelerated letter spacing) kshow\n",
  "    grestore\n",
  "  (Times 12 scaled by 2:)=\n",
  "  72 100 moveto\n",
  "  gsave\n",
  "    2 2 scale\n",
  "    (Ninth) show\n",
  "    grestore\n",
  "  count 0 ne { (Left on stack:)= pstack } if\n",
  "  flush\n",
  "  } if\n",
  0
};
