/*

	GALILEI Research Project

	KViewProfile.cpp

	Window to manipulate a given profile - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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
// includes files for Qt
#include <qpixmap.h>
#include <qtabwidget.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>
#include <qgprofile.h>
#include <guser.h>
#include <gprofile.h>
#include <gsubprofile.h>
#include <gdocproxy.h>
#include <ggroup.h>
#include <ggroups.h>
#include <gsession.h>
#include <qlistviewitemtype.h>
#include <qgsubprofiles.h>
#include <rqt.h>
#include <gpluginmanagers.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kviewprofile.h"
#include "qsessionprogress.h"
#include "kdoc.h"
#include <qtable.h>



//-----------------------------------------------------------------------------
//
// class KViewProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewProfile::KViewProfile(GProfile* profile,KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Profile(profile), Fdbks(0)
{
	// Window initialisation
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
	setCaption("Profile \""+QString(ToQString(Profile->GetName()))+"\"");

	// initialisation of the tab widget
	Infos=new QTabWidget(this);
	Infos->resize(size());

	// Initialisation of the General Information Widget
	General = new QGProfile(Infos,Profile);
	Infos->insertTab(General,"General Information");

	// Initialisation of the User Widget
	User = new QListView(Infos);
	Infos->insertTab(User,"User");
	User->addColumn("Variable");
	User->addColumn("Value");
	ConstructUser();

	// Initialisation of the Descriptions Widget
	Desc=new QGSubProfiles(Infos,Doc->GetSession(),Profile);

	// Initialisation of the Groups Widget
	Groups=new QListView(Infos);
	Infos->insertTab(Groups,"Groups");
	Groups->addColumn(QString("Profiles"));
	Groups->addColumn(QString("Users"));
	Groups->addColumn(QString("Attached"));
	Groups->setRootIsDecorated(true);
	connect(Groups,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructGroups();

	// Initialisation of the Feedbacks Widget
	Fdbks = new QListView(Infos);
	Infos->insertTab(Fdbks,"Documents");
	Fdbks->addColumn(QString("Document"));
	Fdbks->addColumn(QString("URL"));
	Fdbks->addColumn(QString("Date"));
	Fdbks->setRootIsDecorated(true);
	connect(Fdbks,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructFdbks();

	// Initialisation of the Links Widget
	FdbksLinks = new QListView(Infos);
	Infos->insertTab(FdbksLinks,"Links");
	FdbksLinks->addColumn(QString("Document"));
	FdbksLinks->addColumn(QString("URL"));
	FdbksLinks->addColumn(QString("Date"));
	FdbksLinks->setRootIsDecorated(true);
	connect(FdbksLinks,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructLinks();

	Pov = new QListView(Infos);
	Infos->insertTab(Pov,"Point Of View");
	Pov->addColumn(QString("Point of view list of documents"));
	//Pov->addColumn(QString("attached words"));
	Pov->setRootIsDecorated(true);
}


//-----------------------------------------------------------------------------
void KViewProfile::ConstructFdbks(void)
{
	QListViewItem *p;
	RDate d;
	char sDate[20];
	RCursor<GFdbk> Docs;
	RCursor<GSubProfile> SubCur;
	GDoc* doc;

	if(!Fdbks) return;

	// Init different judgements
	Fdbks->clear();
	QListViewItemType* ok= new QListViewItemType(Fdbks, "Relevant Documennts");
	ok->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("ok.png",KIcon::Small)));
	QListViewItemType* ko= new QListViewItemType(Fdbks, "Fuzzy Relevant Documents");
	ko->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("remove.png",KIcon::Small)));
	QListViewItemType* hs= new QListViewItemType(Fdbks, "Irrelevant Documents");
	hs->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("stop.png",KIcon::Small)));

	// Add Judgements from profiles.
	Docs=Profile->GetFdbks();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		switch(Docs()->GetFdbk())
		{
			case djOK:
				p=ok;
				break;
			case djKO:
				p=ko;
				break;
			case djOutScope:
				p=hs;
				break;
			default:
				p=0;
				break;
		}
		if(!p) continue;
		doc=GSession::Get()->GetDoc(Docs()->GetDoc()->GetId());
		d=doc->GetUpdated();
		sprintf(sDate,"%i/%i/%i",d.GetDay(),d.GetMonth(),d.GetYear());
		QListViewItemType* prof = new QListViewItemType(doc,p,ToQString(doc->GetName()),ToQString(doc->GetURL()),sDate);
		prof->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("konqueror.png",KIcon::Small)));
	}
}


//-----------------------------------------------------------------------------
void KViewProfile::ConstructLinks(void)
{
	QListViewItem *p;
	RDate d;
	RString iconName="";
	char sDate[20];
	RCursor<GFdbk> Docs;
	RCursor<GSubProfile> SubCur;
	GDoc* doc;

	if(!FdbksLinks) return;

	// Init different judgements for document from link analysis.
	FdbksLinks->clear();
	QListViewItemType* lh= new QListViewItemType(FdbksLinks, "Hub Documents");
	lh->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("ok.png",KIcon::Small)));
	QListViewItemType* la= new QListViewItemType(FdbksLinks, "Authority Documents");
	la->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("ok.png",KIcon::Small)));

	// Add Judgements from profiles.
	Docs=Profile->GetFdbks();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		switch(Docs()->GetFdbk())
		{
			case (djOK | djAutority):
				p=la;
				iconName="konquerorAutho.png";
				break;
			case (djOK | djHub):
				p=lh;
				iconName="konquerorHub.png";
				break;
			default:
				p=0;
				break;
		}
		if(!p) continue;
		doc=GSession::Get()->GetDoc(Docs()->GetDoc()->GetId());
		d=doc->GetUpdated();
		sprintf(sDate,"%i/%i/%i",d.GetDay(),d.GetMonth(),d.GetYear());
		QListViewItemType* prof = new QListViewItemType(doc,p,ToQString(doc->GetName()),ToQString(doc->GetURL()),sDate);
		prof->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon(ToQString(iconName),KIcon::Small)));
	}
}


//-----------------------------------------------------------------------------
void KViewProfile::ConstructGroups(void)
{
	GSubProfile* sub;
	R::RCursor<GFactoryLang> CurLang;
	GLang* lang;
	char sDate[20];
	RDate d;
	RCursor<GSubProfile> Sub;

	Groups->clear();
	CurLang=(dynamic_cast<GLangManager*>(GPluginManagers::PluginManagers.GetManager("Lang")))->GetFactories();
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		lang=CurLang()->GetPlugin();
		if(!lang) continue;
		R::RCursor<GGroup> grs=Doc->GetSession()->GetGroupsCursor(lang);
		QListViewItemType* grsitem = new QListViewItemType(Groups,ToQString(lang->GetName()));
		grsitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("locale.png",KIcon::Small)));
		sub=Profile->GetSubProfile(lang);
		if(!sub) continue;
		for (grs.Start(); !grs.End(); grs.Next())
		{
			GGroup* gr=grs();
			if(!gr->IsIn(sub)) continue;
			Sub=grs()->GetSubProfilesCursor();
			for(Sub.Start(); !Sub.End(); Sub.Next())
			{
				GSubProfile* sub=Sub();

				d=sub->GetAttached();
				sprintf(sDate,"%i/%i/%i",d.GetDay(),d.GetMonth(),d.GetYear());
				QListViewItemType* subitem=new QListViewItemType(sub->GetProfile(),grsitem,ToQString(sub->GetProfile()->GetName()),ToQString(sub->GetProfile()->GetUser()->GetFullName()),sDate);
				subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
			}
		}
	}
}


//-----------------------------------------------------------------------------
void KViewProfile::ConstructUser(void)
{
	const GUser* usr=Profile->GetUser();

	User->clear();
	new QListViewItem(User,"ID",ToQString(itou(usr->GetId())));
	new QListViewItem(User,"Full Name",ToQString(usr->GetFullName()));
	new QListViewItem(User,"Name",ToQString(usr->GetName()));
}


//-----------------------------------------------------------------------------
void KViewProfile::update(unsigned int cmd)
{
	if(cmd==1)
	{
		General->slotProfileChanged();
		Desc->slotProfileChanged();
	}
	if(cmd==2)
	{
		ConstructGroups();
	}
	if(cmd==3)
	{
		ConstructLinks();
	}
}


//-----------------------------------------------------------------------------
void KViewProfile::resizeEvent(QResizeEvent *)
{
	Infos->resize(size());
}



//-----------------------------------------------------------------------------
void KViewProfile::ConstructPov(GProfile* /*profile*/)
{
}


//-----------------------------------------------------------------------------
char* KViewProfile::GetNameDoc(char* /*longname*/)
{
/*	int len = 0;
	int l = 0;
	int i = 0;
	int k = 0;
	char shortname[128];

	memset(shortname,0,128);
	len = strlen(longname);
	for( i = len; longname[i] != '/'; i--, l++)
	;
	for(k = 0 ; l > 0 ; l--, i++, k++)
		shortname[k] = longname[i];
		return shortname;*/
	return(0);
}


//-----------------------------------------------------------------------------
void KViewProfile::ConstructPov2(GProfile* /*profile*/)
{
/*	GInOutputBase *datainput = NULL;
	GInOutputBase *listdoc;
	GStandardInOutPutCursor Outpt;
	GStandardInOutPutCursor  scan;
	GStandardInOutPutCursor  liste;
	GLang *lang;
	int i, pos;
	int rows[200][200];
	int x = 200;
	int y = 200;
	int j,k,l;
	int z;
	unsigned int result;
	char lword[128];
	QListViewItem *p = 0;

	lang=(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLang("en");   // A CORRIGER ERREUR POUR DEVELOPPEMENT
	datainput = Doc->GetSession()->LoadPointOfView(profile->GetSubProfile(lang));

	for(j = 0; j < x; j++)
		for(k=0; k < y; k++)
			rows[j][k] = -1;

	if(datainput)
	{
		Outpt = datainput->GetGStandardInOutPutCursor();
		scan = datainput->GetGStandardInOutPutCursor();
		Pov->clear();
		for( i = 1, pos = 0; Doc->GetSession()->LevelExist(i,profile->GetSubProfile(lang)->GetId()); i++) // scan all the levels
		{
			for(Outpt.Start(); !Outpt.End(); Outpt.Next()) // for each level scan the container
		  	{
				if(i == 1 && Outpt()->GetLevel() == 1) // heads
				{
						rows[pos][0] = Outpt()->GetIdw();
						pos++;
						QListViewItemType* heads = new QListViewItemType(Pov,Outpt()->GetWord());
						heads->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("locale.png",KIcon::Small)));
						// construit la liste de ce headword

					for(scan.Start(); !scan.End(); scan.Next()) // for each level scan the container
		  			{
						if(scan()->GetLevel() == 1)
							continue;
						for( k = 0; (result = scan()->GetParentPos(k)) != 0; k++)
						{
     	 			   			if(result == Outpt()->GetIdw())
							{
								for( j = 0; rows[j][0] != -1; j++)
								{
									for( l = 0; rows[j][l] != -1; l++)
									{
										if(result == rows[j][l])
										{
											z = 0;
									   		while( rows[j][z] != -1) z++;
											rows[j][z] = scan()->GetIdw();
											if(listdoc = Doc->GetSession()->GetDocList(profile->GetSubProfile(lang)->GetGroup()->GetId(),scan()->GetCurProfile()->GetId(), scan()->GetIdw())) // recupere liste documents
		       									{
												liste = listdoc->GetGStandardInOutPutCursor();
												memset(lword,0,128);
												sprintf(lword,"%s",scan()->GetWord());
												QListViewItemType* subitem=new QListViewItemType(heads,lword);
												subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
*/
}

//-----------------------------------------------------------------------------
void KViewProfile::GetListSubPov(GInOutputBase* /*datainput*/)
{
/*
	for ( int i = 0 ; i < 10; i++ )
	{
		new QListViewItem( Pov, (*datainput->StdInOutPut)()->GetWord() );
	}
*/
}

//tableau contenant les Qlist


//-----------------------------------------------------------------------------
void KViewProfile::ComputeProfile(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Compute Profile");
	if(!Dlg.Run(new QComputeProfile(Profile,false,false,false)))
		return;
	General->slotProfileChanged();
	Desc->slotProfileChanged();
}


//-----------------------------------------------------------------------------
KViewProfile::~KViewProfile(void)
{
}
