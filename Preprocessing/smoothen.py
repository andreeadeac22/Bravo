#!/usr/bin/env python
# -*- coding: utf-8 -*-


"""
    This file iterates through all the decompressed snappy files (which are stored in decomp_data directory after running decompress_all.py) and all the points in each file. For each consecutive pair of height, it computes the difference and if it is too close, it deletes the point, while if it is too far, it adds additional points with heights in that interval. 
    Assumptions: Already ran geotiff2posgres.py, created sdata directory and created table sdata with same columns as table data
    Usage: ./smoothen.py postgres
    Output: New snappy files should now be in sdata directory and sdata should be populated
"""

import sys
import numpy as np
import snappy
import math
import os.path

from pg import DB
from PIL import Image
from base64 import b64encode
from snappy import uncompress

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

no_steps= 10
db = DB(dbname='grpproj', host='localhost', user=sys.argv[1])
min_threshold = 10
max_threshold = 1000000

_id =0


for u in range(0, 25500, 500):           #iterate through all decompressed files
    for v in range( 1000, 20500, 500):
        file = 'decomp_data/(%d,%d)' % (u,v)  # check if file exists
        if os.path.isfile(file):
            f = open(file,"r")
            data = np.fromfile(f, dtype=np.uint32)   #put data from file in array
            f.close()
            for i in range(0,len(data)):
                    if i>0:
                        if i < len(data) and not math.isnan(data[i]):
                            y= int(data[i])
                            if y-x > max_threshold:   # close to edge
                                step_size = int((y-x)/no_steps)
                                for step in range(x + step_size,y, step_size): #add intermediary values for higher density
                                    i=i+1
                                    data = np.insert(data,i,step)
                            else:
                                if y-x < min_threshold:
                                    data= np.delete(data,i) # nothing changes, can have sparse data
                                    i=i-1
                    if i< len(data ) and not math.isnan(data[i]):
                        x= int(data[i])
            print len(data)
            compressed_data = snappy.compress(data.tobytes())
            sql = 'INSERT INTO sdata (id, row, col, data) VALUES (%d, %d, %d, decode(\'%s\', \'base64\'))'
            sql = sql % (_id, v, u, b64encode(compressed_data))

            # write the data out to file too, so that we don't have to recompute it all
            with open('sdata/(%d, %d).snappy' % (u, v), 'wb') as f:
                f.write(compressed_data)
            db.query(sql)
            _id= _id +1
