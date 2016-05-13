/**
 * @file AramorpherDictionary.h
 * @brief Hold stems, prefixes, or suffixes and complete analysis, read from a Buckwalter data dictionary.
 * @author David Edelstein
 * @date 2-18-09
 */
#ifndef ARAMORPH_DICTIONARY_H
#define ARAMORPH_DICTIONARY_H

#include <set>
#include <boost/algorithm/string_regex.hpp>
#include "AramorpherDefs.h"

using std::set;
using std::fstream;

namespace Aramorph {

/*********** Constants ******************/
/** Default name for Buckwalter Dictionary of stems */
const static string STEM_DICTIONARY = "dictstems.txt";
/** Default name for Buckwalter Dictionary of prefixes */
const static string PREFIX_DICTIONARY = "dictprefixes.txt";
/** Default name for Buckwalter Dictionary of suffixes */
const static string SUFFIX_DICTIONARY = "dictsuffixes.txt";

/** Key for POS F (FUNC WORD) */
const static string POS_F = "FUNC_WORD";
/** Key for POS IV (VERB IMPERFECT) */
const static string POS_IV = "VERB_IMPERFECT";
/** Key for POS PV (VERB PERFECT) */
const static string POS_PV = "VERB_PERFECT";
/** Key for POS CV (VERB IMPERATIVE) */
const static string POS_CV = "VERB_IMPERATIVE";
/** Key for POS Nprop (PROPER NOUN) */
const static string POS_NP = "NOUN_PROP";
/** Key for POS N (NOUN) */
const static string POS_N = "NOUN";

/** Map used as a lookup table. */
typedef map<const string, string> LKP_TBL;
/** Pairs in lookup table */
typedef pair<const string, string> LKP_PR;
/** Lookup table iterator */
typedef LKP_TBL::const_iterator LKP_TBL_I;

/**
 * Pairs used in POS_MAP, mapping a morphological label (or its prefix) to the grammatical category
 */
const static LKP_PR posprs[] = {
	LKP_PR("F", POS_F),
	LKP_PR("IV", POS_IV),
	LKP_PR("PV", POS_PV),
	LKP_PR("CV", POS_CV),
	LKP_PR("Nprop", POS_NP),
	// DO NOT INCLUDE -- don't want to confuse with NP, handled separately in getPOS
//	pair<string, string>("N", N)
};

/**
 * A map of morphological label prefixes to grammatical categories. The third field in a Buckwalter dictionary
 * contains a string that can be checked against the keys in this map to determine its category.
 */
const static LKP_TBL POS_MAP(posprs, posprs + sizeof(posprs) / sizeof(posprs[0]));

/** Pattern for recognizing upper ASCII character sets to be replaced with LOWER_ASCII_FMT */
const static string UPPER_ASCII_PAT = "([¿¡¬√ƒ≈])|(«)|([»… À])|([ÃÕŒœ])|(—)|([“”‘’÷])|([Ÿ⁄€‹])|([‡·‚„‰Â])|(Á)|([ËÈÍÎ])|([ÏÌÓÔ])|(Ò)|([ÚÛÙıˆ])|([˘˙˚¸])|(∆)|(ä)|(é)|(ﬂ)|(Ê)|(ö)|(û)";
/** Formatting to use to replace UPPER_ASCII_PAT */
const static string LOWER_ASCII_FMT = "(?1A)(?2C)(?3E)(?4I)(?5N)(?6O)(?7U)(?8a)(?9c)(?10e)(?11i)(?12n)(?13o)(?14u)(?15AE)(?16Sh)(?17Zh)(?18ss)(?19ae)(?20sh)(?21zh)";
/** Regex to replace UPPER_ASCII_PAT with LOWER_ASCII_FMT */
const static boost::regex UPPER_ASCII_RGX(UPPER_ASCII_PAT);

/** To recognize POS tags: on a match, gloss is captured in group 1 (regex_match).str(1)), and POS (between tags) is captured in group 2 */
const static boost::regex POS_RGX("(.*)<pos>(.*)</pos>.*");

/** Used as POS label for a null prefix or suffix */
const static string NULL_POS = " ";
/** Null Prefix */
const static string NULL_PREF = "Pref-0";
/** Null Suffix */
const static string NULL_SUFF = "Suff-0";

/**
* \class AramorpherDictionary
* \brief Object that reads a BAMA dictionary file into memory and allows queries.
*/
class AramorpherDictionary {

public:

	/**
	 * \struct Entry
	 * \brief Each lookup word is associated with a Entry in an AramorpherDictionary.
	 *
	 * A Entry contains the unique lemma assigned in a Buckwalter dictionary (which will be
	 * empty for prefixes and suffixes), the undiacritized "stem" lookup, the fully diacritized form, the
	 * grammatical category, and the English gloss. Entry can be sorted using EntryComp, which
	 * compares two StemEntries using the undiacritized stem first, then compares lemmas, and if those are
	 * equal, compares the fully diacritized forms.
	 */
	struct Entry {
		/** Unique Buckwalter-assigned lemma string (only stem words have these) */
		string lemma;
		/** the undiacritized stem (this is the "lookup word") */
		string stem;
		/** The fully diacritized entry */
		string diac;
		/** The grammatical category of this entry */
		string cat;
		/** The part-of-speech label for this entry */
		string pos;
		/** The English gloss for this entry */
		string gloss;
	};

	/**
	 * \struct EntryComp
	 * \brief Comparison operator for Entry structs.
	 *
	 * Compares by alphabetical order, in order, the undiacritized stem, then the lemma entry, then the fully diacritized form.
	 */
	struct EntryComp {
		bool operator()(const Entry& s1, const Entry& s2) const {
			return s1.stem == s2.stem ?
					(s1.lemma == s2.lemma ? s1.diac < s2.diac : s1.lemma < s2.lemma) :
						s1.stem < s2.stem;
		}
	};

	/** Map of stems with their info */
	typedef std::multimap<const string, Entry > EntryMap;
	/** Set of Entries, using EntryComp to sort. */
	typedef set<Entry, EntryComp> EntrySet;
	/** Constant iterator for an EntrySet */
	typedef EntrySet::const_iterator EntrySetConstIter;

	/** Entry in a EntryMap */
	typedef std::pair<const string, Entry > EntryPair;

private:
	/** Unique set of lemmas */
	typedef set<string> LemmaSet;
	/** Default setting for converting upper ASCII chars to lower ASCII */
	const static bool DEFAULT_CONVERT_UPPER_ASCII = true;
	/** Default setting for verbosity */
	const static bool DEFAULT_VERBOSITY = false;

	/** string used as delimiter in segmenting dictionary entries when reading */
	const string delim;
	/** name of the dictionary file */
	string fn;
	/** verbose output */
	bool verbose;
	/** set option to convert upper ASCII characters to lower */
	bool conv_uascii;
	/** Unique lemmas */
	LemmaSet lemmas;
	/** map of lookups to their entries */
	EntryMap lookups;

	/** reads the named file into dictionary */
	void readFile();
	/** Turn a tokenizer into an Entry and return a pair mapping the entry */
	EntryPair make_entry_pair(const tokenizer &segments, const string &lemma) const;
	/** Add gloss and POS to Entry from the fourth field in dictionary */
	void addGlossPOS(Entry &st, const string &glosspos) const;
	/** Assign the POS to string, or else exit fail */
	void parsePOS(string &pos, const Entry &st) const;
	/** Assign the gloss to string, or else exit fail */
	void parseGloss(string &gloss) const;
	/** Strip &lt;pos&gt;&lt;/pos&gt; tags and everything in between from a pos-gloss string */
	void strip_pos(string &gloss) const;
	/** Do conversion in a string of upper ASCII chars into lower ASCII */
	string convertUpperAscii(const string &str) const;

public:
	/**
	* Default constructor. Reads a Buckwalter dictionary and assumes tab delimiters for every entry.
	* Automatically converts upper-ASCII characters to lower ASCII (because of problems with UTF8 encoding).
	* \param filename file to read
	*/
	AramorpherDictionary(const string &filename);

	/**
	* Constructor that reads a Buckwalter dictionary and assumes tab delimiters for every entry.
	* \param filename file to read
	* \param verbosity true if Dictionary should be verbose
	*/
	AramorpherDictionary(const string &filename, const bool &verbosity);

	/**
	* Constructor that reads a Buckwalter dictionary and assumes tab delimiters for every entry, and allows verbosity to be set.
	* \param filename file to read
	* \param verbosity true if Dictionary should be verbose
	* \param uascii set whether or not upper-ASCII characters should be converted to lower ASCII
	*/
	AramorpherDictionary(const string &filename, const bool &verbosity, const bool &uascii);

	/**
	 * Default destructor.
	 */
	~AramorpherDictionary();

	/**
	 * Look up a Entry by a stem word. Note that the stem provided may be diacritized or undiacritized,
	 * but its undiacritized form will be the one used for dictionary lookups. If you want to look up an entry
	 * by fully diacritized form, use find_diacritized_form instead.
	 * @param stem the stem word (which will be converted to undiacritized form) to look up
	 * @return a EntrySet of every Entry in this dictionary for the given stem
	 */
	EntrySet find_entry(const string& stem) const;

	/**
	 * Look up a Entry by its fully diacritized form (disambiguating it from other stems which may be identical except
	 * for the vowelizations).
	 * @param stem the stem word to look up
	 * @return a EntrySet of every Entry in this dictionary that has that specific diacritization
	 */
	EntrySet find_diacritized_entry(const string& stem) const;

	/**
	 * Get the size of this dictionary. This is the total number of undiacritized lookup words that have
	 * associated Entry. Remember that each lookup word may have more than one entry (and thus will
	 * be counted more than once).
	 * @return number of entries
	 */
	EntryMap::size_type size() const;

	/**
	 * Number of <i>unique</i> lemmas in this dictionary.
	 * @return number of lemmas
	 */
	LemmaSet::size_type lemma_count() const;

	/**
	 * Return the filename (including path) that was read into this dictionary.
	 * @return filename for this dictionary
	 */
	string filename() const;


	/**
	 * This method allows access to private variables for use in global << operators to serialize a AramorpherDictionary to an ostream.
	 * @param strm the output stream in which this AramorpherDictionary is to be serialized
	 */
	void printOn(std::ostream& strm) const;

};

} // namespace

/**
 * Serializes AramorpherDictionary to a stream, allowing it to be output with the << operator.
 * @param strm an ostream to serialize into
 * @param d AramorpherDictionary to output
 * @return the ostream with d's serialization added to it
 */
inline
std::ostream& operator << (std::ostream& strm, const Aramorph::AramorpherDictionary& d) {
	d.printOn(strm);
	return strm;
}

#endif
