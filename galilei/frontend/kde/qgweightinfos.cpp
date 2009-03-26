/*

	GALILEI Research Project

	QGWeightInfos.cpp

	Widget to display the description of an object - Implementation.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
#include <ui_qgweightinfos.h>
#include <qgweightinfos.h>
#include <gweightinfo.h>
#include <gsession.h>
#include <gstorage.h>
#include <gxmlindex.h>
#include <ggalileiapp.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class QGWeightInfos
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QGWeightInfos::QGWeightInfos(QWidget* parent)
	: QWidget(parent), Ui(new Ui_QGWeightInfos())
{
	static_cast<Ui_QGWeightInfos*>(Ui)->setupUi(this);
}



//------------------------------------------------------------------------------
void QGWeightInfos::Set(GWeightInfos* obj)
{
	if(!obj) return;

	// Init
	GSession* Session=GALILEIApp->GetSession();
	GConceptType* IndexSpace=Session->GetInsertConceptType("XMLIndex","XML Index");
	QTreeWidget* Infos(static_cast<Ui_QGWeightInfos*>(Ui)->Infos);
	Infos->clear();

	// Show the information entities
	RCursor<GWeightInfo> Words(obj->GetInfos());
	for(Words.Start();!Words.End();Words.Next())
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
		QString w(QString::number(Words()->GetWeight()));
		while(w.length()<10)
			w.prepend(' ');
		new QTreeWidgetItem(Infos,QStringList()<<name<<type<<w<<QString::number(Words()->GetConcept()->GetId()));
	}
	static_cast<Ui_QGWeightInfos*>(Ui)->Infos->sortItems(0,Qt::AscendingOrder);
}


//------------------------------------------------------------------------------
QGWeightInfos::~QGWeightInfos(void)
{
	delete static_cast<Ui_QGWeightInfos*>(Ui);
}
