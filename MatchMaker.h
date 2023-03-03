#ifndef MATCHMAKER_INCLUDED
#define MATCHMAKER_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class MemberDatabase;
class AttributeTranslator;
struct EmailCount;
class MatchMaker {
public:
	MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at);
	~MatchMaker();
	std::vector<EmailCount> IdentifyRankedMatches(std::string email, int threshold) const;
private:
	const MemberDatabase* m;
	const AttributeTranslator* a;
};

#endif