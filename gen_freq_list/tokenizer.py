
import nltk.tokenize


def tokenize(data):

    sent_tokenize = nltk.tokenize.sent_tokenize

    tokenizer = nltk.tokenize.RegexpTokenizer(u"[\s\.,-?!'\"،؟\d·•—()×«»%\[\]|↑*]+", gaps=True)
    word_tokenize = tokenizer.tokenize

    
    for text, blockname, textname in data:
        sentences = sent_tokenize(text.strip())
        for sentence in sentences:
            words = word_tokenize(sentence)
            for word in words:
                if len(word) > 1:
                    yield (word, sentence, blockname, textname)
