import os.path


def read_file(filename):
    f = open(filename, encoding="utf-8")
    blocks = []

    pname = os.path.splitext(os.path.basename(filename))[0]


    txt = f.read()
    txt = txt.replace("\n", " ")
    txt = txt.replace("\r", " ")

    yield txt, "", pname

