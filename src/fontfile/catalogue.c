/*
 * Copyright © 2007 Red Hat, Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Author:
 *   Kristian Høgsberg <krh@redhat.com>
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <X11/fonts/fntfilst.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

static const char CataloguePrefix[] = "catalogue:";

static int CatalogueFreeFPE (FontPathElementPtr fpe);

static int
CatalogueNameCheck (char *name)
{
    return strncmp(name, CataloguePrefix, sizeof(CataloguePrefix) - 1) == 0;
}

typedef struct _CatalogueRec {
    time_t mtime;
    int fpeCount, fpeAlloc;
    FontPathElementPtr *fpeList;
} CatalogueRec, *CataloguePtr;

static int
CatalogueAddFPE (CataloguePtr cat, FontPathElementPtr fpe)
{
    FontPathElementPtr *new;

    if (cat->fpeCount >= cat->fpeAlloc)
    {
	if (cat->fpeAlloc == 0)
	    cat->fpeAlloc = 16;
	else
	    cat->fpeAlloc *= 2;
	
	new = xrealloc(cat->fpeList,
		       cat->fpeAlloc * sizeof(FontPathElementPtr));
	if (new == NULL)
	    return AllocError;

	cat->fpeList = new;
    }

    cat->fpeList[cat->fpeCount++] = fpe;

    return Successful;
}

static const char PriorityAttribute[] = "pri=";

static int
ComparePriority(const void *p1, const void *p2)
{
    FontDirectoryPtr dir1 = (*(FontPathElementPtr*) p1)->private;
    FontDirectoryPtr dir2 = (*(FontPathElementPtr*) p2)->private;
    const char *pri1 = strstr(dir1->attributes, PriorityAttribute);
    const char *pri2 = strstr(dir2->attributes, PriorityAttribute);

    if (pri1 == NULL && pri2 == NULL)
	return 0;
    else if (pri1 == NULL)
	return 1;
    else if (pri2 == NULL)
	return -1;
    else
	return
	    atoi(pri1 + strlen(PriorityAttribute)) -
	    atoi(pri2 + strlen(PriorityAttribute));
}

static void
CatalogueUnrefFPEs (FontPathElementPtr fpe)
{
    CataloguePtr	cat = fpe->private;
    FontPathElementPtr	subfpe;
    int			i;

    for (i = 0; i < cat->fpeCount; i++)
    {
	subfpe = cat->fpeList[i];
	subfpe->refcount--;
	if (subfpe->refcount == 0)
	{
	    FontFileFreeFPE (subfpe);
	    xfree(subfpe->name);
	    xfree(subfpe);
	}
    }

    cat->fpeCount = 0;
}

static int
CatalogueRescan (FontPathElementPtr fpe)
{
    CataloguePtr	cat = fpe->private;
    char		link[MAXFONTFILENAMELEN];
    char		dest[MAXFONTFILENAMELEN];
    char		*attrib;
    FontPathElementPtr	subfpe;
    struct stat		statbuf;
    const char		*path;
    DIR			*dir;
    struct dirent	*entry;
    int			len;

    path = fpe->name + strlen(CataloguePrefix);
    if (stat(path, &statbuf) < 0 || !S_ISDIR(statbuf.st_mode))
	return BadFontPath;

    if (statbuf.st_mtime <= cat->mtime)
	return Successful;

    dir = opendir(path);
    if (dir == NULL)
    {
	xfree(cat);
	return BadFontPath;
    }

    CatalogueUnrefFPEs (fpe);
    while (entry = readdir(dir), entry != NULL)
    {
	snprintf(link, sizeof link, "%s/%s", path, entry->d_name);
	len = readlink(link, dest, sizeof dest);
	if (len < 0)
	    continue;

	attrib = strchr(link, ':');
	if (attrib && len + strlen(attrib) < sizeof dest)
	{
	    memcpy(dest + len, attrib, strlen(attrib));
	    len += strlen(attrib);
	}

	subfpe = xalloc(sizeof *subfpe);
	if (subfpe == NULL)
	    continue;

	/* The fonts returned by OpenFont will point back to the
	 * subfpe they come from.  So set the type of the subfpe to
	 * what the catalogue fpe was assigned, so calls to CloseFont
	 * (which uses font->fpe->type) goes to CatalogueCloseFont. */
	subfpe->type = fpe->type;
	subfpe->name_length = len;
	subfpe->name = xalloc (len + 1);
	if (subfpe == NULL)
	{
	    xfree(subfpe);
	    continue;
	}

	memcpy(subfpe->name, dest, len);
	subfpe->name[len] = '\0';

	/* The X server will manipulate the subfpe ref counts
	 * associated with the font in OpenFont and CloseFont, so we
	 * have to make sure it's valid. */
	subfpe->refcount = 1;

	if (FontFileInitFPE (subfpe) != Successful)
	{
	    xfree(subfpe->name);
	    xfree(subfpe);
	    continue;
	}

	if (CatalogueAddFPE(cat, subfpe) != Successful)
	{
	    FontFileFreeFPE (subfpe);
	    xfree(subfpe);
	    continue;
	}
    }

    qsort(cat->fpeList,
	  cat->fpeCount, sizeof cat->fpeList[0], ComparePriority);

    cat->mtime = statbuf.st_mtime;

    return Successful;
}

static int
CatalogueInitFPE (FontPathElementPtr fpe)
{
    CataloguePtr	cat;

    cat = (CataloguePtr) xalloc(sizeof *cat);
    if (cat == NULL)
	return AllocError;

    fpe->private = (pointer) cat;
    cat->fpeCount = 0;
    cat->fpeAlloc = 0;
    cat->fpeList = NULL;
    cat->mtime = 0;

    return CatalogueRescan (fpe);
}

static int
CatalogueResetFPE (FontPathElementPtr fpe)
{
    /* Always just tell the caller to close and re-open */
    return FPEResetFailed;	
}

static int
CatalogueFreeFPE (FontPathElementPtr fpe)
{
    CataloguePtr	cat = fpe->private;

    /* If the catalogue is modified while the xserver has fonts open
     * from the previous subfpes, we'll unref the old subfpes when we
     * reload the catalogue, and the xserver will the call FreeFPE on
     * them once it drops its last reference. Thus, the FreeFPE call
     * for the subfpe ends up here and we just forward it to
     * FontFileFreeFPE. */

    if (!CatalogueNameCheck (fpe->name))
	return FontFileFreeFPE (fpe);

    CatalogueUnrefFPEs (fpe);
    xfree(cat->fpeList);
    xfree(cat);

    return Successful;
}

static int
CatalogueOpenFont (pointer client, FontPathElementPtr fpe, Mask flags, 
		   char *name, int namelen, 
		   fsBitmapFormat format, fsBitmapFormatMask fmask,
		   XID id, FontPtr *pFont, char **aliasName, 
		   FontPtr non_cachable_font)
{
    CataloguePtr cat = fpe->private;
    FontPathElementPtr subfpe;
    FontDirectoryPtr dir;
    int i, status;

    CatalogueRescan (fpe);

    for (i = 0; i < cat->fpeCount; i++)
    {
	subfpe = cat->fpeList[i];
	dir = subfpe->private;
	status = FontFileOpenFont(client, subfpe, flags,
				  name, namelen, format, fmask, id,
				  pFont, aliasName, non_cachable_font);
	if (status == Successful || status == FontNameAlias)
	    return status;
    }
	    
    return BadFontName;
}

static void
CatalogueCloseFont (FontPathElementPtr fpe, FontPtr pFont)
{
    /* Note: this gets called with the actual subfpe where we found
     * the font, not the catalogue fpe. */

    FontFileCloseFont(fpe, pFont);
}

static int
CatalogueListFonts (pointer client, FontPathElementPtr fpe, char *pat, 
		    int len, int max, FontNamesPtr names)
{
    CataloguePtr cat = fpe->private;
    FontPathElementPtr subfpe;
    FontDirectoryPtr dir;
    int i;

    CatalogueRescan (fpe);

    for (i = 0; i < cat->fpeCount; i++)
    {
	subfpe = cat->fpeList[i];
	dir = subfpe->private;
	FontFileListFonts(client, subfpe, pat, len, max, names);
    }

    return Successful;
}

int
FontFileStartListFonts(pointer client, FontPathElementPtr fpe, 
		       char *pat, int len, int max, 
		       pointer *privatep, int mark_aliases);

typedef struct _LFWIData {
    pointer		*privates;
    int			current;
} LFWIDataRec, *LFWIDataPtr;

static int
CatalogueStartListFonts(pointer client, FontPathElementPtr fpe, 
			char *pat, int len, int max, pointer *privatep,
			int mark_aliases)
{
    CataloguePtr	cat = fpe->private;
    LFWIDataPtr		data;
    int			ret, i, j;

    CatalogueRescan (fpe);

    data = (LFWIDataPtr) xalloc (sizeof *data +
				 sizeof data->privates[0] * cat->fpeCount);
    if (!data)
	return AllocError;
    data->privates = (pointer *) (data + 1);

    for (i = 0; i < cat->fpeCount; i++)
    {
	ret = FontFileStartListFonts(client, cat->fpeList[i], pat, len,
				     max, &data->privates[i], mark_aliases);
	if (ret != Successful)
	    goto bail;
    }

    data->current = 0;
    *privatep = (pointer) data;
    return Successful;

 bail:
    for (j = 0; j < i; j++)
	/* FIXME: we have no way to free the per-fpe privates. */;
    xfree (data);

    return AllocError;
}

static int
CatalogueStartListFontsWithInfo(pointer client, FontPathElementPtr fpe, 
				char *pat, int len, int max, 
				pointer *privatep)
{
    return CatalogueStartListFonts(client, fpe, pat, len, max, privatep, 0);
}

static int
CatalogueListNextFontWithInfo(pointer client, FontPathElementPtr fpe, 
			      char **namep, int *namelenp, 
			      FontInfoPtr *pFontInfo,
			      int *numFonts, pointer private)
{
    LFWIDataPtr		data = private;
    CataloguePtr	cat = fpe->private;
    int			ret;

    if (data->current == cat->fpeCount)
    {
	xfree(data);
	return BadFontName;
    }

    ret = FontFileListNextFontWithInfo(client, cat->fpeList[data->current],
				       namep, namelenp,
				       pFontInfo, numFonts,
				       data->privates[data->current]);
    if (ret == BadFontName)
    {
	data->current++;
	return CatalogueListNextFontWithInfo(client, fpe, namep, namelenp,
					     pFontInfo, numFonts, private);
    }

    return ret;
}

static int
CatalogueStartListFontsAndAliases(pointer client, FontPathElementPtr fpe, 
				  char *pat, int len, int max, 
				  pointer *privatep)
{
    return CatalogueStartListFonts(client, fpe, pat, len, max, privatep, 1);
}

static int
CatalogueListNextFontOrAlias(pointer client, FontPathElementPtr fpe, 
			     char **namep, int *namelenp, char **resolvedp,
			     int *resolvedlenp, pointer private)
{
    LFWIDataPtr		data = private;
    CataloguePtr	cat = fpe->private;
    int			ret;

    if (data->current == cat->fpeCount)
    {
	xfree(data);
	return BadFontName;
    }

    ret = FontFileListNextFontOrAlias(client, cat->fpeList[data->current],
				      namep, namelenp,
				      resolvedp, resolvedlenp,
				      data->privates[data->current]);
    if (ret == BadFontName)
    {
	data->current++;
	return CatalogueListNextFontOrAlias(client, fpe, namep, namelenp,
					    resolvedp, resolvedlenp, private);
    }

    return ret;
}

void
CatalogueRegisterLocalFpeFunctions (void)
{
    RegisterFPEFunctions(CatalogueNameCheck,
			 CatalogueInitFPE,
			 CatalogueFreeFPE,
			 CatalogueResetFPE,
			 CatalogueOpenFont,
			 CatalogueCloseFont,
			 CatalogueListFonts,
			 CatalogueStartListFontsWithInfo,
			 CatalogueListNextFontWithInfo,
			 NULL,
			 NULL,
			 NULL,
			 CatalogueStartListFontsAndAliases,
			 CatalogueListNextFontOrAlias,
			 FontFileEmptyBitmapSource);
}