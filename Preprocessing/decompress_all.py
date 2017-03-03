#!/usr/bin/env python
"""
    Decompress a snappy file, in the format that we're storing it. This is because
    we're using a slightly different format to the snappy command line tool.
    
    Assumptions: Already ran geotiff2postgres.py and created decomp_data directory.
    Usage: ./decompress.py <in file> <out file>
    """
import sys
import os.path

from snappy import uncompress

if __name__ == "__main__":
    for u in range(0, 25500, 500):
        for v in range( 1000, 20500, 500):   # iterate through all snappy files in data directory
            file= 'data/(%d, %d).snappy' % (u,v)
            if os.path.isfile(file):
                _if = file
                of = 'decomp_data/(%d,%d)' % (u,v) #decompress in decomp_data directory
                with open(_if, 'rb') as f:
                    data = f.read()
                    decomp = uncompress(data)
                    with open(of, 'wb') as oof:
                        oof.write(decomp)
