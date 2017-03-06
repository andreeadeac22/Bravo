#!/usr/bin/env python
## @file
# Decompress all snappy files, in the format that we're storing them. This is because we're using a slightly different format to the snappy command line tool.
# Assumptions: Already ran geotiff2postgres.py and created decomp_data directory.
# Usage: ./decompress.py

import sys
import os.path

from snappy import uncompress

## box_size is the width and the height of each tile
box_size = 500
## usize is the maximum of the first tag of the snappy files
usize = 25500
## vsize is the maximum of the second tag of the snappy files
vsize = 20500

if __name__ == "__main__":
    for u in range(0, usize, box_size):
        # iterate through all snappy files in data directory
        for v in range(0, vsize, box_size):
            ## compressed files are in data directory
            file= 'data/(%d, %d).snappy' % (u,v)
            if os.path.isfile(file):
                _if = file
                ## decompressed files are in decomp_data directory
                of = 'decomp_data/(%d,%d)' % (u,v) 
                with open(_if, 'rb') as f:
                    ## data copied from compressed file
                    data = f.read()
                    ## uncompressed data
                    decomp = uncompress(data)
                    with open(of, 'wb') as oof:
                        oof.write(decomp)
