/*

	GALILEI Research Project

	QGObjectVars.cpp

	Widget to show the variables of a given object - Implementation.

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



//------------------------------------------------------------------------------
// include files for Qt/QKDE


//------------------------------------------------------------------------------
// include files for R/GALILEI
#include <rqt.h>
#include <glang.h>
#include <gtopic.h>
#include <gdoc.h>
#include <gprofile.h>
#include <gcommunity.h>
#include <qgobjectvars.h>
#include <ui_qgobjectvars.h>
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class QGObjectVars
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QGObjectVars::QGObjectVars(QWidget* parent)
	: QWidget(parent), Ui(new Ui_QGObjectVars())
{
	static_cast<Ui_QGObjectVars*>(Ui)->setupUi(this);
}


//------------------------------------------------------------------------------
void QGObjectVars::Set(GDoc* doc)
{
	if(!doc) return;
	static_cast<Ui_QGObjectVars*>(Ui)->Vars->clear();
	AddVar("Identifier",doc->GetId());
	AddVar("URL",doc->GetURL()());
	AddVar("Name",doc->GetName());
	AddVar("MIME",doc->GetMIMEType());
	AddVar("Language",doc->GetLang());
	AddVar("Last Updated",doc->GetUpdated());
	AddVar("Last Computed",doc->GetComputed());
	AddVar("Last Attached",doc->GetAttached());
	AddVar("State",doc->GetState());
	static_cast<Ui_QGObjectVars*>(Ui)->Vars->resizeColumnToContents(0);
	static_cast<Ui_QGObjectVars*>(Ui)->Vars->resizeColumnToContents(1);
}


//------------------------------------------------------------------------------
void QGObjectVars::Set(GTopic* topic)
{
	AddVar("Identifier",topic->GetId());
	AddVar("Last Updated",topic->GetUpdated());
	AddVar("Last Computed",topic->GetComputed());
	AddVar("Status",topic->GetState());
	static_cast<Ui_QGObjectVars*>(Ui)->Vars->resizeColumnToContents(0);
	static_cast<Ui_QGObjectVars*>(Ui)->Vars->resizeColumnToContents(1);
}


//------------------------------------------------------------------------------
void QGObjectVars::Set(GCommunity* community)
{
	AddVar("Identifier",community->GetId());
	AddVar("Last Updated",community->GetUpdated());
	AddVar("Last Computed",community->GetComputed());
	AddVar("Status",community->GetState());
	static_cast<Ui_QGObjectVars*>(Ui)->Vars->resizeColumnToContents(0);
	static_cast<Ui_QGObjectVars*>(Ui)->Vars->resizeColumnToContents(1);
}


//------------------------------------------------------------------------------
void QGObjectVars::Set(GProfile* profile)
{
	AddVar("Identifier",profile->GetId());
	AddVar("Social",profile->IsSocial());
	AddVar("Last Updated",profile->GetUpdated());
	AddVar("Last Computed",profile->GetComputed());
	AddVar("Last Attached",profile->GetAttached());
	AddVar("Status",profile->GetState());
	static_cast<Ui_QGObjectVars*>(Ui)->Vars->resizeColumnToContents(0);
	static_cast<Ui_QGObjectVars*>(Ui)->Vars->resizeColumnToContents(1);
}


//------------------------------------------------------------------------------
void QGObjectVars::AddVar(const QString& var,const RString& value)
{
	new QTreeWidgetItem(static_cast<Ui_QGObjectVars*>(Ui)->Vars,QStringList()<<var<<ToQString(value));
}


//------------------------------------------------------------------------------
void QGObjectVars::AddVar(const QString& var,size_t value)
{
	new QTreeWidgetItem(static_cast<Ui_QGObjectVars*>(Ui)->Vars,QStringList()<<var<<QString::number(value));
}


//------------------------------------------------------------------------------
void QGObjectVars::AddVar(const QString& var,const GLang* value)
{
	new QTreeWidgetItem(static_cast<Ui_QGObjectVars*>(Ui)->Vars,QStringList()<<var<<((value)?ToQString(value->GetName()):"?????"));
}


//------------------------------------------------------------------------------
void QGObjectVars::AddVar(const QString& var,bool value)
{
	new QTreeWidgetItem(static_cast<Ui_QGObjectVars*>(Ui)->Vars,QStringList()<<var<<((value)?"Yes":"No"));
}


//------------------------------------------------------------------------------
void QGObjectVars::AddVar(const QString& var,const tObjState value)
{
	new QTreeWidgetItem(static_cast<Ui_QGObjectVars*>(Ui)->Vars,QStringList()<<var<<ToQString(GetState(value)));
}


//------------------------------------------------------------------------------
QGObjectVars::~QGObjectVars(void)
{
	delete static_cast<Ui_QGObjectVars*>(Ui);
}
