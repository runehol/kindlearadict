/**
 * @file arabic_utf8.cpp
 * @brief Implementation file for functions defining Arabic encodings in UTF-8.
 *
 *  Created on: Apr 22, 2009
 *      Author: David Edelstein
 */

#include <sstream>
#include "utf8.h"
#include "arabic_utf8.h"

using namespace enc_utf8;

void enc_utf8::strip_bom(string& str) {
    if (str.size() > BOM_SIZE) {
        if (utf8::is_bom(str.c_str())) {
        	// erase BOM
        	str.erase(0, BOM_SIZE);
        }
    }
}

bool enc_utf8::is_arabic(const string& word) {
	// first check for invalid utf-8
    string::const_iterator end_it = utf8::find_invalid(word.begin(), word.end());
	if (end_it == word.end()) {
		string::const_iterator w = word.begin();
		while (w != end_it) {
			// we checked valid above, so can use unchecked here
			utf8::uint32_t cp = utf8::unchecked::next(w);
			if (UTF8_BW.find(cp) == UTF8_BW.end()) {
				// the codepoint wasn't found in our map
				return false;
			}
		}
		// all codepoints were Arabic
		return true;
	}
	// word did not have valid UTF-8 encoding
	return false;
}

string enc_utf8::utf8_to_bw(const string& word, bool& is_arabic) {
	std::ostringstream bwstr;
	// first check for invalid utf-8
    string::const_iterator end_it = utf8::find_invalid(word.begin(), word.end());
	if (end_it != word.end()) {
		is_arabic = false;
	} else {
		// default to true unless we hit non-Arabic letter
		is_arabic = true;
		string::const_iterator w = word.begin();
		while (w != end_it) {
			// we checked valid above, so can use unchecked here
			utf8::uint32_t cp = utf8::unchecked::next(w);
			utf8bwmp_ci bwi = UTF8_BW.find(cp);
			if (bwi == UTF8_BW.end()) {
				bwstr << cp;
				is_arabic = false;
			} else {
				bwstr << bwi->second;
			}
		}
	}
	return bwstr.str();
}
