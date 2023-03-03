#include "MemberDatabase.h"

MemberDatabase::MemberDatabase() {
}
MemberDatabase::~MemberDatabase() {
	// Delete all dynamically allocated personal profile
	for (int i = 0; i < prof.size(); i++) {
		delete prof.at(i);
	}
}
bool MemberDatabase::LoadDatabase(std::string filename) {
	ifstream inFile(filename);
	if (!inFile) {
		return false;
	}
	std::string line;
	int k = 0;
	// Get a line
	while (getline(inFile, line)) {
		// Get name, email, and number of attribute
		std::string name = line;
		std::string email;
		getline(inFile, email);
		std::string number;
		getline(inFile, number);
		istringstream iss(number);
		int n;
		iss >> n;

		PersonProfile* pp = new PersonProfile(name, email);
		for (int i = 0; i < n; i++) {
			// Insert a AttValPair into personal profile
			std::string attVal;
			getline(inFile, attVal);
			istringstream av(attVal);
			std::string attribute;
			std::string value;
			getline(av, attribute, ',');
			getline(av, value);
			AttValPair pair(attribute, value);
			pp->AddAttValPair(pair);

			// Map each attribute to vector of emails
			vector<std::string>* emails = attrToEmail.search(attVal);
			if (emails == nullptr) {
				vector<std::string> e;
				attrToEmail.insert(attVal, e);
				emails = attrToEmail.search(attVal);
				emails->push_back(email);
			}
			else {
				emails->push_back(email);
			}
		}
		// Ignore a empty line
		getline(inFile, line);

		// Store the pointer of each personal profile into a vector
		// Store the email as key and the index of personal profile poiner vector as value
		prof.push_back(pp);
		emailToProfPtr.insert(email, k);
		k++;
	}
	return true;
}
std::vector<std::string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const {
	// Return empty vector if no matching members
	// Return vector of strings if there are matching members
	std::string str = input.attribute + "," + input.value;
	vector<std::string> matches;
	if (attrToEmail.search(str) != nullptr) {
		matches = *(attrToEmail.search(str));
	}
	return matches;
}
const PersonProfile* MemberDatabase::GetMemberByEmail(std::string email) const {
	// RadixTree email-->index
	// vector index-->personal profile pointer
	int* n = emailToProfPtr.search(email);
	if (n == nullptr) {
		return nullptr;
	}
	PersonProfile* p = prof.at(*n);
	return p;
}