

//---------------------------------------------------------------------------
#ifndef GWordOccursH
#define GWordOccursH


//---------------------------------------------------------------------------
// include files for Rainbow

#include <rstd/rcontainer.h>
#include <rstd/rstring.h>

using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei
#include <galilei.h>
#include <gdocs/gdoc.h>



//---------------------------------------------------------------------------
namespace GALILEI{
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// class GWordOccurs
class GWordOccurs : public RContainer<GWordOccur,unsigned,true,true>
{
	GDoc *Owner;

public:
	GWordOccurs(GDoc *owner,unsigned nb) throw(bad_alloc);
	void Insert(unsigned id,GDoc *doc) throw(bad_alloc);
	GWordOccur* GetPtr(unsigned id,GDoc *doc) throw(bad_alloc);
};





}  //-------- End of namespace Galilei-----------------------------------

//---------------------------------------------------------------------------
#endif
