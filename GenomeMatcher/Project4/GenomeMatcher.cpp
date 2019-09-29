#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <list>

#include "Trie.h"
#include "provided.h"

using namespace std;


class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
	
	int minSearchL;
	vector<Genome> library;
	Trie<vector<int>> GenFrags; 
	
	static bool compareGMatch(GenomeMatch first, GenomeMatch second) {
		if (first.percentMatch > second.percentMatch) {
			return true;
		}
		if (first.percentMatch < second.percentMatch) {
			return false;
		}
		if (first.percentMatch == second.percentMatch) {
			if (first.genomeName > second.genomeName) {
				return false;
			}
			else {
				return true;
			}
		}
		return false;
	}
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
	minSearchL = minSearchLength;
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
	
	library.push_back(genome);
	int GenomeNumber = library.size();
	int i = 0;
	string fragment;
	vector<int> vec;
	while (genome.extract(i, minSearchL, fragment)) {
		vec.push_back(GenomeNumber);
		vec.push_back(i);
		GenFrags.insert(fragment, vec);
		vec.clear();
		fragment = "";
		i++;
	}
}

int GenomeMatcherImpl::minimumSearchLength() const
{
	return minSearchL;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	if (fragment.size() < minimumLength || minimumLength < minSearchL) {	//return false if neccesary conditions are not met
		cerr << "neccesary condtions not met, returning false" << endl;
		return false;
	}
	
	matches.clear();
	string search = fragment.substr(0, minimumLength);
	
	vector<vector<int>> trieMatchVals = GenFrags.find(search, exactMatchOnly);	//get all matching fragments from trie 

	for (int i = 0; i < trieMatchVals.size(); i++) {
		bool isMatch = true;
		string extracted;
		bool failedOnce = exactMatchOnly;

		library[(trieMatchVals[i])[0] - 1].extract(((trieMatchVals[i])[1]),fragment.size(), extracted);
		int j = 0;
		while (j < extracted.size()) {
			if (extracted[j] == fragment[j]) {
				j++;
				continue;
			}
			else if(failedOnce){
				if (j < minimumLength) {
					isMatch = false;
				}
				break;
			}
			else {
				failedOnce = true;
			}
			j++;
		}
		if (isMatch) {
			DNAMatch newDNA;
			newDNA.length = j;
			newDNA.genomeName = library[(trieMatchVals[i])[0] - 1].name();	//genome position in library is first int in arr stored in trie
			newDNA.position = (trieMatchVals[i])[1];					//position in genome is second int in arr stored in trie

			matches.push_back(newDNA);
		}
	}

	for (int i = 0; i < matches.size(); i++) {			//this function removes DNAmatches with the same genome name based on length and position
		for(int j = 0; j < matches.size(); j++) {
			if (j == i) {
				continue;
			}
			if (matches[i].genomeName == matches[j].genomeName) {
				if (matches[i].length > matches[j].length) {
					matches.erase(matches.begin() + j);
					j--;
					continue;
				}
				else if (matches[i].length < matches[j].length) {
					matches.erase(matches.begin() + i);
					i--;
					break;
				}
				else {
					if (matches[i].position < matches[j].position) {
						matches.erase(matches.begin() + j);
						j--;
						continue;
					}
					else {
						matches.erase(matches.begin() + i);
						i--;
						break;
					}
				}
			}
		}
	}
	if (matches.empty()) {
		return false;
	}
	else {
		return true;
	}
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	if (fragmentMatchLength < minSearchL) {
		return false;
	}

	map<string, int> matchCounts;
	
	int i = 0;
	double S = 0;
	
	string extracted;
	while (query.extract(i, fragmentMatchLength, extracted)) {
		vector<DNAMatch> matches;
		if (findGenomesWithThisDNA(extracted, fragmentMatchLength, exactMatchOnly, matches)) {
			for (int j = 0; j < matches.size(); j++) {
				matchCounts[matches[j].genomeName] += 1;
			}
		}
		i += fragmentMatchLength;
		S++;
	}

	for (map<string, int>::iterator it = matchCounts.begin(); it != matchCounts.end(); it++) {
		double p = (it->second / S) * 100;
		if (p > matchPercentThreshold) {
			GenomeMatch newMatch;
			newMatch.genomeName = it->first;
			newMatch.percentMatch = p;
			results.push_back(newMatch);
		}
	}

	sort(results.begin(), results.end(), compareGMatch);


	if (results.empty()) {
		return false;
	}
	return true;
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}