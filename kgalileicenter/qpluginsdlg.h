/*

  qpluginsdlg.h

  Description - Header.

  (c) 2002 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
#ifndef QPluginsDlgH
#define QPluginsDlgH


//-----------------------------------------------------------------------------
// include file for GALILEI
#include <galilei.h>
namespace GALILEI
{
	class QProfileDescPluginConf;
	class QLinkCalcPluginConf;
}
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qdialog.h>
class QWidget;
class QTabWidget;
class QPushButton;
class QVBoxLayout;
class QListView;
class QListViewItem;
class QTextView;


//-----------------------------------------------------------------------------
// include files for current application
class KGALILEICenterApp;


//-----------------------------------------------------------------------------
/**
* The QPluginsDlg provides a dialog box for managing the plugins of galilei.
* @author Pascal Francq
* @short Plugins Dialog Box.
*/
class QPluginsDlg : public QDialog
{
	class QPluginListView;

	Q_OBJECT

	/**
	* Main Widget to handle the different plugins.
	*/
	QTabWidget* Main;

	/**
	* Current plugin selected in the listview.
	*/
	QPluginListView* Cur;

	/**
	* Current Configuration Widget.
	*/
	QWidget* Conf;

	/**
	* Widget to show when the plugin has nothing to configure.
	*/
	QTextView* NoConf;

	/**
	* List of all plugins.
	*/
	QListView* Plugins;

	/**
	* Apply button to make the changin in the configuration effective.
	*/
	QPushButton* buttonApply;


	/**
	* List of all configuration for the link description method.
	*/
	R::RContainer<QLinkCalcPluginConf,unsigned int,true,true>* LinkCalcs;

	/**
	* Application to condfigure.
	*/
	KGALILEICenterApp* App;

public:

	/**
	* Constructor.
	* @param app            Application to configure.
	* @param name           Name of the dialog box.
	*/
	QPluginsDlg(KGALILEICenterApp* app,const char* name=0);

	/**
	* Register a plugin to configure a link description method.
	*/
	void RegisterLinkCalcPluginConf(QLinkCalcPluginConf* ins) throw(bad_alloc);

	/**
	* Construct the list of plugins.
	*/
	void ConstructPlugins(void) throw(bad_alloc);

signals:

	/**
	* Signal emit when the plugins must aplly the current configuration.
	*/
	void signalApply(void);

private slots:

	/**
	* Slot called when a plugin is selected in the list of plugins.
	* @param item           Item selected.
	*/
	void slotPlugin(QListViewItem* item);

public:

	/**
	* Destructor.
	*/
	~QPluginsDlg(void);
};


//-----------------------------------------------------------------------------
#endif
