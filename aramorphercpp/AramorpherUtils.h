/**
 * @file AramorpherUtils.h
 * @brief General utility functions for string processing and other class-independent tasks.
 * @author David Edelstein
 * @date 2-18-09
 */
#ifndef ARAMORPHERUTILS_H_
#define ARAMORPHERUTILS_H_

#include <stdexcept>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

using std::ifstream;
using std::ofstream;
using std::string;

namespace Aramorph {

/** used as index for string positions */
typedef string::size_type str_pos;

/**
 * \class BadFileException
 * \brief Exception thrown when a file is not found, empty, or otherwise can't be read
 */
class BadFileException : public std::runtime_error {

public:
	/**
	* Constructor with the name of the file (with path) that was not found or could not be read.
	*/
	BadFileException(const string &fn, const string &msg);
};

/**
 * \class EncodingException
 * \brief Exception thrown when an encoding attempt fails
 */
class EncodingException : public std::runtime_error {

public:
	/**
	* Constructor with the name of the desired encoding
	*/
	EncodingException(const string &enctr);

	/**
	* Constructor with the name of the desired encoding, and an additional error message.
	*/
	EncodingException(const string &encstr, const string &msg);

};

/**
 * Check whether a file exists and open it for reading if it does, throw a BadFileException if not.
 * \param f filehandle to check
 * \param fn name of file
 * \throws BadFileException if the file wasn't opened
 */
void checkfile(ifstream &f, const string &fn);

/**
 * Create an output file and open it for writing.
 * \param f filehandle to check
 * \param fn name of file
 * \throws BadFileException if the file wasn't opened
 */
void checkfile(ofstream &f, const string &fn);

/**
 * Replaces the last char in a string, if and only if it matches the specified one.
 * Returns true if a replacement occurred, false if the last char in the string did not match the one
 * searched for (so no change happened).
 * \param str string to replace the last char
 * \param last char to be replace IF it is the last char in str
 * \param repl char with which the last char in str will be replaced IF it was last
 * \return true if str's last char was replaced with repl
 */
bool replace_lastchar(string& str, const char& last, const char& repl);

} // namespace



#endif /* ARAMORPHERUTILS_H_ */
