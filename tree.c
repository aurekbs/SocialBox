#include "tree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

/********************************************************************
 * tree_node_t
 ********************************************************************/

/**
 * Construit et initialise un nouveau nœud d'un arbre binaire de recherche.
 * Le facteur d'équilibre est initialisé à zero.
 * 
 * @param[in] key Clé du nouveau nœud.
 * @param[in] data Donnée à affecter au nouveau nœud.
 * @return le nouveau nœud créé
 */
static struct tree_node_t * new_tree_node(void * key, void * data) {
	struct tree_node_t * node = malloc(sizeof(struct tree_node_t));
	node->key = key;
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	return node;
}

void * get_tree_node_key(const struct tree_node_t * node) {
	assert(node);
	return node->key;
}

void * get_tree_node_data(const struct tree_node_t * node) {
	assert(node);
	return node->data;
}

struct tree_node_t * get_left(const struct tree_node_t * node) {
	assert(node);
	return node->left;
}

struct tree_node_t * get_right(const struct tree_node_t * node) {
	assert(node);
	return node->right;
}

void set_tree_node_key(struct tree_node_t * node, void * new_key) {
	assert(node);
	node->key = new_key;
}

void set_tree_node_data(struct tree_node_t * node, void * new_data) {
	assert(node);
	node->data = new_data;
}

void set_left(struct tree_node_t * node, struct tree_node_t * new_left) {
	assert(node);
	node->left = new_left;
}

void set_right(struct tree_node_t * node, struct tree_node_t * new_right) {
	assert(node);
	node->right = new_right;
}

/*********************************************************************
 * tree_t
 *********************************************************************/

struct tree_t * new_tree() {
	struct tree_t * tree = malloc(sizeof(struct tree_t));
	tree->root = NULL;
	tree->size = 0;
	return tree;
}

int tree_is_empty(const struct tree_t * T) {
	assert(T);
	if(T->size == 0) {
		return 1;
	} else {
		return 0;
	}
}

unsigned int get_tree_size(const struct tree_t * T) {
	assert(T);
	return T->size;
}

struct tree_node_t * get_root(const struct tree_t * T) {
	assert(T);
	return T->root;
}

void increase_tree_size(struct tree_t * T) {
	assert(T);
	T->size += 1;
}

void decrease_tree_size(struct tree_t * T) {
	assert(T);
	T->size -= 1;
}

void set_root(struct tree_t * T, struct tree_node_t * new_root) {
	assert(T);
	T->root = new_root;
}

/**
 * Libérer récursivement le sous-arbre raciné au nœud \p curr .
 * Dans le cas où le pointeur de fonction \p freeKey (respectivement
 * \p freeData ) n'est pas NULL, la mémoire de la clé (respectivement de la
 * donnée) du nœud actuel est aussi libérée.
 * NB : procédure récursive.
 * 
 * @param[in] curr
 * @param[in] freeKey 
 * @param[in] freeData 
 */
static void delete_tree_node(struct tree_node_t * curr, void (*freeKey)(void *), void (*freeData)(void *)) {
	if(curr != NULL) {
			delete_tree_node(get_left(curr), freeKey, freeData);
			delete_tree_node(get_right(curr), freeKey, freeData);
		if(freeKey != NULL) {
			freeKey(get_tree_node_key(curr));
		}
		if(freeData != NULL) {
			freeData(get_tree_node_data(curr));
		}


		free(curr);
		}
	}


// NB : Utiliser la procédure récursive delete_tree_node.
void delete_tree(struct tree_t * T, void (*freeKey)(void *), void (*freeData)(void *)) {//à vérifier
	assert(T);
	if(get_root(T)!= NULL) {
		delete_tree_node(get_root(T), freeKey, freeData);
	}
	free(T);
}

/**
 * Afficher récursivement le sous-arbre raciné au nœud \p curr en utilisant un
 * ordre infixe.
 * NB : procédure récursive.
 *
 * @param[in] curr 
 * @param[in] viewKey 
 * @param[in] viewData 
 */
static void view_tree_inorder(struct tree_node_t * curr, void (*viewKey)(const void *), void (*viewData)(const void *)) {

	if(curr != NULL) {
		view_tree_inorder(get_left(curr), viewKey, viewData);
		printf("\n Key: ");
		viewKey(get_tree_node_key(curr));
		printf("  Data: ");
		viewData(get_tree_node_data(curr));
		view_tree_inorder(get_right(curr), viewKey, viewData);
	}
}
// NB : Utiliser la procédure récursive view_tree_inorder.
void view_tree(const struct tree_t * T, void (*viewKey)(const void * key), void (*viewData)(const void * data)) {
	assert(T);
	if(get_root(T) != NULL) {
		view_tree_inorder(get_root(T), viewKey, viewData);
	}
}

/**
 * Insérer un nouveau nœud de clé \p key et de donnée \p data au sous-arbre
 * binaire de recherche raciné au nœud \p curr .
 * Les clés sont comparées en utilisant le pointeur de fonction \p precedes .
 * Le nœud curant \p curr est en entrée/sortie et doit être mis à jour.
 * NB : fonction récursive.
 * 
 * @param[in,out] curr
 * @param[in] key 
 * @param[in] data 
 * @param[in] precedes
 */
static void insert_tree_node(struct tree_node_t ** curr, void * key, void * data,
							int (*precedes)(const void *, const void *)) {
	if(*curr == NULL) {
		*curr = new_tree_node(key, data);

	} else {
		if(precedes(get_tree_node_key(*curr), key) ){ //si la clé du noeud est plus petit que key
			struct tree_node_t *right = get_right(*curr);
			insert_tree_node(&right, key, data, precedes);
			set_right(*curr, right);
		}
		else {
			struct tree_node_t *left = get_left(*curr);
			insert_tree_node(&left, key, data, precedes);
			set_left(*curr, left);
		}
	}
}
// NB : Utiliser la fonction récursive insert_tree_node.
void tree_insert(struct tree_t * T, void * key, void * data, int (*precedes)(const void * a, const void * b)) {
	assert(T);
	struct tree_node_t * curr = get_root(T);
	if(curr==NULL) {
		T->root = new_tree_node(key, data);
	}
	else {
		insert_tree_node(&curr, key, data, precedes);
	}

	increase_tree_size(T);
}

struct tree_node_t * tree_find_min(struct tree_node_t * curr) {
	assert(curr);
	if (get_left(curr) == NULL) {
		return curr;
	}
	else {
		return tree_find_min(get_left(curr));
	}
}

struct tree_node_t * tree_find_max(struct tree_node_t * curr) {
	assert(curr);
	if (get_right(curr) == NULL) {
		return curr;
	}
	else {
		return tree_find_max(get_right(curr));
	}

}

struct tree_node_t * tree_find_node(struct tree_node_t * curr, const void * key, int (*precedes)(const void *, const void *)) {
	if (curr == NULL) {
		return NULL;
	}
	if (get_tree_node_key(curr) == key)
		return curr;

	else {
		if(precedes(get_tree_node_key(curr), key) ){ //si la clé du noeud est plus petit que key
			return tree_find_node(get_right(curr), key, precedes);
		}
		else
			return tree_find_node(get_left(curr), key, precedes);
	}
}

struct tree_node_t * tree_find_predecessor(struct tree_node_t * curr, const void * key, int (*precedes)(const void *, const void *)) {
	assert(curr);
	struct tree_node_t *predecessor = NULL;

	if (precedes(get_tree_node_key(curr), key)) {

		predecessor = tree_find_predecessor(get_right(curr), key, precedes);
		if (predecessor == NULL) {
			predecessor = curr;
		}
		return predecessor;
	}


	if (precedes(key, get_tree_node_key(curr))) {
		return tree_find_predecessor(get_left(curr), key, precedes);
	}


	if (get_left(curr) != NULL) {
		return tree_find_max(get_left(curr));
	}
	return NULL;
}

struct tree_node_t * tree_find_successor(struct tree_node_t * curr, const void * key, int (*precedes)(const void *, const void *)) {
	assert(curr);
	struct tree_node_t *successor = NULL;
	if (precedes(key, get_tree_node_key(curr))) {
		successor = tree_find_successor(get_left(curr), key, precedes);
		if (successor == NULL) {
			successor = curr;
		}
		return successor;
	}

	if (precedes(get_tree_node_key(curr), key)) {
		return tree_find_successor(get_right(curr), key, precedes);
	}

	if (get_right(curr) != NULL) {
		return tree_find_min(get_right(curr));
	}
	return NULL;

}

/**
 * Supprimer le nœud de clé \p key du sous-arbre binaire de recherche raciné au
 * nœud \p curr . La donnée du nœud supprimé est restituée, tandis que la
 * mémoire du nœud avec clé \p key est libérée (mais pas la mémoire de la clé).
 * Les clés sont comparées en utilisant le pointeur de fonction \p precedes.
 * Le nœud curant \p curr est en entrée/sortie et doit être mis à jour.
 * NB : fonction récursive.
 * 
 * @param[in,out] curr
 * @param[in] key
 * @param[in] precedes
 * @return la donnée du nœud supprimé
 */
static void * remove_tree_node(struct tree_node_t ** curr, void * key, int (*precedes)(const void *, const void *)) {
	assert(curr);
	if (*curr == NULL) {
		return NULL;
	}

	if (precedes(key, get_tree_node_key(*curr))) {
		return remove_tree_node(&(*curr)->left, key, precedes);
	} else if (precedes(get_tree_node_key(*curr), key)) {
		return remove_tree_node(&(*curr)->right, key, precedes);
	} else {
		struct tree_node_t *temp;
		void * data = get_tree_node_data(*curr);

		if (get_left(*curr) == NULL) {
			// pas de fils gauche
			temp = *curr;
			*curr = get_right(*curr);
			free(temp);
		} else if (get_right(*curr) == NULL) {
			// pas de fils droit
			temp = *curr;
			*curr = get_left(*curr);
			free(temp);
		} else {
			// fils droit et gauche
			const struct tree_node_t *successor = tree_find_successor(*curr, key, precedes);
			void* successor_key = get_tree_node_key(successor);
			void* successor_data = get_tree_node_data(successor);
			set_tree_node_key(*curr, successor_key);
			set_tree_node_data(*curr, successor_data);

			remove_tree_node(&(*curr)->right, successor_key, precedes);
		}

		return data;
	}
}


// NB : Utiliser la fonction récursive remove_tree_node.
void * tree_remove(struct tree_t * T, void * key, int (*precedes)(const void * a, const void * b)) {
	assert(T);
	struct tree_node_t * curr = get_root(T);
	void * data = remove_tree_node(&curr, key, precedes);
	set_root(T,curr);
	decrease_tree_size(T);
	return data;
}