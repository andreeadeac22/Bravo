#!/usr/bin/env python
# -*- coding: utf-8 -*-
## @file

"""
Convert an input (Geo)TIFF file to ESRI data. For some reason I get errors when
I tried to run this on my installation of Python3, but it works on Python2 ¯\_(ツ)_/¯

Assumptions: Database grpproj and table data (with columns id(integer), row(integer), col(integer), data(bytea)) already exist. Also there is a folder data where snappy files will be saved. 

Usage: ./geotiff2postgres.py <tiff file> <postgres user>
"""

import sys
import snappy
import numpy as np

from pg import DB
#from PIL import Image
from base64 import b64encode
from osgeo import gdal
from scipy.misc import imresize
import matplotlib.pyplot as plt
from skimage.transform import downscale_local_mean

def store_img(_id, x, y, data):
    """
        Compress each tile into a snappy file and insert it as a new record in the data table.
    """
    compressed_data = snappy.compress(data.tobytes())

    sql = 'INSERT INTO data (id, row, col, data) VALUES (%d, %d, %d, decode(\'%s\', \'base64\'))'
    sql = sql % (_id, y, x, b64encode(compressed_data))

    # write the data out to file too, so that we don't have to recompute it all
    with open('data/(%d, %d).snappy' % (x, y), 'wb') as f:
        f.write(compressed_data)

    db.query(sql)


# avoid a DecompressionBombWarning
#Image.MAX_IMAGE_PIXELS = 1000000000

#img = Image.open(sys.argv[1])
## Open the BigTIFF file
img = gdal.Open(sys.argv[1])
data = np.array(img.GetRasterBand(1).ReadAsArray())
data = downscale_local_mean(data, (100, 100))
data = data.astype(np.uint16)
print data.dtype, data.shape

width, height = data.shape

# Python Geospatial Development Essentials (page 36: GeoTIFF)
## Width and height of a tile
box_size = 128

db = DB(dbname='grpproj', host='localhost', user=sys.argv[2])
_id = 0

bottom_threshold = 16
normal_threshold = 50
top_threshold = 100

clean_sql = 'DELETE FROM data'
db.query(clean_sql)
data.flags.writeable = True

# we'll break down the image into (up to) 500px*500px chunks
for x in range(0, width, box_size):
    for y in range(0, height, box_size):
        frame_width = min(width - x, 128)
        frame_height = min(height - y, 128)
        for u in range(x, x+frame_width):
            for v in range(y,y+frame_height):
                """ Applying median filter"""
                """
                    Bad if apply on whole tiff. Need a way to apply it only outside crack.
                point_height1= data[max(u-1,0), max(v-1,0)]
                point_height2 = data[max(u-1,0), v]
                point_height3 = data[max(u-1,0), min(v+1,box_size-1)]
                    
                point_height4= data[u, max(v-1,0)]
                point_height5= data[u, v]
                point_height6= data[u,min(v+1,box_size-1)]
                    
                point_height7= data[min(u+1,box_size-1), max(v-1,0)]
                point_height8= data[min(u+1,box_size-1), v]
                point_height9= data[min(u+1,box_size-1),min(v+1,box_size-1)]
                    
                filt_array = np.array([point_height1, point_height2, point_height3, point_height4, point_height5, point_height6, point_height7, point_height8, point_height9])
                    
                point_height = np.median(filt_array)
                """
                point_height = data[u,v]
                if point_height > bottom_threshold and point_height <normal_threshold:
                    data[u,v] = data[u,v] *2
                if point_height <= bottom_threshold:
                    data[u,v]= data[u,v]/2
                if point_height >top_threshold:
                    data[u,v]= top_threshold
        frame_data = data[x:x+frame_width,y:y+frame_height]
        if np.count_nonzero(frame_data) >0:
            store_img(_id, x, y, frame_data)
            _id = _id + 1
