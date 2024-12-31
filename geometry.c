#include "geometry.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "rational.h"
#include "util.h"


struct Point * new_point(struct Rational x, struct Rational y) {
	struct Point * p = malloc(sizeof(struct Point));
	p->x = x;
	p->y = y;
	return p;
}

struct Rational get_x(const struct Point * p) {
	assert(p);
	return p->x;
}

struct Rational get_y(const struct Point * p) {
	assert(p);
	return p->y;
}

void set_x(struct Point * p, struct Rational new_x) {
	assert(p);
	p->x = new_x;
}

void set_y(struct Point * p, struct Rational new_y) {
	assert(p);
	p->y = new_y;
}

void print_point(const void * p) {
	assert(p);
	printf("( ");
	print_rational(get_x(p));
	printf(" ; ");
	print_rational(get_y(p));
	printf(" )");
}

struct Segment * new_segment(struct Point * endpoint1, struct Point * endpoint2) {
	struct Segment * s = malloc(sizeof(struct Segment));
	s->endpoint1 = endpoint1;
	s->endpoint2 = endpoint2;
	return s;
}

void free_segment(void * s) {
	assert(s);
	// Manque la libération des points
	struct Segment *s1=s;

	free(get_endpoint1(s1));
	free(get_endpoint2(s1));
	free(s1);
}

struct Point * get_endpoint1(const struct Segment * s) {
	assert(s);
	return s->endpoint1;
}

struct Point * get_endpoint2(const struct Segment * s) {
	assert(s);
	return s->endpoint2;
}

void set_endpoint1(struct Segment * s, struct Point * new_endpoint) {
	assert(s);
	assert(new_endpoint);
	s->endpoint1 = new_endpoint;
}

void set_endpoint2(struct Segment * s, struct Point * new_endpoint) {
	assert(s);
	assert(new_endpoint);
	s->endpoint2 = new_endpoint;
}

void print_segment(const void * s) {
	assert(s);
	printf("1ère extrémité:");
	print_point(get_endpoint1(s));
	printf("	2ème extrémité:");
	print_point(get_endpoint2(s));
	printf("\n");
}

int point_precedes(const void * p1, const void * p2) {
	assert(p1);
	assert(p2);
	const struct Rational p1_x = get_x(p1);
	const struct Rational p2_x = get_x(p2);
	const struct Rational p1_y = get_y(p1);
	const struct Rational p2_y = get_y(p2);

	if (lt(p1_x, p2_x) || (eq(p1_x,p2_x) && gt(p2_y,p1_y) )) {
		return 1;
	}
	return 0;
}

int segment_precedes(const struct Segment * s1, const struct Segment * s2, struct Rational x0) {
	assert(s1);
	assert(s2);
	assert(lte(rmin(get_x(get_endpoint1(s1)), get_x(get_endpoint2(s1))), x0) &&
		   lte(x0, rmax(get_x(get_endpoint1(s1)), get_x(get_endpoint2(s1)))));
	assert(lte(rmin(get_x(get_endpoint1(s2)), get_x(get_endpoint2(s2))), x0) &&
		   lte(x0, rmax(get_x(get_endpoint1(s2)), get_x(get_endpoint2(s2)))));

	////////////Cas S1//////////////
	const struct Rational p1_x = get_x(get_endpoint1(s1));
	const struct Rational p1_y = get_y(get_endpoint1(s1));
	const struct Rational p2_x = get_x(get_endpoint2(s1));
	const struct Rational p2_y = get_y(get_endpoint2(s1));

	// Calcul de l'équation de la droite s1 : y= ax+b
	const struct Rational a = rdiv(rsub(p2_y, p1_y),rsub(p2_x,p1_x));
	const struct Rational b = rsub(p1_y,rmul(a,p1_x));

	//calcul de y1 en utilisant cas particulier de l'équation : y1= ax0+ b
	const struct Rational y1 = radd(rmul(a,x0),b);

	////////////Cas S2//////////////
	const struct Rational p3_x = get_x(get_endpoint1(s2));
	const struct Rational p3_y = get_y(get_endpoint1(s2));
	const struct Rational p4_x = get_x(get_endpoint2(s2));
	const struct Rational p4_y = get_y(get_endpoint2(s2));

	// Calcul de l'équation de la droite s1 : y= cx+d
	const struct Rational c = rdiv(rsub(p4_y, p3_y),rsub(p4_x,p3_x));
	const struct Rational d = rsub(p3_y,rmul(c,p3_x));

	//calcul de y2 en utilisant cas particulier de l'équation : y2= ax0+ b
	const struct Rational y2 = radd(rmul(c,x0),d);

	print_rational(y1);
	printf("\n");
	print_rational(y2);
	printf("\n");

	if (gt(y1,y2)) {
		return 1;
	}
	else return 0;
}

/**
 * Vérifie l'orientation du point \p q par rapport au segment défini par les points \p p et \p r .
 *
 * @param[in] p
 * @param[in] q
 * @param[in] r
 * @return
 * 0 si les trois points sont colinéaires,
 * 1 si \p q se trouve dans le sens horaire (clockwise) par rapport au segment [\p p, \p r ],
 * -1 si \p q se trouve dans le sens anti-horaire (counterclockwise) par rapport au segment [\p p, \p r ].
 */
static int get_orientation(const struct Point * p, const struct Point * q, const struct Point * r) {
	assert(p);
	assert(q);
	assert(r);
	struct Rational val =
				rsub(
					rmul(
						rsub(get_y(q), get_y(p)),
						rsub(get_x(r), get_x(q))
						),
					rmul(
						rsub(get_x(q), get_x(p)),
						rsub(get_y(r), get_y(q))
						)
					);
	if (eq(val, (struct Rational) {0,1}))
		return 0;
	else if (gt(val, (struct Rational) {0,1}))
		return 1;
	else
		return -1;
}

/**
 * Vérifie si le point \p q est inclus dans le carré défini par les points \p p et \p r .
 * Si \p p , \p q , \p r sont colinéaires, alors \p q se trouve dans le segment [\p p, \p r ].
 *
 * Remarque : Si le point \p q se trouve dans le bord du carré défini par les points \p p et \p r ,
 * alors on considère que \p q est inclus dans le carré.
 *
 * @param[in] p
 * @param[in] q
 * @param[in] r
 * @return 1 si \p q est inclus dans le carré définit par les points \p p et \p r , sinon 0.
 */
static int is_included(const struct Point * p, const struct Point * q, const struct Point * r) {
	assert(p);
	assert(q);
	assert(r);
	if (lte(rmin(get_x(p), get_x(r)), get_x(q)) && lte(get_x(q), rmax(get_x(p), get_x(r))) &&
		lte(rmin(get_y(p), get_y(r)), get_y(q)) && lte(get_y(q), rmax(get_y(p), get_y(r))))
			return 1;
	return 0;
}

int do_intersect(const struct Segment * s1, const struct Segment * s2) {
	assert(s1);
	assert(s2);

	// Find the four orientations needed for general and special cases
	int o1 = get_orientation(get_endpoint1(s1), get_endpoint2(s1), get_endpoint1(s2));
	int o2 = get_orientation(get_endpoint1(s1), get_endpoint2(s1), get_endpoint2(s2));
	int o3 = get_orientation(get_endpoint1(s2), get_endpoint2(s2), get_endpoint1(s1));
	int o4 = get_orientation(get_endpoint1(s2), get_endpoint2(s2), get_endpoint2(s1));

	// General case
	if ((min(o1, o2) == -1 && max(o1, o2) == 1) && (min(o3, o4) == -1 && max(o3, o4) == 1)) return 1;

	// Special Cases
	// p1, q1 and p2 are co-linear and p2 lies on segment p1q1
	if (o1 == 0 && is_included(get_endpoint1(s1), get_endpoint1(s2), get_endpoint2(s1))) {
		return 2;
	}

	// p1, q1 and q2 are co-linear and q2 lies on segment p1q1
	if (o2 == 0 && is_included(get_endpoint1(s1), get_endpoint2(s2), get_endpoint2(s1))) {
		return 2;
	}

	// p2, q2 and p1 are co-linear and p1 lies on segment p2q2
	if (o3 == 0 && is_included(get_endpoint1(s2), get_endpoint1(s1), get_endpoint2(s2))) {
		return 2;
	}

	// p2, q2 and q1 are co-linear and q1 lies on segment p2q2
	if (o4 == 0 && is_included(get_endpoint1(s2), get_endpoint2(s1), get_endpoint2(s2))) {
		return 2;
	}

	return 0; // Doesn't fall in any of the above cases
}

struct Point * get_intersection_point(const struct Segment * s1, const struct Segment * s2) {
	assert(s1);
	assert(s2);

	if (do_intersect(s1, s2)==0) {
		return NULL;
	}

	if (do_intersect(s1, s2)== 2) {
		ShowMessage("Violation de l'hypothèse de notre algorithme : pas plus qu'un événement sur un seul point",1);
	}

	const struct Rational x1 = get_x(get_endpoint1(s1));
	const struct Rational y1 = get_y(get_endpoint1(s1));
	const struct Rational x2 = get_x(get_endpoint2(s1));
	const struct Rational y2 = get_y(get_endpoint2(s1));
	const struct Rational x3 = get_x(get_endpoint1(s2));
	const struct Rational y3 = get_y(get_endpoint1(s2));
	const struct Rational x4 = get_x(get_endpoint2(s2));
	const struct Rational y4 = get_y(get_endpoint2(s2));

	const struct Rational a = rdiv(rsub(y2,y1),rsub(x2,x1));
	const struct Rational b = rsub(y1,rmul(a,x1));

	const struct Rational c = rdiv(rsub(y4,y3),rsub(x4,x3));
	const struct Rational d = rsub(y3,rmul(c,x3));

	const struct Rational x = rdiv(rsub(d,b),rsub(a,c));
	const struct Rational y = radd(rmul(a,x), b);
	struct Point * i = new_point(x, y);
	return i;
}