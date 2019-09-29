#include <iostream>
#include <cassert>
#include <algorithm>

#include "provided.h"
#include "Trie.h"

using namespace std;


void test1(){

	cerr << "testing trie class: insert and find" << endl;
	Trie<int> test;

	test.insert("hit", 1);
	test.insert("hit", 2);

	test.insert("hip", 10);
	test.insert("hip", 20);

	test.insert("hi", 9);
	test.insert("hi", 17);

	test.insert("hat", 7);
	test.insert("hat", 8);
	test.insert("hat", 9);

	test.insert("a", 14);

	test.insert("to", 22);
	test.insert("to", 23);

	test.insert("tap", 19);
	test.insert("tap", 6);
	test.insert("tap", 32);

	vector<int> vec1 = test.find("hit", true);
	assert(vec1.size() == 2 && find(vec1.begin(), vec1.end(), 2) != vec1.end());

	vector<int> vec2 = test.find("hip", true);
	assert(vec1.size() == 2 && find(vec2.begin(), vec2.end(), 20) != vec2.end());

	vector<int> vec3 = test.find("hit", false);
	assert(vec3.size() == 7 && find(vec3.begin(), vec3.end(), 7) != vec3.end());

	vector<int> vec4 = test.find("hip", false);
	assert(vec4.size() == 4 && find(vec4.begin(), vec4.end(), 2) != vec4.end());

	cerr << "Test 1 Passed" << endl << endl;
}
//
//void test2() {
//	cerr << "testing genome class: extract" << endl;
//	Genome g("oryx",
//		"GCTCGGNACACATCCGCCGCGGACGGGACGGGATTCGGGCTGTCGATTGTCTCACAGATCGTCGACGTACATGACTGGGA");
//	string	f1, f2, f3;
//	bool	result1 = g.extract(0, 5, f1);	 //	result1	=	true,	f1	=	“GCTCG”;
//	bool	result2 = g.extract(74, 6, f2); //	result2 =	true,	f2 =	“CTGGGA”;
//	bool	result3 = g.extract(74, 7, f3); //	result3 =	false,	f3 is	unchanged
//	assert(result1 == true && f1 == "GCTCG");
//	assert(result2 == true && f2 == "CTGGGA");
//	assert(result3 == false && f3 == "");
//
//	cerr << "Test 2 Passed" << endl << endl;
//
//}
//
//void test3() {
//	cerr << "testing genomeMatch class" << endl;
//	Genome g("oryx",
//		"GCTCGGNACACATCCGCCGCGGACGGGACGGGATTCGGGCTGTCGATTGTCTCACAGATCGTCGACGTACATGACTGGGA");
//	GenomeMatcher test(5);
//
//	test.addGenome(g);
//
//	vector<DNAMatch> vec;
//	assert(test.findGenomesWithThisDNA("GGGAT",5,true,vec) && vec.size() == 1);
//	vector<GenomeMatch> vec2;
//	assert(test.findRelatedGenomes(g, 5, true, 90, vec2) && vec2.size() == 1);
//
//	
//
//
//	cerr << "Test 3 Passed" << endl << endl;
//}
//
//int main() {
//	
//	test1();
//	test2();
//	test3();
//	cerr << "All tests succeeded" << endl;
//
//}






#if defined(_MSC_VER)  &&  !defined(_DEBUG)
#include <iostream>
#include <windows.h>
#include <conio.h>

struct KeepWindowOpenUntilDismissed
{
	~KeepWindowOpenUntilDismissed()
	{
		DWORD pids[1];
		if (GetConsoleProcessList(pids, 1) == 1)
		{
			std::cout << "Press any key to continue . . . ";
			_getch();
		}
	}
} keepWindowOpenUntilDismissed;
#endif


#include "provided.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
using namespace std;

// Change the string literal in this declaration to be the path to the
// directory that contains the genome data files we provide, e.g.,
// "Z:/CS32/Geenomics/data" or "/Users/fred/cs32/Geenomics/data"

const string PROVIDED_DIR = "../data";

const string providedFiles[] = {
	"Ferroplasma_acidarmanus.txt",
	"Halobacterium_jilantaiense.txt",
	"Halorubrum_chaoviator.txt",
	"Halorubrum_californiense.txt",
	"Halorientalis_regularis.txt",
	"Halorientalis_persicus.txt",
	"Ferroglobus_placidus.txt",
	"Desulfurococcus_mucosus.txt"
};

void createNewLibrary(GenomeMatcher*& library)
{
	cout << "Enter minimum search length (3-100): ";
	string line;
	getline(cin, line);
	int len = atoi(line.c_str());
	if (len < 3 || len > 100)
	{
		cout << "Invalid prefix size." << endl;
		return;
	}
	delete library;
	library = new GenomeMatcher(len);
}

void addOneGenomeManually(GenomeMatcher* library)
{
	cout << "Enter name: ";
	string name;
	getline(cin, name);
	if (name.empty())
	{
		cout << "Name must not be empty." << endl;
		return;
	}
	cout << "Enter DNA sequence: ";
	string sequence;
	getline(cin, sequence);
	if (sequence.empty())
	{
		cout << "Sequence must not be empty." << endl;
		return;
	}
	if (sequence.find_first_not_of("ACGTNacgtn") != string::npos)
	{
		cout << "Invalid character in DNA sequence." << endl;
		return;
	}
	for (char ch : sequence)
		ch = toupper(ch);
	library->addGenome(Genome(name, sequence));
}

bool loadFile(string filename, vector<Genome>& genomes)
{
	ifstream inputf(filename);
	if (!inputf)
	{
		cout << "Cannot open file: " << filename << endl;
		return false;
	}
	if (!Genome::load(inputf, genomes))
	{
		cout << "Improperly formatted file: " << filename << endl;
		return false;
	}
	return true;
}

void loadOneDataFile(GenomeMatcher* library)
{
	string filename;
	cout << "Enter file name: ";
	getline(cin, filename);
	if (filename.empty())
	{
		cout << "No file name entered." << endl;
		return;
	}
	vector<Genome> genomes;
	if (!loadFile(filename, genomes))
		return;
	for (const auto& g : genomes)
		library->addGenome(g);
	cout << "Successfully loaded " << genomes.size() << " genomes." << endl;
}

void loadProvidedFiles(GenomeMatcher* library)
{
	for (const string& f : providedFiles)
	{
		vector<Genome> genomes;
		if (loadFile(PROVIDED_DIR + "/" + f, genomes))
		{
			for (const auto& g : genomes)
				library->addGenome(g);
			cout << "Loaded " << genomes.size() << " genomes from " << f << endl;
		}
	}
}

void findGenome(GenomeMatcher* library, bool exactMatch)
{
	if (exactMatch)
		cout << "Enter DNA sequence for which to find exact matches: ";
	else
		cout << "Enter DNA sequence for which to find exact matches and SNiPs: ";
	string sequence;
	getline(cin, sequence);
	int minLength = library->minimumSearchLength();
	if (sequence.size() < minLength)
	{
		cout << "DNA sequence length must be at least " << minLength << endl;
		return;
	}
	cout << "Enter minimum sequence match length: ";
	string line;
	getline(cin, line);
	int minMatchLength = atoi(line.c_str());
	if (minMatchLength > sequence.size())
	{
		cout << "Minimum match length must be at least the sequence length." << endl;
		return;
	}
	vector<DNAMatch> matches;
	if (!library->findGenomesWithThisDNA(sequence, minMatchLength, exactMatch, matches))
	{
		cout << "No ";
		if (exactMatch)
			cout << " matches";
		else
			cout << " matches or SNiPs";
		cout << " of " << sequence << " were found." << endl;
		return;
	}
	cout << matches.size();
	if (exactMatch)
		cout << " matches";
	else
		cout << " matches and/or SNiPs";
	cout << " of " << sequence << " found:" << endl;
	for (const auto& m : matches)
		cout << "  length " << m.length << " position " << m.position << " in " << m.genomeName << endl;
}

bool getFindRelatedParams(double& pct, bool& exactMatchOnly)
{
	cout << "Enter match percentage threshold (0-100): ";
	string line;
	getline(cin, line);
	pct = atof(line.c_str());
	if (pct < 0 || pct > 100)
	{
		cout << "Percentage must be in the range 0 to 100." << endl;
		return false;
	}
	cout << "Require (e)xact match or allow (S)NiPs (e or s): ";
	getline(cin, line);
	if (line.empty() || (line[0] != 'e' && line[0] != 's'))
	{
		cout << "Response must be e or s." << endl;
		return false;
	}
	exactMatchOnly = (line[0] == 'e');
	return true;
}

void findRelatedGenomesManual(GenomeMatcher* library)
{
	cout << "Enter DNA sequence: ";
	string sequence;
	getline(cin, sequence);
	int minLength = library->minimumSearchLength();
	if (sequence.size() < minLength)
	{
		cout << "DNA sequence length must be at least " << minLength << endl;
		return;
	}
	double pctThreshold;
	bool exactMatchOnly;
	if (!getFindRelatedParams(pctThreshold, exactMatchOnly))
		return;

	vector<GenomeMatch> matches;
	library->findRelatedGenomes(Genome("x", sequence), 2 * minLength, exactMatchOnly, pctThreshold, matches);
	if (matches.empty())
	{
		cout << "    No related genomes were found" << endl;
		return;
	}
	cout << "    " << matches.size() << " related genomes were found:" << endl;
	cout.setf(ios::fixed);
	cout.precision(2);
	for (const auto& m : matches)
		cout << " " << setw(6) << m.percentMatch << "%  " << m.genomeName << endl;
}

void findRelatedGenomesFromFile(GenomeMatcher* library)
{
	string filename;
	cout << "Enter name of file containing one or more genomes to find matches for: ";
	getline(cin, filename);
	if (filename.empty())
	{
		cout << "No file name entered." << endl;
		return;
	}
	vector<Genome> genomes;
	if (!loadFile(filename, genomes))
		return;
	double pctThreshold;
	bool exactMatchOnly;
	if (!getFindRelatedParams(pctThreshold, exactMatchOnly))
		return;

	int minLength = library->minimumSearchLength();
	for (const auto& g : genomes)
	{
		vector<GenomeMatch> matches;
		library->findRelatedGenomes(g, 2 * minLength, exactMatchOnly, pctThreshold, matches);
		cout << "  For " << g.name() << endl;
		if (matches.empty())
		{
			cout << "    No related genomes were found" << endl;
			continue;
		}
		cout << "    " << matches.size() << " related genomes were found:" << endl;
		cout.setf(ios::fixed);
		cout.precision(2);
		for (const auto& m : matches)
			cout << "     " << setw(6) << m.percentMatch << "%  " << m.genomeName << endl;
	}
}

void showMenu()
{
	cout << "        Commands:" << endl;
	cout << "         c - create new genome library      s - find matching SNiPs" << endl;
	cout << "         a - add one genome manually        r - find related genomes (manual)" << endl;
	cout << "         l - load one data file             f - find related genomes (file)" << endl;
	cout << "         d - load all provided data files   ? - show this menu" << endl;
	cout << "         e - find matches exactly           q - quit" << endl;
}

int main()
{
	const int defaultMinSearchLength = 10;
	test1();

	cout << "Welcome to the Gee-nomics test harness!" << endl;
	cout << "The genome library is initially empty, with a default minSearchLength of " << defaultMinSearchLength << endl;
	showMenu();

	GenomeMatcher* library = new GenomeMatcher(defaultMinSearchLength);

	for (;;)
	{
		cout << "Enter command: ";
		string command;
		if (!getline(cin, command))
			break;
		if (command.empty())
			continue;
		switch (tolower(command[0]))
		{
		default:
			cout << "Invalid command " << command << endl;
			break;
		case 'q':
			delete library;
			return 0;
		case '?':
			showMenu();
			break;
		case 'c':
			createNewLibrary(library);
			break;
		case 'a':
			addOneGenomeManually(library);
			break;
		case 'l':
			loadOneDataFile(library);
			break;
		case 'd':
			loadProvidedFiles(library);
			break;
		case 'e':
			findGenome(library, true);
			break;
		case 's':
			findGenome(library, false);
			break;
		case 'r':
			findRelatedGenomesManual(library);
			break;
		case 'f':
			findRelatedGenomesFromFile(library);
			break;
		}
	}
}
