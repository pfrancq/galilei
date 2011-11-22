/*

	KGALILEICenter

	QPlugInsList.cpp

	Widget to configure a list of plug-ins - Implementation.

	Copyright 2008-2011 by Pascal Francq (pascal@francq.info).

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
// include files for Qt/KDE
#include <QtGui/QTreeWidget>


//-----------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <ggalileiapp.h>
#include <rqt.h>
#include <gdocanalyze.h>
#include <gfilter.h>
#include <glinkcalc.h>
#include <gengine.h>
#include <gmetaengine.h>
#include <gcommunitycalc.h>
#include <gtopiccalc.h>
#include <ggroupprofiles.h>
#include <ggroupdocs.h>
#include <glang.h>
#include <gprofilecalc.h>
#include <gstorage.h>
#include <gmeasure.h>
#include <gtool.h>
#include <gcomputesugs.h>
#include <gcomputetrust.h>
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
// include files for current application
#include <qpluginslist.h>



//------------------------------------------------------------------------------
//
// class QPlugIn
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * The QPlugIn represents a plug-in in the list.
 */
class QPlugIn : public QListWidgetItem
{
public:
	GPlugInFactory* PlugIn;          // Plug-in Configuration.
	bool Enable;                     // Must the plug-in be enabled?
	bool WasEnable;                  // Was the plug-in enabled ?

	QPlugIn(QListWidget* lst,GPlugInFactory* fac,const QString& desc,bool enable)
		: QListWidgetItem(desc,lst), PlugIn(fac), Enable(enable), WasEnable(enable)	{}
};



//------------------------------------------------------------------------------
//
// class QPlugInsList
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QPlugInsList::QPlugInsList(QWidget* parent)
	: QWidget(parent), Ui_QPlugInsList()
{
	setupUi(this);
}


//-----------------------------------------------------------------------------
void QPlugInsList::init(const RString& mng,bool current,bool enable,bool updown,const RString& list)
{
	QPlugIn* def;
	QPlugIn* cur;
	QString str;
	int idx;

	// Init part
	HasCurrent=current;
	HasUpDown=updown;
	Current->setVisible(current);
	Enable->setVisible(enable);
	Up->setVisible(updown);
	Down->setVisible(updown);
	GPlugInFactory* Fac(GALILEIApp->GetCurrentFactory(mng,list,false));

	// Goes through the plug-ins
	def=cur=0;
	if(current)
		Current->insertItem(0,"None");
	cur=0;
	RCursor<GPlugInFactory> Cur(GALILEIApp->GetFactories(mng,list));
	for(Cur.Start(),idx=1;!Cur.End();Cur.Next(),idx++)
	{
		str=ToQString(Cur()->GetDesc());
		str+=" [";
		str+=ToQString(Cur()->GetLib());
		str+="]";
		bool Enabled;
		if(Cur()->GetMng()->GetPlugInType()==GPlugInManager::ptListSelect)
			Enabled=GALILEIApp->GetConfig()->GetBool("Enable",Cur()->GetMng()->GetName(),Cur()->GetList(),Cur()->GetName());
		else
			Enabled=GALILEIApp->GetConfig()->GetBool("Enable",Cur()->GetMng()->GetName(),Cur()->GetName());
		cur=new QPlugIn(List,Cur(),str,Enabled);
		if(current)
		{
			Current->insertItem(idx,ToQString(Cur()->GetName()));
			if(Cur()==Fac)
				Current->setCurrentIndex(idx);
		}
		if(!def)
			def=cur;
	}
	if(def)
	{
		List->setCurrentItem(def,QItemSelectionModel::Select);
		if(enable)
			Enable->setEnabled(true);
		if(current)
			Current->setEnabled(true);
	}
}


//-----------------------------------------------------------------------------
void QPlugInsList::init(PlugInType type,const RString& cat)
{
	Type=type;
	Cat=cat;
	switch(Type)
	{
		case Storages:
			setObjectName("Storage");
			init("Storage",true,false,false);
			break;
		case Filters:
			setObjectName("Filters");
			init("Filter",false,true,false);
			break;
		case Langs:
			setObjectName("Languages");
			init("Lang",false,true,false);
			break;
		case ComputeSugs:
			setObjectName("ComputeSugs");
			init("ComputeSugs",false,true,true);
			break;
		case ComputeTrust:
			setObjectName("ComputeTrust");
			init("ComputeTrust",false,true,true);
			break;
		case ProfileCalcs:
			setObjectName("ProfileCalcs");
			init("ProfileCalc",true,true,false);
			break;
		case GroupProfiles:
			setObjectName("GroupProfiles");
			init("GroupProfiles",true,true,false);
			break;
		case GroupDocs:
			setObjectName("GroupDocs");
			init("GroupDocs",true,true,false);
			break;
		case CommunityCalcs:
			setObjectName("CommunityCalcs");
			init("CommunityCalc",true,true,false);
			break;
		case TopicCalcs:
			setObjectName("TopicCalcs");
			init("TopicCalc",true,true,false);
			break;
		case Tools:
			setObjectName(ToQString(Cat));
			init("Tools",true,true,false,Cat);
			break;
		case LinkCalcs:
			setObjectName("LinkCalcs");
			init("LinkCalc",true,true,false);
			break;
		case Tokenizers:
			setObjectName("Tokenizers");
			init("Tokenizer",true,true,false);
			break;
		case Analyzers:
			setObjectName("Analyzers");
			init("Analyzer",false,true,true);
			break;
		case Engines:
			setObjectName("Engines");
			init("Engine",false,true,false);
			break;
		case MetaEngines:
			setObjectName("MetaEngines");
			init("MetaEngine",true,true,false);
			break;
		case Measures:
			setObjectName(ToQString(Cat));
			init("Measures",true,true,false,Cat);
			break;
	}
}


//-----------------------------------------------------------------------------
void QPlugInsList::apply(GSession* session)
{
	for(int i=0;i<List->count();i++)
	{
		QPlugIn* item=dynamic_cast<QPlugIn*>(List->item(i));
		item->PlugIn->SetLevel(i);
		if(item->PlugIn->GetMng()->GetPlugInType()==GPlugInManager::ptListSelect)
			GALILEIApp->GetConfig()->SetBool("Enable",item->Enable,item->PlugIn->GetMng()->GetName(),item->PlugIn->GetList(),item->PlugIn->GetName());
		else
			GALILEIApp->GetConfig()->SetBool("Enable",item->Enable,item->PlugIn->GetMng()->GetName(),item->PlugIn->GetName());
		if(item->Enable)
		{
			if(!item->WasEnable)
			{
				item->PlugIn->Create(session);
				item->PlugIn->GetPlugIn()->CreateConfig();
			}
			if(HasUpDown)
				item->PlugIn->GetPlugIn()->FindParam<RParamValue>("Level")->SetInt(i);
			item->PlugIn->GetPlugIn()->ApplyConfig();
		}
		else
		{
			if(item->WasEnable)
				item->PlugIn->Delete();
		}
	}

	if(HasCurrent)
	{
		switch(Type)
		{
			case Storages:
				GALILEIApp->SetCurrentPlugIn("Storage",FromQString(Current->currentText()),false);
				break;
			case GroupDocs:
				GALILEIApp->SetCurrentPlugIn("GroupDocs",FromQString(Current->currentText()),false);
				break;
			case GroupProfiles:
				GALILEIApp->SetCurrentPlugIn("GroupProfiles",FromQString(Current->currentText()),false);
				break;
			case CommunityCalcs:
				GALILEIApp->SetCurrentPlugIn("CommunityCalc",FromQString(Current->currentText()),false);
				break;
			case TopicCalcs:
				GALILEIApp->SetCurrentPlugIn("TopicCalc",FromQString(Current->currentText()),false);
				break;
			case MetaEngines:
				GALILEIApp->SetCurrentPlugIn("MetaEngine",FromQString(Current->currentText()),false);
				break;
			case Tokenizers:
				GALILEIApp->SetCurrentPlugIn("Tokenizer",FromQString(Current->currentText()),false);
				break;
			case Analyzers:
				GALILEIApp->SetCurrentPlugIn("Analyzer",FromQString(Current->currentText()),false);
				break;
			case LinkCalcs:
				GALILEIApp->SetCurrentPlugIn("LinkCalc",FromQString(Current->currentText()),false);
				break;
			case ProfileCalcs:
				GALILEIApp->SetCurrentPlugIn("ProfileCalc",FromQString(Current->currentText()),false);
				break;
			case Measures:
				GALILEIApp->SetCurrentPlugIn("Measures",FromQString(Current->currentText()),Cat,false);
				break;
			case Tools:
				GALILEIApp->SetCurrentPlugIn("Tools",FromQString(Current->currentText()),Cat,false);
				break;
			default:
				std::cout<<objectName().toLatin1().data()<<" is not treated"<<std::endl;
		}
	}
}


//-----------------------------------------------------------------------------
void QPlugInsList::slotConfigure(void)
{
	 if(!List->currentItem()) return;
	 QPlugIn* f=dynamic_cast<QPlugIn*>(List->currentItem());
	 if(!f) return;
	 f->PlugIn->Configure();
}


//-----------------------------------------------------------------------------
void QPlugInsList::slotAbout(void)
{
	 if(!List->currentItem()) return;
	 QPlugIn* f=dynamic_cast<QPlugIn*>(List->currentItem());
	 if(!f) return;
	 f->PlugIn->About();
}


//-----------------------------------------------------------------------------
void QPlugInsList::slotChange(QListWidgetItem* act,QListWidgetItem*)
{
	if(!act) return;
	QPlugIn* f=dynamic_cast<QPlugIn*>(act);
	if(!f) return;
	Configure->setEnabled(f->PlugIn->GetPlugIn()&&f->PlugIn->HasConfigure());
	About->setEnabled(f->PlugIn->GetPlugIn()&&f->PlugIn->HasAbout());
	Enable->setChecked(f->Enable);
	Params->setEnabled(f->PlugIn->GetPlugIn());
}


//-----------------------------------------------------------------------------
void QPlugInsList::slotEnable(bool state)
{
	if(!List->currentItem()) return;
	QPlugIn* f=dynamic_cast<QPlugIn*>(List->currentItem());
	if(!f) return;
	f->Enable=state;
	Configure->setEnabled(f->PlugIn->GetPlugIn()&&f->PlugIn->HasConfigure());
	About->setEnabled(f->PlugIn->GetPlugIn()&&f->PlugIn->HasAbout());
	Params->setEnabled(f->PlugIn->GetPlugIn());
}


//-----------------------------------------------------------------------------
void QPlugInsList::slotUp(void)
{
	if(!List->currentItem()) return;
	QPlugIn* f=dynamic_cast<QPlugIn*>(List->currentItem());
	if(!f) return;
	int pos=List->currentRow();
	if(!pos) return;
	List->takeItem(pos);
	List->insertItem(pos-1,f);
}


//-----------------------------------------------------------------------------
void QPlugInsList::slotDown(void)
{
	if(!List->currentItem()) return;
	QPlugIn* f=dynamic_cast<QPlugIn*>(List->currentItem());
	if(!f) return;
	int pos=List->currentRow();
	if(pos==List->count()-1) return;
	List->takeItem(pos);
	List->insertItem(pos+1,f);
}


//-----------------------------------------------------------------------------
void QPlugInsList::slotParams(void)
{
	if(!List->currentItem()) return;
	QPlugIn* f=dynamic_cast<QPlugIn*>(List->currentItem());
	if(!f) return;
	KDialog Dlg(this);
	Dlg.setCaption(ToQString("Parameters of "+f->PlugIn->GetName()));
	QTreeWidget* widget(new QTreeWidget(&Dlg));
	widget->setRootIsDecorated(false);
	widget->setColumnCount(2);
	widget->setHeaderLabels(QStringList()<<"Name"<<"Description");
	widget->header()->setResizeMode(0,QHeaderView::ResizeToContents);
	widget->header()->setResizeMode(1,QHeaderView::ResizeToContents);
	RContainer<RString,true,false> Cats(10);
	f->PlugIn->GetPlugIn()->GetCategories(Cats);
	Cats.InsertPtr(new RString(RString::Null));
	RCursor<RString> Cur(Cats);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		QTreeWidgetItem* parent(0);
		if((*Cur())!=RString::Null)
			parent=new QTreeWidgetItem(widget,QStringList()<<ToQString(*Cur())<<"Category");
		RCursor<RParam> Cur2(f->PlugIn->GetPlugIn()->GetParams(*Cur()));
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			QTreeWidgetItem* item(0);
			if(parent)
				item=new QTreeWidgetItem(widget,parent);
			else
				item=new QTreeWidgetItem(widget);
			item->setText(0,ToQString(Cur2()->GetName()));
			item->setText(1,ToQString(Cur2()->GetDescription()));
		}
	}
	Dlg.setMainWidget(widget);
	Dlg.setButtons(KDialog::Ok);
	Dlg.resize(sizeHint());
	Dlg.exec();
}
