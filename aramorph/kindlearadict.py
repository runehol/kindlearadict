# Explore using a pickled file rather than redis

import cPickle as pickle
from collections import defaultdict
from aramorpher import Morpheme, Aramorpher
from process_files import process_textfile, process_tableXY

prefixes = defaultdict(list)
stems    = defaultdict(list)
suffixes = defaultdict(list)

ab = defaultdict(list)
bc = defaultdict(list)
ac = defaultdict(list)
prefixes_for_cat = defaultdict(list)
suffixes_for_cat = defaultdict(list)

def process_prefixes():
    for (unvowelled, vowelled, cat, pos, gloss, root) in process_textfile("dictprefixes.txt"):
        prefixes[unvowelled].append(Morpheme(vowelled, cat, pos, gloss, root))

def process_stems():
    for (unvowelled, vowelled, cat, pos, gloss, root) in process_textfile("dictstems.txt"):
        stems[unvowelled].append(Morpheme(vowelled, cat, pos, gloss, root))

def process_suffixes():
    for (unvowelled, vowelled, cat, pos, gloss, root) in process_textfile("dictsuffixes.txt"):
        suffixes[unvowelled].append(Morpheme(vowelled, cat, pos, gloss, root))

def process_tableAB():
    for (left, right) in process_tableXY("tableab.txt"):
        ab[left].append(right)
        for key, lst in prefixes.iteritems():
            for entry in lst:
                if entry.cat == right:
                    prefixes_for_cat[left].append(key)
             

def process_tableBC():
    for (left, right) in process_tableXY("tablebc.txt"):
        bc[left].append(right)
        for key, lst in suffixes.iteritems():
            for entry in lst:
                if entry.cat == right:
                    suffixes_for_cat[left].append(key)

def process_tableAC():
    for (left, right) in process_tableXY("tableac.txt"):
        ac[left].append(right)

def gen_dict():
    process_prefixes()
    process_stems()
    process_suffixes()
    process_tableAB()
    process_tableBC()
    process_tableAC()

    # now construct AramorphInfo
    aramorph = Aramorpher(prefixes, stems, suffixes, ab, bc, ac)
    print len(stems), len(prefixes), len(suffixes)


    

    for stem in {key:value for key,value in stems.items()[0:10]}:
        for prefix in prefixes:
            for suffix in suffixes:
                entries = aramorph.check_compatibility("", prefix, stem, suffix)
                if not entries: continue
                print entries
        
    
if __name__ == "__main__":
    gen_dict()
