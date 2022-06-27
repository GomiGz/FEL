#include <stdbool.h>
#include "generics.h"

#define span_t T_NAME(span, TAlias)
#define span_(NAME) F_NAME(span, TAlias, NAME)

typedef struct span_t {
    T * data;
    int length;
} span_t;
