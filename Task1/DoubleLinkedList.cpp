#include "DoubleLinkedList.h"

DoubleLinkedList::DoubleLinkedList()
{
	head = nullptr;
	tail = nullptr;
}

bool DoubleLinkedList::add(Node* data, int pos)
{
	//index out of range
	if (pos > getSize() || pos < 0) return 0;

	if (getSize() == 0)
	{
		head = data;
		tail = data;
	}
	else
	{
		if (pos == 0) head = data;
		if (pos == getSize()) tail = data;

	data->setPrev(nodeAt(pos - 1));
	data->setNext(nodeAt(pos));
	nodeAt(pos - 1)->setNext(data);
	if (nodeAt(pos) != nullptr)
		nodeAt(pos)->setPrev(data);
	}
	setSize(getSize() + 1);
	return 1;
}

bool DoubleLinkedList::remove(int pos)
{
	//index out of range
	if (pos > getSize() || pos < 0) return 0;

	Node* temp = nodeAt(pos);
	if (pos == 0)
	{
		head = nodeAt(pos + 1);
		nodeAt(pos + 1)->setPrev(nullptr);
	}
	else if (pos == getSize() - 1)
	{
		tail = nodeAt(pos - 1);
		nodeAt(pos - 1)->setNext(nullptr);
	}
	else
	{
		nodeAt(pos + 1)->setPrev(nodeAt(pos - 1));
		nodeAt(pos - 1)->setNext(nodeAt(pos + 1));
	}
	delete temp;
	setSize(getSize() - 1);
	return 1;
}

bool DoubleLinkedList::replace(Node* data, Node* _new)
{
	Node* current = head;
	int i;
	for (i = 0; i < getSize(); i++)
	{
		if (current != data)
		{
			current = current->getNext();
		}
		else break;
	}
	//could not find data in list
	if (current == nullptr) return 0;

	//no objects to replace
	if (getSize() == 0)
	{
		return 0;
	}
	else if (getSize() == 1)
	{
		delete data;
		head = _new;
		tail = _new;
	}
	else
	{
		if (search(data) == 0)
		{
			nodeAt(search(data) + 1)->setPrev(_new);
			_new->setNext(data->getNext());
			head = _new;
			delete data;
		}
		else if (search(data) == getSize() - 1)
		{
			nodeAt(search(data) - 1)->setNext(_new);
			_new->setPrev(data->getPrev());
			tail = _new;
			delete data;
		}
		else
		{
			_new->setPrev(data->getPrev());
			_new->setNext(data->getNext());
			nodeAt(search(data) + 1)->setPrev(_new);
			nodeAt(search(data) - 1)->setNext(_new);
			delete data;
		}
	}
	return 1;
}

int DoubleLinkedList::search(Node* _data)
{
	int i = 0;
	Node* current = head;
	while(current !=  _data)
	{
		if (current != nullptr)
		{
			current = current->getNext();
			i++;
		}
		else
			return -1;
	}
	return i;
}

Node* DoubleLinkedList::nodeAt(int pos)
{
	if (pos >= getSize() || pos < 0) return nullptr;
	int i = 0;
	Node* current = head;
	while (i < pos)
	{
		current = current->getNext();
		i++;
	}
	return current;
}

void DoubleLinkedList::display_forward()
{
	Node* current = head;
	while (current != nullptr)
	{
		std::cerr << current->getData() << std::endl;
		current = current->getNext();
	}
}

void DoubleLinkedList::display_backward()
{
	Node* current = tail;
	while (current != nullptr)
	{
		std::cerr << current->getData() << std::endl;
		current = current->getPrev();
	}
}

void DoubleLinkedList::setSize(int _size)
{
	size = _size;
}

int DoubleLinkedList::getSize()
{
	return size;
}