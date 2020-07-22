#include "Node.h"

Node::Node()
{

}

int Node::getData()
{
	return data;
}

void Node::setData(int _data)
{
	data = _data;
}

Node* Node::getNext()
{
	return next;
	
}

void Node::setNext(Node* _next)
{
	next = _next;
}

Node* Node::getPrev()
{
	return prev;
}

void Node::setPrev(Node* _prev)
{
	prev = _prev;
}