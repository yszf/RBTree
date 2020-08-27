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
	assert(nullptr != ptree && nullptr != ptree->head && nullptr != x);
	assert(x != ptree->head && x != ptree->nil);
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
	assert(x != ptree->head && x != ptree->nil);
	RBTNode* new_parent = x->left_child;
	new_parent->parent = x->parent;

	x->left_child = new_parent->right_child;
	if (new_parent->right_child != ptree->nil) {
		new_parent->right_child->parent = x;
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
	assert(x != ptree->head && x != ptree->nil);
	while (x != ptree->head->parent && kRed == x->parent->color) {
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

	ptree->head->parent->color = kBlack;
}

bool Insert(RBTree* ptree, RBTNode* pa, ValueType val) {
	assert(nullptr != ptree && nullptr != ptree->head && nullptr != pa);
	assert(pa != ptree->head && pa != ptree->nil);
	RBTNode* new_node = MakeNode(pa);
	if (nullptr == new_node) {
		std::cout << "make node failed!" << std::endl;
		return false;
	}
	new_node->key_value = val;
	new_node->left_child = ptree->nil;
	new_node->right_child = ptree->nil;

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
	ptree->curr_size++;
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
		ptree->curr_size++;
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

// 删除黑色结点后, 可能违反红黑树的(2)根结点是黑色;(4)父结点是红色，则子结点一定是黑色;
// (5)从任意结点到子孙结点的所有路径上包含相同数目的黑色结点等三个特性。因此给替换的z结点添加额外的黑色属性，
// 此时z的颜色是红色+黑色/黑色+黑色，可能违反红黑树的(1)结点不是黑色就是红色;(3)根结点是黑色;
// (4)父结点是红色，则子结点一定是黑色等三个特性。
// 解决办法：将z所包含的黑色属性不断向根结点上移，直至遇到以下情况则调整完毕。
// 1. z指向红+黑结点，直接将z设为黑结点。 2. z指向根结点，将z设为黑结点。
void DeleteAdjust(RBTree* ptree, RBTNode* z) {
	assert(nullptr != ptree && nullptr != ptree->head && nullptr != z);
	assert(z != ptree->head && z != ptree->nil);

	while (z != ptree->head->parent && z->color != kRed) {
		RBTNode* w;// 兄弟结点
		if (z == z->parent->left_child) {
			w = z->parent->right_child;
			// case 1
			// 此时双亲是黑色，右兄弟的左右孩子是黑色
			// 如果要向右兄弟借一个黑色结点，但要保证右兄弟分支黑色数目不变，所以要左旋转
			// 因为要保证右兄弟左孩子在旋转之后黑色数目保持不变，故设双亲结点为红色，因此并没有借到黑色结点
			if (kRed == w->color) {
				w->color = kBlack;
				z->parent->color = kRed;
				RotateLeft(ptree, z->parent);
				w = z->parent->right_child;
			}

			// case 2
			if (kBlack == w->left_child->color && kRed == w->right_child->color) {
				z = z->parent;
				w->color = kRed;
			}
		}
		else {
			w = z->parent->left_child;
		}
	}
	// z指向红色+黑色结点
	z->color = kBlack;
}

void Replace(RBTree* ptree, RBTNode* y, RBTNode* z) {
	assert(nullptr != ptree && nullptr != ptree->head);
	assert(nullptr != y && y != ptree->nil && nullptr != z);
	z->parent = y->parent;
	if (y == ptree->head->parent) {
		ptree->head->parent = z;
	}
	else {
		if (y == y->parent->left_child) {
			if (y == ptree->head->left_child) {
				ptree->head->left_child = Next(ptree, y);
			}
			z = y->parent->left_child;
		}
		else {
			if (y == ptree->head->right_child) {
				ptree->head->right_child = Prev(ptree, y);
			}
			z = y->parent->right_child;
		}
	}
}

bool Delete(RBTree* ptree, RBTNode* x) {
	assert(nullptr != ptree && nullptr != ptree->head && nullptr != x);
	assert(x != ptree->head && x != ptree->nil);

	RBTNode* y; // 真正要删除的结点
	RBTNode* z; // 替换后的结点
	if (x->left_child != ptree->nil && x->right_child != ptree->nil) {
		y = Next(ptree, x); // 后继结点
	}
	else {
		y = x;
	}

	if (x != y) {
		x->key_value = y->key_value;
	}

	z = y->left_child == ptree->nil ? y->right_child : y->left_child;
	Replace(ptree, y, z);

	if (kBlack == y->color) {
		DeleteAdjust(ptree, z);
	}

	delete y;
	ptree->curr_size--;
	return true;
}

bool DeleteNode(RBTree* ptree, ValueType val) {
	assert(nullptr != ptree && nullptr != ptree->head);
	RBTNode* p = ptree->head->parent;
	while (p != ptree->nil && p->key_value != val) {
		p = p->key_value < val ? p->right_child : p->left_child;
	}

	if (p == ptree->nil) {
		return false;
	}

	return Delete(ptree, p);
}

RBTNode* Min(RBTree* ptree) {
	assert(nullptr != ptree && nullptr != ptree->head);
	return ptree->head->left_child;
}

RBTNode* Max(RBTree* ptree) {
	assert(nullptr != ptree && nullptr != ptree->head);
	return ptree->head->right_child;
}

RBTNode* Next(RBTree* ptree, RBTNode* cur) {
	assert(nullptr != ptree && nullptr != ptree->head && nullptr != cur);
	assert(cur != ptree->head && cur != ptree->nil);

	RBTNode* p = cur->right_child;
	if (p != ptree->nil) {
		while (p->left_child != ptree->nil) {
			p = p->left_child;
		}
		return p;
	}
	else {
		RBTNode* pa = cur->parent;
		while (pa != ptree->head && pa->left_child != cur) {
			cur = pa;
			pa = cur->parent;
		}
		return pa;
	}

	return ptree->nil;
}

RBTNode* Prev(RBTree* ptree, RBTNode* cur) {
	assert(nullptr != ptree && nullptr != ptree->head && nullptr != cur);
	assert(cur != ptree->head && cur != ptree->nil);

	RBTNode* p = cur->left_child;
	if (p != ptree->nil) {
		while (p->right_child != ptree->nil) {
			p = p->right_child;
		}
		return p;
	}
	else {
		RBTNode* pa = cur->parent;
		while (pa != ptree->head && pa->right_child != cur) {
			cur = pa;
			pa = cur->parent;
		}
		return pa;
	}

	return ptree->nil;
}

void InOrder(RBTree* ptree) {
	assert(nullptr != ptree && nullptr != ptree->head);

	for (RBTNode* it = Min(ptree); it != ptree->nil && it != ptree->head; it = Next(ptree, it)) {
		std::cout << it->key_value;
		it->color == kRed ? std::cout << "(Red) " : std::cout << "(Black) ";
	}
	std::cout << std::endl;
}

void NiceInOrder(RBTree* ptree) {
	assert(nullptr != ptree && nullptr != ptree->head);

	for (RBTNode* it = Max(ptree); it != ptree->nil && it != ptree->head; it = Prev(ptree, it)) {
		std::cout << it->key_value;
		it->color == kRed ? std::cout << "(Red) " : std::cout << "(Black) ";
	}
	std::cout << std::endl;
}

void DestroyTree(RBTree* ptree) {
	assert(nullptr != ptree && nullptr != ptree->head);
	delete ptree->head;
	ptree->head = nullptr;
	delete ptree->nil;
	ptree->nil = nullptr;
	ptree->curr_size = 0;
}