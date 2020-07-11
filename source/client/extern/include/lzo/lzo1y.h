/* lzo1y.h -- public interface of the LZO1Y compression algorithm

   This file is part of the LZO-Professional data compression library.

   Copyright (C) 2006 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2005 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2004 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2003 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2002 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2001 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2000 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1999 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1998 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1997 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1996 Markus Franz Xaver Johannes Oberhumer
   All Rights Reserved.

   CONFIDENTIAL & PROPRIETARY SOURCE CODE.

   ANY USAGE OF THIS FILE IS SUBJECT TO YOUR LICENSE AGREEMENT.

   Markus F.X.J. Oberhumer
   <markus@oberhumer.com>
   http://www.oberhumer.com/products/lzo-professional/
 */


#ifndef __LZOPRO_LZO1Y_H_INCLUDED
#define __LZOPRO_LZO1Y_H_INCLUDED

#ifndef __LZOCONF_H_INCLUDED
#include "lzoconf.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
// fast decompressors
************************************************************************/

#define LZOPRO_LZO1Y_MEM_DECOMPRESS       (0)

LZO_EXTERN(int)
lzopro_lzo1y_decompress         ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem /* NOT USED */ );

LZO_EXTERN(int)
lzopro_lzo1y_decompress_safe    ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem /* NOT USED */ );


/* Additional versions of the decompressor that can be faster on some
 * architechtures. You should use these only after thorough performance
 * testing. */
LZO_EXTERN(int)
lzopro_lzo1y_decompress_a00     ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem /* NOT USED */ );
LZO_EXTERN(int)
lzopro_lzo1y_decompress_a01     ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem /* NOT USED */ );
LZO_EXTERN(int)
lzopro_lzo1y_decompress_a02     ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem /* NOT USED */ );
LZO_EXTERN(int)
lzopro_lzo1y_decompress_a03     ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem /* NOT USED */ );
LZO_EXTERN(int)
lzopro_lzo1y_decompress_a04     ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem /* NOT USED */ );
LZO_EXTERN(int)
lzopro_lzo1y_decompress_a05     ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem /* NOT USED */ );
LZO_EXTERN(int)
lzopro_lzo1y_decompress_a06     ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem /* NOT USED */ );
LZO_EXTERN(int)
lzopro_lzo1y_decompress_a07     ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem /* NOT USED */ );


/***********************************************************************
// good compressors
************************************************************************/

/* NOTE: you must provide nalloc/nfree callbacks via 'cb' */

LZO_EXTERN(int)
lzopro_lzo1y_99_compress       ( const lzo_bytep src, lzo_uint  src_len,
                                       lzo_bytep dst, lzo_uintp dst_len,
                                       lzo_callback_p cb,
                                       int compression_level /* 1..10 */ );


/***********************************************************************
// fast compressors
************************************************************************/

#define LZOPRO_LZO1Y_1_06_MEM_COMPRESS    (256)

LZO_EXTERN(int)
lzopro_lzo1y_1_06_compress      ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem );

#define LZOPRO_LZO1Y_1_07_MEM_COMPRESS    (512)

LZO_EXTERN(int)
lzopro_lzo1y_1_07_compress      ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem );


#define LZOPRO_LZO1Y_1_08_MEM_COMPRESS    (1 * 1024UL)

LZO_EXTERN(int)
lzopro_lzo1y_1_08_compress      ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem );


#define LZOPRO_LZO1Y_1_09_MEM_COMPRESS    (2 * 1024UL)

LZO_EXTERN(int)
lzopro_lzo1y_1_09_compress      ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem );


#define LZOPRO_LZO1Y_1_10_MEM_COMPRESS    (4 * 1024UL)

LZO_EXTERN(int)
lzopro_lzo1y_1_10_compress      ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem );


#define LZOPRO_LZO1Y_1_11_MEM_COMPRESS    (8 * 1024UL)

LZO_EXTERN(int)
lzopro_lzo1y_1_11_compress      ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem );


#define LZOPRO_LZO1Y_1_12_MEM_COMPRESS    (16 * 1024UL)

LZO_EXTERN(int)
lzopro_lzo1y_1_12_compress      ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem );


#define LZOPRO_LZO1Y_1_13_MEM_COMPRESS    (32 * 1024UL)

LZO_EXTERN(int)
lzopro_lzo1y_1_13_compress      ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem );


#define LZOPRO_LZO1Y_1_14_MEM_COMPRESS    (64 * 1024UL)

LZO_EXTERN(int)
lzopro_lzo1y_1_14_compress      ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem );


#define LZOPRO_LZO1Y_1_15_MEM_COMPRESS    (128 * 1024UL)

LZO_EXTERN(int)
lzopro_lzo1y_1_15_compress      ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem );


#define LZOPRO_LZO1Y_1_16_MEM_COMPRESS    (256 * 1024UL)

LZO_EXTERN(int)
lzopro_lzo1y_1_16_compress      ( const lzo_bytep src, lzo_uint  src_len,
                                        lzo_bytep dst, lzo_uintp dst_len,
                                        lzo_voidp wrkmem );


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* already included */


/* vim:set ts=4 et: */
