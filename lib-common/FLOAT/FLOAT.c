#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	long long A = a;
	long long B = b;
	long long res = A * B;
	return res >> 16;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	/* Dividing two 64-bit integers needs the support of another library
	 * `libgcc', other than newlib. It is a dirty work to port `libgcc'
	 * to NEMU. In fact, it is unnecessary to perform a "64/64" division
	 * here. A "64/32" division is enough.
	 *
	 * To perform a "64/32" division, you can use the x86 instruction
	 * `div' or `idiv' by inline assembly. We provide a template for you
	 * to prevent you from uncessary details.
	 *
	 *     asm volatile ("??? %2" : "=a"(???), "=d"(???) : "r"(???), "a"(???), "d"(???));
	 *
	 * If you want to use the template above, you should fill the "???"
	 * correctly. For more information, please read the i386 manual for
	 * division instructions, and search the Internet about "inline assembly".
	 * It is OK not to use the template above, but you should figure
	 * out another way to perform the division.
	 */

	long long A = Fabs(a);
	A <<= 16;
 	int B = Fabs(b);
	int A_h = A >> 32; // high 32 bits of A
	int A_l = A & 0xffffffff; // low 32 bits of A
	int res, rem;
	asm volatile ("div %2" : "=a"(res), "=d"(rem) : "r"(B), "a"(A_l), "d"(A_h));
	if ((a < 0 &&  b > 0) || (a > 0 && b < 0)) res = -res;
	return res;
/*
	long long A = Fabs(a);
	long long B = Fabs(b);
	int c;
	FLOAT res = 0;;
	A <<= 16;
	B <<= 16;
	c = 16;
	while(A != 0) {
		if (A >= B) {
			A = A - B;
			res = res | (1 << c);
		}
		if (c == 0) break;
		B = B >> 1;
		c --;
	}
	if ((a < 0 &&  b > 0) || (a > 0 && b < 0)) res = -res;
	return res;*/
}

FLOAT f2F(float a) {
	/* You should figure out how to convert `a' into FLOAT without
	 * introducing x87 floating point instructions. Else you can
	 * not run this code in NEMU before implementing x87 floating
	 * point instructions, which is contrary to our expectation.
	 *
	 * Hint: The bit representation of `a' is already on the
	 * stack. How do you retrieve it to another variable without
	 * performing arithmetic operations on it directly?
	 */

	int p = *(int*)&a;
	int res = p & 0x7fffff;
	int e = (p >> 23) & 0xff;
	if(e == 0) {
		e = 1 - 0x7f - 7;
	}
	else {
		e = e - 0x7f - 7;
		res |= 0x800000;
	}
	if(e >= 0)
		res <<= e;
	else
		res >>= (-e);
	if(p < 0)
		res = ~res + 1;
	return res;
}

FLOAT Fabs(FLOAT a) {
	if (a < 0)
		a = -a;
	return a;
}

/* Functions below are already implemented */

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}
