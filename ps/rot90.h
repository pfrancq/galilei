const char *rot90[] = {
  "% Copyright (C) 1995, Digital Equipment Corporation.\n",
  "% All rights reserved.\n",
  "% See the file pstotext.txt for a full description.\n",
  "%\n",
  "% Lectern:\n",
  "%   PostScript to be prepended to a job to rotate its images by //angle,\n",
  "%   adjusting the page shape appropriately.  This works only for\n",
  "%   devices (such as ppmraw) that allow arbitrary imaging areas.\n",
  "\n",
  "% Last modified on Thu Aug  1 11:36:25 PDT 1996 by mcjones\n",
  "%      modified on Tue Mar 14 14:55:13 PST 1995 by birrell\n",
  "\n",
  "4 dict begin\n",
  "/myDict currentdict def\n",
  "/angle 90 def\n",
  "\n",
  "/languagelevel where { pop languagelevel 2 ge } { FALSE } ifelse \n",
  "  {\n",
  "  % Use setpagedevice for level 2 implementations\n",
  "  /lastW 0 def\n",
  "  /lastH 0 def\n",
  "  <<\n",
  "    /BeginPage {\n",
  "      //myDict begin\n",
  "      pop % page number\n",
  "      currentpagedevice /PageSize get aload pop % stack: W H\n",
  "%      (H = ) print dup =\n",
  "%      (W = ) print 1 index = flush\n",
  "%      (lastH = ) print lastH =\n",
  "%      (lastW = ) print lastW = flush\n",
  "      lastH ne exch lastW ne or {\n",
  "        % /PageSize has changed: rotate it\n",
  "        <<\n",
  "          /PageSize [\n",
  "            currentpagedevice /PageSize get aload pop % stack: W H\n",
  "            exch % stack: newW newH\n",
  "            /lastW 2 index def\n",
  "            /lastH 1 index def\n",
  "            ]\n",
  "          >> setpagedevice % calls BeginPage recursively\n",
  "%        (H := ) print lastH =\n",
  "%        (W := ) print lastW = flush\n",
  "        } {\n",
  "        % Rotate ctm.  Don't do this if we're also adjusting PageSize,\n",
  "        % because that causes another BeginPage call, which does this.\n",
  "        currentpagedevice /PageSize get aload pop % stack: width height\n",
  "        2 div exch 2 div exch % stack: center\n",
  "        2 copy translate //angle rotate neg exch neg translate\n",
  "        } ifelse\n",
  "      end % //myDict\n",
  "      }\n",
  "    >> setpagedevice\n",
  "  } {\n",
  "  % Use ghostscript specific code for earlier systems\n",
  "  % flip current frame buffer\n",
  "    currentdevice getdeviceprops >> /HWSize get\n",
  "    aload pop exch 2 array astore\n",
  "    mark exch /HWSize exch currentdevice putdeviceprops pop\n",
  "  % fix showpage to add a rotation and translation\n",
  "    /adbFixup {\n",
  "        currentdevice getdeviceprops >>\n",
  "        dup /HWSize get aload pop 3 -1 roll\n",
  "        /HWResolution get aload pop\n",
  "        % stack: xsize ysize xres yres\n",
  "        exch 4 -1 roll exch div 72 mul\n",
  "        % stack: ysize yres xpts\n",
  "        3 1 roll div 72 mul\n",
  "        % stack: xpts ypts\n",
  "        2 div exch 2 div exch\n",
  "        2 copy translate //angle rotate neg exch neg translate\n",
  "        } bind odef\n",
  "    /adbShowpage /showpage load def\n",
  "    userdict begin /showpage { adbShowpage adbFixup } bind odef end\n",
  "    adbFixup\n",
  "  % define new \".setpagesize\" that flips its arguments\n",
  "    statusdict begin\n",
  "        /adbSet /.setpagesize load def\n",
  "        /.setpagesize {\n",
  "          exch adbSet adbFixup\n",
  "          } bind def\n",
  "        end\n",
  "  } ifelse\n",
  "\n",
  "end % //myDict\n",
  0
};
