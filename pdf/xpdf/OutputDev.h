//========================================================================
//
// OutputDev.h
//
// Copyright 1996-2003 Glyph & Cog, LLC
//
//========================================================================

#ifndef OUTPUTDEV_H
#define OUTPUTDEV_H

#include <aconf.h>

#ifdef USE_GCC_PRAGMAS
#pragma interface
#endif

#include "gtypes.h"
#include "CharTypes.h"

class GString;
class GfxState;
class GfxColorSpace;
class GfxImageColorMap;
class Stream;
class Link;
class Catalog;

//------------------------------------------------------------------------
// OutputDev
//------------------------------------------------------------------------

class OutputDev {
public:

  // Constructor.
  OutputDev() {}

  // Destructor.
  virtual ~OutputDev() {}

  //----- get info about output device

  // Does this device use upside-down coordinates?
  // (Upside-down means (0,0) is the top left corner of the page.)
  virtual GBool upsideDown() = 0;

  // Does this device use drawChar() or drawString()?
  virtual GBool useDrawChar() = 0;

  // Does this device use beginType3Char/endType3Char?  Otherwise,
  // text in Type 3 fonts will be drawn with drawChar/drawString.
  virtual GBool interpretType3Chars() = 0;

  // Does this device need non-text content?
  virtual GBool needNonText() { return gTrue; }

  //----- initialization and control

  // Set default transform matrix.
  virtual void setDefaultCTM(double *ctm);

  // Start a page.
  virtual void startPage(int pageNum, GfxState *state) {}

  // End a page.
  virtual void endPage() {}

  // Dump page contents to display.
  virtual void dump() {}

  //----- coordinate conversion

  // Convert between device and user coordinates.
  virtual void cvtDevToUser(double dx, double dy, double *ux, double *uy);
  virtual void cvtUserToDev(double ux, double uy, int *dx, int *dy);

  //----- link borders
  virtual void drawLink(Link*, Catalog*) {}

  //----- save/restore graphics state
  virtual void saveState(GfxState*) {}
  virtual void restoreState(GfxState*) {}

  //----- update graphics state
  virtual void updateAll(GfxState*);
  virtual void updateCTM(GfxState*, double, double,double, double, double, double) {}
  virtual void updateLineDash(GfxState*) {}
  virtual void updateFlatness(GfxState*) {}
  virtual void updateLineJoin(GfxState*) {}
  virtual void updateLineCap(GfxState*) {}
  virtual void updateMiterLimit(GfxState*) {}
  virtual void updateLineWidth(GfxState*) {}
  virtual void updateFillColor(GfxState*) {}
  virtual void updateStrokeColor(GfxState*) {}
  virtual void updateFillOpacity(GfxState*) {}
  virtual void updateStrokeOpacity(GfxState*) {}

  //----- update text state
  virtual void updateFont(GfxState*) {}
  virtual void updateTextMat(GfxState*) {}
  virtual void updateCharSpace(GfxState*) {}
  virtual void updateRender(GfxState*) {}
  virtual void updateRise(GfxState*) {}
  virtual void updateWordSpace(GfxState*) {}
  virtual void updateHorizScaling(GfxState*) {}
  virtual void updateTextPos(GfxState*) {}
  virtual void updateTextShift(GfxState*, double) {}

  //----- path painting
  virtual void stroke(GfxState*) {}
  virtual void fill(GfxState*) {}
  virtual void eoFill(GfxState*) {}

  //----- path clipping
  virtual void clip(GfxState*) {}
  virtual void eoClip(GfxState*) {}

  //----- text drawing
  virtual void beginString(GfxState*,GString*) {}
  virtual void endString(GfxState*) {}
  virtual void drawChar(GfxState*, double, double,double, double,double, double,CharCode, Unicode*, int) {}
  virtual void drawString(GfxState*, GString*) {}
  virtual GBool beginType3Char(GfxState *state,CharCode code, Unicode *u, int uLen);
  virtual void endType3Char(GfxState*) {}

  //----- image drawing
  virtual void drawImageMask(GfxState *state, Object *ref, Stream *str,
			     int width, int height, GBool invert,
			     GBool inlineImg);
  virtual void drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 int *maskColors, GBool inlineImg);

#if OPI_SUPPORT
  //----- OPI functions
  virtual void opiBegin(GfxState *state, Dict *opiDict);
  virtual void opiEnd(GfxState *state, Dict *opiDict);
#endif

  //----- Type 3 font operators
  virtual void type3D0(GfxState*, double, double) {}
  virtual void type3D1(GfxState*, double, double,double, double, double, double) {}

  //----- PostScript XObjects
  virtual void psXObject(Stream*, Stream*) {}

private:

  double defCTM[6];		// default coordinate transform matrix
  double defICTM[6];		// inverse of default CTM
};

#endif
