/*

	GALILEI Research Project

	KViewPrg.cpp

	Window to run a program - Implementation.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rqt.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsession.h>
#include <gsessionprg.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <qthread.h>
#include <qpixmap.h>
#include <qmessagebox.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kapplication.h>
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kviewprg.h"
#include "kdoc.h"
using namespace std;



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
	GSession* Session;

public:
	MyThread(GSession* session,KViewPrg* rec,RString name);
	virtual void run(void);
};


//-----------------------------------------------------------------------------
KViewPrg::MyThread::MyThread(GSession* session,KViewPrg* rec,RString name)
	: QThread(), Rec(rec), Name(name), Session(session)
{
}


//-----------------------------------------------------------------------------
void KViewPrg::MyThread::run(void)
{
	try
	{
		Session->RunPrg(Rec,Name);
		if(GSession::Break())
		{
			QMessageBox::information(Rec,"KGALILEICenter","Program Aborded");
			GSession::ResetBreak();
		}
		else
		{
			QMessageBox::information(Rec,"KGALILEICenter","Program Executed");
		}
	}
	catch(GException& e)
	{
		QMessageBox::critical(Rec,"KGALILEICenter - GALILEI Exception",e.GetMsg());
	}
	catch(RException& e)
	{
		QMessageBox::critical(Rec,"KGALILEICenter - R Exception",e.GetMsg());
	}
	catch(std::exception& e)
	{
		QMessageBox::critical(Rec,"KGALILEICenter - std::exception",e.what());
	}
	catch(...)
	{
		QMessageBox::critical(Rec,"KGALILEICenter - Undefined Error","Problem");
	}
	Rec->endPrg();
}



//-----------------------------------------------------------------------------
//
// class KViewPrg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewPrg::KViewPrg(KDoc* doc, QWidget* parent,RString name,int wflags)
	: KView(doc,parent,name,wflags), GSlot(), Name(name), Prg(0)
{
	// Window proprieties
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("make.png",KIcon::Small)));
	setCaption(ToQString(name));

	Output=new QTextEdit(this,"Output");
	Output->setReadOnly(true);
	Output->resize(size());
}


//-----------------------------------------------------------------------------
void KViewPrg::update(tObjType)
{
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
		GSessionPrg Prg("",0,0);
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
		Prg=new MyThread(Doc->GetSession(),this,Name);
		Prg->run();
	}
}


//-----------------------------------------------------------------------------
void KViewPrg::resizeEvent(QResizeEvent *)
{
	Output->resize(size());
}


//-----------------------------------------------------------------------------
void KViewPrg::endPrg(void)
{
	delete Prg;
	Prg=0;
}


//-----------------------------------------------------------------------------
bool KViewPrg::canClose(void)
{
	// If nothing running -> can close
	if(!Prg)
	{
		return(true);
	}

	// Ah ah, something runs -> ask to break it
	GSession::SetBreak();
	return(false);
}


//-----------------------------------------------------------------------------
KViewPrg::~KViewPrg(void)
{
}
