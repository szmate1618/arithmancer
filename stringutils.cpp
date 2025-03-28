#include <numeric>  // for std::accumulate

#include "stringutils.hpp"


std::string join(const std::vector<std::string>& strings, const std::string& delimiter) {
	return std::accumulate(std::next(strings.begin()), strings.end(), strings[0],
		[&delimiter](const std::string& a, const std::string& b) { return a + delimiter + b; });
}
