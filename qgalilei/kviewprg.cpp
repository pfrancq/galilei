/*

	GALILEI Research Project

	KViewPrg.cpp

	Window to run a program - Implementation.

	Copyright 2002-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R/GALILEI
#include <rqt.h>
#include <gsession.h>
#include <ggalileiapp.h>
#include <ggalileiprg.h>
#include <rprgfunc.h>
#include <rprgclass.h>
using namespace std;


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtCore/QThread>
#include <QtGui/QCloseEvent>
#include <QMessageBox>


//-----------------------------------------------------------------------------
// application specific includes
#include <kviewprg.h>
#include <qgalileiwin.h>



//-----------------------------------------------------------------------------
//
// class KViewPrg::MyThread
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
class KViewPrg::MyThread : public QThread
{
	KViewPrg* Rec;
	RString Name;

public:
	MyThread(KViewPrg* rec,RString name);
	virtual void run(void);
};


//-----------------------------------------------------------------------------
KViewPrg::MyThread::MyThread(KViewPrg* rec,RString name)
	: QThread(), Rec(rec), Name(name)
{
}


//-----------------------------------------------------------------------------
void KViewPrg::MyThread::run(void)
{
	try
	{
		GALILEIApp->RunPrg(Rec,Name);
		if(Rec->Win->getSession()->MustBreak())
		{
			QMessageBox::information(Rec,"Run Script","Program Aborded",QMessageBox::Ok);
			Rec->Win->getSession()->ResetBreak();
		}
		else
		{
			QMessageBox::information(Rec,"Run Script","Program Executed",QMessageBox::Ok);
		}
	}
	catch(GException& e)
	{
		QMessageBox::critical(0,QWidget::tr("GALILEI Exception"),QWidget::trUtf8(e.GetMsg()),QMessageBox::Ok);
	}
	catch(RException& e)
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
	Rec->endPrg();
}



//-----------------------------------------------------------------------------
//
// class KViewPrg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewPrg::KViewPrg(QGALILEIWin* win,const RString& name)
	: QMdiSubWindow(), Ui_KViewPrg(), Win(win), Name(name), Prg(0)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(ToQString(Name));
}


//-----------------------------------------------------------------------------
void KViewPrg::NextDoc(const GDoc*)
{
	QApplication::processEvents();
}


//-----------------------------------------------------------------------------
void KViewPrg::NextProfile(const GProfile*)
{
	QApplication::processEvents();
}


//-----------------------------------------------------------------------------
void KViewPrg::NextGroupLang(const GLang*)
{
	QApplication::processEvents();
}


//-----------------------------------------------------------------------------
void KViewPrg::WriteStr(const RString& str)
{
	Output->append(ToQString(str));
	QApplication::processEvents();
}


//-----------------------------------------------------------------------------
void KViewPrg::Interact(void)
{
	QApplication::processEvents();
}


//-----------------------------------------------------------------------------
void KViewPrg::run(void)
{
	QApplication::processEvents();
	if(Name.IsEmpty())
	{
		GGALILEIPrg Prg(0);
		RCursor<RPrgClass> Classes(Prg.GetClasses());
		for(Classes.Start();!Classes.End();Classes.Next())
		{
			QString Help;
			Help="<H1>"+ToQString(Classes()->GetName())+"</H1><DL>";
			RCursor<RPrgFunc> Methods(Classes()->GetMethods());
			for(Methods.Start();!Methods.End();Methods.Next())
			{
				Help+="<DT><B>"+ToQString(Methods()->GetName())+"</B></DT><DD>"+ToQString(Methods()->GetDescription())+"</DD>";
			}
			Help+="</DL>";
			Output->append(Help);
		}
	}
	else
	{
		Prg=new MyThread(this,Name);
		Prg->run();
	}
}


//-----------------------------------------------------------------------------
void KViewPrg::endPrg(void)
{
	delete Prg;
	Prg=0;
}


//-----------------------------------------------------------------------------
void KViewPrg::closeEvent(QCloseEvent *event)
{
	if(!Prg)
		event->accept();
	else
	{
		// Ah ah, something runs -> ask to break it
		Win->getSession()->SetBreak();
		event->ignore();
	}
}


//-----------------------------------------------------------------------------
KViewPrg::~KViewPrg(void)
{
}
