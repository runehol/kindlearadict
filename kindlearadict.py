#!/usr/bin/env pypy3

from collections import defaultdict
from xml.sax.saxutils import escape, quoteattr
import argparse
import os.path
import time

from process_files import process_textfile, process_tableXY, process_frequency_list_file
import transliterate
import opfgen

prefixes = []
lemmas    = []
suffixes = []

ab = defaultdict(list)
bc = defaultdict(list)
ac = defaultdict(list)

prefixes_for_cat = defaultdict(list)
suffixes_for_cat = defaultdict(list)

freqlist = set()
unvowelled_freqlist = set()

class Morpheme(object):
    def __init__(self, unvowelled, vowelled, cat, pos, gloss, root, lemma):
        self.unvowelled   = unvowelled
        self.vowelled   = vowelled
        self.gloss      = gloss
        self.cat        = cat  # for verifying compatibility
        self.pos        = pos  # human-readable part of speech
        self.root       = root # only really valid for (a subset of) stems, 
                               # empty for everything else
        self.lemma      = lemma # only really valid for (a subset of) stems, 
                               # empty for everything else
                               
    def __str__(self):
        return "%s (%s : %s) %s %s %s" % (self.vowelled, self.root, self.lemma, self.cat, self.pos, self.gloss)
    
    def __repr__(self):
        return self.__str__()


def process_prefixes():
    for (unvowelled, vowelled, cat, pos, gloss, root, lemma) in process_textfile("dictprefixes.txt"):
        prefixes.append(Morpheme(unvowelled, vowelled, cat, pos, gloss, root, lemma))

def process_stems():
    curr_lemma = "not a valid lemma"
    for (unvowelled, vowelled, cat, pos, gloss, root, lemma) in process_textfile("dictstems.txt"):
        if lemma != curr_lemma:
            curr_lemma = lemma
            lemmas.append( (lemma, root, []) )
        lemmas[-1][-1].append(Morpheme(unvowelled, vowelled, cat, pos, gloss, root, lemma))

def process_suffixes():
    for (unvowelled, vowelled, cat, pos, gloss, root, lemma) in process_textfile("dictsuffixes.txt"):
        suffixes.append(Morpheme(unvowelled, vowelled, cat, pos, gloss, root, lemma))

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


def read_freq_list(freq_list_name):
    for (word, count) in process_frequency_list_file(freq_list_name):
        freqlist.add(word)
        unvowelled = transliterate.unicode_strip_vowels(word)
        unvowelled_freqlist.add(unvowelled)

        
def are_prefix_stem_compatible(prefix_morpheme, stem_morpheme):
    return stem_morpheme in ab[prefix_morpheme.cat]
    
def are_stem_suffix_compatible(stem_morpheme, suffix_morpheme):
    return suffix_morpheme.cat in bc[stem_morpheme]

def are_prefix_suffix_compatible(prefix_morpheme, suffix_morpheme):
    return suffix_morpheme.cat in ac[prefix_morpheme.cat]

        
def gen_dict(dest_file, is_mini, freq_list_names, gen_vowelled_forms):
    process_prefixes()
    process_stems()
    process_suffixes()
    process_tableAB()
    process_tableBC()
    process_tableAC()

    filter_by_freq_list = freq_list_names != []
    for fname in freq_list_names:
        read_freq_list(fname)


    dirname, fname = os.path.split(dest_file)

    title = "The Morphological Arabic-English Dictionary"
    if filter_by_freq_list:
        title = "The Abridged Morphological Arabic-English Dictionary"
    if is_mini:
        title = "Test Dictionary"

    out_dict = opfgen.KindleDictGenerator(title, "https://github.com/runehol/kindlearadict/", ["Rune Holm"], "ar", "en", "../datafiles/aradict-cover.jpg", "../datafiles/title-page.html", dirname, fname)

    prefix_suffix_table = defaultdict(list)
    for stem_cat in prefixes_for_cat.keys():
        for prefix_entry in prefixes:
            if not are_prefix_stem_compatible(prefix_entry, stem_cat): continue
            for suffix_entry in suffixes:
                if not are_stem_suffix_compatible(stem_cat, suffix_entry): continue
                if not are_prefix_suffix_compatible(prefix_entry, suffix_entry): continue
                prefix_suffix_table[stem_cat].append( (prefix_entry, suffix_entry) )
                

    if is_mini:
        lemma_selection = [lemma for lemma in lemmas if lemma[1][0:1] == "E"]
    else:
        lemma_selection = lemmas
    start_time = time.clock()
    n_stems = 0
    lemma_idx = 0

    lemmas_to_entry = dict()
    index_to_lemmas = defaultdict(list)

    print("Iterating and processing lemmas...")
    for lemma, root, stem_list in lemma_selection:
        processed_lemma = transliterate.b2u(lemma.split("-")[0].split("_")[0])
        formatted_head_word = "<b>%s</b>" % (escape(processed_lemma))
        form_set = set()
        all_defs = []
        for stem_entry in stem_list:
            n_stems += 1
            first = True
            for prefix_entry, suffix_entry in prefix_suffix_table[stem_entry.cat]:

                unvowelled_form = prefix_entry.unvowelled + \
                                    stem_entry.unvowelled + \
                                    suffix_entry.unvowelled


                u_unvowelled = transliterate.b2u(unvowelled_form)
                
                vowelled_form = prefix_entry.vowelled + \
                                stem_entry.vowelled + \
                                suffix_entry.vowelled
                if first:
                    uvowelled = transliterate.b2u(vowelled_form)


                    gloss = stem_entry.gloss
                    if "verb" in stem_entry.pos and not "Adverb" in stem_entry.pos:
                        gloss = "to " + gloss

                    entry = """<li> %s <i>%s</i> %s</li>\n""" % (escape(uvowelled), escape(stem_entry.pos), escape(gloss))
                    if entry not in all_defs:
                        all_defs.append(entry)


                form = u_unvowelled
                if not filter_by_freq_list or form in unvowelled_freqlist:

                    if not filter_by_freq_list or form in freqlist:
                        form_set.add(form)

                    if gen_vowelled_forms:
                        for form in transliterate.b2u_vowelled_unvowelled_combinations(vowelled_form):
                            if not filter_by_freq_list or form in freqlist:
                                form_set.add(form)


                first = False


                    

        lemma_idx += 1
        if (lemma_idx & 1023 == 0):
            print("Progress: %.2f %%" % (100.0 * float(lemma_idx) / len(lemma_selection)))

        formatted_desc = "<ul>\n"
        formatted_desc += "".join(all_defs)
        formatted_desc += "\n</ul>\n"

        if root == "":
            formatted_desc += "No root\n"
        else:
            processed_root = transliterate.b2u(root)
            formatted_desc += "Root: %s\n" % (escape(processed_root),)

        formatted_desc += "<hr\>\n"

        lemmas_to_entry[lemma] = (formatted_head_word, formatted_desc)
        for form in form_set:
            index_to_lemmas[form].append(lemma)
        #out_dict.add_dict_entry(formatted_head_word, sorted(list(form_set)), formatted_desc)
        

    print("Sorting index...")
    dict_entries = defaultdict(list)
        
    for index, lemma_list in index_to_lemmas.items():
        lemma_tuple = tuple(lemma_list)
        dict_entries[lemma_tuple].append(index)

    print("Generating dictionary...")
    for lemma_tuple, indices in dict_entries.items():
        formatted_head_word, formatted_desc = lemmas_to_entry[lemma_tuple[0]]
        for lemma in lemma_tuple[1:]:
            hw, desc = lemmas_to_entry[lemma]
            formatted_desc += hw + desc

        out_dict.add_dict_entry(formatted_head_word, indices, formatted_desc)
        
    out_dict.finalize()

    elapsed_time = time.clock() - start_time
    print("Generated %d original entries, %d expanded entries, %d empty entries, %d index size from %d lemmas and %d stems in %f seconds" % (out_dict.n_orig_entries, out_dict.n_expanded_entries, out_dict.n_empty_entries, out_dict.index_size, len(lemma_selection), n_stems, elapsed_time))

                
        
    
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generate an Arabic-English Kindle dictionary')
    parser.add_argument('--dest-file',
                    help='Destination opf file')
    parser.add_argument('--mini', action='store_true',
                    help='Generate small test dictionary (only roots starting with ع)')
    parser.add_argument('--vowelled-forms', action='store_true',
                    help='Generate vowelled forms (best used with frequency list filter)')
    parser.add_argument('--frequency-list', nargs='*', default=[],
                    help='Frequency list to filter by (more than one allowed)')

    arg = parser.parse_args()
    gen_dict(arg.dest_file, arg.mini, arg.frequency_list, arg.vowelled_forms)
