#include "rbtree.h"

RBTNode* MakeNode(RBTNode* pa, ColorType color = kRed) {
	RBTNode* new_node = nullptr;
	new_node = new (std::nothrow) RBTNode;
	if (nullptr != new_node) {
		new_node->color = color;
		new_node->parent = pa;
		new_node->left_child = nullptr;
		new_node->right_child = nullptr;
	}
	return new_node;
}

void InitTree(RBTree* ptree) {
	ptree->head = MakeNode(nullptr, kBlack);
	ptree->nil = MakeNode(ptree->head, kBlack);
	if (nullptr == ptree->head || nullptr == ptree->nil) {
		std::cout << "init rb_tree error!" << std::endl;
	}
	ptree->head->left_child = ptree->nil;
	ptree->head->right_child = ptree->nil;
	ptree->curr_size = 0;
}

void RotateLeft(RBTree* ptree, RBTNode* x) {
	assert(nullptr == ptree && nullptr == ptree->head && nullptr == x);
	RBTNode* new_parent = x->right_child;
	new_parent->parent = x->parent;

	x->right_child = new_parent->left_child;
	if (new_parent->left_child != ptree->nil) {
		new_parent->left_child->parent = x;
	}

	if (x == ptree->head->parent) {
		ptree->head->parent = new_parent;
	}
	else {
		if (x == x->parent->left_child) {
			x->parent->left_child = new_parent;
		}
		else {
			x->parent->right_child = new_parent;
		}
	}
	x->parent = new_parent;
	new_parent->left_child = x;
}

void RotateRight(RBTree* ptree, RBTNode* x) {
	assert(nullptr != ptree && nullptr != ptree->head && nullptr != x);
	RBTNode* new_parent = x->left_child;
	new_parent->parent = x->parent;

	x->left_child = new_parent->right_child;
	if (new_parent->right_child != ptree->nil) {
		new_parent->right_child->parent = x->left_child;
	}

	if (x == ptree->head->parent) {
		ptree->head->parent = new_parent;
	}
	else {
		if (x == x->parent->left_child) {
			x->parent->left_child = new_parent;
		}
		else {
			x->parent->right_child = new_parent;
		}
	}
	x->parent = new_parent;
	new_parent->right_child = x;
}

void InsertAdjust(RBTree* ptree, RBTNode* x) {
	assert(nullptr != ptree && nullptr != ptree->head && nullptr != x);
	while (x->parent != ptree->head->parent && x->parent->color == kRed) {
		RBTNode* y;
		if (x->parent == x->parent->parent->left_child) {
			y = x->parent->parent->right_child;
			if (kRed == y->color) {
				y->color = kBlack;
				x->parent->color = kBlack;
				x->parent->parent->color = kRed;
				x = x->parent->parent;
			}
			else {
				if (x == x->parent->right_child) {
					x = x->parent;
					RotateLeft(ptree, x);
				}
				x->parent->color = kBlack;
				x->parent->parent->color = kRed;
				RotateRight(ptree, x->parent->parent);
			}
		}
		else {
			y = x->parent->parent->left_child;
			if (kRed == y->color) {
				y->color = kBlack;
				x->parent->color = kBlack;
				x->parent->parent->color = kRed;
				x = x->parent->parent;
			}
			else {
				if (x == x->parent->left_child) {
					x = x->parent;
					RotateRight(ptree, x);
				}
				x->parent->color = kBlack;
				x->parent->parent->color = kRed;
				RotateLeft(ptree, x->parent->parent);
			}
		}
	}

	x->color = kBlack;
}

bool Insert(RBTree* ptree, RBTNode* pa, ValueType val) {
	assert(nullptr != ptree && nullptr != ptree->head && nullptr != pa);
	RBTNode* new_node = MakeNode(pa);
	if (nullptr == new_node) {
		std::cout << "make node failed!" << std::endl;
		return false;
	}

	if (pa->key_value > val) {
		pa->left_child = new_node;
		if (new_node->key_value < ptree->head->left_child->key_value) {
			ptree->head->left_child = new_node;
		}
	}
	else {
		pa->right_child = new_node;
		if (new_node->key_value > ptree->head->right_child->key_value) {
			ptree->head->right_child = new_node;
		}
	}

	InsertAdjust(ptree, new_node);

	return true;
}

bool InsertNode(RBTree* ptree, ValueType val) {
	assert(nullptr != ptree && nullptr != ptree->head);
	if (nullptr == ptree->head->parent || 0 == ptree->curr_size) {
		RBTNode* root = MakeNode(ptree->head, kBlack);
		assert(nullptr != root);
		root->key_value = val;
		ptree->head->parent = root;
		ptree->head->left_child = root;
		ptree->head->right_child = root;
		root->left_child = ptree->nil;
		root->right_child = ptree->nil;
		return true;
	}

	RBTNode* pa = ptree->head;
	RBTNode* p = ptree->head->parent;
	while (p != ptree->nil && p->key_value != val) {
		pa = p;
		p = p->key_value < val ? p->right_child : p->left_child;
	}

	if (p != ptree->nil && p->key_value == val) {
		std::cout << "key_value repeated!" << std::endl;
		return false;
	}

	return Insert(ptree, pa, val);
}