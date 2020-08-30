#include "rbtree.h"
#include <vld.h>

int main() {
	RBTree tree;
	// ��ʼ�������
	InitTree(&tree);

//	int arr[10] = { 45, 67, 23, 91, 12, 78, 34, 89, 56, 100 };

	int arr[10] = { 11, 23, 56, 78, 34, 100, 99, 45, 9, 67 };

//	int arr[] = { 41, 38, 31, 12, 19, 8, 20, 55, 34, 25};

	for (int i = 0; i < 10; ++i) {
		if (!InsertNode(&tree, arr[i])) {
			cout << "����[" << arr[i] << "]ʧ�ܣ�" << endl;
			break;
		}
		else {
			cout << "����[" << arr[i] << "]��" << endl;
			InOrder(&tree);
		//	NiceInOrder(&tree);
		}
	}

//	cout << "�������ս����" << endl;
//	InOrder(&tree);
	NiceInOrder(&tree);

	for (int i = 0; i < 10; ++i) {
		if (!DeleteNode(&tree, arr[i])) {
			cout << "ɾ��[" << arr[i] << "]ʧ�ܣ�" << endl;
			break;
		}
		else {
			cout << "ɾ��[" << arr[i] << "]��" << endl;
			InOrder(&tree);
		//	NiceInOrder(&tree);
		}
	}

//	cout << "ɾ�����ս����" << endl;
//	InOrder(&tree);
//	NiceInOrder(&tree);

	if (0 != tree.curr_size) {
		for (RBTNode* it = Min(&tree); it != tree.nil && it != tree.head; it = Next(&tree, it)) {
			if (!DeleteNode(&tree, it->key_value)) {
				exit(1);
			}
		}
	}

	// ���ٺ����
	DestroyTree(&tree);

	return 0;
}