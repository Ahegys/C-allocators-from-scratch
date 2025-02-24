#ifndef __UTIL_H__
#define __UTIL_H__

#define DEFINE_TUPLE(name, error_type, error_name, data_type, data_name) \
typedef struct { \
    error_type error_name; \
    data_type data_name; \
} name;


DEFINE_TUPLE(ArenaResult, int*, error, void*, data)
#endif // __UTIL_H__