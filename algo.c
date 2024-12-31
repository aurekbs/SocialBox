#include <algo.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "util.h"
#include "rational.h"
#include "list.h"
#include "tree.h"
#include "geometry.h"

// ATTENTION : Pour simplifier vos algorithmes, pensez à comparer les deux points
// de chaque segment à l'aide de la fonction point_precedes et enregistrer le point
// qui précède sur le membre endpoint1 de la struct Segment, tandis que l'autre
// point sur le membre endpoint2 de la struct Segment.

struct list_t * load_segments(const char * input_filename) {
	FILE * fichier= fopen(input_filename, "r");

	//si fichier NULL, retourner erreur??
	if (fichier==NULL) {
		printf("Impossible de charger fichier\n");
		return NULL;
	}
	else
	{
		struct list_t* segments = new_list();
		long long t[8];

		while (fscanf(fichier,"%lld/%lld,%lld/%lld %lld/%lld,%lld/%lld",
			&t[0],&t[1],&t[2],&t[3],&t[4],&t[5],&t[6],&t[7]) == 8)
		{
			// Créer les points
			struct Rational x1, y1, x2, y2;
			set_numerator(&x1,t[0]);
			set_denominator(&x1,t[1]);
			set_numerator(&y1,t[2]);
			set_denominator(&y1,t[3]);


			set_numerator(&x2,t[4]);
			set_denominator(&x2,t[5]);
			set_numerator(&y2,t[6]);
			set_denominator(&y2,t[7]);

			struct Point *p1 = new_point(x1, y1);
			struct Point *p2 = new_point(x2, y2);

			// Créer le segment et l'ajouter à la liste
			struct Segment *seg = new_segment(p1, p2);
			if(point_precedes(p1,p2)) {
				seg->endpoint1=p1;
				seg->endpoint2=p2;
			}
			else {
				seg->endpoint1=p2;
				seg->endpoint2=p1;
			}
			list_insert_last(segments,seg);
		}

		fclose(fichier);
		return segments;
	}
}



void save_intersections(const char * output_filename, const struct list_t * intersections){

	FILE * fichier = fopen(output_filename, "w"); // mode w ou a?

	if (fichier == NULL){
		printf("Impossible de charger fichier\n");
	}
	else {
		const struct list_node_t * aux = get_list_head(intersections);
		while (aux) {
			const struct Point *p = get_list_node_data(aux);
			const struct Rational x = get_x(p);
			const struct Rational y = get_y(p);

			fprintf(fichier,"%lld/%lld,%lld/%lld \n", get_numerator(x),get_denominator(x),get_numerator(y),get_denominator(y));
			aux = get_successor(aux);
		}
	}
	fclose(fichier);
}

struct list_t * all_pairs(const struct list_t * segments) {
	//glouton
	assert(segments);
	struct list_t * intersections = new_list();
	const struct list_node_t * aux = get_list_head(segments);

	while (aux) {
		const struct list_node_t * aux2= get_successor(aux);
		while(aux2) {
			const struct Segment *s = get_list_node_data(aux2);
			const struct Segment *t = get_list_node_data(aux);
			if(do_intersect(s,t)) {
				list_insert_last(intersections, get_intersection_point(s,t));
			}
			aux2=get_successor(aux2);
		}
		aux=get_successor(aux);
	}
	return intersections;
}

///////////////////////////////////////////////////////////////////////////////
//////////////////// ALGORITHME DE Bentley-Ottmann ///////////////////////////
///////////////////////////////////////////////////////////////////////////////

struct Event * new_event(int type, struct Point * event_point, struct Segment * s1, struct Segment * s2) {
	struct Event * e = malloc(sizeof(struct Event));
	e->type = type;
	e->event_point = event_point;
	e->s1 = s1;
	e->s2 = s2;
	return e;
}

int get_event_type(const struct Event * event) {
	assert(event);
	return event->type;
}

struct Point * get_event_point(const struct Event * event) {
	assert(event);
	return event->event_point;
}

struct Segment * get_event_segment1(const struct Event * event) {
	assert(event);
	return event->s1;
}

struct Segment * get_event_segment2(const struct Event * event) {
	assert(event);
	return event->s2;
}

/**
 * Initialise la structure des événements (arbre binaire de recherche) avec les
 * événements connus d'avance (début et fin des segments de la liste
 * \p segments ).
 * Pour rappel, l'arbre est ordonné selon l'ordre impliqué par la fonction
 * point_precedes.
 *
 * @param[in] segments
 * @return l'arbre des événements connus d'avance
 */
static struct tree_t * initialize_events(const struct list_t * segments) {
	assert(segments);
	struct tree_t * t = new_tree();
	const struct list_node_t * aux = get_list_head(segments);
	while (aux) {
		struct Point * p1 = get_endpoint1(get_list_node_data(aux));
		struct Event * event1 = new_event(2,p1,get_list_node_data(aux),NULL);

		struct Point * p2 = get_endpoint2(get_list_node_data(aux));
		struct Event * event2 = new_event(3,p2,get_list_node_data(aux),NULL);

		tree_insert(t,p1,event1,point_precedes);
		tree_insert(t,p2,event2,point_precedes);

		aux = get_successor(aux);
	}
	return t;
}

/**
 * Vérifie si les segments \p s1 et \p s2 s'intersectent après la position de
 * l'événement \p event . Si oui et si cette intersection n'est pas détectée
 * auparavant, elle sera ajoutée dans la structure des événements (\p events ),
 * ainsi que dans la liste des intersections (\p intersections ).
 *
 * @param[in] s1
 * @param[in] s2
 * @param[in] event
 * @param[in,out] events
 * @param[in,out] intersections
 */

static void test_and_set_intersection(struct Segment * s1, struct Segment * s2, struct Event * event, struct tree_t * events, struct list_t * intersections) {
	assert(s1);
	assert(s2);
	assert(event);
	assert(events);
	assert(intersections);
	struct Point * intersection = get_intersection_point(s1,s2);
	if (intersection != NULL
		&& point_precedes(get_event_point(event),intersection)
		&& tree_find_node(get_root(events), intersection, point_precedes)){ //problèmes test appartenance à l'arbre
			struct Event * newEvent = new_event(0,intersection,s1,s2);
			tree_insert(events, intersections,newEvent, point_precedes );
			list_insert_last(intersections, intersection);
		}
}

/**
 * Gestion de l'événement \p event de type intersection.
 * L'état de la ligne de balayage décrite par la liste \p status doit être mis
 * à jour. La structure des événements (\p events ) ainsi que la liste des
 * intersections (\p intersections ) seront éventuellement mises à jour si une
 * nouvelle intersection sera détectée (utilisez la procedure
 * test_and_set_intersection).
 *
 * @param[in] event
 * @param[in,out] status
 * @param[in,out] events
 * @param[in,out] intersections
 */
static void handle_intersection_event(struct Event * event, struct list_t * status, struct tree_t * events, struct list_t * intersections) {
	assert(event);
	assert(status);
	assert(events);
	assert(intersections);

	struct Segment * s1 = get_event_segment1(event);
	struct Segment * s2 = get_event_segment2(event);

	struct list_node_t * node_s1 = list_find_node(status,s1);
	struct list_node_t * node_s2 = list_find_node(status,s2);
	list_swap_nodes_data(node_s1,node_s2);

	if (segment_precedes(s1,s2,get_x(get_event_point(event)))) {
		test_and_set_intersection(s1,get_list_node_data(get_successor(node_s1)),event,events,intersections);
		test_and_set_intersection(s2,get_list_node_data(get_predecessor(node_s2)),event,events,intersections);
	}
	else {
		test_and_set_intersection(s2,get_list_node_data(get_successor(node_s2)),event,events,intersections);
		test_and_set_intersection(s1,get_list_node_data(get_predecessor(node_s1)),event,events,intersections);
	}
}

/**
 * Gestion de l'événement \p event de type begin.
 * L'état de la ligne de balayage décrite par la liste \p status doit être mis
 * à jour. La structure des événements (\p events ) ainsi que la liste des
 * intersections (\p intersections ) seront éventuellement mises à jour si une
 * nouvelle intersection sera détectée (utilisez la procedure
 * test_and_set_intersection).
 *
 * @param[in] event
 * @param[in,out] status
 * @param[in,out] events
 * @param[in,out] intersections
 */
static void handle_begin_event(struct Event * event, struct list_t * status, struct tree_t * events, struct list_t * intersections) {
	assert(event);
	assert(status);
	assert(events);
	assert(intersections);
	const struct list_node_t * aux = get_list_head(status);
	while (aux && segment_precedes(get_list_node_data(aux),get_event_segment1(event),get_x(get_event_point(event)))){
		aux = get_successor(aux);
		}
	if (aux){
		list_insert_after(status,get_event_segment1(event),get_predecessor(aux));
	}
	else{
		list_insert_last(status,get_event_segment1(event));
	}
	struct Segment * predecesseur = get_list_node_data(get_predecessor(list_find_node(status,get_event_segment1(event))));
	struct Segment * successeur = get_list_node_data(get_successor(list_find_node(status,get_event_segment1(event))));

	test_and_set_intersection(get_event_segment1(event),successeur,event,events,intersections);
	test_and_set_intersection(get_event_segment1(event),predecesseur,event,events,intersections);
}

/**
 * Gestion de l'événement \p event de type end.
 * L'état de la ligne de balayage décrite par la liste \p status doit être mis
 * à jour. La structure des événements (\p events ) ainsi que la liste des
 * intersections (\p intersections ) seront éventuellement mises à jour si une
 * nouvelle intersection sera détectée (utilisez la procedure
 * test_and_set_intersection).
 *
 * @param[in] event
 * @param[in,out] status
 * @param[in,out] events
 * @param[in,out] intersections
 */
static void handle_end_event(struct Event * event, struct list_t * status, struct tree_t * events, struct list_t * intersections) {
	assert(event);
	assert(status);
	assert(events);
	assert(intersections);
	struct list_node_t * list_node = list_find_node(status,get_event_segment1(event));

	test_and_set_intersection(get_list_node_data(get_successor(list_node)),get_list_node_data(get_predecessor(list_node)),event,events,intersections);
	list_remove_node(status,list_node);
}

struct list_t * BentleyOttmann(const struct list_t * segments) {
	assert(segments);
	struct tree_t * file_priorite = initialize_events(segments);
	struct list_t * etat = new_list();
	struct list_t * intersections = new_list();

	while (file_priorite){
		struct Event * event = get_tree_node_data(tree_find_min(get_root(file_priorite)));
		if (get_event_type(event) == 0)
		{
			handle_intersection_event(event,etat,file_priorite,intersections);
		}
		else if (get_event_type(event) == 1)
		{
			handle_begin_event(event,etat,file_priorite,intersections);
		}
		else
		{
			handle_end_event(event,etat,file_priorite,intersections);
		}
		tree_remove(file_priorite, get_event_point(event), point_precedes);
	}
	delete_list(etat, free_segment);

	return intersections;
}