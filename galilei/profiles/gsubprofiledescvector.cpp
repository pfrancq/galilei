/*

	GALILEI Research Project

	GSubProfileDescVector.cpp

	Sub-Profile Description using a vector - Implementation.

	(C) 2002 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
//include files for GALILEI
#include<profiles/gsubprofiledescvector.h>
#include<langs/glang.h>
#include<infos/giword.h>
#include<infos/giwordweight.h>
#include<infos/giwordsweights.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GSubProfileDescVector
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GSubProfileDescVector::GSubProfileDescVector(GSubProfile* sub,GGroup* grp,const char* a) throw(bad_alloc)
  : GSubProfileDesc(sub,grp,a), Vector(0)
{
}


//-----------------------------------------------------------------------------
bool GALILEI::GSubProfileDescVector::IsDefined(void) const
{
	// The Vector can't be empty.
	return(Vector&&(Vector->NbPtr));
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfileDescVector::Similarity(const GSubProfileDesc* desc) const
{
	double Sim=0;
	unsigned int NbComp;
	GIWordsWeights* CompVector=((GSubProfileDescVector*)desc)->Vector;
	
	for(Vector->Start();!Vector->End();Vector->Next())
	{
		for(CompVector->Start();!CompVector->End();CompVector->Next())
			if((*Vector)()->GetId()==(*CompVector)()->GetId()) Sim++;
	}
	NbComp=Vector->NbPtr+CompVector->NbPtr;
	Sim=(2.0*Sim)/static_cast<double>(NbComp);
	return(Sim);
}


//-----------------------------------------------------------------------------
GALILEI::GSubProfileDescVector::~GSubProfileDescVector(void)
{
	if(Vector) delete Vector;
}
