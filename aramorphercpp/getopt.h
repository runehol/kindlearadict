/**
 * @file getopt.h
 * @brief Simple implementation of UNIX-like getopts (from <unistd.h>), usable in Windows environment.
 * @author David Edelstein
 * @date 2-18-09
 */
#ifndef GETOPT_H
#define GETOPT_H

extern char *optarg;
extern int optind;
extern int opterr;
extern int optopt;

/**
 * Use this function to parse command-line options.
 * @param argc the argc value from the main function
 * @param *argv args from the main function
 * @param *optstr an option string definining what options are allowed/mandatory
 */
int getopt(int argc, char* const *argv, const char *optstr);

#endif /* GETOPT_H */
