//
// span_map
// kuchynskiandrei@gmail.com
// 2023
//

#include <iostream>
#include <chrono>

#include "span_map.hpp"

using K = int;
using V = int;

template<typename K, typename V, V default_value>
int test(span_map<K, V, default_value> &map, const K &begin, const K &end, const V &val)
{
	auto initial_map(map);

	map.insert(begin, end, val);

	for (auto i = 0; i < 256; i++) {
		if (i < begin) {
			if (map[i] != initial_map[i]) {
				std::cout << "err1" << std::endl;
				return -1;
			}
		} else if (i >= end) {
			if (map[i] != initial_map[i]) {
				std::cout << "err2 " << std::endl;
				return -1;
			}
		} else {
			if (map[i] != val) {
				std::cout << "err3" << std::endl;
				return -1;
			}
		}
	}

	return 0;
}

template<typename K, typename V, V default_value>
void print_ranges(span_map<K, V, default_value> &map)
{
	std::cout << "ranges:" << std::endl;
	for (auto i = 0; i < map.size(); i++) {
		std::pair<K,K> key_pair;
		if (map.range(i, key_pair) && map[key_pair.first] != default_value)
			std::cout << " [" << key_pair.first << ", " << key_pair.second << ") == " << map[key_pair.first] << std::endl;
	}
}

bool run_test()
{
	span_map<K, V, 65> map;

	std::srand(std::time(nullptr));

	for (auto i = 0; i < 1000000; i++) {
		const K begin = std::rand() / ((RAND_MAX + 1u) / 256);
		const K end = std::rand() / ((RAND_MAX + 1u) / 256);
		const V val = std::rand() / ((RAND_MAX + 1u) / 256);

		if (test<K, V>(map, begin, end, val)) {
			std::cout << "test failed" << std::endl;
			return false;
		}
	}

	std::cout << "test passed " << std::endl;
	print_ranges<K, V>(map);

	return true;
}

int main(int argc, char** argv)
{
	run_test();
	
	const V default_value = ' ';
	span_map<K, V, default_value> map;
	map.insert(6, 10, 'C');
	map.insert(8, 12, 'D');
	map.insert(8, 10, 'F');
	map.erase_range(9);
	//map.erase(8, 11);
	map.insert(0, 2, ' ');
	
	//for (auto i = -10; i < 20; i++) std::cout << i << ": " << map[i] << std::endl;

	print_ranges<K, V>(map);

	return 0;
}
