


#ifndef GWordCalcsH
#define GWordCalcsH
//---------------------------------------------------------------------------
//include files for Rainbow
 #include <rstd/rcontainer.h>


//---------------------------------------------------------------------------
//include files for GALILEI
#include<gdocs/gdoc.h>
#include<glangs/glang.h>
#include<gsessions/gsession.h>
#include <gdocs/gwordcalc.h>

using namespace GALILEI;

namespace GALILEI{

class GDoc;

//---------------------------------------------------------------------------
class GWordCalcs : public RStd::RContainer<GWordCalc,unsigned,true,true>
{
public:
	double NbWordsDocs;
	GWordCalc **Order;
    GWordCalc **CurOrder;

	GWordCalcs(GLang *lang,GSession *session) throw(bad_alloc);
	int Compare(GWordCalcs &calcs);
	int Compare(GWordCalcs *calcs);
	void Analyse(GDoc *doc);
	void EndCalc(void);
	unsigned int NextWord(void);
	virtual ~GWordCalcs(void);

	static int sortOrder(const void *a,const void *b);
};
}
       #endif
