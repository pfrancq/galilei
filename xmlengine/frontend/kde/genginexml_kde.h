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

//------------------------------------------------------------------------------
// include files for current project

using namespace std;

#include <dirent.h>
#include <iostream>
#include <kdialog.h>
#include <rstring.h>
#include <rcontainer.h>
#include <ui_genginexml.h>
//#include <ui_config.h>
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


