#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>
#include <stack>

struct Node;

template<typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

      // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
	struct Node {
		std::vector<ValueType> values;
		std::vector<Node*> children;
		std::vector<char> childLabel;
	};

	Node* root;
	void fullDelete(Node* start);
	std::vector<ValueType> findHelper(Node* start, const std::string& key, bool exactMatchOnly, int n) const;
};



template<typename ValueType>
inline
void Trie<ValueType>::fullDelete(Node* start) {
	if (start == nullptr) {
		return;
	}
	for (int i = 0; i < start->children.size(); i++) {
		fullDelete(start->children[i]);
	}
	delete start;
}

template<typename ValueType>
Trie<ValueType>::Trie() {
	root = new Node();
}

template<typename ValueType>
Trie<ValueType>::~Trie() {
	fullDelete(root);
}

template<typename ValueType>
void Trie<ValueType>::reset() {
	fullDelete(root);

	root = new Node();
}

template<typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value) {
	Node* temp = root;
	bool found = false;
	for (int i = 0; i < key.size(); i++) {
		for (int j = 0; j < temp->children.size(); j++) {
			if (key[i] == temp->childLabel[j]) {
				temp = temp->children[j];
				found = true;
				break;
			}
		}
		if (!found) {
			temp->children.push_back(new Node());
			temp->childLabel.push_back(key[i]);
			temp = temp->children[temp->children.size() - 1];
		}
		found = false;
	}
	temp->values.push_back(value);
}

template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const {
	std::vector<ValueType> empty;
	if (root == nullptr) {
		return empty;
	}

	return findHelper(root, key, exactMatchOnly, 0);
}

template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::findHelper(Node* start, const std::string& key, bool exactMatchOnly, int n) const {

	std::vector<ValueType> ret;
	if (key.size() == 0) {
		return ret;
	}
	if (start->children.empty()) {
		return start->values ;
	}
	if (n == key.size()) {
		return start->values;
	}

	for (int i = 0; i < start->children.size(); i++) {
		if (key[n] == start->childLabel[i]) {
			std::vector<ValueType> temp = findHelper(start->children[i], key, exactMatchOnly, n + 1);
			ret.insert(ret.end(), temp.begin(), temp.end());
		}
		else if (!exactMatchOnly && n > 0) {
			std::vector<ValueType> temp = findHelper(start->children[i], key, true, n + 1);
			ret.insert(ret.end(), temp.begin(), temp.end());
		}
	}
	return ret;
}

#endif // TRIE_INCLUDED
