/**
 * @file arabic_8859_6.cpp
 * @brief Implementation file for converting ISO-8859-6 encoding to Buckwalter
 * @author David Edelstein
 * @date: Created on: Sep 26, 2009
 */

#include <sstream>
#include "arabic_8859_6.h"
#include "utf8.h"

using namespace enc_88596;

string enc_88596::iso88596_to_bw(const string& word, bool& is_arabic) {
	std::ostringstream bwstr;
	// default to true unless we hit non-Arabic letter
	is_arabic = true;
	for (string::const_iterator it = word.begin(); it != word.end(); it++) {
		// this is kind of a hack, as the utf8 method isn't meant to be used externally.
		// Should use a more portable/better method to cast each letter to a cp1256 8-bit codepoint
		iso88596 cp = utf8::internal::mask8(*it);
		iso88596bwmp_ci pr = ISO88596_BW.find(cp);
		if (pr == ISO88596_BW.end()) {
			bwstr << cp;
			is_arabic = false;
		} else {
			bwstr << pr->second;
		}
	}
	return bwstr.str();
}
