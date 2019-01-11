
#include "BinaryTree.h"
#include <iostream>
template <class T>
void print(T data)
{
	std::cout << data << " ";
}

int main()
{
	BinaryTree<int> bt{};
	/*for (int i = 0; i < 7150; ++i)
	{
		bt.add(i);
	}*/

	bt.add(10);
	bt.add(30);
	bt.add(0);
	bt.add(5);

	bt.levelOrderTraversal(&print);
	std::cout << "bt.size(): " << bt.getSize() << std::endl;
	return 0;
}