/*

	Wikipedia Plug-Ins

	WikiDumpHistory.h

	Parse a Wikipedia Dump History File - Implementation.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R/GALILEI
#include <gsession.h>
#include <gdoc.h>
#include <guser.h>


//-----------------------------------------------------------------------------
// include files for current plug-in
#include <wikidumphistory.h>
#include <../wikitoken/wikitoken.h>
#include <wikipedia.h>





//-----------------------------------------------------------------------------
//
// class WikiDumpHistory
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
WikiDumpHistory::WikiDumpHistory(Wikipedia* plugin,const RString& file)
	: RXMLParser(file,"utf-8"), PlugIn(plugin), Stock(1000), Revisions(1000)
{
	Content.SetLen(2048);
}


//-----------------------------------------------------------------------------
void WikiDumpHistory::Open(void)
{
	NbAnalyzedArticles=0;
	NbAnalyzedRevisions=0;
	InRevision=false;
	InContributor=false;
	InContent=false;
	InRedirect=false;
	InAttribute=false;
	RXMLParser::Open(RIO::Read);
}


//-----------------------------------------------------------------------------
void WikiDumpHistory::BeginTag(const RString&,const RString& lName,const RString&)
{
	if(InRevision)
	{
		if(InContributor)
		{
			if((lName=="id")||(lName=="ip")||(lName=="username"))
			{
				Content.SetLen(0);
				InContent=true;
			}
		}
		else if(lName=="contributor")
		{
			InContributor=true;
			UserId=cNoRef;
		}
		else if(lName=="timestamp")
		{
			Content.SetLen(0);
			InContent=true;
		}
	}
	else if(lName=="revision")
	{
		InRevision=true;
	}
	else if(lName=="title")
	{
		InContent=true;
		Content.SetLen(0);
	}
	else if(lName=="redirect")
	{
		InRedirect=true;
		Original=false;
	}
	else if(lName=="page")
	{
		Original=true;
		Revisions.Clear();
	}
}


//-----------------------------------------------------------------------------
void WikiDumpHistory::BeginTagParsed(const RString&,const RString& lName,const RString&)
{
	if((!InRevision)&&(lName=="redirect"))
	{
		if(!Page)
		{
			Content.Replace(' ','_');
			URI="https://en.wikipedia.org/wiki/"+Content;
			Page=PlugIn->GetSession()->GetObj(pDoc,URI,true,true);
		}
	}
}


//-----------------------------------------------------------------------------
void WikiDumpHistory::EndTag(const RString&,const RString& lName,const RString&)
{
	if(InRevision)
	{
		if(InContributor)
		{
			if(lName=="ip")
			{
				InContent=false;
				UserName=Content;
				Anonymous=true;
			}
			else if(lName=="id")
			{
				InContent=false;
				UserId=Content.ToSizeT();
				Anonymous=false;
			}
			else if(lName=="username")
			{
				InContent=false;
				UserName=Content;
			}
			else if(lName=="contributor")
			{
				InContributor=false;
			}
		}
		else if(lName=="timestamp")
		{
			if(Content.IsEmpty())
				RevisionDate=RDate::Null;
			else
				RevisionDate.SetDate(Content.Mid(0,10)+" "+Content.Mid(11,8));
			InContent=false;
		}
		else if(lName=="revision")
		{
			if(Page&&UserName!="Conversion script")   // Don't import revisions from 'Conversion script'.
			{
				// Compute the name of the user
				UserName=UserName.Trim();
/*				if(UserName=="DanKeshet")
					cout<<"Debug"<<endl;*/
				if((!PlugIn->MustMergeUsers())&&(!Anonymous))
					UserName+=" ("+RString::Number(UserId)+")";

				// Look if the user is existing
				GUser* User;
				if(Anonymous)
					User=PlugIn->GetSession()->GetObj(pUser,1,true,true);
				else
					User=PlugIn->GetSession()->GetObj(pUser,UserName,true,true);
				if(!User)
				{
					User=new GUser(PlugIn->GetSession(),cNoRef,UserName,UserName,1);
					PlugIn->GetSession()->InsertObj(User);
				}

				// Look if the profile is existing
				GProfile* Profile(User->GetProfile(UserName));
				if(!Profile)
				{
					Profile=new GProfile(PlugIn->GetSession(),User,ptExpertise,UserName,true);
					PlugIn->GetSession()->InsertObj(Profile);
				}

				// Look if the profile has already a feedback
				bool Find;
				size_t Idx(Revisions.GetIndex(Profile,Find));
				if(Find)
				{
					// Yes -> Put the last date
					cRevision* Revision(Revisions[Idx]);
					if(RevisionDate>Revision->Date)
						Revision->Date=RevisionDate;
				}
				else
				{
					// No -> Insert a new one
					cRevision* Revision;
					if(Revisions.GetNb()==Stock.GetNb())
						Stock.InsertPtr(Revision=new cRevision());
					else
						Revision=Stock[Revisions.GetNb()];
					Revision->Profile=Profile;
					Revision->Date=RevisionDate;
					Revisions.InsertPtrAt(Revision,Idx,false);
				}
			}

			NbAnalyzedRevisions++;
			if((!PlugIn->MustImportAllRevisions())&&PlugIn->GetNbRevisions()&&(NbAnalyzedRevisions>=PlugIn->GetNbRevisions()))
			{
				// Insert all the revisions for the current page
				RCursor<cRevision> Revision(Revisions);
				for(Revision.Start();!Revision.End();Revision.Next())
					PlugIn->GetSession()->InsertFdbk(Revision()->Profile,Page,ftRelevant,Revision()->Date);
				Revisions.Clear();
				StopAnalysis();
			}
			InRevision=false;
		}
	}
	else if(lName=="title")
	{
		Content.Replace(' ','_');
		URI="https://en.wikipedia.org/wiki/"+Content;
		PlugIn->GetSlot()->StartJob("Extract revisions from '"+URI+"'");
		Page=PlugIn->GetSession()->GetObj(pDoc,URI,true,true);
	}
	else if(lName=="page")
	{
		// Insert all the revisions
		RCursor<cRevision> Revision(Revisions);
		for(Revision.Start();!Revision.End();Revision.Next())
			PlugIn->GetSession()->InsertFdbk(Revision()->Profile,Page,ftRelevant,Revision()->Date);

		// Be sure to make the current page finished
		Page=0;
		URI.SetLen(0);

		if(Original)
			NbAnalyzedArticles++;
		if(PlugIn->MustImportAllRevisions()&&PlugIn->GetNbArticles()&&(NbAnalyzedArticles>=PlugIn->GetNbArticles()))
			 StopAnalysis();
	}
	else if(lName=="redirect")
	{
		InRedirect=false;
	}
}


//-----------------------------------------------------------------------------
void WikiDumpHistory::Text(const RString& text)
{
	if(InContent)
		Content+=text;
}


//-----------------------------------------------------------------------------
void WikiDumpHistory::AddAttribute(const RString& namespaceURI,const RString& lName,const RString& name)
{
	if(InRedirect&&lName=="title")
	{
		Content.SetLen(0);
		InAttribute=true;
	}
	else
		InAttribute=false;
}


//-----------------------------------------------------------------------------
void WikiDumpHistory::Value(const RString& value)
{
	if(InAttribute)
		Content+=value;
}