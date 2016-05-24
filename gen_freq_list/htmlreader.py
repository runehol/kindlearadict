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


def parse_html(pname, section, content):

    parser = TextExtractParser()
    parser.feed(content)
    for item in parser.items:
        yield item, section, pname
        

def read_file(filename):
    pname = os.path.splitext(os.path.basename(filename))[0]

    f = open(filename, encoding="utf-8")
    data = f.read()
    yield from parse_html(pname, "", data)


