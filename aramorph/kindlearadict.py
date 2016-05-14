# Explore using a pickled file rather than redis

import cPickle as pickle
from collections import defaultdict
from aramorpher import Morpheme, Aramorpher
from process_files import process_textfile, process_tableXY
import transliterate
import time

prefixes = []
stems    = []
suffixes = []

ab = defaultdict(list)
bc = defaultdict(list)
ac = defaultdict(list)

prefixes_for_cat = defaultdict(list)
suffixes_for_cat = defaultdict(list)


def process_prefixes():
    for (unvowelled, vowelled, cat, pos, gloss, root, lemma) in process_textfile("dictprefixes.txt"):
        prefixes.append(Morpheme(vowelled, cat, pos, gloss, root, lemma))

def process_stems():
    curr_lemma = "not a valid lemma"
    for (unvowelled, vowelled, cat, pos, gloss, root, lemma) in process_textfile("dictstems.txt"):
        if lemma != curr_lemma:
            curr_lemma = lemma
            stems.append( (lemma, []) )
        stems[-1][1].append(Morpheme(vowelled, cat, pos, gloss, root, lemma))

def process_suffixes():
    for (unvowelled, vowelled, cat, pos, gloss, root, lemma) in process_textfile("dictsuffixes.txt"):
        suffixes.append(Morpheme(vowelled, cat, pos, gloss, root, lemma))

def process_tableAB():
    for (left, right) in process_tableXY("tableab.txt"):
        ab[left].append(right)
        for entry in prefixes:
            if entry.cat == left:
                prefixes_for_cat[right].append(entry)
             

def process_tableBC():
    for (left, right) in process_tableXY("tablebc.txt"):
        bc[left].append(right)
        for entry in suffixes:
            if entry.cat == right:
                suffixes_for_cat[left].append(entry)

def process_tableAC():
    for (left, right) in process_tableXY("tableac.txt"):
        ac[left].append(right)

def are_prefix_stem_compatible(prefix_morpheme, stem_morpheme):
    return stem_morpheme.cat in ab[prefix_morpheme.cat]
    
def are_stem_suffix_compatible(stem_morpheme, suffix_morpheme):
    return suffix_morpheme.cat in bc[stem_morpheme.cat]

def are_prefix_suffix_compatible(prefix_morpheme, suffix_morpheme):
    return suffix_morpheme.cat in ac[prefix_morpheme.cat]

        
def gen_dict():
    process_prefixes()
    process_stems()
    process_suffixes()
    process_tableAB()
    process_tableBC()
    process_tableAC()

    prefix_suffix_table = defaultdict(list)
    for stem_cat, prefixes_list in prefixes_for_cat.iteritems():
        for prefix_entry in prefixes_list:
            for suffix_entry in suffixes_for_cat[stem_cat]:
                if not are_prefix_suffix_compatible(prefix_entry, suffix_entry): continue
                prefix_suffix_table[stem_cat].append( (prefix_entry, suffix_entry) )
                

    
    print len(stems), len(prefixes), len(suffixes)

    stem_selection = stems[:1000]
    n_generated = 0
    start_time = time.clock()
    for lemma, stem_list in stem_selection:
        for stem_entry in stem_list:
            for prefix_entry, suffix_entry in prefix_suffix_table[stem_entry.cat]:

                vowelled_form = prefix_entry.vowelled + \
                                    stem_entry.vowelled + \
                                    suffix_entry.vowelled

                gloss = stem_entry.gloss

                uvowelled = transliterate.b2u(vowelled_form)
                n_generated += 1


    elapsed_time = time.clock() - start_time
    print "Generated %d forms from %d lemmas in %f seconds" % (n_generated, len(stem_selection), elapsed_time)
                
        
    
if __name__ == "__main__":
    gen_dict()
