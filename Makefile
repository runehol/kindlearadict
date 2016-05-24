
.PHONY: all mini clean install

all: aradict/morphological_arabic_english_dict.mobi
mini: minidict/minidict.mobi


aradict/morphological_arabic_english_dict.mobi: aradict/morphological_arabic_english_dict.opf
	cd aradict &&  /usr/bin/time kindlegen -c2 -verbose -dont_append_source morphological_arabic_english_dict.opf; true

aradict/morphological_arabic_english_dict.opf: $(wildcard *.py) $(wildcard *.txt)
	./kindlearadict.py --dest-file=$@ --vowelled-forms --frequency-list freqlist-ebooks.txt  freqlist-subtitles-2012.txt --min-word-frequency=3

install: aradict/morphological_arabic_english_dict.mobi
	rm -r /Volumes/Kindle/documents/morphological_arabic_english_dict.*; cp aradict/morphological_arabic_english_dict.mobi /Volumes/Kindle/documents/morphological_arabic_english_dict.mobi


minidict/minidict.mobi: minidict/minidict.opf
	cd minidict &&  /usr/bin/time kindlegen -c2 -verbose -dont_append_source minidict.opf; true

minidict/minidict.opf: $(wildcard *.py) $(wildcard *.txt)
	./kindlearadict.py --mini --dest-file=$@ --vowelled-forms --frequency-list freqlist-ebooks.txt  freqlist-subtitles-2012.txt --min-word-frequency=3

installmini: minidict/minidict.mobi
	rm -r /Volumes/Kindle/documents/morphological_arabic_english_dict.*; cp minidict/minidict.mobi /Volumes/Kindle/documents/morphological_arabic_english_dict.mobi





clean:
	rm -r aradict minidict

