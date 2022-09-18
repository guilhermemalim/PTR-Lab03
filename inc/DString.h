#ifndef _DSTRING_H_
#define _DSTRING_H_

typedef struct SDString {
    char* buffer;
    int size;
} DString;

DString *create_dstring_from_char(char *string);
DString *create_dstring_from_int(int size);
DString *create_dstring_from_dstring(DString *dstring);

DString *concatenar_dstrings(DString *s1, DString *s2);
int get_dstring_size(DString *s);
char *get_dstring_buffer(DString *s);
void destroyDString(DString *s);

#endif
