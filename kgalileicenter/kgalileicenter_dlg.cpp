/*

	GALILEI Research Project

	KGALILEICenter_dlg.cpp

	Main Window - Implementation of the Dialog part.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdoc.h>
#include <langs/glang.h>
#include <docs/gdocoptions.h>
#include <galilei/gurlmanagerkde.h>
#include <galilei/qlistviewitemtype.h>
#include <sessions/gsession.h>
#include <profiles/guser.h>
#include <profiles/gprofdoc.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofiledesc.h>
#include <groups/ggrouping.h>
#include <profiles/gprofilecalc.h>
#include <galilei/qcomputingpluginconfstat.h>
#include <galilei/qcomputingpluginconfreweighting.h>
#include <galilei/qcomputingpluginconffeedback.h>
#include <galilei/qgroupingpluginconfsim.h>
#include <galilei/qgroupingpluginconfgga.h>
#include <galilei/qgroupingpluginconfkcos.h>
#include <galilei/qgroupingpluginconfsupkmeans.h>
#include <galilei/qgroupingpluginconfkprotos.h>
#include <galilei/qgroupingpluginconfcure.h>
#include <galilei/qgroupcalcpluginconfrelevant.h>
#include <galilei/qgroupcalcpluginconfgravitation.h>
#include <galilei/qlinkcalchitspluginconf.h>
#include <galilei/qlinkcalccorrespondencepluginconf.h>
#include <galilei/qlinkcalcsalsapluginconf.h>
#include <galilei/qlinkcalctreshpluginconf.h>
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
#include "qdocsoptionsdlg.h"
#include "qpluginsdlg.h"
#include "qplugins.h"
#include "qgalileiitem.h"



//-----------------------------------------------------------------------------
//
// class KGALILEICenterApp
//
//-----------------------------------------------------------------------------

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
	dlg.txtMinDocs->setValue(DocOptions->MinDocs);
	dlg.txtMaxDocs->setValue(DocOptions->MaxDocs);
	dlg.txtMinOccC->setValue(DocOptions->MinOccurCluster);
	dlg.txtNbIteration->setValue(DocOptions->NbIteration);
	dlg.txtWindowSize->setValue(DocOptions->WindowSize);
	dlg.txtNbDocsMin->setValue(DocOptions->NbDocsMin);
	tmp.setNum(DocOptions->MinConfidence);
	dlg.txtMinConf->setText(tmp);
	dlg.cbDistance->setChecked(DocOptions->Distance);
	dlg.cbNonWord->setChecked(DocOptions->NonLetterWords);
	dlg.frameNonWord->setEnabled(DocOptions->NonLetterWords);
	dlg.cbUseLink->setChecked(DocOptions->UseLink);
	dlg.cbExternalLink->setChecked(DocOptions->UseExternalLink);
	dlg.cbRedirection->setChecked(DocOptions->UseRedirection);

	if(dlg.exec())
	{
		double d=dlg.txtMinStop->text().toDouble();
		if((d>=0.0)&&(d<=1.0))
			DocOptions->MinStopWords=d;
		DocOptions->StaticLang=dlg.cbStaticLang->isChecked();
		DocOptions->MinWordSize=dlg.txtMinWordSize->value();
		DocOptions->MinStemSize=dlg.txtMinStemSize->value();
		DocOptions->MinOccur=dlg.txtMinOcc->value();
		DocOptions->MinDocs=dlg.txtMinDocs->value();
		DocOptions->MaxDocs=dlg.txtMaxDocs->value();
		DocOptions->MinOccurCluster=dlg.txtMinOccC->value();
		DocOptions->NbIteration=dlg.txtNbIteration->value();
		DocOptions->WindowSize=dlg.txtWindowSize->value();
		DocOptions->NbDocsMin=dlg.txtNbDocsMin->value();
		d=dlg.txtMinConf->text().toDouble();
		if((d>=0.0)&&(d<=1.0))
			DocOptions->MinConfidence=d;
		DocOptions->Distance=dlg.cbDistance->isChecked();
		DocOptions->NonLetterWords=dlg.cbNonWord->isChecked();
		DocOptions->UseLink=dlg.cbUseLink->isChecked();
		DocOptions->UseExternalLink=dlg.cbExternalLink->isChecked();
		DocOptions->UseRedirection=dlg.cbRedirection->isChecked();

		cout<< "use sturct : "<<DocOptions->Distance<<" et a la source : "<<dlg.cbDistance->isChecked()<<endl;
		cout<< "option docs :  "<<DocOptions->UseLink<<" et a la source : "<< dlg.cbUseLink->isChecked() <<endl;;
	}
}                                


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotPlugins(void)
{
	GFactoryFilterCursor Cur;
	QPlugins dlg(this,"Plugins Dialog");
	QString str;
	QListViewItem* def=0;
	QListViewItem* cur=0;

	// Goes through filters
	Cur=GetFiltersCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		str=Cur()->GetName();
		str+=" [";
		str+=Cur()->GetLib();
		str+=" ]";
		cur=new QFilterItem(dlg.Filters,Cur(),str);
		if(!def)
			def=cur;
	}
	if(def)
	{
		dlg.Filters->setSelected(def,true);
		dlg.changeFilter(def);
		dlg.EnableFilter->setEnabled(true);
	}
	if(dlg.exec())
	{
		// Goes through filters
		QFilterItem* item=dynamic_cast<QFilterItem*>(dlg.Filters->firstChild());
		while(item)
		{
			if(item->Enable)
				item->Fac->Create();
			else
				item->Fac->Delete();
			item=dynamic_cast<QFilterItem*>(item->itemBelow());
		}
	}

}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotOldPlugins(void)
{
	QPluginsDlg dlg(this,"Plugins Dialog");
	dlg.RegisterComputingPluginConf(new QComputingPluginConfStat(&StatisticalParams));
	dlg.RegisterComputingPluginConf(new QComputingPluginConfReWeighting(&ReWeightingParams));
	dlg.RegisterComputingPluginConf(new QComputingPluginConfFeedback(&FeedbackParams));
	dlg.RegisterGroupingPluginConf(new QGroupingPluginConfSim(&SimParams));
	dlg.RegisterGroupingPluginConf(new QGroupingPluginConfGGA(&IRParams));
	dlg.RegisterGroupingPluginConf(new QGroupingPluginConfKCos(&KMeansParams));
	dlg.RegisterGroupingPluginConf(new QGroupingPluginConfSupKMeans(&SupKMeansParams));
	dlg.RegisterGroupingPluginConf(new QGroupingPluginConfKProtos(&KProtosParams));
	dlg.RegisterGroupingPluginConf(new QGroupingPluginConfCure(&CureParams));
	dlg.RegisterGroupCalcPluginConf(new QGroupCalcPluginConfRelevant(&CalcRelevantParams));
	dlg.RegisterGroupCalcPluginConf(new QGroupCalcPluginConfGravitation(&CalcGravitationParams));
	dlg.RegisterLinkCalcPluginConf(new QLinkCalcHITSPluginConf(&LinkCalcHITSParams));
	dlg.RegisterLinkCalcPluginConf(new QLinkCalcCorrespondencePluginConf(&LinkCalcCorrespondenceParams));
	dlg.RegisterLinkCalcPluginConf(new QLinkCalcSALSAPluginConf(&LinkCalcSALSAParams));
	dlg.RegisterLinkCalcPluginConf(new QLinkCalcTreshPluginConf(&LinkCalcTreshParams));
	dlg.exec();
}
