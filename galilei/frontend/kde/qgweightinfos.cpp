/*

	GALILEI Research Project

	QGWeightInfos.cpp

	Widget to display the description of an object - Implementation.

	Copyright 2008 by the Université libre de Bruxelles.

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
// include files for R Project
#include <rcursor.h>
#include <rqt.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <qgweightinfos.h>
#include <gweightinfo.h>
#include <gsession.h>
#include <gstorage.h>
#include <gxmlindex.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class QGWeightInfos
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QGWeightInfos::QGWeightInfos(QWidget* parent,GWeightInfos* obj,GSession* session)
	: QListView(parent), Session(session)
{
	addColumn("Concept");
	addColumn("Concept Type");
	addColumn("Weight");
	IndexSpace=Session->GetInsertConceptType("XMLIndex","XML Index");
	SetObject(obj);
}



//------------------------------------------------------------------------------
void QGWeightInfos::SetObject(GWeightInfos* obj)
{
	class LocalItem : QListViewItem
	{
	public:
		double Val;

		LocalItem(QListView* v,QString name,QString type,double d) : QListViewItem(v,name,type,QString::number(d)), Val(d) {}
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

	clear();
	Object=obj;
	if(!Object) return;

	// Show the information entities
	RCursor<GWeightInfo> Words(Object->GetInfos());
	for (Words.Start();!Words.End();Words.Next())
	{
		QString name;
		if(Words()->GetType()==IndexSpace)
		{
			GXMLIndex* idx=dynamic_cast<GXMLIndex*>(Words()->GetConcept());
			RString FullTagName(Session->GetStorage()->LoadConcept(idx->GetXMLTag()->GetId(),idx->GetXMLTag()->GetType()));
			RString TagName(FullTagName.Mid(FullTagName.Find(':',-1)+1));
			name="<"+ToQString(TagName)+">";
			bool SkipComma(true);
			RCursor<GConcept> Universal(idx->GetUniversalTerms());
			for(Universal.Start();!Universal.End();Universal.Next())
			{
				if(SkipComma)
					SkipComma=false;
				else
					name+=" ";
				name+=ToQString(Session->GetStorage()->LoadConcept(Universal()->GetId(),Universal()->GetType()));
			}
			RCursor<GConcept> Stems(idx->GetStems());
			for(Stems.Start();!Stems.End();Stems.Next())
			{
				if(SkipComma)
					SkipComma=false;
				else
					name+=" ";
				name+=ToQString(Session->GetStorage()->LoadConcept(Stems()->GetId(),Stems()->GetType()));
			}
			name+="</"+ToQString(TagName)+">";
		}
		else
		{
			name=ToQString(Session->GetStorage()->LoadConcept(Words()->GetId(),Words()->GetType()));
		}
		QString type=ToQString(Words()->GetType()->GetDescription());
		new LocalItem(this,name,type,Words()->GetWeight());
	}

}
