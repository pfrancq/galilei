/*

	GALILEI Research Project

	GTextAnalyse_KDE.h

	Analyze a document (KDE Part) - Header.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
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



//------------------------------------------------------------------------------
#ifndef GTextAnalyse_KDEH
#define GTextAnalyse_KDEH


//------------------------------------------------------------------------------
// include files for R
#include <rqt.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdocanalyze.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <kaboutdata.h>
#include <kaboutapplicationdialog.h>
#include <KDE/KLocale>
#include <ui_config.h>
#include <QtGui/QInputDialog>


//------------------------------------------------------------------------------
class Config : public KDialog, public Ui_Config
{
	Q_OBJECT

public:
	Config(void);
public slots:

	void toggleExtractStruct( bool toggle );
	void toggleFullIndex(bool toggle );
	void clickedMetadata(void);
	void addTag(void);
	void removeTag(void);
};


//------------------------------------------------------------------------------
#endif
