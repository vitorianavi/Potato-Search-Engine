#include "libstring_operations.h"

char  _is_num[256];
char  _valid[256];
char  _invalid[256];
unsigned char  _convert_with_accs[256];
unsigned char  _convert_without_accs[256];
unsigned char _should_convert;
unsigned char _upper_case[256];

using namespace std;

void loadStopWordsHash(unordered_map<string, int>& swords_hash, const char *file_name) {
  FILE *file;
  char term_ch[50];
  string term_str;

  file = fopen(file_name, "r");
  while(fscanf(file, "%s", term_ch) == 1) {
    term_str = string(term_ch);
    swords_hash[term_str] = 1;
  }
  fclose(file);
}

void cleanText(char *buff, char *end, char *new_text, int size) {
  int cont_char = 0;
  
  while(!isalnum(*buff)) {
    buff++;
  }
  
  while(buff < end) {
    if(isalnum(*buff)) {
      *new_text = *buff;
      new_text++; buff++;       
      cont_char++;
      
      if(cont_char == size -1){ break; }
    }
    else{
      buff++;
    }
  }
  *new_text = 0;
}

void normText(char *str, char *str_norm) {
  char str_aux[MAX_TERM_SIZE];
  convertBufferWithoutAccents(str, &str[strlen(str)], str_aux);
  cleanText(str_aux, &str_aux[strlen(str_aux)], str_norm, MAX_TERM_SIZE);
}

void startValid() {
  unsigned x;

  for(x = 0; x < 128 ; x++) {
    _is_num[x]=0;
    if(validCh(x)) {
      _valid[x] = 1;
      _invalid[x] = 0;
    }
    else {
      _valid[x] = 0;
      _invalid[x] = 1;
    }
  }
  for( x = 128; x < 256; x++) {
    _is_num[x]=0;
    _valid[x] = 0;
    _invalid[x] = 1;
  }
  _valid[0]=_invalid[0]=0;
  for(x = 48; x < 58; x++) {
    _is_num[x] = 1;
  }
  _valid[224] = 1;  _invalid[224] = 0;
  _valid[225] = 1;  _invalid[225] = 0;
  _valid[226] = 1;  _invalid[226] = 0;
  _valid[227] = 1;  _invalid[227] = 0;
  _valid[228] = 1;  _invalid[228] = 0;
  _valid[229] = 1;  _invalid[229] = 0;
  _valid[231] = 1;  _invalid[231] = 0;
  _valid[232] = 1;  _invalid[232] = 0;
  _valid[233] = 1;  _invalid[233] = 0;
  _valid[234] = 1;  _invalid[234] = 0;
  _valid[235] = 1;  _invalid[235] = 0;
  _valid[236] = 1;  _invalid[236] = 0;
  _valid[237] = 1;  _invalid[237] = 0;
  _valid[238] = 1;  _invalid[238] = 0;
  _valid[239] = 1;  _invalid[239] = 0;
  _valid[241] = 1;  _invalid[241] = 0;
  _valid[242] = 1;  _invalid[242] = 0;
  _valid[243] = 1;  _invalid[243] = 0;
  _valid[244] = 1;  _invalid[244] = 0;
  _valid[245] = 1;  _invalid[245] = 0;
  _valid[246] = 1;  _invalid[246] = 0;
  _valid[249] = 1;  _invalid[249] = 0;
  _valid[250] = 1;  _invalid[250] = 0;
  _valid[251] = 1;  _invalid[251] = 0;
  _valid[252] = 1;  _invalid[252] = 0;
  _valid[253] = 1;  _invalid[253] = 0;
  _valid[255] = 1;  _invalid[255] = 0;
}

void startConvertWithoutAccents() {
  unsigned x;
  
  startValid();
  for(x = 1; x < 65 ; x++) {
    _convert_without_accs[x] = x;
  }
  for(x = 65; x < 91; x++) {
    _convert_without_accs[x] = x+32;
  }
  for(; x < 128; x++) {
    _convert_without_accs[x] = x;
  }
  _convert_without_accs[45] = 45; 
  _convert_without_accs[128] = 99; // 0x80
  _convert_without_accs[129] = 0; 
  _convert_without_accs[130] = 0;
  _convert_without_accs[131] = 0;
  _convert_without_accs[132] = 0;
  _convert_without_accs[133] = 32; //0x85
  _convert_without_accs[134] = 0;
  _convert_without_accs[135] = 32; // 87
  _convert_without_accs[136] = 0;
  _convert_without_accs[137] = 0;
  _convert_without_accs[138] = 0; // 0x8A
  _convert_without_accs[139] = 0;
  _convert_without_accs[140] = 0;
  _convert_without_accs[141] = 0; 
  _convert_without_accs[142] = 0;
  _convert_without_accs[143] = 0; // 8F
  _convert_without_accs[144] = 0; // 90
  _convert_without_accs[145] = 32;
  _convert_without_accs[146] = 32;
  _convert_without_accs[147] = 32;
  _convert_without_accs[148] = 32;
  _convert_without_accs[149] = 32; // 95
  _convert_without_accs[150] = 32;
  _convert_without_accs[151] = 32;
  _convert_without_accs[152] = 32;
  _convert_without_accs[153] = 32;
  _convert_without_accs[154] = 32; // 9A
  _convert_without_accs[155] = 0;
  _convert_without_accs[156] = 0;
  _convert_without_accs[157] = 0;
  _convert_without_accs[158] = 0;
  _convert_without_accs[159] = 0;  // 9F
  _convert_without_accs[160] = 32; // A0
  _convert_without_accs[161] = 161;
  _convert_without_accs[162] = 162;
  _convert_without_accs[163] = 163;
  _convert_without_accs[164] = 164;
  _convert_without_accs[165] = 165; //A5
  _convert_without_accs[166] = 166;
  _convert_without_accs[167] = 167;
  _convert_without_accs[168] = 168;
  _convert_without_accs[169] = 169;
  _convert_without_accs[170] = 32; //AA
  _convert_without_accs[171] = 171;
  _convert_without_accs[172] = 172;
  _convert_without_accs[173] = 173;
  _convert_without_accs[174] = 174;
  _convert_without_accs[175] = 175; //AF
  _convert_without_accs[176] = 176; //B0
  _convert_without_accs[177] = 177;
  _convert_without_accs[178] = 178;
  _convert_without_accs[179] = 179;
  _convert_without_accs[180] = 180;
  _convert_without_accs[181] = 181; //B5
  _convert_without_accs[182] = 182;
  _convert_without_accs[183] = 183;
  _convert_without_accs[184] = 184;
  _convert_without_accs[185] = 185;
  _convert_without_accs[186] = 186; // BA
  _convert_without_accs[187] = 187;
  _convert_without_accs[188] = 188;
  _convert_without_accs[189] = 189;
  _convert_without_accs[190] = 190;
  _convert_without_accs[191] = 191; //BF
  _convert_without_accs[192] = 97;  // C0
  _convert_without_accs[193] = 97;
  _convert_without_accs[194] = 97;
  _convert_without_accs[195] = 97;
  _convert_without_accs[196] = 97;
  _convert_without_accs[197] = 97; // C5
  _convert_without_accs[198] = 198;
  _convert_without_accs[199] = 99;
  _convert_without_accs[200] = 101;
  _convert_without_accs[201] = 101;
  _convert_without_accs[202] = 101; // CA
  _convert_without_accs[203] = 101;
  _convert_without_accs[204] = 105;
  _convert_without_accs[205] = 105;
  _convert_without_accs[206] = 105;
  _convert_without_accs[207] = 105; // CF
  _convert_without_accs[208] = 208; // D0
  _convert_without_accs[209] = 110;
  _convert_without_accs[210] = 111;
  _convert_without_accs[211] = 111;
  _convert_without_accs[212] = 111;
  _convert_without_accs[213] = 111; // D5
  _convert_without_accs[214] = 111;
  _convert_without_accs[215] = 215;
  _convert_without_accs[216] = 216;
  _convert_without_accs[217] = 117;
  _convert_without_accs[218] = 117; // DA
  _convert_without_accs[219] = 117;
  _convert_without_accs[220] = 117;
  _convert_without_accs[221] = 121;
  _convert_without_accs[222] = 122;
  _convert_without_accs[223] = 223; //DF
  _convert_without_accs[224] = 97 ; // E0
  _convert_without_accs[225] = 97 ;
  _convert_without_accs[226] = 97 ;
  _convert_without_accs[227] = 97 ;
  _convert_without_accs[228] = 97 ;
  _convert_without_accs[229] = 97 ; // E5
  _convert_without_accs[230] = 230;
  _convert_without_accs[231] = 99 ;
  _convert_without_accs[232] = 101;
  _convert_without_accs[233] = 101;
  _convert_without_accs[234] = 101; // EA
  _convert_without_accs[235] = 101;
  _convert_without_accs[236] = 105;
  _convert_without_accs[237] = 105;
  _convert_without_accs[238] = 105;
  _convert_without_accs[239] = 105; // EF
  _convert_without_accs[240] = 240; // F0
  _convert_without_accs[241] = 110;
  _convert_without_accs[242] = 111;
  _convert_without_accs[243] = 111;
  _convert_without_accs[244] = 111;
  _convert_without_accs[245] = 111; // F5
  _convert_without_accs[246] = 111;
  _convert_without_accs[247] = 247;
  _convert_without_accs[248] = 248;
  _convert_without_accs[249] = 117;
  _convert_without_accs[250] = 117; // FA
  _convert_without_accs[251] = 117;
  _convert_without_accs[252] = 117;
  _convert_without_accs[253] = 121;
  _convert_without_accs[254] = 254;
  _convert_without_accs[255] = 121; // FF

}

char convertBufferUTF8(char ch) {
	switch( (int)ch ) {
		case -92:{ return 'a'; break; }
		case -93:{ return 'a'; break; }
		case -94:{ return 'a'; break; }
		case -95:{ return 'a'; break; }
		case -96:{ return 'a'; break; }
		case -124:{ return 'a'; break; }
		case -125:{ return 'a'; break; }
		case -126:{ return 'a'; break; }
		case -127:{ return 'a'; break; }
		case -128:{ return 'a'; break; }
		case -85:{ return 'e'; break; }
		case -86:{ return 'e'; break; }
		case -87:{ return 'e'; break; }
		case -88:{ return 'e'; break; }
		case -117:{ return 'e'; break; }
		case -118:{ return 'e'; break; }
		case -119:{ return 'e'; break; }
		case -120:{ return 'e'; break; }
		case -81:{ return 'i'; break; }
		case -82:{ return 'i'; break; }
		case -83:{ return 'i'; break; }
		case -84:{ return 'i'; break; }
		case -113:{ return 'i'; break; }
		case -114:{ return 'i'; break; }
		case -115:{ return 'i'; break; }
		case -116:{ return 'i'; break; }
		case -74:{ return 'o'; break; }
		case -75:{ return 'o'; break; }
		case -76:{ return 'o'; break; }
		case -77:{ return 'o'; break; }
		case -78:{ return 'o'; break; }
		case -106:{ return 'o'; break; }
		case -107:{ return 'o'; break; }
		case -108:{ return 'o'; break; }
		case -109:{ return 'o'; break; }
		case -110:{ return 'o'; break; }
		case -68:{ return 'u'; break; }
		case -69:{ return 'u'; break; }
		case -70:{ return 'u'; break; }
		case -71:{ return 'u'; break; }
		case -100:{ return 'u'; break; }
		case -101:{ return 'u'; break; }
		case -102:{ return 'u'; break; }
		case -103:{ return 'u'; break; }
		case -89:{ return 'c'; break; }
		case -121:{ return 'c'; break; }
		default: { return ch; }
	}
}

void convertBufferWithoutAccents(char *ch, char *end, char *new_) {
  char ch_temp;

  startConvertWithoutAccents();

  while(ch < end) {
    if(*ch < 0) {
      if(*ch == -61) {
        ch++;
        *new_ = convertBufferUTF8(*ch);
        new_++;
      }
    }
    else{
      *new_ = _convert_without_accs[*ch];
      new_++;
    }	
    ch++;
  }
  *new_ = '\0';
}

void convertBufferWithoutSpaces(register unsigned char *ch, register unsigned char *end, char accent) {
    register unsigned char *ch2;
    
    if (accent){
        ch2 = ch;
        while(ch < end) {
            *ch2 = _convert_with_accs[*ch];
            ch2++;
            if(*ch == ' ') {
                do {
                    ch++;
                } while(*ch == ' ');
            }
            else {
                ch++;
            }
        }
        *ch2 = 0;
    } else {
        ch2 = ch;
        while(ch < end) {
            *ch2 = _convert_without_accs[*ch];
            ch2++;
            if(*ch == ' ') {
                do {
                    ch++;
                } while(*ch == ' ');
            }
            else {
                ch++;
            }
        }
        *ch2 = 0;
    }
}

int isUpperAccents(unsigned char c) {
	if(((c >= 65) && (c <= 90)) || ((c >= 192) && (c <= 214)) || ((c >= 216) && (c <= 221))) {
		return 1;
	}
	return 0;
}

unsigned char charUpperAccent(unsigned char c) {
	return _upper_case[c];
}

void strUpperAccent(unsigned char *str) {
	while(*str) {
		*str = _upper_case[*str];
		str++;
	}
}