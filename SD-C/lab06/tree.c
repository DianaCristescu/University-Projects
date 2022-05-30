/*
*	Created by Nan Mihai on 05.05.2021
*	Copyright (c) 2020 Nan Mihai. All rights reserved.
*	Laborator 6 - Structuri de date
*	Grupa 314CD
*	Facultatea de Automatica si Calculatoare
*	Anul Universitar 2020-2021, Seria CD
*/
#include "tree.h"

/*
*	Funcție care creează un arbore cu un singur nod
*/
Tree createTree(Tree parent, Item value) {
	parent = NULL;
	parent = malloc(sizeof(TreeNode));
	parent->parent = NULL;
	parent->value = value;
	parent->left = NULL;
	parent->right = NULL;
	return parent;
}

/*
*	Funcție care inițializează un nod de arbore
*		- îi alocă memorie
*		- îi setează câmpul valoare
*		- setează left și right să pointeze către NULL
*/
void init(Tree *root, Tree parent, Item value) {
	// TODO 0
	*root = NULL;
	*root = malloc(sizeof(TreeNode));
	(*root)->value = value;
	(*root)->left = NULL;
	(*root)->right = NULL;
	(*root)->parent = parent;
}

/*
*	Funcție care inserează o valoare într-un arbore binar, respectând
* proprietățile unui arbore binar de căutare
*/
Tree insert(Tree root, Item value) {
	// TODO 1
	if(root==NULL) {
		root = createTree(root, value);
		return root;
	} else if(root->value == value) {
		return root;
	} else {
		if(root->value > value) {
			if(root->left == NULL) {
				init(&root->left, root, value);
				return root;
			} else {
				root->left = insert(root->left, value);
				return root;
			}
		} else {
			if(root->right == NULL) {
				init(&root->right, root, value);
				return root;
			} else {
				root->right = insert(root->right, value);
				return root;
			}
		}
	}
}


/*
*	Funcție care verifică dacă un arbore este vid sau nu
*		- întoarce 1 dacă arborele este vid
*		- întoarce 0 dacă arborele nu este vid
*/
int isEmpty(Tree root) {
	// TODO 2
	if(root == NULL) return 1;
	else return 0;
}

/*
*	Funcție care verifică dacă o valoare există sau nu în arbore
*		- 1 dacă value există în arbore
*		- 0 dacă value nu există în arbore
*/
int contains(Tree tree, Item value) {
	// TODO 3

	if(isEmpty(tree)) return 0;
	if(tree->value == value) return 1;

	if(value < tree->value)
		return contains(tree->left, value);
	if(value > tree->value)
		return contains(tree->right, value);

}

/*
*	Funcție care determină elementul minim dintr-un arbore binar de căutare
*		- NULL pentru arbore vid
*/
Tree minimum(Tree tree) {
	// TODO 4.1

	if(isEmpty(tree) || isEmpty(tree->left)) return tree;
	else return minimum(tree->left);

}

/*
*	Funcție care determină elementul maxim dintr-un arbore binar de căutare
*		- NULL pentru arbore vid
*/
Tree maximum(Tree tree) {
	// TODO 4.2

	if(isEmpty(tree) || isEmpty(tree->right)) return tree;
	else return maximum(tree->right);

}

Tree getNode(Tree tree, Item value) {
	// TODO 3

	if(isEmpty(tree)) return NULL;
	if(tree->value == value) return tree;

	if(value < tree->value)
		return getNode(tree->left, value);
	if(value > tree->value)
		return getNode(tree->right, value);

}

/*
*	Funcție care determină succesorul în inordine pentru nodul care conține
* valoarea value.
*/
Tree successor(Tree root, Item value) {
	// TODO 5

	if(maximum(root) == NULL || maximum(root)->value == value)  return NULL;

	Tree node = getNode(root, value);
	
	if(!isEmpty(node->right)) return minimum(node->right);
	else {
		while(node->parent->left != node) {
			node = node->parent;
		}
		return node->parent;
	}

}

/*
*	Funcție care determină predecesorul în inordine pentru nodul care conține
* valoarea value.
*/
Tree predecessor(Tree root, Item value) {
	// TODO 6

	if(minimum(root) == NULL || minimum(root)->value == value)  return NULL;

	Tree node = getNode(root, value);

	if(!isEmpty(node->left)) return maximum(node->left);
	else {
		while(node->parent->right != node) {
			node = node->parent;
		}
		return node->parent;
	}

}

/*
*	Funcție care dealocă întreaga memorie alocată pentru un arbore binar
*		- root va pointa către NULL după ce se va apela funcția
*/
void destroyTree(Tree *root) {
	// TODO 6
	if(!isEmpty(*root)) {
		if(!isEmpty((*root)->left)) destroyTree(&((*root)->left));
		if(!isEmpty((*root)->right)) destroyTree(&((*root)->right));
		free(*root);
		*root = NULL;
	}
	return;
}

/*
*	Funcție care șterge un nod cu o anumită valoare din arbore
*/
Tree delete(Tree root, Item value) {	//1 copil -> leg. se transfera la parinte; 2 copii -> se inlocuieste cu succesorul/predecesorul
	// TODO 7

	if(isEmpty(root)) return root;

	if(root->value > value)
		root->left = delete(root->left, value);
	else if(root->value < value)
		root->right = delete(root->right, value);
	else {
		Tree tmp = NULL;
		if(!isEmpty(root->left) && !isEmpty(root->right)) {
			tmp = minimum(root->right);
			root->value = tmp->value;
			root->right = delete(root->right, tmp->value);
		} else {
			tmp = root;
			if(!isEmpty(root->left)) 
				root = root->left;
			else
				root = root->right;
			free(tmp);
		}
	}
	
	return root;
}

/*
*	Funcție care determină cel mai apropiat strămoș comun pentru
* două noduri având cheile value1 și value2
*/
Tree lowestCommonAncestor(Tree root, Item value1, Item value2) {
	// TODO 8
	if(root == NULL) return NULL;
	if(root->value < value1 && root->value < value2)
		return lowestCommonAncestor(root->right, value1, value2);
	if(root->value > value1 && root->value > value2)
		return lowestCommonAncestor(root->left, value1, value2);
	return root;
}
