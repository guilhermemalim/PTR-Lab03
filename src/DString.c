#include <stdlib.h>
#include <string.h>
#include "DString.h"

DString *create_dstring_from_char(char *string) {
    DString *s = malloc(sizeof(DString));
    s->size = strlen(string);
    s->buffer = malloc(sizeof(char)*s->size);
    strcpy(s->buffer, string);
    
    return s;
}
DString *create_dstring_from_int(int size) {
    DString *s = malloc(sizeof(DString));
    s->buffer = malloc(sizeof(char)*size);
    s->size = size;
    
    return s;
}
DString *create_dstring_from_dstring(DString *dstring) {
    DString *ds = create_dstring_from_char(get_dstring_buffer(dstring));

    return ds;
}

DString *concatenar_dstrings(DString *s1, DString *s2) {
    int size = s1->size + s2->size;
    DString *s3 = create_dstring_from_int(size);

    for (int i = 0; i < s1->size; i++) {
        s3->buffer[i] = s1->buffer[i];
    }

    for (int i = 0; i < s2->size; i++) {
        s3->buffer[s1->size + i] = s2->buffer[i];
    }

    return s3;
}
int get_dstring_size(DString *s) {
    return s->size;
}
char *get_dstring_buffer(DString *s) {
    return s->buffer;
}

void destroyDString(DString *s) {
    free(s->buffer);
    free(s);
}
