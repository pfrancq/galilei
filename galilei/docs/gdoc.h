/*

        GALILEI Research Project

        gdoc.h

        Basic Information - Implementation.

        (C) 2001 by P. Francq.

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

//---------------------------------------------------------------------------

#ifndef GDocH
#define GDocH


//---------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>

using namespace RStd;

//---------------------------------------------------------------------------
// include files for Galilei

#include <galilei.h>
#include <gdocs/gwordoccurs.h>
#include <glangs/glang.h>
#include <gdocs/gdocs.h>

using namespace GALILEI;


//---------------------------------------------------------------------------

namespace GALILEI{

//---------------------------------------------------------------------------

class GDocs;
class GWordOccurs;

//---------------------------------------------------------------------------
// class GDoc

class GDoc
{

        char* Buffer;                                        // Pointer
        char Word[51];                                 // Word
        GDocs *Owner;         // Owner (Container)

public:

        GWordOccurs* Words;              // Occurencies of Words
        unsigned NbWords;                // Total number of words
        unsigned NbDiffWords;            // Number of different words
        GLang* Lang;                        // Pointer to the language
        RString URL;                           // URL
        unsigned Id;                     // Unique identifier
        char* Content;                   // Pointer to the content
        bool Calc;                       // Must the doc be analysed?
        bool bSave;                                                                                         // Must the doc be saved?

        GDoc(GDocs *owner,const RString& URL) throw(bad_alloc);
        int Compare(const GDoc& doc);
        int Compare(const GDoc* doc);
        int Compare(const unsigned id);
        unsigned char GetChar(void);
        bool IsSpace(void);
        void SkipSpaces(void);
        void InitWords(void);
        bool NextWord(void);
        void Analyse(char* body) throw(GException);
      //  virtual void Load(void) throw(GException,bad_alloc)=0;
       // virtual void Save(void) throw(GException)=0;
       // virtual void Download(void)=0;
        virtual ~GDoc(void);

};




}  //-------- End of namespace Galilei-----------------------------------

//---------------------------------------------------------------------------
#endif
