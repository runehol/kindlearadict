#!/usr/bin/env python3

from collections import defaultdict
from process_files import process_textfile, process_tableXY
import transliterate
import time
import kindledictgen
from xml.sax.saxutils import escape, quoteattr

prefixes = []
lemmas    = []
suffixes = []

ab = defaultdict(list)
bc = defaultdict(list)
ac = defaultdict(list)

prefixes_for_cat = defaultdict(list)
suffixes_for_cat = defaultdict(list)


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

    out_dict = kindledictgen.KindleDictGenerator("The Morphological Arabic-English Dictionary", "http://github.com/runehol/kindlearadict/", ["Rune Holm"], "ar", "en", "../../manual/aradict-cover.jpg", "../../manual/title-page.html", "aradict", "aradict.opf")

    prefix_suffix_table = defaultdict(list)
    for stem_cat, prefixes_list in prefixes_for_cat.items():
        for prefix_entry in prefixes_list:
            for suffix_entry in suffixes_for_cat[stem_cat]:
                if not are_prefix_suffix_compatible(prefix_entry, suffix_entry): continue
                prefix_suffix_table[stem_cat].append( (prefix_entry, suffix_entry) )
                

    
    print(len(lemmas), len(prefixes), len(suffixes))

    lemma_selection = lemmas #[:1000]
    n_generated = 0
    start_time = time.clock()
    n_stems = 0
    lemma_idx = 0
    for lemma, root, stem_list in lemma_selection:

        processed_lemma = transliterate.b2u(lemma.split("-")[0].split("_")[0])
        processed_root = transliterate.b2u(root)
        if root == "": processed_root = "No root"
        formatted_head_word = "<b>%s - %s</b>" % (escape(processed_lemma), escape(processed_root))
        forms = []
        formatted_desc = "<ul>\n"
        for stem_entry in stem_list:
            n_stems += 1
            first = True
            for prefix_entry, suffix_entry in prefix_suffix_table[stem_entry.cat]:

                unvowelled_form = prefix_entry.unvowelled + \
                                    stem_entry.unvowelled + \
                                    suffix_entry.unvowelled


                u_unvowelled = transliterate.b2u(unvowelled_form)
                forms.append(u_unvowelled)
                

                if first:
                    first = False
                    vowelled_form = prefix_entry.vowelled + \
                                    stem_entry.vowelled + \
                                    suffix_entry.vowelled
                    uvowelled = transliterate.b2u(vowelled_form)

                    gloss = stem_entry.gloss
                    if "verb" in stem_entry.pos:
                        gloss = "to " + gloss

                    entry = """<li> %s <i>%s</i> %s</li>\n""" % (escape(uvowelled), escape(stem_entry.pos), escape(gloss))

                    formatted_desc += entry

                    

        lemma_idx += 1
        if (lemma_idx & 1023 == 0):
            print("Progress: %.2f %%" % (100.0 * float(lemma_idx) / len(lemma_selection)))
       
        formatted_desc += "\n</ul>\n"
        out_dict.add_dict_entry(formatted_head_word, forms, formatted_desc)
        

    elapsed_time = time.clock() - start_time
    print("Generated %d forms from %d lemmas and %d stems in %f seconds" % (n_generated, len(lemma_selection), n_stems, elapsed_time))

    out_dict.finalize()
                
        
    
if __name__ == "__main__":
    gen_dict()
