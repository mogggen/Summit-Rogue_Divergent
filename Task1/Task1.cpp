#include "DoubleLinkedList.h"
#include <iostream>


int main()
{
	DoubleLinkedList dll;
	Node* Node1 = new Node();
	Node* Node2 = new Node();
	Node* Node3 = new Node();
	Node* Node4 = new Node();

	Node1->setData(5);
	Node2->setData(3);
	Node3->setData(33);
	Node4->setData(35);

	dll.add(Node1, 0);
	dll.add(Node2, 1);
	dll.add(Node3, 2);
	dll.add(Node4, 3);

	Node* Node5 = new Node();
	dll.replace(Node2, Node5);
	Node5->setData(44);

	dll.display_forward();
	dll.remove(1);
	std::cout << std::endl;
	dll.display_forward();
	dll.remove(2);
	std::cout << std::endl;

	dll.display_forward();
	std::cout << std::endl;
	dll.display_backward();
	std::cout << std::endl;

	std::cout << dll.search(Node3);
}