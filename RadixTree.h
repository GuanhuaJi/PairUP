#include<string>
#include<iostream>
#include<cassert>
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
	struct Node {
		Node();
		Node(ValueType value) {
			for (int i = 0; i < 128; i++) {
				e[i] = nullptr;
			}
			end = true;
			val = value;
		}
		Edge* e[128];
		bool end;
		ValueType val;
	};
	struct Edge {
		Edge() {
		}
		Edge(std::string str) {
			s = str;
		}
		std::string s;
		Node* nextNode;
	};
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
	std::string cut(std::string str, int n) const {
		std::string s;
		for (int i = n; i < str.size(); i++) {
			s += str.at(i);
		}
		return s;
	}
	std::string commonStr(std::string s1, std::string s2) const {
		string str;
		int n = sameChar(s1, s2);
		for (int i = 0; i < n; i++) {
			str += s1.at(i);
		}
		return str;
	}
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
	Node* root;
};

template <typename ValueType>
RadixTree<ValueType>::Node::Node() {
	for (int i = 0; i < 128; i++) {
		e[i] = nullptr;
	}
	end = false;
}

template <typename ValueType>
RadixTree<ValueType>::RadixTree() {
	root = new Node();
}

template <typename ValueType>
RadixTree<ValueType>::~RadixTree() {

}


template <typename ValueType>
void RadixTree<ValueType>::insert(std::string key, const ValueType& value) {
	// step 1: trace until different
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
	// step 2: decompose or add on
	int n = (int)key.at(0);
	if (p->e[n] == nullptr) {
		p->e[n] = new Edge();
		p->e[n]->s = key;
		p->e[n]->nextNode = new Node(value);
	}
	else {
		//find common string
		std::string comStr = commonStr(key, p->e[n]->s);
		Node* tempN = p->e[n]->nextNode;
		string tempS = p->e[n]->s;
		string leftOver = cut(tempS, sameChar(key, tempS));
		key = cut(key, sameChar(key, tempS));
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

template <typename ValueType>
ValueType* RadixTree<ValueType>::search(std::string key) const {
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
	if (key == "" && p->end) {
		return &(p->val);
	}
	return nullptr;
}