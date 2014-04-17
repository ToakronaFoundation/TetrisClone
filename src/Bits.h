#ifndef __LOLIROFLE_ESSENTIALS_BITS_MINIMAL_H_INCLUDED__
#define __LOLIROFLE_ESSENTIALS_BITS_MINIMAL_H_INCLUDED__

#include "Types.h"
#include <limits.h>

#define Bits_getByteIndex(bitIndex) ((bitIndex)/CHAR_BIT)
#define Bits_getBitOffset(bitIndex) ((bitIndex)%CHAR_BIT)

/**
 * Size of the given amount of bits in bytes
 *
 * @returns <Number> Bytes
 */
#define Bits_sizeof(bits) (Bits_getBitOffset(bits)==0?Bits_getByteIndex(bits):Bits_getByteIndex(bits)+1)

#define Bits_malloc_bytes(bytes) ((byte*)malloc(bytes))
#define Bits_malloc(bits) ((byte*)malloc(Bits_sizeof(bits)))

#define Bits_set1(bytes,index) (bytes)[Bits_getByteIndex(index)]|=1<<(Bits_getBitOffset(index))
#define Bits_set0(bytes,index) (bytes)[Bits_getByteIndex(index)]&=~(1<<(Bits_getBitOffset(index)))

#define Bits_get(bytes,index) ((bytes)[Bits_getByteIndex(index)]&(1<<(Bits_getBitOffset(index))))

#endif
