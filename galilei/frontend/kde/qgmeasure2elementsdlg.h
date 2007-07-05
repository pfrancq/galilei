/*

	GALILEI Research Project

	QGMeasure2ElementsDlg.h

	Configuration For Measures Between Two Elements - Header.

	Copyright 2007 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef QGMeasure2ElementsDlgH
#define QGMeasure2ElementsDlgH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <dlgmeasure2elements.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * Generic dialog box to configure a measure between two elements. A plug-in
 * must be created with the GMeasure2Elements class.
 * @code
 * void Configure(GFactoryMeasure* params)
 * {
 *    QGMeasure2ElementsDlg dlg(Similarities between subprofiles");
 *    dlg.Configure(params);
 * }
 * @endcode
 * @author Pascal Francq
 * @short Configuration For Measures Between Two Elements
 */
class QGMeasure2ElementsDlg : public DlgMeasure2Elements
{
public:	
	/**
	 * Construct the dialog box.
	 * @param title          Title of the dialog box.      
	 */
	QGMeasure2ElementsDlg(const char* str);
	
	/**
	 * Launch the configuration.
	 * @param params         Pointer to the factory.
	 */
	void Configure(GFactoryMeasure* params);

	/**
	 * Method called before the dialog box is initiliazed. By default, it hides
	 * the groupbox 'MesureSpecific' reserved for specific parameters for the
	 * measures.
	 * 
	 * This method can be override to add new widgets. The methods 'Init' and
	 * 'Done' must be overrided to link these widgets to parameters of the
	 * plug-in.
	 * @code
	 * void MyDlg::Panel(void)
	 * {
	 *    QHBoxLayout* layout = new QHBoxLayout(0,0,6);
	 *    QLabel* text = new QLabel(MeasureSpecific);
	 *    text->setText("Minimum common documents");
	 *    layout->addWidget(text);
	 *    layout->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
	 *    MinDocs = new QSpinBox(MeasureSpecific,"MinDocs");    // 'MinDocs' must be added to the class. 
	 *    layout->addWidget(MinDoc);
	 *    MeasureSpecificLayout->addLayout(layout);
	 * }
	 * @endcode
	 */
	virtual void Panel(void);
		
	/**
	 * Method called before the dialog box is opened. It uses the values of the
	 * parameters to initialize the dialog box. This method can be overrided
	 * (normally with the 'Panel' and 'Done' methods) to synchronized new
	 * parameters and widgets.
	 * @code
	 * void MyDlg::Init(GFactoryMeasure* params)
	 * {
	 *    QGMeasure2ElementsDlg::Init(params);
	 *    MinDocs->setValue(params->GetUInt("MinDocs"));    // 'MinDocs' must be added to the class.
	 * }
	 * @encode
	 * @param params         Pointer to the factory.
	 */
	virtual void Init(GFactoryMeasure* params);
	
	/**
	 * Method called when the dialog box is closed with the 'OK' button. The
	 * parameters are set based on the value of the dialog box. This method can
	 * be overrided (normally with the 'Panel' and 'Init' methods) to
	 * synchronized new parameters and widgets.
	 * @code
	 * void MyDlg::Done(GFactoryMeasure* params)
	 * {
	 *    params->SetUInt("MinDocs",MinDocs->value());    // 'MinDocs' must be added to the class.
	 *    QGMeasure2ElementsDlg::Done(params);
	 * }
	 * @endcode
	 * @param params         Pointer to the factory.
	 */	
	virtual void Done(GFactoryMeasure* params);
};


} //------- End of namespace GALILEI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
