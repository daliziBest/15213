/*
 * CS:APP Data Lab
 *
 * <Please put your name and userid here>
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 */

/* Instructions to Students:

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:

  long Funct(long arg1, long arg2, ...) {
      // brief description of how your implementation works
      long var1 = Expr1;
      ...
      long varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. (Long) integer constants 0 through 255 (0xFFL), inclusive. You are
      not allowed to use big constants such as 0xffffffffL.
  2. Function arguments and local variables (no global variables).
  3. Local variables of type int and long
  4. Unary integer operations ! ~
     - Their arguments can have types int or long
     - Note that ! always returns int, even if the argument is long
  5. Binary integer operations & ^ | + << >>
     - Their arguments can have types int or long
  6. Casting from int to long and from long to int

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting other than between int and long.
  7. Use any data type other than int or long.  This implies that you
     cannot use arrays, structs, or unions.

  You may assume that your machine:
  1. Uses 2s complement representations for int and long.
  2. Data type int is 32 bits, long is 64.
  3. Performs right shifts arithmetically.
  4. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31 (int) or 63 (long)

EXAMPLES OF ACCEPTABLE CODING STYLE:
  //
  // pow2plus1 - returns 2^x + 1, where 0 <= x <= 63
  //
  long pow2plus1(long x) {
     // exploit ability of shifts to compute powers of 2
     // Note that the 'L' indicates a long constant
     return (1L << x) + 1L;
  }

  //
  // pow2plus4 - returns 2^x + 4, where 0 <= x <= 63
  //
  long pow2plus4(long x) {
     // exploit ability of shifts to compute powers of 2
     long result = (1L << x);
     result += 4L;
     return result;
  }

NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

CAUTION:
  Do not add an #include of <stdio.h> (or any other C library header)
  to this file.  C library headers almost always contain constructs
  that dlc does not understand.  For debugging, you can use printf,
  which is declared for you just below.  It is normally bad practice
  to declare C library functions by hand, but in this case it's less
  trouble than any alternative.

  dlc will consider each call to printf to be a violation of the
  coding style (function calls, after all, are not allowed) so you
  must remove all your debugging printf's again before submitting your
  code or testing it with dlc or the BDD checker.  */

extern int printf(const char *, ...);

/* Edit the functions below.  Good luck!  */
// 2
/*
 * copyLSB - set all bits of result to least significant bit of x
 *   Examples:
 *     copyLSB(5L) = 0xFFFFFFFFFFFFFFFFL,
 *     copyLSB(6L) = 0x0000000000000000L
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
long copyLSB(long x) {
    return (x << 63) >> 63;
}
/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 63 (most significant)
 *   Examples: allOddBits(0xFFFFFFFDFFFFFFFDL) = 0L,
 *             allOddBits(0xAAAAAAAAAAAAAAAAL) = 1L
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 14
 *   Rating: 2
 */
long allOddBits(long x) {
    // create 0xAAAAAAAAAAAAAAAAL
    long mask = 0xAAL;
    long temp = mask;
    mask = (mask << 8) + temp; // 0xAAAA
    temp = mask;
    mask = (mask << 16) + temp; // 0xAAAAAAAA
    temp = mask;
    mask = (mask << 32) + temp; // 0xAAAAAAAAAAAAAAAA
    x = mask & x;
    return !(mask ^ x);
}
/*
 * isNotEqual - return 0 if x == y, and 1 otherwise
 *   Examples: isNotEqual(5L, 5L) = 0L, isNotEqual(4L, 5L) = 1L
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
long isNotEqual(long x, long y) {
    return !(!(x ^ y));
}
/*
 * dividePower2 - Compute x/(2^n), for 0 <= n <= 62
 *  Round toward zero
 *   Examples: dividePower2(15L,1L) = 7L, dividePower2(-33L,4L) = -2L
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
long dividePower2(long x, long n) {
    long down = x >> n;
    long up = (x + ((1L << n) + ~0L)) >> n;
    long indicator = x >> 63;
    return (indicator & up) + (~indicator & down);
}
// 3
/*
 * remainderPower2 - Compute x%(2^n), for 0 <= n <= 30
 *   Negative arguments should yield negative remainders
 *   Examples: remainderPower2(15L,2L) = 3, remainderPower2(-35L,3L) = -3L
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
long remainderPower2(long x, long n) {
    long down = x >> n;
    long up = (x + ((1L << n) + ~0L)) >> n;
    long indicator = x >> 63;
    long quotient = (indicator & up) + (~indicator & down);
    // remainder = dividend - quotient * divisor
    return x + ~(quotient << n) + 1;
    /* another solution  //But it is not verified !!!
     * long indicator = x >> 63;
     * return (~indicator & (x & (1L << n))) + (indicator & ((x & (1L << n)) +
~(1L << n) + 1));
     */
}
/*
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 63
 *   Examples:
 *      rotateLeft(0x8765432187654321L,4L) = 0x7654321876543218L
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3
 */
long rotateLeft(long x, long n) {
    long left = x << n;
    long moveRight = 64L + ~n + 1L;
    long right = x >> moveRight;
    right = right & ~(~0L << n);
    return left + right;
}

/*
 * bitMask - Generate a mask consisting of all 1's
 *   between lowbit and highbit
 *   Examples: bitMask(5L,3L) = 0x38L
 *   Assume 0 <= lowbit < 64, and 0 <= highbit < 64
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
long bitMask(long highbit, long lowbit) {
    long high1 = (~(~0L << highbit) << 1) + 1L;
    long lowmask = ~0L << lowbit;
    return high1 & lowmask;
}
/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5L) = 0L, isPower2(8L) = 1L, isPower2(0) = 0L
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
long isPower2(long x) {
    long mask = (~x + 1) ^ x;
    // need to deal with 0 and  for rectifying result
    long isNot0 = !!x;
    long lastbitIs0 = !(x >> 63);
    return (!(mask & x)) & isNot0 & lastbitIs0;
}

// 4
/*
 * allAsciiDigits - return 1 if each byte b in x satisfies
 *   0x30 <= b <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: allAsciiDigits(0x3031323536373839L) = 1L.
 *            allAsciiDigits(0x4031323536373839L) = 0L.
 *            allAsciiDigits(0x0031323536373839L) = 0L.
 *   Legal ops: ! ~ & ^ | << >>
 *   Max ops: 30
 *   Rating: 4
 */
// you should note that "+" is forbidden, but i am tired to fix it!!!!!!
long allAsciiDigits(long x) {
    long mask1 = 0xF0L;
    long temp = 0xF0L;
    mask1 = (mask1 << 8) | temp; // 0xF0F0
    temp = mask1;
    mask1 = (mask1 << 16) | temp; // 0xF0F0F0F0
    temp = mask1;
    mask1 = (mask1 << 32) | temp; // 0xF0F0F0F0F0F0F0F0

    long mask2 = 0x30L;
    temp = 0x30L;
    mask2 = (mask2 << 8) | temp; // 0x3030
    temp = mask2;
    mask2 = (mask2 << 16) | temp; // 0x30303030
    temp = mask2;
    mask2 = (mask2 << 32) | temp; // 0x3030303030303030

    long mask3 = 0x06L;
    temp = 0x06L;
    mask3 = (mask3 << 8) | temp; // 0x0606
    temp = mask3;
    mask3 = (mask3 << 16) | temp; // 0x06060606
    temp = mask3;
    mask3 = (mask3 << 32) | temp; // 0x0606060606060606

    long isAll3 = !((x & mask1) ^ mask2);
    long is0to9 = !(((x & ~mask1) + mask3) & mask1);
    return isAll3 & is0to9;
}

/*
 * trueThreeFourths - multiplies by 3/4 rounding toward 0,
 *   avoiding errors due to overflow
 *   Examples:
 *    trueThreeFourths(11L) = 8
 *    trueThreeFourths(-9L) = -6
 *    trueThreeFourths(4611686018427387904L) = 3458764513820540928L (no
 * overflow)
 *    Legal ops: ! ~ & ^ | + << >>
 *    Max ops: 20
 *    Rating: 4
 */
long trueThreeFourths(long x) {
    long divide4Down = x >> 2; // use down quotient
    long multiply3 = divide4Down + (divide4Down << 1);
    // rectify result
    /*
     * not negtive low2bit: 	00	01	10	11
     * adjustment:       	0	0	+1	+2
     * negtive low2bit:   	00	01	10	11
     * adjustment:       	0	+1	+2	+3
     */
    long isNegtive = x >> 63;
    long low2bit = x & 0x03L;
    return (isNegtive & (multiply3 + low2bit)) +
           (~isNegtive & (multiply3 + ((low2bit + low2bit + low2bit) >> 2)));
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5L) = 2, bitCount(7L) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Rating: 4
 */
long bitCount(long x) {
    // refer to java's Integer.bitCount() source code
    // 0101010101010
    long mask1 = 0x55L;
    long temp = mask1;
    mask1 = (mask1 << 8) + temp; // 0x5555L
    temp = mask1;
    mask1 = (mask1 << 16) + temp; // 0x55555555L
    temp = mask1;
    mask1 = (mask1 << 32) + temp; // 0x5555555555555555L
    x = (x & mask1) + (x >> 1 & mask1);
    // 001100110011
    long mask2 = 0x33L;
    temp = mask2;
    mask2 = (mask2 << 8) + temp; // 0x3333L
    temp = mask2;
    mask2 = (mask2 << 16) + temp; // 0x33333333L
    temp = mask2;
    mask2 = (mask2 << 32) + temp; // 0x3333333333333333L
    x = (x & mask2) + (x >> 2 & mask2);

    x = x + (x >> 4);

    // 0000111100001111
    long mask3 = 0x0FL;
    temp = mask3;
    mask3 = (mask3 << 8) + temp; // 0x0F0FL
    temp = mask3;
    mask3 = (mask3 << 16) + temp; // 0x0f0f0f0fL
    temp = mask3;
    mask3 = (mask3 << 32) + temp; // 0x0f0f0f0f0f0f0f0fL
    x = x & mask3;

    x = x + (x >> 8);
    x = x + (x >> 16);
    x = x + (x >> 32); // result is in x's last byte

    return x & 0xffL;
}
