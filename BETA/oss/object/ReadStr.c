/***************************************
*
* ReadStr.c
*
*******************/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmp_s.h"

#include "ATOMATRIX.h"



/* read in a string at current file position, string ends with ^C */
string ReadStr(FILE *F)
{
	extern void*		(*allocm)(unsigned int);
	char			buf[BUFSIZ];
	int			i;
	string                  ptr;
	int			cnt = 0;
	int			r = 0;
	long			pos;

	pos = ftell(F);
	while (fgets(buf, BUFSIZ, F) != (char*)0)
	{
		r++;
		i = 0;
	RSLOOP:	/* use goto instead of while() so the break works correctly */
		if (i == BUFSIZ)
			continue;
		if (buf[i] == 3)
			break;
		if (buf[i] == 0)
			continue;
		cnt++;
		i++;
		goto RSLOOP;
	}
	ptr = allocm(cnt + 1);
	if (r == 1)
		strncpy(ptr, buf, cnt + 1);
	else {
		fseek(F, pos, 0);
		fread(ptr, cnt, 1, F);
		getc(F); /* skip ^C */
		getc(F); /* skip LF */
	}
	ptr[cnt] = 0;
	return ptr;
}
