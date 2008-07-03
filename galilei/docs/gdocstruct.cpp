/*

	GALILEI Research Project

	GDocStruct.cpp

	Structure of the document content - Implementation.

	Copyright 2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for GALILEI
#include <gdocstruct.h>
#include <gweightinfo.h>
#include <gconcepttype.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GOccurInfo
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GOccurInfo::GOccurInfo(GWeightInfo* info,size_t pos)
	: Info(info), Pos(pos)
{
}


//------------------------------------------------------------------------------
int GOccurInfo::Compare(const GOccurInfo& info) const
{
	return(Pos-info.Pos);
}


//------------------------------------------------------------------------------
void GOccurInfo::Print(void)
{
	cout<<"    "<<Pos<<" : "<<Info->GetConcept()->GetName()<<" - "<<Info->GetType()->GetName()<<endl;
}



//------------------------------------------------------------------------------
//
//  GOccursInfo
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GOccursInfo::GOccursInfo(GWeightInfo* info)
	: R::RContainer<GOccurInfo,false,false>(5), Info(info)
{
}


//------------------------------------------------------------------------------
int GOccursInfo::Compare(const GOccursInfo& info) const
{
	return(Info->Compare(*info.Info));
}


//------------------------------------------------------------------------------
int GOccursInfo::Compare(const GConcept& concept) const
{
	return(Info->Compare(concept));
}



//------------------------------------------------------------------------------
//
//  GDocStructNode
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocStructNode::GDocStructNode(GWeightInfo* info,size_t pos,NodeType type)
	: GOccurInfo(info,pos), RNode<GDocStructNode,true,false>(), Type(type),
	  Content(20,50)
{
}


//------------------------------------------------------------------------------
void GDocStructNode::InsertContent(GOccurInfo* info)
{
	Content.InsertPtr(info);
}


//------------------------------------------------------------------------------
R::RCursor<GOccurInfo> GDocStructNode::GetContent(void) const
{
	return(R::RCursor<GOccurInfo>(Content));
}


//------------------------------------------------------------------------------
void GDocStructNode::Print(void)
{
	cout<<Pos<<" : "<<Info->GetConcept()->GetName()<<" - "<<Info->GetType()->GetName();
	if(Parent&&Parent->Info)
		cout<<" ("<<Parent->Info->GetConcept()->GetName()<<")";
	cout<<endl;
	R::RCursor<GOccurInfo> Cur(Content);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Print();
}



//------------------------------------------------------------------------------
//
//  GDocSruct
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocStruct::GDocStruct(size_t nb)
	: RTree<GDocStructNode,true,false>(nb,nb), Infos(nb)
{
	RTree<GDocStructNode,true,false>::InsertNode(0,new GDocStructNode(0,0,GDocStructNode::Root));
}


//------------------------------------------------------------------------------
R::RCursor<GOccurInfo> GDocStruct::GetOccurences(const GConcept* concept) const
{
	GOccursInfo* ptr=Infos.GetPtr(concept);
	if(!ptr)
		return(R::RCursor<GOccurInfo>());
	return(R::RCursor<GOccurInfo>(*ptr));
}


//------------------------------------------------------------------------------
void GDocStruct::InsertNode(GDocStructNode* parent,GDocStructNode* occur)
{
	GOccursInfo* ptr=Infos.GetInsertPtr(occur->GetInfo());
	ptr->InsertPtr(occur);
	RTree<GDocStructNode,true,false>::InsertNode(parent,occur);
}


//------------------------------------------------------------------------------
void GDocStruct::InsertContent(GDocStructNode* parent,GWeightInfo* info,size_t pos)
{
	GOccurInfo* occur=new GOccurInfo(info,pos);
	GOccursInfo* ptr=Infos.GetInsertPtr(occur->GetInfo());
	ptr->InsertPtr(occur);
	parent->InsertContent(occur);
}


//------------------------------------------------------------------------------
void GDocStruct::Print(GDocStructNode* ptr)
{
	if(!ptr)
		ptr=GetTop();
	RCursor<GDocStructNode> Nodes(ptr->GetNodes());
	for(Nodes.Start();!Nodes.End();Nodes.Next())
	{
		Nodes()->Print();
		Print(Nodes());
	}
}


//------------------------------------------------------------------------------
void GDocStruct::Clear(void)
{
	Infos.Clear();
	RTree<GDocStructNode,true,false>::Clear();
	RTree<GDocStructNode,true,false>::InsertNode(0,new GDocStructNode(0,0,GDocStructNode::Root));
}
