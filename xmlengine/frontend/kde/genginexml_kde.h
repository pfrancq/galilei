/*
 * genginexml_kde.h
 *
 *  Created on: 27-févr.-2009
 *      Author: fgaultier
 */

#ifndef GENGINEXML_KDE_H_
#define GENGINEXML_KDE_H_

//------------------------------------------------------------------------------
// include files for R/GALILEI
#include <rdb.h>
using namespace R;
//-----------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------
// include files for current project





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
using namespace std;

#include <dirent.h>
#include <iostream>
//#include <kapplication.h>
#include <rstring.h>
#include <rcontainer.h>
#include <ui_genginexml.h>
#include <QtCore/QObject>


class GEngineXML_KDE : public KDialog, public Ui_Config
{
	Q_OBJECT

public:

	GEngineXML_KDE(void);
	virtual ~GEngineXML_KDE(void);

public slots:


	void ExternAddPath(const char *path, bool inc_subf);
	char* GetAllPaths();
	virtual void OnUpdate();
	void CheckLView();
	void init();
	void SetUpdate_Reset(bool state_update, bool state_reset);
	void OnOk();
	void EditLine();
	virtual void AddFolder();
	void RemoveFolder();
	//------------------------------------------------------------------------------
	void OpenDir();
	void SelectItem();
};
//-----------------------------------------------------------------------------
#endif


