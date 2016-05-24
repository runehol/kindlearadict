import os.path
from ebooklib import epub
import htmlreader

        

def read_file(filename):
    book = epub.read_epub(filename)
    pname = os.path.splitext(os.path.basename(filename))[0]

    numitems = 0
    for item in book.items:
        if isinstance(item, epub.EpubHtml):

            data = item.content.decode("utf-8")
            yield from htmlreader.parse_html(pname, "section %d" % numitems, data)
            numitems += 1


