#ifndef STATUS_H
#define STATUS_H

typedef enum
{
    SUCCESS = 0,

    NULL_POINTER_PASSED = -1,
    INVALID_ARGUMENT = -2,
    INDEX_OUT_OF_BOUNDS = -3,

    NO_SPACE = -11,
    INSUFFICIENT_ARRAY_PASSED = -12, //Array passed does not meet the size requirement of the function
    FREE_CHUNK_ACCESS_ATTEMPT = -13,

    FILE_CHUNK_EXTENT_LIMIT_EXCEEDED = -21
} StatusCode;

#endif