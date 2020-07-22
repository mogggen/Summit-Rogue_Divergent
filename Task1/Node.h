#pragma once
#include <iostream>

class Node
{
	int data;
	Node* next = nullptr;
	Node* prev = nullptr;
public:
	Node();
	int getData();
	void setData(int _data);
	Node* getNext();
	void setNext(Node* _next);
	Node* getPrev();
	void setPrev(Node* _prev);
};