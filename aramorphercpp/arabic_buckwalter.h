/**
 * @file arabic_buckwalter.h
 * @brief Defines constants for encoding Arabic using Buckwalter transliteration.
 *
 *  Created on: Apr 18, 2009
 *      Author: David Edelstein
 */

#ifndef ARABIC_BUCKWALTER_H_
#define ARABIC_BUCKWALTER_H_

#include <string>
#include <boost/regex.hpp>

using std::string;

/**
 * \namespace enc_bw
 * \brief Definitions used to encode Arabic using Buckwalter transliteration.
 *
 * Defines all the Buckwalter transliteration symbols, and some regexes. All other encodings
 * should reference these constants to transform other encodings into the Buckwalter transliteration used by the BAMA dictionaries.
 */
namespace enc_bw{
	/** typedef for a letter using Buckwalter transliteration */
	typedef const char BW;

	/** All the letters recognized using Buckwalter transliteration */
	static BW ALEF_WASLA 		= '{';
	static BW VEH 				= 'V';
	static BW SUPERSCRIPT_ALEF = '`';
	static BW PEH 				= 'P';
	static BW JEH 				= '?';
	static BW TCHEH 			= 'J';
	static BW GAF 				= 'G';
	static BW HAMZA 			= '\'';
	static BW ALEF_W_MADDA 		= '|';
	static BW ALEF_UNDER_HAMZA	= '>';
	static BW WAW_UNDER_HAMZA 	= '&';
	static BW ALEF_OVER_HAMZA 	= '<';
	static BW YEH_UNDER_HAMZA 	= '}';
	static BW ALEF 				= 'A';
	static BW BEH 				= 'b';
	static BW TEH_MARBUTA 		= 'p';
	static BW TEH 				= 't';
	static BW THEH 				= 'v';
	static BW JEEM 				= 'j';
	static BW HAH 				= 'H';
	static BW KHAH 				= 'x';
	static BW DAL 				= 'd';
	static BW THAL 				= '*';
	static BW REH 				= 'r';
	static BW ZAIN 				= 'z';
	static BW SEEN 				= 's';
	static BW SHEEN 			= '$';
	static BW SAD 				= 'S';
	static BW DAD 				= 'D';
	static BW TAH 				= 'T';
	static BW ZAH 				= 'Z';
	static BW AIN 				= 'E';
	static BW GHAIN 			= 'g';
	static BW FEH 				= 'f';
	static BW QAF 				= 'q';
	static BW KAF 				= 'k';
	static BW LAM 				= 'l';
	static BW MEEM 				= 'm';
	static BW NOON 				= 'n';
	static BW HEH 				= 'h';
	static BW WAW 				= 'w';
	static BW ALEF_MAKSURA 		= 'Y';
	static BW YEH 				= 'y';
	static BW FATHATAN 			= 'F';
	static BW DAMMATAN 			= 'N';
	static BW KASRATAN	 		= 'K';
	static BW FATHA 			= 'a';
	static BW DAMMA 			= 'u';
	static BW KASRA 			= 'i';
	static BW SHADDA 			= '~';
	static BW SUKUN 			= 'o';
	static BW TATWEEL 			= '_';

	/** Diacritic symbols (in Buckwalter transliteration). Does not include HAMZA or TATWEEL. */
	const static string DIACRITICS = "auioFKN~";

	/** Special characters found in Arabic text: SUPERSCRIPT_ALEF, HAMZA, and TATWEEL. */
	const static string SPECIAL_CHARS = "`'_";

	/** Special Persio-Arabic characters: VEH, PEH, JEH, TCHEH, GAF*/
	const static string NON_ARABIC = "VPJG?";

	/** Every symbol in the Buckwalter transliteration scheme. */
	const static string ARABIC_LETTERS = "PJRG,;?'`|>&<{}AbptvjHxd*rzs$SDTZEg_fqklmnhwYyFNKaui~o";

	/** Regex that recognizes DIACRITICS, SPECIAL_CHARS, and NON_ARABIC, to be used in normalizing Arabic text
	 * (such as stripping diacritics and other non-MSA symbols). */
	const static boost::regex NORMALIZER_RGX("[" + DIACRITICS + SPECIAL_CHARS + NON_ARABIC + "]");

	/** used in replacement regex */
	const static string ALEF_WASLA_STR(1, ALEF_WASLA);
	/** used in replacement regex */
	const static string ALEF_STR(1, ALEF);


	/**
	 * <p>This is a normalizing function to process a Buckwalter transliteration before using it to look up an entry in the BAMA
	 * dictionaries.
	 *
	 * <p>Given a string of Buckwalter transliteration chars, strip all diacritic marks
	 * (short vowels, including tanweens). Also strips SPECIAL_CHARS and Persio-Arabic letters.
	*
	*  Also transforms Alef wasla ('{') into Alef ('A').
	 *
	 * <p>This method is for use in turning a diacritized word into its undiacritized "lookup word"
	 * in an AramorpherDictionary; it is not necessarily suitable as a general-purpose
	 * undiacritizing function.
	 *
	 * @param diacstr string to be normalized
	 */
	void normalize(string& diacstr);

	/**
	 * Test whether a word is Arabic by every character being found in the set of ARABIC_LETTERS. By default, also allows the NON_ARABIC
	 * letters. If strict is set to true, then Persio-Arabic letters will cause the word to return false.
	 * @param word word to test (encoded in Buckwalter transliteration)
	 * @param strict should NON_ARABIC (Persian letters) cause test to fail? (default is false)
	 * @return true if every letter in word is Arabic (subject to strict flag)
	 */
	bool is_arabic(const string& word, const bool strict = false);
}

#endif /* ARABIC_BUCKWALTER_H_ */
