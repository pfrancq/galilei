/*

	GALILEI Research Project

	QGObjectsList.h

	Widget to manipulate a list of objects - Header.

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
#ifndef QGobjectsListH
#define QGobjectsListH


//------------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QTreeWidget>


//------------------------------------------------------------------------------
// include files for R/GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The QGObjectsList provides a widget to list a given set of objects. When the
 * user double clicks on one object, the widget emits a corresponding signal.
 * @short Objects List Widget
 * @author Pascal Francq
 */
class QGObjectsList : public QWidget
{
public:

	/**
	 * Type of the list managed by the widget.
	 */
	enum oType
	{
		Nothing,            /** Nothing populated.*/
		Docs,               /** Documents are listed. */
		Links,              /** Links are listed. */
		Topics,             /** Topics are listed. */
		CompTopics,         /** Topics with their quality are listed. */
		IdealTopics,        /** Ideal topics are listed. */
		Users,              /** Users are listed. */
		Profiles,           /** Profiles are listed. */
		Communities,        /** Communities are listed.*/
		CompCommunities,    /** Communities with their quality are listed.*/
		IdealCommunities,   /** Ideal communities are listed.*/
		Assessments,        /** Assessments are listed. */
		Subjects,           /** Subjects are listed.*/
		Classes             /** Classes are listed.*/
	};

private:

	Q_OBJECT

	/**
	 * Pointer to the widget created by Qt.
	 */
	void* Ui;

public:

	/**
	 * Construct the list.
	 * @param parent         Parent widget.
	 */
	QGObjectsList(QWidget* parent=0);

	/**
	 * Set the list with a given type.
	 * @param session        Session.
	 * @param type           Type of the list.
	 */
	void Set(GSession* session,oType type);

	/**
	 * Set the list with a given type for a given document.
	 * @param type           Type of the list.
	 * @param doc            Document.
	 */
	void Set(oType type,GDoc* doc);

	/**
	 * Set the list with a given type for a given profile.
	 * @param type           Type of the list.
	 * @param profile        Profile.
	 */
	void Set(oType type,GProfile* profile);

	/**
	 * Set the list with a given type for a given topic.
	 * @param type           Type of the list.
	 * @param topic            Topic.
	 */
	void Set(oType type,GTopic* topic);

	/**
	 * Set the list with a given type for a given community.
	 * @param type           Type of the list.
	 * @param community      Community.
	 */
	void Set(oType type,GCommunity* community);

	/**
	 *  Set the list for a given engine.
	 * @param type           Type of the list.
	 * @param engine         Engine.
	 * @param nbres          Number of results to show.
	 */
	void Set(oType type,GMetaEngine* engine,size_t nbres);

	/**
	 * Get the tree.
	 * @return Pointer.
	 */
	QTreeWidget* tree(void);

	/**
	 * Get the current item.
	 * @return Pointer.
	 */
	QTreeWidgetItem* currentItem(void);

	/**
	* Get the current user selected in this window.
	* @returns Pointer to GUser or 0 if no user is currently selected.
	*/
	GUser* GetCurrentUser(void);

	/**
	* Get the current profile selected in this window.
	* @returns Pointer to GProfile or 0 if no profile is currently selected.
	*/
	GProfile* GetCurrentProfile(void);

	/**
	* Get the current document selected in this window.
	* @returns Pointer to GDoc or 0 if no document is currently selected.
	*/
	GDoc* GetCurrentDoc(void);

	/**
	* Get the current topic selected in this window.
	* @returns Pointer to GTopic or 0 if no topic is currently selected.
	*/
	GTopic* GetCurrentTopic(void);

	/**
	* Get the current community selected in this window.
	* @returns Pointer to GCommunity or 0 if no community is currently selected.
	*/
	GCommunity* GetCurrentCommunity(void);

	/**
	 * Find the next item containing a given string.
	 * @param what           Text to search for.
	 * @param desc           Search (eventually) in the descriptions.
	 */
	void FindNext(const QString& what,bool desc);

private:

	/**
	 * Function that transform an extract in a string that can be printed.
    * @param extract        Extract to print.
    * @return a QString.
    */
	QString PrintExtract(const R::RString& extract);

	/**
	 * Destruct the list.
	 * @return
	 */
	virtual ~QGObjectsList(void);

private slots:

	/**
	 * This slot is called when the user double clicks on a given item.
	 * @param item           Item chosen.
	 * @param column         Corresponding column.
	 */
	void HandleItem(QTreeWidgetItem* item,int column);

signals:

	/**
	 * A document must be shown.
	 */
	void Show(GDoc* doc);

	/**
	 * A topic must be shown.
	 */
	void Show(GTopic* topic);

	/**
	 * A profile must be shown.
	 */
	void Show(GProfile* profile);

	/**
	 * A community must be shown.
	 */
	void Show(GCommunity* community);

	/**
	 * A class must be shown.
	 */
	void Show(GClass* theclass);
};


} //------- End of namespace GALILEI ------------------------------------------


//------------------------------------------------------------------------------
#endif
