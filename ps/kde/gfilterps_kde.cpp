/*

	GALILEI Research Project

	GFilterPS_KDE.cpp

	A KDE about box for PostScript filter - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gfilter.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <klocale.h>
#include <kaboutapplication.h>


//-----------------------------------------------------------------------------
// Description of the application
static const char *description =
	I18N_NOOP("This is the PostScript filter for GALILEI.");



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "ps", I18N_NOOP("PostScript Filter"),
		"1.0", description, KAboutData::License_GPL,
		"(c) 2003, UniversitÃ© Libre de Bruxelles\nCAD/CAM Department\n"
		" It is based on the pstotext program.", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Pascal Francq",I18N_NOOP("Maintainer"), "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Paul McJones",I18N_NOOP("pstotext author"), "mcjones@pa.dec.com");
	aboutData.addAuthor("Andrew Birrell",I18N_NOOP("pstotext author"), "birrell@pa.dec.com");
	aboutData.setLicenseText( I18N_NOOP("GRANT.  Subject to the provisions contained herein, Digital Equipment\n"
		"Corporation (\"Digital\") hereby grants you a non-exclusive license to\n"
		"use its accompanying proprietary software product and associated\n"
		"documentation (\Software\") free of charge pursuant to the terms and\n"
		"conditions of this Agreement.\n\n"
		"You are not entitled to support or telephone assistance in connection\n"
		"with your use of the Software.\n\n"
		"SOFTWARE AND DOCUMENTATION.  Digital shall furnish the Software to you\n"
		"electronically or on media in source code form.  This license does not\n"
		"grant you any right to any enhancement or update to the Software and\n"
		"Documentation.\n\n"
		"USE RESTRICTIONS.  You may use, copy, modify, and distribute the\n"
		"Software in source code or object code form, subject to the following\n"
		"conditions:\n\n"
		"        (1) If the Software is modified, any Software containing\n"
		"modifications must prominently state in the modified product or\n"
		"documentation (i) that it has been modified, (ii) the identity of the\n"
		"person or entity that made the modifications, and (iii) the date the\n"
		"modifications were made.\n\n"
		"        (2) Each copy of the Software made by you shall be subject to\n"
		"the terms of this Agreement and shall contain all of Digital's notices\n"
		"regarding copyrights, trademarks and other proprietary rights as\n"
		"contained in the Software originally provided to you.\n\n"
		"        (3) The Software may not be transferred to any third party\n"
		"unless such third party receives a copy of this Agreement and agrees\n"
		"to be bound by all of its terms and conditions.\n\n"
		"TITLE.  Title, ownership rights, and intellectual property rights in\n"
		"and to the Software shall remain in Digital and/or its suppliers. The\n"
		"Software is protected by the copyright laws of the United States and\n"
		"international copyright treaties.\n\n"
		"CONTENT.  Title, ownership rights, and intellectual property rights in\n"
		"and to the content accessed through the Software is the property of\n"
		"the applicable content owner and may be protected by applicable\n"
		"copyright or other law.  This License gives you no rights to such\n"
		"content.\n\n"
		"DISCLAIMER OF WARRANTY.  Since the Software is provided free of\n"
		"charge, the Software is provided on an \"AS IS\" basis, without warranty\n"
		"of any kind, including without limitation the warranties of\n"
		"merchantability, fitness for a particular purpose and\n"
		"non-infringement.  The entire risk as to the quality and performance\n"
		"of the Software is borne by you.  Should the Software prove defective,\n"
		"you, and not Digital assume the entire cost of any service and repair.\n"
		"This disclaimer of warranty constitutes an essential part of the\n"
		"agreement.\n\n"
		"LIMITATION OF LIABILITY.  UNDER NO CIRCUMSTANCES AND UNDER NO LEGAL\n"
		"THEORY, TORT, CONTRACT, OR OTHERWISE, SHALL DIGITAL OR ITS SUPPLIERS\n"
		"RESELLERS, OR LICENSEES BE LIABLE TO YOU OR ANY OTHER PERSON FOR ANY\n"
		"DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES OF ANY\n"
		"CHARACTER INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF GOODWILL,\n"
		"STOPPAGE, COMPUTER FAILURE OR MALFUNCTION, OR ANY AND ALL OTHER\n"
		"COMMERCIAL DAMAGES OR LOSSES, EVEN IF DIGITAL SHALL HAVE BEEN INFORMEDv"
		"OF THE POSSIBILITY OF SUCH DAMAGES, OR FOR ANY CLAIM BY ANY OTHER\n"
		"PARTY.\n\n"
		"EXPORT CONTROLS.  You may not download or otherwise export or reexport\n"
		"the Software or any underlying information or technology except in\n"
		"full compliance with all United States and other applicable laws and\n"
		"regulations.  By downloading or using the Software, you are agreeing\n"
		"to the foregoing.\n\n"
		"TERMINATION.  This Agreement shall automatically terminate upon\n"
		"failure by you to comply with its terms, in which case you shall\n"
		"immediately discontinue the use of the Software and shall within ten\n"
		"(10) days return to Digital or destroy all copies of the Software.\n"
		"You may also terminate this Agreement at any time by destroying the\n"
		"Software and all copies thereof.\n\n"
		"MISCELLANEOUS.  This Agreement represents the complete and exclusive\n"
		"statement of the agreements concerning this license between the\n"
		"parties.  It may be amended only by a writing executed by both\n"
		"parties.  If any provision of this Agreement is held to be\n"
		"unenforceable for any reason, such provision shall be reformed only tov"
		"the extent necessary to make it enforceable, and such decision shall\n"
		"not affect the enforceability (i) of such provision under other\n"
		"circumstances or (ii) of the remaining provisions hereof under all\n"
		"circumstances.  Headings shall not be considered in interpreting this\n"
		"Agreement.  This Agreement shall be governed by and construed under\n"
		"the laws of the Commonwealth of Massachusetts, except as governed by\n"
		"Federal law.  This Agreement will not be governed by the United\n"
		"Nations Convention of Contracts for the International Sale of Goods,\n"
		"the application of which is hereby expressly excluded.\n\n"
		"U.S. Government Restricted Rights.  Use, duplication or disclosure by\n"
		"the Government is subject to restrictions set forth in subparagraphs\n"
		"(a) through (d) of the Commercial Computer-Restricted Rights clause at\n"
		"FAR 52 227-19 when applicable, or in subparagraph (c) (1) (ii) of the\n"
		"Rights in Technical Data and Computer Software clause at DFARS\n"
		"252.227-7013, and in similar clauses in the NASA FAR Supplement.\n"
		"Contractor/manufacturer is Digital Equipment Corporation."));
	KAboutApplication dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
