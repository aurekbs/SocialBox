#include "application_Intersections.h"

#include <stdlib.h>

#include "list.h"
#include "geometry.h"
#include "algo.h"

JNIEXPORT void JNICALL Java_application_Intersections_runAllPairs
  (JNIEnv * env, jobject obj, jstring input_filename, jstring output_filename) {
	const char * segments_filename = (* env)->GetStringUTFChars(env, input_filename, NULL);
	const char * intersections_filename = (* env)->GetStringUTFChars(env, output_filename, NULL);

	struct list_t * segments = load_segments(segments_filename);
	struct list_t * intersections = all_pairs(segments);
	save_intersections(intersections_filename, intersections);
	delete_list(intersections, free);
	delete_list(segments, free_segment);
}

/**
 * Exécute l'algorithme Bentley-Ottmann sur les segments rangés
 * dans le fichier texte de nom \p input_filename et range les points
 * d'intersection dans le fichier texte de nom \p output_filename .
 *
 * @param[in] input_filename
 * @param[in] output_filename
 */
JNIEXPORT void JNICALL Java_application_Intersections_runBentleyOttmann
  (JNIEnv * env, jobject obj, jstring input_filename, jstring output_filename) {
	const char * segments_filename = (* env)->GetStringUTFChars(env, input_filename, NULL);
	const char * intersections_filename = (* env)->GetStringUTFChars(env, output_filename, NULL);

	struct list_t * segments = load_segments(segments_filename);
	struct list_t * intersections = BentleyOttmann(segments);
	save_intersections(intersections_filename, intersections);
	delete_list(intersections, free);
	delete_list(segments, free_segment);
}