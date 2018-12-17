/*
 * @file       util.h
 * @brief      Utility function definitions.
 * @author     Elijah Schutz
 * @date       12/12/18
 */
#include <stdbool.h>

#define CEILDIV(x, y) ((x + y - 1) / y)

bool isdigits(char *, int);

char *slice(char *, int, int);
