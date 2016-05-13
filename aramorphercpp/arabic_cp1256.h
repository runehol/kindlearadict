/**
 * @file arabic_cp1256.h
 * @brief header file for converting cp1256 encoding to Buckwalter
 * @author David Edelstein
 * @date: Created on: Sep 26, 2009
 */

#ifndef ARABIC_CP1256_H_
#define ARABIC_CP1256_H_

#include <map>
#include "arabic_buckwalter.h"

using std::map;
using std::pair;

/**
 * \namespace enc_cp1256
 * \brief Definitions and methods for encoding/decoding Arabic from cp1256
 */
namespace enc_cp1256 {
	/** cp1256 letter */
	typedef const unsigned int cp1256;
	/** Map of cp1256 to Buckwalter symbols */
	typedef map<cp1256, enc_bw::BW> cp1256bwmp;
	/** Pair for a cp1256bwmp */
	typedef pair<cp1256, enc_bw::BW> cp1256bwpr;
	/** Constant iterator for cp1256bwmp */
	typedef cp1256bwmp::const_iterator cp1256bwmp_ci;

	static cp1256 PEH 				= 0x81;
	static cp1256 TTEH 				= 0x8A;
	static cp1256 TCHEH 			= 0x8D;
	static cp1256 JEH 				= 0x8E;
	static cp1256 GAF 				= 0x90;
	static cp1256 HAMZA 			= 0xC1;
	static cp1256 ALEF_W_MADDA		= 0xC2;
	static cp1256 ALEF_UNDER_HAMZA	= 0xC3;
	static cp1256 WAW_UNDER_HAMZA	= 0xC4;
	static cp1256 ALEF_OVER_HAMZA	= 0xC5;
	static cp1256 YEH_UNDER_HAMZA	= 0xC6;
	static cp1256 ALEF				= 0xC7;
	static cp1256 BEH				= 0xC8;
	static cp1256 TEH_MARBUTA		= 0xC9;
	static cp1256 TEH				= 0xCA;
	static cp1256 THEH				= 0xCB;
	static cp1256 JEEM 				= 0xCC;
	static cp1256 HAH 				= 0xCD;
	static cp1256 KHAH 				= 0xCE;
	static cp1256 DAL				= 0xCF;
	static cp1256 THAL				= 0xD0;
	static cp1256 REH				= 0xD1;
	static cp1256 ZAIN				= 0xD2;
	static cp1256 SEEN				= 0xD3;
	static cp1256 SHEEN				= 0xD4;
	static cp1256 SAD 				= 0xD5;
	static cp1256 DAD				= 0xD6;
	static cp1256 TAH				= 0xD8;
	static cp1256 ZAH				= 0xD9;
	static cp1256 AIN 				= 0xDA;
	static cp1256 GHAIN				= 0xDB;
	static cp1256 TATWEEL			= 0xDC;
	static cp1256 FEH				= 0xDD;
	static cp1256 QAF 				= 0xDE;
	static cp1256 KAF				= 0xDF;
	static cp1256 LAM				= 0xE1;
	static cp1256 MEEM				= 0xE3;
	static cp1256 NOON				= 0xE4;
	static cp1256 HEH				= 0xE5;
	static cp1256 WAW				= 0xE6;
	static cp1256 ALEF_MAKSURA		= 0xEC;
	static cp1256 YEH				= 0xED;
	static cp1256 FATHATAN			= 0xF0;
	static cp1256 DAMMATAN			= 0xF1;
	static cp1256 KASRATAN			= 0xF2;
	static cp1256 FATHA				= 0xF3;
	static cp1256 DAMMA				= 0xF5;
	static cp1256 KASRA				= 0xF6;
	static cp1256 SHADDA			= 0xF8;
	static cp1256 SUKUN				= 0xFA;

	/**
	 * Pairs of arb_cp codepoints matched to Buckwalter transliteration symbols
	 */
	const static cp1256bwpr cp1256_bw_prs[] = {
			cp1256bwpr(HAMZA, enc_bw::HAMZA),
			cp1256bwpr(PEH, enc_bw::PEH),
			cp1256bwpr(JEH, enc_bw::JEH),
			cp1256bwpr(TCHEH, enc_bw::TCHEH),
			cp1256bwpr(GAF, enc_bw::GAF),
			cp1256bwpr(HAMZA, enc_bw::HAMZA),
			cp1256bwpr(ALEF_W_MADDA, enc_bw::ALEF_W_MADDA),
			cp1256bwpr(ALEF_UNDER_HAMZA, enc_bw::ALEF_UNDER_HAMZA),
			cp1256bwpr(WAW_UNDER_HAMZA, enc_bw::WAW_UNDER_HAMZA),
			cp1256bwpr(ALEF_OVER_HAMZA, enc_bw::ALEF_OVER_HAMZA),
			cp1256bwpr(YEH_UNDER_HAMZA, enc_bw::YEH_UNDER_HAMZA),
			cp1256bwpr(ALEF, enc_bw::ALEF),
			cp1256bwpr(BEH, enc_bw::BEH),
			cp1256bwpr(TEH_MARBUTA, enc_bw::TEH_MARBUTA),
			cp1256bwpr(TEH, enc_bw::TEH),
			cp1256bwpr(THEH, enc_bw::THEH),
			cp1256bwpr(JEEM, enc_bw::JEEM),
			cp1256bwpr(HAH, enc_bw::HAH),
			cp1256bwpr(KHAH, enc_bw::KHAH),
			cp1256bwpr(DAL, enc_bw::DAL),
			cp1256bwpr(THAL, enc_bw::THAL),
			cp1256bwpr(REH, enc_bw::REH),
			cp1256bwpr(ZAIN, enc_bw::ZAIN),
			cp1256bwpr(SEEN, enc_bw::SEEN),
			cp1256bwpr(SHEEN, enc_bw::SHEEN),
			cp1256bwpr(SAD, enc_bw::SAD),
			cp1256bwpr(DAD, enc_bw::DAD),
			cp1256bwpr(TAH, enc_bw::TAH),
			cp1256bwpr(ZAH, enc_bw::ZAH),
			cp1256bwpr(AIN, enc_bw::AIN),
			cp1256bwpr(GHAIN, enc_bw::GHAIN),
			cp1256bwpr(FEH, enc_bw::FEH),
			cp1256bwpr(QAF, enc_bw::QAF),
			cp1256bwpr(KAF, enc_bw::KAF),
			cp1256bwpr(LAM, enc_bw::LAM),
			cp1256bwpr(MEEM, enc_bw::MEEM),
			cp1256bwpr(NOON, enc_bw::NOON),
			cp1256bwpr(HEH, enc_bw::HEH),
			cp1256bwpr(WAW, enc_bw::WAW),
			cp1256bwpr(ALEF_MAKSURA, enc_bw::ALEF_MAKSURA),
			cp1256bwpr(YEH, enc_bw::YEH),
			cp1256bwpr(FATHATAN, enc_bw::FATHATAN),
			cp1256bwpr(DAMMATAN, enc_bw::DAMMATAN),
			cp1256bwpr(KASRATAN, enc_bw::KASRATAN),
			cp1256bwpr(FATHA, enc_bw::FATHA),
			cp1256bwpr(DAMMA, enc_bw::DAMMA),
			cp1256bwpr(KASRA, enc_bw::KASRA),
			cp1256bwpr(SHADDA, enc_bw::SHADDA),
			cp1256bwpr(SUKUN, enc_bw::SUKUN),
			cp1256bwpr(TATWEEL, enc_bw::TATWEEL)
	};


	/** Map of all cp1256 constants to Buckwalter transliteration */
	const static cp1256bwmp CP1256_BW(cp1256_bw_prs, cp1256_bw_prs + sizeof(cp1256_bw_prs) / sizeof(cp1256_bw_prs[0]));

/**
 * Convert a string encoded in cp1256 encoding to Buckwalter, and also set flag testing whether all letters were Arabic.
 * @param word a string encoded in cp1256
 * @param is_arabic boolean flag; will be set to true if all letters in word were recognized as Arabic, else false
 * @return word transliterated into Buckwalter encoding
 */
string cp1256_to_bw(const string& word, bool& is_arabic);

}



#endif /* ARABIC_CP1256_H_ */
