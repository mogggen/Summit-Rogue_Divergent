/*
 * SinglyLinkedList.cpp
 *
 *  Created on: 27 Nov 2018
 *      Author: teelai
 */

#include "SinglyLinkedList.h"
#include <iostream>

using namespace std;

SinglyLinkedList::SinglyLinkedList() {
	this->head = nullptr;
}

SinglyLinkedList::~SinglyLinkedList() {
	// TODO Auto-generated destructor stub
}

void SinglyLinkedList::addFirst(Node *data) {

	if (head != nullptr) {
		data->setNext(head);
	}
	head = data;

}
Node* SinglyLinkedList::remove(Node *data) {

	Node *current = head;
	Node *previous = head;
	while (current != nullptr) {
		if (current == data) {
			previous->setNext(current->getNext());
			return current;
		}
		previous = current;
		current = current->getNext();
	}
	return nullptr;
}

void SinglyLinkedList::display() {
	Node *current = head;
	while (current != nullptr) {
		cout << current->getData() << " ";
		current = current->getNext();
	}
	cout << endl;
}

Node* SinglyLinkedList::search(Node *data) {
	Node *current = head;
	while (current != nullptr) {
		if (current == data) {
			return current;
		}
		current = current->getNext();
	}
	return nullptr;
}

int Node::getData() {
	return data;
}
void Node::setData(int data) {
	this->data = data;
}
Node* Node::getNext() {
	return next;
}
void Node::setNext(Node *next) {
	this->next = next;
}

Node::Node(int d) {
	this->data = d;
	this->next = nullptr;
}

Node::~Node() {
}

int main() {

	SinglyLinkedList list;
	Node *n1 = new Node(1);
	Node *n2 = new Node(2);
	Node *n3 = new Node(3);
	Node *n4 = new Node(4);
	list.addFirst(n1);
	list.addFirst(n2);
	list.addFirst(n3);
	list.addFirst(n4);
	list.display();

	Node *n = list.remove(n2);
	cout << n->getData() << endl;
	delete n;

	n = list.search(n3);
	cout << n->getData() << endl;

}

