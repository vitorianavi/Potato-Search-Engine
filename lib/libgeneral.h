#ifndef LIBGENERAL_H
#define LIBGENERAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include <iostream>

typedef struct {
	int number;
	char title[MAX_TITLE_SIZE];
	char major_sub[MAX_TEXT_SIZE];
	char minor_sub[MAX_TEXT_SIZE];
	char text[MAX_TEXT_SIZE];
} Record;

#endif