/**
 * @file arabic_utf8.h
 * @brief Header file for functions defining Arabic encodings in UTF-8.
 * @date Apr 18, 2009
 * @author David Edelstein
 *
 */
#ifndef ARABIC_UTF8_ENCODINGS_H_
#define ARABIC_UTF8_ENCODINGS_H_

#include <map>
#include "utf8.h"
#include "arabic_buckwalter.h"

using std::map;
using std::pair;

/**
 * \namespace enc_utf8
 * \brief Definitions and methods for encoding/decoding Arabic from UFT-8
 */
namespace enc_utf8 {
	/** Specific Arabic Unicode constants */
	typedef const utf8::uint32_t arb_cp;

	/** Map of arb_cp to Buckwalter symbols */
	typedef map<arb_cp, enc_bw::BW> utf8bwmp;
	/** Pair for a utf8bwmp */
	typedef pair<arb_cp, enc_bw::BW> utf8bwpr;
	/** Constant iterator for utf8bwmp */
	typedef utf8bwmp::const_iterator utf8bwmp_ci;

	// the Unicode codepoints in Arabic
	static arb_cp HAMZA 				= 0x0621;
	static arb_cp ALEF_W_MADDA 		= 0x0622;
	static arb_cp ALEF_UNDER_HAMZA	= 0x0623;
	static arb_cp WAW_UNDER_HAMZA		= 0x0624;
	static arb_cp ALEF_OVER_HAMZA		= 0x0625;
	static arb_cp YEH_UNDER_HAMZA		= 0x0626;
	static arb_cp ALEF				= 0x0627;
	static arb_cp BEH					= 0x0628;
	static arb_cp TEH_MARBUTA			= 0x0629;
	static arb_cp TEH					= 0x062A;
	static arb_cp THEH				= 0x062B;
	static arb_cp JEEM				= 0x062C;
	static arb_cp HAH					= 0x062D;
	static arb_cp KHAH				= 0x062E;
	static arb_cp DAL					= 0x062F;
	static arb_cp THAL				= 0x0630;
	static arb_cp REH					= 0x0631;
	static arb_cp ZAIN				= 0x0632;
	static arb_cp SEEN				= 0x0633;
	static arb_cp SHEEN				= 0x0634;
	static arb_cp SAD					= 0x0635;
	static arb_cp DAD					= 0x0636;
	static arb_cp TAH					= 0x0637;
	static arb_cp ZAH					= 0x0638;
	static arb_cp AIN					= 0x0639;
	static arb_cp GHAIN				= 0x063A;
	static arb_cp TATWEEL				= 0x0640;
	static arb_cp FEH					= 0x0641;
	static arb_cp QAF					= 0x0642;
	static arb_cp KAF					= 0x0643;
	static arb_cp LAM					= 0x0644;
	static arb_cp MEEM				= 0x0645;
	static arb_cp NOON				= 0x0646;
	static arb_cp HEH					= 0x0647;
	static arb_cp WAW					= 0x0648;
	static arb_cp ALEF_MAKSURA		= 0x0649;
	static arb_cp YEH					= 0x064A;
	static arb_cp FATHATAN			= 0x064B;
	static arb_cp DAMMATAN			= 0x064C;
	static arb_cp KASRATAN			= 0x064D;
	static arb_cp FATHA				= 0x064E;
	static arb_cp DAMMA				= 0x064F;
	static arb_cp KASRA				= 0x0650;
	static arb_cp SHADDA				= 0x0651;
	static arb_cp SUKUN				= 0x0652;
	static arb_cp SUPERSCRIPT_ALEF 	= 0x0670;
	static arb_cp ALEF_WASLA 			= 0x0671;
	static arb_cp PEH 				= 0x067E;
	static arb_cp TCHEH 				= 0x0686;
	static arb_cp JEH					= 0x0698;
	static arb_cp VEH 				= 0x06A4;
	static arb_cp GAF					= 0x06AF;

	/**
	 * Pairs of arb_cp codepoints matched to Buckwalter transliteration symbols
	 */
	const static utf8bwpr utf8_bw_prs[] = {
		utf8bwpr(HAMZA, enc_bw::HAMZA),
		utf8bwpr(ALEF_WASLA, enc_bw::ALEF_WASLA),
		utf8bwpr(VEH, enc_bw::VEH),
		utf8bwpr(SUPERSCRIPT_ALEF, enc_bw::SUPERSCRIPT_ALEF),
		utf8bwpr(PEH, enc_bw::PEH),
		utf8bwpr(JEH, enc_bw::JEH),
		utf8bwpr(TCHEH, enc_bw::TCHEH),
		utf8bwpr(GAF, enc_bw::GAF),
		utf8bwpr(HAMZA, enc_bw::HAMZA),
		utf8bwpr(ALEF_W_MADDA, enc_bw::ALEF_W_MADDA),
		utf8bwpr(ALEF_UNDER_HAMZA, enc_bw::ALEF_UNDER_HAMZA),
		utf8bwpr(WAW_UNDER_HAMZA, enc_bw::WAW_UNDER_HAMZA),
		utf8bwpr(ALEF_OVER_HAMZA, enc_bw::ALEF_OVER_HAMZA),
		utf8bwpr(ALEF_OVER_HAMZA, enc_bw::ALEF_OVER_HAMZA),
		utf8bwpr(YEH_UNDER_HAMZA, enc_bw::YEH_UNDER_HAMZA),
		utf8bwpr(ALEF, enc_bw::ALEF),
		utf8bwpr(BEH, enc_bw::BEH),
		utf8bwpr(TEH_MARBUTA, enc_bw::TEH_MARBUTA),
		utf8bwpr(TEH, enc_bw::TEH),
		utf8bwpr(THEH, enc_bw::THEH),
		utf8bwpr(JEEM, enc_bw::JEEM),
		utf8bwpr(HAH, enc_bw::HAH),
		utf8bwpr(KHAH, enc_bw::KHAH),
		utf8bwpr(DAL, enc_bw::DAL),
		utf8bwpr(THAL, enc_bw::THAL),
		utf8bwpr(REH, enc_bw::REH),
		utf8bwpr(ZAIN, enc_bw::ZAIN),
		utf8bwpr(SEEN, enc_bw::SEEN),
		utf8bwpr(SHEEN, enc_bw::SHEEN),
		utf8bwpr(SAD, enc_bw::SAD),
		utf8bwpr(DAD, enc_bw::DAD),
		utf8bwpr(TAH, enc_bw::TAH),
		utf8bwpr(ZAH, enc_bw::ZAH),
		utf8bwpr(AIN, enc_bw::AIN),
		utf8bwpr(GHAIN, enc_bw::GHAIN),
		utf8bwpr(FEH, enc_bw::FEH),
		utf8bwpr(QAF, enc_bw::QAF),
		utf8bwpr(KAF, enc_bw::KAF),
		utf8bwpr(LAM, enc_bw::LAM),
		utf8bwpr(MEEM, enc_bw::MEEM),
		utf8bwpr(NOON, enc_bw::NOON),
		utf8bwpr(HEH, enc_bw::HEH),
		utf8bwpr(WAW, enc_bw::WAW),
		utf8bwpr(ALEF_MAKSURA, enc_bw::ALEF_MAKSURA),
		utf8bwpr(YEH, enc_bw::YEH),
		utf8bwpr(FATHATAN, enc_bw::FATHATAN),
		utf8bwpr(DAMMATAN, enc_bw::DAMMATAN),
		utf8bwpr(KASRATAN, enc_bw::KASRATAN),
		utf8bwpr(FATHA, enc_bw::FATHA),
		utf8bwpr(DAMMA, enc_bw::DAMMA),
		utf8bwpr(KASRA, enc_bw::KASRA),
		utf8bwpr(SHADDA, enc_bw::SHADDA),
		utf8bwpr(SUKUN, enc_bw::SUKUN),
		utf8bwpr(TATWEEL, enc_bw::TATWEEL)
	};

	/** Map of all arb_cp Unicode codepoints to Buckwalter transliteration */
	const static utf8bwmp UTF8_BW(utf8_bw_prs, utf8_bw_prs + sizeof(utf8_bw_prs) / sizeof(utf8_bw_prs[0]));

	/**
	 * <p>Check whether a string is a valid sequence of UTF-8 Arabic characters <em>for the purposes of Buckwalter transliteration</em>.
	 * If there is a single non-Arabic codepoint decoded from the UTF-8, or an invalid UTF-8 encoding. returns false.
	 * Whitespace should be stripped out first, as a whitespace character will test false.
	 * <p>Note that this test is based on all letters included in UTF8_BW; this includes some letters that are not actually MSA letters, such as PEH.
	 * Also note that it tests only for those letters that have a Buckwalter transliteration; it does <em>not</em> test every letter that is formally defined
	 * part of the Arabic Unicode set.
	 * @param str a UTF-8 encoded string
	 * @return true if str consists only of Arabic Unicode codepoints, false otherwise
	 */
	bool is_arabic(const string& str);

	/** Size of UTF-8 Byte-Order-Mark */
	const static unsigned int BOM_SIZE = 3;

	/**
	 * This checks a string for an encoding-specific Byte-Order-Mark at the beginning, and if it exists,
	 * strips the BOM from the beginning of the string.
	 * @param str string that may or may not begin with a BOM; BOM is stripped in-place
	 */
	void strip_bom(string& str);

	/**
	 * Convert a UTF-8 encoded string into a Buckwalter-transliterated string. Any non-Arabic characters will be appended unconverted. No diacritics are stripped.
	 * A flag is set indicating whether every codepoint in the UTF-8 string was Arabic. (See is_arabic()).
	 * @param word the UTF-8 word to convert
	 * @param is_arabic this flag will be reset: true if word is entirely Arabic, false if any non-Arabic codepoints are found
	 * @return the Buckwalter transliteration of word
	 */
	string utf8_to_bw(const string& word, bool& is_arabic);
}

#endif /* ENCODINGS_H_ */
