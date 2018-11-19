/*
**  stctok() -- public domain by Ray Gardner, modified by Bob Stout.
**              Renamed to stctok from stptok and modified by Brian Staton
**
**   You pass this function a string to parse, a buffer to receive the
**   "token" that gets scanned, the length of the buffer, and a string of
**   "break" characters that stop the scan.  It will copy the string into
**   the buffer up to any of the break characters, or until the buffer is
**   full, and will always leave the buffer null-terminated.  It will
**   return a pointer to the first non-breaking character after the one
**   that stopped the scan.
**
**   Back-to-back break characters can be handled one of two ways.
**   collapseFlag == 0 will return a zero-length NULL when back-to-back breaks
**   are encountered.  The return char* advances one position in the string.
**   collapseFlag != 0 will advance past all back-to-back break characters.
*/
#ifndef _STCTOK_H_
#define _STCTOK_H_

#include <string.h>
#include <stdlib.h>

char *stctok(const char *s, char *tok, size_t toklen, char *brk, int collapseFlag);
#endif



