#include <stdbool.h>
#include "generics.h"

#define array_t T_NAME(array, TAlias)
#define array_(NAME) F_NAME(array, TAlias, NAME)

#include "span.h"

typedef struct array_t {
    T * data;
    int length;
} array_t;

array_t array_(null)();
void array_(new)(array_t * array, int length);
void array_(delete)(array_t * array);

span_t array_(asSpan)(array_t * array);
span_t array_(getSpan)(array_t * array, int index, int length);
