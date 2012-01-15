/*

	GALILEI Research Project

	GenericSims_KDE.h

	Generic Tensor Space Model Similarity Measure (KDE Part) - Implementation.

	Copyright 2003-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2003 by Valery Vandaele.
	Copyright 2003-2007 by the Université Libre de Bruxelles (ULB).

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
#ifndef GGenericSims_KDEH
#define GGenericSims_KDEH


//------------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QCheckBox>
#include <knuminput.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <qgmatrixmeasuredlg.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
/**
 * Dialog class to configure a plug-in implementing a tensor space model
 * similarity measure. It must called:
 * @code
 * void Configure(GFactoryMeasure* params)
 * {
 *    GGenericSimsDlg dlg;
 *    dlg.Configure(params);
 * }
 * @endcode
 */
class GGenericSimsDlg : public QGMatrixMeasureDlg
{
	/**
	 * Similarity measure.
	 */
	QComboBox* SimType;

	/**
	 * Product factor.
	 */
	KDoubleNumInput* Factor;

	/**
	 * Textual Capacity/Weight.
	 */
	KDoubleNumInput* TextualCapacity;

	/**
	 * Semantic Capacity/Weight.
	 */
	KDoubleNumInput* SemanticCapacity;

	/**
	 * Metadata Capacity/Weight.
	 */
	KDoubleNumInput* MetadataCapacity;

	/**
	 * Capacity associated to the pair (Textual,Semantic).
	 */
	KDoubleNumInput* TextualSemanticCapacity;

	/**
	 * Capacity associated to the pair (Textual,Medatadata).
	 */
	KDoubleNumInput* TextualMetadataCapacity;

	/**
	 * Capacity associated to the pair (Semantic,Medatadata).
	 */
	KDoubleNumInput* SemanticMetadataCapacity;

public:

   /**
    * Build the dialog box.
    * @param title          Title of the dialog box.
    */
	GGenericSimsDlg(const QString& title);

   /**
    * Add a particular weight/capacity to the dialog box.
    * @param cap            The widget managing the weight/capacuty.
    * @param str            Label.
    * @param grid           The grid where it is placed.
    * @param row            The row in the grid.
    * @param col            The column in the grid.
    */
	void AddCapacity(KDoubleNumInput* &cap,const char* str,QGridLayout* grid,int row,int col);

   /**
    * Construct the panel with the parameters of the tensor space model
    * similarity.
    */
	virtual void Panel(void);

   /**
    * Initialize the dialog box with the values of the parameters.
    * @param plugin         Plug-in.
    */
	virtual void Init(GPlugIn* plugin);

   /**
    * Assign the values of the dialog box to the parameters.
    * @param plugin         Plug-in.
    */
	virtual void Done(GPlugIn* plugin);

	 /**
    * This method should check if the parameters of the integral of Choquet are
	 * OK.
    * @return true if it is the case.
    */
   virtual bool IsDlgOK(void);
};


//------------------------------------------------------------------------------
#endif
