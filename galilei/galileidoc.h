/*

	GALILEI Research Project

	GALILEIDoc.h

	GALILEI Documentation - Header.

	Copyright 2009-2014 by Pascal Francq (pascal@francq.info).

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
#ifndef GALILEIDOCH
#define GALILEIDOCH


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * \mainpage GALILEI Research Project
 *
 * \section intro_sec Introduction
 *
 * GALILEI is a research and development platform in the field of information
 * science.
 *
 * \section galilei_main_classes_sec Main classes
 * The main classes are:
 * - GGALILEIApp.
 * - GSession.
 *
 * A new application using the GALILEI platform should define a class
 * inheriting from GGALILEIApp. This class will manage the plug-ins and their
 * configurations. A global variable GALILEIApp is created.
 *
 * To work on a given "world" (a given set of documents, users and profiles),
 * the application should create a session through GALILEIApp.CreateSession. To
 * delete the session, GALILEIApp.DeleteSession should be called.
 *
 * The GSession is a complex class that inherits from several other classes that
 * represent all the objects that are managed. These include :
 * - GKB represents the knowledge base. It is a root class (virtually) inherited
 *   by many of the parent classes of GSession (and all of the classes described
 *   below).
 * - GObjects<GDoc,hDocs> manages documents.
 * - GObjects<GTopic,hTopics> manages topics (clusters of documents).
 * - GObjects<GUser,hUsers> manages users.
 * - GObjects<GProfile,hProfiles> manages profiles (a user has multiple
 *   profiles).
 * - GObjects<GCommunity,hCommunities> manages communities (clusters of
 *   profiles).
 * - GClasses manages classes (hierarchy of documents).
 * - GSubjects manages subjects. A subject is an "ideal" group of documents and
 *   profiles. It is uses for validation purposes (to verify if the topics and
 *   communities computed are similar to groupings provided).
 *
 * \section galilei_pseudo-parameter_sec Pseudo-parameter
 * Several member methods, in particular in GSession (and parent classes), use
 * a pseudo-parameter as first argument. This parameter, a constant pointer, is
 * needed to differentiate different methods with the same name but working on
 * different objects. Several constant variables are declared to be used as
 * pseudo-parameter (<a href="namespacemembers_vars.xhtml">pDoc, pProfile</a>,
 * etc.). The following code explains their usage:
 * @code
 * void PrintInfo(GSession* session)
 * {
 *    cout<<"Name of the session: "<<session->GetName()<<endl;
 *    cout<<"Number of documents: "<<session->GetNbObjs(pDoc)<<endl;
 *    cout<<"Number of users: "<<session->GetNbObjs(pUser)<<endl;
 *    cout<<"Number of profiles: "<<session->GetNbObjs(pProfile)<<endl;
 *    cout<<"Number of concept types: "<<session->GetNbObjs(pConceptType)<<endl;
 *    cout<<"Number of concepts: "<<session->GetNbObjs(pConcept)<<endl;
 * }
 * @endcode
 * Here, the method GObjects::GetNbObjs returns the number of objects of a given
 * type (documents, users, profiles, etc.). The pseudo-parameter (pDoc, pUser,
 * pProfile, etc.) specifies which type is concerned.
 *
 * The choice to use a pseudo-paramter for several methods was motivated for
 * programming constraints (since GSession inherits several types from the same
 * template class, it is necessary to distinct the methods) and to ensure the
 * coherence of the API.
 *
 * \section galilei_notifications_sec Notifications
 * The GALILEI platform uses the notification mechanism provided by the <a
 * href="../rcore"> R Core library</a> through the inheritance of the
 * <a href="../rcore/classR_1_1RObject.xhtml#details">R::RObject</a> class.
 *
 * The notifications are used by the GALILEI platform to communicate some events
 * to modules (plug-ins, application, etc.). A typical example is to inform that
 * the current description of a document is changed : a first notification will
 * be emitted before its replacement to specify that the description will be
 * deleted, and a second one will be emitted after the new description
 * assignment to inform that a new description id added. Typically, a plug-in
 * proposing a similarity measure will observe the second notification to know
 * that all the measures concerning that particular document are outdated.
 *
 * Several notifications are send by the objects at different moment. They are
 * declared by a series of arrays:
 * -# The tObjEvent enumeration that defines a list of possible events generated
 *    by the objects.
 * -# Several arrays of notifications (<a href="namespacemembers_vars.xhtml">
 *    hDocs, hProfiles</a>, etc.) using tObjEvent as indexer.
 *
 * GSession sends a notification GALILEI::hReInit each time it forces the
 * computations on objects of a given type. An example is to force a new
 * analysis of the documents. In this case, all their descriptions will be
 * cleared and, rather than to send a notification for each document, the
 * session send one notification.
 *
 * GPlugInList sends also a notification GALILEI::hCurrentPlugIn each time a
 * plug-in becomes its current one. This can be useful because plug-ins may have
 * dependencies. For example, a measure plug-in computing a similarity between
 * two documents may rely on a measure plug-in proposing a
 * weighting schema for the document descriptions.
 *
 * When programming modules, in particular plug-ins, developers should
 * carefully choose the notifications to observe.
 *
 * The following code defines a plug-in that will print information on the
 * status of the descriptions of documents.
 * @code
 * class MyPlugIn : public RObject, public GMeasure
 * {
 * public:
 *    MyPlugIn(GSession* session,GPlugInFactory* fac);
 *    virtual void Init(void);
 *    void HandleAddDoc(const R::RNotification& notification);
 *    void HandleDelDoc(const R::RNotification& notification);
 *    void HandleReInit(const R::RNotification& notification);
 *    void HandleCurrentPlugIn(const R::RNotification& notification);
 * };
 *
 * MyPlugIn::MyPlugIn(GSession* session,GPlugInFactory* fac)
 * : RObject(fac->GetName()), GMeasure(session,fac)
 * {
 *    InsertObserver(HANDLER(MyPlugIn::HandleAddDoc),hDocs[oeUpdated]);
 *    InsertObserver(HANDLER(MyPlugIn::HandleDelDoc),hDocs[oeAboutToBeUpdated]);
 *    InsertObserver(HANDLER(MyPlugIn::HandleDelDoc),hDocs[oeAboutToBeDeleted]);
 *    InsertObserver(HANDLER(MyPlugIn::HandleReInit),hReInit,session);
 * }
 *
 * void MyPlugIn::Init(void)
 * {
 *    // The insertion must be done here since the order of the creation of the
 *    // plug-ins are unknown.
 *    InsertObserver(HANDLER(MyPlugIn::HandleCurrentPlugIn),hCurrentPlugIn,GALILEIApp->GetManager("Measures")->GetPlugInList("Features Evaluation"));
 * }
 *
 * void MyPlugIn::HandleAddDoc(const RNotification& notification)
 * {
 *    // Verify that the sender is a document
 *  	GDoc* Doc(dynamic_cast<GDoc*>(notification.GetSender()));
 *  	if(Doc)
 *       cout<<"Document "<<Doc->GetId()<<" has a new description"<<endl;
 * }
 *
 * void MyPlugIn::HandleDelDoc(const RNotification& notification)
 * {
 *    // Verify that the sender is a document
 *  	GDoc* Doc(dynamic_cast<GDoc*>(notification.GetSender()));
 *  	if(Doc)
 *       cout<<"The description of document "<<Doc->GetId()<<" will be deleted"<<endl;
 * }
 *
 * void MyPlugIn::HandleReInit(const R::RNotification& notification)
 * {
 *    GSessionMsg& Msg(GetData<GSessionMsg&>(notification));
 *    if(Msg.GetType()==otDoc)
 *       cout<<"The description of all documents will be deleted"<<endl;
 * }
 *
 * void MyPlugIn::HandleCurrentPlugIn(const R::RNotification& notification)
 * {
 *	   GMeasure* Weighting(dynamic_cast<GMeasure*>(GetData<GPlugIn*>(notification)));
 * 	   cout<<Weighting->GetName()<<endl;
 * }
 * @endcode
 * For safety raesons, the methods HandleAddDoc and HandleDelDoc verifies that
 * the sender of the notification is a document.
 */


//------------------------------------------------------------------------------
/**
* \namespace GALILEI
* \brief GALILEI Classes.
*
* This namespace declares all the classes implemented during the GALILEI
* project.
*/


}  //-------- End of namespace GALILEI -----------------------------------------

//------------------------------------------------------------------------------
#endif
