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

public:
	const V& operator[](const K &key) const
	{
		auto it_find = std::map<K,V>::upper_bound(key);
		return (it_find == std::map<K,V>::begin())? default_element : (--it_find)->second;
	}

	size_t size() const { return std::map<K,V>::size(); }
	bool empty() const { return size() == 0; }
	void clear() { std::map<K,V>::clear(); }

	void insert(const K &key_begin, const K &key_end, const V &value)
	{
		if (key_begin < key_end) {
			// Step 1.1: define the end element value
			auto it_end_erase = std::map<K,V>::upper_bound(key_end);
			auto it_after_value = it_end_erase;
			const auto after_value = (it_after_value == std::map<K,V>::begin())? default_value : (--it_after_value)->second;

			// Step 2: define and insert the begin element
			auto it_begin_erase = std::map<K,V>::lower_bound(key_begin);
			auto it_before_value = it_begin_erase;
			const auto &before_value = (it_before_value == std::map<K,V>::begin())? default_value : (--it_before_value)->second;
			if (before_value != value) {
				it_begin_erase = std::map<K,V>::insert_or_assign(it_begin_erase, key_begin, value);
				it_begin_erase++;
			}

			// Step 1.2: shift the hint for the end element insertion
			it_after_value = it_begin_erase;
			it_after_value--;

			// Step 3: Erase everything inside the new range
			std::map<K,V>::erase(it_begin_erase, it_end_erase);

			// Step 1.3: Insert end element
			if (after_value != value) {
				std::map<K,V>::insert_or_assign(it_after_value, key_end, after_value);
			}
		}
	}

	void change_range(const K &key, const V &value)
	{
		auto it_erase = std::map<K,V>::upper_bound(key);

		if (it_erase != std::map<K,V>::begin()) {
			(--it_erase)->second = value;
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
		int ranges_number = size() - 1;
		
		if (ranges_number > 0) {
			auto it_find = std::map<K,V>::begin();
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
			auto it_find = std::map<K,V>::begin();
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
const V span_map<K, V, default_value>::default_element{default_value};

#endif
