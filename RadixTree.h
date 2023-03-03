#ifndef RADIXTREE_INCLUDED
#define RADIXTREE_INCLUDED

#include<string>
#include<iostream>
#include<cassert>
#include<vector>
using namespace std;

template <typename ValueType>
class RadixTree {
public:
	RadixTree();
	~RadixTree();
	void insert(std::string key, const ValueType& value);
	ValueType* search(std::string key) const;
private:
	struct Edge;
	// Each node will contain a array of 128 pointer to edge, a boolean value that marks whether it is the end of a string, and a ValueType that could be used to store key
	struct Node {
		Node();
		Node(ValueType value) {
			for (int i = 0; i < 128; i++) {
				e[i] = nullptr;
			}
			end = true;
			val = value;
		}
		~Node() {
			for (int i = 0; i < 128; i++) {
				if (e[i] != nullptr) {
					delete e[i];
				}
			}
		}
		bool isEmpty() {
			for (int i = 0; i < 128; i++) {
				if (e[i] != nullptr) {
					return false;
				}
			}
			return true;
		}
		Edge* e[128];
		bool end;
		ValueType val;
	};
	// Each edge will contains a string, and a pointer to the next node
	struct Edge {
		Edge() {
		}
		Edge(std::string str) {
			s = str;
		}
		std::string s;
		Node* nextNode;
	};
	// This function will return the first index where two strings have different characters
	// For example
	// s1: aaabbb
	// s2: aaappb
	// This function will return 3 since index 3 is the first place where two characters are different
	int sameChar(std::string s1, std::string s2) const {
		int n;
		if (s1.size() < s2.size()) {
			n = s1.size();
		}
		else {
			n = s2.size();
		}
		for (int i = 0; i < n; i++) {
			if (s1.at(i) != s2.at(i)) {
				return i;
			}
		}
		return n;
	}
	// This function will return a string that cuts off the first n characters
	// For example
	// str: 123456789
	// n  : 3
	// return: 456789
	std::string cut(std::string str, int n) const {
		std::string s;
		for (int i = n; i < str.size(); i++) {
			s += str.at(i);
		}
		return s;
	}
	// This function will return a string that is common for both strings until any different characters occur
	// For example
	// s1: aaabbb
	// s2: aaaddb
	// return: aaa
	std::string commonStr(std::string s1, std::string s2) const {
		string str;
		int n = sameChar(s1, s2);
		for (int i = 0; i < n; i++) {
			str += s1.at(i);
		}
		return str;
	}
	// This function will determine whether the the program should keep tracing down the radix tree or should the program stop
	bool stopTracing(std::string key, std::string edge) const {
		if (key.size() < edge.size()) {
			return true;
		}
		for (int i = 0; i < edge.size(); i++) {
			if (key.at(i) != edge.at(i)) {
				return true;
			}
		}
		return false;
	}
	// This function will delete all dynamically created nodes and edges
	void deleteAll(Node* p) {
		vector<Node*> np;
		for (int i = 0; i < 128; i++) {
			if (p->e[i] != nullptr) {
				np.push_back(p->e[i]->nextNode);
			}
		}
		delete p;
		for (int i = 0; i < np.size(); i++) {
			deleteAll(np.at(i));
		}
	}
	Node* root;
};

// construct a node with a array of 128 nullptrs and end marker is false
template <typename ValueType>
RadixTree<ValueType>::Node::Node() {
	for (int i = 0; i < 128; i++) {
		e[i] = nullptr;
	}
	end = false;
}

// construct a RadixTree by pointing root to a new empty Node
template <typename ValueType>
RadixTree<ValueType>::RadixTree() {
	root = new Node();
}

// destruct a RadixTree by calling deleteAll function
template <typename ValueType>
RadixTree<ValueType>::~RadixTree() {
	deleteAll(root);
}


template <typename ValueType>
void RadixTree<ValueType>::insert(std::string key, const ValueType& value) {
	// Step 1: trace down the RadixTree until we reach somewhere to break apart or stop
	// Everytime we trace down a path, we update our key by cutting off the common part
	// For example, if the tree has a path that looks like this
	// wa - t - er
	// and insert key is
	// watch
	// After the first step, the new key will be "ch" ("wat" is cut off), and the node pointer will land on the node which has a pointer that points to the edge "er"
	Node* p = root;
	bool keepTracing = true;
	while (keepTracing && key.size() > 0) {
		int n = (int)key.at(0);
		if (p->e[n] == nullptr || stopTracing(key, p->e[n]->s)) {
			keepTracing = false;
		}
		else {
			key = cut(key, sameChar(key, p->e[n]->s));
			p = p->e[n]->nextNode;
		}
	}
	// After we done the first step, we reach a node where next edge no longer fit the key, or the key is depleted, or the there is no next edge.

	// If the remaining key is empty, that means we should assign value to the current node
	// For example, if the tree has a path that looks like this
	// wa - t - er - melon
	// and insert key is
	// water
	// Then the remaining key after we trace down the tree will be empty
	if (key.size() == 0) {
		p->val = value;
		p->end = true;
	}
	else {
		int n = (int)key.at(0);
		// If there is no next corresponding edges, then we create a new Edge, which points to a new Node that marks the end with value assigned
		// For example, if the tree has a path that looks like this
		// wa - t - er
		// and the insert key is
		// watermalon
		// Then the remaining key after we trace down the tree will be "melon"
		if (p->e[n] == nullptr) {
			p->e[n] = new Edge();
			p->e[n]->s = key;
			p->e[n]->nextNode = new Node(value);
		}
		// If the next edge does not completely fit the remaining key, then we should leave the common part and break up the different part
		// For, example, if the tree has a path that looks like this
		// wa - ter - melon
		// and the insert key is
		// watch
		// Then the remaining key after we trace down the tree will be "tch", and "ter" does not really fit
		else {
			//find the common part
			std::string comStr = commonStr(key, p->e[n]->s);
			Node* tempN = p->e[n]->nextNode;
			string tempS = p->e[n]->s;
			string leftOver = cut(tempS, sameChar(key, tempS));
			key = cut(key, sameChar(key, tempS));
			//break down the remaining part
			int nextn = (int)leftOver.at(0);
			delete p->e[n];
			p->e[n] = new Edge(comStr);
			p->e[n]->nextNode = new Node();
			p->e[n]->nextNode->e[nextn] = new Edge(leftOver);
			p->e[n]->nextNode->e[nextn]->nextNode = tempN;
			if (key == "") {
				p->e[n]->nextNode->val = value;
				p->e[n]->nextNode->end = true;
			}
			else {
				int keyn = (int)key.at(0);
				p->e[n]->nextNode->e[keyn] = new Edge(key);
				p->e[n]->nextNode->e[keyn]->nextNode = new Node(value);
			}
		}
	}
}

template <typename ValueType>
ValueType* RadixTree<ValueType>::search(std::string key) const {
	// Trace down the tree until we reach a break point
	Node* p = root;
	bool keepTracing = true;
	while (keepTracing && key.size() > 0) {
		int n = (int)key.at(0);
		if (p->e[n] == nullptr || stopTracing(key, p->e[n]->s)) {
			keepTracing = false;
		}
		else {
			key = cut(key, sameChar(key, p->e[n]->s));
			p = p->e[n]->nextNode;
		}
	}
	// If the remaining key is empty and the current node marks the end, then we return a pointer ot the value
	if (key == "" && p->end) {
		return &(p->val);
	}
	// Otherwise, return nullptr
	return nullptr;
}

#endif