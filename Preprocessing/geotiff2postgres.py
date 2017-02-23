#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Convert an input (Geo)TIFF file to ESRI data. For some reason I get errors when
I run this on my installation of Python3, but it works on Python2 ¯\_(ツ)_/¯

Usage: ./geotiff2postgres.py <tiff file> <postgres user>
"""
import sys
import snappy
import numpy as np

from pg import DB
from PIL import Image
from base64 import b64encode

def store_img(_id, x, y, data):
    compressed_data = snappy.compress(frame_data.tobytes())

    sql = 'INSERT INTO data (id, row, col, data) VALUES (%d, %d, %d, decode(\'%s\', \'base64\'))'
    sql = sql % (_id, y, x, b64encode(compressed_data))

    # write the data out to file too, so that we don't have to recompute it all
    with open('data/(%d, %d).snappy' % (x, y), 'wb') as f:
        f.write(compressed_data)

    db.query(sql)


# avoid a DecompressionBombWarning
Image.MAX_IMAGE_PIXELS = 1000000000

img = Image.open(sys.argv[1])
data = np.asarray(img)

width, height = data.shape

# Python Geospatial Development Essentials (page 36: GeoTIFF)
box_size = 500

db = DB(dbname='grpproj', host='localhost', user=sys.argv[2])
_id = 0

# we'll break down the image into (up to) 500px*500px chunks
for x in range(0, width, box_size):
    for y in range(0, height, box_size):
        frame_width = min(width - x, 500)
        frame_height = min(height - y, 500)

        frame_data = data[x:x+frame_width,y:y+frame_height]
        store_img(_id, x, y, frame_data)
        _id = _id + 1
