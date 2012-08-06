#include <sstream>
#include <algorithm>
#include <iostream>

#include "Assembler.h"
#include "Node.h"
#include "Suffix.h"

void Assembler::compute_overlaps(GeneralSuffixTree& gst) {
	label_nodes(gst);
	for (int j = 0; j < gst.string_index.size(); j++) {
		Node* x = gst.root;
		int depth = 0;
		while (!x->is_leaf()) {
			for (int i = 0; i < x->node_labels.size(); i++) {
					int min_length = std::min(	gst.string_index[x->node_labels[i]].second,
												gst.string_index[j].second);
					add_overlap(x->node_labels[i], j, std::min(min_length, depth));
				}
			x = x->get_child(gst, gst.tree_string[gst.string_index[j].first + depth]);
			depth += x->edge_length();
		}
	}
}

void Assembler::label_nodes(GeneralSuffixTree& gst) {
	std::vector<Node*> nodes_to_visit (1, gst.root);
	std::vector<int> depth_stack (1, 0);
	while (!nodes_to_visit.empty()) {
		Node* current_node = nodes_to_visit.back();
		int current_depth = depth_stack.back();
		nodes_to_visit.pop_back();
		depth_stack.pop_back();
		if (current_depth >= min_overlap)
			label_node(gst, current_node);
		if (!current_node->is_leaf()) {
			current_node->get_children(nodes_to_visit);
			current_node->get_child_depths(current_depth, depth_stack);
		}
	}
}

void Assembler::label_node(GeneralSuffixTree& gst, Node* node) {
	static int node_number = 0;
	node_number++;
	if (node_number % 1000 == 0) std::cout << "Labelling node: " << node_number << std::endl;
	std::vector<Node*> children;
	node->get_children(children);
	for (int i = 0; i < children.size(); i++) {
		if (isdigit(gst.tree_string[children[i]->begin_index])) {
		std::vector<Node*> leaves = gst.retrieve_leaves(Suffix(children[i], *children[i]->end_index));
		for (int j = 0; j < leaves.size(); j++)
			node->node_labels.push_back(gst.character_index[leaves[j]->ID]);

		}
	}
}

void Assembler::add_overlap(int string_i, int string_j, short int overlap) {
	Overlap* to_add = new Overlap(string_i, string_j, overlap);
	if (head == NULL) {
		head = to_add;
	}
	else if (overlap >= head->overlap) {
		to_add->next = head;
		head = to_add;
	}
	else {
		Overlap* position = head;;
		while (position->next != NULL && overlap < position->next->overlap)
			position = position->next;
		to_add->next = position->next;
		position->next = to_add;
	}
}

void Assembler::print_overlaps(const GeneralSuffixTree& gst) {
	std::cout << "entered print overlaps" << std::endl;
	freopen("overlap_log", "w", stdout);
	Overlap* scan = head;
	while (scan != NULL) {
		int i = scan->string_i;
		int j = scan->string_j;
		std::cout << "Strings: " 
		<< gst.tree_string.substr(gst.string_index[i].first, gst.string_index[i].second) 
		<< "/" 
		<< gst.tree_string.substr(gst.string_index[j].first, gst.string_index[j].second) 
		<< " overlap by: " << scan->overlap << std::endl;
		scan = scan->next;
	}
	freopen( "CON", "w", stdout );
}



