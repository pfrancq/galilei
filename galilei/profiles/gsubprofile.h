

#ifdef GSubProfileH
#define GSubProfileH
//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gprofiles/gprofile.h>
#include <glangs/glang.h>
#include <gdocs/gwordcalcs.h>
#include <gdocs/gwordlist.h>
#include <gprofiles/gprofdoc.h>
#include <gprofiles/gsubprofile>

using namespace GALILEI;
//-----------------------------------------------------------------------------
// include files for Rainbow

using namespace Rstd;

//-----------------------------------------------------------------------------
/**
* This class represents a sub-profile for a specific language.
* @author Pascal Francq.
* @short Sub-Profile class.
*/
class GSubProfile
{
public:
	GProfile *Owner;
	unsigned Id;     		 // Identifier
	GLang *Lang;          // Language
   GWordCalcs *MOK;		
   GWordCalcs *MKO;		
	GWordList *OK;        // List OK
	GWordList *KO;        // List KO
	GWordList *Common;    // List Common
   unsigned int NbOK;
	unsigned int NbKO;
	bool bToAttach;			// Must Attach to group
   time_t Attached;

	GSubProfile(void) throw(bad_alloc);
	GSubProfile(GProfile *owner,unsigned int id,GLang *lang) throw(bad_alloc);
	bool NeedOK(void) {return((NbOK<30)&&(NbOK<MOK->NbPtr));}
	bool NeedKO(void) {return((NbKO<30)&&(NbOK<MKO->NbPtr));}
	int Compare(const unsigned int id);
	int Compare(const GLang* lang);
	int Compare(const GSubProfile &subprofile);
	int Compare(const GSubProfile *subprofile);	
	void Analyse(GProfDoc *profdoc);
	void Analyse(void);
	bool Similar(GSubProfile *sub);
	~GSubProfile(void);
};


