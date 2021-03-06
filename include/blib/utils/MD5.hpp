#pragma once

/* MD5
 converted to C++ class by Frank Thilo (thilo@unix-ag.org)
 for bzflag (http://www.bzflag.org)
 http://www.zedwood.com/article/cpp-md5-function
   based on:

   md5.h and md5.c
   reference implementation of RFC 1321

   Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

*/

#include <string>
#include <iostream>
#include <cstdint>
#include <cstdio>

namespace blib {
  // a small class for calculating MD5 hashes of strings or byte arrays
// it is not meant to be fast or secure
//
// usage: 1) feed it blocks of uchars with update()
//      2) finalize()
//      3) get hexdigest() string
//      or
//      MD5(std::string).hexdigest()
//
// assumes that char is 8 bit and int is 32 bit
    // Constants for MD5Transform routine.
  struct SS {
    static const std::uint32_t SS::S11 = 7;
    static const std::uint32_t SS::S12 = 12;
    static const std::uint32_t SS::S13 = 17;
    static const std::uint32_t SS::S14 = 22;
    static const std::uint32_t SS::S21 = 5;
    static const std::uint32_t SS::S22 = 9;
    static const std::uint32_t SS::S23 = 14;
    static const std::uint32_t SS::S24 = 20;
    static const std::uint32_t SS::S31 = 4;
    static const std::uint32_t SS::S32 = 11;
    static const std::uint32_t SS::S33 = 16;
    static const std::uint32_t SS::S34 = 23;
    static const std::uint32_t SS::S41 = 6;
    static const std::uint32_t SS::S42 = 10;
    static const std::uint32_t SS::S43 = 15;
    static const std::uint32_t SS::S44 = 21;
  };


  class MD5 {
  public:
    typedef std::uint32_t size_type; // must be 32bit

    MD5();
    MD5( const std::string& text );
    void update( const unsigned char *buf, const std::size_t length );
    void update( const char *buf, const std::size_t length );
    MD5& finalize();
    std::string hexdigest() const;
    friend std::ostream& operator<<( std::ostream&, MD5 md5 );

  private:
    void init();
    enum { blocksize = 64 }; // VC6 won't eat a const static int here

    void transform( const std::uint8_t block[blocksize] );
    static void decode( std::uint32_t output[], const std::uint8_t input[], size_type len );
    static void encode( std::uint8_t output[], const std::uint32_t input[], size_type len );

    bool finalized;
    std::uint8_t buffer[blocksize]; // bytes that didn't fit in last 64 byte chunk
    std::uint32_t count[2];   // 64bit counter for number of bits (lo, hi)
    std::uint32_t state[4];   // digest so far
    std::uint8_t digest[16]; // the result

    // low level logic operations
    static inline std::uint32_t F( std::uint32_t x, std::uint32_t y, std::uint32_t z );
    static inline std::uint32_t G( std::uint32_t x, std::uint32_t y, std::uint32_t z );
    static inline std::uint32_t H( std::uint32_t x, std::uint32_t y, std::uint32_t z );
    static inline std::uint32_t I( std::uint32_t x, std::uint32_t y, std::uint32_t z );
    static inline std::uint32_t rotate_left( std::uint32_t x, int n );
    static inline void FF( std::uint32_t &a, std::uint32_t b, std::uint32_t c, std::uint32_t d, std::uint32_t x, std::uint32_t s, std::uint32_t ac );
    static inline void GG( std::uint32_t &a, std::uint32_t b, std::uint32_t c, std::uint32_t d, std::uint32_t x, std::uint32_t s, std::uint32_t ac );
    static inline void HH( std::uint32_t &a, std::uint32_t b, std::uint32_t c, std::uint32_t d, std::uint32_t x, std::uint32_t s, std::uint32_t ac );
    static inline void II( std::uint32_t &a, std::uint32_t b, std::uint32_t c, std::uint32_t d, std::uint32_t x, std::uint32_t s, std::uint32_t ac );
  };

  inline std::string md5( const std::string& str );

  //////////////////////////////////////////////

  // F, G, H and I are basic MD5 functions.
  inline std::uint32_t MD5::F( std::uint32_t x, std::uint32_t y, std::uint32_t z ) {
    return x&y | ~x&z;
  }

  inline std::uint32_t MD5::G( std::uint32_t x, std::uint32_t y, std::uint32_t z ) {
    return x&z | y&~z;
  }

  inline std::uint32_t MD5::H( std::uint32_t x, std::uint32_t y, std::uint32_t z ) {
    return x^y^z;
  }

  inline std::uint32_t MD5::I( std::uint32_t x, std::uint32_t y, std::uint32_t z ) {
    return y ^ (x | ~z);
  }

  // rotate_left rotates x left n bits.
  inline std::uint32_t MD5::rotate_left( std::uint32_t x, int n ) {
    return (x << n) | (x >> (32 - n));
  }

  // FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
  // Rotation is separate from addition to prevent recomputation.
  inline void MD5::FF( std::uint32_t &a, std::uint32_t b, std::uint32_t c, std::uint32_t d, std::uint32_t x, std::uint32_t s, std::uint32_t ac ) {
    a = rotate_left( a + F( b, c, d ) + x + ac, s ) + b;
  }

  inline void MD5::GG( std::uint32_t &a, std::uint32_t b, std::uint32_t c, std::uint32_t d, std::uint32_t x, std::uint32_t s, std::uint32_t ac ) {
    a = rotate_left( a + G( b, c, d ) + x + ac, s ) + b;
  }

  inline void MD5::HH( std::uint32_t &a, std::uint32_t b, std::uint32_t c, std::uint32_t d, std::uint32_t x, std::uint32_t s, std::uint32_t ac ) {
    a = rotate_left( a + H( b, c, d ) + x + ac, s ) + b;
  }

  inline void MD5::II( std::uint32_t &a, std::uint32_t b, std::uint32_t c, std::uint32_t d, std::uint32_t x, std::uint32_t s, std::uint32_t ac ) {
    a = rotate_left( a + I( b, c, d ) + x + ac, s ) + b;
  }

  //////////////////////////////////////////////

  // default ctor, just initailize
  inline MD5::MD5() {
    init();
  }

  //////////////////////////////////////////////

  // nifty shortcut ctor, compute MD5 for string and finalize it right away
  inline MD5::MD5( const std::string &text ) {
    init();
    update( text.c_str(), text.length() );
    finalize();
  }

  //////////////////////////////

  inline void MD5::init() {
    finalized = false;

    count[0] = 0;
    count[1] = 0;

    // load magic initialization constants.
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;
  }

  //////////////////////////////

  // decodes input (unsigned char) into output (std::uint32_t). Assumes len is a multiple of 4.
  inline void MD5::decode( std::uint32_t output[], const std::uint8_t input[], size_type len ) {
    for (unsigned int i = 0, j = 0; j < len; i++, j += 4)
      output[i] = ((std::uint32_t)input[j]) | (((std::uint32_t)input[j + 1]) << 8) |
      (((std::uint32_t)input[j + 2]) << 16) | (((std::uint32_t)input[j + 3]) << 24);
  }

  //////////////////////////////

  // encodes input (std::uint32_t) into output (unsigned char). Assumes len is
  // a multiple of 4.
  inline void MD5::encode( std::uint8_t output[], const std::uint32_t input[], size_type len ) {
    for (size_type i = 0, j = 0; j < len; i++, j += 4) {
      output[j] = input[i] & 0xff;
      output[j + 1] = (input[i] >> 8) & 0xff;
      output[j + 2] = (input[i] >> 16) & 0xff;
      output[j + 3] = (input[i] >> 24) & 0xff;
    }
  }

  //////////////////////////////

  // apply MD5 algo on a block
  inline void MD5::transform( const std::uint8_t block[blocksize] ) {
    std::uint32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];
    decode( x, block, blocksize );

    /* Round 1 */
    FF( a, b, c, d, x[0], SS::S11, 0xd76aa478 ); /* 1 */
    FF( d, a, b, c, x[1], SS::S12, 0xe8c7b756 ); /* 2 */
    FF( c, d, a, b, x[2], SS::S13, 0x242070db ); /* 3 */
    FF( b, c, d, a, x[3], SS::S14, 0xc1bdceee ); /* 4 */
    FF( a, b, c, d, x[4], SS::S11, 0xf57c0faf ); /* 5 */
    FF( d, a, b, c, x[5], SS::S12, 0x4787c62a ); /* 6 */
    FF( c, d, a, b, x[6], SS::S13, 0xa8304613 ); /* 7 */
    FF( b, c, d, a, x[7], SS::S14, 0xfd469501 ); /* 8 */
    FF( a, b, c, d, x[8], SS::S11, 0x698098d8 ); /* 9 */
    FF( d, a, b, c, x[9], SS::S12, 0x8b44f7af ); /* 10 */
    FF( c, d, a, b, x[10], SS::S13, 0xffff5bb1 ); /* 11 */
    FF( b, c, d, a, x[11], SS::S14, 0x895cd7be ); /* 12 */
    FF( a, b, c, d, x[12], SS::S11, 0x6b901122 ); /* 13 */
    FF( d, a, b, c, x[13], SS::S12, 0xfd987193 ); /* 14 */
    FF( c, d, a, b, x[14], SS::S13, 0xa679438e ); /* 15 */
    FF( b, c, d, a, x[15], SS::S14, 0x49b40821 ); /* 16 */

    /* Round 2 */
    GG( a, b, c, d, x[1], SS::S21, 0xf61e2562 ); /* 17 */
    GG( d, a, b, c, x[6], SS::S22, 0xc040b340 ); /* 18 */
    GG( c, d, a, b, x[11], SS::S23, 0x265e5a51 ); /* 19 */
    GG( b, c, d, a, x[0], SS::S24, 0xe9b6c7aa ); /* 20 */
    GG( a, b, c, d, x[5], SS::S21, 0xd62f105d ); /* 21 */
    GG( d, a, b, c, x[10], SS::S22, 0x2441453 ); /* 22 */
    GG( c, d, a, b, x[15], SS::S23, 0xd8a1e681 ); /* 23 */
    GG( b, c, d, a, x[4], SS::S24, 0xe7d3fbc8 ); /* 24 */
    GG( a, b, c, d, x[9], SS::S21, 0x21e1cde6 ); /* 25 */
    GG( d, a, b, c, x[14], SS::S22, 0xc33707d6 ); /* 26 */
    GG( c, d, a, b, x[3], SS::S23, 0xf4d50d87 ); /* 27 */
    GG( b, c, d, a, x[8], SS::S24, 0x455a14ed ); /* 28 */
    GG( a, b, c, d, x[13], SS::S21, 0xa9e3e905 ); /* 29 */
    GG( d, a, b, c, x[2], SS::S22, 0xfcefa3f8 ); /* 30 */
    GG( c, d, a, b, x[7], SS::S23, 0x676f02d9 ); /* 31 */
    GG( b, c, d, a, x[12], SS::S24, 0x8d2a4c8a ); /* 32 */

    /* Round 3 */
    HH( a, b, c, d, x[5], SS::S31, 0xfffa3942 ); /* 33 */
    HH( d, a, b, c, x[8], SS::S32, 0x8771f681 ); /* 34 */
    HH( c, d, a, b, x[11], SS::S33, 0x6d9d6122 ); /* 35 */
    HH( b, c, d, a, x[14], SS::S34, 0xfde5380c ); /* 36 */
    HH( a, b, c, d, x[1], SS::S31, 0xa4beea44 ); /* 37 */
    HH( d, a, b, c, x[4], SS::S32, 0x4bdecfa9 ); /* 38 */
    HH( c, d, a, b, x[7], SS::S33, 0xf6bb4b60 ); /* 39 */
    HH( b, c, d, a, x[10], SS::S34, 0xbebfbc70 ); /* 40 */
    HH( a, b, c, d, x[13], SS::S31, 0x289b7ec6 ); /* 41 */
    HH( d, a, b, c, x[0], SS::S32, 0xeaa127fa ); /* 42 */
    HH( c, d, a, b, x[3], SS::S33, 0xd4ef3085 ); /* 43 */
    HH( b, c, d, a, x[6], SS::S34, 0x4881d05 ); /* 44 */
    HH( a, b, c, d, x[9], SS::S31, 0xd9d4d039 ); /* 45 */
    HH( d, a, b, c, x[12], SS::S32, 0xe6db99e5 ); /* 46 */
    HH( c, d, a, b, x[15], SS::S33, 0x1fa27cf8 ); /* 47 */
    HH( b, c, d, a, x[2], SS::S34, 0xc4ac5665 ); /* 48 */

    /* Round 4 */
    II( a, b, c, d, x[0], SS::S41, 0xf4292244 ); /* 49 */
    II( d, a, b, c, x[7], SS::S42, 0x432aff97 ); /* 50 */
    II( c, d, a, b, x[14], SS::S43, 0xab9423a7 ); /* 51 */
    II( b, c, d, a, x[5], SS::S44, 0xfc93a039 ); /* 52 */
    II( a, b, c, d, x[12], SS::S41, 0x655b59c3 ); /* 53 */
    II( d, a, b, c, x[3], SS::S42, 0x8f0ccc92 ); /* 54 */
    II( c, d, a, b, x[10], SS::S43, 0xffeff47d ); /* 55 */
    II( b, c, d, a, x[1], SS::S44, 0x85845dd1 ); /* 56 */
    II( a, b, c, d, x[8], SS::S41, 0x6fa87e4f ); /* 57 */
    II( d, a, b, c, x[15], SS::S42, 0xfe2ce6e0 ); /* 58 */
    II( c, d, a, b, x[6], SS::S43, 0xa3014314 ); /* 59 */
    II( b, c, d, a, x[13], SS::S44, 0x4e0811a1 ); /* 60 */
    II( a, b, c, d, x[4], SS::S41, 0xf7537e82 ); /* 61 */
    II( d, a, b, c, x[11], SS::S42, 0xbd3af235 ); /* 62 */
    II( c, d, a, b, x[2], SS::S43, 0x2ad7d2bb ); /* 63 */
    II( b, c, d, a, x[9], SS::S44, 0xeb86d391 ); /* 64 */

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    // Zeroize sensitive information.
    memset( x, 0, sizeof x );
  }

  //////////////////////////////

  // MD5 block update operation. Continues an MD5 message-digest
  // operation, processing another message block
  inline void MD5::update( const unsigned char input[], const std::size_t length ) {
    // compute number of bytes mod 64
    size_type index = count[0] / 8 % blocksize;

    // Update number of bits
    if ((count[0] += (length << 3)) < (length << 3))
      count[1]++;
    count[1] += (length >> 29);

    // number of bytes we need to fill in buffer
    size_type firstpart = 64 - index;

    size_type i;

    // transform as many times as possible.
    if (length >= firstpart) {
      // fill buffer first, transform
      memcpy( &buffer[index], input, firstpart );
      transform( buffer );

      // transform chunks of blocksize (64 bytes)
      for (i = firstpart; i + blocksize <= length; i += blocksize)
        transform( &input[i] );

      index = 0;
    }
    else
      i = 0;

    // buffer remaining input
    memcpy( &buffer[index], &input[i], length - i );
  }

  //////////////////////////////

  // for convenience provide a verson with signed char
  inline void MD5::update( const char input[], const std::size_t length ) {
    update( (const unsigned char*)input, length );
  }

  //////////////////////////////

  // MD5 finalization. Ends an MD5 message-digest operation, writing the
  // the message digest and zeroizing the context.
  inline MD5& MD5::finalize() {
    static unsigned char padding[64] = {
      0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    if (!finalized) {
      // Save number of bits
      unsigned char bits[8] = { 0 };
      encode( bits, count, 8 );

      // pad out to 56 mod 64.
      size_type index = count[0] / 8 % 64;
      size_type padLen = (index < 56) ? (56 - index) : (120 - index);
      update( padding, padLen );

      // Append length (before padding)
      update( bits, 8 );

      // Store state in digest
      encode( digest, state, 16 );

      // Zeroize sensitive information.
      memset( buffer, 0, sizeof buffer );
      memset( count, 0, sizeof count );

      finalized = true;
    }

    return *this;
  }

  //////////////////////////////

  // return hex representation of digest as string
  inline std::string MD5::hexdigest() const {
    if (!finalized)
      return "";

    char buf[33] = { 0 };
    for (int i = 0; i < 16; i++)
      sprintf_s( buf + i * 2, 33 - i * 2, "%02x", digest[i] );

    return std::string( buf );
  }

  //////////////////////////////

  inline std::ostream& operator<<( std::ostream& out, MD5 md5 ) {
    return out << md5.hexdigest();
  }

  //////////////////////////////

  inline std::string md5( const std::string& str ) {
    const MD5 md5 = MD5( str );

    return md5.hexdigest();
  }

}