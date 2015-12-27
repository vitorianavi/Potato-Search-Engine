#ifndef LIBSTRING_OPERATIONS_H
#define LIBSTRING_OPERATIONS_H

#include <ctype.h>
#include <unordered_map>
#include "libgeneral.h"

#ifndef validCh

#define validCh(ch)  (isalnum(ch)) 
#define invalidCh(ch) (!validCh(ch))

#endif

extern char  _is_num[256];
extern char  _valid[256];
extern char  _invalid[256];
extern unsigned char  _convert_with_accs[256];
extern unsigned char  _convert_without_accs[256];
extern unsigned char _should_convert;

void convertBufferWithoutSpaces(unsigned char *ch, unsigned char *end, char accent);
void startConvertWithoutAccents();
void startConvertWithAccents();
void startValid();
void convertBufferWithoutAccents(char *ch, char *end , char *new_);
void convertBufferWithAccents(unsigned char *ch, unsigned char *end);

int isUpperAccents(unsigned char c);
void strUpperAccent(unsigned char *str);

unsigned char charUpperAccent(unsigned char c);

#endif
