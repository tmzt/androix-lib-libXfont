/* $Xorg: private.c,v 1.4 2001/02/09 02:04:04 xorgcvs Exp $ */

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

#include    "fontmisc.h"
#include    "fontstruct.h"

int _FontPrivateAllocateIndex;

int
AllocateFontPrivateIndex ()
{
    return _FontPrivateAllocateIndex++;
}

void
ResetFontPrivateIndex ()
{
    _FontPrivateAllocateIndex = 0;
}

Bool
_FontSetNewPrivate (pFont, n, ptr)
    FontPtr pFont;
    int	    n;
    pointer ptr;
{
    pointer *new;

    if (n > pFont->maxPrivate)
    {
	new = (pointer *) xrealloc (pFont->devPrivates, (n + 1) * sizeof (pointer));
	if (!new)
	    return FALSE;
	pFont->devPrivates = new;
	/* zero out new, uninitialized privates */
	while(++pFont->maxPrivate < n)
	    pFont->devPrivates[pFont->maxPrivate] = (pointer)0;
    }
    pFont->devPrivates[n] = ptr;
    return TRUE;
}
