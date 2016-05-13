/**
 * @file AramorpherApp.cpp
 * @brief Contains the main function for creating a stand-alone Aramorpher application that duplicates the Perl script aramorpher.pl.
 * @date Sep 22, 2009
 * @author David Edelstein
 */

#include "Aramorpher.h"

#ifdef WIN32
	extern "C" {
		#include "getopt.h"
	}
#else
	#include "unistd.h"
#endif

using namespace Aramorph;
using namespace std;

// verbose macro
#define VERBOSE if (verbose) cout

// non-class functions, used by main
void parseOpts(int *argc, char* argv[]);
void print_usage();

/** GLOBAL VARIABLES */
bool verbose = false;
bool abbreviated = false;
// set in main if the option is defined
string datapath = "";
// set in main if the option is defined
string encoding = "UTF-8";

/**
 * <p>This allows Aramorpher to be run as a stand-alone application.
 * <p>Command: Aramorpher -[options] &lt;input file&gt; &lt;output file&gt;
 * <p>Output file is optional: if not specified, then output is written to STDOUT.
 * <p><b>Options:</b>
 * <ul>
 * <li><b>-a</b>: abbreviated output. By default, every word encountered in the input file will have its full Analysis printed out
 * each and every time it is encountered. The -a option sets Aramorpher's <i>redundant</i> flag to false, so each unique word's Analysis is only
 * output once.
 * <li><b>-e &lt;encoding&gt;</b>: select the input file's encoding. Default is UTF-8. Recognizes most case-insensitive variations. (E.g., "UTF-8", "utf8", "cp1256", "win1256", etc.)
 * <li><b>-d &lt;data directory&gt;</b>: specify where to find the BAMA data files. (Default is current directory.)
 * <li><b>-h</b>: Help. Prints these options and exits.
 * <li><b>-v</b>: Verbose output. Prints information to STDOUT about file i/o, how many analyses have been performed, etc.
 * </ul>
 * <p>An Aramorpher object is initialized with the path to the data directory containing the BAMA data files.
 * <p>It reads all the dictionaries and compatibility tables, and can then be queried for the morphological analysis of a given word.
 * <p>Aramorpher assumes that input and output files are to be encoded in UTF-8.
 * @return EXIT_SUCCESS if application loads and runs, else EXIT_FAILURE
 */
int main(int argc, char* argv[]) {
	// track advancement past opts
	int oldargc = argc;
	parseOpts(&argc, argv);
	std::ostream *os;
	if (argc < 1) {
		cerr << "no input file!" << endl;
		print_usage();
	} else if (argc > 2) {
		cerr << "wrong number of arguments" << endl;
		print_usage();
	}
	int argstart = oldargc - argc;
	string fn = argv[argstart];
	ofstream of;
	string outfn;
	if (argc == 2) {
		outfn = argv[argstart+1];
		checkfile(of, outfn);
		os = &of;
	} else {
		os = &cout;
	}
	// read the dictionaries
	try {
		Aramorpher aram(datapath, verbose, encoding);
		VERBOSE << "created " << aram << endl;
		VERBOSE << "reading from file " << fn << endl;
		if (of.is_open()) {
			VERBOSE << "writing to file " << outfn << endl;
		} else {
			VERBOSE << "writing to STDOUT" << endl;
		}
		int line_count = aram.analyze_file(fn, *os, !abbreviated);
		VERBOSE << "read " << line_count << " lines from " << fn << endl;
	} catch (const EncodingException& enc) {
		cerr << "unrecognized encoding: " << enc.what() << endl;
		cerr << "available encodings: " << Aramorpher::availableEncodings() << endl;
		return EXIT_FAILURE;
	} catch (const BadFileException& fnf) {
		cerr << "Morphological analysis failed: " << fnf.what() << endl;
		return EXIT_FAILURE;
	}
	if (of.is_open()) {
		of.close();
	}
	return EXIT_SUCCESS;
}

/** parse command-line options */
void parseOpts(int *argc, char* argv[]) {
	// vars for opt processing from unistd.h
	int c;
	extern char *optarg;
    extern int /*optopt,*/ optind;
    while ((c = getopt(*argc, argv, "ad:e:hv")) != -1) {
    	switch (c) {
			case 'a':
				abbreviated = true;
				break;
    		case 'd': // set datapath
    			datapath = optarg;
    			break;
    		case 'e': // set encoding
    			encoding = optarg;
    			break;
    		case 'h': // help and exit
    			print_usage();
    			break;
    		case 'v': // set verbose output
    			verbose = true;
    			break;
    	}
    }
    // advance index of argc past options
    *argc -= optind;
}

/**
 * Invoked by -h option; prints usage message and exits.
 */
void print_usage() {
	cout << "Aramorpher [-ae:d:hv] input_file <output file>" << endl
		 << "\t-a abbreviated output? (redundant flag set to false)" << endl
		 << "\t-e <encoding> set encoding of input file (defaults to UTF-8)" << endl
		 << "\t-d <data path> set location of BAMA data directory" << endl
		 << "\t-h print usage information" << endl
		 << "\t-v verbose output" << endl;
	exit(0);
}
