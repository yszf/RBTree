#include "rbtree.h"
#include <vld.h>

int main() {
	RBTree tree;
	InitTree(&tree);

//	int arr[10] = { 45, 67, 23, 91, 12, 78, 34, 89, 56, 100 };

	int arr[10] = { 11, 23, 56, 78, 34, 100, 99, 45, 9, 67 };

	for (int i = 0; i < 10; ++i) {
		if (!InsertNode(&tree, arr[i]))
			break;
	}

	InOrder(&tree);
	NiceInOrder(&tree);

	return 0;
}