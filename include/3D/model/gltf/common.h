#ifndef GLTF_COMMON_H
#define GLTF_COMMON_H

#include <cglm/cglm.h>
#include <zzzjson/zzzjson.h>
#include <stdbool.h>
#include <pthread.h>

// Integer types
#define u8      __UINT8_TYPE__
#define u16     __UINT16_TYPE__
#define u32     __UINT32_TYPE__
#define u64     __UINT64_TYPE__
#define u128    __uint128_t
#define i8      __INT8_TYPE__
#define i16     __INT16_TYPE__
#define i32     __INT32_TYPE__
#define i64     __INT64_TYPE__
#define i128    __int128_t

// String type
#define string char*

// Bool type
#define byte u8

#endif