/*

	Main.cpp

	Main program - Implementation.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
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
// include files for Qt
#include <QMessageBox>


//-----------------------------------------------------------------------------
// include files for current application
#include <qgalilei.h>


//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	// Run
	QApplication Super(argc,argv);
	try
	{
		QGALILEI App(argc,argv);
		App.Execute();
	}
	catch(GALILEI::GException& e)
	{
		QMessageBox::critical(0,QWidget::tr("GALILEI Exception"),QWidget::trUtf8(e.GetMsg()),QMessageBox::Ok);
	}
	catch(R::RException& e)
	{
		QMessageBox::critical(0,QWidget::tr("R Exception"),QWidget::trUtf8(e.GetMsg()),QMessageBox::Ok);
	}
	catch(std::exception& e)
	{
		QMessageBox::critical(0,QWidget::tr("Standard exception"),QWidget::trUtf8(e.what()),QMessageBox::Ok);
	}
	catch(...)
	{
		QMessageBox::critical(0,QWidget::tr("Unknown exception"),QWidget::trUtf8("Unknown problem"),QMessageBox::Ok);
	}
 	return(0);
}
