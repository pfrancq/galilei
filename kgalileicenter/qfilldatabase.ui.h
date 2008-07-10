/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include "qchooseparent.h"
#include "qmessagebox.h"
#include "qlistview.h"
#include "kurlrequester.h"
#include "knuminput.h"
using namespace std;

// include for R project
#include <rstring.h>
#include <rdb.h>
#include <rqt.h>

void QFillEmptyDatabase::ChangeParent( int s )
{
    if(s==QButton::On)
    {
	ParentName->setEnabled(true);
	ChooseBtn->setEnabled(true);
    }
    if(s==QButton::Off)
    {
	ParentName->setText("");
	ParentName->setEnabled(false);
	ChooseBtn->setEnabled(false);
    }
 //   if(s==Qbutton:NoChange)
}

void QFillEmptyDatabase::InsertSubItem(R::RDb* db,int parentId,QListViewItem* item)
{
	QListViewItem* t;
	R::RString sSQL("");

	sSQL="SELECT * FROM topics where parent="+R::RString::Number(parentId);
	auto_ptr<R::RQuery> insert(db->Query(sSQL));

	for(insert->Start();!insert->End();insert->Next())
	{
		item->insertItem(t=new QListViewItem(item,ToQString((*insert)[1])));
		InsertSubItem(db,atoi((*insert)[0]),t);
	}

}

void QFillEmptyDatabase::ChooseParentName()
{
/*	R::RString sSQL("");
	QListViewItem* t;

	try
	{
		//Init dlg
		QChooseParent dlg(this,0,true);
		dlg.TopicsView->setColumnWidth(0,300);

		GSubjects * 	GetSubjects

		sSQL="SELECT * FROM topics where parent=0";
		R::RQuery mainTopics(Db,sSQL);

		for(mainTopics.Start();!mainTopics.End();mainTopics.Next())
		{
			dlg.TopicsView->insertItem(t=new QListViewItem(dlg.TopicsView,ToQString(mainTopics[1])));
			InsertSubItem(Db,atoi(mainTopics[0].Latin1()),t);
		}

		if(dlg.exec())
		{
			R::RString tmp("");
			t=dlg.TopicsView->selectedItem();
			if(t)
			{
				tmp=R::FromQString(t->text(0));
				t=t->parent();
				while(t)
				{
					tmp=R::FromQString(t->text(0))+"/"+tmp;
					t=t->parent();
				}
				ParentName->setText(ToQString(tmp));
			}
		}
	}
	//catch(GException& e)
	//{
	//	QMessageBox::critical(this,"KGALILEICenter",QString(e.GetMsg()));
	//}
	catch(R::RException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter",QString(e.GetMsg()));
	}
	catch(std::bad_alloc)
	{
		QMessageBox::critical(this,"KGALILEICenter",QString("Memory Error"));
	}
	catch(...)
	{
		QMessageBox::critical(this,"KGALILEICenter",QString("Undefined Error"));
	}*/
	std::cout<<"Choose Parent not implemented"<<std::endl;
}

