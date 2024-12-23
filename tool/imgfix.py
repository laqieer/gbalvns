#!/usr/bin/env python3

import sys
from PIL import Image
import struct

def main(argv):
    for file in argv:
        im = Image.open(file)
        iw, ih = im.size
        f = open(file[:-4] + ".img", "rb")
        x = f.read()
        f.close()
        f = open(file[:-4] + ".img", 'wb')
        f.write(struct.pack('HH', iw, ih))
        f.write(x)
        f.close()


if __name__ == "__main__":
   main(sys.argv[1:])
