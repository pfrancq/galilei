/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include <qfiledialog.h>
#include <qthread.h>
#include <dirent.h>
#include <iostream>
#include <kapplication.h>
using namespace std;

#include <rstring.h>
#include <rcontainer.h>
using namespace R;

//DlgProgress_Qt *ptr_dlgprog = 0;
/*
void set_ptr_dlgprog(DlgProgress_Qt *_ptr_dlgprog)
{
	ptr_dlgprog = _ptr_dlgprog;
}
*
//______________________________________________________________________________
//------------------------------------------------------------------------------
void fct_print(const char *str)
{
	if (ptr_dlgprog)
		ptr_dlgprog->AddLine(str);
}
*/
//______________________________________________________________________________
//------------------------------------------------------------------------------
void DlgConfig_Qt::init()
{
	pathLView->setColumnWidthMode(0, QListView::Maximum);
	pathLView->setColumnWidthMode(1, QListView::Manual);
	pathLView->setColumnWidth(0, 430);
	pathLView->setColumnWidth(1, 90);
	pathLView->setColumnAlignment(1, AlignHCenter);
	pathLEdit->setText("/home/fgaultier/Documents/CollectionsXML/Inex06/Collection/essaiJB1doc/");
	OnUpdate();
	CheckLView();
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void DlgConfig_Qt::AddFolder()
{
	QListViewItem *lvitem;
	QString path(pathLEdit->text());
	
	if (path[path.length() - 1] != '/')
	{
		path += "/";
		pathLEdit->setText(path);
	}
	lvitem = pathLView->findItem(pathLEdit->text(), 0, CaseSensitive);
	if (lvitem != 0)
			lvitem->setText(1, subfCBox->isChecked() ? "x" : " ");
	else
	{
		lvitem = new QListViewItem(pathLView);
		lvitem->setText(0, pathLEdit->text());
		lvitem->setText(1, subfCBox->isChecked() ? "x" : " ");
		pathLView->insertItem(lvitem);
	}
	CheckLView();
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void DlgConfig_Qt::RemoveFolder()
{
	pathLView->takeItem(pathLView->selectedItem());
	CheckLView();
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void DlgConfig_Qt::OpenDir()
{
	QFileDialog *fdlg;
	QString pathres;
	
	fdlg = new QFileDialog(this);
	fdlg->setDir(pathLEdit->text());
	fdlg->setMode(QFileDialog::Directory);
//	fdlg->setFilter("*");
	if (fdlg->exec() == QDialog::Accepted)
		pathres = fdlg->selectedFile();
	pathLEdit->setText(pathres);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void DlgConfig_Qt::CheckLView()
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
	remButton->setEnabled(pathLView->childCount());
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void DlgConfig_Qt::EditLine()
{
	CheckLView();
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void DlgConfig_Qt::SelectItem()
{
	if (pathLView->selectedItem())
		pathLEdit->setText(pathLView->selectedItem()->text(0));
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void DlgConfig_Qt::OnOk()
{
	update = updateCBox->isChecked();
	reset = resetCBox->isChecked();
	accept();
/*
	QListViewItem *lvitem;

	XAnalyser analyser(fct_print);
	
	dlg_prog = new DlgProgress_Qt(this, 0, 0, (Qt::WStyle_Customize | Qt::WStyle_Title) & ~Qt::WStyle_SysMenu);
	set_ptr_dlgprog(dlg_prog);
	
	dlg_prog->setModal(true);
	dlg_prog->show();
	KApplication::kApplication()->processEvents();
	for (lvitem = pathLView->firstChild(); lvitem != 0; lvitem = lvitem->nextSibling())
		analyser.AddPath(RString(lvitem->text(0).latin1()), lvitem->text(1) == "x");
	analyser.Update();
	dlg_prog->closeButton->setEnabled(true);
	KApplication::kApplication()->processEvents();*/
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void DlgConfig_Qt::ModalClosed()
{
/*	nbres = nbresSBox->value();
	weight = weightSBox->value() / 100.0;
	dlg_prog->close();
	delete dlg_prog;
	accept();*/
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void DlgConfig_Qt::SetUpdate_Reset(bool state_update, bool state_reset)
{
	updateCBox->setChecked(state_update);
	resetCBox->setChecked(state_reset);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
char *DlgConfig_Qt::GetAllPaths()
{
	QListViewItem *lvitem;
	RString temp;
	RString car[2] = {"0", "1"};
	char *res;
	
	for (lvitem = pathLView->firstChild(); lvitem != 0; lvitem = lvitem->nextSibling())
		temp += RString(car[lvitem->text(1) == "x"] + lvitem->text(0).latin1()) + ".";
	res = new char[temp.GetLen() + 1];
	sprintf(res, "%s", temp.Latin1());
	return res;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void DlgConfig_Qt::ExternAddPath(const char *path, bool inc_subf)
{
	DIR *dir;
	QListViewItem *lvitem;
	
	dir = opendir(path);
	if (dir != 0)
	{
		closedir(dir);
		lvitem = new QListViewItem(pathLView);
		lvitem->setText(0, path);
		lvitem->setText(1, inc_subf ? "x" : " ");
		pathLView->insertItem(lvitem);
	}
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void DlgConfig_Qt::OnUpdate()
{
	if (updateCBox->isChecked())
		resetCBox->setEnabled(true);
	else
		resetCBox->setEnabled(false);
}
