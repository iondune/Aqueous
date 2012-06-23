#if defined(WITH_COMMENTS)
/*
 * fintrf.h	- MATLAB/FORTRAN interface header file. This file
 *		  contains the declaration of the pointer type needed
 *		  by the MATLAB/FORTRAN interface.
 *
 * Copyright 1984-2004 The MathWorks, Inc.
 * All Rights Reserved.
 * $Revision: 1.11.6.5 $  $Date: 2005/01/29 08:42:29 $
 */
#endif
#if defined(__ia64__) || defined(__x86_64__) || defined(_M_AMD64)
#define mwpointer integer*8
#define MWPOINTER INTEGER*8
#else
#define mwpointer integer
#define MWPOINTER INTEGER
#endif

