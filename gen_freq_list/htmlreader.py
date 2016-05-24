import os.path
from html.parser import HTMLParser

class TextExtractParser(HTMLParser):
    def __init__(self):
        super().__init__(convert_charrefs=True)
        self.items = []
        self.ignore_level = 0
        self.ignore_tags = set(["style", "script"])

    def handle_starttag(self, tag, attrs):
        if tag in self.ignore_tags:
            self.ignore_level += 1

    def handle_endtag(self, tag):
        if tag in self.ignore_tags:
            self.ignore_level -= 1

    def handle_data(self, data):
        if self.ignore_level == 0:
            self.items.append(data)


def parse_html(pname, section, contents):

    parser = TextExtractParser()
    parser.feed(contents)
    for item in parser.items:
        yield item, section, pname

def parse_file_html(pname, section, f):

    parser = TextExtractParser()
    chunk_size = 1024*1024
    while True:
        data = f.read(chunk_size)
        if not data: break
        parser.feed(data)
    for item in parser.items:
        yield item, section, pname
        

def read_file_file(filename):
    pname = os.path.splitext(os.path.basename(filename))[0]

    f = open(filename, encoding="utf-8")
    yield from parse_html(pname, "", f)


