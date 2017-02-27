#!/usr/bin/env python
"""
Decompress a snappy file, in the format that we're storing it. This is because
we're using a slightly different format to the snappy command line tool.

Usage: ./decompress.py <in file> <out file>
"""
import sys

from snappy import uncompress

if __name__ == "__main__":
    _if = sys.argv[1]
    of = sys.argv[2]

    with open(_if, 'rb') as f:
        data = f.read()
        decomp = uncompress(data)
        with open(of, 'wb') as oof:
            oof.write(decomp)
