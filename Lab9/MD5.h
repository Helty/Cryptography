#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <bitset>
#include <bit>
#include <sstream>
#include <iomanip>

using WordArray = std::vector<uint32_t>;

class CMD5
{
public:

	static std::string Hashed(std::string const& message);

private:

	static WordArray PadingMessage(std::string const& message);
};

