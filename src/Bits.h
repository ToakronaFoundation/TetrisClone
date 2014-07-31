#ifndef __LOLIROFLE_ESSENTIALS_BITS_MINIMAL_H_INCLUDED__
#define __LOLIROFLE_ESSENTIALS_BITS_MINIMAL_H_INCLUDED__

#include "Types.h"
#include <limits.h>

#define Bits_getByteIndex(bitIndex) ((bitIndex)/CHAR_BIT)
#define Bits_getBitOffset(bitIndex) ((bitIndex)%CHAR_BIT)

/**
 * Size of the given amount of bits in bytes
 *
 * @return <Number> Bytes
 */
#define Bits_sizeof(bits) (Bits_getBitOffset(bits)==0?Bits_getByteIndex(bits):Bits_getByteIndex(bits)+1)

/**
 * Allocates n bytes of memory
 *
 * @param  <size_t> bytes
 * @return <byte*>
 */
#define Bits_malloc_bytes(bytes) ((byte*)malloc(bytes))

 /**
  * Allocates n bits of memory, which results in a rounded amount of bytes based on amount of bits per byte
  *
  * @param  <size_t> bits
  * @return <byte*>
  */
#define Bits_malloc(bits) ((byte*)malloc(Bits_sizeof(bits)))

/**
 * Sets the nth bit to the on state (1)
 *
 * @param  <byte[]> bytes
 * @param  <size_t> index
 * @return <void>
 */
#define Bits_set1(bytes,index) (bytes)[Bits_getByteIndex(index)]|=1<<(Bits_getBitOffset(index))

 /**
  * Sets the nth bit to the on state (0)
  *
  * @param  <byte[]> bytes
  * @param  <size_t> index
  * @return <void>
  */
#define Bits_set0(bytes,index) (bytes)[Bits_getByteIndex(index)]&=~(1<<(Bits_getBitOffset(index)))

/**
 * Gets the nth bit
 *
 * @param  <byte[]> bytes
 * @param  <size_t> index
 * @return <bool>
 */
#define Bits_get(bytes,index) ((bytes)[Bits_getByteIndex(index)]&(1<<(Bits_getBitOffset(index))))

#endif
