#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './configure.ui'
**
** Created: mer mai 2 18:17:35 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "configure.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlabel.h>
#include <kurlrequester.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "./configure.ui.h"

/*
 *  Constructs a ConfigureDlg as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ConfigureDlg::ConfigureDlg( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ConfigureDlg" );
    setSizeGripEnabled( TRUE );
    ConfigureDlgLayout = new QVBoxLayout( this, 11, 6, "ConfigureDlgLayout"); 

    tabWidget = new QTabWidget( this, "tabWidget" );

    Widget8 = new QWidget( tabWidget, "Widget8" );
    Widget8Layout = new QVBoxLayout( Widget8, 11, 6, "Widget8Layout"); 

    layout4 = new QHBoxLayout( 0, 0, 6, "layout4"); 

    textLabel2 = new QLabel( Widget8, "textLabel2" );
    layout4->addWidget( textLabel2 );

    PlugInsConfig = new KURLRequester( Widget8, "PlugInsConfig" );
    layout4->addWidget( PlugInsConfig );
    Widget8Layout->addLayout( layout4 );

    layout5 = new QHBoxLayout( 0, 0, 6, "layout5"); 

    textLabel3 = new QLabel( Widget8, "textLabel3" );
    layout5->addWidget( textLabel3 );

    LogFile = new KURLRequester( Widget8, "LogFile" );
    layout5->addWidget( LogFile );
    Widget8Layout->addLayout( layout5 );

    layout6 = new QHBoxLayout( 0, 0, 6, "layout6"); 

    textLabel4 = new QLabel( Widget8, "textLabel4" );
    layout6->addWidget( textLabel4 );

    DebugFile = new KURLRequester( Widget8, "DebugFile" );
    layout6->addWidget( DebugFile );
    Widget8Layout->addLayout( layout6 );
    spacer4 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Widget8Layout->addItem( spacer4 );
    tabWidget->insertTab( Widget8, QString::fromLatin1("") );

    Widget9 = new QWidget( tabWidget, "Widget9" );
    Widget9Layout = new QHBoxLayout( Widget9, 11, 6, "Widget9Layout"); 

    Dirs = new QListBox( Widget9, "Dirs" );
    Widget9Layout->addWidget( Dirs );

    layout8 = new QVBoxLayout( 0, 0, 6, "layout8"); 

    buttonAdd = new QPushButton( Widget9, "buttonAdd" );
    layout8->addWidget( buttonAdd );

    buttonDelete = new QPushButton( Widget9, "buttonDelete" );
    layout8->addWidget( buttonDelete );
    spacer5 = new QSpacerItem( 20, 100, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout8->addItem( spacer5 );
    Widget9Layout->addLayout( layout8 );
    tabWidget->insertTab( Widget9, QString::fromLatin1("") );
    ConfigureDlgLayout->addWidget( tabWidget );

    Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1"); 

    buttonHelp = new QPushButton( this, "buttonHelp" );
    buttonHelp->setAutoDefault( TRUE );
    Layout1->addWidget( buttonHelp );
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setAutoDefault( TRUE );
    Layout1->addWidget( buttonCancel );
    ConfigureDlgLayout->addLayout( Layout1 );
    languageChange();
    resize( QSize(441, 377).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonAdd, SIGNAL( clicked() ), this, SLOT( newAddDir() ) );
    connect( buttonDelete, SIGNAL( clicked() ), this, SLOT( newRemoveDir() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ConfigureDlg::~ConfigureDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ConfigureDlg::languageChange()
{
    setCaption( tr2i18n( "Configure GALILEI" ) );
    textLabel2->setText( tr2i18n( "Plug-ins configuration:" ) );
    textLabel3->setText( tr2i18n( "Log file:" ) );
    textLabel4->setText( tr2i18n( "Debug file:" ) );
    tabWidget->changeTab( Widget8, tr2i18n( "Ge&neral" ) );
    buttonAdd->setText( tr2i18n( "Add ..." ) );
    buttonDelete->setText( tr2i18n( "Delete" ) );
    tabWidget->changeTab( Widget9, tr2i18n( "Plug-Ins Directories" ) );
    buttonHelp->setText( tr2i18n( "&Help" ) );
    buttonHelp->setAccel( QKeySequence( tr2i18n( "F1" ) ) );
    buttonOk->setText( tr2i18n( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr2i18n( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
}

#include "configure.moc"
