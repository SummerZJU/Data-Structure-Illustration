#ifndef _AVL_H_
#define _AVL_H_
#include <iostream>
#include <stack>
#include <string>
using namespace std;

#define AVL_DEBUG

template<typename T>
struct TNode {
	T key;
	TNode *left;
	TNode *right;
	int height;

	TNode(const T& key);
	~TNode(); // auto-recursion freeTree! hahah ==!
};

template<typename T>
class AVLTree {	
	int getHeight(TNode<T> *pos);
	
	TNode<T> *getRoot();
	TNode<T> *RR(TNode<T> *pos);
	TNode<T> *LL(TNode<T> *pos);
	TNode<T> *RL(TNode<T> *pos);
	TNode<T> *LR(TNode<T> *pos);
	TNode<T> *root;
public:
#ifdef AVL_DEBUG
	void print(TNode<T> *cur);
	void print();
#endif
	AVLTree();
	virtual ~AVLTree();
	TNode<T> *insert(TNode<T> *pos, const T& key); //recurson for implementation
	void insert(const T& key);
	void erase(TNode<T> *pos);
	void erase(const T& key);
	TNode<T> *find(const T& key);
};

//------------------------------------------------------------------------------//
//TNode template declaration
template <typename T>
TNode<T>::TNode(const T& key):
	key(key),
	left(nullptr),
	right(nullptr),
	height(0)
{

}

// root-first recurrensive dtor!
template <typename T>
TNode<T>::~TNode()
{
	if(left) delete left;
	if(right) delete right;
}

//---------------------------------------------------------------------------//
//AVLTree template declaration
template <typename T>
AVLTree<T>::AVLTree(): 
	root(nullptr)
{

}

template <typename T>
AVLTree<T>::~AVLTree()
{
	if(root) delete root;
}

template <typename T>
TNode<T> *AVLTree<T>::getRoot()
{ 
	return root; 
}

template<typename T>
int AVLTree<T>::getHeight(TNode<T> *pos)
{
	if(!pos) {
		return -1;
	} else {
		int lHeight = getHeight(pos->left);
		int rHeight = getHeight(pos->right);
		pos->height = lHeight > rHeight ? lHeight + 1 : rHeight + 1;
		return pos->height;
	}
}

template<typename T>
TNode<T> *AVLTree<T>::RR(TNode<T> *pos)
{
	TNode<T> *temp = pos->right;
	pos->right = temp->left;
	temp->left = pos;

	return temp;
}

template<typename T>
TNode<T> *AVLTree<T>::LL(TNode<T> *pos)
{
	TNode<T> *temp = pos->left;
	pos->left = temp->right;
	temp->right = pos;

	return temp;
}

template<typename T>
TNode<T> *AVLTree<T>::RL(TNode<T> *pos)
{
	pos->right = LL(pos->right);
	return RR(pos);
}

template<typename T>
TNode<T> *AVLTree<T>::LR(TNode<T> *pos)
{
	pos->left = RR(pos->left);
	return LL(pos);
}

template<typename T>
TNode<T> *AVLTree<T>::insert(TNode<T> *pos, const T& key)
{
	TNode<T> *cur = new TNode<T>(key), *ret = pos;
	if(root) {
		if(!pos) {
			ret = cur;
		} else if(pos->key > key) {
			pos->left = insert(pos->left, key);
			if(getHeight(pos->left) - getHeight(pos->right) > 1) {
				if(key > pos->left->key) {
					ret = LR(pos);
				} else {
					ret = LL(pos);
				}
			}
		} else {
			pos->right = insert(pos->right, key);
			if(getHeight(pos->right) - getHeight(pos->left) > 1) {
				if(key < pos->right->key) {
					ret = RL(pos);
				} else {
					ret = RR(pos);
				}
			}
		}
		ret->height = getHeight(ret);
	} else {
		ret = root = cur;
	}

	return ret;
}

template <typename T>
void AVLTree<T>::insert(const T& key)
{
	root = insert(root, key);

	return;
}

// assume pos != nullptr
template <typename T>
void AVLTree<T>::erase(TNode<T> *pos)
{
	stack<TNode<T> *> myStack;
	TNode<T> *work = root;
	while(work->key != pos->key) {
		myStack.push(work);
		if(work->key > pos->key) {
			work = work->left;
		} else {
			work = work->right;
		}
	} // pos is not inStack()
	TNode<T> *delPos = pos;
	if(pos->right && pos->left) {
		myStack.push(pos);
		delPos = pos->left;
		while(delPos->right) {
			myStack.push(delPos);
			delPos = delPos->right;
		}
		pos->key = delPos->key;
	}
	TNode<T> *after = delPos->right ? delPos->right : delPos->left, *level;
	if(delPos == root) {
		root = after; // maybe nullptr;
	} else {
		// at least one TNode in stack
		TNode<T> *temp = myStack.top();
		myStack.pop();

		level = temp; // at least oldRoot!!!
		if(temp->left == delPos) {
			temp->left = after;
			if(getHeight(temp->right) - getHeight(temp->left) > 1) {
				if(getHeight(temp->right->left) > getHeight(temp->right->right)) {
					level = RL(temp);
				} else {
					level = RR(temp);
				}
			}
		} else {
			temp->right = after;
			if(getHeight(temp->left) - getHeight(temp->right) > 1) {
				if(getHeight(temp->right->right) > getHeight(temp->right->left)) {
					level = LR(temp);
				} else {
					level = LL(temp);
				}
			}
		}
		level->height = getHeight(level);
		delPos->left = delPos->right = nullptr;
		delete delPos;

		while(!myStack.empty()) {
			TNode<T> *top = myStack.top();
			myStack.pop();
			if(top->left == temp) {
				top->left = level;
				level = top;
				if(getHeight(top->right) - getHeight(top->left) > 1) {
					if(getHeight(top->right->left) > getHeight(top->right->right)) {
						level = RL(top);
					} else {
						level = RR(top);
					}
				}
			} else {
				top->right = level;
				level = top;
				if(getHeight(top->left) - getHeight(top->right) > 1) {
					if(getHeight(top->left->right) > getHeight(top->left->left)) {
						level = LR(top);
					} else {
						level = LL(top);
					}
				}
			}
			temp = top;
			level->height = getHeight(level);
		}
		root = level;
	}
	return;
}

template <typename T>
TNode<T> *AVLTree<T>::find(const T& key)
{
	TNode<T> *ret = nullptr, *work = root;
	while(work) {
		if(work->key == key) {
			ret = work;
			break;
		} else if(work->key > key) {
			work = work->left;
		} else {
			work = work->right;
		}
	}
	return ret;
}

template <typename T>
void AVLTree<T>::erase(const T& key)
{
	TNode<T> *work = find(key);
	if(work != nullptr) erase(work);
}


// just for debug

#ifdef AVL_DEBUG
template <typename T>
void AVLTree<T>::print(TNode<T> *cur)
{
	if(cur) {
		print(cur->left);
		cout << "Key = " << cur->key << ", Height = " << cur->height << endl;
		cout << "left is " << (cur->left ? to_string(cur->left->key) : "nullptr") << endl;
		cout << "right is " << (cur->right ? to_string(cur->right->key) : "nullptr") << endl;
		print(cur->right);
	}
}

template <typename T>
void AVLTree<T>::print()
{
	print(root);
	return;
}
#endif

#endif