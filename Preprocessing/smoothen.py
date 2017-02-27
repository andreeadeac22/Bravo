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

def delete_point(id, surface,no_del):
    no_del = no_del +1
    """
    print id, surface, "delete"
    sql = 'DELETE * FROM data WHERE row= %d AND col = %d'
    sql = sql % (x,y)

    db.query(sql)
    """


def insert_point(id, surface, no):
    no = no +1
    print id, surface, "insert"
    """
    sql = 'INSERT INTO data(id,row,col,surface) VALUES (%d,%d,%d,%f)'
    sql = sql % (_id, row, col, surface)
    with open('data/(%d, %d).txt' % (x, y), 'wb') as f:
        f.write(surface)
    db.query(sql)
    """

no_steps= 10
db = DB(dbname='grpproj', host='localhost', user=sys.argv[1])
min_threshold = 50
max_threshold = 1
width = height = 500

total_no =0

#file = sys.argv[1]

for u in range(0, 25500, 500):           #iterate through all decompressed files
    for v in range( 1000, 20500, 500):
        no=0
        no_del =0
        file = 'decomp_data/(%d,%d)' % (u,v)  # check if file exists
        if os.path.isfile(file):
            data = np.fromfile(file)   #put data from file in array
            for i in range(0,len(data)):
                    if i>0:
                        y=data[i]
                        if not math.isnan(y):
                            y= int(y)
                            if y-x > max_threshold:   # close to edge
                                step_size = int((y-x)/no_steps)
                                for step in range(x + step_size,y, step_size): #add intermediary values for higher density
                                    insert_point(i,step,no)
                            else:
                                if y-x < min_threshold:
                                    delete_point(i,y, no_del) # nothing happens, can have sparse data
                    x=data[i]
                    if not math.isnan(x):
                        x= int(x)
            if no > 0:
                total_no = total_no +1
print total_no
