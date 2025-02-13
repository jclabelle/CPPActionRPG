#include "PAS_TinyXMLFunctions.h"

#include <iostream>

#define debug

// Finds all nodes starting from tinyxml2::XMLhandle and saves them in KeyA vector of handles
void xml_node_list(tinyxml2::XMLHandle handle, std::vector<tinyxml2::XMLHandle>& node_list)
{
#ifdef debug
	std::cout << "building recursive list, new node found: " << handle.ToNode()->Value() << '\n';
#endif

	// save the new handle
	node_list.push_back(handle);

	// recurse through the xml
	if (handle.FirstChildElement().ToNode())
	{
		tinyxml2::XMLHandle next = handle.FirstChildElement();
		xml_node_list(next, node_list);
	}
	if (handle.NextSiblingElement().ToNode())
	{
		tinyxml2::XMLHandle next = handle.NextSiblingElement();
		xml_node_list(next, node_list);
	}
	return;
}

tinyxml2::XMLHandle xml_find_element(std::vector<tinyxml2::XMLHandle> node_list, std::string search_term) {
	for (auto node : node_list) {
		if (node.ToElement()->Name() == search_term)
			return node;
	}
	std::cout << "term not found, returning first node";
	return node_list[0];
}

void xml_print_node_list(std::vector<tinyxml2::XMLHandle> node_list)
{
	for (auto handle : node_list)
	{
		std::cout << handle.ToElement()->Value() << '\n';
		if (handle.ToElement()->GetText())
			std::cout << handle.ToElement()->GetText();
	}
}