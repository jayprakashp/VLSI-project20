#include <stdint.h>
#include "params.h"
#include "reduce.h"
#include "ap_cint.h"


/*************************************************
* Name:        montgomery_reduce
*
* Description: For finite field element a with 0 <= a <= Q*2^32,
*              compute r \equiv a*2^{-32} (mod Q) such that 0 <= r < 2*Q.
*
* Arguments:   - uint64_t: finite field element a
*
* Returns r.
**************************************************/
#ifdef DEBUG_OPT
extern int montgomery_count;
#endif

uint32_t montgomery_reduce(uint64_t a) {
  uint64_t t, t2;


#ifdef DEBUG_OPT
  montgomery_count++;
#endif

#ifdef NO_DEBUG_OPT1
  uint64_t temp;

  temp = a;
  temp = a * QINV;
  //temp = - (temp<<26) + (temp<<23) - (temp<<13) - temp;
  temp &= (1ULL << 32) - 1;
  //temp &= (1ULL << 32) - 1;
  t = temp;
  t = temp*Q;
  //t = (t<<23) - (t<<13) + t + a;
  t = a + t;
  t >>= 32;
#else
  uint32_t temp;

  temp = a;
  //temp = a * QINV;
  temp = - (temp<<26) + (temp<<23) - (temp<<13) - temp;
  //temp &= (1ULL << 32) - 1;
  //temp &= (1ULL << 32) - 1;
  t = temp;
  //t = temp*Q;

  t = (t<<23) - (t<<13) + t + a;
  //t = a + t;
  t >>= 32;
#endif
  return t;
}

/*************************************************
* Name:        reduce32
*
* Description: For finite field element a, compute r \equiv a (mod Q)
*              such that 0 <= r < 2*Q.
*
* Arguments:   - uint32_t: finite field element a
*
* Returns r.
**************************************************/
uint32_t reduce32(uint32_t a) {
  uint32_t t;
  uint23 temp2;
  uint24 temp;

//  t = a & 0x7FFFFF;
//  a >>= 23;
//  t += (a << 13) - a;

  temp2 = a & 0x7FFFFF;
  a >>= 23;
  temp = temp2 + (a << 13) - a;
  t = temp;

  return t;
}

/*************************************************
* Name:        csubq
*
* Description: Subtract Q if input coefficient is bigger than Q.
*
* Arguments:   - uint32_t: finite field element a
*
* Returns r.
**************************************************/
uint32_t csubq(uint32_t a) {
//	uint32_t temp, temp2;
//	temp2 = a;
//	temp = a;
	if(a >= Q)
		a = a - Q;
//  a -= Q;
//  a += ((int32_t)a >> 31) & Q;
//  if(temp != a)
//	  printf("not equal");
    return a;
}

/*************************************************
* Name:        freeze
*
* Description: For finite field element a, compute standard
*              representative r = a mod Q.
*
* Arguments:   - uint32_t: finite field element a
*
* Returns r.
**************************************************/
uint32_t freeze(uint32_t a) {
  a = reduce32(a);
  a = csubq(a);
  return a;
}
