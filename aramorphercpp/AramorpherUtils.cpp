/**
 * @file AramorpherUtils.cpp
 * @brief Implementation file for aramorpherutils.
 * @author David Edelstein
 * @date 2-18-09
 */
#include <iostream>
#include "AramorpherUtils.h"

using namespace Aramorph;

// File Exception -- not found, couldn't be read, etc.
BadFileException::BadFileException(const string &fn, const string &msg):std::runtime_error(fn + ": " + msg) {}

// EncodingException with name of the encoding
EncodingException::EncodingException(const string &enc):std::runtime_error("encoding " + enc + " failed") {}

// EncodingException with name of the encoding and extra error message
EncodingException::EncodingException(const string &enc, const string &msg):
	std::runtime_error("encoding " + enc + " failed: " + msg) {}

// exits with error message if file is not open
void Aramorph::checkfile(ifstream &f, const string &fn) {
	f.open(fn.c_str(), std::ios::in);
	if (f.fail()) {
        throw BadFileException(fn, "missing file");
	}
}

// exits with error message if file is not open
void Aramorph::checkfile(ofstream &f, const string &fn) {
	f.open(fn.c_str(), std::ios::out);
	if (f.fail()) {
        throw BadFileException(fn, "could not write to file");
	}
}

bool Aramorph::replace_lastchar(string& str, const char& last, const char &repl) {
	bool rep = false;
	 if (str[str.length()-1] == last) {
		 str[str.length()-1] = repl;
		 rep = true;
	 }
	 return rep;
}
