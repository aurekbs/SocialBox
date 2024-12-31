#include "rational.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "util.h"

long long get_numerator(struct Rational r) {
	return r.numerator;
}

long long get_denominator(struct Rational r) {
	return r.denominator;
}

void set_numerator(struct Rational * r, long long new_numerator) {
	assert(r);
	r->numerator = new_numerator;
}

void set_denominator(struct Rational * r, long long new_denominator) {
	assert(r);
	r->denominator = new_denominator;
}

/**
 * Restitue le Plus Grand Diviseur Commun de \p a et \p b .
 *
 * @param[in] a
 * @param[in] b
 * @return PGCD(a,b)
 */
static long long gcd(long long a, long long b) {
	while(b!=0){
		long long r = a%b;
		a = b;
		b = r;
	}
	return a;
}

void simplify(struct Rational * r) {
	assert(r);
	long long g = gcd(r->numerator, r->denominator);
	r->numerator = r->numerator/ g;
	r->denominator = r->denominator/ g;

	if(r->denominator < 0) {
		r->numerator = -(r->numerator);
		r->denominator = -(r->denominator);
	}

}

int gt(struct Rational a, struct Rational b) {
	if (a.numerator * b.denominator > b.numerator * a.denominator) {
		return 1;
	}
	else return 0;
}

int lt(struct Rational a, struct Rational b) {
	if(a.numerator * b.denominator < b.numerator * a.denominator) {
		return 1;
	}
	else return 0;
}

int gte(struct Rational a, struct Rational b) {
	if(a.numerator * b.denominator >= b.numerator * a .denominator) {
		return 1;
	}
	else return 0;
}

int lte(struct Rational a, struct Rational b) {
	if(a.numerator* b.denominator <= b.numerator* a.denominator) {
		return 1;
	}
	else return 0;
}

int eq(struct Rational a, struct Rational b) {
	if(a.numerator* b.denominator == b.numerator* a.denominator) {
		return 1;
	}
	else return 0;
}

int neq(struct Rational a, struct Rational b) {
	if(a.numerator* b.denominator != b.numerator*a.denominator) {
		return 1;
	}
	else return 0;
}

// Pensez à simplifier le résultat.
struct Rational radd(struct Rational a, struct Rational b) {
	struct Rational c;
	c.numerator= a.numerator*b.denominator+ a.denominator*b.numerator;
	c.denominator= a.denominator*b.denominator;
	simplify(&c);
	return c;

}

// Pensez à simplifier le résultat.
struct Rational rsub(struct Rational a, struct Rational b) {
	struct Rational c;
	c.numerator= a.numerator*b.denominator - a.denominator*b.numerator;
	c.denominator= a.denominator*b.denominator;
	simplify(&c);
	return c;
}

// Pensez à simplifier le résultat.
struct Rational rmul(struct Rational a, struct Rational b) {
	struct Rational c;
	c.numerator=a.numerator*b.numerator;
	c.denominator=a.denominator*b.denominator;
	simplify(&c);
	return c;
}

// Pensez à simplifier le résultat.
struct Rational rdiv(struct Rational a, struct Rational b) {
	struct Rational c;
	if ((b.numerator == 0) || (a.denominator == 0)){
		ShowMessage("Le dénominateur du résultat est null", 1);;
	}
	c.numerator = a.numerator * b.denominator;
	c.denominator = a.denominator * b.numerator;
	simplify(&c);
	return c;
}

struct Rational rmax(struct Rational a, struct Rational b) {
	if (gte(a,b)) {
		return a;
	}
	else {return b;}
}

struct Rational rmin(struct Rational a, struct Rational b) {
	if (gte(a,b)) {
		return b;
	}
	else {return a;}
}

void print_rational(struct Rational r) {
	if(r.denominator == 1) {printf("%lld", r.numerator);}
	else {
		printf("%lld/%lld", r.numerator,r.denominator);
	}

}