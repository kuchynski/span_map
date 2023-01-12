//
// span_map
// kuchynskiandrei@gmail.com
// 2023
//

#ifndef H_span_map
#define H_span_map

#include <map>

template<typename K, typename V, V default_value>
class span_map: protected std::map<K,V>
{
private:
	const static V default_element;

	const V& previous_element(auto it) const {
		return it == std::map<K,V>::begin()? default_element : (--it)->second;
	}

public:
	const V& operator[](const K &key) const
	{
		return previous_element(std::map<K,V>::upper_bound(key));
	}

	size_t size() const { return std::map<K,V>::size(); }
	bool empty() const { return size() == 0; }
	void clear() { std::map<K,V>::clear(); }

	void insert(const K &key_begin, const K &key_end, const V &value)
	{
		if (key_begin < key_end) {
			auto it_begin_range = std::map<K,V>::lower_bound(key_begin);
			const auto it_end_range = std::map<K,V>::upper_bound(key_end);

			// Step 3.1: define and save the end element value
			const auto after_value = previous_element(it_end_range);

			// Step 1: define and insert the begin element
			if (previous_element(it_begin_range) != value) {
				it_begin_range = std::map<K,V>::insert_or_assign(it_begin_range, key_begin, value);
				it_begin_range++;
			}

			// Step 2: Erase everything inside the new range
			std::map<K,V>::erase(it_begin_range, it_end_range);

			// Step 3.2: Insert the end element
			if (after_value != value) {
				std::map<K,V>::insert_or_assign(it_end_range, key_end, after_value);
			}
		}
	}

	void change_range(const K &key, const V &value)
	{
		auto it_find = std::map<K,V>::upper_bound(key);

		if (it_find != std::map<K,V>::begin()) {
			(--it_find)->second = value;
		}
	}

	void erase(const K &key_begin, const K &key_end)
	{
		insert(key_begin, key_end, default_element);
	}

	void erase_range(const K &key)
	{
		change_range(key, default_element);
	}

	bool range(int number, std::pair<K,K> &key_pair)
	{
		const int ranges_number = size() - 1;
		
		if (ranges_number > 0) {
			if (number < 0)
				number = ranges_number - 1;
			
			if (number < ranges_number) {
				auto it_find = std::map<K,V>::begin();
				for (auto i = 0; i < number; i++, it_find++)
				{}
				key_pair.first = it_find->first;
				key_pair.second = (++it_find)->first;
				return true;
			}
		}
		return false;
	}

	bool range_with_key(const K &key, std::pair<K,K> &key_pair)
	{
		auto it_find = std::map<K,V>::upper_bound(key);

		if (it_find != std::map<K,V>::begin() && it_find != std::map<K,V>::end()) {
			key_pair.second = it_find->first;
			key_pair.first = (--it_find)->first;
			return true;
		}
		return false;
	}

	bool start_key(int number, K& key) const // get begin of the rang number 'number'
	{
		if (!empty()) {
			if (number < 0)
				number = size() - 1;
			
			if (number < size()) {
				auto it_find = std::map<K,V>::begin();
				for (auto i = 0; i < number; i++, it_find++)
				{}
				key = it_find->first;
				return true;
			}
		}
		return false;
	}
};

template<typename K, typename V, V default_value>
constexpr V span_map<K, V, default_value>::default_element{default_value};

#endif
