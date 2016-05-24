
import collections
import math

class UniqueWords:
    def __init__(self):
        self.word_count = 0
        self.stems = dict()


    def reg_word(self, stem, word, sentence, blockname, textname):
        self.word_count += 1

        uw = self.stems.get(stem)
        if uw is None:
            uw = UniqueWord(stem)
            self.stems[stem] = uw

        uw.reg_word(word, sentence, blockname, textname)


    def uniquify(self, tokens):
        for (word, sentence, blockname, textname) in tokens:
            stem = word
            self.reg_word(stem, word, sentence, blockname, textname)



    def weed_out_uninteresting_words(self, lang):
        for stem in list(self.stems.keys()):
            uw = self.stems[stem]

            variants = uw.get_variants()
            if not lang.is_interesting(variants):
                self.word_count -= uw.count
                del self.stems[stem]
        

        
    def num_unique_words(self):
        return len(self.stems)


        
class UniqueWord:

    def __init__(self, stem):
        self.stem = stem
        self.word_freqs = collections.defaultdict(int)
        self.word_examples = dict()
        self.count = 0

    def reg_word(self, word, sentence, blockname, textname):
        if not word in self.word_examples:
            self.word_examples[word] = (sentence, blockname, textname)

        self.word_freqs[word] += 1
        self.count += 1

    def get_variants(self):
        t = [(v, k) for k, v in self.word_freqs.items()]
        t.sort(reverse=True)
        return t

    def repr_word(self):
        t = self.get_variants()
        if len(t):
            # try to get a lower-case word if possible
            idx = 0
            while idx < len(t) - 1 and t[idx][1].istitle():
                idx += 1
                
            
            return t[idx][1]


    def get_example(self):
        word = self.repr_word()
        sentence, blockname, textname = self.word_examples[word]
        return (word, sentence, blockname, textname)




def rank(unique_words):
    t = [(uw.count, uw.stem, uw) for uw in unique_words.stems.values()]
    t.sort(reverse=True)
    accum_count = 0
    word_count = unique_words.word_count
    scalefac = 1.0
    if word_count > 0:
        scalefac = 100.0/word_count

    num_unique_words = unique_words.num_unique_words()

    table_entries = []


    thresholds = [0.80, 0.85, 0.90, 0.95, 1.0]
    thresholds.sort()
    int_thresholds = [int(math.ceil(threshold*word_count)) for threshold in thresholds]
    words_to_thresholds = [0]*len(thresholds)
    unknown_to_thresholds = [0]*len(thresholds)
    threshold_idx = 0


    for idx, entry in enumerate(t):
        uw = entry[2]
        count = uw.count

        accum_percent = scalefac*accum_count
        percent = count*scalefac
        table_entries.append( (accum_percent, percent, uw) )
        

        accum_count += count
        if threshold_idx < len(thresholds) and accum_count >= int_thresholds[threshold_idx]:
            words_to_thresholds[threshold_idx] = idx+1
            unknown_to_thresholds[threshold_idx] = len(table_entries)
            threshold_idx += 1



    words_per_family = 0.0
    if num_unique_words > 0:
        words_per_family = float(word_count)/num_unique_words

    #print("%d word families over %d words, %.1f words per family" % (num_unique_words, word_count, words_per_family))

    for (accum_percent, percent, uw) in table_entries:
        word, sentence, blockname, textname = uw.get_example()
        #print("%.2f\t%.3f\t%s\t%s" % (accum_percent, percent, word, uw.stem))
        #print("%.2f\t%.3f\t%s\t%s\t%s %s" % (accum_percent, percent, word, sentence, textname, blockname))
        print("%s %d" % (uw.stem, uw.count))
