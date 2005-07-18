//========================================================================
//
// TextBufOutputDev.h
//
// Copyright 1997-2003 Glyph & Cog, LLC
//
// Modify by Pascal Francq (pfrancq@ulb.ac.be)
//
//========================================================================

#ifndef TEXTBUFOUTPUTDEV_H
#define TEXTBUFOUTPUTDEV_H

#include <aconf.h>
#include <rstd.h>
#include <rstring.h>

#ifdef USE_GCC_PRAGMAS
#pragma interface
#endif

#include <stdio.h>
#include "gtypes.h"
#include "GfxFont.h"
#include "OutputDev.h"

class GString;
class GList;
class GfxFont;
class GfxState;



//------------------------------------------------------------------------
typedef void (*TextBufOutputFunc)(void *stream, char *text, int len);


//------------------------------------------------------------------------
// TextBufFontInfo
//------------------------------------------------------------------------

class TextBufFontInfo {
public:

  TextBufFontInfo(GfxState *state);
  ~TextBufFontInfo();

  GBool matches(GfxState *state);

private:

  GfxFont *gfxFont;
  double horizScaling;

  double minSpaceWidth;		// min width for inter-word space, as a
				//   fraction of the font size
  double maxSpaceWidth;		// max width for inter-word space, as a
				//   fraction of the font size


  friend class TextBufWord;
  friend class TextBufPage;
};

//------------------------------------------------------------------------
// TextBufWord
//------------------------------------------------------------------------

class TextBufWord {
public:

  // Constructor.
  TextBufWord(GfxState *state, double x0, double y0, int charPosA,
	   TextBufFontInfo *fontA, double fontSize);


  // Destructor.
  ~TextBufWord();

  // Add a character to the word.
  void addChar(GfxState *state, double x, double y,
	       double dx, double dy, Unicode u);


private:

  GBool xyBefore(TextBufWord *word2);
  void merge(TextBufWord *word2);

  double xMin, xMax;		// bounding box x coordinates
  double yMin, yMax;		// bounding box y coordinates
  double yBase;			// baseline y coordinate
  Unicode *text;		// the text
  double *xRight;		// right-hand x coord of each char
  int len;			// length of text and xRight
  int size;			// size of text and xRight arrays
  int charPos;                  // character position (within content stream)
  int charLen;                  // number of content stream characters in
                                //   this word
  TextBufFontInfo *font;		// font information
  double fontSize;		// font size
  GBool spaceAfter;		// set if there is a space between this
				//   word and the next word on the line
  TextBufWord *next;		// next word in line (before lines are
				//   assembled: next word in xy order)


  friend class TextBufLine;
  friend class TextBufPage;
};

//------------------------------------------------------------------------
// TextBufLine
//------------------------------------------------------------------------

class TextBufLine {
public:

  TextBufLine();
  ~TextBufLine();

private:

  GBool yxBefore(TextBufLine *line2);
  void merge(TextBufLine *line2);

  double xMin, xMax;		// bounding box x coordinates
  double yMin, yMax;		// bounding box y coordinates
  double yBase;			// primary baseline y coordinate
  double xSpaceL, xSpaceR;	// whitespace to left and right of this line
  TextBufFontInfo *font;		// primary font
  double fontSize;		// primary font size
  TextBufWord *words;		// words in this line
  TextBufWord *lastWord;		// last word in this line
  Unicode *text;		// Unicode text of the line, including
				//   spaces between words
  double *xRight;		// right-hand x coord of each Unicode char
  int *col;			// starting column number of each Unicode char
  int len;			// number of Unicode chars
  int convertedLen;		// total number of converted characters
  GBool hyphenated;		// set if last char is a hyphen
  TextBufLine *pageNext;		// next line on page
  TextBufLine *next;		// next line in block
  TextBufLine *flowNext;		// next line in flow

  friend class TextBufBlock;
  friend class TextBufPage;
};

//------------------------------------------------------------------------
// TextBufBlock
//------------------------------------------------------------------------

class TextBufBlock {
public:

  TextBufBlock();
  ~TextBufBlock();

private:

  GBool yxBefore(TextBufBlock *blk2);
  void mergeRight(TextBufBlock *blk2);
  void mergeBelow(TextBufBlock *blk2);

  double xMin, xMax;		// bounding box x coordinates
  double yMin, yMax;		// bounding box y coordinates
  double xSpaceL, xSpaceR;	// whitespace to left and right of this block
  double ySpaceT, ySpaceB;	// whitespace above and below this block
  double maxFontSize;		// max primary font size
  TextBufLine *lines;		// lines in block
  TextBufBlock *next;		// next block in flow
  TextBufBlock *stackNext;		// next block on traversal stack

  friend class TextBufFlow;
  friend class TextBufPage;
};

//------------------------------------------------------------------------
// TextBufFlow
//------------------------------------------------------------------------

class TextBufFlow {
public:

  TextBufFlow();
  ~TextBufFlow();

private:

  double yMin, yMax;		// bounding box y coordinates
  double ySpaceT, ySpaceB;	// whitespace above and below this flow
  TextBufBlock *blocks;		// blocks in flow
  TextBufLine *lines;		// lines in flow
  TextBufFlow *next;		// next flow on page

  friend class TextBufPage;
};


//------------------------------------------------------------------------
// TextBufPage
//------------------------------------------------------------------------

class TextBufPage {
public:

  // Constructor.
  TextBufPage(GBool rawOrder);

  // Destructor.
  ~TextBufPage();

  // Update the current font.
  void updateFont(GfxState *state);


  // Begin a new word.
  void beginWord(GfxState *state, double x0, double y0);

  // Add a character to the current word.
  void addChar(GfxState *state, double x, double y,
	       double dx, double dy,
	       CharCode c, Unicode *u, int uLen);

  // End the current word, sorting it into the list of words.
  void endWord();

  // Add a word, sorting it into the list of words.
  void addWord(TextBufWord *word);


  // Coalesce strings that look like parts of the same line.
  void coalesce(GBool physLayout);

  // Find a string.  If <top> is true, starts looking at top of page;
  // otherwise starts looking at <xMin>,<yMin>.  If <bottom> is true,
  // stops looking at bottom of page; otherwise stops looking at
  // <xMax>,<yMax>.  If found, sets the text bounding rectangle and
  // returns true; otherwise returns false.
  GBool findTextBuf(Unicode *s, int len,
		 GBool top, GBool bottom,
		 double *xMin, double *yMin,
		 double *xMax, double *yMax);

  // Get the text which is inside the specified rectangle.
  GString *getTextBuf(double xMin, double yMin,
		   double xMax, double yMax);

  // Find a string by character position and length.  If found, sets
  // the text bounding rectangle and returns true; otherwise returns
  // false.
  GBool findCharRange(int pos, int length,
		      double *xMin, double *yMin,
		      double *xMax, double *yMax);

  // Dump contents of page to a file.
  void dump(void *outputStream, TextBufOutputFunc outputFunc,
	    GBool physLayout);

  // Start a new page.
  void startPage(GfxState *state);


private:

  void clear();
  double lineFit(TextBufLine *line, TextBufWord *word, double *space);
  GBool lineFit2(TextBufLine *line0, TextBufLine *line1);
  GBool blockFit(TextBufBlock *blk, TextBufLine *line);
  GBool blockFit2(TextBufBlock *blk0, TextBufBlock *blk1);
  GBool flowFit(TextBufFlow *flow, TextBufBlock *blk);

  GBool rawOrder;		// keep text in content stream order

  double pageWidth, pageHeight;	// width and height of current page
  TextBufWord *curWord;		// currently active string
  int charPos;			// next character position (within content
				//   stream)
  TextBufFontInfo *font;		// current font
  double fontSize;		// current font size
  int nest;			// current nesting level (for Type 3 fonts)
  int nTinyChars;		// number of "tiny" chars seen so far

  TextBufWord *words;		// words, in xy order (before they're
				//   sorted into lines)
  TextBufWord *wordPtr;		// cursor for the word list

  TextBufLine *lines;		// lines, in xy order
  TextBufFlow *flows;		// flows, in reading order

  GList *fonts;			// all font info objects used on this
				//   page [TextBufFontInfo]


};

//------------------------------------------------------------------------
// TextBufOutputDev
//------------------------------------------------------------------------

class TextBufOutputDev: public OutputDev {
public:
	static R::RChar* RCharBuffer;
	static unsigned int RCharLen;
	static unsigned int RCharMax;

  // Open a text output file.  If <fileName> is NULL, no file is
  // written (this is useful, e.g., for searching text).  If
  // <physLayoutA> is true, the original physical layout of the text
  // is maintained.  If <rawOrder> is true, the text is kept in
  // content stream order.
  TextBufOutputDev(GBool physLayoutA,
		GBool rawOrderA, GBool append);

  // Destructor.
  virtual ~TextBufOutputDev();

  // Check if file was successfully created.
  virtual GBool isOk() { return ok; }

  //---- get info about output device

  // Does this device use upside-down coordinates?
  // (Upside-down means (0,0) is the top left corner of the page.)
  virtual GBool upsideDown() { return gTrue; }

  // Does this device use drawChar() or drawString()?
  virtual GBool useDrawChar() { return gTrue; }

  // Does this device use beginType3Char/endType3Char?  Otherwise,
  // text in Type 3 fonts will be drawn with drawChar/drawString.
  virtual GBool interpretType3Chars() { return gFalse; }

  // Does this device need non-text content?
  virtual GBool needNonTextBuf() { return gFalse; }

  //----- initialization and control

  // Start a page.
  virtual void startPage(int pageNum, GfxState *state);

  // End a page.
  virtual void endPage();

  //----- update text state
  virtual void updateFont(GfxState *state);

  //----- text drawing
  virtual void beginString(GfxState *state, GString *s);
  virtual void endString(GfxState *state);
  virtual void drawChar(GfxState *state, double x, double y,
			double dx, double dy,
			double originX, double originY,
			CharCode c, Unicode *u, int uLen);

  //----- path painting

  //----- special access

  // Find a string.  If <top> is true, starts looking at top of page;
  // otherwise starts looking at <xMin>,<yMin>.  If <bottom> is true,
  // stops looking at bottom of page; otherwise stops looking at
  // <xMax>,<yMax>.  If found, sets the text bounding rectangle and
  // returns true; otherwise returns false.
  GBool findTextBuf(Unicode *s, int len,
		 GBool top, GBool bottom,
		 double *xMin, double *yMin,
		 double *xMax, double *yMax);

  // Get the text which is inside the specified rectangle.
  GString *getTextBuf(double xMin, double yMin,
		   double xMax, double yMax);

  // Find a string by character position and length.  If found, sets
  // the text bounding rectangle and returns true; otherwise returns
  // false.
  GBool findCharRange(int pos, int length,
		      double *xMin, double *yMin,
		      double *xMax, double *yMax);


private:

  TextBufOutputFunc outputFunc;	// output function
  TextBufPage *text;		// text for the current page
  GBool physLayout;		// maintain original physical layout when
				//   dumping text
  GBool rawOrder;		// keep text in content stream order
  GBool ok;			// set up ok?

};

#endif
