/**********************************************************************\
*                Copyright (C) Michael Kerrisk, 2010.                  *
*                                                                      *
* This program is free software. You may use, modify, and redistribute *
* it under the terms of the GNU Affero General Public License as       *
* published by the Free Software Foundation, either version 3 or (at   *
* your option) any later version. This program is distributed without  *
* any warranty. See the file COPYING for details.                      *
\**********************************************************************/

/* print_rusage.h

   Header file for print_rusage.c.
*/
#ifndef PRINT_RUSAGE_H      /* Prevent accidental double inclusion */
#define PRINT_RUSAGE_H

#include <sys/resource.h>

void printRusage(const char *leader, const struct rusage *ru);

#endif
