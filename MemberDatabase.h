#ifndef MEMBERDATABASE_INCLUDED
#define MEMBERDATABASE_INCLUDED

#include "PersonProfile.h"
#include "provided.h"
#include <iostream>
#include <sstream>
#include <fstream>

class MemberDatabase {
public:
	MemberDatabase();
	~MemberDatabase();
	bool LoadDatabase(std::string filename);
	std::vector<std::string> FindMatchingMembers(const AttValPair& input) const;
	const PersonProfile* GetMemberByEmail(std::string email) const;
private:
	RadixTree<vector<std::string>> attrToEmail;
	RadixTree<int> emailToProfPtr;
	vector<PersonProfile*> prof;
};

#endif