/*

	KGALILEICenter

	QPlugInsList.h

	Widget to configure a list of plug-ins - Header.

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).

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
#ifndef QPluginsListH
#define QPluginsListH


//-----------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <gsession.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// include files
#include <ui_qpluginslist.h>


//------------------------------------------------------------------------------
/**
 * The QPLugInsList provides a widget to manage the configuration of a set of
 * plug-ins.
 * @short Plug-Ins
 * @author Pascal Francq
 */
class QPlugInsList : public QWidget, public Ui_QPlugInsList
{
	Q_OBJECT

public:

	/**
	 * Type of plug-ins managed in the list.
	 */
	enum PlugInType
	{
		Storages,                   /** Storage.*/
		Filters,                    /** Filters.*/
		Langs,                      /** Languages.*/
		ProfileCalcs,               /** Profile Computing Methods.*/
		GroupProfiles,              /** Profiles Clustering Methods.*/
		GroupDocs,                  /** Document Clustering Methods.*/
		CommunityCalcs,             /** Community Computing Methods.*/
		TopicCalcs,                 /** Topic Computing Methods.*/
		LinkCalcs,                  /** Link Analysis Methods.*/
		Tokenizers,                 /** Tokenizer Method.*/
      Analyzers,                  /** Text Analyzers Methods. */
		Engines,                    /** Search Engines.*/
		MetaEngines,                /** Meta-search Engines.*/
		Measures,                   /** Measures for a given Category.*/
		ComputeSugs,                /** Compute Suggestions.*/
		ComputeTrust,               /** Compute Trust.*/
		Tools,                      /** Tools. */
      Functions                   /** Functions.*/
	};

private:

	/**
	 * Type of the plug-ins managed.
	 */
	PlugInType Type;

	/**
	 * Specify if a current methods must be chosen.
	 */
	bool HasCurrent;

	/**
	 * Specify if the methods can be ordered.
	 */
	bool HasUpDown;

	/**
	 * Category of the plug-ins managed (only for measures).
	 */
	RString Cat;

public:

	/**
	 * Construct the widget.
	 * @param parent         Parent widget.
	 */
	QPlugInsList(QWidget* parent);

private:

	/**
	 * Populate the widget with all the plug-ins.
	 * @param mng            Manager responsible for the plug-ins.
	 * @param current        Current plug-in may be chosen.
	 * @param enable         Plug-ins can be enabled/disabled.
	 * @param updown         Plug-ins can be ordered.
	 * @param list           List of plug-in (eventually).
	 */
	void init(const RString& mng,bool current,bool enable,bool updown,const RString& list=RString::Null);

public:

	/**
	 * Populate the widget with all the plug-ins of a given type.
	 * @param type           Type fo the plug-ins.
	 * @param cat            Category fo the plug-ins (only for Measures).
	 */
	void init(PlugInType type,const RString& cat=RString::Null);

	/**
	 * Apply the configuration of the plug-ins.
	 * @param session        Session.
	 */
	void apply(GSession* session);

public slots:

	/**
	 * Slot called when the Configure button is pressed.
	 */
	void slotConfigure(void);

	/**
	 * Slot called when the About button is pressed.
	 */
	void slotAbout(void);

	/**
	 * Slot called when a new plug-in is chosen in the list.
	 * @param act            New plug-in chosen.
	 * @param prev           Previous plug-in chosen.
	 */
	void slotChange(QListWidgetItem* act,QListWidgetItem* prev);

	/**
	 * Slot called when the Enable property is checked or unchecked.
	 * @param state          New status of the plug-in.
	 */
	void slotEnable(bool state);

	/**
	 * Slot called when the Up button is pressed.
	 */
	void slotUp(void);

	/**
	 * Slot called when the Down button is pressed.
	 */
	void slotDown(void);

	/**
	 * Slot called when the Push button is pressed.
	 */
	void slotParams(void);
};


//------------------------------------------------------------------------------
#endif
