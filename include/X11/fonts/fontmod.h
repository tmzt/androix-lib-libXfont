/* $XFree86$ */

#ifndef _FONTMOD_H_
#define _FONTMOD_H_

typedef void (*InitFont)(void);

typedef struct {
    InitFont	initFunc;
    char *	name;
    pointer	module;
} FontModule;

extern FontModule *FontModuleList;

#endif /* _FONTMOD_H_ */
