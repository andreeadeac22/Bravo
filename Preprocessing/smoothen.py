#!/usr/bin/env python
# -*- coding: utf-8 -*-


"""
    This file iterates through all the decompressed snappy files (which are stored in decomp_data directory after running decompress_all.py) and all the points in each file. For each consecutive pair of height, it computes the difference and if it is too close, it deletes the point, while if it is too far, it adds additional points with heights in that interval. 
"""

import sys
import numpy as np
import math
import os.path

from pg import DB
from PIL import Image


no_steps= 10
db = DB(dbname='grpproj', host='localhost', user=sys.argv[1])
min_threshold = 10
max_threshold = 1000000


width = height = 500


clear_table = 'DELETE FROM data'
db.query(clear_table)

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
            f= open(file,"w")
            f.write(data)
            f.close()
