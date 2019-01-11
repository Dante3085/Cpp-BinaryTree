#include <exception>
#include "LinkedList.h"

template <class T>
class Node
{
public:
	Node(T data) : data{data}, leftChild{nullptr}, rightChild{nullptr} {}
	~Node(){}

	T getData() const
	{
		return data;
	}

	
	void setData(T data)
	{
		this->data = data;
	}

	Node<T>* getLeftChild() const
	{
		return leftChild;
	}

	void setLeftChild(Node<T>* leftChild)
	{
		this->leftChild = leftChild;
	}

	Node<T>* getRightChild() const
	{
		return rightChild;
	}

	void setRightChild(Node<T>* rightChild)
	{
		this->rightChild = rightChild;
	}

	int getDepth()
	{
		return depth;
	}

	void setDepth(int depth)
	{
		this->depth = depth;
	}

private:
	T data;
	Node<T>* leftChild;
	Node<T>* rightChild;
};

template <class T>
class BinaryTree
{
public:
	BinaryTree() : root{nullptr}, size{0} {}
	~BinaryTree()
	{
		deleteRecursive(root);
	}

	int getSize()
	{
		return size;
	}

	bool isEmpty()
	{
		return root == nullptr;
	}

	void add(T data)
	{
		root = root == nullptr ? new Node<T>{data} : addRecursive(root, data);
		++size;
	}

	bool contains(T data)
	{
		return findRecursive(root, data);
	}

	void remove(T data)
	{		
		root = removeRecursive(root, data);
		if (size > 0)
			--size;
	}

	void traversePreOrder(void(*work)(T data))
	{
		traversePreOrderRecursive(root, work);
	}

	void traverseInOrder(void(*work)(T data))
	{
		traverseInOrderRecursive(root, work);
	}

	void traversePostOrder(void(*work)(T data))
	{
		traversePostOrderRecursive(root, work);
	}

	void levelOrderTraversal(void(*work)(T data))
	{
		Queue<Node<T>*>* queue = new Queue<Node<T>*>{};
		if (root != nullptr)
			queue->enqueue(root);
		levelOrderTraversalRecursive(queue, work);
	}

private:
	Node<T>* root;
	int size;

	// NOTE(moritz): Can't figure out how to properly insert new lines so that
	// each level with it's Nodes is properly printed.
	void levelOrderTraversalRecursive(Queue<Node<T>*>* queue, void(*work)(T data))
	{
		if (queue->isEmpty())
			return;
		Node<T>* current = queue->dequeue();
		std::cout << current->getData() << " ";

		if (current->getLeftChild() != nullptr)
			queue->enqueue(current->getLeftChild());
		if (current->getRightChild() != nullptr)
			queue->enqueue(current->getRightChild());

		levelOrderTraversalRecursive(queue, work);
	}

	// DESTRUCTOR HELPER FUNCTION
	void deleteRecursive(Node<T>* current)
	{
		Node<T>* leftChild = current->getLeftChild();
		Node<T>* rightChild = current->getRightChild();

		if (leftChild != nullptr)
			deleteRecursive(leftChild);
		if (rightChild != nullptr)
			deleteRecursive(rightChild);
		
		delete current;
	}

	Node<T>* addRecursive(Node<T>* current, T data)
	{
		// CREATE NEW NODE
		if (current == nullptr)
			return new Node<T>{data};

		// GO LEFT SUBTREE
		else if (data <= current->getData())
			current->setLeftChild(addRecursive(current->getLeftChild(), data));

		// GO RIGHT SUBTREE (else if for safety)
		else if (data > current->getData())
			current->setRightChild(addRecursive(current->getRightChild(), data));
	}

	bool containsRecursive(Node<T>* current, T data)
	{
		// DIDN'T FIND DATA
		if (current == nullptr)
			return false;

		// FOUND DATA
		else if (data == current->getData())
			return true;

		// SEARCH LEFT SUBTREE
		else if (data < current->getData())
			return findRecursive(current->getLeftChild(), data);

		// SEARCH RIGHT SUBTREE
		else
			return findRecursive(current->getRightChild(), data);
	} 

	Node<T>* removeRecursive(Node<T>* current, T data)
	{
		// TODO(moritz): Think about where to return current.
		// TODO(moritz): delete vicitim node.

		// DIDN'T FIND DATA
		if (current == nullptr)
			return nullptr;

		Node<T>* leftChild = current->getLeftChild();
		Node<T>* rightChild = current->getRightChild();

		// FOUND DATA
		if (data == current->getData())
		{
			// NODE IS LEAF
			if (leftChild == nullptr && rightChild == nullptr)
				return nullptr;
			
			// NODE HAS ONE CHILD - RIGHT CHILD
			if (leftChild == nullptr)
				return rightChild;

			// NODE HAS ONE CHILD - LEFT CHILD
			if (rightChild == nullptr)
				return leftChild;
			
			// NODE HAS TWO CHILDREN
			T biggestValueLeftSubtree = findBiggestValue(leftChild);
			current->setData(biggestValueLeftSubtree);
			current->setLeftChild(removeRecursive(leftChild, biggestValueLeftSubtree));
			return current;
		}

		// SEARCH LEFT SUBTREE
		if (data < current->getData())
		{
			current->setLeftChild(removeRecursive(leftChild, data));
			return current;
		}
		
		// SEARCH RIGHT SUBTREE
		current->setRightChild(removeRecursive(rightChild, data));
		return current;
	}

	T findSmallestValue(Node<T>* current)
	{
		return current->getLeftChild() == nullptr ? current->getData() : findSmallestValue(current->getLeftChild());
	}

	T findBiggestValue(Node<T>* current)
	{
		return current->getRightChild() == nullptr ? current->getData() : findBiggestValue(current->getRightChild());
	}

	void traversePreOrderRecursive(Node<T>* current, void(*work)(T data))
	{
		if (current == nullptr)
			return;

		work(current->getData());
		traversePreOrderRecursive(current->getLeftChild(), work);
		traversePreOrderRecursive(current->getRightChild(), work);
	}

	void traverseInOrderRecursive(Node<T>* current, void(*work)(T data))
	{
		if (current == nullptr)
			return;

		traverseInOrderRecursive(current->getLeftChild(), work);
		work(current->getData());
		traverseInOrderRecursive(current->getRightChild(), work);
	}

	void traversePostOrderRecursive(Node<T>* current, void(*work)(T data))
	{
		if (current == nullptr)
			return;

		traversePostOrderRecursive(current->getLeftChild(), work);
		traversePostOrderRecursive(current->getRightChild(), work);
		work(current->getData());
	}
};