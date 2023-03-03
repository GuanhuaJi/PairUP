#include "AttributeTranslator.h"

AttributeTranslator::AttributeTranslator() {
	// Do nothing
}
AttributeTranslator::~AttributeTranslator() {
	// Do nothing
}
bool AttributeTranslator::Load(std::string filename) {
	ifstream inFile(filename);
	// Return false is cannot open the file
	if (!inFile) {
		return false;
	}
	std::string line;
	int i = 0;
	// Take a line
	while (getline(inFile, line)) {
		if (line != "") {
			// Seperate this single line into four components
			istringstream iss(line);
			std::string sourceA;
			std::string sourceV;
			std::string compA;
			std::string compV;
			getline(iss, sourceA, ',');
			getline(iss, sourceV, ',');
			getline(iss, compA, ',');
			getline(iss, compV);
			string source = sourceA + "," + sourceV;
			AttValPair comp(compA, compV);

			// IF the source pair is NOT in the RadixTree, insert the source pair as key and a vector of AttValPair as value
			// IF the source pair is already in the RadixTree, add another compatible pair into the vector
			vector<AttValPair>* compatible = sourcecompPair.search(source);
			if (compatible == nullptr) {
				vector<AttValPair> compPair;
				sourcecompPair.insert(source, compPair);
				compatible = sourcecompPair.search(source);
				compatible->push_back(comp);
			}
			else {
				bool unique = true;
				for (int i = 0; i < compatible->size(); i++) {
					if (compatible->at(i) == comp) {
						unique = false;
					}
				}
				if (unique) {
					compatible->push_back(comp);
				}
			}
		}
	}
	return true;
}
std::vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const {
	// Return empty vector if no matches
	// Return vector of AttValPairs if there are compatibles
	std::string str = source.attribute + "," + source.value;
	vector<AttValPair> att;
	if (sourcecompPair.search(str) == nullptr) {
		return att;
	}
	att = *(sourcecompPair.search(str));
	return att;
}