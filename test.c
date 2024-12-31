#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "rational.h"
#include "list.h"
#include "tree.h"
#include "geometry.h"
#include "algo.h"
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////// TEST RATIONAL /////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int testRational() {
	struct Rational x= {0,1};
	struct Rational y= {4,-5};
	struct Rational z= {56,8};
	struct Rational w= {-32,-64};

	//test fct simplifier
	simplify(&x);
	simplify(&y);
	simplify(&z);
	simplify(&w);
	if ( x.numerator != 0 || x.denominator != 1 || y.numerator != -4 || y.denominator != 5
		|| z.numerator != 7 || z.denominator != 1 || w.numerator != 1 || w.denominator != 2 ) {
		printf("problem simplify");
		return 0;
		}

	if ( gt(x,y) != 1 || gt(y,x) !=0 || gt(w,z) !=0) {
		printf("problem gt");
		return 0;
	}
	if ( lt(x,y) != 0 || lt(y,x) !=1 || lt(w,z) != 1) {
		printf("problem lt");
		return 0;
	}

	print_rational(x);
	return 1;

}

/////////////////////////////////////////////////////////////////////////////
///////////////////////////////// TEST LIST /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void test_list_insert_first() { // juste
	int * i1 = malloc(sizeof(int));

	int * i3 = malloc(sizeof(int));
	*i1 = 1;

	*i3 = 3;
	struct list_t *l1=new_list();
	list_insert_first(l1, i3);
	view_list(l1,viewInt);
	list_insert_first(l1, i1);
	view_list(l1, viewInt);

}
void test_list_remove_first() {
	//int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	//*i1 = 1;
	*i2 = 2;
	*i3 = 3;
	struct list_t *l1=new_list();
	list_insert_first(l1, i3);
	list_insert_first(l1, i2);
	view_list(l1, viewInt);
	list_remove_first(l1);
	view_list(l1, viewInt);
}

void test_list_remove_last() {
	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	*i1 = 1;
	*i2 = 2;
	*i3 = 3;
	struct list_t *l1=new_list();
	list_insert_first(l1, i3);
	list_insert_first(l1, i2);
	list_insert_first(l1, i1);
	view_list(l1, viewInt);
	list_remove_last(l1);
	view_list(l1, viewInt);
}

void test_find_list() {
	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	*i1 = 1;
	*i2 = 2;
	*i3 = 3;
	struct list_t *l1=new_list();
	list_insert_first(l1, i3);
	list_insert_first(l1, i2);
	list_insert_first(l1, i1);
	view_list(l1, viewInt);
	struct list_node_t *noeud=list_find_node(l1, i1);
	struct list_t *l2=new_list();
	list_insert_first(l2,noeud->data );
	view_list(l2, viewInt);
}
void test_swap() {
	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	*i1 = 1;
	*i2 = 2;
	*i3 = 3;
	struct list_t *l1=new_list();
	list_insert_first(l1, i3);
	list_insert_first(l1, i2);
	list_insert_first(l1, i1);
	view_list(l1, viewInt);
	struct list_node_t *node1=list_find_node(l1, i1);
	struct list_node_t *node2=list_find_node(l1, i3);
	list_swap_nodes_data(node1,node2);
	view_list(l1, viewInt);
}
static int compare_lists(const struct list_t * l1, void * l2[], const unsigned int size) {
	if (get_list_size(l1) != size)
    {
        return 0;
    }
    if (get_list_size(l1) != 0 && get_list_head(l1) == NULL)
    {
        return 0;
    }
    if (get_list_head(l1) == NULL)
        return 1;

    struct list_node_t * curr = get_list_head(l1);
    unsigned int i = 0;
    while (curr != NULL && l2) {
        if (get_list_node_data(curr) != l2[i])
        {
            return 0;
        }
        curr = get_successor(curr);
        i++;
    }

    curr = get_list_tail(l1);
    i = size-1;
    while (curr != NULL && l2) {
        if (get_list_node_data(curr) != l2[i])
        {
            return 0;
        }
        curr = get_predecessor(curr);
        i--;
    }
    return 1;
}

void test_list_insert_last() {
	int * i1 = malloc(sizeof(int));
    int * i2 = malloc(sizeof(int));
    int * i3 = malloc(sizeof(int));
    *i1 = 1;
    *i2 = 2;
    *i3 = 3;

    struct list_t * L = new_list();
    int * tab[3];
    tab[0] = i1; tab[1] = i2; tab[2] = i3;

    list_insert_last(L, i1);
    view_list(L,viewInt);
    if (compare_lists(L, (void*) tab, 1) == 0) printf("problè");
    list_insert_last(L, i2);
    view_list(L,viewInt);
    if (compare_lists(L, (void*) tab, 2) == 0) printf("problème");
    list_insert_last(L, i3);
    view_list(L,viewInt);
    if (compare_lists(L, (void*) tab, 3) == 0) printf("problème");

}

void test_delete_list() {
	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));

	*i1 = 1;
	*i2 = 2;

	struct list_t * l1=new_list();
	list_insert_first(l1, i1);
	list_insert_last(l1, i2);
	view_list(l1, viewInt);
	delete_list(l1,freeInt);
	l1=new_list();
	view_list(l1, viewInt);

}


///////////////////////////////////////////////////////////////////////
//////////////////////////////// TEST GEOMETRY/////////////////////////
///////////////////////////////////////////////////////////////////////
void testPrintSegment() {
	const struct Rational x1 = {4,3};
	const struct Rational y1 = {2,1};
	const struct Rational x2 = {1,2};
	const struct Rational y2 = {0,1};

	struct Point *p = new_point(x1,y1);
	struct Point *q = new_point(x2,y2);

	struct Segment *s = new_segment(p,q);
	print_segment(s);

}


void testGeometry() {
	const struct Rational x1 = {2,1};
	const struct Rational y1 = {10,1};
	const struct Rational x2 = {5,1};
	const struct Rational y2 = {5,1};
	const struct Rational x3 = {3,1};
	const struct Rational y3 = {9,1};
	const struct Rational x4 = {6,1};
	const struct Rational y4 = {12,1};

	struct Point *p1 = new_point(x1,y1);
	struct Point *p2 = new_point(x2,y2);
	struct Point *p3 = new_point(x3,y3);
	struct Point *p4 = new_point(x4,y4);

	const struct Segment *s1 = new_segment(p1,p2);
	const struct Segment *s2 = new_segment(p3,p4);
	print_segment(s1);
	print_segment(s2);

	//struct Point *i = get_intersection_point(s1,s2);
	//print_point(i);
	/*
	int n= point_precedes(p1,p2);
	printf("n=%d\n",n);
	*/
	const struct Rational x0 = {4,1};
	const int m= segment_precedes(s1,s2,x0);
	printf("m=%d\n",m);

}


///////////////////////////////////////////////////////////////////////
////////////////////////// TEST SUR LES ARBRES/////////////////////////
///////////////////////////////////////////////////////////////////////

void test_print_tree() {
	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	*i1 = 1;
	*i2 = 2;
	*i3 = 3;
	struct tree_t * t = new_tree();

	int *key1=malloc(sizeof(int));
	int *key2=malloc(sizeof(int));
	int *key3 = malloc(sizeof(int));
	*key1 = 4;
	*key2 = 5;
	*key3 = 6;

	tree_insert(t,key1,i1,precedes_int);
	tree_insert(t,key2,i2,precedes_int);
	tree_insert(t,key3,i3,precedes_int);
	view_tree(t,viewInt,viewInt);
	delete_tree(t,freeInt,freeInt);

}

void test_find_max() { //à changer
	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	*i1 = 1;
	*i2 = 3;
	*i3 = 2;
	struct tree_t * t = new_tree();

	int *key1=malloc(sizeof(int));
	int *key2=malloc(sizeof(int));
	int *key3 = malloc(sizeof(int));
	*key1 = 4;
	*key2 = 5;
	*key3 = 6;

	tree_insert(t,key1,i1,precedes_int);
	tree_insert(t,key2,i2,precedes_int);
	tree_insert(t,key3,i3,precedes_int);
	view_tree(t,viewInt,viewInt);
	printf("\n");
	printf("le max :");
	viewInt(get_tree_node_data(tree_find_max(get_root(t))));
	//printf("%d\n",get_tree_node_data(tree_find_max(get_root(t))));


	delete_tree(t,freeInt,freeInt);


}

void test_find_min() { //à changer
	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	int * i4 = malloc(sizeof(int));
	*i1 = 2;
	*i2 = 3;
	*i3 = 1;
	*i4=0;
	struct tree_t * t = new_tree();

	int *key1=malloc(sizeof(int));
	int *key2=malloc(sizeof(int));
	int *key3 = malloc(sizeof(int));
	int *key4 = malloc(sizeof(int));
	*key1 = 4;
	*key2 = 5;
	*key3 = 6;
	*key4 = 7;


	tree_insert(t,key1,i1,precedes_int);
	tree_insert(t,key2,i2,precedes_int);
	tree_insert(t,key3,i3,precedes_int);
	tree_insert(t,key4,i4,precedes_int);
	view_tree(t,viewInt,viewInt);

	printf("\n");
	printf("le min :");

	printf("%d\n", *(int *)get_tree_node_data(tree_find_min(get_root(t))));


	delete_tree(t,freeInt,freeInt);
}


void test_tree_find_node() {
	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	int * i4 = malloc(sizeof(int));

	*i1 = 2;
	*i2 = 3;
	*i3 = 1;
	*i4 = 5;

	struct tree_t * t = new_tree();

	int *key1=malloc(sizeof(int));
	int *key2=malloc(sizeof(int));
	int *key3 = malloc(sizeof(int));
	int *key4 = malloc(sizeof(int));
	int *key5 = malloc(sizeof(int));
	*key1 = 4;
	*key2 = 5;
	*key3 = 6;
	*key4 = 7;
	*key5 = 10;


	tree_insert(t,key1,i1,precedes_int);
	tree_insert(t,key2,i2,precedes_int);
	tree_insert(t,key3,i3,precedes_int);
	tree_insert(t,key4,i4,precedes_int);

	view_tree(t,viewInt,viewInt);
	printf("\n");
	printf("Recherche du noeud :");


	const struct tree_node_t * node = tree_find_node(get_root(t),key5,precedes_int);

	if(node!=NULL) {
		printf("Le noeud existe:");
		viewInt(get_tree_node_data(node));
	}else printf("Le noeud n'existe pas");



	delete_tree(t,freeInt,freeInt);
}

void test_tree_remove(){
	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	int * i4 = malloc(sizeof(int));
	int * i5 = malloc(sizeof(int));
	int * i6 = malloc(sizeof(int));
	int * i7 = malloc(sizeof(int));
	int * i8 = malloc(sizeof(int));
	int * i9 = malloc(sizeof(int));
	int * i10 = malloc(sizeof(int));

	*i1 = 1;
	*i2 = 2;
	*i3 = 3;
	*i4 = 4;
	*i5 = 5;
	*i6 = 6;
	*i7 = 7;
	*i8 = 8;
	*i9 = 9;
	*i10 = 10;

	struct tree_t * t = new_tree();

	int *key1 =malloc(sizeof(int));
	int *key2 =malloc(sizeof(int));
	int *key3 =malloc(sizeof(int));
	int *key4 =malloc(sizeof(int));
	int *key5 =malloc(sizeof(int));
	int *key6 =malloc(sizeof(int));
	int *key7 =malloc(sizeof(int));
	int *key8 =malloc(sizeof(int));
	int *key9 =malloc(sizeof(int));
	int *key10 =malloc(sizeof(int));

	*key1 = 8;
	*key2 = 6;
	*key3 = 14;
	*key4 = 2;
	*key5 = 1;
	*key6 = 3;
	*key7 = 11;
	*key8 = 15;
	*key9 = 9;
	*key10 = 12;


	tree_insert(t,key1,i1,precedes_int);
	tree_insert(t,key2,i2,precedes_int);
	tree_insert(t,key3,i3,precedes_int);
	tree_insert(t,key4,i4,precedes_int);
	tree_insert(t,key5,i5,precedes_int);
	tree_insert(t,key6,i6,precedes_int);
	tree_insert(t,key7,i7,precedes_int);
	tree_insert(t,key8,i8,precedes_int);
	tree_insert(t,key9,i9,precedes_int);
	tree_insert(t,key10,i10,precedes_int);

	view_tree(t,viewInt,viewInt);
	tree_remove(t,key3,precedes_int);
	printf("\n après changement");
	view_tree(t,viewInt,viewInt);

	delete_tree(t,freeInt,freeInt);
}

void test_tree_find_predecessor() {
	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	int * i4 = malloc(sizeof(int));
	int * i5 = malloc(sizeof(int));
	int * i6 = malloc(sizeof(int));

	*i1 = 1;
	*i2 = 2;
	*i3 = 3;
	*i4 = 4;
	*i5 = 5;
	*i6 = 6;

	struct tree_t * t = new_tree();

	int *key1=malloc(sizeof(int));
	int *key2=malloc(sizeof(int));
	int *key3 = malloc(sizeof(int));
	int *key4=malloc(sizeof(int));
	int *key5=malloc(sizeof(int));
	int *key6 = malloc(sizeof(int));

	*key1 = 10;
	*key2 = 5;
	*key3 = 15;
	*key4 = 3;
	*key5 = 7;
	*key6 = 20;

	tree_insert(t,key1,i1,precedes_int);
	tree_insert(t,key2,i2,precedes_int);
	tree_insert(t,key3,i3,precedes_int);
	tree_insert(t,key4,i4,precedes_int);
	tree_insert(t,key5,i5,precedes_int);
	tree_insert(t,key6,i6,precedes_int);

	view_tree(t,viewInt,viewInt);
	if (!tree_find_predecessor(get_root(t),key4,precedes_int))
	{
		printf("\n Il n'y a pas de prédecesseur");
	}
	else
	{
		printf("\n la data du prédecesseur : ");
		viewInt(get_tree_node_data(tree_find_predecessor(get_root(t),key4,precedes_int)));

		printf("\n la key du successeur : ");
		viewInt(get_tree_node_key(tree_find_predecessor(get_root(t),key4,precedes_int)));
	}

	delete_tree(t,freeInt,freeInt);
}

void test_tree_find_successor() {

	int * i1 = malloc(sizeof(int));
	int * i2 = malloc(sizeof(int));
	int * i3 = malloc(sizeof(int));
	int * i4 = malloc(sizeof(int));
	int * i5 = malloc(sizeof(int));
	int * i6 = malloc(sizeof(int));
	int * i7 = malloc(sizeof(int));

	*i1 = 1;
	*i2 = 2;
	*i3 = 3;
	*i4 = 4;
	*i5 = 5;
	*i6 = 6;
	*i7 = 7;
	struct tree_t * t = new_tree();

	int *key1=malloc(sizeof(int));
	int *key2=malloc(sizeof(int));
	int *key3 = malloc(sizeof(int));
	int *key4=malloc(sizeof(int));
	int *key5=malloc(sizeof(int));
	int *key6 = malloc(sizeof(int));
	int *key7 = malloc(sizeof(int));

	*key1 = 10;
	*key2 = 5;
	*key3 = 15;
	*key4 = 3;
	*key5 = 7;
	*key6 = 20;
	*key7=14;

	tree_insert(t,key1,i1,precedes_int);
	tree_insert(t,key2,i2,precedes_int);
	tree_insert(t,key3,i3,precedes_int);
	tree_insert(t,key4,i4,precedes_int);
	tree_insert(t,key5,i5,precedes_int);
	tree_insert(t,key6,i6,precedes_int);
	tree_insert(t,key7,i7,precedes_int);

	view_tree(t,viewInt,viewInt);

	if(tree_find_successor(get_root(t),key6,precedes_int)==NULL) {
		printf("\n Il n'y a pas de successor");
	}
	else {
		printf("\n la data du successeur : ");
		viewInt(get_tree_node_data(tree_find_successor(get_root(t),key6,precedes_int)));

		printf("\n la key du successeur : ");
		viewInt(get_tree_node_key(tree_find_successor(get_root(t),key6,precedes_int)));
	}

	delete_tree(t,freeInt,freeInt);
}
///////////////////////////////////////////////////////////////////////
//////////////////////////////// TEST ALGO ////////////////////////////
///////////////////////////////////////////////////////////////////////

void test_load_segment(){
	struct list_t * l= load_segments("data/input");
	view_list(l,print_segment);
	delete_list(l,free_segment);
}

void test_algo(){
	const struct Rational x1 = {4,1};
	const struct Rational y1 = {68,10};
	const struct Rational x2 = {10,1};
	const struct Rational y2 = {0,1};
	const struct Rational x3 = {7,2};
	const struct Rational y3 = {8,3};
	const struct Rational x4 = {10,1};
	const struct Rational y4 = {6,1};
	const struct Rational x5 = {92,10};
	const struct Rational y5 = {4,1};
	const struct Rational x6 = {12,1};
	const struct Rational y6 = {2,1};
	const struct Rational x7 = {6,1};
	const struct Rational y7 = {2,1};
	const struct Rational x8 = {9,1};
	const struct Rational y8 = {8,1};

	struct Point *p1 = new_point(x1,y1);
	struct Point *p2 = new_point(x2,y2);
	struct Point *p3 = new_point(x3,y3);
	struct Point *p4 = new_point(x4,y4);
	struct Point *p5 = new_point(x5,y5);
	struct Point *p6 = new_point(x6,y6);
	struct Point *p7 = new_point(x7,y7);
	struct Point *p8 = new_point(x8,y8);

	struct Segment *s1 = new_segment(p1,p2);
	struct Segment *s2 = new_segment(p3,p4);
	struct Segment *s3 = new_segment(p5,p6);
	struct Segment *s4 = new_segment(p7,p8);

	struct list_t *segments = new_list();
	list_insert_first(segments,s1);
	list_insert_first(segments,s2);
	list_insert_first(segments,s3);
	list_insert_first(segments,s4);

	view_list(segments,print_segment);
	struct list_t * inter = new_list();
	inter=all_pairs(segments);

	view_list(inter,print_point);
	struct list_t *test = new_list();
	test=BentleyOttmann(segments);
	view_list(test,print_segment);
	delete_list(segments,free_segment);
}



///////////////////////////////////////////////////////////////////////
//////////////////////////////// MAIN /////////////////////////////////
///////////////////////////////////////////////////////////////////////

int main() {

	//***TEST SUR LES LISTES ***//
		//test_list_insert_first();
		//test_list_insert_last();
		//test_list_remove_first();
		//test_list_remove_last();
		//test_find_list();
		//test_swap();
		//compare_lists();
		//test_delete_list();

	//***TEST SUR LA GÉOMETRIE***//
		//testGeometry();
		//testPrintSegment();

	//***TEST SUR ALGO***//
		//test_load_segment();
		test_algo();

	//***TEST SUR LES ARBRES***//
		//test_print_tree(); //ok
		//test_find_min(); //ok
		//test_find_max(); //ok
		//test_tree_find_node(); //ok
		//test_tree_remove(); //okkkkk !!
		//test_tree_find_predecessor(); //ok
		//test_tree_find_successor(); //oui

}
