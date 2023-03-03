#ifndef ATTRIBUTETRANSLATOR_INCLUDED
#define ATTRIBUTETRANSLATOR_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include "RadixTree.h"
#include "provided.h"
using namespace std;

class AttributeTranslator {
public:
	AttributeTranslator();
	~AttributeTranslator();
	bool Load(std::string filename);
	std::vector<AttValPair> FindCompatibleAttValPairs(const AttValPair& source) const;
private:
	RadixTree<vector<AttValPair>> sourcecompPair;
};

#endif
