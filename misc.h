//
//	Routines used by the entire operating system.
//

#ifndef	_misc_h_
#define	_misc_h_

extern char*	dstrcpy(char*, const char*);
extern char*	dstrncpy(char*, const char*, int n);
extern const char *dstrstr (const char *, const char *);
extern int	dstrlen (const char *);
extern int	dstrncmp (const char *, const char *, int);
extern int	dstrtol(char*, char**, int);
extern const char*	dindex(const char*, int);
extern const char*	dmindex(const char*, const char *);

inline
int
isspace (char c)
{
  return ((c == ' ') || (c == '\t') || (c == '\n'));
}

inline
int
isxdigit (char c)
{
  return (((c >= '0') && (c <= '9')) ||
	  ((c >= 'a') && (c <= 'f')) ||
	  ((c >= 'A') && (c <= 'A')));
}

#endif	// !_misc_h_
