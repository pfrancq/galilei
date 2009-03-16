/*

	GALILEI Research Project

	GEngineGoogle_KDE.cpp

	A KDE dialog box for Text analyser - Implementation.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors:
		Valery Vandaele(vavdaele@ulb.ac.be).

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
// include files for R
#include <rqt.h>
using namespace R;

// include files for GALILEI
#include <gengine.h>
using namespace GALILEI;

//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QColorGroup>
#include <kaboutdata.h>
#include <kaboutapplicationdialog.h>
#include <KDE/KLocale>
#include <kaboutapplicationdialog.h>
#include <knuminput.h>
#include <kurlrequester.h>
#include <Qt3Support/q3table.h>
#include <ui_genginexml.h>
#include <QtGui/qfiledialog.h>
#include <genginexml_kde.h>

//------------------------------------------------------------------------------
GEngineXML_KDE::GEngineXML_KDE(void)
{
		setCaption("Configure XML Engine Plug-In");
		QWidget* widget=new QWidget(this);
		setupUi(widget);
		setMainWidget(widget);
		setButtons(KDialog::Cancel|KDialog::Apply);
		connect(this,SIGNAL(applyClicked()),this,SLOT(accept()));
		connect(openButton, SIGNAL(clicked()), this, SLOT(OpenDir()));
		connect(remButton, SIGNAL(clicked()), this, SLOT(RemoveFolder()));
		connect(addButton, SIGNAL(clicked()), this, SLOT(AddFolder()));
		connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
		connect(okButton, SIGNAL(clicked()), this, SLOT(GEngineXML_KDE::OnOk()));

		adjustSize();
}

GEngineXML_KDE::~GEngineXML_KDE(void)
{
}

void GEngineXML_KDE::ExternAddPath(const char *path, bool inc_subf)
	{
		DIR *dir;
		QTreeWidgetItem *lvitem;

		dir = opendir(path);
		if (dir != 0)
		{
			closedir(dir);

			lvitem = new QTreeWidgetItem(pathLView);
			lvitem->setText(0, path);
			lvitem->setText(1, inc_subf ? "x" : " ");
			pathLView->addTopLevelItem(lvitem);
		}
	}

	char* GEngineXML_KDE::GetAllPaths()
	{
		QTreeWidgetItem *lvitem;
		RString temp;
		RString car[2] = {"0", "1"};
		char *res;
		int cpt=1;
		lvitem = pathLView->itemAt(1,0);
		while (lvitem){
			temp += RString(car[lvitem->text(1) == "x"] + lvitem->text(0).toLatin1().constData()) + ".";
			cpt++;
			lvitem = pathLView->itemAt(cpt,0);
		}
		//for (lvitem = pathLView->firstChild(); lvitem != 0; lvitem = lvitem->nextSibling())
		//	temp += RString(car[lvitem->text(1) == "x"] + lvitem->text(0).toLatin1().constData()) + ".";
		res = new char[temp.GetLen() + 1];
		sprintf(res, "%s", temp.Latin1());
		return res;
	}
	void GEngineXML_KDE::OnUpdate()
	{
		if (updateDB->isChecked())
			resetDB->setEnabled(true);
		else
			resetDB->setEnabled(false);
	}

	void GEngineXML_KDE::CheckLView()
	{
		DIR *dir;
		bool ok = false;

		dir = opendir(pathLEdit->text().ascii());
		if (dir != 0)
		{
			ok = true;
			closedir(dir);
		}
		addButton->setEnabled(ok);
		remButton->setEnabled(pathLView->topLevelItemCount());
	}

	void GEngineXML_KDE::init()
	{

		pathLView->setColumnWidth(0, 430);
		pathLView->setColumnWidth(1, 90);
		pathLEdit->setText("/home/fgaultier/Documents/CollectionsXML/Inex06/Collection/essaiJB1doc/");
		OnUpdate();
		CheckLView();
	}
	void GEngineXML_KDE::SetUpdate_Reset(bool state_update, bool state_reset)
	{
		updateDB->setChecked(state_update);
		resetDB->setChecked(state_reset);
	}
	void GEngineXML_KDE::OnOk()
	{
		accept();
	}
	void GEngineXML_KDE::EditLine()
	{
		CheckLView();
	}
	void GEngineXML_KDE::AddFolder()
	{
		QTreeWidgetItem *lvitem;
		QString path(pathLEdit->text());

		if (path[path.length() - 1] != '/')
		{
			path += "/";
			pathLEdit->setText(path);
		}
		lvitem = pathLView->findItems(pathLEdit->text(),Qt::MatchCaseSensitive, 0).first();
		if (lvitem != 0)
				lvitem->setText(1, subfCBox->isChecked() ? "x" : " ");
		else
		{
			lvitem = new QTreeWidgetItem(pathLView);
			lvitem->setText(0, pathLEdit->text());
			lvitem->setText(1, subfCBox->isChecked() ? "x" : " ");
			pathLView->addTopLevelItem(lvitem);
		}
		CheckLView();
	}

	//______________________________________________________________________________
	//------------------------------------------------------------------------------
	void GEngineXML_KDE::RemoveFolder()
	{

		pathLView->takeTopLevelItem(pathLView->indexOfTopLevelItem(pathLView->selectedItems().first()));
		CheckLView();
	}

	//______________________________________________________________________________
	//------------------------------------------------------------------------------
	void GEngineXML_KDE::OpenDir()
	{
		QFileDialog *fdlg;
		QString pathres;

		fdlg = new QFileDialog(this);
		fdlg->setDir(pathLEdit->text());
		fdlg->setMode(QFileDialog::Directory);
		fdlg->setFilter("*");
		if (fdlg->exec() == QDialog::Accepted)
			pathres = fdlg->selectedFile();
		pathLEdit->setText(pathres);
	}

	void GEngineXML_KDE::SelectItem()
	{
		if (pathLView->selectedItems().first())
			pathLEdit->setText(pathLView->selectedItems().first()->text(0));
	}



//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" {
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
		void About(void)
		{
			KAboutData aboutData("xmlengine", 0,ki18n("XML Search Engine"), "1.0", ki18n("This is a XML search engine."), KAboutData::License_GPL,
					ki18n("(c) 1998-2003, Université Libre de Bruxelles\nCAD/CAM Department"), KLocalizedString(), "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
			aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pfrancq@ulb.ac.be");
			aboutData.addAuthor(ki18n("faiza abbaci"),ki18n("Contributor"), "jvalsami@ulb.ac.be");
			KAboutApplicationDialog dlg(&aboutData);
			dlg.exec();
		}

//------------------------------------------------------------------------------
void Configure(GFactoryEngine* params)
{
 	GEngineXML_KDE dlg;

	char *buff;
		RString paths;
		RContainer<RString, true, false> pathlist(10);
		RCursor<RString> curs;


	//	throw RException("Adapt to the new structure");
		dlg.Name->setUrl(ToQString(params->Get("Name")));
		dlg.nbres->setValue(params->GetDouble("NbResults"));
		dlg.weight->setValue(params->GetDouble("Weight"));
		dlg.updateDB->setChecked(params->GetBool("Update"));
		dlg.resetDB->setChecked(params->GetBool("Reset"));

		paths = params->Get("Paths");
		paths.Split(pathlist, '.');
		curs.Set(pathlist);
		for (curs.Start(); !curs.End(); curs.Next())
			dlg.ExternAddPath(curs()->Mid(1).Latin1(), curs()->Mid(0, 1) == "1");

		//promethee
			dlg.DocScP->setValue(params->FindParam<RParamStruct>("DocSc Criterion")->Get<RParamValue>("P")->GetDouble());
			dlg.DocScQ->setValue(params->FindParam<RParamStruct>("DocSc Criterion")->Get<RParamValue>("Q")->GetDouble());
			dlg.DocScWeight->setValue(params->FindParam<RParamStruct>("DocSc Criterion")->Get<RParamValue>("Weight")->GetDouble());

			dlg.DisP->setValue(params->FindParam<RParamStruct>("Distance Criterion")->Get<RParamValue>("P")->GetDouble());
			dlg.DisQ->setValue(params->FindParam<RParamStruct>("Distance Criterion")->Get<RParamValue>("Q")->GetDouble());
			dlg.DisWeight->setValue(params->FindParam<RParamStruct>("Distance Criterion")->Get<RParamValue>("Weight")->GetDouble());

			dlg.TfiefP->setValue(params->FindParam<RParamStruct>("Tfief Criterion")->Get<RParamValue>("P")->GetDouble());
			dlg.TfiefQ->setValue(params->FindParam<RParamStruct>("Tfief Criterion")->Get<RParamValue>("Q")->GetDouble());
			dlg.TfiefWeight->setValue(params->FindParam<RParamStruct>("Tfief Criterion")->Get<RParamValue>("Weight")->GetDouble());

			dlg.SpecifP->setValue(params->FindParam<RParamStruct>("Specificity Criterion")->Get<RParamValue>("P")->GetDouble());
			dlg.SpecifQ->setValue(params->FindParam<RParamStruct>("Specificity Criterion")->Get<RParamValue>("Q")->GetDouble());
			dlg.SpecifWeight->setValue(params->FindParam<RParamStruct>("Specificity Criterion")->Get<RParamValue>("Weight")->GetDouble());



	if(dlg.exec())
		{

			params->Set("Name",FromQString(dlg.Name->url().url()));
			params->SetUInt("NbResults", dlg.nbres->value());
			params->SetDouble("Weight", dlg.weight->value());
			params->SetBool("Update", dlg.updateDB->isChecked());
			params->SetBool("Reset", dlg.resetDB->isChecked());
			buff = dlg.GetAllPaths();
			params->Set("Paths", buff);
			delete buff;

			//promethee
			params->FindParam<RParamStruct>("DocSc Criterion")->Get<RParamValue>("P")->SetDouble(dlg.DocScP->value());
			params->FindParam<RParamStruct>("DocSc Criterion")->Get<RParamValue>("Q")->SetDouble(dlg.DocScQ->value());
			params->FindParam<RParamStruct>("DocSc Criterion")->Get<RParamValue>("Weight")->SetDouble(dlg.DocScWeight->value());

			params->FindParam<RParamStruct>("Distance Criterion")->Get<RParamValue>("P")->SetDouble(dlg.DisP->value());
			params->FindParam<RParamStruct>("Distance Criterion")->Get<RParamValue>("Q")->SetDouble(dlg.DisQ->value());
			params->FindParam<RParamStruct>("Distance Criterion")->Get<RParamValue>("Weight")->SetDouble(dlg.DisWeight->value());

			params->FindParam<RParamStruct>("Specificity Criterion")->Get<RParamValue>("P")->SetDouble(dlg.SpecifP->value());
			params->FindParam<RParamStruct>("Specificity Criterion")->Get<RParamValue>("Q")->SetDouble(dlg.SpecifQ->value());
			params->FindParam<RParamStruct>("Specificity Criterion")->Get<RParamValue>("Weight")->SetDouble(dlg.SpecifWeight->value());

			params->FindParam<RParamStruct>("Tfief Criterion")->Get<RParamValue>("P")->SetDouble(dlg.TfiefP->value());
			params->FindParam<RParamStruct>("Tfief Criterion")->Get<RParamValue>("Q")->SetDouble(dlg.TfiefQ->value());
			params->FindParam<RParamStruct>("Tfief Criterion")->Get<RParamValue>("Weight")->SetDouble(dlg.TfiefWeight->value());

			params->Apply();
		}
}



//------------------------------------------------------------------------------

	}
// end of extern

