/****************************************************************************
** Form interface generated from reading ui file './qchooseparent.ui'
**
** Created: Fri Aug 20 10:59:46 2004
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef QCHOOSEPARENT_H
#define QCHOOSEPARENT_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QListView;
class QListViewItem;
class QPushButton;

class QChooseParent : public QDialog
{
    Q_OBJECT

public:
    QChooseParent( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~QChooseParent();

    QLabel* textLabel1;
    QListView* TopicsView;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

protected:
    QGridLayout* QChooseParentLayout;
    QHBoxLayout* Layout1;

protected slots:
    virtual void languageChange();

};

#endif // QCHOOSEPARENT_H
