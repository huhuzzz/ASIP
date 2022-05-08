/*
-- File : trv32p3_longlong.h
--
-- Contents : Long long integer application layer for the trv32p3 processor.
--
-- Copyright (c) 2019-2020 Synopsys, Inc. This Synopsys processor model
-- captures an ASIP Designer Design Technique. The model and all associated
-- documentation are proprietary to Synopsys, Inc. and may only be used
-- pursuant to the terms and conditions of a written license agreement with
-- Synopsys, Inc.  All other use, reproduction, modification, or distribution
-- of the Synopsys processor model or the associated  documentation is
-- strictly prohibited.
*/

#ifndef INCLUDED_TRV32P3_LONGLONG_H_
#define INCLUDED_TRV32P3_LONGLONG_H_


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ type conversions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

promotion  operator  unsigned long long (   signed long long   ) = nil;
promotion  operator    signed long long ( unsigned long long   ) = nil;

promotion  operator  void*              (   signed long long   ) = undefined;
promotion  operator  void*              ( unsigned long long   ) = undefined;
promotion  operator    signed long long ( void*                ) = undefined;
promotion  operator  unsigned long long ( void*                ) = undefined;

namespace trv32p3_primitive {

  inline     dint to_dint_se(int i) property(dont_generate) { return dint(i,i>>31); }
  inline     dint to_dint_ze(int i) property(dont_generate) { return dint(i,0); }
  inline     int  to_int(dint w)    property(dont_generate) { return w.low; }

};

promotion  operator    signed long long (   signed int         ) = dint to_dint_se(  signed int);
promotion  operator    signed long long ( unsigned int         ) = dint to_dint_ze(  signed int);
promotion  operator  unsigned long long (   signed int         ) = dint to_dint_se(  signed int);
promotion  operator  unsigned long long ( unsigned int         ) = dint to_dint_ze(  signed int);

inline     operator    signed long long (   signed char s      ) { return (  signed int)s; }
inline     operator    signed long long ( unsigned char s      ) { return (unsigned int)s; }
inline     operator    signed long long (   signed short s     ) { return (  signed int)s; }
inline     operator    signed long long ( unsigned short s     ) { return (unsigned int)s; }

inline     operator    signed char      (   signed long long l ) { return chess_dont_warn_range((  signed char)(  signed int)l); }
inline     operator  unsigned char      (   signed long long l ) { return (  signed int)l; }
inline     operator    signed short     (   signed long long l ) { return chess_dont_warn_range((  signed short)(  signed int)l); }
inline     operator  unsigned short     (   signed long long l ) { return (  signed int)l; }

promotion  operator    signed int       (   signed long long   ) =   signed int to_int(dint);
promotion  operator    signed int       ( unsigned long long   ) =   signed int to_int(dint);
promotion  operator  unsigned int       (   signed long long   ) =   signed int to_int(dint);
promotion  operator  unsigned int       ( unsigned long long   ) =   signed int to_int(dint);

inline     operator  unsigned long long (   signed char s      ) { return (  signed int)s; }
inline     operator  unsigned long long ( unsigned char s      ) { return (unsigned int)s; }
inline     operator  unsigned long long (   signed short s     ) { return (  signed int)s; }
inline     operator  unsigned long long ( unsigned short s     ) { return (unsigned int)s; }

inline     operator    signed char      ( unsigned long long l ) { return chess_dont_warn_range((  signed char)(unsigned int)l); }
inline     operator  unsigned char      ( unsigned long long l ) { return (unsigned int)l; }
inline     operator    signed short     ( unsigned long long l ) { return chess_dont_warn_range((  signed short)(unsigned int)l); }
inline     operator  unsigned short     ( unsigned long long l ) { return (unsigned int)l; }


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ add & subtract
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace trv32p3_primitive {

  promotion unsigned int add(unsigned int,unsigned int) property(duplicate_at_using_opn2 dont_generate) = w32 add(w32,w32);
  promotion unsigned int sub(unsigned int,unsigned int) property(duplicate_at_using_opn2 dont_generate) = w32 sub(w32,w32);

  inline dint l_add(dint a, dint b) property(dont_generate) {
    // precondtion: a != 0 && b != 0
    dint r;
    if      (chess_manifest(a.low==0))  {
      r.low  = b.low;
      r.high = a.high + b.high;
    }
    else if (chess_manifest(b.low==0))  {
      r.low  = a.low;
      r.high = a.high + b.high;
    }
    // else if a.high==0 or b.high==0
    //     there may be a carry from low to high, so no optimisation
    //     is possible
    else {
      r.low  = add(a.low, b.low);
      r.high = add(a.high, b.high);
      //here generate the carry using the fact that the result can also give info about the carry
      r.high = r.high + (r.low < a.low);
    }
    return r;
  }

  inline dint l_sub(dint a, dint b) property(dont_generate) {
    dint r;
    if (chess_manifest(b.low==0))  {
      r.low  = a.low;
      r.high = a.high - b.high;
    }
    else {
      r.low  = sub(a.low, b.low);
      r.high = sub(a.high, b.high);
      //here generate the borrow using the fact that the result can also give info about the borrow
      r.high = r.high - (a.low < b.low);
    }
    return r;
  }
};

promotion    signed long long  operator + (  signed long long,   signed long long) = dint l_add(dint,dint);
promotion    signed long long  operator - (  signed long long,   signed long long) = dint l_sub(dint,dint);

promotion  unsigned long long  operator + (unsigned long long, unsigned long long) = dint l_add(dint,dint);
promotion  unsigned long long  operator - (unsigned long long, unsigned long long) = dint l_sub(dint,dint);


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ bitwise operators
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace trv32p3_primitive {

  inline dint l_and(dint a, dint b) property(dont_generate) { return dint( a.low & b.low , a.high & b.high); }
  inline dint l_or (dint a, dint b) property(dont_generate) { return dint( a.low | b.low , a.high | b.high); }
  inline dint l_xor(dint a, dint b) property(dont_generate) { return dint( a.low ^ b.low , a.high ^ b.high); }
  inline dint l_complement(dint a)  property(dont_generate) { return dint( ~a.low , ~a.high); }

};

promotion    signed long long  operator & (  signed long long,   signed long long) = dint l_and(dint,dint);
promotion    signed long long  operator | (  signed long long,   signed long long) = dint l_or (dint,dint);
promotion    signed long long  operator ^ (  signed long long,   signed long long) = dint l_xor(dint,dint);
promotion    signed long long  operator ~ (  signed long long)      = dint l_complement(dint);

promotion  unsigned long long  operator & (unsigned long long, unsigned long long) = dint l_and(dint,dint);
promotion  unsigned long long  operator | (unsigned long long, unsigned long long) = dint l_or (dint,dint);
promotion  unsigned long long  operator ^ (unsigned long long, unsigned long long) = dint l_xor(dint,dint);
promotion  unsigned long long  operator ~ (unsigned long long)      = dint l_complement(dint);


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ multiply
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// emulation function in libtrv32p3
extern "C++"  signed long long  mul_called (signed long long,signed long long) property(functional loop_free dont_generate);

inline    signed long long  operator *  (  signed long long a,   signed long long b) property(functional loop_free) {
  return mul_called(a,b);
}

inline  unsigned long long  operator *  (unsigned long long a, unsigned long long b) property(functional loop_free) {
  return mul_called((  signed long long)a,(  signed long long)b);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ compare operators
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace trv32p3_primitive {

  inline bool l_lts(dint a, dint b) property(dont_generate) {
    if ((signed int)a.high < (signed int)b.high)
      return true;
    else
      if (a.high == b.high)
        return a.low < b.low;
      else
        return false;
  }

  inline bool l_les(dint a, dint b) property(dont_generate) {
    if ((signed int)a.high < (signed int)b.high)
      return true;
    else
      if (a.high == b.high)
        return a.low <= b.low;
      else
        return false;
  }

  inline bool l_ltu(dint a, dint b) property(dont_generate) {
    if ((unsigned int)a.high < (unsigned int)b.high)
      return true;
    else
      if (a.high == b.high)
        return a.low < b.low;
      else
        return false;
  }

  inline bool l_leu(dint a, dint b) property(dont_generate) {
    if ((unsigned int)a.high < (unsigned int)b.high)
      return true;
    else
      if (a.high == b.high)
        return a.low <= b.low;
      else
        return false;
  }

  inline bool l_eq(dint a, dint b) property(dont_generate) {
    return a.high == b.high && a.low == b.low;
  }

  inline bool l_ne(dint a, dint b) property(dont_generate) {
    return a.high != b.high || a.low != b.low;
  }

};


promotion  bool operator <  (  signed long long,     signed long long)   = bool l_lts(dint,dint);
promotion  bool operator <= (  signed long long,     signed long long)   = bool l_les(dint,dint);
promotion  bool operator == (  signed long long,     signed long long)   = bool l_eq(dint,dint);
promotion  bool operator != (  signed long long,     signed long long)   = bool l_ne(dint,dint);
inline     bool operator >  (  signed long long a,   signed long long b) { return b < a; }
inline     bool operator >= (  signed long long a,   signed long long b) { return b <= a; }

promotion  bool operator <  (unsigned long long,   unsigned long long)   = bool l_ltu(dint,dint);
promotion  bool operator <= (unsigned long long,   unsigned long long)   = bool l_leu(dint,dint);
promotion  bool operator == (unsigned long long,   unsigned long long)   = bool l_eq(dint,dint);
promotion  bool operator != (unsigned long long,   unsigned long long)   = bool l_ne(dint,dint);
inline     bool operator >  (unsigned long long a, unsigned long long b) { return b < a; }
inline     bool operator >= (unsigned long long a, unsigned long long b) { return b <= a; }


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ shift operators
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace trv32p3_primitive {

  inline dint l_lsl(dint a, signed int f) property(dont_generate) {
    unsigned int carries;
    dint r;
    if (f == 0) return a;
    if (f < 32) {
      carries = a.low >> (32 - f);
      r.low = chess_dont_warn_range(a.low << f);
      r.high = chess_dont_warn_range(a.high << f) | carries;
    }
    else {  // f >= 32
      carries = a.low << (f - 32);
      r.low = 0;
      r.high = carries;
    }
    return r;
  }

  inline dint l_asr(dint a, signed int f) property(dont_generate) {
    unsigned int carries;
    dint r;
    if (f == 0) return a;
    if (f < 32) {
      carries = a.high << (32 - f);
      r.low = chess_dont_warn_range(a.low >> f) | carries;
      r.high = chess_dont_warn_range((signed int)a.high >> f);
    }
    else { // f >= 32
      carries = (signed int)a.high >> (f - 32);
      r.low = carries;
      r.high = (signed int)carries>>31;
    }
    return r;
  }

  inline dint l_lsr(dint a, signed int f) property(dont_generate) {
    unsigned int carries;
    dint r;
    if (f == 0) return a;
    if (f < 32) {
      carries = a.high << (32 - f);
      r.low = chess_dont_warn_range(a.low >> f) | carries;
      r.high = chess_dont_warn_range(a.high >> f);
    }
    else { // f >= 32
      carries = a.high >> (f - 32);
      r.low = carries;
      r.high = 0;
    }
    return r;
  }

}

promotion    signed long long  operator << (  signed long long,  signed int) = dint l_lsl(dint,  signed int);
promotion    signed long long  operator >> (  signed long long,  signed int) = dint l_asr(dint,  signed int);
promotion  unsigned long long  operator << (unsigned long long,  signed int) = dint l_lsl(dint,  signed int);
promotion  unsigned long long  operator >> (unsigned long long,  signed int) = dint l_lsr(dint,  signed int);


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ Divide - replace with call to emulation function in libtrv32p3
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

extern "C++"  signed long long  div_called (signed long long, signed long long, chess_output signed long long&) property(functional dont_generate);

extern "C++"  unsigned long long  div_called (unsigned long long, unsigned long long, chess_output unsigned long long&) property(functional dont_generate);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ signed divide and modulo

// additional indirection - to enable common subexpression identification
inline  signed long long  div_called_ (signed long long a, signed long long b, signed long long& r) property(functional dont_generate) {
  return div_called(a,b,r);
}

inline  signed long long  operator / (signed long long a, signed long long b) {
  signed long long r;
  return div_called_(a,b,r);
}

inline  signed long long  operator % (signed long long a, signed long long b) {
  signed long long r;
  div_called_(a,b,r);
  return r;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ unsigned divide and modulo

// additional indirection - to enable common subexpression identification
inline  unsigned long long  div_called_ (unsigned long long a, unsigned long long b, unsigned long long& r) property(functional dont_generate) {
  return div_called(a,b,r);
}

inline  unsigned long long  operator / (unsigned long long a, unsigned long long b) {
  unsigned long long r;
  return div_called_(a,b,r);
}

inline  unsigned long long  operator % (unsigned long long a, unsigned long long b) {
  unsigned long long r;
  div_called_(a,b,r);
  return r;
}

#endif /* INCLUDED_TRV32P3_LONGLONG_H_ */
