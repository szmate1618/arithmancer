#include <numeric>  // for std::accumulate

#include "stringutils.hpp"


std::string join(const std::vector<std::string>& strings, const std::string& delimiter) {
	size_t total_length = 0;
	for (const std::string& s : strings) {
		total_length += s.length();
	}
	total_length += delimiter.length() * (strings.size() - 1);

	std::string ret(total_length, ' ');
	size_t index = 0;
	for (const std::string& s : strings) {
		for (const char c: s)
		{
			ret[index] = c;
			index++;
		}
		if (index < total_length) {
			for (const char c : delimiter)
			{
				ret[index] = c;
				index++;
			}
		}
	}

	return ret;
}
