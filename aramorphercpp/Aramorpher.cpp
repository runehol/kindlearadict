/**
 * @file Aramorpher.cpp
 * @brief Implementation file for the Aramorpher class.
 * @author David Edelstein
 * @date 2-18-09
 */
#include "Aramorpher.h"
#include "ArabicEncodings.h"
#include <boost/lexical_cast.hpp>

using namespace Aramorph;
using namespace enc_bw;
using std::endl;

// default constructor
Aramorpher::Aramorpher():path(""),verbose(false),enc(UTF8) {
	init();
}

// constructor with datapath, default encoding
Aramorpher::Aramorpher(const string& p):path(p),verbose(false),enc(UTF8) {
	init();
}

// constructor with verbosity
Aramorpher::Aramorpher(const bool& v):path(""),verbose(v),enc(UTF8) {
	init();
}

// constructor with datapath, default encoding, verbosity
Aramorpher::Aramorpher(const string& p, const bool& v):path(p),verbose(v),enc(UTF8) {
	init();
}

// constructor with datapath, encoding, verbosity
Aramorpher::Aramorpher(const string& p, const bool& v, const string& e):path(p),verbose(v) {
	setEncoding(e);
	init();
}

// destructor
Aramorpher::~Aramorpher() {
}

void Aramorpher::setEncoding(const string& encstr) {
	enc = getEncoding(encstr);
}

const Encoding Aramorpher::encoding() const {
	return enc;
}

const string Aramorpher::encoding_name() const {
	return enc_str(enc);
}

void Aramorpher::clear_analyses() {
	analyses.clear();
}

const bool Aramorpher::is_verbose() const {
	return verbose;
}

const string Aramorpher::datapath() const {
	return path;
}

int Aramorpher::analyze_file(const string& fn, std::ostream& strm, bool redundant) {
	ifstream f;
	// throws FNFException if not opened
	checkfile(f, fn);
    unsigned line_count = 0;
	string line;
	// used to prevent redundant output
	an_set analy_cache;
    // Read the first line of the file
	if (!getline(f, line)) {
		throw BadFileException(fn, "empty file");
	} else {
		// first line only -- remove BOM if it exists
		if (enc == UTF8) {
			enc_utf8::strip_bom(line);
		}
	    do {
	    	line_count++;
	    	an_vec analyses = analyze_text(line);
	    	for (an_vec_ci ai = analyses.begin(); ai != analyses.end(); ai++) {
	    		if (redundant) {
	    			// print everything
		    		printAnalysis(strm, *ai, line_count);
	    		} else {
	    			an_set_ci it = analy_cache.find(*ai);
    				// only gets printed the first time we see it, then add to cache
	    			if (it == analy_cache.end()) {
			    		printAnalysis(strm, *ai, line_count);
			    		analy_cache.insert(*ai);
	    			}
	    		}
	    	}
	    } while (getline(f, line));
	}
	f.close();
	return line_count;
}

an_vec Aramorpher::analyze_text(const string& text) {
	// first tokenize using whitespace
	tokenizer words(text, DROP_WHITESPACE);
	// and analyze each word
	an_vec analyses;
	for (token_ci iter = words.begin(); iter != words.end(); iter++) {
		Analysis analysis = analyze(*iter);
		analyses.push_back(analysis);
	}
	return analyses;
}

// get one or more solutions for a word
Analysis Aramorpher::analyze(const string& word) {
	Analysis analysis;
	// have we already looked up this word?
	an_map::const_iterator lookupit = analyses.find(word);
	if (lookupit != analyses.end()) {
		analysis = lookupit->second;
	} else {
		analysis.input = word;
		bool is_arabic;
		string lookup;
		// TODO: Encoding should be made a class, with virtual functions to do this conversion
		switch (enc) {
			case UTF8:
				lookup = enc_utf8::utf8_to_bw(word, is_arabic);
				break;
			case CP1256:
				lookup = enc_cp1256::cp1256_to_bw(word, is_arabic);
				break;
			case ISO_8859_6:
				lookup = enc_88596::iso88596_to_bw(word, is_arabic);
				break;
			case BUCKWALTER:
				lookup = word;
				is_arabic = enc_bw::is_arabic(word);
				break;
			default:
				// should never happen!
				throw EncodingException(enc_str(enc), "not recognized");
		}
		enc_bw::normalize(lookup);
		if (lookup.empty()) {
			analysis.type = ENCODE_ERR;
		// now find each one that is valid Arabic
		} else if (is_arabic) {
			analysis.lookup = lookup;
			sol_vec solutions;
			vector<segmentation> segmentations;
			segmentword(lookup, segmentations);
			for (vector<segmentation>::const_iterator iter = segmentations.begin(); iter != segmentations.end(); iter++) {
				analyze_segment(*iter, solutions);
			}
			if (solutions.empty()) {
				str_set alternatives = get_alternatives(lookup);
				if (alternatives.empty()) {
					analysis.type = NOTFOUND;
				} else {
					analysis.type = ALTSOL;
					for (str_setiter alti = alternatives.begin(); alti != alternatives.end(); alti++) {
						vector<segmentation> altsegs;
						sol_vec altsolutions;
						segmentword(*alti, altsegs);
						for (vector<segmentation>::const_iterator altiter = altsegs.begin(); altiter != altsegs.end(); altiter++) {
							analyze_segment(*altiter, altsolutions);
						}
						AltSolPr pr = std::make_pair(*alti, altsolutions);
						analysis.altsolutions.insert(pr);
					}
				}
			} else {
				analysis.type = SOL;
				analysis.solutions = solutions;
			}
		} else {
			analysis.type = NONALPHA;
		}
		// cache it for later lookup
		pair<const string, Analysis> lookuppr = std::make_pair(word, analysis);
		analyses.insert(lookuppr);
	}
	return analysis;
}

// test each possible segmentation of a word
void Aramorpher::analyze_segment(const segmentation &seg, sol_vec &solutions) const {
	string prefix = seg.at(0);
	AramorpherDictionary::EntrySet prefixes = find_prefix(prefix);
	if (!prefixes.empty()) {
		string suffix = seg[2];
		AramorpherDictionary::EntrySet suffixes = find_suffix(suffix);
		if (!suffixes.empty()) {
			string stem = seg[1];
			AramorpherDictionary::EntrySet stems = find_stem(stem);
			if (!stems.empty()) {
				// all entries exist, but are they compatible?
				// solutions vector is filled here
				check_compat(prefixes, stems, suffixes, solutions);
			}
		}
	}
}

str_set Aramorpher::get_alternatives(const string& word) {
	str_set alts;
	alts.insert(word);
	// check words ending with alif maqsura + hamza or yeh + hamza (y/Y)
	if (boost::iends_with(word, "y'")) {
		// replace all alif maqsuras with yehs
		string temp = boost::replace_all_copy(word, "Y", "y");
		alts.insert(temp);
		// replace waw + hamza with seated hamza over waw
		boost::replace_all(temp, "w'", "&");
		alts.insert(temp);
		// reset, redo alif maqsura -> yeh
		temp = boost::replace_all_copy(word, "Y", "y");
		// replace word-final yeh + hamza with seated hamza over alif
		if (boost::ends_with(temp, "y'")) {
			boost::replace_last(temp, "y'", "}");
		}
		alts.insert(temp);
		// again replace waw + hamza with seated hamza over waw
		boost::replace_all(temp, "w'", "&");
		alts.insert(temp);
	} else if (boost::ends_with(word, "Y")) {
		string temp = boost::replace_all_copy(word, "Y", "y");
		alts.insert(temp);
		boost::replace_all(temp, "w'", "&");
		alts.insert(temp);
	} else if (boost::ends_with(word, "y")) {
		// replace all alif maqsuras with yehs
		string temp = boost::replace_all_copy(word, "Y", "y");
		alts.insert(temp);
		// replace waw + hamza with seated hamza over waw
		boost::replace_all(temp, "w'", "&");
		alts.insert(temp);
		// reset, redo alif maqsura -> yeh
		temp = boost::replace_all_copy(word, "Y", "y");
		// replace word-final yeh with alif-maqsura
		if (replace_lastchar(temp, 'y', 'Y')) {
			alts.insert(temp);
		}
		// again replace waw + hamza with seated hamza over waw
		boost::replace_all(temp, "w'", "&");
		alts.insert(temp);
	} else if (boost::ends_with(word, "h") || boost::ends_with(word, "p")) {
		// replace all alif maqsuras with yehs
		string temp = boost::replace_all_copy(word, "Y", "y");
		alts.insert(temp);
		// replace waw + hamza with seated hamza over waw
		boost::replace_all(temp, "w'", "&");
		alts.insert(temp);
		char lastchar = word[word.length()-1];
		char altlast = lastchar == 'h' ? 'p' : 'h';
		replace_lastchar(temp, lastchar, altlast);
		alts.insert(temp);
	} else {
		// replace all alif maqsuras with yehs
		string temp = boost::replace_all_copy(word, "Y", "y");
		alts.insert(temp);
		// replace waw + hamza with seated hamza over waw
		boost::replace_all(temp, "w'", "&");
		alts.insert(temp);
	}
	// take original out of alts
	alts.erase(word);
	return alts;
}

// wrapper methods for lookup_entry
AramorpherDictionary::EntrySet Aramorpher::find_prefix(const string& prefix) const {
	return lookup_entry(DIC_NAMES[PREFIX], prefix);
}

AramorpherDictionary::EntrySet Aramorpher::find_stem(const string& stem) const {
	return lookup_entry(DIC_NAMES[STEM], stem);
}

AramorpherDictionary::EntrySet Aramorpher::find_suffix(const string& suffix) const {
	return lookup_entry(DIC_NAMES[SUFFIX], suffix);
}

AramorpherDictionary::EntrySet Aramorpher::lookup_entry(const string& dic, const string& lookup) const {
	return dictionaries.find(dic)->second.find_entry(lookup);
}

// this method actually creates Solutions and inserts them into the vector provided
void Aramorpher::check_compat(
		const AramorpherDictionary::EntrySet &prefixes,
		const AramorpherDictionary::EntrySet &stems,
		const AramorpherDictionary::EntrySet &suffixes,
		sol_vec &solutions) const {
	for (AramorpherDictionary::EntrySetConstIter prefi = prefixes.begin(); prefi != prefixes.end(); prefi++) {
		string pref_cat = prefi->cat;
		for (AramorpherDictionary::EntrySetConstIter stemi = stems.begin(); stemi != stems.end(); stemi++) {
			string stem_cat = stemi->cat;
			if (check_prefix_stem(pref_cat, stem_cat)) {
				for (AramorpherDictionary::EntrySetConstIter suffi = suffixes.begin(); suffi != suffixes.end(); suffi++) {
					string suff_cat = suffi->cat;
					if (check_prefix_suffix(pref_cat, suff_cat)) {
						if (check_stem_suffix(stem_cat, suff_cat)) {
							Solution sol;
							sol.lemmaID = stemi->lemma;
							sol.voc_str = prefi->diac + stemi->diac + suffi->diac;
							sol.pos_str = stemi->pos;
							if (!prefi->pos.empty()) {
								sol.pos_str = prefi->pos + " " + sol.pos_str;
							}
							if (!suffi->pos.empty()) {
								sol.pos_str = sol.pos_str + " " + suffi->pos;
							}
							sol.gloss_str = stemi->gloss;
							if (!prefi->gloss.empty()) {
								sol.gloss_str = prefi->gloss + " + " + sol.gloss_str;
							}
							if (!suffi->gloss.empty()) {
								sol.gloss_str = sol.gloss_str + " + " + suffi->gloss;
							}
							solutions.push_back(sol);
						}
					}
				}
			}
		}
	}
}

bool Aramorpher::check_prefix_suffix(const string& prefix, const string& suffix) const {
	return check_categories(TBL_NAMES[PREF_SUFF], prefix, suffix);
}

bool Aramorpher::check_prefix_stem(const string& prefix, const string& stem) const {
	return check_categories(TBL_NAMES[PREF_STEM], prefix, stem);
}

bool Aramorpher::check_stem_suffix(const string& stem, const string& suffix) const {
	return check_categories(TBL_NAMES[STEM_SUFF], stem, suffix);
}

bool Aramorpher::check_categories(const string& tbl, const string& cat1, const string& cat2) const {
	return comptables.find(tbl)->second.matches(cat1, cat2);
}

void Aramorpher::segmentword(const string &word, vector<segmentation> &segmentations) {
	int prefix_len = 0;
	int suffix_len = 0;
	int str_len = word.size();

	while (prefix_len <= MAX_PREFIX_LEN && prefix_len < str_len) {
		string prefix = word.substr(0, prefix_len);
	    int stem_len = str_len - prefix_len;
	    suffix_len = 0;
	    while ((stem_len >= 1) && (suffix_len <= MAX_SUFFIX_LEN)) {
	    	string stem = word.substr(prefix_len, stem_len);
	    	string suffix = word.substr(prefix_len+stem_len, suffix_len);
	        segmentation seg(3);
	        seg[0] = prefix;
	        seg[1] = stem;
	        seg[2] = suffix;
	        segmentations.push_back(seg);
	        stem_len--;
	        suffix_len++;
	    }
	    prefix_len++;
   }
}

// initializer calls all other initializing functions
void Aramorpher::init() {
//	initEncoding();
	setDatapath();
	initDictionaries();
	initCompatTables();
}

// adds path separator to the end unless we're using current directory
void Aramorpher::setDatapath() {
    if (!path.empty()) {
    	path += PATH_SEP;
    }
}

void Aramorpher::initDictionaries() {
	for (int i = 0; i < 3; i++) {
		string dicname = DIC_NAMES[i];
		string fn = path + dicname;
		AramorpherDictionary dic(fn, verbose, conv_uascii);
		dicname_pr pr = std::make_pair(dicname, dic);
		dictionaries.insert(pr);
	}
}

void Aramorpher::initCompatTables() {
	for (int i = 0; i < 3; i++) {
		string tblname = TBL_NAMES[i];
		string fn = path + tblname;
		CompatTable tbl(fn, verbose);
		tblname_pr pr = std::make_pair(tblname, tbl);
		comptables.insert(pr);
	}
}

void Aramorpher::printOn(std::ostream& strm) const {
	strm << "Aramorpher: datapath=" << path << ", encoding=" << encoding_name();
}

// static function
string Aramorpher::availableEncodings() {
	string encstr = "";
	str_vec encs = all_encodings();
	int len = encs.size();
	int i = 0;
	for (str_vec_ci it = encs.begin(); it != encs.end(); it++, i++) {
		encstr += *it;
		if (i < len-1) {
			encstr += ", ";
		}
	}
	return encstr;
}

// static function
void Aramorpher::printAnalysis(std::ostream& strm, const Analysis& an, const int& line) {
	strm << endl << "INPUT STRING: " << an.input << endl;
	int i;
	switch (an.type) {
		case SOL:
			strm << "LOOK-UP WORD: " << an.lookup << endl;
			i = 0;
			for (sol_vec_ci si = an.solutions.begin(); si != an.solutions.end(); si++) {
				strm << "  SOLUTION " << ++i << ": (" << si->voc_str << ") [" << si->lemmaID << "] " << si->pos_str << endl
					 << "     (GLOSS): " << si->gloss_str << endl;
			}
			break;
		case ALTSOL:
			strm << "LOOK-UP WORD: " << an.lookup << endl;
			strm << "\tComment: " << an.lookup << " NOT FOUND" << endl;
			for (AltSol_ci alti = an.altsolutions.begin(); alti != an.altsolutions.end(); alti++) {
				string alt = alti->first;
				sol_vec altsols = alti->second;
				strm << " ALTERNATIVE: " << alt << endl;
				if (altsols.empty()) {
					strm << "\tComment: " << alt << " NOT FOUND" << endl;
				} else {
					i = 0;
					for (sol_vec_ci altsoli = altsols.begin(); altsoli != altsols.end(); altsoli++) {
						strm << "  SOLUTION " << ++i << ": (" << altsoli->voc_str << ") [" << altsoli->lemmaID << "] " << altsoli->pos_str << endl
							 << "     (GLOSS): " << altsoli->gloss_str << endl;
					}
				}
			}
			break;
		case NOTFOUND:
			strm << "LOOK-UP WORD: " << an.lookup << endl;
			strm << "\tComment: " << an.lookup << " NOT FOUND" << endl;
			break;
		case NONALPHA:
			strm << "\tComment: Non-Alphabetic Data" << endl;
			break;
		case ENCODE_ERR:
			std::cerr << "invalid encoding in : " + an.input + " (line " + boost::lexical_cast<string>(line) + ")" << endl;
			break;
		default:
			// should never happen!
			std::cerr << "Line " << line << ": unrecognized Analysis type: " << an.type << endl;
			exit(EXIT_FAILURE);
	}
}
