#ifndef __RBTREE_H__
#define __RBTREE_H__

#include <iostream>
#include <cassert>

// 红黑树的性质：
// 1.每个结点要么是红结点，要么是黑结点
// 2.根结点是黑色的
// 3.子孙结点(nil空结点)是黑色结点
// 4.如果父结点是红色结点，则其孩子结点一定是黑色
// 5.从任意结点到子孙结点的所有路径包含相同数目的黑色结点

typedef enum {
	kRed,
	kBlack
}ColorType;

typedef int ValueType;

typedef struct RBTNode{
	ValueType key_value; // 键值
	ColorType color; // 结点颜色（红色/黑色）
	struct RBTNode* parent; // 双亲结点
	struct RBTNode* left_child; // 左孩子结点
	struct RBTNode* right_child; // 右孩子结点
}RBTNode; // 结点

typedef struct {
	size_t curr_size; // 元素/结点个数
	RBTNode* head; // 头结点 (其左孩子表示当前最小值的结点，右孩子表示当前最大值的结点)
	RBTNode* nil;  // 空结点
}RBTree;

// 制造结点
RBTNode* MakeNode(RBTNode* pa, ColorType color);

// 初始化红黑树
void InitTree(RBTree* ptree);

// 插入结点
void InsertNode(RBTree* ptree, ValueType val);

// 删除结点
void DeleteNode(RBTree* ptree, ValueType val);

// 中序从小到大遍历
void InOrder(RBTree* ptree);

// 中序从大到小遍历
void NiceOrder(RBTree* ptree);

// 销毁红黑树，释放资源
void DestroyTree(RBTree* ptree);


#endif // __RBTREE_H__
