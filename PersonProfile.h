#ifndef PERSONPROFILE_INCLUDED
#define PERSONPROFILE_INCLUDED


#include "RadixTree.h"
#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class PersonProfile {
public:
	PersonProfile(std::string name, std::string email);
	~PersonProfile();
	std::string GetName() const;
	std::string GetEmail() const;
	void AddAttValPair(const AttValPair& attval);
	int GetNumAttValPairs() const;
	bool GetAttVal(int attribute_num, AttValPair& attval) const;
private:
	std::string n;
	std::string e;
	RadixTree<std::string> pair;
	vector<std::string> val;
	int numAtt;
};



#endif