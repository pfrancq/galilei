/*

	GALILEI Research Project

	KGAStat.h

	Window for the GA - Header.

	(C) 2002 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

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
#ifndef KViewGAH
#define KViewGAH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rga/rgasignals.h>
#include <rga/qgamonitor.h>
#include <rga/qxmlcontainer.h>
using namespace RGA;
#include <rpromethee/rpromcriterion.h>
using namespace RPromethee;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gir.h>
#include <groups/gchromoir.h>
#include <groups/ginstir.h>
#include <groups/ggroupinggga.h>
#include <galilei/qggroupsir.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>
#include <qtabwidget.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewGA class provides the view widget for a genetic algorithm used for
* the groupment of the profile.
* @author Pascal Francq.
* @version $Revision$
* @short Window for GA.
*/
class KViewGA : public KView, public RGASignalsReceiver<GInstIR,GChromoIR,GFitnessIR>
{
	Q_OBJECT

	/**
	* The tab widget.
	*/
	QTabWidget* TabWidget;
	
	/**
	* The widget that handle statistics about the running GA.
	*/
	QGAMonitor* Monitor;
	
	/**
	* The widget that handle the debug information of the running GA.
	*/
	QXMLContainer* Debug;
	
	/**
	* The splitter that separate the monitor and the debug part.
	*/
	QSplitter* StatSplitter;
	
	/**
	* The best solution.
	*/
	QGGroupsIR* Best;
	
	/**
	* The solutions.
	*/
	QGGroupsIR* Sol;

	/**
	* The ideal solution.
	*/
	QGGroupsIR* Ideal;
	
	/**
	* Identificator of the current showed solution.
	*/
	unsigned int CurId;

	/**
	* The GA that will be used.
	*/
	GInstIR* Instance;

	/**
	* Number of generation already executed.
	*/
	unsigned int Gen;

	/**
	* Maximal number of generation to execute.
	*/
	unsigned int MaxGen;

	/**
	* Number of generation to execute at each step.
	*/
	unsigned int StepGen;

	/**
	* Execute GA by steps.
	*/
	bool Step;

	/**
	* Minimum similarity level between the profiles of a group.
	*/
	double MinSimLevel;

	/**
	* Minimum number of common OK documents needed to force two profiles to be
	* in the same group.
	*/
	double MinCommonOK;

	/**
	* Minimum number of common documents judged differently needed to force two
	* profiles to be in two different groups.
	*/
	double MinCommonDiff;

	/**
	* Size of the Population.
	*/
	unsigned int PopSize;

	/**
	* Parameter for the criterion "Similariry".
	*/
	RPromethee::RPromCriterionParams ParamsSim;

	/**
	* Parameter for the criterion "NB Profiles".
	*/
	RPromethee::RPromCriterionParams ParamsNb;

	/**
	* Parameter for the criterion "OK Factor".
	*/
	RPromethee::RPromCriterionParams ParamsOK;

	/**
	* Parameter for the criterion "Diff Factor".
	*/
	RPromethee::RPromCriterionParams ParamsDiff;

	/**
	* Parameter for the criterion "Diff Social".
	*/
	RPromethee::RPromCriterionParams ParamsSocial;

	/**
	* SubProfiles that must be grouped again.
	*/
	RStd::RContainer<GSubProfile,unsigned int,false,true>* SubProfiles;

	/**
	* Objects representing SubProfiles that must be grouped again.
	*/
	RGA::RObjs<GObjIR>* Objs;

	/**
	* Similarities between the subprofiles to group.
	*/
	GProfilesSim* Sims;

	/**
	* Ideal Groups handled by the system.
	*/
	RStd::RContainer<GGroups,unsigned int,true,true> IdealGroups;

public:

	/**
	* Constructor for the view.
	* @param doc            Document instance that the view represents.
	* @param l              Lang to group.
	* @param global         Global Similarities.
	* @param scratch        Compute the grouping from scratch.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewGA(KDoc* doc,const char* l,bool global,bool scratch,QWidget* parent,const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gGA);}

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

	/**
	* GA signal to indicate that a new generation has been done.
	*/
	virtual void receiveGenSig(GenSig* sig);
	
	/**
	* GA signal to interact with the system.
	*/
	virtual void receiveInteractSig(InteractSig* sig);

	/**
	* GA signal to signify that the best chromosome has changed.
	*/
	virtual void receiveBestSig(BestSig *sig);

	/**
	* Run the GA.
	*/
	void RunGA(void);

	/**
	* Pause the GA.
	*/
	void PauseGA(void);

	/**
	* Stop the GA.
	*/
	void StopGA(void);

protected:

	/**
	* Key release event method. The implementation is needed to change the
	* chromosome to show when the user uses the keys "Page Up" and "Page Down".
	* With "Ctrl-G", a dialog box appears to select directly the identificator.
	*/
	virtual void keyReleaseEvent(QKeyEvent* e);

	/**
	* The function that handle the resize event.
	*/
	virtual void resizeEvent(QResizeEvent*);

signals:

	/**
	* Signal to emit after a generation is done.
	*/
	void signalSetGen(const unsigned int gen,const unsigned int best,const double value);

public:

	/**
	* Destructor for the view.
	*/
	virtual ~KViewGA(void);
};


//-----------------------------------------------------------------------------
#endif
