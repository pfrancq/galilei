/*

	GALILEI Research Project

	KSomMap.cpp

	Widget to represent a self organizing map- Implementation.

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
// include files for KDE
#include "ksommap.h"
#include <kmainwindow.h>
#include <klocale.h>
#include <limits.h>


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpointarray.h>
#include <qpoint.h>


//-----------------------------------------------------------------------------
KSomMap::KSomMap(QWidget* parent)
	: QWidget(parent), MapX(0), MapY(0), Radius(0), MaxNbData(0), MinNbData(UINT_MAX),  NbDisplayedLabels(1), Neurons(0), Labels(0)
{
	//init the painter
	Painter=new QPainter(this);
	if (!Painter->isActive())
		Painter->begin(this);

	bShowLabels=false;
	//init the reference color
	Color= new QColor (255,0,0);
	Color_Vary=Green;
	
	//init the geometry of neurons
	Neuron_Type=Hexagonal;
}


//-----------------------------------------------------------------------------
void KSomMap::paintEvent(QPaintEvent*)
{
	if (bShowLabels)
		HideLabels();
	DrawMap();
}


//-----------------------------------------------------------------------------
void KSomMap::mousePressEvent( QMouseEvent *e)
{
	/*unsigned*/ int selected;
	selected=FindSelectedNeuron(e);
	if (selected!=-1)
	{
		emit ClickedNeuron(selected);
		DrawMap(selected);
	}
}


//-----------------------------------------------------------------------------
void KSomMap::slotNbLabelsChanged(int nblabels)
{
	unsigned int i;
	int nb;
	char tmp[500];
	char test[500];
	const char* tmpstr;

	//set eh global parameter
	NbDisplayedLabels=nblabels;

	//update the labels displayed
	for (i=0; i<(MapX*MapY); i++)
	{
		nb=1;
		if (!Neurons[i].nblabels) continue;
		tmpstr=Neurons[i].labels[0];
		sprintf(test, "%s", tmpstr);

		while(nb<nblabels && nb<Neurons[i].nblabels)
		{
			strcat(test,"<br>");
			strcat(test,Neurons[i].labels[nb]);
			nb++;
		}
		sprintf(tmp, "<center><font size='2'>%s</font></center>",test);

		//update  the label
		Labels[i]->setText(tmp);
	}
}


//-----------------------------------------------------------------------------
void KSomMap::slotGeometryChanged(const QString& type)
{
	if (type=="Circle")
		Neuron_Type=Circle;
	if (type=="Hexagonal")
		Neuron_Type=Hexagonal;
	if (type=="Rectangular")
		Neuron_Type=Rectangular;
	DrawMap();
	// way to force refresh (maybe find another method..)
	this->resize(this->width()-1, this->height()-1);
}


//-----------------------------------------------------------------------------
int KSomMap::FindSelectedNeuron( QMouseEvent *e)
{
	unsigned int i, cx, cy;
	unsigned int row_number;
	unsigned int col_number;
	double distance;
	double neuron_width=sqrt(3)*double(Radius);

	for (i=0; i<(MapX*MapY); i++)
	{
		row_number=int(i/MapX);
		col_number=int(i%MapX);
		cy=Radius+int(1.5*Radius*row_number);
		cx=int(double(col_number)*neuron_width+0.5*neuron_width);
		if (row_number%2)
			cx=cx+int(0.5*neuron_width);
		distance=(e->x()-cx)*(e->x()-cx);
		distance+=(e->y()-cy)*(e->y()-cy);
		if (sqrt(distance)<Radius)
			return i;
	}
	return (-1);
}


//-----------------------------------------------------------------------------
void KSomMap::DrawMap(int selected_index)
{
	int i;
	unsigned int cx, cy;
	double neuron_width;
	QColor color;
	unsigned int color_level;
	unsigned int row_number;
	unsigned int col_number;

	//configure painter
	if (!Painter->isActive())
		Painter->begin(this);
	Painter->setPen(black);

	//compute the Radius of the neurons:
	ComputeRadius();

	//set the neuron width
	neuron_width=sqrt(3)*double(Radius); // distance E,C

	//draw the map
	for (i=0; i<(MapX*MapY); i++)
	{
		row_number=int(i/MapX);
		col_number=int(i%MapX);
		cy=Radius+int(1.5*Radius*row_number);
		cx=int(double(col_number)*neuron_width+0.5*neuron_width);
		if (row_number%2)
			cx=cx+int(0.5*neuron_width);
		if (i==selected_index)
			color.setRgb(103,176,150); //fixed color for selected neuron
		else
		{
			if (MaxNbData!=MinNbData)
			{
				// color empty neurons in speicific color
				if (!Neurons[i].nbcontent)
				{
					color.setRgb(50,50,50); //gray
					DrawNeuron(cx,cy, Radius, color);
					continue;
				}
				else
					color_level=200-int((Neurons[i].nbcontent-MinNbData)*(200.0/double(MaxNbData-MinNbData)));
			}
			else
				color_level=255;
			switch(Color_Vary)
			{
				case Red:
					color.setRgb(color_level,Color->green(),Color->blue());
					break;
				case Green:
					color.setRgb(Color->red(),color_level,Color->blue());
					break;
				case Blue:
					color.setRgb(Color->red(),Color->green(),color_level);
					break;
			}
		}
		DrawNeuron(cx,cy, Radius, color);
		CreateLabel(i, cx, cy, color,NbDisplayedLabels);
	}

	if (bShowLabels)
		ShowLabels();
	else
		HideLabels();
}



//-----------------------------------------------------------------------------
void KSomMap::DrawInitMap(void)
{
	unsigned int i, cx, cy;
	double neuron_width;
	QColor color;
	unsigned int row_number;
	unsigned int col_number;

	//configure painter
	if (!Painter->isActive())
		Painter->begin(this);
	Painter->setPen(white);

	//compute the Radius of the neurons:
	ComputeRadius();

	//set the neuron width
	neuron_width=sqrt(3)*Radius; // distance E,C

	//draw the map
	for (i=0; i<(MapX*MapY); i++)
	{
		row_number=i/MapX;
		col_number=i%MapX;
		cy=Radius+int(1.5*Radius*row_number);
		cx=int(double(col_number)*neuron_width+0.5*neuron_width);
		if (row_number%2)
			cx=cx+int(0.5*neuron_width);
		color.setRgb(0,0,0);
		DrawNeuron(cx,cy, Radius, color);
	}
}


//-----------------------------------------------------------------------------
void KSomMap::DrawInitNeuron(int i)
{
	QColor color;
	int row_number, col_number;
	int cx,cy;
	double neuron_width;

	neuron_width=sqrt(3)*Radius; // distance E,C
	color.setRgb(100,239,119); //fixed color for selected neuron
	row_number=int(i/MapX);
	col_number=int(i%MapX);
	cy=Radius+int(1.5*Radius*row_number);
	cx=int(double(col_number)*neuron_width+0.5*neuron_width);

	if (row_number%2)
			cx=cx+int(0.5*neuron_width);

	DrawNeuron(cx,cy,Radius, color);
}


//-----------------------------------------------------------------------------
 void KSomMap::DrawNeuron(int cx, int cy, int r, QColor color)
 {
	switch(Neuron_Type)
	{
		case(Rectangular) :
			DrawRectNeuron(cx,cy,r,color);
			break;
		case(Hexagonal) :
			DrawHexNeuron(cx,cy,r,color);
			break;
		case(Circle) :
			DrawCircleNeuron(cx,cy,r,color);
			break;
		default :
			DrawHexNeuron(cx,cy,r,color);
			break;
	}
 }

//-----------------------------------------------------------------------------
void KSomMap::DrawHexNeuron(int cx, int cy, int r, QColor color)
{
	QColor black(0,0,0);
	QPointArray pts(6);
	QPoint A(cx,cy-r);
	QPoint B(cx+int(sqrt(3)/2*r),cy-int(0.5*r));
	QPoint C(cx+int(sqrt(3)/2*r),cy+int(0.5*r));
	QPoint D(cx,cy+r);
	QPoint E(cx-int(sqrt(3)/2*r),cy+int(0.5*r));
	QPoint F(cx-int(sqrt(3)/2*r),cy-int(0.5*r));

	 pts.setPoint(0,A);
	 pts.setPoint(1,B);
	 pts.setPoint(2,C);
	 pts.setPoint(3,D);
	 pts.setPoint(4,E);
	 pts.setPoint(5,F);

	if (!Painter->isActive())
		Painter->begin(this);
	Painter->setBackgroundMode( Qt::OpaqueMode );
	Painter->setBrush(QBrush(color, Qt::SolidPattern ) );
	Painter->drawPolygon(pts);
	Painter->setBrush(QBrush(black, Qt::SolidPattern ) );
	Painter->drawPoint(cx,cy);
}


//-----------------------------------------------------------------------------
void KSomMap::DrawRectNeuron(int cx, int cy, int r, QColor color)
{
	QColor black(0,0,0);
	QPointArray pts(6);
//	QPoint A(cx,cy-r);
//	QPoint B(cx+int(sqrt(3)/2*r),cy-int(0.5*r));
	QPoint C(cx+int(sqrt(3)/2*r),cy+int(0.75*r));
//	QPoint D(cx,cy+r);
//	QPoint E(cx-int(sqrt(3)/2*r),cy+int(0.5*r));
	QPoint F(cx-int(sqrt(3)/2*r),cy-int(0.75*r));

	QRect rect(C,F);

	if (!Painter->isActive())
		Painter->begin(this);
	Painter->setBackgroundMode( Qt::OpaqueMode );
	Painter->setBrush(QBrush(color, Qt::SolidPattern ) );
	Painter->drawRect(rect);
	Painter->setBrush(QBrush(black, Qt::SolidPattern ) );
	Painter->drawPoint(cx,cy);
}



//-----------------------------------------------------------------------------
void KSomMap::DrawCircleNeuron(int cx, int cy, int r, QColor color)
{
	QColor black(0,0,0);
	QPointArray pts(6);
//	QPoint A(cx,cy-r);
//	QPoint B(cx+int(sqrt(3)/2*r),cy-int(0.5*r));
	QPoint C(cx+int(sqrt(3)/2*r),cy+int(0.75*r));
//	QPoint D(cx,cy+r);
//	QPoint E(cx-int(sqrt(3)/2*r),cy+int(0.5*r));
	QPoint F(cx-int(sqrt(3)/2*r),cy-int(0.75*r));

	QRect rect(C,F);

	if (!Painter->isActive())
		Painter->begin(this);
	Painter->setBackgroundMode( Qt::OpaqueMode );
	Painter->setBrush(QBrush(color, Qt::SolidPattern ) );
	Painter->drawEllipse(cx-int(sqrt(3)/2*r),cy-int(sqrt(3)/2*r),int(sqrt(3)*r),int(sqrt(3)*r));
	Painter->setBrush(QBrush(black, Qt::SolidPattern ) );
	Painter->drawPoint(cx,cy);
}


//-----------------------------------------------------------------------------
void KSomMap::SetColor(QColor* c)
{
	Color=c;

	//check wich color needs to vary
	if (Color->red()<=Color->green() &&Color->red()<=Color->blue())
		Color_Vary=Red;
	if (Color->green()<=Color->red() &&Color->green()<=Color->blue())
		Color_Vary=Green;
	if (Color->blue()<=Color->red() &&Color->blue()<=Color->green())
		Color_Vary=Blue;
}


//-----------------------------------------------------------------------------
void KSomMap::SetDim(unsigned int x, unsigned int y)
{
	unsigned int i;

	MapX=x;
	MapY=y;

	//init neurons
	Neurons=new neuron[MapX*MapY];
	for (i=0; i<(MapX*MapY);i++)
		Neurons[i].id=i;

	//init labels
	Labels=new QLabel* [MapX*MapY];
	for (i=0; i<(MapX*MapY);i++)
		Labels[i]=new QLabel(this);
}


//-----------------------------------------------------------------------------
void KSomMap::CreateLabel(unsigned int i, unsigned int cx, unsigned int cy, QColor color, unsigned int nblabels)
{
 	unsigned int  nb;
	const char* tmpstr;
	char tmp[500];
	char test[500];

	if (!Neurons[i].nblabels) return;
	tmpstr=Neurons[i].labels[0];
 	sprintf(test, "%s", tmpstr);
	nb=1;
	while(nb<nblabels && nb<Neurons[i].nblabels)
	{
		strcat(test,"<br>");
		strcat(test,Neurons[i].labels[nb]);
		nb++;
	}

	//compute the size of letter
	sprintf(tmp, "<center><font size='2'>%s</font></center>",test);

	//create the label
	Labels[i]->setText(tmp);
	Labels[i]->setBackgroundColor(color);
	Labels[i]->setGeometry(cx+1-int(0.5*sqrt(3)*Radius),cy-int(0.5*Radius), int(sqrt(3)*Radius)-1, Radius-1);
}


//-----------------------------------------------------------------------------
void KSomMap::SwitchShowLabels(void)
{
	bShowLabels=!bShowLabels;
}


//-----------------------------------------------------------------------------
void KSomMap::ShowLabels(void)
{
	unsigned int i;

	for (i=0; i<(MapX*MapY); i++)
		if (Neurons[i].nblabels)
			Labels[i]->show();
}


//-----------------------------------------------------------------------------
void KSomMap::HideLabels(void)
{
	unsigned int i;

	for (i=0; i<(MapX*MapY); i++)
		if (Neurons[i].nblabels)
			Labels[i]->hide();
}


//-----------------------------------------------------------------------------
void KSomMap::ComputeRadius()
{
	unsigned int r1, r2;

	//compute the Radius of the neurons:
	r1=int((2*width())/((2*MapX+1)*sqrt(3)));
	r2=int((2*height())/(3*MapY+1));
	Radius= (r1<r2) ? r1 : r2;
}


//-----------------------------------------------------------------------------
void KSomMap::InitNeuronData(unsigned int neuronid, unsigned int nbdata)
{
	// init neuron data structure
	Neurons[neuronid].datas=new data[nbdata];
	Neurons[neuronid].nbcontent=nbdata;

	//check the mindata and maxdata parameters
	if(nbdata>MaxNbData)
		MaxNbData=nbdata;
	if(nbdata<MinNbData)
		MinNbData=nbdata;
}


//-----------------------------------------------------------------------------
void KSomMap::InitNeuronLabels(unsigned int neuronid, unsigned int nblabels)
{
	Neurons[neuronid].nblabels=nblabels;
	if (nblabels==0)
		return;
 	Neurons[neuronid].labels=new QString[nblabels];
}


//-----------------------------------------------------------------------------
KSomMap::~KSomMap()
{
	//end painter
	if (Painter->isActive())
		Painter->end();
	delete Painter;

	if (Color)
		delete (Color);
	if (Neurons)
		delete[] Neurons;
	if (Labels)
		delete[] Labels;
}

