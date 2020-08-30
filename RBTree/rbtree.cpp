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
		cout << "init rb_tree error!" << endl;
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
		cout << "make node failed!" << endl;
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
		cout << "key_value repeated!" << endl;
		return false;
	}

	return Insert(ptree, pa, val);
}

// ɾ����ɫ����, ����Υ���������(2)������Ǻ�ɫ;(4)������Ǻ�ɫ�����ӽ��һ���Ǻ�ɫ;
// (5)�������㵽�����������·���ϰ�����ͬ��Ŀ�ĺ�ɫ�����������ԡ���˸��滻��z�����Ӷ���ĺ�ɫ���ԣ�
// ��ʱz����ɫ�Ǻ�ɫ+��ɫ/��ɫ+��ɫ������Υ���������(1)��㲻�Ǻ�ɫ���Ǻ�ɫ;(3)������Ǻ�ɫ;
// (4)������Ǻ�ɫ�����ӽ��һ���Ǻ�ɫ���������ԡ�
// ����취����z�������ĺ�ɫ���Բ������������ƣ�ֱ��������������������ϡ�
// 1. zָ���+�ڽ�㣬ֱ�ӽ�z��Ϊ�ڽ�㡣 2. zָ�����㣬��z��Ϊ�ڽ�㡣
void DeleteAdjust(RBTree* ptree, RBTNode* z) {
	assert(nullptr != ptree && nullptr != ptree->head && nullptr != z);
	assert(z != ptree->head);

	while (z != ptree->head->parent && kRed != z->color) {
		RBTNode* w;// �ֵܽ��
		if (z == z->parent->left_child) {
			w = z->parent->right_child;
			// case 1
			// ��ʱ˫���Ǻ�ɫ�����ֵܵ����Һ����Ǻ�ɫ
			// ���Ҫ�����ֵܽ�һ����ɫ��㣬��Ҫ��֤���ֵܷ�֧��ɫ��Ŀ���䣬����Ҫ����ת
			// ��ΪҪ��֤���ֵ���������ת֮���ɫ��Ŀ���ֲ��䣨�����������ģ��������һ����ɫ��������˫�׽��Ϊ��ɫ����˲�û�н赽��ɫ���
			if (kRed == w->color) {
				w->color = kBlack;
				z->parent->color = kRed;
				RotateLeft(ptree, z->parent);
				w = z->parent->right_child;
			}

			// case 2
			// ��ɫ���ƣ���Ϊ���ֵ�û�п��Խ�ĺ�ɫ���
			if (kBlack == w->left_child->color && kBlack == w->right_child->color) {
				z = z->parent;
				w->color = kRed;
			}
			else {
				// case 3 
				// ת��case 4, ����֤���ֵܷ�֧���ʲ���
				if (kBlack == w->right_child->color) {
					w->color = kRed;
					w->left_child->color = kBlack;
					RotateRight(ptree, w);
				}

				// case 4
				// �赽��ɫ������֤���ֵܷ�֧���ʲ��䣬�������
				w->right_child->color = kBlack;
				w->color = z->parent->color;
				z->parent->color = kBlack;
				RotateLeft(ptree, z->parent);
				z = ptree->head->parent;
			}

		}
		else {
			w = z->parent->left_child;
			// case 1
			// ��ʱ˫�׽���Ǻ�ɫ�����ֵܵ����Һ��ӽ���Ǻ�ɫ
			// ���Ҫ�����ֵܽ�һ����ɫ��㣬���Խ��ֵܽ������Ϊ��ɫ��
			// ��Ҫ��֤���ֵܷ�֧���ʱ��ֲ��䣬���Ա��뽫˫�׽����Ϊ��ɫ����������ת
			// ������û�н赽��ɫ���
			if (kRed == w->color) {
				w->color = kBlack;
				z->parent->color = kRed;
				RotateRight(ptree, z->parent);
				w = z->parent->left_child;
			}

			// case 2
			// ��ʱw����ɫ�϶��Ǻ�ɫ, ��Ϊ�����ֵܷ�֧�費����ɫ�����Ժ�ɫ���ƣ�����֤��������ƽ��
			if (kBlack == w->left_child->color && kBlack == w->right_child->color) {
				z = z->parent;
				w->color = kRed;
			}
			else {
				// case 3
				if (kBlack == w->left_child->color) {
					w->color = kRed;
					w->right_child->color = kBlack;
					RotateLeft(ptree, w);
				}

				// case 4
				w->left_child->color = kBlack;
				w->color = z->parent->color;
				z->parent->color = kBlack;
				RotateRight(ptree, z->parent);
				z = ptree->head->parent;
			}
		}
	}
	// zָ���ɫ+��ɫ���
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
			y->parent->left_child = z;
		}
		else {
			if (y == ptree->head->right_child) {
				ptree->head->right_child = Prev(ptree, y);
			}
			y->parent->right_child = z;
		}
	}
}

bool Delete(RBTree* ptree, RBTNode* x) {
	assert(nullptr != ptree && nullptr != ptree->head && nullptr != x);
	assert(x != ptree->head && x != ptree->nil);

	RBTNode* y; // ����Ҫɾ���Ľ��
	RBTNode* z; // �滻��Ľ��
	if (x->left_child != ptree->nil && x->right_child != ptree->nil) {
		y = Next(ptree, x); // ��̽��
	}
	else {
		y = x;
	}

	if (x != y) {
		x->key_value = y->key_value;
	}
	else {
		if (x == ptree->head->left_child) {
			ptree->head->left_child = Next(ptree, x);
		}
		else if (x == ptree->head->right_child) {
			ptree->head->right_child = Prev(ptree, x);
		}
	}

	z = y->left_child == ptree->nil ? y->right_child : y->left_child;

	Replace(ptree, y, z);

	if (kBlack == y->color) {
		DeleteAdjust(ptree, z);
	}

	delete y;
	ptree->curr_size--;
	if (0 == ptree->curr_size) {
		ptree->head->parent = ptree->nil;
		ptree->head->left_child = ptree->nil;
		ptree->head->right_child = ptree->nil;
	}

	return true;
}

bool DeleteNode(RBTree* ptree, ValueType val) {
	assert(nullptr != ptree && nullptr != ptree->head);
	
	if (0 == ptree->curr_size) {
		return false;
	}

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

RBTNode* Prev(RBTree* ptree, RBTNode* pcur) {
	assert(nullptr != ptree && nullptr != ptree->head && nullptr != pcur);
	assert(pcur != ptree->head && pcur != ptree->nil);

	RBTNode* p = pcur->left_child;
	if (p != ptree->nil) {
		while (p->right_child != ptree->nil) {
			p = p->right_child;
		}
		return p;
	}
	else {
		RBTNode* pa = pcur->parent;
		while (pa != ptree->head && pa->right_child != pcur) {
			pcur = pa;
			pa = pcur->parent;
		}
		return pa;
	}

	return ptree->nil;
}

RBTNode* Next(RBTree* ptree, RBTNode* pcur) {
	assert(nullptr != ptree && nullptr != ptree->head && nullptr != pcur);
	assert(pcur != ptree->head && pcur != ptree->nil);

	RBTNode* p = pcur->right_child;
	if (p != ptree->nil) {
		while (p->left_child != ptree->nil) {
			p = p->left_child;
		}
		return p;
	}
	else {
		RBTNode* pa = pcur->parent;
		while (pa != ptree->head && pa->left_child != pcur) {
			pcur = pa;
			pa = pcur->parent;
		}
		return pa;
	}

	return ptree->nil;
}

void InOrder(RBTree* ptree) {
	assert(nullptr != ptree && nullptr != ptree->head);

	for (RBTNode* it = Min(ptree); it != ptree->nil && it != ptree->head; it = Next(ptree, it)) {
		cout << it->key_value;
		it->color == kRed ? cout << "(Red) " : cout << "(Black) ";
	}
	cout << endl;
}

void NiceInOrder(RBTree* ptree) {
	assert(nullptr != ptree && nullptr != ptree->head);

	for (RBTNode* it = Max(ptree); it != ptree->nil && it != ptree->head; it = Prev(ptree, it)) {
		cout << it->key_value;
		it->color == kRed ? cout << "(Red) " : cout << "(Black) ";
	}
	cout << endl;
}

void DestroyTree(RBTree* ptree) {
	assert(nullptr != ptree && nullptr != ptree->head);
	delete ptree->head;
	ptree->head = nullptr;
	delete ptree->nil;
	ptree->nil = nullptr;
	ptree->curr_size = 0;
}