/*

	GALILEI Research Project

	QGSubProfileVector.cpp

	Widget to show a subprofile desciption in vector model - Implementation.

	Copyright 2001-2007 by the Université Libre de Bruxelles.

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
// include files for R
#include <rqt.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>
#include <qgsubprofiles.h>
#include <gsubprofile.h>
#include <gprofile.h>
#include <gweightinfo.h>
#include <gsession.h>
#include <gstorage.h>
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
	int Compare(const SubProfile* s) const {return(Desc->GetId()-s->Desc->GetId());}
	int Compare(const SubProfile& s) const {return(Desc->GetId()-s.Desc->GetId());}
	int Compare(unsigned int id) const {return(Desc->GetId()-id);}
};


//-----------------------------------------------------------------------------
QGSubProfiles::SubProfile::SubProfile(QWidget* parent,GSession* session,GSubProfile* desc)
	: QWidget(parent), Session(session), Desc(desc)
{
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
	QtLayout->addWidget(Info);

	// Create the 'Vector' ListView
	Vector = new QListView(this, "Vector" );
	Vector->addColumn("Information Entity");
	Vector->addColumn(QString("Weight"));
	QtLayout->addWidget(Vector);
}


//-----------------------------------------------------------------------------
void QGSubProfiles::SubProfile::Construct(void)
{
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
			return(key( col, ascending ).lower().compare( i->key( col, ascending).lower()));
    	}
	};

	// Read 'Ok'
	Vector->clear();
	RCursor<GWeightInfo> Cur(Desc->GetInfos());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		new LocalItem(Vector,ToQString(Session->GetStorage()->LoadConcept(Cur()->GetId(),Cur()->GetType())),Cur()->GetWeight());
	}
}



//-----------------------------------------------------------------------------
//
// class QGSubProfiles
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QGSubProfiles::QGSubProfiles(QTabWidget* parent,GSession* session,GProfile* profile)
	: QWidget(parent,"QGSubProfiles"), Session(session), Profile(profile), Desc(profile->GetNb(),5)
{
	QSize act=size(),tmp, cbsize;

	// Construct the combo box
	QtLayout = new QVBoxLayout(this,0,0, "QtLayout");
	Lang=new QComboBox(this);
	tmp=Lang->size();
	cbsize=Lang->size();
	Lang->setEditable(false);
	QtLayout->addWidget(Lang);
	connect(Lang,SIGNAL(activated(int)),this,SLOT(slotLangChanged(int)));
	slotProfileChanged();
	parent->insertTab(this,"Descriptions");
	slotProfileChanged();
}


//---------------------------------------------------------------------------
void QGSubProfiles::slotLangChanged(int index)
{
	Current->hide();
	Current=Desc[index];
	Current->show();
}


//---------------------------------------------------------------------------
void QGSubProfiles::slotProfileChanged(void)
{
	// For each subprofile create a widget if necessary	
	RCursor<GSubProfile> s(Profile->GetSubProfiles());
	for(s.Start();!s.End();s.Next())
	{
		// If language is not handled -> do not treat it
		if(!s()->GetLang())
			continue;
		
		// Verify if a widget must be created
		SubProfile* w=Desc.GetPtr(s()->GetId());
		if(!w)
		{		
			w=new SubProfile(this,Session,s());
			if(!w) break;
			Desc.InsertPtr(w);
			Lang->insertItem(ToQString(s()->GetLang()->GetName()),Desc.GetNb()-1);
			QtLayout->addWidget(w);
			if(Desc.GetNb()==1)
			{
				Current=w;
				Lang->setCurrentItem(Desc.GetNb()-1);
				w->show();
			}
			else
				w->hide();
		}
		w->Construct();
	}
	
	// Update Subprofiles
	RCursor<SubProfile> Cur(Desc);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Construct();
}


//-----------------------------------------------------------------------------
QGSubProfiles::~QGSubProfiles(void)
{
}

