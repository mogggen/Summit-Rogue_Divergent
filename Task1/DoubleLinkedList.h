#pragma once
#include "Node.h"
#include <iostream>

class DoubleLinkedList
{
	int size;
	Node* head;
	Node* tail;
public:
	DoubleLinkedList();
	bool add(Node *_data, int pos);
	bool remove(int pos);
	bool replace(Node* _old, Node* _new);
	int search(Node* _data);
	Node* nodeAt(int pos);
	void display_forward();
	void display_backward();
	void setSize(int size);
	int getSize();
};