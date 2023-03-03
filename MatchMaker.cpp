#include "MatchMaker.h"
#include "AttributeTranslator.h"
#include "MemberDatabase.h"
#include "provided.h"
#include "PersonProfile.h"
#include <vector>
#include <set>

MatchMaker::MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at) {
	m = &mdb;
	a = &at;
}
MatchMaker::~MatchMaker() {
}

std::vector<EmailCount> MatchMaker::IdentifyRankedMatches(std::string email, int threshold) const {
	set<std::string> compPairS;
	vector<EmailCount> ec;
	vector<EmailCount> emailCount;
	set<std::string> distinctEmails;
	RadixTree<int> matchingEmails;

	// Find all the compatible attribute-value pair
	// And put them into a set so that every pair is different
	const PersonProfile* pp = m->GetMemberByEmail(email);
	for (int i = 0; i < pp->GetNumAttValPairs(); i++) {
		AttValPair att;
		pp->GetAttVal(i, att);
		vector<AttValPair> pair = a->FindCompatibleAttValPairs(att);
		for (int j = 0; j < pair.size(); j++) {
			std::string comp = pair.at(j).attribute + "," + pair.at(j).value;
			compPairS.insert(comp);
		}
	}

	// Go through each compatible value pair
	set<std::string>::iterator i;
	for (i = compPairS.begin(); i != compPairS.end(); i++) {
		// Find matching members for each compatible value pair
		std::string comp = *i;
		istringstream iss(comp);
		std::string attribute;
		std::string value;
		getline(iss, attribute, ',');
		getline(iss, value);
		AttValPair att(attribute, value);
		vector<std::string> emails = m->FindMatchingMembers(att);
		// Put email and number of repetition into a RadixTree
		// Key: email     Value: number of repetition
		for (int i = 0; i < emails.size(); i++) {
			int* n = matchingEmails.search(emails.at(i));
			if (n == nullptr) {
				matchingEmails.insert(emails.at(i), 1);
			}
			else {
				(*n)++;
			}
			distinctEmails.insert(emails.at(i));
		}
	}

	// Go through each emails and generate EmailCount for each email
	for (i = distinctEmails.begin(); i != distinctEmails.end(); i++) {
		int count = *(matchingEmails.search(*i));
		EmailCount ecount(*i, count);
		ec.push_back(ecount);
	}

	// Rearrange the vector and eliminate those emails that has number of compatible value pair less than the threshold
	for (int i = 0; i < ec.size(); i++) {
		if (ec.at(i).count >= threshold) {
			vector<EmailCount>::iterator it = emailCount.begin();
			for (; it != emailCount.end(); it++) {
				if ((*it).count < ec.at(i).count) {
					it = emailCount.insert(it, ec.at(i));
					break;
				}
				else if ((*it).count == ec.at(i).count && (*it).email > ec.at(i).email) {
					it = emailCount.insert(it, ec.at(i));
					break;
				}
			}
			if (it == emailCount.end()) {
				emailCount.push_back(ec.at(i));
			}
		}
	}
	return emailCount;
}

