/*

	GALILEI Research Project

	KViewProfile.cpp

	Window to manipulate a given profile - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <infos/glang.h>
#include <galilei/qgprofile.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <docs/gdoc.h>
#include <profiles/gprofdoc.h>
#include <groups/ggroup.h>
#include <groups/ggroups.h>
#include <sessions/gsession.h>
#include <galilei/qlistviewitemtype.h>
#include <galilei/qgsubprofiledescs.h>
using namespace GALILEI;


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
	setCaption("Profile \""+QString(Profile->GetName())+"\"");

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
	Desc=new QGSubProfileDescs(Infos,Doc->GetSession(),Profile,sdVector);         

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

	FdbksLinks = new QListView(Infos);
	Infos->insertTab(FdbksLinks,"Links");
	FdbksLinks->addColumn(QString("Document"));
	FdbksLinks->addColumn(QString("URL"));
	FdbksLinks->addColumn(QString("Date"));
	FdbksLinks->setRootIsDecorated(true);
	connect(FdbksLinks,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));

	ConstructFdbks();

	Pov = new QListView(Infos);
	Infos->insertTab(Pov,"Point Of View");
	Pov->addColumn(QString("Point of view list of documents"));
	//Pov->addColumn(QString("attached words"));
	Pov->setRootIsDecorated(true);

	connect(Pov,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	//connect(Pov,SIGNAL(clicked(int row, int col, int bttn, QPoint mousePos)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructPov2(profile);	
	
}


//-----------------------------------------------------------------------------
void KViewProfile::ConstructFdbks(void)
{
	QListViewItem *p;
	RDate d;
	char sDate[20];
	GProfDocCursor Docs;
	GSubProfileCursor SubCur;

	if(!Fdbks) return;
	if(!FdbksLinks) return;


	// Init different judgements
	Fdbks->clear();
	QListViewItemType* ok= new QListViewItemType(Fdbks, "Relevant Documennts");
	ok->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("ok.png",KIcon::Small)));
	QListViewItemType* ko= new QListViewItemType(Fdbks, "Fuzzy Relevant Documents");
	ko->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("remove.png",KIcon::Small)));
	QListViewItemType* hs= new QListViewItemType(Fdbks, "Irrelevant Documents");
	hs->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("stop.png",KIcon::Small)));

	// Init different judgements for document from link analysis.
	FdbksLinks->clear();
	QListViewItemType* lh= new QListViewItemType(FdbksLinks, "Hub Documents");
	//lh->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("",KIcon::Small)));
	QListViewItemType* la= new QListViewItemType(FdbksLinks, "Authority Documents");
	//la->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("",KIcon::Small)));


	// Add Judgements from profiles.
	Docs=Profile->GetProfDocCursor();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		switch(Docs()->GetFdbk())
		{
			case djOK:
				p=ok;
				break;
			case (djOK|djAutority):
				p=la;
				break;
			case djKO:
				p=ko;
				break;
			case djHub:
				p=lh;
				break;
			case djOutScope:
				p=hs;
				break;
			default:
				p=0;
				break;
		}
		if(!p) continue;
		d=Docs()->GetUpdated();
		sprintf(sDate,"%i/%i/%i",d.GetDay(),d.GetMonth(),d.GetYear());
		QListViewItemType* prof = new QListViewItemType(Docs()->GetDoc(),p,Docs()->GetDoc()->GetName().Latin1(),Docs()->GetDoc()->GetURL().Latin1(),sDate);
		prof->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("konqueror.png",KIcon::Small)));
	}

	// Add Judgements from subprofiles.
	SubCur=Profile->GetSubProfilesCursor();
	for (SubCur.Start(); !SubCur.End(); SubCur.Next())
	{
		Docs=SubCur()->GetProfDocCursor();
		for(Docs.Start();!Docs.End();Docs.Next())
		{
			switch(Docs()->GetFdbk())
			{
				case djOK:
					p=ok;
					break;
				case (djOK| djAutority):
					p=la;
					break;
				case djKO:
					p=ko;
					break;
				case djHub:
					p=lh;
					break;
				case djOutScope:
					p=hs;
					break;
				default:
					p=0;
					break;
			}
			if(!p) continue;
			d=Docs()->GetUpdated();
			sprintf(sDate,"%i/%i/%i",d.GetDay(),d.GetMonth(),d.GetYear());
			QListViewItemType* prof = new QListViewItemType(Docs()->GetDoc(),p,Docs()->GetDoc()->GetName().Latin1(),Docs()->GetDoc()->GetURL().Latin1(),sDate);
			prof->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("konqueror.png",KIcon::Small)));
		}
	}
}

//-----------------------------------------------------------------------------
void KViewProfile::ConstructGroups(void)
{
	GSubProfile* sub;
	GFactoryLangCursor CurLang;
	GLang* lang;
	char sDate[20];
	RDate d;

	Groups->clear();
	CurLang=Doc->GetSession()->GetLangs()->GetLangsCursor();
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		lang=CurLang()->GetPlugin();
		GGroups* grs=Doc->GetSession()->GetGroups(lang);
		QListViewItemType* grsitem = new QListViewItemType(Groups,lang->GetName());
		grsitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("locale.png",KIcon::Small)));
		sub=Profile->GetSubProfile(lang);
		if(!sub) continue;
		for (grs->Start(); !grs->End(); grs->Next())
		{
			GGroup* gr=(*grs)();
			if(!gr->IsIn(sub)) continue;
			for(gr->Start(); !gr->End(); gr->Next())
			{
				GSubProfile* sub=(*gr)();

				d=sub->GetAttached();
				sprintf(sDate,"%i/%i/%i",d.GetDay(),d.GetMonth(),d.GetYear());
				QListViewItemType* subitem=new QListViewItemType(sub->GetProfile(),grsitem,sub->GetProfile()->GetName(),sub->GetProfile()->GetUser()->GetFullName(),sDate);
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
	new QListViewItem(User,"ID",itou(usr->GetId()).Latin1());
	new QListViewItem(User,"Full Name",usr->GetFullName());
	new QListViewItem(User,"Name",usr->GetName());
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
}


//-----------------------------------------------------------------------------
void KViewProfile::resizeEvent(QResizeEvent *)
{
	Infos->resize(size());
}


//-----------------------------------------------------------------------------
void KViewProfile::ComputeProfile(void)
{
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Compute Profile");
	d->ComputeProfile(Profile);
	Desc->slotProfileChanged();
	General->slotProfileChanged();
}


//-----------------------------------------------------------------------------
void KViewProfile::ConstructPov(GProfile* /*profile*/)
{
// 	GInOutputBase *datainput = NULL;
// 	GStandardInOutPutCursor Outpt; //= GetGOutPutCursor();
// 	GStandardInOutPutCursor liste;
// 	GInOutputBase *listdoc;
// 	GLang *lang;
// 	int rows[200][200];
// 	int x = 200;
// 	int y = 200;
// 	datainput = new GInOutputBase();
// 	char str[256];
// 	int i,j,k,z,l,pos;
// 	int result = 0;
// 	QTable *table;
// 	lang=Doc->GetSession()->GetLangs()->GetLang("en");   // A CORRIGER ERREUR POUR DEVELOPPEMENT
// 
// // 	datainput = Doc->GetSession()->LoadPointOfView(profile->GetSubProfile(lang));
// // 	i = Doc->GetSession()->LevelExist(1, profile->GetSubProfile(lang)->GetId());
// // 	i = profile->GetId();
// 
// 	table = new QTable(x,y,this);
// 	QStringList items;
// 
// 	for(j = 0; j < x; j++)
// 		for(k=0; k < y; k++)
// 			rows[j][k] = -1;
// 
// 	if(datainput)
// 	{
// 		Outpt = datainput->GetGStandardInOutPutCursor();
// //		if(!Doc->GetSession()->LevelExist(1,profile->GetId())) // no level 1
// //		{
// //			for(i = 2; Doc->GetSession()->LevelExist(i,profile->GetId())== 0; i++)
// //			; // boucle de positionnement
// //			for(Outpt.Start(); !Outpt.End(); Outpt.Next())
// //		  {
// //				if(Outpt()->GetLevel() == i)
// //					Outpt()->SetLevel(1);
// //			}
// //		}
// 
// 		for( i = 1, pos = 0; Doc->GetSession()->LevelExist(i,profile->GetSubProfile(lang)->GetId()); i++) // scan all the levels
// 		{
// 			for(Outpt.Start(); !Outpt.End(); Outpt.Next()) // for each level scan the container
// 			  {
// 				if(i == 1 && Outpt()->GetLevel() == 1) // heads
// 				{
// 					rows[pos][0] = Outpt()->GetIdw();
// 					//table->setItem( 0, pos,  new QTableItem( table, QTableItem::WhenCurrent, QString( Outpt()->GetWord() ) ) );
// 					QStringList items;
// 					listdoc = new GInOutputBase();
// 					if(listdoc = Doc->GetSession()->GetDocList(profile->GetSubProfile(lang)->GetGroup()->GetId(),Outpt()->GetCurProfile()->GetId(), Outpt()->GetIdw())) // recupere liste documents
// 					{
// 						liste = listdoc->GetGStandardInOutPutCursor();
// 						items<<Outpt()->GetWord();
// 						for(liste.Start(); !liste.End(); liste.Next())
// 						{
// 							strcpy(str,GetNameDoc(liste()->DocRef));
// 							items<<str; 		//items<<liste()->DocRef;
// 						}
// 						table->setItem(0,pos,new QComboTableItem(table, items, true));
// 						delete(listdoc);
// 						pos++;
// 					}
// 				}
// 				else
// 				{
//          				if(Outpt()->GetLevel() == i)
// 					{
// 						for( k = 0; (result = Outpt()->GetParentPos(k)) != 0; k++)
// 						{
//            						for( j = 0; rows[j][0] != -1; j++)
// 							{
// 						 		for( l = 0; rows[j][l] != -1; l++)
// 								{
//              								if(rows[j][l] == result )
// 									{
// 										z = 0;
// 										while( rows[j][z] != -1) z++;
// 										rows[j][z] = Outpt()->GetIdw();
//  										QListViewItem *item = new QListViewItem( Pov );
// 										QStringList items;
// 										listdoc = new GInOutputBase();
// 										if(listdoc = Doc->GetSession()->GetDocList(profile->GetSubProfile(lang)->GetGroup()->GetId(),Outpt()->GetCurProfile()->GetId(), Outpt()->GetIdw())) // recupere liste documents
// 										{
// 											liste = listdoc->GetGStandardInOutPutCursor();
// 											items<<Outpt()->GetWord();
// 											for(liste.Start(); !liste.End(); liste.Next())
// 											{
// 												strcpy(str,GetNameDoc(liste()->DocRef));
// 												items<<str;
// 												//items<<liste()->DocRef;
// 											}
// 											table->setItem(z,j,new QComboTableItem(table, items, true));
// 											delete(listdoc);
// 										}
// 										break;
// 									}
// 								}
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 	 }// fin if datainput
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
/**	GInOutputBase *datainput = NULL;
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

	lang=Doc->GetSession()->GetLangs()->GetLang("en");   // A CORRIGER ERREUR POUR DEVELOPPEMENT
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
KViewProfile::~KViewProfile(void)
{
}
