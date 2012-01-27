/*

	GALILEI Research Project

	GMetaEngineSum_KDE.cpp

	Meta-engine based on a Weighted Sum - Header (KDE Part).

   Copyright 2003-2012 by Pascal Francq.
	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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
#include <gmetaenginesum.h>
#include <rqt.h>
using namespace R;
using namespace std;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QPushButton>
#include <kaboutdata.h>
#include <klocale.h>
#include <kaboutapplicationdialog.h>
#include <knuminput.h>
#include <ui_config.h>


//------------------------------------------------------------------------------
class Config : public KDialog, public Ui_Config
{
	Q_OBJECT

public:
	Config(void);

public slots:

	void changeType(int index);
};

