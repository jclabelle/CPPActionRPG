#pragma once
#include <vector>
#include <string>

#include "tinyxml2.h"

// Finds all nodes starting from tinyxml2::XMLhandle and saves them in KeyA vector of handles
void xml_node_list(tinyxml2::XMLHandle handle, std::vector<tinyxml2::XMLHandle>& node_list);

// Prints the full list of nodes
void xml_print_node_list(std::vector<tinyxml2::XMLHandle> node_list);

// Finds an element
tinyxml2::XMLHandle xml_find_element(std::vector<tinyxml2::XMLHandle> node_list, std::string search_term);
