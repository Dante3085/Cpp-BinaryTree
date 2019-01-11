#include <iostream>
#include <random>
#include <exception>
#include <map>

template <class T>
class LinkedList
{
public:
	LinkedList() : first{nullptr}, last{nullptr}, size{0} {}
	virtual ~LinkedList() 
	{
		int SIZE = size;
		for (int i = 0; i < SIZE; ++i)
			remove(0);
	}

	void append(T data)
	{
		Link<T>* newL = new Link<T>{data, nullptr};
		if (first == nullptr)
			first = last = newL;
		else
		{
			last->next = newL;
			last = newL;
		}
		++size;
	}

	T remove(int index)
	{
		// INVALID INDEX
		if (index < 0 || index >= size)
			throw std::invalid_argument( "index < 0 || index >= size" );

		Link<T>* ptr;
		T tempData;

		// REMOVE FIRST
		if (index == 0)
		{
			ptr = first->next;
			tempData = first->data;
			delete first;
			first = ptr;
		}

		// REMOVE ANYWHERE
		else
		{
			ptr = first;
			int count = 0;
			while(count++ != (index - 1))
				ptr = ptr->next;

			Link<T>* temp = ptr->next->next;
			tempData = ptr->next->data;
			delete ptr->next;
			ptr->next = temp;
		}
		--size;
		return tempData;
	}

	void traverse(void(*work)(T data))
	{
		Link<T>* ptr = first;
		while(ptr != nullptr)
		{
			work(ptr->data);
			ptr = ptr->next;
		}
	}

	int getSize()
	{
		return size;
	}

	bool isEmpty()
	{
		return size == 0;
	}

private:

	// NOTE(moritz): Komische Kompilierungsfehler treten auf,
	// wenn "template <class T>" fehlt und das struct nach
	// den beiden Link Membern steht.
	template <class T>
	struct Link
	{
		Link(T data, Link<T>* next) : data{data}, next{next} {}
		~Link() {}

		T data;
		Link<T>* next;
	};

	Link<T>* first;
	Link<T>* last;
	int size;
};

template <class T>
class Queue : public LinkedList<T>
{
public:
	Queue() : LinkedList{} {}
	~Queue() {}

	void enqueue(T data)
	{
		append(data);
	}

	T dequeue()
	{
		return remove(0);
	}
};

template <class T>
class Node
{
public:
	Node(T data, int depth) : data{data}, depth{depth} leftChild{nullptr}, rightChild{nullptr} {}
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
	int depth;
	Node<T>* leftChild;
	Node<T>* rightChild;
};

template <class T>
class BinaryTree
{
public:
	BinaryTree() : root{nullptr}, count{0} {}
	~BinaryTree()
	{
		deleteRecursive(root);
	}

	int size()
	{
		return count;
	}

	bool isEmpty()
	{
		return root == nullptr;
	}

	void add(T data)
	{
		root = root == nullptr ? new Node<T>{data, 0} : addRecursive(root, data);
		++count;
	}

	bool contains(T data)
	{
		return findRecursive(root, data);
	}

	void remove(T data)
	{		
		root = removeRecursive(root, data);
		if (count > 0)
			--count;
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
	int count;

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

template <class T>
void print(T data)
{
	std::cout << data;
}

int main()
{
	BinaryTree<int> bt{};
	bt.add(5);
	bt.add(2);
	bt.add(7);
	bt.add(1);
	bt.add(3);

	bt.levelOrderTraversal(&print);	

	std::cout << "END_OF_PROGRAM" << std::endl;
	return 0;
}