/**
 * @file AramorpherDefs.h
 * @brief constants and typedefs used by Aramorpher.
 * @author David Edelstein
 * @date 2-18-09
 */
#ifndef ARAMORPHERDEFS_H_
#define ARAMORPHERDEFS_H_

#include <vector>
#include <string>
#include <map>
#include <set>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

using std::string;
using std::map;
using std::pair;
using std::iterator;
using std::set;
using std::vector;

#define VERBOSE if (verbose) cout

/**
 * \namespace Aramorph
 * \brief Definitions used in the C++ implementation of the Buckwalter Morphological Analyzer.
 */
namespace Aramorph {
	/** Capital English letters (used for guessing proper nouns) */
	const static string CAP_ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	/** Arabic noun-ending iy~  */
	const static string NOUN_YA_END = "iy~";
	/** Comment char used to begin comment lines in dict files */
	static const char COMMENT_CHAR = ';';
	/** Tab-delimiter constant (for tokenizing and segmenting) */
	const static string TAB_DELIMITER = "\t";

	/** vector of strings **/
	typedef vector<string> str_vec;
	/** iterator for a string vector **/
	typedef str_vec::iterator str_vec_i;
	/** const iterator for string vector **/
	typedef str_vec::const_iterator str_vec_ci;
	/** set of strings */
	typedef set<string> str_set;
	/** iterator for a string set **/
	typedef str_set::iterator str_setiter;
	/** Tokenizer from boost library */
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	/** constant iterator for tokenizer */
	typedef tokenizer::const_iterator token_ci;
	/** Used for a tokenizer on whitespace that drops whitespace tokens from output */
	const static boost::char_separator<char> DROP_WHITESPACE(" \t\f\v\n\r", "", boost::drop_empty_tokens);
	/** Used for a tokenizer on whitespace that keeps whitespace tokens from output */
	const static boost::char_separator<char> KEEP_WHITESPACE(" \t\f\v\n\r", "", boost::keep_empty_tokens);
	/** Used for a tokenizer on dict fields that keeps whitespace tokens from output */
	const static boost::char_separator<char> KEEP_TABS("\t", "", boost::keep_empty_tokens);

	/**
	 * \struct Solution
	 * \brief Contains values parsed from an Arabic morphological analysis
	 */
	struct Solution {
		/** the unique lemmaID associated with this solution's stem */
		string lemmaID;
		/** Complete fully diacritized string */
		string voc_str;
		/** Complete sequence of POS tags */
		string pos_str;
		/** English gloss for the complete word */
		string gloss_str;
	};

	/** A vector of Solution structs */
	typedef vector<Solution> sol_vec;
	/** A Solutions iterator */
	typedef sol_vec::iterator sol_vec_i;
	/** A Solutions const_iterator */
	typedef sol_vec::const_iterator sol_vec_ci;

	/** Map of alternative lookup words to Solution vectors, for ANALYSIS_T ALTSOL */
	typedef map<const string, sol_vec> AltSol;
	/** An AltSol pair */
	typedef std::pair<const string, sol_vec> AltSolPr;
	/** An AltSol const iterator */
	typedef AltSol::const_iterator AltSol_ci;

	/** Category assigned to an Analysis */
	enum ANALYSIS_T {
		/** The Analysis has one or more regular Solutions */
		SOL,
		/** The Analysis didn't render a regular Solution, but has one or more alternative Solutions */
		ALTSOL,
		/** Input word was non-Arabic text */
		NONALPHA,
		/** The Analysis found the input word to be Arabic, but no regular or alternative Solution was found */
		NOTFOUND,
		/** Assigned if the input string was not encoded in the expected encoding. */
		ENCODE_ERR
	};

	/**
	 * \struct Analysis
	 * \brief The results of analyzing an input word using the Aramorpher::analyze method.
	 * \details Assigns an ANALYSIS_T, the UTF-8 input string, the lookup word (in Buckwalter transliteration), and either a Solution vector
	 * or a set of orthographic variants for the input word mapped to Solution vectors for those alternatives.
	 */
	struct Analysis {
		ANALYSIS_T type;
		string input;
		string lookup;
		sol_vec solutions;
		AltSol altsolutions;
	};

	/**
	 * \struct AnalysisComp
	 * \brief Comparison operator for Analysis structs.
	 *
	 * Compares types, then input word. No comparison done with lookup, solutions, or altsolutions because these should be the same if input is the same.
	 */
	struct AnalysisComp {
		bool operator()(const Analysis& a1, const Analysis& a2) const {
			return (a1.type == a2.type) ? (a1.input < a2.input) : (a1.type < a2.type);
		}
	};

	/** Analysis vector */
	typedef vector<Analysis> an_vec;
	/** const iterator for an_vec */
	typedef an_vec::const_iterator an_vec_ci;
	/** Map of lookup words to Analyses */
	typedef map<const string, Analysis> an_map;
	/** Analysis set using AnalysisComp to compare Analysis */
	typedef set<Analysis, AnalysisComp> an_set;
	/** Constant iterator for Analysis set */
	typedef an_set::const_iterator an_set_ci;

	/**
	 * Generate a string from a Solution.
	 * @param sol the Solution to be stringified
	 * @return sol in string form
	 */
	inline string solution_str(const Solution &sol) {
		std::ostringstream ss;
		ss << "lemma: " << sol.lemmaID
		   << " vocalization: " << sol.voc_str
		   << " POS: " << sol.pos_str
		   << " gloss: " << sol.gloss_str;
		return ss.str();
	}

}

#endif
