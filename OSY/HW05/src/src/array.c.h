#include <stdlib.h>
#include "array.h"

array_t array_(null)()
{
    array_t array;
    array.length = 0;
    array.data = NULL;
    return array;
}

void array_(new)(array_t * array, int length)
{
    array->length = length;
    array->data = malloc(array->length * sizeof(T));
}

void array_(delete)(array_t * array)
{
    free(array->data);
}

span_t array_(asSpan)(array_t * array)
{
    return (span_t){ .data = array->data, .length = array->length };
}

span_t array_(getSpan)(array_t * array, int index, int length)
{
    return (span_t){ .data = array->data + index, .length = length };
}
