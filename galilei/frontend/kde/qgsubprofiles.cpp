/*

	GALILEI Research Project

	QGSubProfileVector.cpp

	Widget to show a subprofile desciption in vector model - Implementation.

	Copyright 2001 by the Universit� Libre de Bruxelles.

	Authors:
		

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
#include <qlayout.h>


//-----------------------------------------------------------------------------
// include files for R
#include <frontend/kde/rqt.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <infos/glang.h>
#include <frontend/kde/qgsubprofiles.h>
#include <profiles/gsubprofile.h>
#include <infos/gweightinfo.h>
#include <sessions/gsession.h>
#include <sessions/gstorage.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
//
// struct QGSubProfiles::SubProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class QGSubProfiles::SubProfile : public QWidget
{
	QListView* Info;
	QListView* Vector;
	GSession* Session;
	GSubProfile* Desc;
	char tmpWord[50];

public:
	
	SubProfile(QWidget* parent,GSession* session,GSubProfile* desc);
	void Construct(void);
	int Compare(const SubProfile*) const {return(-1);}
	int Compare(const SubProfile&) const {return(-1);}
};

//-----------------------------------------------------------------------------
GALILEI::QGSubProfiles::SubProfile::SubProfile(QWidget* parent,GSession* session,GSubProfile* desc)
	: QWidget(parent), Session(session), Desc(desc)
{
	char tmp[20];
	char sDate[20];
	RDate d;

	QVBoxLayout* QtLayout = new QVBoxLayout(this,0,0, "QtLayout");

	// Create the 'Info' ListView
	Info = new QListView(this,"Information");
	Info->addColumn("Variable");
	Info->addColumn("Value");
	new QListViewItem(Info,"ID",QString::number(desc->GetId()));
	d=desc->GetUpdated();
	sprintf(sDate,"%i/%i/%i",d.GetDay(),d.GetMonth(),d.GetYear());
	new QListViewItem(Info,"Last Updated",sDate);
	d=desc->GetComputed();
	sprintf(sDate,"%i/%i/%i",d.GetDay(),d.GetMonth(),d.GetYear());
	new QListViewItem(Info,"Last Computed",sDate);
	d=desc->GetAttached();
	sprintf(sDate,"%i/%i/%i",d.GetDay(),d.GetMonth(),d.GetYear());
	new QListViewItem(Info,"Last Attached",sDate);
	Info->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
	QtLayout->addWidget(Info);

	// Create the 'Vector' ListView
	sprintf(tmp,"Words (%u)",Desc->GetNbNoNull());
	Vector = new QListView(this, "Vector" );
	Vector->addColumn(tmp);
	Vector->addColumn(QString("Weights"));
	Vector->setSorting(2);
	Construct();
	QtLayout->addWidget(Vector);
}


//-----------------------------------------------------------------------------
void GALILEI::QGSubProfiles::SubProfile::Construct(void)
{
	GWeightInfoCursor Cur;
	char tmp[20];
	class LocalItem : QListViewItem
	{
	public:
		double Val;

		LocalItem(QListView* v,QString str,double d) : QListViewItem(v,str, QString::number(d)), Val(d) {}
		virtual int compare( QListViewItem *i, int col, bool ascending ) const
    	{
			if(col==1)
			{
				double d=Val-static_cast<LocalItem*>(i)->Val;
				if(d==0.0) return(key(0, ascending ).compare( i->key(0, ascending)));
				if(d>0)
					return(1);
				return(-1);
			}
			return(key( col, ascending ).compare( i->key( col, ascending)));
    	}
	};

	// Change the label of the first column
	sprintf(tmp,"Words (%u)",Desc->GetNbNoNull());
	Vector->setColumnText(0,tmp);

	// Read 'Ok'
	Vector->clear();
	Cur=Desc->GetWeightInfoCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		new LocalItem(Vector,ToQString(Session->GetStorage()->LoadWord(Cur()->GetId(),Desc->GetLang()->GetCode())),Cur()->GetWeight());
	}
}



//-----------------------------------------------------------------------------
//
// class QGSubProfiles
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::QGSubProfiles::QGSubProfiles(QTabWidget* parent,GSession* session,GProfile* profile)
	: QWidget(parent,"QGSubProfiles"), Session(session), Profile(profile), Desc(profile->NbPtr,5)
{
	QSize act=size(),tmp, cbsize;
	GSubProfileCursor s;
	SubProfile* w;

	// Construct the combo box
	Lang=new QComboBox(this);
	tmp=Lang->size();
	cbsize=Lang->size();
	Lang->resize(act.width(),tmp.height());
	Lang->setEditable(false);
	connect(Lang,SIGNAL(activated(int)),this,SLOT(slotLangChanged(int)));

	// For each subprofile create a widget
	s=Profile->GetSubProfilesCursor();
	for(s.Start();!s.End();s.Next())
	{
		// If language is not handled -> do not treat it
		if(!s()->GetLang()) 
			continue;
		w=new SubProfile(this,Session,s());
		break;

		if(!w) return;
		Desc.InsertPtr(w);
		Lang->insertItem(ToQString(s()->GetLang()->GetName()),Desc.GetNb()-1);
		w->move(0,cbsize.height());
		w->resize(act.width(),act.height()-cbsize.height());
		if(Desc.GetNb()==1)
		{
			Current=w;
			Lang->setCurrentItem(Desc.GetNb()-1);
			w->show();
		}
		else
			w->hide();
	}
	parent->insertTab(this,"Description");
}


//---------------------------------------------------------------------------
void GALILEI::QGSubProfiles::slotLangChanged(int index)
{
	Current->hide();
	Current=Desc.Tab[index];
	Current->show();
}


//---------------------------------------------------------------------------
void GALILEI::QGSubProfiles::slotProfileChanged(void)
{
	RCursor<SubProfile> Cur(Desc);

	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Construct();
}


//-----------------------------------------------------------------------------
void GALILEI::QGSubProfiles::resizeEvent(QResizeEvent *)
{
	RCursor<SubProfile> Cur(Desc);
	QSize act=size(),tmp;
	tmp=Lang->size();

	// Resize Combo box Lang.
	Lang->resize(act.width(),tmp.height());

	// Resize SubProfiles.
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->resize(act.width(),act.height()-tmp.height());
}


//-----------------------------------------------------------------------------
GALILEI::QGSubProfiles::~QGSubProfiles(void)
{
}

