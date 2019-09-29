#include <string>
#include <vector>
#include <iostream>
#include <istream>

#include "provided.h"
using namespace std;


class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
	string m_name;
	string DNAseq;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
	DNAseq = "";
	m_name = nm;
	for (int i = 0; i < sequence.size(); i++) {
		DNAseq += toupper(sequence[i]);
	}
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
	genomes.clear();

	string line;
	string DNA;
	string name;

	bool nameLast = false;

	if (!getline(genomeSource, line)) {
		return false;
	}
	if (line[0] != '>') {
		return false;
	}
	name = line;
	name.erase(0,1);
	nameLast = true;
	while (getline(genomeSource, line)) {		//continue getting line after line until it no longer can't
		if (line[0] == '>') {					//if it encounters a name make a genome with the last chunk of DNA and the current name then reset name and DNA
			if (nameLast) {
				return false;
			}
			Genome newGenome(name, DNA);
			genomes.push_back(newGenome);
			name = line;
			name.erase(0,1);
			nameLast = true;
			DNA = "";
			line = "";
		}
		else {
			for (int i = 0; i < line.size(); i++) {
				if (toupper(line[i]) != 'A' &&
					toupper(line[i]) != 'C' &&
					toupper(line[i]) != 'T' &&
					toupper(line[i]) != 'G' &&
					toupper(line[i]) != 'N'  ) {
					return false;
				}
			}
			DNA += line;
			nameLast = false;
			line = "";
		}
	}
	Genome newGenome(name, DNA);
	genomes.push_back(newGenome);
	return true;
}

int GenomeImpl::length() const
{
    return DNAseq.size();  // This compiles, but may not be correct
}

string GenomeImpl::name() const
{
    return m_name;  // This compiles, but may not be correct
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
	if (position < 0) {
		return false;
	}
	if ((position + length) >= DNAseq.size()) {
		return false;
	}
	fragment = DNAseq.substr(position, length);
	return true;

}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.


Genome::Genome(const string& nm, const string& sequence)
{
	m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
	delete m_impl;
}

Genome::Genome(const Genome& other)
{
	m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
	GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
	delete m_impl;
	m_impl = newImpl;
	return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes)
{
	return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
	return m_impl->length();
}

string Genome::name() const
{
	return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
	return m_impl->extract(position, length, fragment);
}
