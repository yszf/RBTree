#ifndef __RBTREE_H__
#define __RBTREE_H__

#include <iostream>
#include <cassert>

// ����������ʣ�
// 1.ÿ�����Ҫô�Ǻ��㣬Ҫô�Ǻڽ��
// 2.������Ǻ�ɫ��
// 3.������(nil�ս��)�Ǻ�ɫ���
// 4.���������Ǻ�ɫ��㣬���亢�ӽ��һ���Ǻ�ɫ
// 5.�������㵽�����������·��������ͬ��Ŀ�ĺ�ɫ���

typedef enum {
	kRed,
	kBlack
}ColorType;

typedef int ValueType;

typedef struct RBTNode{
	ValueType key_value; // ��ֵ
	ColorType color; // �����ɫ����ɫ/��ɫ��
	struct RBTNode* parent; // ˫�׽��
	struct RBTNode* left_child; // ���ӽ��
	struct RBTNode* right_child; // �Һ��ӽ��
}RBTNode; // ���

typedef struct {
	size_t curr_size; // Ԫ��/������
	RBTNode* head; // ͷ��� (�����ӱ�ʾ��ǰ��Сֵ�Ľ�㣬�Һ��ӱ�ʾ��ǰ���ֵ�Ľ��)
	RBTNode* nil;  // �ս��
}RBTree;

// ������
RBTNode* MakeNode(RBTNode* pa, ColorType color);

// ��ʼ�������
void InitTree(RBTree* ptree);

// ������
void InsertNode(RBTree* ptree, ValueType val);

// ɾ�����
void DeleteNode(RBTree* ptree, ValueType val);

// �����С�������
void InOrder(RBTree* ptree);

// ����Ӵ�С����
void NiceOrder(RBTree* ptree);

// ���ٺ�������ͷ���Դ
void DestroyTree(RBTree* ptree);


#endif // __RBTREE_H__
