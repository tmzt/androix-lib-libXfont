/* $Xorg: fontmisc.h,v 1.4 2001/02/09 02:04:04 xorgcvs Exp $ */

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

#ifndef _FONTMISC_H_
#define _FONTMISC_H_

#include <X11/Xfuncs.h>

#ifndef X_NOT_STDC_ENV
#include <stdlib.h>
#else
extern int rand();
#endif
#include <stdio.h>

#ifndef X_NOT_POSIX
#include <unistd.h>
#else
extern int close();
#endif

typedef unsigned char	*pointer;
typedef int		Bool;

#ifndef X_PROTOCOL
#ifndef _XSERVER64
typedef unsigned long	Atom;
typedef unsigned long	XID;
#else
#include <X11/Xmd.h>
typedef CARD32 XID;
typedef CARD32 Atom;
#endif 
#endif

#ifndef LSBFirst
#define LSBFirst	0
#define MSBFirst	1
#endif

#ifndef None
#define None	0l
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

extern char	    *NameForAtom ();

#define xalloc(n)   Xalloc ((unsigned) n)
#define xfree(p)    Xfree ((pointer) p)
#define xrealloc(p,n)	Xrealloc ((pointer)p,n)
#define lowbit(x) ((x) & (~(x) + 1))

#define assert(x)

extern void
BitOrderInvert(
#if NeedFunctionPrototypes
    register unsigned char *,
    register int
#endif
);

extern void
TwoByteSwap(
#if NeedFunctionPrototypes
    register unsigned char *,
    register int
#endif
);

extern void
FourByteSwap(
#if NeedFunctionPrototypes
    register unsigned char *,
    register int
#endif
);

extern int
RepadBitmap (
#if NeedFunctionPrototypes
    char*, 
    char*,
    unsigned, 
    unsigned,
    int, 
    int
#endif
);

#endif /* _FONTMISC_H_ */
