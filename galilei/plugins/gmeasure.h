/*

	GALILEI Research Project

	GMeasure.h

	Generic Measure - Implementation.

	Copyright 2005-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2005-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GMeasureH
#define GMeasureH


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdarg.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GMeasure class provides a representation for a specific measure (ex:
* Similarities between profiles, similarities between documents and groups,
* agreement ratios between profiles, ...).
* @author Pascal Francq
* @short Measures.
*/
class GMeasure : public GPlugIn
{
public:

	/**
	* Constructor of a measure.
	* @param session         Session.
	* @param fac             Plug-in factory.
	*/
	GMeasure(GSession* session,GPlugInFactory* fac);

	/**
	* The measure must be re-initialized, i.e. all values must be considered
	* as dirty.
	*/
	virtual void ReInit(void);

	/**
	* Get a measure. Each plug-in can handle several types of measure (first
	* parameter of the method). The other parameters (including eventually the
	* result of the measure) depend of the particular plug-in. This means that
	* the caller must exactly know which parameters are requested by a
	* particular plug-in. This method is intended to be called in an intensive
	* way (in contrary of Info)
	* @param measure         Type of the measure.
	*
	* Suppose that the method takes two int arguments and should return a
	* double that represents the product (measure=1) or the sum (measure=2). This
	* method should be written:
	* @code
	* void Measure(int measure,...)
	* {
	* 	va_list ap;
	* 	va_start(ap,measure);
	* 	int i=va_arg(ap,int);
	* 	int j=va_arg(ap,int);
	* 	double* d=va_arg(ap,double*);
	* 	switch(measure)
	* 	{
	* 		case 1 : (*d)=i*j; break;
	* 		case 2 : (*d)=i+j; break;
	* 	}
	* 	va_end(ap);
	* }
	* @endcode
	* The method should be call:
	* @code
	* GMeasure* ptr;
	* double d;
	* ...
	* ptr->Measure(1,3,4,&d);
	* cout<<d<<endl;
	* @endcode
	*/
	virtual void Measure(size_t measure,...)=0;

	/**
	* Exchange a particular information with the plug-in. The other parameters
	* (including eventually the result of the measure) depend of the particular
	* plug-in. This means that the caller must exactly know which parameters
	* are requested by a particular plug-in. This method is not intended to be
	* called in an intensive way (in contrary of Measure).
	* @param info           Information to get.
	*
	* Suppose the function should simply return a double. This method should
	* be written:
	* @code
	* void Info(int info,...)
	* {
	* 	va_list ap;
	* 	double* d=va_arg(ap,double*);
	* 	switch(info)
	* 	{
	* 		case 1 : (*d)=2; break;
	* 		case 2 : (*d)=3; break;
	* 	}
	* 	va_end(ap);
	* }
	* @endcode
	* The method should be call:
	* @code
	* GMeasure* ptr;
	* double d;
	* ...
	* ptr->Info(1,&d);
	* cout<<d<<endl;
	* @endcode
	 */
	virtual void Info(size_t info,...);

	/**
	* Destruct the measure.
	*/
	virtual ~GMeasure(void);
};


//-------------------------------------------------------------------------------
#define CREATE_MEASURE_FACTORY(type,name,desc,plugin) \
	CREATE_FACTORY(GALILEI::GMeasure,plugin,"Measures",type,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
