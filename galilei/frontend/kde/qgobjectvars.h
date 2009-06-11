/*

	GALILEI Research Project

	QGObjectVars.h

	Widget to show the variables of a given object - Header.

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
#ifndef QGOjectVarsH
#define QGOjectVarsH


//------------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QWidget>


//------------------------------------------------------------------------------
// include files for R/GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GGObjectVars provides a widget to manage the information of a given
 * object.
 * @short Object Information Widget
 * @author Pascal Francq
 */
class QGObjectVars : public QWidget
{
	/**
	 * Pointer to the widget created by Qt.
	 */
	void* Ui;

public:

	/**
	 * Construct the widget.
	 * @param parent         Parent.
	 */
	QGObjectVars(QWidget* parent);

	/**
	 * Put the variables of a given document.
	 * @param doc            Document.
	 */
	void Set(GDoc* doc);

	/**
	 * Put the variables of a given topic.
	 * @param topic          Topic.
	 */
	void Set(GTopic* topic);

	/**
	 * Put the variables of a given community.
	 * @param community      Community.
	 */
	void Set(GCommunity* community);

	/**
	 * Put the variables of a given profile.
	 * @param profile        Profile.
	 */
	void Set(GProfile* profile);

private:

	/**
	 * Add a variable.
	 * @param var            Name of the variable.
	 * @param value          Value of the variable.
	 */
	void AddVar(const QString& var,const R::RString& value);

	/**
	 * Add a variable.
	 * @param var            Name of the variable.
	 * @param value          Value of the variable.
	 */
	void AddVar(const QString& var,size_t value);

	/**
	 * Add a variable.
	 * @param var            Name of the variable.
	 * @param value          Value of the variable.
	 */
	void AddVar(const QString& var,const GLang* value);

	/**
	 * Add a variable.
	 * @param var            Name of the variable.
	 * @param value          Value of the variable.
	 */
	void AddVar(const QString& var,bool value);

	/**
	 * Add a variable.
	 * @param var            Name of the variable.
	 * @param value          Value of the variable.
	 */
	void AddVar(const QString& var,const tObjState value);

public:

	/**
	 * Destruct the widget.
	 */
	virtual ~QGObjectVars(void);
};



} //------- End of namespace GALILEI ------------------------------------------

//------------------------------------------------------------------------------
#endif
