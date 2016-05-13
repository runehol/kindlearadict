/**
 * @file CompatTable.cpp
 * @brief Implementation file for CompatTable class.
 *
 *  Created on: Apr 9, 2009
 *      Author: David Edelstein Edelstein
 */

//#include <algorithm>
#include <boost/algorithm/string.hpp>
#include "CompatTable.h"

using namespace Aramorph;
using namespace std;

CompatTable::CompatTable(const string &filename):fn(filename),verbose(false) {
	readFile();
}

CompatTable::CompatTable(const string &filename, const bool &v):fn(filename),verbose(v) {
	readFile();
}

// stub destructor
CompatTable::~CompatTable() {}

// get the filename
string CompatTable::filename() const {
	return fn;
}

bool CompatTable::matches(const string& cat1, const string &cat2) const {
	bool match = false;
	// every category that can be matched with cat1
	str_set matches = getAllMatches(cat1);
	match = matches.find(cat2) != matches.end();
	return match;
}

str_set CompatTable::getAllMatches(const string& str) const {
	str_set matches;
	CompatMap::const_iterator iter = cmp.find(str);
	if (iter != cmp.end()) {
		matches.insert(iter->second.begin(), iter->second.end());
	}
	return matches;
}

/** reads the named file into dictionary */
void CompatTable::readFile() {
	ifstream tf;
	// throws FNFException if not opened
	checkfile(tf, fn);
	VERBOSE << "reading file: " << fn << endl;
	int lineno = 0;
	string line;
    while (getline(tf, line)) {
    	lineno++;
    	// skip comments starting with ';'
    	if (line.at(0) != COMMENT_CHAR) {
    		boost::algorithm::trim_right(line);
    		tokenizer toks(line, DROP_WHITESPACE);
    		string entry;
    		string compatstr;
    		int i = 0;
    		for (token_ci iter = toks.begin(); iter != toks.end(); iter++, i++) {
				switch (i) {
					case 0:
						entry = *iter;
						break;
					case 1:
    					compatstr = *iter;
						break;
					default:
						cerr << "error in line " << lineno << ": " << line << " = " << (i+1) << " tokens " << endl;
						exit(EXIT_FAILURE);
				}
    		}
        	// does the first token already exist in the map?
        	CompatMap::iterator iter = cmp.find(entry);
        	// if not, put the second token as the first member of the set mapped to the first
        	if (iter == cmp.end()) {
        		set<string> compstrs;
        		compstrs.insert(compatstr);
        		std::pair<const string, set<string> > newpair = std::make_pair(entry, compstrs);
        		cmp.insert(newpair);
        	} else {
        		iter->second.insert(compatstr);
        	}
    	}
    }
    VERBOSE << "read " << lineno << " lines and " << cmp.size() << " entries" << endl;
	tf.close();
}
