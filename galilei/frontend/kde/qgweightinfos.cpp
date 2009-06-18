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
// include files for Qt/KDE
#include <kmessagebox.h>


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
// class Item
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class Item : public QTreeWidgetItem
{
public:

	Item(QTreeWidget* parent,const QStringList & strings) : QTreeWidgetItem(parent,strings) {}

private:

	bool operator<(const QTreeWidgetItem &other) const
	{
		int column(treeWidget()->sortColumn());
		if(column==2)
			return(text(column).toDouble()<other.text(column).toDouble());
		else if(column==3)
			return(text(column).toUInt()<other.text(column).toUInt());
		return(text(column).toLower()<other.text(column).toLower());
	}
};



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
void QGWeightInfos::Set(const GWeightInfos* obj)
{
	if(!obj) return;

	try
	{
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
				RString FullTagName(Session->GetStorage()->LoadConcept(idx->GetXMLTag()->GetId()));
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
					name+=ToQString(Session->GetStorage()->LoadConcept(Universal()->GetId()));
				}
				RCursor<GConcept> Stems(idx->GetStems());
				for(Stems.Start();!Stems.End();Stems.Next())
				{
					if(SkipComma)
						SkipComma=false;
					else
						name+=" ";
					name+=ToQString(Session->GetStorage()->LoadConcept(Stems()->GetId()));
				}
				name+="</"+ToQString(TagName)+">";
			}
			else
			{
				name=ToQString(Session->GetStorage()->LoadConcept(Words()->GetId()));
			}
			QString type=ToQString(Words()->GetType()->GetDescription());
			QString w(QString::number(Words()->GetWeight()));
			while(w.length()<10)
				w.prepend(' ');
			new Item(Infos,QStringList()<<name<<type<<w<<QString::number(Words()->GetConcept()->GetId()));
		}
		static_cast<Ui_QGWeightInfos*>(Ui)->Infos->sortItems(0,Qt::AscendingOrder);
		static_cast<Ui_QGWeightInfos*>(Ui)->Infos->resizeColumnToContents(0);
		static_cast<Ui_QGWeightInfos*>(Ui)->Infos->resizeColumnToContents(1);
		static_cast<Ui_QGWeightInfos*>(Ui)->Infos->resizeColumnToContents(2);
		static_cast<Ui_QGWeightInfos*>(Ui)->Infos->resizeColumnToContents(3);
	}
	catch(GException& e)
	{
		KMessageBox::error(this,e.GetMsg(),"GALILEI Exception");
	}
	catch(RException& e)
	{
		KMessageBox::error(this,e.GetMsg(),"R Exception");
	}
	catch(std::exception& e)
	{
		KMessageBox::error(this,e.what(),"std::exception");
	}
	catch(...)
	{
		KMessageBox::error(this,"Undefined Error");
	}
}


//------------------------------------------------------------------------------
QGWeightInfos::~QGWeightInfos(void)
{
	delete static_cast<Ui_QGWeightInfos*>(Ui);
}
