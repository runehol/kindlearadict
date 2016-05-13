/**
 * @file arabic_buckwalter.cpp
 * @brief Implementation file for encoding Arabic using Buckwalter transliteration.
 * @author David Edelstein
 */

#include "arabic_buckwalter.h"
#include <boost/algorithm/string_regex.hpp>

using namespace enc_bw;

bool enc_bw::is_arabic(const string& word, const bool strict) {
	bool arb = word.find_first_not_of(ARABIC_LETTERS) == string::npos;
	if (arb && strict) {
		arb = word.find_first_of(NON_ARABIC) == string::npos;
	}
	return arb;
}

void enc_bw::normalize(string& diacstr) {
	boost::algorithm::erase_all_regex(diacstr, NORMALIZER_RGX);
	boost::replace_all(diacstr, ALEF_WASLA_STR, ALEF_STR);
}

