/*

	GALILEI Research Project

	GXML_Qt.cpp

	XML Filter (Qt Part) - Implementation.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for Qt
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>


//------------------------------------------------------------------------------
// include files for R/GALILEI
#include <rnodecursor.h>
#include <gxml_qt.h>
#include <rxmlstruct.h>
#include <rxmlfile.h>
#include <rqt.h>
#include <qraboutdialog.h>
#include <gfilter.h>
using namespace R;
using namespace std;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class Config
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Config::Config(void)
	: QDialog(), Ui_Config()
{	
	setupUi(this);
	setWindowTitle("Configure XML Filter");
	connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
	connect(ExtractMetadata,SIGNAL(clicked()),this,SLOT(clickedMetadata()));
	connect(DetectMetadataTag,SIGNAL(clicked()),this,SLOT(clickedMetadata()));
	connect(AddMetadataTag,SIGNAL(clicked()),this,SLOT(addMetadataTag()));
	connect(RemoveMetadataTag,SIGNAL(clicked()),this,SLOT(removeMetadataTag()));
	connect(AddExcludeTag,SIGNAL(clicked()),this,SLOT(addExcludeTag()));
	connect(RemoveExcludeTag,SIGNAL(clicked()),this,SLOT(removeExcludeTag()));
	connect(ImportTags,SIGNAL(clicked()),this,SLOT(importFile()));
	adjustSize();
}


//------------------------------------------------------------------------------
void Config::clickedMetadata(void)
{
	MetadataMaxTerms->setEnabled(DetectMetadataTag->isChecked()&ExtractMetadata->isChecked());
	MetadataMaxDepth->setEnabled(DetectMetadataTag->isChecked()&ExtractMetadata->isChecked());
	MetadataChildTags->setEnabled(DetectMetadataTag->isChecked()&ExtractMetadata->isChecked());
	MetadataMaxOccurs->setEnabled(DetectMetadataTag->isChecked()&ExtractMetadata->isChecked());
	DetectMetadataTag->setEnabled(ExtractMetadata->isChecked());
	MetadataDetection->setEnabled(DetectMetadataTag->isEnabled());
}


//------------------------------------------------------------------------------
void Config::addMetadataTag(void)
{
	bool ok;
	QString tag(QInputDialog::getText(this,tr("Add a Metadata Tag"),tr("Tag:"),QLineEdit::Normal,"",&ok));
	if(ok&&(!tag.isEmpty()))
	{
		MetadataTags->addItem(tag);
		RemoveMetadataTag->setEnabled(true);
	}
}


//------------------------------------------------------------------------------
void Config::removeMetadataTag(void)
{
	int row(MetadataTags->currentRow());
	if(row!=-1)
	{
		QListWidgetItem* item(MetadataTags->takeItem(row));
		delete item;
	}
	if(!MetadataTags->count())
		RemoveMetadataTag->setEnabled(false);
}


//------------------------------------------------------------------------------
void Config::addExcludeTag(void)
{
	bool ok;
	QString tag(QInputDialog::getText(this,tr("Add a Exclude Tag"),tr("Tag:"),QLineEdit::Normal,"",&ok));
	if(ok&&(!tag.isEmpty()))
	{
		ExcludeTags->addItem(tag);
		RemoveExcludeTag->setEnabled(true);
	}
}


//------------------------------------------------------------------------------
void Config::removeExcludeTag(void)
{
	int row(ExcludeTags->currentRow());
	if(row!=-1)
	{
		QListWidgetItem* item(ExcludeTags->takeItem(row));
		delete item;
	}
	if(!ExcludeTags->count())
		RemoveExcludeTag->setEnabled(false);
}


//------------------------------------------------------------------------------
void Config::importFile(void)
{
	QString File(QFileDialog::getOpenFileName(this,tr("Choose a XML file that defines tag"), "~", tr("*.xml")));
	if(File.isEmpty())
		return;
	try
	{
		RXMLStruct Tags;
		RXMLFile In(FromQString(File),&Tags);
		In.Open(RIO::Read);
		RXMLTag* Root(Tags.GetTag("tags"));
		if(!Root)
			throw bad_alloc();
		RNodeCursor<RXMLStruct,RXMLTag> Tag(Root);
		for(Tag.Start();!Tag.End();Tag.Next())
		{
			if(Tag()->GetName()=="metadata")
			{
				// Add the tag to metadata if not existing
				QString Add(ToQString(Tag()->GetContent()));
				QList<QListWidgetItem*> Find(MetadataTags->findItems(Add,Qt::MatchExactly));
				if(Find.empty())
				{
					MetadataTags->addItem(Add);
					RemoveMetadataTag->setEnabled(true);
				}
			}
			if(Tag()->GetName()=="exclude")
			{
				// Add the tag to exclude tag if not existing
				QString Add(ToQString(Tag()->GetContent()));
				QList<QListWidgetItem*> Find(ExcludeTags->findItems(Add,Qt::MatchExactly));
				if(Find.empty())
				{
					ExcludeTags->addItem(Add);
					RemoveExcludeTag->setEnabled(true);
				}
			}

		}
	}
	catch(RException& e)
	{
		QMessageBox::critical(this,"Error",ToQString(e.GetMsg()));
	}
	catch(...)
	{
		QMessageBox::critical(this,"Error","Problem when parsing the XML file '"+File+"'");
	}
}


//------------------------------------------------------------------------------
extern "C" void About(void)
{
	QRAboutDialog dlg("XML Filter","1.0");
	dlg.setDescription("This is the XML filter for GALILEI.");
	dlg.setCopyright(QWidget::trUtf8("(C) 2001-2008 by the Université Libre de Bruxelles (ULB)<br/>(C) 2010-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.exec();
}


//------------------------------------------------------------------------------
extern "C" bool Configure(GPlugIn* fac)
{
	Config dlg;

	// Semantic
	dlg.ExtractSemantic->setChecked(fac->FindParam<RParamValue>("ExtractSemantic")->GetBool());
	dlg.UseDefaultNamespace->setChecked(fac->FindParam<RParamValue>("UseDefaultNamespace")->GetBool());
	dlg.DefaultNamespace->setEnabled(fac->FindParam<RParamValue>("UseDefaultNamespace")->GetBool());
	dlg.DefaultNamespace->setText(ToQString(fac->FindParam<RParamValue>("DefaultNamespace")->Get()));
	dlg.ExtractMetadata->setChecked(fac->FindParam<RParamValue>("ExtractMetadata")->GetBool());
	dlg.DetectMetadataTag->setChecked(fac->FindParam<RParamValue>("DetectMetadataTag")->GetBool());
	dlg.MetadataMaxTerms->setValue(fac->FindParam<RParamValue>("MetadataMaxTerms")->GetInt());
	dlg.MetadataMaxDepth->setValue(fac->FindParam<RParamValue>("MetadataMaxDepth")->GetInt());
	dlg.MetadataMaxOccurs->setValue(fac->FindParam<RParamValue>("MetadataMaxOccurs")->GetInt());
	dlg.MetadataChildTags->setChecked(fac->FindParam<RParamValue>("MetadataChildTags")->GetBool());

	// Metadata
	RCursor<RString> Tags(fac->FindParam<RParamList>("MetadataTags")->GetList());
	for(Tags.Start();!Tags.End();Tags.Next())
		dlg.MetadataTags->addItem(ToQString(*Tags()));
	if(!dlg.MetadataTags->count())
		dlg.RemoveMetadataTag->setEnabled(false);
	dlg.clickedMetadata();

	// Exclude
	Tags=fac->FindParam<RParamList>("ExcludeTags")->GetList();
	for(Tags.Start();!Tags.End();Tags.Next())
		dlg.ExcludeTags->addItem(ToQString(*Tags()));
	if(!dlg.ExcludeTags->count())
		dlg.RemoveExcludeTag->setEnabled(false);

	if(dlg.exec())
	{
		// Semantic
		fac->FindParam<RParamValue>("ExtractSemantic")->SetBool(dlg.ExtractSemantic->isChecked());
		fac->FindParam<RParamValue>("UseDefaultNamespace")->SetBool(dlg.UseDefaultNamespace->isChecked());
		fac->FindParam<RParamValue>("DefaultNamespace")->Set(FromQString(dlg.DefaultNamespace->text()));
		fac->FindParam<RParamValue>("ExtractMetadata")->SetBool(dlg.ExtractMetadata->isChecked());
		fac->FindParam<RParamValue>("MetadataMaxTerms")->SetUInt(dlg.MetadataMaxTerms->value());
		fac->FindParam<RParamValue>("MetadataMaxDepth")->SetUInt(dlg.MetadataMaxDepth->value());
		fac->FindParam<RParamValue>("MetadataChildTags")->SetBool(dlg.MetadataChildTags->isChecked());
		fac->FindParam<RParamValue>("MetadataMaxOccurs")->SetUInt(dlg.MetadataMaxOccurs->value());
		fac->FindParam<RParamValue>("DetectMetadataTag")->SetBool(dlg.DetectMetadataTag->isChecked());

		// Metadata
		RParamList* MetadataTags(fac->FindParam<RParamList>("MetadataTags"));
		MetadataTags->Reset();
		for(int i=0;i<dlg.MetadataTags->count();i++)
		{
			QListWidgetItem* item(dlg.MetadataTags->item(i));
			if(!item) continue;
			MetadataTags->Insert(FromQString(item->text()));
		}

		// Exclude
		RParamList* ExcludeTags(fac->FindParam<RParamList>("ExcludeTags"));
		ExcludeTags->Reset();
		for(int i=0;i<dlg.ExcludeTags->count();i++)
		{
			QListWidgetItem* item(dlg.ExcludeTags->item(i));
			if(!item) continue;
			ExcludeTags->Insert(FromQString(item->text()));
		}

		return(true);
	}
	return(false);
}
