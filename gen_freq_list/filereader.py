# working out which format something is in and read the file

import srtreader
import txtreader
import epubreader
import htmlreader
import os.path


class FileFormatError(BaseException):
    pass



def read_file(fname):
    ext = os.path.splitext(fname)[1]

    if ext == ".srt":
        return srtreader.read_file(fname)

    elif ext == ".epub":
        return epubreader.read_file(fname)

    elif ext == ".html" or ext == ".xhtml" or ext == ".htm" or ext == ".xml":
        return htmlreader.read_file(fname)

    elif ext == ".txt":
        return txtreader.read_file(fname)

    else:
        raise FileFormatError()


        
