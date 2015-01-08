/*

	KGALILEICenter

	Configure.h

	Configure Dialog Box - Header.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef ConfigureDlgH
#define ConfigureDlgH


//-----------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <galilei.h>
using namespace GALILEI;


//------------------------------------------------------------------------------
// include files for Qt
#include <QDialog>


//------------------------------------------------------------------------------
// include files for current project
#include <ui_configure.h>
class QGALILEIWin;  // forward declaration


//------------------------------------------------------------------------------
/**
 * The Configure provides a dialog box to configure the different plug-ins for
 * the current session.
 * @short Configure Dialog.
 * @author Pascal Francq
 */
class Configure : QDialog, Ui_Configure
{
	Q_OBJECT

	class Type
	{
	public:
		GSession* Session;
		RString Name;
		size_t DescBlock;
		size_t DescTolerance;
		size_t DescCache;
		size_t DescType;
		bool CreateIndex;
		size_t IndexBlock;
		size_t IndexTolerance;
		size_t IndexCache;
		size_t IndexType;
		bool CreateTree;
		size_t TreeBlock;
		size_t TreeTolerance;
		size_t TreeCache;
		size_t TreeType;

		Type(GSession* session,const RString& name);
		void Apply(void);
		int Compare(const Type& type) const {return(Name.Compare(type.Name));}
		int Compare(const RString& type) const {return(Name.Compare(type));}
	};

	/**
	 * Object type.
	 */
	RContainer<Type,true,true> Types;

	/**
	 * Current type.
	 */
	Type* CurType;

	/**
	 * The main window.
	 */
	QGALILEIWin* Win;

public:

	/**
	 * Construct the dialog box.
	 * @param app            Application.
	 */
	Configure(QGALILEIWin* win);

	/**
	 * Execute the dialog boxes. In practice, when the 'Apply' button is
	 * pressed, the variables are applied.
	 */
	void exec(void);

	/**
	 * Read the options of a configuration file.
	 */
	static void readOptions(void);

	/**
	 * Save the options of a configuration file.
	 */
	static void saveOptions(void);

private:

	/**
	 * Initialize the session part.
	 */
	void initSession(void);

	/**
	 * Apply the session part.
	 */
	void applySession(void);

	/**
	 * Add a subject to the list of subjects.
	 * @param subject        Subject.
	 * @param parent         Parent intem (if null, the subject is a top one).
	 */
	void addSubject(GSubject* subject,QTreeWidgetItem* parent);

	/**
	 * Initialize the simulation part.
	 */
	void initSimulation(void);

	/**
	 * Apply the configuration for all sub-subjects of a given one.
	 * @param parent         Parent subject.
	 */
	void applySubSubjects(QTreeWidgetItem* parent);

	/**
	 * Apply the simulation part.
	 */
	void applySimulation(void);

	/**
	 * Initialize the plug-ins part.
	 */
	void initPlugIns(void);

	/**
	 * Apply the plug-ins part.
	 */
	void applyPlugIns(void);

public slots:

	/**
	 * Slot called when a subject is clicked.
	 * @param item           Item.
	 * @param column         Column.
	 */
	void subjectClicked(QTreeWidgetItem* item, int column);

	/**
	 * Method called when the object is changed.
	 * @param obj            Type of object changed.
	 */
	void objectChanged(const QString& text);
};



//------------------------------------------------------------------------------
#endif
