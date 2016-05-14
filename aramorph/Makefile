
.PHONY: all clean install

all: aradict/aradict.mobi


aradict/aradict.mobi: aradict/aradict.opf
	cd aradict &&  /usr/bin/time kindlegen -c2 -verbose -dont_append_source aradict.opf; true

aradict/aradict.opf: $(wildcard *.py) $(wildcard *.txt)
	./kindlearadict.py

install: aradict/aradict.mobi
	rm -r /Volumes/Kindle/documents/aradict.* && cp aradict/aradict.mobi /Volumes/Kindle/documents/


clean:
	rm -r aradict

