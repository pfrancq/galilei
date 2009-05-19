/*

	GALILEI Research Project

	QGObjectsList.cpp

	Widget to manipulate a list of objects - Implementation.

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



//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <ui_qgobjectslist.h>


//------------------------------------------------------------------------------
// include files for R/GALILEI
#include <rqt.h>
#include <rcontainer.h>
#include <qgobjectslist.h>
#include <ggalileiapp.h>
#include <glang.h>
#include <gprofile.h>
#include <guser.h>
#include <gdoc.h>
#include <gsubject.h>
#include <gsubjects.h>
#include <gsession.h>
#include <gtopic.h>
#include <gcommunity.h>
#include <gmetaengine.h>
using namespace std;
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class QGObject
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class QGObject : public QTreeWidgetItem
{
public:

	/**
	* Pointer to the element represented by the QListViewItemType.
	*/
	union
	{
		GLang* Lang;
		GDoc* Doc;
		GTopic* Topic;
		GProfile* Profile;
		GEngineDoc* EngineDoc;
		GCommunity* Community;
		GUser* User;
		GSubject* Subject;
	} Obj;

	/**
	* type of the item
	*/
	tObjType Type;

	QGObject(QTreeWidget* parent,GLang* lang)
		: QTreeWidgetItem(parent, QStringList()<<((lang)?ToQString(lang->GetName()):"?????")<<""), Type(otLang)
	{
		Obj.Lang=lang;
		setIcon(0,KIconLoader::global()->loadIcon("flag-yellow",KIconLoader::Small));
	}

	QGObject(QTreeWidget* parent,GSubject* subject,size_t nb)
		: QTreeWidgetItem(parent, QStringList()<<ToQString(subject->GetName())<<QString::number(nb)), Type(otSubject)
	{
		Obj.Subject=subject;
		setIcon(0,KIconLoader::global()->loadIcon("dashboard-show",KIconLoader::Small));
	}

	QGObject(QTreeWidgetItem* parent,GDoc* doc)
		: QTreeWidgetItem(parent, QStringList()<<ToQString(doc->GetName())<<ToQString(doc->GetURL()())), Type(otDoc)
	{
		Obj.Doc=doc;
		setIcon(0,KIconLoader::global()->loadIcon("text-xml",KIconLoader::Small));
	}

	QGObject(QTreeWidget* parent,GDoc* doc,const RDate& when)
		: QTreeWidgetItem(parent, QStringList()<<ToQString(doc->GetName())<<ToQString(when)), Type(otDoc)
	{
		Obj.Doc=doc;
		setIcon(0,KIconLoader::global()->loadIcon("text-xml",KIconLoader::Small));
	}

	QGObject(QTreeWidgetItem* parent,GDoc* doc,const RDate& when)
		: QTreeWidgetItem(parent, QStringList()<<ToQString(doc->GetName())<<ToQString(when)), Type(otDoc)
	{
		Obj.Doc=doc;
		setIcon(0,KIconLoader::global()->loadIcon("text-xml",KIconLoader::Small));
	}

	QGObject(QTreeWidgetItem* parent,GProfile* prof,const RDate& when)
		: QTreeWidgetItem(parent, QStringList()<<ToQString(prof->GetName())+" ("+ToQString(prof->GetUser()->GetFullName())+")"<<ToQString(when)), Type(otProfile)
	{
		Obj.Profile=prof;
		setIcon(0,KIconLoader::global()->loadIcon("personal",KIconLoader::Small));
	}

	QGObject(QTreeWidget* parent,GProfile* prof,const RDate& when)
		: QTreeWidgetItem(parent, QStringList()<<ToQString(prof->GetName())+" ("+ToQString(prof->GetUser()->GetFullName())+")"<<ToQString(when)), Type(otProfile)
	{
		Obj.Profile=prof;
		setIcon(0,KIconLoader::global()->loadIcon("personal",KIconLoader::Small));
	}

	QGObject(QTreeWidgetItem* parent,GProfile* prof)
		: QTreeWidgetItem(parent, QStringList()<<ToQString(prof->GetName())<<ToQString(prof->GetUser()->GetFullName())), Type(otProfile)
	{
		Obj.Profile=prof;
		setIcon(0,KIconLoader::global()->loadIcon("edit-find-user",KIconLoader::Small));
	}

	QGObject(QTreeWidget* parent,GUser* user)
		: QTreeWidgetItem(parent, QStringList()<<ToQString(user->GetFullName())<<QString::number(user->GetNb())), Type(otUser)
	{
		Obj.User=user;
		setIcon(0,KIconLoader::global()->loadIcon("personal",KIconLoader::Small));
	}

	QGObject(QTreeWidget* parent,GTopic* topic)
		: QTreeWidgetItem(parent, QStringList()<<ToQString(topic->GetName())<<QString::number(topic->GetNbObjs())), Type(otTopic)
	{
		Obj.Topic=topic;
		setIcon(0,KIconLoader::global()->loadIcon("window_new",KIconLoader::Small));
	}

	QGObject(QTreeWidget* parent,GTopic* topic,const QString& score)
		: QTreeWidgetItem(parent, QStringList()<<ToQString(topic->GetName())<<score), Type(otTopic)
	{
		Obj.Topic=topic;
		setIcon(0,KIconLoader::global()->loadIcon("window_new",KIconLoader::Small));
	}

	QGObject(QTreeWidget* parent,GCommunity* community)
		: QTreeWidgetItem(parent, QStringList()<<ToQString(community->GetName())<<QString::number(community->GetNbObjs())), Type(otCommunity)
	{
		Obj.Community=community;
		setIcon(0,KIconLoader::global()->loadIcon("user-group-new",KIconLoader::Small));
	}

	QGObject(QTreeWidget* parent,GCommunity* community,const QString& score)
		: QTreeWidgetItem(parent, QStringList()<<ToQString(community->GetName())<<score), Type(otCommunity)
	{
		Obj.Community=community;
		setIcon(0,KIconLoader::global()->loadIcon("group-user-new",KIconLoader::Small));
	}

	QGObject(QTreeWidget* parent,tDocAssessment assess)
		: QTreeWidgetItem(parent), Type(otEngineDoc)
	{
		setText(1,"");
		switch(assess)
		{
			case djOK:
				setText(0,"Relevant Documents");
				setIcon(0,KIconLoader::global()->loadIcon("folder-green",KIconLoader::Small));
				break;
			case djKO:
				setText(0,"Fuzzy Relevant Documents");
				setIcon(0,KIconLoader::global()->loadIcon("folder-orange",KIconLoader::Small));
				break;
			case djOutScope:
				setText(0,"Irrelevant Documents");
				setIcon(0,KIconLoader::global()->loadIcon("folder-red",KIconLoader::Small));
				break;
			case djAuthority:
				setText(0,"Authorities");
				break;
			case djHub:
				setText(0,"Hubs");
				break;
			default:
				break;
		}
	}

	QGObject(QTreeWidget* parent,GEngineDoc* doc)
		: QTreeWidgetItem(parent,QStringList()<<ToQString(doc->GetTitle())<<ToQString(doc->GetUrl()))
	{
		Obj.EngineDoc=doc;
		setIcon(0,KIconLoader::global()->loadIcon("document",KIconLoader::Small));
	}

	bool HasDescription(void) const
	{
		return((Type==otDoc)||(Type==otTopic)||(Type==otProfile)||(Type==otCommunity));
	}
};


//------------------------------------------------------------------------------
class LangItem
{
public:
	GLang* Lang;
	QGObject* Item;

	LangItem(GLang* lang,QGObject* item) : Lang(lang), Item(item) {}
	int Compare(const LangItem& item) const
	{
		if((!Lang)&&(!item.Lang)) return(0);
		if(!Lang) return(1);
		if(!item.Lang) return(-1);
		return(Lang->Compare(item.Lang));
	}
	int Compare(const GLang* item) const
	{
		if((!Lang)&&(!item)) return(0);
		if(!Lang) return(1);
		if(!item) return(-1);
		return(Lang->Compare(item));
	}
};



//------------------------------------------------------------------------------
//
// class QGObjectsList
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QGObjectsList::QGObjectsList(QWidget* parent)
	: QWidget(parent), Ui(new Ui_QGObjectsList)
{
	static_cast<Ui_QGObjectsList*>(Ui)->setupUi(this);
	connect(static_cast<Ui_QGObjectsList*>(Ui)->List,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(HandleItem(QTreeWidgetItem*,int)));
}


//------------------------------------------------------------------------------
void QGObjectsList::Set(oType type)
{
	GSession* Session(GALILEIApp->GetSession());
	if(!Session)
		return;
	QTreeWidget* List(static_cast<Ui_QGObjectsList*>(Ui)->List);
	List->clear();

	switch(type)
	{
		case Docs:
		{
			// Go trough each language and create a Item.
			RCursor<GLang> Langs(GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns());
			RContainer<LangItem,true,true> LangItems(Langs.GetNb());
			LangItems.InsertPtr(new LangItem(0,new QGObject(List,static_cast<GLang*>(0))));
			for(Langs.Start();!Langs.End();Langs.Next())
				LangItems.InsertPtr(new LangItem(Langs(),new QGObject(List,Langs())));

			// Go through the documents and attach to the item of the corresponding language.
			RCursor<GDoc> Docs(Session->GetDocs());
			for(Docs.Start();!Docs.End();Docs.Next())
				new QGObject(LangItems.GetPtr<const GLang*>(Docs()->GetLang())->Item,Docs());
			break;
		}
		case Topics:
		{
			RCursor<GTopic> Grps(Session->GetTopics());
			for(Grps.Start();!Grps.End();Grps.Next())
			{
				QGObject* item(new QGObject(List,Grps()));
				RCursor<GDoc> Objs(Grps()->GetObjs());
				for(Objs.Start();!Objs.End();Objs.Next())
					new QGObject(item,Objs());
			}
			break;
		}
		case CompTopics:
		{
			RCursor<GTopic> Grps(Session->GetTopics());
			for(Grps.Start();!Grps.End();Grps.Next())
			{
				QGObject* item(new QGObject(List,Grps(),
						       "Precision="+QString::number(GALILEIApp->GetSession()->GetSubjects()->GetPrecision(Grps()))+
						       " - Recall="+QString::number(GALILEIApp->GetSession()->GetSubjects()->GetRecall(Grps()))));
				RCursor<GDoc> Objs(Grps()->GetObjs());
				for(Objs.Start();!Objs.End();Objs.Next())
					new QGObject(item,Objs());
			}
			break;
		}
		case IdealTopics:
		{
			// Go through each subjects
			R::RCursor<GSubject> Grps(Session->GetSubjects()->GetNodes());
			for(Grps.Start();!Grps.End();Grps.Next())
			{
				if(!Grps()->GetNbObjs(otDoc))
					continue;
				QGObject* item(new QGObject(List,Grps(),Grps()->GetNbObjs(otDoc)));
				RCursor<GDoc> Objs(Grps()->GetObjs(static_cast<GDoc*>(0)));
				for(Objs.Start();!Objs.End();Objs.Next())
					new QGObject(item,Objs());
			}
			break;
		}
		case Users:
		{
			RCursor<GUser> Cur(Session->GetUsers());
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				QGObject* item(new QGObject(List,Cur()));
				RCursor<GProfile> Profiles(*Cur());
				for(Profiles.Start();!Profiles.End();Profiles.Next())
					new QGObject(item,Profiles());
			}
			break;
		}
		case Communities:
		{
			RCursor<GCommunity> Grps(Session->GetCommunities());
			for(Grps.Start();!Grps.End();Grps.Next())
			{
				QGObject* item= new QGObject(List,Grps());
				RCursor<GProfile> Objs(Grps()->GetObjs());
				for(Objs.Start();!Objs.End();Objs.Next())
					new QGObject(item,Objs());
			}
			break;
		}
		case CompCommunities:
		{
			RCursor<GCommunity> Grps(Session->GetCommunities());
			for(Grps.Start();!Grps.End();Grps.Next())
			{
				QGObject* item(new QGObject(List,Grps(),
						       "Precision="+QString::number(GALILEIApp->GetSession()->GetSubjects()->GetPrecision(Grps()))+
						       " - Recall="+QString::number(GALILEIApp->GetSession()->GetSubjects()->GetRecall(Grps()))));
				RCursor<GProfile> Objs(Grps()->GetObjs());
				for(Objs.Start();!Objs.End();Objs.Next())
					new QGObject(item,Objs());
			}
			break;
		}
		case IdealCommunities:
		{
			// Go through each subjects
			R::RCursor<GSubject> Grps(Session->GetSubjects()->GetNodes());
			for(Grps.Start();!Grps.End();Grps.Next())
			{
				if(!Grps()->GetNbObjs(otCommunity))
					continue;
				QGObject* item(new QGObject(List,Grps(),Grps()->GetNbObjs(otCommunity)));
				RCursor<GProfile> Objs(Grps()->GetObjs(static_cast<GProfile*>(0)));
				for(Objs.Start();!Objs.End();Objs.Next())
					new QGObject(item,Objs());
			}
			break;
		}
		default:
			std::cerr<<"QGObjectsList::Populate(oType): No populate"<<std::endl;
			break;
	}

	List->resizeColumnToContents(0);
	List->resizeColumnToContents(1);
}


//-----------------------------------------------------------------------------
void QGObjectsList::Set(oType type,GDoc* doc)
{
	if((!doc)||(type!=Assessments))
		return;
	GSession* Session(GALILEIApp->GetSession());
	QTreeWidget* List(static_cast<Ui_QGObjectsList*>(Ui)->List);
	List->clear();

	// Init different Assessments
	QGObject* ok= new QGObject(List,djOK);
	QGObject* ko= new QGObject(List,djKO);
	QGObject* hs= new QGObject(List,djOutScope);

	// Add Assessment
	RNumContainer<size_t,true>* Profiles(doc->GetFdbks());
	if(!Profiles)
		return;
	for(Profiles->Start();!Profiles->End();Profiles->Next())
	{
		GProfile* prof(Session->GetProfile((*Profiles)()));
		if(!prof)
			continue;
		GFdbk* fdbk=prof->GetFdbk(doc->GetId());
		if(!fdbk)
			continue;
		QGObject *p(0);
		switch(fdbk->GetFdbk())
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
		new QGObject(p,prof,fdbk->GetWhen());
	}

	List->resizeColumnToContents(0);
	List->resizeColumnToContents(1);
}


//-----------------------------------------------------------------------------
void QGObjectsList::Set(oType type,GProfile* profile)
{
	if((!profile)||((type!=Assessments)&&(type!=Links)))
		return;
	GSession* Session(GALILEIApp->GetSession());
	QTreeWidget* List(static_cast<Ui_QGObjectsList*>(Ui)->List);
	List->clear();

	// Init different Assessments
	QGObject *ok(0),*ko(0),*hs(0),*lh(0),*la(0);
	if(type==Assessments)
	{
		ok=new QGObject(List,djOK);
		ko=new QGObject(List,djKO);
		hs=new QGObject(List,djOutScope);
	}
	if(type==Links)
	{
		lh=new QGObject(List,djAuthority);
		la=new QGObject(List,djHub);
	}

	// Add Assessment
	RCursor<GFdbk> Docs(profile->GetFdbks());
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		GDoc* doc(Session->GetDoc(Docs()->GetDocId()));
		if(!doc)
			continue;
		QGObject *p(0);
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
			case djAuthority:
				p=la;
				break;
			case djHub:
				p=lh;
				break;
			default:
				p=0;
				break;
		}
		if(!p) continue;
		new QGObject(p,doc,Docs()->GetWhen());
	}
	List->resizeColumnToContents(0);
	List->resizeColumnToContents(1);
}


//-----------------------------------------------------------------------------
void QGObjectsList::Set(oType type,GTopic* topic)
{
	if((!topic)||(type!=Docs))
		return;
	QTreeWidget* List(static_cast<Ui_QGObjectsList*>(Ui)->List);
	List->clear();
	RCursor<GDoc> Cur(topic->GetObjs());
	for(Cur.Start();!Cur.End();Cur.Next())
		new QGObject(List,Cur(),Cur()->GetAttached());
	List->resizeColumnToContents(0);
	List->resizeColumnToContents(1);
}


//-----------------------------------------------------------------------------
void QGObjectsList::Set(oType type,GCommunity* community)
{
	if((!community)||(type!=Profiles))
		return;
	QTreeWidget* List(static_cast<Ui_QGObjectsList*>(Ui)->List);
	List->clear();
	RCursor<GProfile> Cur(community->GetObjs());
	for(Cur.Start();!Cur.End();Cur.Next())
		new QGObject(List,Cur(),Cur()->GetAttached());
	List->resizeColumnToContents(0);
	List->resizeColumnToContents(1);
}


//-----------------------------------------------------------------------------
void QGObjectsList::Set(oType type,GMetaEngine* engine,size_t nbres)
{
	if((!engine)||(type!=Docs))
		return;
	QTreeWidget* List(static_cast<Ui_QGObjectsList*>(Ui)->List);
	List->clear();

	RCursor<GEngineDoc> Cur(engine->GetEngineDocs());
	size_t i;
	for(Cur.Start(),i=0;(!Cur.End())&&(i<nbres);Cur.Next(),i++)
	{
		QTreeWidgetItem* ptr=new QGObject(List,Cur());
		new QTreeWidgetItem(ptr,QStringList()<<ToQString(Cur()->GetDescription())<<"");
	}
	List->resizeColumnToContents(0);
	List->resizeColumnToContents(1);
}


//-----------------------------------------------------------------------------
QTreeWidget* QGObjectsList::tree(void)
{
	return(static_cast<Ui_QGObjectsList*>(Ui)->List);
}


//-----------------------------------------------------------------------------
QTreeWidgetItem* QGObjectsList::currentItem(void)
{
	return(static_cast<Ui_QGObjectsList*>(Ui)->List->currentItem());
}


//-----------------------------------------------------------------------------
GUser* QGObjectsList::GetCurrentUser(void)
{
	QGObject* obj=dynamic_cast<QGObject*>(static_cast<Ui_QGObjectsList*>(Ui)->List->currentItem());
	if(!obj) return(0);
	if(obj->Type!=otUser) return(0);
	return(obj->Obj.User);
}


//-----------------------------------------------------------------------------
GProfile* QGObjectsList::GetCurrentProfile(void)
{
	QGObject* obj=dynamic_cast<QGObject*>(static_cast<Ui_QGObjectsList*>(Ui)->List->currentItem());
	if(!obj) return(0);
	if(obj->Type!=otProfile) return(0);
	return(obj->Obj.Profile);
}


//-----------------------------------------------------------------------------
GDoc* QGObjectsList::GetCurrentDoc(void)
{
	QGObject* obj=dynamic_cast<QGObject*>(static_cast<Ui_QGObjectsList*>(Ui)->List->currentItem());
	if(!obj) return(0);
	if(obj->Type!=otDoc) return(0);
	return(obj->Obj.Doc);
}


//-----------------------------------------------------------------------------
GTopic* QGObjectsList::GetCurrentTopic(void)
{
	QGObject* obj=dynamic_cast<QGObject*>(static_cast<Ui_QGObjectsList*>(Ui)->List->currentItem());
	if(!obj) return(0);
	if(obj->Type!=otTopic) return(0);
	return(obj->Obj.Topic);
}


//-----------------------------------------------------------------------------
GCommunity* QGObjectsList::GetCurrentCommunity(void)
{
	QGObject* obj=dynamic_cast<QGObject*>(static_cast<Ui_QGObjectsList*>(Ui)->List->currentItem());
	if(!obj) return(0);
	if(obj->Type!=otCommunity) return(0);
	return(obj->Obj.Community);
}


//-----------------------------------------------------------------------------
void QGObjectsList::FindNext(const QString& what,bool desc)
{
	QTreeWidget* List(static_cast<Ui_QGObjectsList*>(Ui)->List);
	QTreeWidgetItemIterator* it;
	bool Cont=true;
	bool LastTimeFound;

	if(List->currentItem())
	{
		it=new QTreeWidgetItemIterator(List->currentItem());
		LastTimeFound=true;
		// If starting from current item, begin with the next one
		if(**it)
			(++(*it));
	}
	else
	{
		it=new QTreeWidgetItemIterator(List);
		LastTimeFound=false;
	}
	while(Cont)
	{
		while((**it)&&Cont)
		{
			QGObject* item=dynamic_cast<QGObject*>(**it);
			if(item)
			{
				if((item->text(0).contains(what,Qt::CaseInsensitive))||(item->text(1).contains(what,Qt::CaseInsensitive)))
				{
					Cont=false;
					List->setCurrentItem(item);
				}
				else if((desc)&&(item->HasDescription()))
				{
					GWeightInfos* Infos(0);
//					GDoc* sub=item->Obj.Doc;
//					GConcept* find=sub->GetLang()->GetDict()->GetConcept(sub->GetLang()->GetStemming(str.latin1()));
					GConcept* find(0);
					RToDo("Find right concept");
					if(find&&(Infos->IsIn(find)))
					{
						Cont=false;
						List->setCurrentItem(item);
					}
				}
			}
			(++(*it));
		}
		if(Cont)
		{
			if(LastTimeFound)
			{
				delete it;
				it=new QTreeWidgetItemIterator(List);
				LastTimeFound=false;
			}
			else
				Cont=false;
		}
	}
	delete it;
}


//-----------------------------------------------------------------------------
void QGObjectsList::HandleItem(QTreeWidgetItem* item,int)
{
	QGObject* obj=dynamic_cast<QGObject*>(item);
	if(!obj) return;

	switch(obj->Type)
	{
		case otProfile:
			emit Show(obj->Obj.Profile);
			break;
		case otDoc:
			emit Show(obj->Obj.Doc);
			break;
		case otCommunity:
			emit Show(obj->Obj.Community);
			break;
		case otTopic:
			emit Show(obj->Obj.Topic);
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
QGObjectsList::~QGObjectsList(void)
{
	delete static_cast<Ui_QGObjectsList*>(Ui);
}
