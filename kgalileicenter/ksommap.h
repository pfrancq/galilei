/*

	GALILEI Research Project

	KViewSom.h

	Window to display a self Organizing Map - Header.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
#ifndef _MAP_H_
#define _MAP_H_


//-----------------------------------------------------------------------------
// inlcude files for Qt
#include <qlistview.h>
#include <qpainter.h>
#include <qgroupbox.h>
#include <qwidget.h>
#include <qstring.h>
#include <qscrollview.h>
#include <qlabel.h>



//---------------------------------------------------------------------------
/**
* The KSomMap class represents a 2D widget for self organizing map
* @author David Wartel
* @short SOM Widget
*/
//-----------------------------------------------------------------------------
 struct data
 {
 	unsigned int id;
	unsigned int neuronid;
 };

//-----------------------------------------------------------------------------
 struct neuron
 {
 	unsigned int id;
	unsigned int nbcontent;
	unsigned int nblabels;
	QString* labels;
	data* datas;
 };

 //-----------------------------------------------------------------------------
class KSomMap : public QWidget
{
	Q_OBJECT

	/**
	*
	*/
	QPainter * Painter;

	/**
	*
	*/
	unsigned int MapX;

	/**
	*
	*/
	unsigned int MapY;

	/**
	*
	*/
	unsigned int Radius;

	/*
	*
	*/
	bool bShowLabels;

	/**
	*
	*/
	QColor* Color;

	/**
	*
	*/
	enum ColorType {Red=0, Green=1, Blue=2};
	
	/**
	*
	*/
	enum NeuronType {Rectangular=0, Hexagonal=1, Circle=2};

	/**
	*
	*/
	ColorType Color_Vary;

	/**
	*
	*/
	NeuronType Neuron_Type;

	/**
	* maximum data per neuron
	*/
	unsigned int MaxNbData;

	/**
	*  minim data per neuron
	*/
	unsigned int MinNbData;

	/**
	*
	*/
	unsigned int NbDisplayedLabels;

public :

	/**
	*
	*/
	neuron* Neurons;

	/**
	*
	*/
	QLabel** Labels;

signals:

	void ClickedNeuron(int);


public slots:

	/**
	* slot to redraw the map
	*/
	void slotNbLabelsChanged(int nblabels);
	
	/**
	* slot to change neurons geometry
	*/
	void slotGeometryChanged(const QString&  geometry);


public:

	/**
	* Default Constructor
	*/
	KSomMap(QWidget* parent);

	/**
	* event called by qt
	*/
	void paintEvent(QPaintEvent*);

	/**
	* event called by qt when mouse is pressed
	*/
	void mousePressEvent( QMouseEvent *e);

	/**
	* method to find the clicked neuron from a mouse event
	* @param e                 mouse event to treat
	*/
	int FindSelectedNeuron( QMouseEvent *e);

	/**
	* draws the mapn with a special color for the selected neuron
	* @param selected_index    id of the selected neuron.
	*/
	void DrawMap(int selected_index=-1);

	/**
	* draws the map on initialization
	*/
	void DrawInitMap(void);

	/**
	* draw a neuron on initialization
	* @param i                 index of the neuron to draw
	*/
	void DrawInitNeuron(int i);

	/**
	* draws a neuron
	* @param cx                hexagonal x_coordonate of the neuron
	* @param cy                hexagonal y_coordonate of the neuron
	* @param color             color to draw the neuron
	*/
	void DrawNeuron(int cx, int cy, int r, QColor color);
		
		/**
	* draws a hexagonal neuron
	* @param cx                hexagonal x_coordonate of the neuron
	* @param cy                hexagonal y_coordonate of the neuron
	* @param color             color to draw the neuron
	*/
	void DrawHexNeuron(int cx, int cy, int r, QColor color);

	/**
	* draws a rectalgular neuron
	* @param cx                hexagonal x_coordonate of the neuron
	* @param cy                hexagonal y_coordonate of the neuron
	* @param color             color to draw the neuron
	*/
	void DrawRectNeuron(int cx, int cy, int r, QColor color);

	/**
	* draws a rectalgular neuron
	* @param cx                hexagonal x_coordonate of the neuron
	* @param cy                hexagonal y_coordonate of the neuron
	* @param color             color to draw the neuron
	*/
	void DrawCircleNeuron(int cx, int cy, int r, QColor color);
	
	/**
	* sets the dimensions of the map
	* @param x                 x_dimension
	* @param y                 y_dimension
	*/
	void SetDim(unsigned int x, unsigned int y);

	/**
	* returns the x_dimension of the map
	*/
	unsigned int X() {return MapX;}

	/**
	* returns the x_dimension of the map
	*/
	unsigned int Y() {return MapY;}

	/**
	* returns the color of the map (identified by rgb)
	*/
	QColor* GetColor(void) {return Color;}

	/**
	* sets the color of the map
	* @param c                 color to set
	*/
	void SetColor(QColor* c);

	/**
	* set the minimum if data behind a neuron
	*/
	void SetMinNbData(unsigned int min) {MinNbData=min;}

	/**
	* set the maximum if data behind a neuron
	*/
	void SetMaxNbData(unsigned int max) {MaxNbData=max;}

	/**
	* returns the radius od a neuron
	*/
	unsigned int GetRadius(void) {return Radius;}

	/*
	* switch the "show label" state
	*/
	void SwitchShowLabels(void);

	/**
	* returns the "Show Labels" boolean
	*/
	bool GetbShowLabels(void) {return bShowLabels;}

	/**
	* returns the number of labels displayed per neuron
	*/
	unsigned int GetNbDisplayedLabels(void) {return NbDisplayedLabels;}

	/**
	* shows the labels
	*/
	void ShowLabels(void);

	/**
	*  hide the labels
	*/
	void HideLabels(void);

 	/*
	* creates a label
	* @param i                 index of the corresponding neuron
	* @param cx                x_coordonate of the corresponding neuron
	* @param cy                y_coordonate of the corresponding neuron
	* @param color             color to set to the label
	* @param nblabels          number of labels (string) to put in the label
	*/
	void CreateLabel(unsigned int i, unsigned int cx, unsigned int cy, QColor color, unsigned int nblabels);

	/*
	* computes the radius of the neurons (depending on time)
	*/
	void ComputeRadius(void);

	/**
	* initializes the data behind the neurons
	* @param neuronid          index of the neuron
	* @param nbdata            number of data behind the neuron
	*/
	void InitNeuronData(unsigned int neuronid, unsigned int nbdata);

	/**
	* intializes the labels of the neurons
	* @param neuronid          index of the neuron
	* @param nblabels            number of labels behind the neuron
	*/
	void InitNeuronLabels(unsigned int neuronid, unsigned int nblabels);

	/**
	* Default Destructor
	*/
	virtual ~KSomMap();
};


//-----------------------------------------------------------------------------
#endif // _SOM_H_

