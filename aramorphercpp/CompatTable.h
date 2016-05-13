/**
 * @file CompatTable.h
 * @brief Holds compatibility information between parts of an Arabic stem, loaded from Buckwalter tables.
 * @author David Edelstein
 * @date 2-25-09
 */
#ifndef _ARAMORPHER_COMPATTABLE_H_
#define _ARAMORPHER_COMPATTABLE_H_

#include <set>
#include "AramorpherDefs.h"
#include "AramorpherUtils.h"

using std::set;

namespace Aramorph {
/*********** Constants ******************/
/** Default name for Buckwalter prefix-to-stem compatibility table */
const static string PREFIX_STEM = "tableab";
/** Default name for Buckwalter prefix-to-suffix compatibility table */
const static string PREFIX_SUFFIX = "tableac";
/** Default name for Buckwalter stem-to-suffix compatibility table */
const static string STEM_SUFFIX = "tablebc";

/**
* \class CompatTable
* \brief Object that reads a BAMA compatibility table into memory so that prefixes/stems/suffixes can be compared.
 */
class CompatTable {

	/** Each compatibility table is a map, mapping the first string type to a set of strings falling into the
	 * second string type that are compatible with the first */
	typedef map<const string, set<string> > CompatMap;

private:
	/** name of the compat table file */
	string fn;
	/** Assign verbosity */
	bool verbose;
	/** mapping of each compatibility */
	CompatMap cmp;

	/** reads the named file into dictionary */
	void readFile();

public:
	/**
	* Default constructor. Reads a Buckwalter compatibility table and assumes tab delimiters for every entry.
	* Automatically converts upper-ASCII characters to lower ASCII (because of problems with UTF8 encoding).
	* \param filename file to read
	*/
	CompatTable(const string &filename);

	/**
	* Constructor that sets verbosity. Reads a Buckwalter compatibility table and assumes tab delimiters for every entry.
	* Automatically converts upper-ASCII characters to lower ASCII (because of problems with UTF8 encoding).
	* \param filename file to read
	* \param verbosity set whether to be verbose
	*/
	CompatTable(const string &filename, const bool &verbosity);

	/** stub destructor */
	~CompatTable();

	/**
	 * Returns the name of the file (including path) that was read into this CompatTable.
	 * @return
	 */
	string filename() const;

	/**
	 * Test whether the categories of two types of segments (taken from Entry structs in an AramorpherDictionary) are compatible.
	 * \param cat1
	 * \param cat2
	 * \return true if cat1 matches cat2
	 */
	bool matches(const string& cat1, const string& cat2) const;

	/**
	 * Return the set of matching labels that are compatible with the given label.
	 * @param str the label to match
	 * @return
	 */
	str_set getAllMatches(const string& str) const;
};

}

#endif /* _ARAMORPHER_COMPATTABLE_H_ */
