#include "PersonProfile.h"

PersonProfile::PersonProfile(std::string name, std::string email) {
	n = name;
	e = email;
	numAtt = 0;
}
PersonProfile::~PersonProfile() {
}
std::string PersonProfile::GetName() const {
	return n;
}
std::string PersonProfile::GetEmail() const {
	return e;
}
void PersonProfile::AddAttValPair(const AttValPair& attval) {
	pair.insert(attval.value, attval.attribute);
	val.push_back(attval.value);
	numAtt++;
}
int PersonProfile::GetNumAttValPairs() const {
	return numAtt;
}
bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attval) const {
	// Take a value from vector val and find its corresponding attribute from RadixTree pair
	if (attribute_num >= 0 && attribute_num < GetNumAttValPairs()) {
		std::string value = val.at(attribute_num);
		attval.value = value;
		attval.attribute = *(pair.search(value));
		return true;
	}
	return false;
}