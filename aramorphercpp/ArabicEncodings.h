/**
 * @file ArabicEncodings.h
 * @brief Defines encodings Aramorpher can use, and includes all the necessary encoding header files for each.
 *
 * @date Sep 26, 2009
 * @author David Edelstein
 */

#ifndef ARABICENCODINGS_H_
#define ARABICENCODINGS_H_

#include <map>
#include <string>
#include <vector>
#include <boost/algorithm/string_regex.hpp>
#include "AramorpherUtils.h"
#include "arabic_utf8.h"
#include "arabic_buckwalter.h"
#include "arabic_cp1256.h"
#include "arabic_8859_6.h"

using std::map;
using std::string;
using std::vector;

namespace Aramorph {
	/** To recognize UTF-8 (with or without dash, case-insensitive */
	const static boost::regex UTF8_RGX("UTF\\-?8", boost::regex::icase);
	/** To recognize CP1256 ("cp" or "win" prefix, with or without dash, case-insensitive) */
	const static boost::regex CP1256_RGX("(cp|win(dows)?)\\-?1256", boost::regex::icase);
	/** To recognize ISO-8859-6 ("ISO" or "IEC" prefix, with or without dashes and spaces, case-insensitive) */
	const static boost::regex ISO88596_RGX("(iso|iec)[\\-_]?8859[\\-_]?6", boost::regex::icase);

	/** Encodings that the Aramorpher can process. */
	enum Encoding {
		/** Buckwalter encoding */
		BUCKWALTER,
		/** UTF-8 encoding */
		UTF8,
		/** CP-1256 (Win1256) encoding */
		CP1256,
		/** ISO/IEC 8859-6 encoding */
		ISO_8859_6
	};
	/** Map of Encodings to their names. */
	typedef map<const Encoding, string> enc_map;
	/** Constant iterator for enc_map. */
	typedef enc_map::const_iterator enc_map_ci;
	/** Pair mapping an Encoding to its name. */
	typedef std::pair<const Encoding, string> enc_pr;


	/** Array of enc_prs used in created ENC_MAP. */
	const static enc_pr enc_strs[] = {
			enc_pr(BUCKWALTER, 	"Buckwalter"),
			enc_pr(UTF8, 		"UTF-8"),
			enc_pr(CP1256, 		"cp1256"),
			enc_pr(ISO_8859_6, 	"ISO/IEC 8859-6")
	};
	/** Map of each Encoding to its display name. */
	const static enc_map ENC_MAP(enc_strs, enc_strs + sizeof(enc_strs) / sizeof(enc_strs[0]));

	/**
	 * Turn Encoding into string representation.
	 * @param enc Encoding
	 * @return the normalized name of ENC
	 */
	inline string enc_str(const Encoding& enc) {
		string encstr;
		enc_map_ci it = ENC_MAP.find(enc);
		if (it != ENC_MAP.end()) {
			encstr = it->second;
		} else {
			// should not happen!
			encstr = "unrecognized encoding: " + boost::lexical_cast<string>(enc);
		}
		return encstr;
	}

	/**
	 * Given a string, attempt to determine which Encoding constant is being requested and return it.
	 * Uses regexes to perform case-insensitive searches, including with and without dashes, etc.
	 * @param encstr name of the desired encoding.
	 * @return Encoding matching the name
	 * @throws EncodingException if encstr is not recognized
	 */
	inline Encoding getEncoding(const string& encstr) {
		Encoding enc;
		if (boost::istarts_with(encstr, "buck")) {
			enc = BUCKWALTER;
		} else if (boost::regex_match(encstr, UTF8_RGX)) {
			enc = UTF8;
		} else if (boost::regex_match(encstr, CP1256_RGX)) {
			enc = CP1256;
		} else if (boost::regex_match(encstr, ISO88596_RGX)) {
			enc = ISO_8859_6;
		} else {
			throw EncodingException(encstr);
		}
		return enc;
	}

	/**
	 * Get all the Encodings recognized by Aramorpher.
	 * @return a Vector of Encoding names
	 */
	inline vector<string> all_encodings() {
		vector<string> encs(ENC_MAP.size());
		int i = 0;
		for (enc_map_ci it = ENC_MAP.begin(); it != ENC_MAP.end(); it++, i++) {
			encs[i] = it->second;
		}
		return encs;
	}

}

#endif /* ARABICENCODINGS_H_ */
