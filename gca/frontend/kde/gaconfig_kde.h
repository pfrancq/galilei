/*

	GALILEI Research Project

	GAConfig_KDE.cpp

	Generic GCA Plug-in (KDE Part) - Header.

	Copyright 2003-2009 by Pascal Francq (pascal@francq.info).
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
// include files for R
#include <rconfig.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for current plug-in
#include <ui_config.h>


//------------------------------------------------------------------------------
class GAConfigDlg : public KDialog, public Ui_Config
{
public:
	GAConfigDlg(void);
	bool Configure(RConfig* params );
};
