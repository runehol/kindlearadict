/**
 * @file Aramorpher.h
 * @brief Buckwalter Morphological Analyzer, reimplemented in C++
 * @author David Edelstein
 * @date 9-30-09
 *
 * This is a modified version of the Buckwalter Morphological Analyzer (v. 1.0),
 * aramorph.pl, made available by the Linguistic Data Consortium (LDC),
 * and is released under the same GNU GPL.
 *
 * Portions (c) 2002 QAMUS LLC (www.qamus.org),
 * (c) 2002 Trustees of the University of Pennsylvania
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details (../gpl.txt).
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * You can contact LDC by sending electronic mail to: ldc@ldc.upenn.edu
 * or by writing to:
 *                Linguistic Data Consortium
 *                3600 Market Street
 *                Suite 810
 *                Philadelphia, PA, 19104-2653, USA.
 *
 * Aramorpher Modifications:
 *
 * David Edelstein (amadan@users.sourceforge.net)
 * Note that this file contains both the main Aramorpher class, and the main function to create a stand-alone application.
 */
#ifndef ARAMORPHER_CLASS_H_
#define ARAMORPHER_CLASS_H_

#include <stdlib.h>
#include "AramorpherDefs.h"
#include "ArabicEncodings.h"
#include "AramorpherDictionary.h"
#include "AramorpherUtils.h"
#include "CompatTable.h"

namespace Aramorph {

/** path separator */
const static string PATH_SEP = "/";
/** Index to DIC_NAMES array */
enum DICTIONARY {PREFIX, STEM, SUFFIX};
/** Used as keys in the dictionary and compattable maps below */
const static string DIC_NAMES[3] = {PREFIX_DICTIONARY, STEM_DICTIONARY, SUFFIX_DICTIONARY};
/** Index to TBL_NAMES array */
enum COMPATIBILITY_TABLE {PREF_STEM, STEM_SUFF, PREF_SUFF};
const static string TBL_NAMES[3] = {PREFIX_STEM, STEM_SUFFIX, PREFIX_SUFFIX};
/** Maximum number of characters in a concatenated string of Arabic prefixes */
const static int MAX_PREFIX_LEN = 4;
/** Maximum number of characters in a concatenated string of Arabic suffixes */
const static int MAX_SUFFIX_LEN = 6;
/** Default encoding */
const static Encoding DEF_ENC = UTF8;
/**
 * \class Aramorpher
 * \brief A Buckwalter Arabic Morphological Analyzer object
 *
 * An Aramorpher object is initialized with the path to the data directory containing the Buckwalter data files.
 * It reads all the dictionaries and compatibility tables, and can then be queried for the morphological analysis of a given word.
 * Aramorpher assumes that input and output files are to be encoded in utf-8.
 */
class Aramorpher {
	private:
		typedef std::pair<const string, AramorpherDictionary> dicname_pr;
		typedef std::pair<const string, CompatTable> tblname_pr;
		// used internally for segmentations of words, should be guaranteed to be 3 members
		typedef vector<string> segmentation;

		/** path to data dictionaries */
		string path;
		map<const string, AramorpherDictionary> dictionaries;
		map<const string, CompatTable> comptables;
		/** Store analyses for each word, to avoid repeat lookups */
		an_map analyses;
		/** Is this a verbose Aramorpher? */
		bool verbose;
		/** should dictionaries automatically convert upper-ascii chars to lower? */
		bool conv_uascii;
		/** Current encoding for the Aramorpher. */
		Encoding enc;

		/**
		 * Initializer, calls all the other init functions.
		 */
		void init();

		/**
		 * Assigns value to global variable, adding path separator at end unless it's the current directory.
		 */
		void setDatapath();

		/**
		 * Assumes datapath has been set; initializes internal lexical dictionaries.
		 */
		void initDictionaries();

		/**
		 * Assumes datapath has been set; initializes internal compatibility tables.
		 */
		void initCompatTables();

		/**
		 * Given a word, segment it and place all possible segmentations into the provided vector.
		 * \param word word to segment
		 * \param segmentations a segmentation vector which will be filled with every possible segmentation of word
		 */
		static void segmentword(const string &word, vector<segmentation> &segmentations);

		/**
		 * Analyzes one segmentation to find solutions. Any solutions for the segmentation will be placed in the solutions vector.
		 * \param seg a segmentation vector (produced by the segmentword method)
		 * \param solutions a Solution vector into which any Solutions for seg will be inserted
		 */
		void analyze_segment(const segmentation &seg, sol_vec &solutions) const;

		/**
		 * Given a set of Entries for prefix, stem, and suffix, add all compatible solutions to the provided vector.
		 * @param prefixes
		 * @param stems
		 * @param suffixes
		 * @param solutions Solutions are put in this vector
		 */
		void check_compat(
				const AramorpherDictionary::EntrySet &prefixes,
				const AramorpherDictionary::EntrySet &stems,
				const AramorpherDictionary::EntrySet &suffixes,
				sol_vec &solutions) const;

		/** wrapper for check_compat */
		bool check_prefix_suffix(const string& prefix, const string& suffix) const;
		/** wrapper for check_compat */
		bool check_prefix_stem(const string& prefix, const string& stem) const;
		/** wrapper for check_compat */
		bool check_stem_suffix(const string& stem, const string& suffix) const;

		/**
		 * \param tbl name of one of the compat tables (one of our constants)
		 * \param cat1 first category to check (from an Entry struct)
		 * \param cat2 second category to chec (from an Entry struct)
		 * \return true if the two categories match in the given table
		 */
		bool check_categories(const string& tbl, const string& cat1, const string& cat2) const;

		/**
		 * Lookup the entry in a named dictionary -- should be prechecked that dic name is one of the dicname constants.
		 */
		AramorpherDictionary::EntrySet lookup_entry(const string& dic, const string& lookup) const;

	public:
		/**
		 * Default constructor. Assumes that the current directory contains the BAMA data files,
		 * @throws EncodingException if the encoding used is not available on this system
		 * @throws FNFException if any of the Buckwalter data files can't be opened
		 */
		Aramorpher();

		/**
		 * Constructor that assumes that the current directory contains the BAMA data files,
		 * @param v flag indicating whether to print verbose operation messages
		 * @throws EncodingException if the encoding used is not available on this system
		 * @throws FNFException if any of the BAMA data files can't be opened
		 */
		Aramorpher(const bool& v);

		/**
		 * Constructor specifying the path to the directory containing the BAMA data files.
		 * @param datapath path to BAMA data
		 * @throws FNFException if any of the BAMA data files can't be opened
		 */
		Aramorpher(const string& datapath);

		/**
		 * Constructor specifying the path to the directory containing the BAMA data files.
		 * @param datapath path to BAMA data
		 * @param v flag indicating whether to print verbose operation messages
		 * @throws FNFException if any of the BAMA data files can't be opened
		 */
		Aramorpher(const string& datapath, const bool& v);

		/**
		 * Constructor specifying the path to the directory containing the BAMA data files and an encoding.
		 * @param datapath path to BAMA data
		 * @param v flag indicating whether to print verbose operation messages
		 * @param e name of Encoding to use
		 * @throws FNFException if any of the BAMA data files can't be opened
		 * @throws EncodingException if e is not recognized
		 */
		Aramorpher(const string& datapath, const bool& v, const string& e);

		/**
		 * Destructor.
		 */
		~Aramorpher();

		/**
		 * Set the encoding the Aramorpher reads from input. Will recognize case-insensitive names and regexes.
		 * @param encstr name of encoding to use
		 * @throws EncodingException if the encoding used is not available on this system
		 */
		void setEncoding(const string& encstr);

		/**
		 * Query whether this Aramorpher has been set to Verbose output or not.
		 * @return true if it is Verbose, false if not
		 */
		const bool is_verbose() const;

		/**
		 * Get the current Encoding used by Aramorpher.
		 * @return Encoding setting.
		 */
		const Encoding encoding() const;

		/**
		 * Get the name of the current Encoding used by Aramorpher. (Convenience method to convert Encoding to string.)
		 * @return string representing the name of the encoding currently used
		 */
		const string encoding_name() const;

		/**
		 * Return the datapath from which this Aramorpher read the Buckwalter libraries.
		 * @return path to Buckwalter data
		 */
		const string datapath() const;

		/**
		 * <p>Perform a full morphological analysis on the named file. Outputs to an ostream (either STDOUT or a file)
		 * <p>The redundant flag indicates whether to generate redundant analyses for words already seen. If set to true, then the Analysis of
		 * every word will be output every time it is encountered (producing many, many redundant entries in the output, and much larger output files).
		 * If false, then each Analysis will only be output once (the first time the word is encountered).
		 * @param fn name of file to read
		 * @param strm the stream to write the results to (either a filestream, or cout)
		 * @param redundant should every entry be output as many times as it appears? (default = true)
		 * @return number of lines read
		 * @throw FNFException if file couldn't be read or was empty
		 */
		int analyze_file(const string& fn, std::ostream& strm, bool redundant = true);

		/**
		 * Given a UTF-8 encoded string, tokenize it and perform a morphological analysis on each token found.
		 * Basically a wrapper function that tokenizes the text (using whitespace as a delimiter) and then runs the
		 * #analyze method on each token.
		 * \param text to be analyzed
		 * \return an Analysis vector
		 */
		an_vec analyze_text(const string& text);

		/**
		 * <p>This is the method for analyzing individual words. The word is segmented into every possible prefix+stem+suffix combination,
		 * and each combination is then checked for compatibility. The string must be encoded in UTF-8. If a non-Arabic character is found (including whitespace), then
		 * depending on the option setting, it will either return a NON_ARABIC analysis, or further tokenize it into
		 * Arabic sequences and try to analyze each one.
		 * <p>This method will <em>cache</em> every word analyzed, so that future look-ups of the same word will return the previously generated Analysis. This increases memory size but greatly speeds performance, and
		 * will cause an Aramorpher to perform faster after it has already processed a significant number of words. In the event that you need to clear the cache of stored Analyses (which should only be necessary if you are running
		 * low on memory or are dynamically reloading a new dictionary and want all words to be reanalyzed), used the clear_analyses method.
		 * @param word to be analyzed
		 * @return an Analysis which contains the input string, lookup word, and Solutions or alternative Solutions for the lookup
		 */
		Analysis analyze(const string& word);

		/**
		 * <p>Find possible alternatives for a word by looking for orthographic variations. These are common orthographic variations caused
		 * by spelling errors/typos; they do not necessarily produce valid Arabic words! Aramorpher automatically uses this method to generate
		 * alternatives to words for which no solution could be produced; each alternative is then analyzed for possible solutions.
		 * <p>The following variations are generated:
		 * <ul>
		 * <li>waw followed by hamza is replaced with a seated hamza over a waw</li>
		 * <li>alif maqsura is replaced with a yeh</li>
		 * <li>word-final yeh is replaced by alif maqsura</li>
		 * <li>word-final hehs are replaced with teh marbutas, and vice versa</li>
		 * </ul>
		 * @param word to be analyzed (in Buckwalter format)
		 * @return a set of alternative strings; empty if no alternatives found
		 */
		static str_set get_alternatives(const string& word);

		/**
		 * Look up a prefix in this Aramorpher's prefix dictionary. The lookup word should be in Buckwalter transliteration,
		 * and may be diacritized or undiacritized, but vowels/diacritics will be stripped for lookup purposes. Returns a set
		 * of dictionary entries matching the lookup word, which will be empty if it was not found,
		 * \param prefix the prefix to look up
		 * \return EntrySet matching the prefix (may be empty)
		 */
		AramorpherDictionary::EntrySet find_prefix(const string& prefix) const;

		/**
		 * Look up a stem in this Aramorpher's stem dictionary. The lookup word should be in Buckwalter transliteration,
		 * and may be diacritized or undiacritized, but vowels/diacritics will be stripped for lookup purposes. Returns a set
		 * of dictionary entries matching the lookup word, which will be empty if it was not found,
		 * \param stem the stem to look up
		 * \return EntrySet matching the stem (may be empty)
		 */
		AramorpherDictionary::EntrySet find_stem(const string& stem) const;

		/**
		 * Look up a suffix in this Aramorpher's stem dictionary. The lookup word should be in Buckwalter transliteration,
		 * and may be diacritized or undiacritized, but vowels/diacritics will be stripped for lookup purposes. Returns a set
		 * of dictionary entries matching the lookup word, which will be empty if it was not found,
		 * \param suffix the suffix to look up
		 * \return EntrySet matching the suffix (may be empty)
		 */
		AramorpherDictionary::EntrySet find_suffix(const string& suffix) const;

		/**
		 * This method allows access to private variables for use in global << operators to serialize an Aramorpher to an ostream.
		 * @param strm the output stream in which this Aramorpher is to be serialized
		 */
		void printOn(std::ostream& strm) const;

		/**
		 * Normally, Aramorpher keeps a list of all words it has looked up previously mapped to each word's analyses, to avoid repeat lookups.
		 * If for some reason you want to clear the analyses stored by Amorpher (for example, to free memory usage, or because you are dynamically reloading new dictionaries),
		 * this method will do that. You shouldn't normally need this.
		 */
		void clear_analyses();

		/**
		 * Get a list of available ENCODINGs that Aramorpher can process. Returns a comma-delimited string.
		 * @return string listing all available encodings
		 */
		static string availableEncodings();

		/**
		 * Method to print an Analysis to an output stream, whether STDOUT or a file. The format is designed to be identical to what the BAMA Version 1.0 script outputs.
		 * The Analysis will be serialized and printed to the ostream.
		 * @param os ostream to print to (either STDOUT or a file stream)
		 * @param an the Analysis to be printed
		 * @param line number from which Analysis was parsed
		 */
		static void printAnalysis(std::ostream& os, const Analysis& an, const int& line);
};


} // namespace

/**
 * Serializes Aramorpher to a stream, allowing it to be output with the << operator.
 * @param strm an ostream to serialize into
 * @param aram Aramorpher to output
 * @return the ostream with aram's serialization added to it
 */
inline
std::ostream& operator << (std::ostream& strm, const Aramorph::Aramorpher& aram) {
	aram.printOn(strm);
	return strm;
}

#endif /* ARAMORPHER_CLASS_H_ */
