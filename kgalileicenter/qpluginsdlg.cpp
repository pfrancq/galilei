/*

  qpluginsdlg.cpp

  Description - Implementation.

  (c) 2002 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//---------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gsession.h>
#include <profiles/gsubprofiledesc.h>
#include <groups/ggrouping.h>
#include <groups/ggroupcalc.h>
#include <profiles/gprofilecalc.h>
#include <galilei/qgroupcalcpluginconf.h>
#include <galilei/qlinkcalcpluginconf.h>
using namespace R;


//---------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtabwidget.h>
#include <qlistview.h>
#include <qtextview.h>


//---------------------------------------------------------------------------
// include files for KDE
#include <klocale.h>


//---------------------------------------------------------------------------
// include files for current project
#include "kgalileicenter.h"
#include "kdoc.h"
#include "qpluginsdlg.h"



//---------------------------------------------------------------------------
//
// class QPluginListView
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class QPluginsDlg::QPluginListView : public QCheckListItem
{
public:
	KGALILEICenterApp* App;
	int Type;
	RString Name;

	QPluginListView(KGALILEICenterApp* a,QListView* v,int t,const char* str) : QCheckListItem(v,str,QCheckListItem::Controller), App(a), Type(t), Name(RString(str)) {setOpen(true);}
	QPluginListView(KGALILEICenterApp* a,QCheckListItem* v,int t,const char* str);
	virtual void activate(void);
};



//---------------------------------------------------------------------------
QPluginsDlg::QPluginListView::QPluginListView(KGALILEICenterApp* a,QCheckListItem* v,int t,const char* str)
	: QCheckListItem(v,str,QCheckListItem::RadioButton), App(a), Type(t), Name(RString(str))
{
	setOpen(true);
	switch(Type)
	{
		case 1:
		{
			RString s=App->GetCurrentProfileDesc();
			if(RString(str)==(App->GetCurrentProfileDesc()))
				setOn(true);
		}
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			if(RString(str)==(App->GetCurrentGroupCalcMethod()))
				setOn(true);
			break;
		case 5:
			if(RString(str)==(App->GetCurrentLinkCalcMethod()))
				setOn(true);
			break;
	}
}


//---------------------------------------------------------------------------
void QPluginsDlg::QPluginListView::activate(void)
{
	QCheckListItem::activate();
	if((!Type)||(!isOn()))
		return;
	switch(Type)
	{
		case 1:
			{
            App->SetCurrentProfileDesc(Name);
			if(App->getDocument())
				((App->getDocument())->GetSession())->SetCurrentProfileDesc(App->GetCurrentProfileDesc());
			}
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
            App->SetCurrentGroupCalcMethod(Name);
			if(App->getDocument())
				((App->getDocument())->GetSession())->SetCurrentGroupCalcMethod(App->GetCurrentGroupCalcMethod());
			break;
		case 5:
            App->SetCurrentLinkCalcMethod(Name);
			if(App->getDocument())
				((App->getDocument())->GetSession())->SetCurrentLinkCalcMethod(App->GetCurrentLinkCalcMethod());
			break;
	}
}



//---------------------------------------------------------------------------
//
// class QPluginsDlg
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
QPluginsDlg::QPluginsDlg(KGALILEICenterApp* app,const char* name)
	: QDialog(app,name,true,Qt::WStyle_ContextHelp), Cur(0), Conf(0), App(app)
{
	GroupCalcs=new R::RContainer<QGroupCalcPluginConf,unsigned int,true,true>(3,3);
	LinkCalcs=new R::RContainer<QLinkCalcPluginConf,unsigned int,true,true>(3,3);

	// Window initialisation
	setIcon(QPixmap("/usr/share/icons/hicolor/16x16/actions/find.png"));
	setCaption("Plugins");
	resize(320,650);

	QVBoxLayout* QPluginsDlgLayout = new QVBoxLayout(this);
	QPluginsDlgLayout->setSpacing(5);
	QPluginsDlgLayout->setMargin(5);

	// initialisation of the tab widget
	Main=new QTabWidget(this);
	Plugins=new QListView(Main,"List of Plugins");
	Plugins->setSelectionMode(QListView::Single);
	Main->insertTab(Plugins,"Plugins");
	Plugins->addColumn("Plugins");
	Plugins->setRootIsDecorated(true);
	NoConf=new QTextView(0,"No Configuration");
	NoConf->setText("<H3><B>No Configurable Options Available</B></H3><BR>Sorry, non configurable options are available for the current selected plugin.");
	QPluginsDlgLayout->addWidget(Main);

	QHBoxLayout* Layout1 = new QHBoxLayout;
	Layout1->setSpacing( 5 );
	Layout1->setMargin( 5 );

	buttonApply = new QPushButton( this, "buttonOk" );
	buttonApply->setText( i18n( "&Apply" ) );
	buttonApply->setAutoDefault( TRUE );
	buttonApply->setDefault( TRUE );
    Layout1->addItem(new QSpacerItem(20,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
	Layout1->addWidget(buttonApply);
    Layout1->addItem(new QSpacerItem(20,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
	QPluginsDlgLayout->addLayout(Layout1);

	// signals and slots connections
	connect(Plugins,SIGNAL(selectionChanged(QListViewItem*)),this,SLOT(slotPlugin(QListViewItem*)));

	ConstructPlugins();
}


//---------------------------------------------------------------------------
void QPluginsDlg::RegisterGroupCalcPluginConf(QGroupCalcPluginConf* ins) throw(bad_alloc)
{
	GroupCalcs->InsertPtr(ins);
}


//---------------------------------------------------------------------------
void QPluginsDlg::RegisterLinkCalcPluginConf(QLinkCalcPluginConf* ins) throw(bad_alloc)
{
	LinkCalcs->InsertPtr(ins);
}


//---------------------------------------------------------------------------
void QPluginsDlg::ConstructPlugins(void) throw(bad_alloc)
{
	QPluginListView* item;

	item=new QPluginListView(App,Plugins,0,"Description Methods");
	item->setSelectable(false);
	for(App->ProfileDesc->Start();!App->ProfileDesc->End();App->ProfileDesc->Next())
		new QPluginListView(App,item,1,(*App->ProfileDesc)()->StrDup());
	item=new QPluginListView(App,Plugins,0,"Group Descritpion Methods");
	item->setSelectable(false);
	for(App->GroupCalcMethod->Start();!App->GroupCalcMethod->End();App->GroupCalcMethod->Next())
		new QPluginListView(App,item,4,(*App->GroupCalcMethod)()->StrDup());

	item=new QPluginListView(App,Plugins,0,"Link Description Methods");
	item->setSelectable(false);
	for(App->LinkCalcMethod->Start();!App->LinkCalcMethod->End();App->LinkCalcMethod->Next())
		new QPluginListView(App,item,5,(*App->LinkCalcMethod)()->StrDup());
}


//---------------------------------------------------------------------------
void QPluginsDlg::slotPlugin(QListViewItem* item)
{
	char tmp[200];
	QPluginListView* p=(QPluginListView*)item;
	bool b;

	if(Conf)
	{
		b=false;
		if((Conf!=NoConf)&&Cur)
		{
			switch(Cur->Type)
			{
				case 1:
					b=false;
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					b=((QGroupCalcPluginConf*)Conf)->ConfigChanged();
					break;
				case 5:
					b=((QLinkCalcPluginConf*)Conf)->ConfigChanged();
					break;

			}
			if(b)
			{
				QMessageBox mb("Unsaved changes",
				               "There are unsaved changes in the current plugin\n\n"
				               "Do you want to apply the changes before to configure another plugin",
				                QMessageBox::Warning,
				                QMessageBox::Yes | QMessageBox::Default,
				                QMessageBox::No,
				                QMessageBox::Cancel | QMessageBox::Escape );
				mb.setButtonText( QMessageBox::Yes, "Apply" );
				mb.setButtonText( QMessageBox::No, "Forget" );
				switch(mb.exec())
				{
					case QMessageBox::Yes:
						emit signalApply();
						break;
					case QMessageBox::Cancel:
						Plugins->setCurrentItem(Cur);
						return;
						break;
				}
			}
		}
 		Main->removePage(Conf);
		Conf->disconnect();
	}
	sprintf(tmp,"Configure [%s]",(p->Name).StrDup());
	switch(p->Type)
	{
		case 1:
			Conf=NoConf;
			Cur=p;
			break;

		case 2:
			break;

		case 3:
			break;

		case 4:
			Conf=NoConf;
			Conf=GroupCalcs->GetPtr<const char*>(p->Name);
			if(!Conf)
				Conf=NoConf;
			else
				((QGroupCalcPluginConf*)Conf)->Set();
			Cur=p;
			break;

		case 5:
			Conf=NoConf;
			Conf=LinkCalcs->GetPtr<const char*>(p->Name);
			if(!Conf)
				Conf=NoConf;
			else
				((QLinkCalcPluginConf*)Conf)->Set();
			Cur=p;
			break;

		default:
			Conf=0;
			Cur=0;
			break;
	}
	if(Conf)
	{
		Main->insertTab(Conf,tmp);
		if(Conf!=NoConf)
		{
			connect(buttonApply,SIGNAL(clicked()),Conf,SLOT(Apply()));
			connect(this,SIGNAL(signalApply()),Conf,SLOT(Apply()));
		}
	}
}


//---------------------------------------------------------------------------
QPluginsDlg::~QPluginsDlg(void)
{
	Main->removePage(Conf);
	if(NoConf) delete NoConf;
	if(GroupCalcs) delete GroupCalcs;
}
