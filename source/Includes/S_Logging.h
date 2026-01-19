#ifndef KRN_LOGGING_H
#define KRN_LOGGING_H 1

#include <stdio.h>
#include <string.h>

#include "D_Common.h"

/* public methods */
unsigned int get_count_errors(void);


char *log_error(char *fmt, ...);
char *log_debug(char *fmt, ...);

#endif
