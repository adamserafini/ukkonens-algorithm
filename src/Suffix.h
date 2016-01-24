#pragma once

class SuffixTree;
class Node;
class Suffix {
public:
    Suffix(Node*, int);
    bool ends_at_node() const;
    bool ends_at_leaf() const;
    bool continues_with_char(const SuffixTree&, int) const;
	bool RULE2_conditions(const SuffixTree&, int) const;
	bool new_internal_node;
	Node* walk_up(int&, int&) const;
	
	Node* node;
    int char_index;
};
