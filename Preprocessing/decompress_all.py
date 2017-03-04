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

#define the constants we assume: (TODO: explain constants)
box_size = 500
usize = 25500
vsize = 20500
vstart = 1000

if __name__ == "__main__":
    for u in range(0, usize, box_size):
        # iterate through all snappy files in data directory
        for v in range(vstart, vsize, box_size):   
            file= 'data/(%d, %d).snappy' % (u,v)
            if os.path.isfile(file):
                _if = file
                #decompress in decomp_data directory
                of = 'decomp_data/(%d,%d)' % (u,v) 
                with open(_if, 'rb') as f:
                    data = f.read()
                    decomp = uncompress(data)
                    with open(of, 'wb') as oof:
                        oof.write(decomp)
