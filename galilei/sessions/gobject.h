/*

	GALILEI Research Project

	GObject.h

	Generic GALILEI Object - Header.

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef GObjectH
#define GObjectH


//------------------------------------------------------------------------------
// include files for R
#include <robject.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GObject provides a class for a generic object in GALILEI. In particular,
 * each object can be part of a statement.
 *
 * The object maintains containers of statements where it appears as subject,
 * predicate or values. By default, these containers are not suppose to be
 * large. But for certain objects, for example those who are predicates, one of
 * these containers can be used. It is then more efficient to verify that it can
 * hold a large number of statements with the method GObject::VerifyStatements.
 * @author Pascal Francq
 * @short Generic Object
 */
class GObject : public R::RObject
{
protected:

	/**
	 * Session of the object.
	 */
	GSession* Session;

	/**
	 * Identifier of the object.
	 */
	size_t Id;

	/**
	* Type of the object.
	*/
	tObjType ObjType;

	/**
	 * Statements where the object is a subject.
	 * 
	 * It is ordered to allow the verification that a given statement doesn't
	 * already exist.
	 */
	R::RContainer<GStatement,false,true>* Subjects;

	/**
	 * Statements where the object is a predicate.
	 */
	R::RContainer<GStatement,false,false>* Predicates;

	/**
	 * Statements where the object is a value.
	 */
	R::RContainer<GStatement,false,false>* Values;

public:

	/**
	 * Construct a object.
	 * @param session        Session.
	 * @param id             Identifier.
	 * @param name           Name.
	 * @param type           Type.
	 */
	GObject(GSession* session,size_t id,const R::RString& name,tObjType type);

	/**
	 * Copy constructor.
	 * @param obj            Original object.
	 */
	GObject(const GObject* obj);

	/**
	 * Verify that a given container can hold a given number of statements.
    * @param what           Type of container to verify.
    * @param size           Size of the container.
    */
	void VerifyStatements(tStatementElement what,size_t size);

	/**
	* Compare method used by R::RContainer.
	* @param obj             Object to compare with.
	*/
	int Compare(const GObject& obj) const;

	/**
	* Compare method used by R::RContainer.
	* @param id              Identifier to compare with.
	*/
	int Compare(size_t id) const;

	/**
	* Compare method used by R::RObjectContainer.
	* @param obj             Object to compare with.
	*/
	int Search(const GObject& obj) const;

	/**
	* Compare method used by R::RContainer.
	* @param name            Name to compare with.
	*/
	int Search(const R::RString& name) const;

	/**
    * @return the class name.
    */
	virtual R::RCString GetClassName(void) const {return("GObject");}

	/**
	 * Defines if the objects have a description.
	 * @return false.
	 */
	static inline bool HasDesc(void) {return(false);}

	/**
	 * Defines of the objects have a concept tree.
	 * @return false.
	 */
	static inline bool HasTree(void){return(false);}

	/**
	* @return the session of the object.
	*/
	inline GSession* GetSession(void) const {return(Session);}

	/**
	* @return the identifier of the object.
	*/
	inline size_t GetId(void) const {return(Id);}

	/**
	 * Set the identifier of the object. It can only be called if the current
	 * identifier is undefined.
	 * @param id             Identifier.
	 */
	void SetId(size_t id);

	/**
	* @return the type of the object.
	*/
	inline tObjType GetObjType(void) const {return(ObjType);}

	/**
	 * Get a given list of statements involving the object.
	 * @param obj            Pseudo-parameter.
    * @param what           Role that the object plays in the statement.
    * @return a cursor.
    */
	R::RCursor<GStatement> GetObjs(const GStatement* obj,tStatementElement what);

private:

	/**
	 * Insert a new statement.
    * @param what           Role that the object plays in the statement.
	 * @param statement      Statement to insert.
    */
	void InsertObj(tStatementElement what,GStatement* statement);

	/**
	 * Delete a statement.
    * @param what           Role that the object plays in the statement.
	 * @param statement      Statement to delete.
    */
	void DeleteObj(tStatementElement what,GStatement* statement);

public:

	/**
	 * Handler called when a notification posted is not caught. In GALILEI,
	 * this function does nothing.
	 * @param notification    Notification posted.
	 */
	virtual void HandlerNotFound(const R::RNotification& notification);

	/**
	 * Destruct the object.
	 */
	virtual ~GObject(void);

	friend class GKB;
	friend class GObjects<GDoc,hDocs>;
	friend class GObjects<GTopic,hTopics>;
	friend class GObjects<GUser,hUsers>;
	friend class GObjects<GProfile,hProfiles>;
	friend class GObjects<GCommunity,hCommunities>;
	friend class GObjects<GClass,hClasses>;
	friend class GClasses;
	friend class GSubjects;
	friend class GConceptList;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
