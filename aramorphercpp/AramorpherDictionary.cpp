/**
 * @file AramorpherDictionary.cpp
 * @brief Implementation file for AramorpherDictionary class.
 * @author David Edelstein
 * @date 2-18-09
 */

#include <sstream>
#include <boost/algorithm/string/trim.hpp>
#include "AramorpherDictionary.h"
#include "AramorpherUtils.h"
#include "arabic_buckwalter.h"

using namespace Aramorph;

using std::endl;
using std::cout;
using std::cerr;

AramorpherDictionary::AramorpherDictionary(const string &filename)
	:delim(TAB_DELIMITER),
	fn(filename),
	verbose(DEFAULT_VERBOSITY),
	conv_uascii(DEFAULT_CONVERT_UPPER_ASCII) {
	readFile();
}

AramorpherDictionary::AramorpherDictionary(const string &filename, const bool &verbosity)
	:delim(TAB_DELIMITER),
	fn(filename),
	verbose(verbosity),
	conv_uascii(DEFAULT_CONVERT_UPPER_ASCII) {
	readFile();
}

AramorpherDictionary::AramorpherDictionary(const string &filename, const bool &verbosity, const bool &uascii)
	:delim(TAB_DELIMITER),
	fn(filename),
	verbose(verbosity),
	conv_uascii(uascii) {
	readFile();
}

// stub destructor
AramorpherDictionary::~AramorpherDictionary() {}

AramorpherDictionary::EntrySet AramorpherDictionary::find_entry(const string& stem) const {
	// we look up by undiacritized form
	string lookup = stem;
	enc_bw::normalize(lookup);
	EntrySet entries;
	EntryMap::const_iterator iter;
	for (iter = lookups.lower_bound(lookup); iter != lookups.upper_bound(lookup); iter++) {
		entries.insert(iter->second);
	}
	return entries;
}

AramorpherDictionary::EntrySet AramorpherDictionary::find_diacritized_entry(const string& stem) const {
	// first get all those matching the undiacritized form
	EntrySet entries = find_entry(stem);
	// now return only those matching the diacritized form
	EntrySet matches;
	for (EntrySet::const_iterator iter = entries.begin(); iter != entries.end(); iter++) {
		if (iter->diac == stem) {
			matches.insert(*iter);
		}
	}
	return matches;
}

// read the filename given in constructor
// throws FNFException if file not read
void AramorpherDictionary::readFile() {
	ifstream df;
	// throws FNFException if not opened
	checkfile(df, fn);
	VERBOSE	<< "reading file: " << fn << endl;
	int lineno = 0;
	string line;
	// won't change unless reading stems
	string lemma = "";
	Lemma curr_lemma;
    while (getline(df, line)) {
    	lineno++;
    	if (line.at(0) == COMMENT_CHAR) {
			// ;; means start of a lemma
			if (line.length() > 2 && line.at(1) == COMMENT_CHAR) {
				if(lemma != "")
				{
					lemma_list.push_back(curr_lemma);
					curr_lemma = Lemma(lemma);
				}
				
				lemma = line.substr(2);
				boost::algorithm::trim(lemma);
       			if (!lemma.empty() && !lemmas.insert(lemma).second) {
       				std::cerr << "dictionary " << fn << " contains non-unique lemma " << lemma << endl;
       		        exit(EXIT_FAILURE);
       			}
    		} // else do nothing, it's just a comment line
    	} else {
    		tokenizer entries(line, KEEP_TABS);
    		EntryPair stempr = make_entry_pair(entries, lemma);
			if(lemma != "")
			{
				curr_lemma.entries.push_back(stempr.second);
			}
    		lookups.insert(stempr);
    	}
    }
	if(lemma != "")
	{
		lemma_list.push_back(curr_lemma);
		curr_lemma = Lemma();
	}
    if (verbose) {
        cout << "read " << lineno << " lines and " << lookups.size() << " entries";
        int lemmact = lemmas.size();
        if (lemmact > 0) {
        	cout << " (" << lemmact << " lemmas)";
        }
        cout << endl;
    }
	df.close();
}

AramorpherDictionary::EntryPair AramorpherDictionary::make_entry_pair(const tokenizer &segments, const string &lemma) const {
	Entry stemst;
	// this will be an empty string unless it's a stem word (not prefix/suffix)
	stemst.lemma = lemma;
	int i = 0;
	for(token_ci tok_iter = segments.begin(); tok_iter != segments.end(); ++tok_iter, ++i) {
		switch (i) {
			case 0:
				// the undiacritized stem
				stemst.stem = *tok_iter;
				break;
			case 1:
				// fully diacritized form
				stemst.diac = *tok_iter;
				break;
			case 2:
				// grammatical category
				stemst.cat = *tok_iter;
				break;
			case 3:
				// now parse and assign the POS and gloss fields
				addGlossPOS(stemst, *tok_iter);
				break;
			default:
				cerr << "lemma " << lemma << " has wrong number of tokens: " << (i+1) << endl;
				exit(EXIT_FAILURE);
		}
	}
	EntryPair lp = std::make_pair(stemst.stem, stemst);
	return lp;
}

// assumes cat, diac, and stem have already been assigned
void AramorpherDictionary::addGlossPOS(Entry &st, const string &glosspos) const {
	// parse both POS and gloss from this string
	st.pos = glosspos;
	st.gloss = glosspos;
	parsePOS(st.pos, st);
	if (st.pos.empty()) {
		std::cerr << "could not find POS from gloss-pos string " << glosspos << endl;
	    exit(EXIT_FAILURE);
	}
	parseGloss(st.gloss);
}

void AramorpherDictionary::parsePOS(string &pos, const Entry &st) const {
	// two ways to get the POS info:
	// look for explicit gloss field: <pos>...</pos>
	boost::match_results<std::string::const_iterator> what;
	if (boost::regex_match(pos, what, POS_RGX)) {
		pos = what.str(2);
	} else {
		//	# (2) by deduction:
		//	# use the cat (and sometimes the diac and gloss) to deduce the appropriate POS
		if (boost::starts_with(st.cat, NULL_PREF) || boost::starts_with(st.cat, NULL_SUFF)) {
			// null prefix or suffix
			pos = NULL_POS;
		} else {
			// check category label
			bool found = false;
			for (LKP_TBL_I iter = POS_MAP.begin(); iter != POS_MAP.end() && !found; iter++) {
				if (boost::starts_with(st.cat, iter->first)) {
					// we're done
					pos = st.diac + "/" + iter->second;
					found = true;
				}
			}
			// handle the other noun cases
			if (!found && boost::starts_with(st.cat, "N")) {
				// if gloss string begins with a capital Roman letter
				if (CAP_ALPHA.find(pos[0]) != string::npos) {
					// educated guess (99% correct)
					pos = st.diac + "/" + POS_NP;
				} else if (boost::ends_with(st.diac, NOUN_YA_END)) {
					pos = st.diac + "/" + POS_N; //(was NOUN_ADJ:
				} else {
					// some of these are really ADJ's and need to be tagged manually)
					pos = st.diac + "/" + POS_N;
				}
			}
		}
	}
}

void AramorpherDictionary::parseGloss(string &gloss) const {
	// Separate gloss information from the glossPOS field
	strip_pos(gloss);
    // remove trailing whitespace
	boost::algorithm::trim_right(gloss);
	// replace semicolon delimiters with slashes
	boost::replace_all(gloss, ";", "/");
	if (conv_uascii) {
		gloss = convertUpperAscii(gloss);
	}
}

void AramorpherDictionary::strip_pos(string &gloss) const {
	// Separate gloss information from the glossPOS field
	boost::match_results<std::string::const_iterator> what;
	if (boost::regex_match(gloss, what, POS_RGX)) {
		gloss = what.str(1);
	}
}

string AramorpherDictionary::convertUpperAscii(const string &str) const {
	std::ostringstream t(std::ios::out | std::ios::binary);
	std::ostream_iterator<char, char> oi(t);
	boost::regex_replace(oi, str.begin(), str.end(), UPPER_ASCII_RGX, LOWER_ASCII_FMT, boost::match_default | boost::format_all);
	string cvt = t.str();
	return cvt;
}

AramorpherDictionary::EntryMap::size_type AramorpherDictionary::size() const {
	return lookups.size();
}

AramorpherDictionary::LemmaSet::size_type AramorpherDictionary::lemma_count() const {
	return lemmas.size();
}

string AramorpherDictionary::filename() const {
	return fn;
}

void AramorpherDictionary::printOn(std::ostream& strm) const {
	strm << "Aramorph Dictionary: " << fn;
}
