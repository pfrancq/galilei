/*

	GALILEI Research Project

	GenericSims_KDE.h

	GenericSims - Implementation.

	Copyright 2005-2008 by the Université Libre de Bruxelles.

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
#ifndef GGenericSims_KDEH
#define GGenericSims_KDEH


//------------------------------------------------------------------------------
// include files for Qt
#include <qcombobox.h>


//------------------------------------------------------------------------------
// include files for KDE
#include <knuminput.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <qgmeasure2elementsdlg.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
/**
 * Dialog class to configure a plug-in implementing a Multi-space similarity
 * measure. It must called:
 * @code
 * void Configure(GFactoryMeasure* params)
 * {
 *    GGenericSimsDlg dlg;
 *    dlg.Configure(params);
 * }
 * @endcode
 */
class GGenericSimsDlg : public QGMeasure2ElementsDlg
{
    QComboBox* SimType;
	KDoubleNumInput* Factor;

public:

	GGenericSimsDlg(const QString& title);
	virtual void Panel(void);
	virtual void Init(GFactoryMeasure* params);
	virtual void Done(GFactoryMeasure* params);
};


//------------------------------------------------------------------------------
#endif
