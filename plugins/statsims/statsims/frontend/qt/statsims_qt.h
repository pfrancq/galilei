/*

	GALILEI Research Project

	StatSims_Qt.h

	Similarity Statistics (Qt Part) - Header.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef StatSims_QtH
#define StatSims_QtH



//------------------------------------------------------------------------------
// include files for R/GALILEI
#include <rqt.h>
#include <qraboutdialog.h>
#include <gplugin.h>
using namespace R;
using namespace GALILEI;


//------------------------------------------------------------------------------
// include files for Qt
#include <QFileDialog>
#include <QtGui/QDialog>
#include <ui_config.h>


//------------------------------------------------------------------------------
class Config : public QDialog, public Ui_Config
{
	Q_OBJECT

public:

	Config(void);

public slots:

	void chooseResults(void);
	void chooseDocsSims(void);
	void chooseDocsIncs(void);
};


//------------------------------------------------------------------------------
#endif
