/* $Xorg: ffcheck.c,v 1.4 2001/02/09 02:04:03 xorgcvs Exp $ */

/*

Copyright 1991, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/

/*
 * Author:  Keith Packard, MIT X Consortium
 */
/* $NCDId: @(#)fontfile.c,v 1.6 1991/07/02 17:00:46 lemke Exp $ */

#include    "fntfilst.h"

/*
 * Map FPE functions to renderer functions
 */

extern int FontFileNameCheck();
extern int FontFileInitFPE();
extern int FontFileResetFPE();
extern int FontFileFreeFPE();
extern void FontFileCloseFont();


/* Here we must check the client to see if it has a context attached to
 * it that allows us to access the printer fonts
 */

int
FontFileCheckOpenFont (client, fpe, flags, name, namelen, format, fmask,
		  id, pFont, aliasName, non_cachable_font)
    pointer		client;
    FontPathElementPtr	fpe;
    int			flags;
    char		*name;
    int			namelen;
    fsBitmapFormat	format;
    fsBitmapFormatMask	fmask;
    XID			id;
    FontPtr		*pFont;
    char		**aliasName;
    FontPtr		non_cachable_font;
{
    if (XpClientIsBitmapClient(client))
	return (FontFileOpenFont  (client, fpe, flags, name, namelen, format, 
		fmask, id, pFont, aliasName, non_cachable_font));
    return BadFontName;
}

int
FontFileCheckListFonts (client, fpe, pat, len, max, names)
    pointer     client;
    FontPathElementPtr fpe;
    char       *pat;
    int         len;
    int         max;
    FontNamesPtr names;
{
    if (XpClientIsBitmapClient(client))
	return FontFileListFonts (client, fpe, pat, len, max, names);
    return BadFontName;
}

int
FontFileCheckStartListFontsWithInfo(client, fpe, pat, len, max, privatep)
    pointer     client;
    FontPathElementPtr fpe;
    char       *pat;
    int         len;
    int         max;
    pointer    *privatep;
{
    if (XpClientIsBitmapClient(client))
	return FontFileStartListFontsWithInfo(client, fpe, pat, len, 
				max, privatep);
    return BadFontName;
}

int
FontFileCheckListNextFontWithInfo(client, fpe, namep, namelenp, pFontInfo,
			     numFonts, private)
    pointer		client;
    FontPathElementPtr	fpe;
    char		**namep;
    int			*namelenp;
    FontInfoPtr		*pFontInfo;
    int			*numFonts;
    pointer		private;
{
    if (XpClientIsBitmapClient(client))
	return FontFileListNextFontWithInfo(client, fpe, namep, namelenp, 
				pFontInfo, numFonts, private);
    return BadFontName;
}

int
FontFileCheckStartListFontsAndAliases(client, fpe, pat, len, max, privatep)
    pointer     client;
    FontPathElementPtr fpe;
    char       *pat;
    int         len;
    int         max;
    pointer    *privatep;
{
    if (XpClientIsBitmapClient(client))
	return FontFileStartListFontsAndAliases(client, fpe, pat, len, 
				max, privatep);
    return BadFontName;
}

int
FontFileCheckListNextFontOrAlias(client, fpe, namep, namelenp, resolvedp,
			    resolvedlenp, private)
    pointer		client;
    FontPathElementPtr	fpe;
    char		**namep;
    int			*namelenp;
    char		**resolvedp;
    int			*resolvedlenp;
    pointer		private;
{
    if (XpClientIsBitmapClient(client))
	return FontFileListNextFontOrAlias(client, fpe, namep, namelenp, 
				resolvedp, resolvedlenp, private);
    return BadFontName;
}

extern void FontFileEmptyBitmapSource();
typedef int (*IntFunc) ();
static int  font_file_check_type;

void
FontFileCheckRegisterFpeFunctions ()
{
    BitmapRegisterFontFileFunctions ();

#ifndef LOWMEMFTPT

#ifndef CRAY
    SpeedoRegisterFontFileFunctions ();
    Type1RegisterFontFileFunctions();
#endif

#endif /* ifndef LOWMEMFTPT */

    font_file_check_type = RegisterFPEFunctions(FontFileNameCheck,
				  FontFileInitFPE,
				  FontFileFreeFPE,
				  FontFileResetFPE,
				  FontFileCheckOpenFont,
				  FontFileCloseFont,
				  FontFileCheckListFonts,
				  FontFileCheckStartListFontsWithInfo,
				  FontFileCheckListNextFontWithInfo,
				  (IntFunc) 0,
				  (IntFunc) 0,
				  (IntFunc) 0,
				  FontFileCheckStartListFontsAndAliases,
				  FontFileCheckListNextFontOrAlias,
				  FontFileEmptyBitmapSource);
}
