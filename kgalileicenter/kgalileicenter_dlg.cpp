/*

	GALILEI Research Project

	KGALILEICenter_dlg.cpp

	Main Window - Implementation of the Dialog part.

	(C) 2002 by Pascal Francq

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
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdoc.h>
#include <docs/gdocoptions.h>
#include <profiles/gprofoptions.h>
#include <groups/ggroupingoptions.h>
#include <urlmanagers/gurlmanagerkde.h>
#include <galilei/qlistviewitemtype.h>
#include <sessions/gsession.h>
#include <filters/gfilter.h>
#include <filters/gmimefilter.h>
#include <profiles/guser.h>
#include <profiles/gprofdoc.h>
#include <profiles/gprofile.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qvalidator.h>
#include <qfiledialog.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qmessagebox.h>
#include <qspinbox.h>
#include <qvalidator.h>
#include <qworkspace.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kapp.h>
#include <klocale.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kgalileicenter.h"
#include "kdoc.h"
#include "kview.h"
#include "kviewusers.h"
#include "kviewdoc.h"
#include "kviewprofile.h"
#include "qloadonedoc.h"
#include "qaddnewuser.h"
#include "qaddjudgementselectdoc.h"
#include "qaddjudgementselectprofile.h"
#include "qeditjudgementselectdoc.h"
#include "qeditjudgementselectprofile.h"
#include "qaddnewdocument.h"
#include "qaddprofile.h"
#include "qgroupsoptionsdlg.h"
#include "qdocsoptionsdlg.h"
#include "qprofilesoptionsdlg.h"




//-----------------------------------------------------------------------------
//
// class KGALILEICenterApp
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotAddUser(void)
{
	QAddNewUser dlg(this,0,true);
	if(dlg.exec())
	{
		Doc->GetSession()->NewUser(dlg.LEUserName->text().latin1(),dlg.LEPassword->text().latin1(),dlg.LEFullname->text().latin1(),dlg.LEEmail->text().latin1(),
		                  dlg.LETitle->text().latin1(),dlg.LEOrganisation->text().latin1(),dlg.LEAddress1->text().latin1(),
		                  dlg.LEAddress2->text().latin1(),dlg.LECity->text().latin1(),dlg.LECountry->text().latin1());
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotAddProfile(void)
{
	KView* m = (KView*)pWorkspace->activeWindow();
	if(m->getType()!=gUsers) return;
	QAddProfile dlg(this,0,true);
	if(dlg.exec())
	{
		Doc->GetSession()->NewProfile(((KViewUsers*)m)->GetCurrentUser(),dlg.Description->text().latin1());
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotAddJudgement(void)
{
	KView* m = (KView*)pWorkspace->activeWindow();
	// if the current window is a profile
	if (m->getType()==gProfile)
	{
		QAddJudgementSelectDoc dlg(this,0,true);
		GProfile* profile =((KViewProfile*)m)->GetProfile();
		
		RContainerCursor<GDoc,unsigned int,true,true> CurDoc (Doc->GetSession()->GetDocs());
		dlg.TLTitle->setText(QString(profile->GetName()));
		dlg.TLURL->setText(QString(profile->GetUser()->GetName()));
		QListViewItem* docitem = new QListViewItem (dlg.LVUsers, "Doc");
		for (CurDoc.Start();!CurDoc.End();CurDoc.Next())
		{
			QListViewItemType* useritem = new QListViewItemType (CurDoc(),docitem, QString (CurDoc()->GetName()));
			docitem->setOpen(true);
			useritem->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/apps/personal.png"));
		}

		if(dlg.exec())
		{
			char fdbk;
			switch (dlg.CBJudgement->currentItem())
			{
				case 0:
					fdbk='O'; break;
				case 1:
					fdbk='K'; break;
				case 2:
					fdbk='N'; break;
				case 3:
					fdbk='H'; break;
			}
			QListViewItem* selecteditem= dlg.LVUsers->selectedItem();
			QListViewItemType* type = (QListViewItemType*)selecteditem;
//			Doc->GetSession()->NewJudgement(profile,type->Obj.Doc,fdbk);
			cout<< profile->GetName()<<type->Obj.Doc->GetName()<<fdbk<<endl;
		}
	}

	// if the current window is a Document	
	else if (m->getType()==gDoc)
	{
		QAddJudgementSelectProfile dlg2(this,0,true);
		GDoc* doc=((KViewDoc*)m)->GetDoc();
		RContainerCursor<GUser,unsigned int,true,true> CurUsr(Doc->GetSession()->GetUsers());
		dlg2.TLTitle->setText(QString(doc->GetName()));
		dlg2.TLURL->setText(QString(doc->GetURL()));
		if(doc->GetLang()) dlg2.TLLang->setText(doc->GetLang()->GetName());
		else dlg2.TLLang->setText(" Le Documnt n a pas encore ete analyse");
		dlg2.MimeFilter->setText(doc->GetMIMEType()->GetName());
		for (CurUsr.Start();!CurUsr.End();CurUsr.Next())
		{
			QListViewItem* useritem = new QListViewItem (dlg2.LVUsers, QString (CurUsr()->GetName()));
			useritem->setOpen(true);
			useritem->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/apps/personal.png"));
			for (CurUsr()->Start(); !CurUsr()->End(); CurUsr()->Next())
				new QListViewItemType((*CurUsr())(),useritem, QString((*CurUsr())()->GetName()));
		}
		if(dlg2.exec())
		{
			// if a item was selected and is a profile
			QListViewItem* selecteditem= dlg2.LVUsers->selectedItem();
			QListViewItemType* type = (QListViewItemType*)selecteditem;
			if ((dlg2.LVUsers->selectedItem())&&(type->Obj.Profile))
			{
				char fdbk;
				switch (dlg2.CBJudgement->currentItem())
				{
					case 0:
						fdbk='O'; break;
					case 1:
						fdbk='K'; break;
					case 2:
						fdbk='N'; break;
					case 3:
						fdbk='H'; break;
				}
				//Doc->GetSession()->NewJudgement(type->Obj.Profile,doc,fdbk);
				cout<< type->Obj.Profile->GetName()<<doc->GetName()<<fdbk<<endl;

			}
			else // if no profile was selected
			{
				QMessageBox mb( QString("Error !!"),QString( "Judgement not registered : No profile seleted  !"),QMessageBox::NoIcon,0,0,1);
				mb.show();
			}
		}
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotEditJudgement(void)
{
	GProfDocCursor Cur;

	// rajouter les 2 cas avec fenetre selectionne = gdocs et fenetre selectionne =  gusers
	KView* m = (KView*)pWorkspace->activeWindow();
	// if the current window is a profile
	if (m->getType()==gProfile)
	{
		QEditJudgementSelectDoc dlg(this,0,true);
		GProfile* profile =((KViewProfile*)m)->GetProfile();

		dlg.TLTitle->setText(QString(profile->GetName()));
		dlg.TLURL->setText(QString(profile->GetUser()->GetName()));
		QListViewItem* docitem = new QListViewItem (dlg.LVUsers, "Doc");
		Cur=profile->GetProfDocCursor();
		for (Cur.Start();!Cur.End();Cur.Next())
		{
			QListViewItemType* useritem = new QListViewItemType(Cur()->GetDoc(),docitem,Cur()->GetDoc()->GetName(), QString(QChar(Cur()->GetFdbk())));
			docitem->setOpen(true);
			useritem->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/apps/personal.png"));
		}

		if(dlg.exec())
		{
			char fdbk;
			switch (dlg.CBJudgement->currentItem())
			{
				case 0:
					fdbk='O'; break;
				case 1:
					fdbk='K'; break;
				case 2:
					fdbk='N'; break;
				case 3:
					fdbk='H'; break;
			}
			QListViewItem* selecteditem= dlg.LVUsers->selectedItem();
			QListViewItemType* type = (QListViewItemType*)selecteditem;
//			Doc->GetSession()->NewJudgement(profile,type->Obj.Doc,fdbk);
			cout<< profile->GetName()<<type->Obj.Doc->GetName()<<fdbk<<endl;
		}
	}	
	else if (m->getType()==gDoc)
	{
		QEditJudgementSelectProfile dlg2(this,0,true);
		GDoc* doc=((KViewDoc*)m)->GetDoc();
		dlg2.TLTitle->setText(QString(doc->GetName()));
		dlg2.TLURL->setText(QString(doc->GetURL()));
		if(doc->GetLang()) dlg2.TLLang->setText(doc->GetLang()->GetName());
		else dlg2.TLLang->setText(" Le Documnt n a pas encore ete analyse");
		dlg2.MimeFilter->setText(doc->GetMIMEType()->GetName());
		Cur=doc->GetProfDocCursor();
		for (Cur.Start();!Cur.End();Cur.Next())
		{
			QListViewItem* useritem = new QListViewItem (dlg2.LVUsers,Cur()->GetProfile()->GetName(), QString(QChar(Cur()->GetFdbk())));
			useritem->setOpen(true);
			useritem->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/apps/personal.png"));
		}
		if(dlg2.exec())
		{
			// if a item was selected and is a profile
			QListViewItem* selecteditem= dlg2.LVUsers->selectedItem();
			QListViewItemType* type = (QListViewItemType*)selecteditem;
			if ((dlg2.LVUsers->selectedItem())&&(type->Obj.Profile))
			{
				char fdbk;
				switch (dlg2.CBJudgement->currentItem())
				{
					case 0:
						fdbk='O'; break;
					case 1:
						fdbk='K'; break;
					case 2:
						fdbk='N'; break;
					case 3:
						fdbk='H'; break;
				}
				
			//Doc->GetSession()->NewJudgement(type->Obj.Profile,doc,fdbk);
			cout<< type->Obj.Profile->GetName()<<doc->GetName()<<fdbk<<endl;
			}
			else // if no profile was selected
			{
				QMessageBox mb( QString("Error !!"),QString( "Judgement not registered : No profile seleted  !"),QMessageBox::NoIcon,0,0,1);
				mb.show();
			}
		}
	}
}



//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotAddDoc(void)
{
	QAddNewDoc dlg(this,0,true);
	dlg.UrlTxt->setText(QString("http://cfao47.ulb.ac.be/"));
	if(dlg.exec())
	{
		switch (dlg.ComboBox1->currentItem())
		{
			case 0:
				Doc->GetSession()->NewDoc(dlg.UrlTxt->text().latin1(),dlg.NameTxt->text().latin1(),"text/html");
				break;
			case 1:
				Doc->GetSession()->NewDoc(dlg.UrlTxt->text().latin1(),dlg.NameTxt->text().latin1(),"text/email");
				break;
		}
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotProfilesOptions(void)
{
	QProfilesOptionsDlg dlg(this,0,true);
	dlg.cbModel->setCurrentItem(ProfOptions->Model-1);
	dlg.vsListSize->setValue(ProfOptions->ListSize);
	dlg.vsMethod->setCurrentItem(ProfOptions->VectorMethod-1);
	if(dlg.exec())
	{
		switch(dlg.cbModel->currentItem())
		{
			case 0:
				ProfOptions->Model=sdVector;
				break;
			default:
				ProfOptions->Model=sdNothing;
				break;
			
		}
		switch(dlg.vsMethod->currentItem())
		{
			case 0:
				ProfOptions->VectorMethod=vmQueryExpansion;
				break;
			case 1:
				ProfOptions->VectorMethod=vmDirectReweighting;
				break;
			default:
				ProfOptions->VectorMethod=vmNothing;
				break;
			
		}
		ProfOptions->ListSize=dlg.vsListSize->value();
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGroupsOptions(void)
{
	QString tmp;

	QGroupsOptionsDlg dlg(this,0,true);
	dlg.LevelSim->setValidator(new QDoubleValidator(0.0,1.0,3,dlg.LevelSim));
	tmp.setNum(GroupingOptions->LevelSim);
	dlg.LevelSim->setText(tmp);
	dlg.FullSim->setChecked(GroupingOptions->FullSim);
	dlg.Method->setCurrentItem(GroupingOptions->Method-1);
	dlg.GAPopSize->setValue(GroupingOptions->GAPopSize);
	dlg.GAMaxGen->setValue(GroupingOptions->GAMaxGen);
	dlg.GAStep->setChecked(GroupingOptions->GAStep);
	dlg.GAStepGen->setEnabled(GroupingOptions->GAStep);
	dlg.GAStepGen->setValue(GroupingOptions->GAStepGen);
	if(dlg.exec())
	{
		double d=dlg.LevelSim->text().toDouble();
		if((d>=0.0)&&(d<=1.0))
			GroupingOptions->LevelSim=d;
		switch(dlg.Method->currentItem())
		{
			case 0:
				GroupingOptions->Method=pgHeuristicSim;
				break;
			case 1:
				GroupingOptions->Method=pgGA;
				break;
			default:
				GroupingOptions->Method=pgNothing;
				break;
		
		}
		GroupingOptions->FullSim=dlg.FullSim->isChecked();
		GroupingOptions->GAPopSize=dlg.GAPopSize->value();
		GroupingOptions->GAMaxGen=dlg.GAMaxGen->value();
		GroupingOptions->GAStep=dlg.GAStep->isChecked();
		GroupingOptions->GAStepGen=dlg.GAStepGen->value();
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDocsOptions(void)
{
	QString tmp;

	QDocsOptionsDlg dlg(this,0,true);
	dlg.cbStaticLang->setChecked(DocOptions->StaticLang);
	tmp.setNum(DocOptions->MinStopWords);
	dlg.txtMinStop->setText(tmp);
	dlg.txtMinWordSize->setValue(DocOptions->MinWordSize);
	dlg.txtMinStemSize->setValue(DocOptions->MinStemSize);
	dlg.txtMinOcc->setValue(DocOptions->MinOccur);
	dlg.cbNonWord->setChecked(DocOptions->NonLetterWords);
	dlg.frameNonWord->setEnabled(DocOptions->NonLetterWords);
	for(Filters.Start();!Filters.End();Filters.Next())
		new QListViewItem(dlg.lvFilters,Filters()->GetName(),Filters()->GetMIMES(),Filters()->GetVersion());
	if(dlg.exec())
	{
		double d=dlg.txtMinStop->text().toDouble();
		if((d>=0.0)&&(d<=1.0))
			DocOptions->MinStopWords=d;
		DocOptions->StaticLang=dlg.cbStaticLang->isChecked();
		DocOptions->MinWordSize=dlg.txtMinWordSize->value();
		DocOptions->MinStemSize=dlg.txtMinStemSize->value();
		DocOptions->MinOccur=dlg.txtMinOcc->value();
		DocOptions->NonLetterWords=dlg.cbNonWord->isChecked();
	}
}
