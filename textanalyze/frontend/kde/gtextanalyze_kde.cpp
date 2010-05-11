/*

	GALILEI Research Project

	GTextAnalyse_KDE.cpp

	Analyze a document (KDE Part) - Implementation.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
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



//-----------------------------------------------------------------------------
// include files for current project
#include <gtextanalyze_kde.h>



//------------------------------------------------------------------------------
//
// class Config
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Config::Config(void)
	: KDialog(), Ui_Config()
{
	setCaption("Configure XML Analyzer Plug-In");
	QWidget* widget=new QWidget(this);
	setupUi(widget);
	setMainWidget(widget);
	setButtons(KDialog::Cancel|KDialog::Apply);
	connect(this,SIGNAL(applyClicked()),this,SLOT(accept()));
	connect(ExtractStruct,SIGNAL(toggled(bool)),this,SLOT(toggleExtractStruct(bool)));
	connect(FullIndex,SIGNAL(toggled(bool)),this,SLOT(toggleFullIndex(bool)));
	connect(ExtractIndex,SIGNAL(clicked()),this,SLOT(clickedMetadata()));
	connect(DetectMetaTag,SIGNAL(clicked()),this,SLOT(clickedMetadata()));
	connect(AddTag,SIGNAL(clicked()),this,SLOT(addTag()));
	connect(RemoveTag,SIGNAL(clicked()),this,SLOT(removeTag()));
	adjustSize();
}


//------------------------------------------------------------------------------
void Config::toggleExtractStruct(bool toggle)
{
	StructIsContent->setEnabled(toggle);
	WeightStruct->setEnabled(toggle && StructIsContent->isChecked());
	ExtractValues->setEnabled(toggle);
	WeightValues->setEnabled(toggle && ExtractValues->isChecked());
	FullIndex->setEnabled(toggle);
}


//------------------------------------------------------------------------------
void Config::toggleFullIndex(bool toggle)
{
	if(toggle)
	{
		ExtractValues->setEnabled(false);
		ExtractValues->setChecked(true);
		WeightValues->setEnabled(true);
	}
	else
		ExtractValues->setEnabled(true);
}


//------------------------------------------------------------------------------
void Config::clickedMetadata(void)
{
	MaxTerms->setEnabled(DetectMetaTag->isChecked()&ExtractIndex->isChecked());
	MaxDepth->setEnabled(DetectMetaTag->isChecked()&ExtractIndex->isChecked());
	ChildTags->setEnabled(DetectMetaTag->isChecked()&ExtractIndex->isChecked());
	MaxOccurs->setEnabled(DetectMetaTag->isChecked()&ExtractIndex->isChecked());
	DetectMetaTag->setEnabled(ExtractIndex->isChecked());
	IndexTags->setEnabled((!DetectMetaTag->isChecked())&ExtractIndex->isChecked());
	AddTag->setEnabled((!DetectMetaTag->isChecked())&ExtractIndex->isChecked());
	RemoveTag->setEnabled((!DetectMetaTag->isChecked())&ExtractIndex->isChecked());
}


//------------------------------------------------------------------------------
void Config::addTag(void)
{
	bool ok;
	QString tag(QInputDialog::getText(this,tr("Add a Meta Tag"),tr("Tag:"),QLineEdit::Normal,"",&ok));
	if(ok&&(!tag.isEmpty()))
	{
		IndexTags->addItem(tag);
		RemoveTag->setEnabled(true);
	}
}


//------------------------------------------------------------------------------
void Config::removeTag(void)
{
	int row(IndexTags->currentRow());
	if(row!=-1)
	{
		QListWidgetItem* item(IndexTags->takeItem(row));
		delete item;
	}
	if(!IndexTags->count())
		RemoveTag->setEnabled(false);
}



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "text", 0, ki18n("Text Analyzer"),
		"1.0", ki18n("This is a text analyzer for documents."), KAboutData::License_GPL,
		ki18n("(C) 2003-2010 by Pascal Francq\n(C) 2003-2008 by Université Libre de Bruxelles (ULB)"),
		KLocalizedString(), "http://www.imrdp.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	aboutData.addCredit(ki18n("Valery Vandaele"),ki18n("Contributor"));
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
	Config dlg;

	// Stems
	dlg.StaticLang->setChecked(fac->FindParam<RParamValue>("StaticLang")->GetBool());
	dlg.MinStopWords->setValue(fac->FindParam<RParamValue>("MinStopWords")->GetDouble());
	dlg.MinWordSize->setValue(fac->FindParam<RParamValue>("MinWordSize")->GetInt());
	dlg.NonLetterWords->setChecked(fac->FindParam<RParamValue>("NonLetterWords")->GetBool());
	dlg.MaxTerms->setEnabled(fac->FindParam<RParamValue>("DetectMetaTag")->GetBool()&&fac->FindParam<RParamValue>("ExtractIndex")->GetBool());
	dlg.MaxDepth->setEnabled(fac->FindParam<RParamValue>("DetectMetaTag")->GetBool()&&fac->FindParam<RParamValue>("ExtractIndex")->GetBool());
	dlg.ChildTags->setEnabled(fac->FindParam<RParamValue>("DetectMetaTag")->GetBool()&&fac->FindParam<RParamValue>("ExtractIndex")->GetBool());
	dlg.MaxOccurs->setEnabled(fac->FindParam<RParamValue>("DetectMetaTag")->GetBool()&&fac->FindParam<RParamValue>("ExtractIndex")->GetBool());
	dlg.DetectMetaTag->setEnabled(fac->FindParam<RParamValue>("DetectMetaTag")->GetBool()&&fac->FindParam<RParamValue>("ExtractIndex")->GetBool());
	dlg.Filtering->setChecked(fac->FindParam<RParamValue>("Filtering")->GetBool());
	dlg.NbSameOccur->setValue(fac->FindParam<RParamValue>("NbSameOccur")->GetInt());
	dlg.NormalRatio->setValue(fac->FindParam<RParamValue>("NormalRatio")->GetDouble());
	dlg.NbSameOccur->setEnabled(fac->FindParam<RParamValue>("Filtering")->GetBool());
	dlg.NormalRatio->setEnabled(fac->FindParam<RParamValue>("Filtering")->GetBool());
	dlg.groupFiltering->setEnabled(fac->FindParam<RParamValue>("NonLetterWords")->GetBool());

	// Structure - Tags and attributes
	dlg.ExtractStruct->setChecked(fac->FindParam<RParamValue>("ExtractStruct")->GetBool());
	dlg.FullIndex->setChecked(fac->FindParam<RParamValue>("FullIndex")->GetBool());
	dlg.StructIsContent->setChecked(fac->FindParam<RParamValue>("StructIsContent")->GetBool());
	dlg.WeightStruct->setValue(fac->FindParam<RParamValue>("WeightStruct")->GetDouble());
	dlg.ExtractValues->setChecked(fac->FindParam<RParamValue>("ExtractValues")->GetBool());
	dlg.WeightValues->setValue(fac->FindParam<RParamValue>("WeightValues")->GetDouble());
	dlg.toggleExtractStruct(fac->FindParam<RParamValue>("ExtractStruct")->GetBool());

	// Structure - Declarative tags
	dlg.UseDefaultNamespace->setChecked(fac->FindParam<RParamValue>("UseDefaultNamespace")->GetBool());
	dlg.DefaultNamespace->setEnabled(fac->FindParam<RParamValue>("UseDefaultNamespace")->GetBool());
	dlg.DefaultNamespace->setText(ToQString(fac->FindParam<RParamValue>("DefaultNamespace")->Get()));
	dlg.ExtractIndex->setChecked(fac->FindParam<RParamValue>("ExtractIndex")->GetBool());
	dlg.DetectMetaTag->setChecked(fac->FindParam<RParamValue>("DetectMetaTag")->GetBool());
	dlg.MaxTerms->setValue(fac->FindParam<RParamValue>("MaxTerms")->GetInt());
	dlg.MaxDepth->setValue(fac->FindParam<RParamValue>("MaxDepth")->GetInt());
	dlg.MaxOccurs->setValue(fac->FindParam<RParamValue>("MaxOccurs")->GetInt());
	dlg.ChildTags->setChecked(fac->FindParam<RParamValue>("ChildTags")->GetBool());

	// Index
	RCursor<RString> Tags(fac->FindParam<RParamList>("IndexTags")->GetList());
	for(Tags.Start();!Tags.End();Tags.Next())
		dlg.IndexTags->addItem(ToQString(*Tags()));
	if(!dlg.IndexTags->count())
		dlg.RemoveTag->setEnabled(false);
	dlg.clickedMetadata();

	if(dlg.exec())
	{
		// Stems
		fac->FindParam<RParamValue>("StaticLang")->SetBool(dlg.StaticLang->isChecked());
		fac->FindParam<RParamValue>("MinStopWords")->SetDouble(dlg.MinStopWords->value());
		fac->FindParam<RParamValue>("MinWordSize")->SetUInt(dlg.MinWordSize->value());
		fac->FindParam<RParamValue>("NonLetterWords")->SetBool(dlg.NonLetterWords->isChecked());
		fac->FindParam<RParamValue>("Filtering")->SetBool(dlg.Filtering->isChecked());
		fac->FindParam<RParamValue>("NbSameOccur")->SetUInt(dlg.NbSameOccur->value());
		fac->FindParam<RParamValue>("NormalRatio")->SetDouble(dlg.NormalRatio->value());

		// Structure - Tags and attributes
		fac->FindParam<RParamValue>("ExtractStruct")->SetBool(dlg.ExtractStruct->isChecked());
		fac->FindParam<RParamValue>("FullIndex")->SetBool(dlg.FullIndex->isChecked());
		fac->FindParam<RParamValue>("StructIsContent")->SetBool(dlg.StructIsContent->isChecked());
		fac->FindParam<RParamValue>("WeightStruct")->SetDouble(dlg.WeightStruct->value());
		fac->FindParam<RParamValue>("ExtractValues")->SetBool(dlg.ExtractValues->isChecked());
		fac->FindParam<RParamValue>("WeightValues")->SetDouble(dlg.WeightValues->value());

		// Structure - Tags and attributes
		fac->FindParam<RParamValue>("UseDefaultNamespace")->SetBool(dlg.UseDefaultNamespace->isChecked());
		fac->FindParam<RParamValue>("DefaultNamespace")->Set(FromQString(dlg.DefaultNamespace->text()));
		fac->FindParam<RParamValue>("ExtractIndex")->SetBool(dlg.ExtractIndex->isChecked());
		fac->FindParam<RParamValue>("MaxTerms")->SetUInt(dlg.MaxTerms->value());
		fac->FindParam<RParamValue>("MaxDepth")->SetUInt(dlg.MaxDepth->value());
		fac->FindParam<RParamValue>("ChildTags")->SetBool(dlg.ChildTags->isChecked());
		fac->FindParam<RParamValue>("MaxOccurs")->SetUInt(dlg.MaxOccurs->value());
		fac->FindParam<RParamValue>("DetectMetaTag")->SetBool(dlg.DetectMetaTag->isChecked());

		// Index
		RParamList* IndexTags(fac->FindParam<RParamList>("IndexTags"));
		IndexTags->Reset();
		for(int i=0;i<dlg.IndexTags->count();i++)
		{
			QListWidgetItem* item(dlg.IndexTags->item(i));
			if(!item) continue;
			IndexTags->Insert(FromQString(item->text()));
		}
		return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
}     // End of extern
//------------------------------------------------------------------------------

