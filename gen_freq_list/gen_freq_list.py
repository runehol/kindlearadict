#!/usr/bin/env python3
import sys
import filereader
import tokenizer
import uniquify
import errno



def main(argv = sys.argv):

    argv = argv[1:]
    
        

    uw = uniquify.UniqueWords()
    for fname in argv:
    
        data = filereader.read_file(fname)
        tokens = tokenizer.tokenize(data)
    
        uw.uniquify(tokens)

    uniquify.rank(uw)



if __name__ == "__main__":
    try:
        main()
    except BrokenPipeError:
        try:
            sys.stdout.close()
        except BrokenPipeError:
            pass

        

    
