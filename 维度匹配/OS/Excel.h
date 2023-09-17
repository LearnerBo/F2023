#ifndef EXCEL_H
#define EXCEL_H
#include "arm_math.h"
#include <stdint.h>
typedef struct
{
    int32_t min_val;
    int32_t max_val;
} Range;

typedef struct
{
    Range grid[6][6];
} Variable;

extern  Variable Sub[6];
#endif 