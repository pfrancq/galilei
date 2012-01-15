/*

	GALILEI Research Project

	KViewPrg.cpp

	Window to run a program - Implementation.

	Copyright 2002-2012 by Pascal Francq (pascal@francq.info).
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
#include <kmessagebox.h>
#include <QtCore/QThread>
#include <QtGui/QCloseEvent>
#include <kapplication.h>


//-----------------------------------------------------------------------------
// application specific includes
#include <kviewprg.h>
#include <kgalileicenter.h>



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
		if(KGALILEICenter::App->getSession()->MustBreak())
		{
			KMessageBox::information(Rec,"Program Aborded");
			KGALILEICenter::App->getSession()->ResetBreak();
		}
		else
		{
			KMessageBox::information(Rec,"Program Executed");
		}
	}
	catch(GException& e)
	{
		KMessageBox::error(Rec,e.GetMsg(),"GException");
	}
	catch(RException& e)
	{
		KMessageBox::error(Rec,e.GetMsg(),"RException");
	}
	catch(std::exception& e)
	{
		KMessageBox::error(Rec,e.what(),"std::exception");
	}
	catch(...)
	{
		KMessageBox::error(Rec,"Undefined Error");
	}
	Rec->endPrg();
}



//-----------------------------------------------------------------------------
//
// class KViewPrg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewPrg::KViewPrg(const RString& name)
	: QMdiSubWindow(), Ui_KViewPrg(), Name(name), Prg(0)
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
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void KViewPrg::NextProfile(const GProfile*)
{
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void KViewPrg::NextGroupLang(const GLang*)
{
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void KViewPrg::WriteStr(const RString& str)
{
	Output->append(ToQString(str));
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void KViewPrg::Interact(void)
{
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void KViewPrg::run(void)
{
	KApplication::kApplication()->processEvents();
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
		KGALILEICenter::App->getSession()->SetBreak();
		event->ignore();
	}
}


//-----------------------------------------------------------------------------
KViewPrg::~KViewPrg(void)
{
}
