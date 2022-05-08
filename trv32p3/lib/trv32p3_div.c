/*
-- File : trv32p3_div.c
--
-- Contents : Emulation of 32-/64-bit division
--
-- Copyright (c) 2019-2020 Synopsys, Inc. This Synopsys processor model
-- captures an ASIP Designer Design Technique. The model and all associated
-- documentation are proprietary to Synopsys, Inc. and may only be used
-- pursuant to the terms and conditions of a written license agreement with
-- Synopsys, Inc.  All other use, reproduction, modification, or distribution
-- of the Synopsys processor model or the associated  documentation is
-- strictly prohibited.
*/

// LLVM frontend uses emulation functions from compiler-rt library
#ifndef __clang__

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Integer division with rounding towards zero, using restoring method.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  This implementation assumes that the unsigned->signed conversion wraps
//  properly, e.g. (signed)0x80000000U == INT_MIN

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ 32-bit Division

//  unsigned division, 'x' divided by 'd' with remainder 'r'.
inline unsigned div_pos(unsigned x,
                        unsigned d,
                        chess_output unsigned& r)
{
  if ((int)d < 0) { // MSB of 'd' is one
    unsigned q = 0;
    r = x;
    if (r >= d) {
      r = r - d;
      q = 1;
    }
    return q;
  }
  // MSB of 'd' is zero
  r = 0;
  for (int i = 0; i < 32; i++) {
    r = r << 1 | chess_dont_warn_range(x >> (32-1));
    x <<= 1;
    if (r >= d) {
      r = r - d;
      x |= 1;
    }
  }
  return x;
}

extern "C++"
unsigned div_called(unsigned x,
                    unsigned d,
                    chess_output unsigned & r)
{
  return div_pos(x,d,r);
}

extern "C++"
int div_called(int x, int d, chess_output int& r)
//  signed division, 'x' divided by 'd' with remainder 'r'.
{
  unsigned xu = x, du = d, ru;
  if (x < 0) xu = -xu;
  if (d < 0) du = -du;
  unsigned qu = div_pos(xu,du,ru);
  r = ru;
  if (x < 0) r = -r;
  return ((x ^ d) < 0)? -qu : qu;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ 64-bit Division

//  unsigned division, 'x' divided by 'd' with remainder 'r'.
inline unsigned long long div_pos(unsigned long long x,
                                  unsigned long long d,
                                  chess_output unsigned long long& r)
{
  if ((long long)d < 0) { // MSB of 'd' is one
    unsigned long long q = 0;
    r = x;
    if (r >= d) {
      r = r - d;
      q = 1;
    }
    return q;
  }
  // MSB of 'd' is zero
  r = 0;
  for (int i = 0; i < 64; i++) {
    r = r << 1 | chess_dont_warn_range(x >> (64-1));
    x <<= 1;
    if (r >= d) {
      r = r - d;
      x |= 1;
    }
  }
  return x;
}

extern "C++"
unsigned long long div_called(unsigned long long x,
                              unsigned long long d,
                              chess_output unsigned long long & r)
{
  return div_pos(x,d,r);
}

extern "C++"
long long  div_called(long long  x, long long d, chess_output long long& r)
//  signed division, 'x' divided by 'd' with remainder 'r'.
{
  unsigned long long xu = x, du = d, ru;
  if (x < 0) xu = -xu;
  if (d < 0) du = -du;
  unsigned long long qu = div_pos(xu,du,ru);
  r = ru;
  if (x < 0) r = -r;
  return ((x ^ d) < 0)? -qu : qu;
}

#endif
