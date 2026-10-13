#ifndef FASTMATHTABLES_H
#define FASTMATHTABLES_H

// #include "Basic_Macros.h"

#define CS_FULL_TABLE_SIZE         (16384)
#define CS_FULL_TABLE_MASK         (CS_FULL_TABLE_SIZE - 1)
#define CS_HALF_TABLE_MAX_INDEX    (CS_FULL_TABLE_SIZE / 2)
#define CS_QUARTER_TABLE_MAX_INDEX (CS_FULL_TABLE_SIZE / 4)
#define CS_QUARTER_TABLE_SIZE      (CS_QUARTER_TABLE_MAX_INDEX + 1)
#define CS_INV_PREC                ((2 * CS_QUARTER_TABLE_MAX_INDEX) / PI)
extern const float CS_TABLE[CS_QUARTER_TABLE_SIZE];

#define ATAN_INV_PREC   (8191)
#define ATAN_TABLE_SIZE (8192)
extern const float ATAN_TABLE[ATAN_TABLE_SIZE];

#define EXP_INV_PREC         (100)
#define EXP_TABLE_SIZE       (2900)
#define LOWEST_DOMAIN_FLOAT  (-18.99f)
#define MAG_LOWEST_DOMAIN    (1899)
#define HIGHEST_DOMAIN_FLOAT (10.00f)
#define HIGHEST_DOMAIN       (1000)
extern const float EXP_TABLE[EXP_TABLE_SIZE];

#endif
