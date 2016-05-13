/**
 * @file arabic_8859_6.h
 * @brief header file for converting ISO-8859-6 encoding to Buckwalter
 * @author David Edelstein
 * @date: Created on: Sep 26, 2009
 */

#ifndef ARABIC_ISO_8859_6_H_
#define ARABIC_ISO_8859_6_H_

#include <map>
#include "arabic_buckwalter.h"

using std::map;
using std::pair;

/**
 * \namespace enc_88596
 * \brief Definitions and methods for encoding/decoding Arabic from ISO-8859-6
 */
namespace enc_88596 {
	/** ISO-8859-6 letter */
	typedef const unsigned int iso88596;
	/** Map of cp1256 to Buckwalter symbols */
	typedef map<iso88596, enc_bw::BW> iso88596bwmp;
	/** Pair for a iso88596bwmp */
	typedef pair<iso88596, enc_bw::BW> iso88596bwpr;
	/** Constant iterator for iso88596bwmp */
	typedef iso88596bwmp::const_iterator iso88596bwmp_ci;

	static iso88596 HAMZA					= 0xC1;
	static iso88596 ALEF_W_MADDA			= 0xC2;
	static iso88596 ALEF_UNDER_HAMZA		= 0xC3;
	static iso88596 WAW_UNDER_HAMZA			= 0xC4;
	static iso88596 ALEF_OVER_HAMZA			= 0xC5;
	static iso88596 YEH_UNDER_HAMZA			= 0xC6;
	static iso88596 ALEF					= 0xC7;
	static iso88596 BEH						= 0xC8;
	static iso88596 TEH_MARBUTA				= 0xC9;
	static iso88596 TEH						= 0xCA;
	static iso88596 THEH					= 0xCB;
	static iso88596 JEEM					= 0xCC;
	static iso88596 HAH						= 0xCD;
	static iso88596 KHAH					= 0xCE;
	static iso88596 DAL						= 0xCF;
	static iso88596 THAL					= 0xD0;
	static iso88596 REH						= 0xD1;
	static iso88596 ZAIN					= 0xD2;
	static iso88596 SEEN					= 0xD3;
	static iso88596 SHEEN					= 0xD4;
	static iso88596 SAD						= 0xD5;
	static iso88596 DAD						= 0xD6;
	static iso88596 TAH						= 0xD7;
	static iso88596 ZAH						= 0xD8;
	static iso88596 AIN						= 0xD9;
	static iso88596 GHAIN					= 0xDA;
	static iso88596 TATWEEL					= 0xE0;
	static iso88596 FEH						= 0xE1;
	static iso88596 QAF						= 0xE2;
	static iso88596 KAF						= 0xE3;
	static iso88596 LAM						= 0xE4;
	static iso88596 MEEM					= 0xE5;
	static iso88596 NOON					= 0xE6;
	static iso88596 HEH						= 0xE7;
	static iso88596 WAW						= 0xE8;
	static iso88596 ALEF_MAKSURA			= 0xE9;
	static iso88596 YEH						= 0xEA;
	static iso88596 FATHATAN				= 0xEB;
	static iso88596 DAMMATAN				= 0xEC;
	static iso88596 KASRATAN				= 0xED;
	static iso88596 FATHA					= 0xEE;
	static iso88596 DAMMA					= 0xEF;
	static iso88596 KASRA					= 0xF0;
	static iso88596 SHADDA					= 0xF1;
	static iso88596 SUKUN					= 0xF2;

	/**
	 * Pairs of arb_cp codepoints matched to Buckwalter transliteration symbols
	 */
	const static iso88596bwpr iso88596_bw_prs[] = {
			iso88596bwpr(HAMZA, enc_bw::HAMZA),
			iso88596bwpr(ALEF_W_MADDA, enc_bw::ALEF_W_MADDA),
			iso88596bwpr(ALEF_UNDER_HAMZA, enc_bw::ALEF_UNDER_HAMZA),
			iso88596bwpr(WAW_UNDER_HAMZA, enc_bw::WAW_UNDER_HAMZA),
			iso88596bwpr(ALEF_OVER_HAMZA, enc_bw::ALEF_OVER_HAMZA),
			iso88596bwpr(YEH_UNDER_HAMZA, enc_bw::YEH_UNDER_HAMZA),
			iso88596bwpr(ALEF, enc_bw::ALEF),
			iso88596bwpr(BEH, enc_bw::BEH),
			iso88596bwpr(TEH_MARBUTA, enc_bw::TEH_MARBUTA),
			iso88596bwpr(TEH, enc_bw::TEH),
			iso88596bwpr(THEH, enc_bw::THEH),
			iso88596bwpr(JEEM, enc_bw::JEEM),
			iso88596bwpr(HAH, enc_bw::HAH),
			iso88596bwpr(KHAH, enc_bw::KHAH),
			iso88596bwpr(DAL, enc_bw::DAL),
			iso88596bwpr(THAL, enc_bw::THAL),
			iso88596bwpr(REH, enc_bw::REH),
			iso88596bwpr(ZAIN, enc_bw::ZAIN),
			iso88596bwpr(SEEN, enc_bw::SEEN),
			iso88596bwpr(SHEEN, enc_bw::SHEEN),
			iso88596bwpr(SAD, enc_bw::SAD),
			iso88596bwpr(DAD, enc_bw::DAD),
			iso88596bwpr(TAH, enc_bw::TAH),
			iso88596bwpr(ZAH, enc_bw::ZAH),
			iso88596bwpr(AIN, enc_bw::AIN),
			iso88596bwpr(GHAIN, enc_bw::GHAIN),
			iso88596bwpr(FEH, enc_bw::FEH),
			iso88596bwpr(QAF, enc_bw::QAF),
			iso88596bwpr(KAF, enc_bw::KAF),
			iso88596bwpr(LAM, enc_bw::LAM),
			iso88596bwpr(MEEM, enc_bw::MEEM),
			iso88596bwpr(NOON, enc_bw::NOON),
			iso88596bwpr(HEH, enc_bw::HEH),
			iso88596bwpr(WAW, enc_bw::WAW),
			iso88596bwpr(ALEF_MAKSURA, enc_bw::ALEF_MAKSURA),
			iso88596bwpr(YEH, enc_bw::YEH),
			iso88596bwpr(FATHATAN, enc_bw::FATHATAN),
			iso88596bwpr(DAMMATAN, enc_bw::DAMMATAN),
			iso88596bwpr(KASRATAN, enc_bw::KASRATAN),
			iso88596bwpr(FATHA, enc_bw::FATHA),
			iso88596bwpr(DAMMA, enc_bw::DAMMA),
			iso88596bwpr(KASRA, enc_bw::KASRA),
			iso88596bwpr(SHADDA, enc_bw::SHADDA),
			iso88596bwpr(SUKUN, enc_bw::SUKUN),
			iso88596bwpr(TATWEEL, enc_bw::TATWEEL)
	};

	/** Map of all ISO-8859-6 constants to Buckwalter transliteration */
	const static iso88596bwmp ISO88596_BW(iso88596_bw_prs, iso88596_bw_prs + sizeof(iso88596_bw_prs) / sizeof(iso88596_bw_prs[0]));

/**
 * Convert a string encoded in ISO-8859-6 encoding to Buckwalter, and also set flag testing whether all letters were Arabic.
 * @param word a string encoded in ISO-8859-6
 * @param is_arabic boolean flag; will be set to true if all letters in word were recognized as Arabic, else false
 * @return word transliterated into Buckwalter encoding
 */
string iso88596_to_bw(const string& word, bool& is_arabic);

}



#endif /* ARABIC_ISO_8859_6_H_ */
