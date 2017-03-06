#!/usr/bin/env python
# -*- coding: utf-8 -*-
## @file
# Perform multiple compression benchmarks.
# Usage: ./geotiff2postgres.py tiff_file

import sys
import time
import snappy
import zlib
import bz2
try:
    import lzma
except ImportError:
    from backports import lzma
import numpy as np

from PIL import Image

## initialize as empty arrays
snappy_res, gz_res, bz2_res, lzma_res = ([] for i in xrange(4))
snappy_sizes, gz_sizes, bz2_sizes, lzma_sizes = ([] for i in xrange(4))
snappy_de_res, gz_de_res, bz2_de_res, lzma_de_res = ([] for i in xrange(4))

def benchmark_one(data, algo, comp_var, decomp_var, size_var):
	"""
	Record the time compression/decompression of some block of data takes for
	a certain algorithm.
	
	Arguments:
	data -- the compressed file (in our case, an image converted into a NumPy array)
	algo -- the algorithm we're using to compress
	comp_var -- a list of times that it takes to compress
	decomp_var -- a list of times that it takes to decompress
	size_var -- a list of the compression sizes
	"""
    t1 = time.time()
    ## @test that there is data that is compressed
    assert data is not None
    comp = algo.compress(data)
    t2 = time.time()
    assert t1 <= t2
    assert len(comp)>0
    size_var.append(len(comp))
    comp_var.append(t2 - t1)
    
    # now, run the same but for decompression
    t1 = time.time()
    decomp = algo.decompress(comp)
    t2 = time.time()
    assert t1 <= t2
    assert len(comp)>0
    decomp_var.append(t2 - t1)

def benchmark(data):
    global snappy_res, gz_res, bz2_res, lzma_res
    global snappy_de_res, gz_de_res, bz2_de_res, lzma_de_res
    global snappy_size, gz_size, bz2_size, lzma_size
    
    assert data is not None
    data = data.tobytes()
    assert data is not None
    
    # please note that we are aggregating over different datas coming
    # from different (boxsize x boxsize px) chunks in the image
    benchmark_one(data, snappy, snappy_res, snappy_de_res, snappy_sizes)
    benchmark_one(data, zlib, gz_res, gz_de_res, gz_sizes)
    benchmark_one(data, bz2, bz2_res, bz2_de_res, bz2_sizes)
    benchmark_one(data, lzma, lzma_res, lzma_de_res, lzma_sizes)
    
def print_results(name, var, sizes):
	"""Pretty print the results from the benchmark test."""
    print(name)
    print('min time: %f s' % min(var))
    print('max time: %f s' % max(var))
    print('ave time: %f s' % (sum(var)/(float)len(var)))
    print('file size: %d' % sum(sizes))

# avoid a DecompressionBombWarning
Image.MAX_IMAGE_PIXELS = 1000000000

img = Image.open(sys.argv[1])
data = np.asarray(img)

width, height = data.shape
print(len(data.tobytes()))
sys.exit(0)
## our chunks are box_size x box_size px
box_size = 500


# break down the image into box_size x box_size chunks (smaller near borders)
# and use each chunk as a data
for x in range(0, width, box_size):
    for y in range(0, height, box_size):
        frame_width = min(width - x, box_size)
        frame_height = min(height - y, box_size)
        
        ## @test test that width is positive
        assert frame_width > 0
         ## @test test that height is positive
        assert frame_height > 0
        
        frame_data = data[x:x+frame_width,y:y+frame_height]
        benchmark(frame_data)

print_results('SNAPPY COMP', snappy_res, snappy_sizes)
print_results('SNAPPY DECOMP', snappy_de_res, snappy_sizes)
print_results('GZ COMP', gz_res, gz_sizes)
print_results('GZ DECOMP', gz_de_res, gz_sizes)
print_results('BZ2 COMP', bz2_res, bz2_sizes)
print_results('BZ2 DECOMP', bz2_de_res, bz2_sizes)
print_results('LZMA COMP', lzma_res, lzma_sizes)
print_results('LZMA DECOMP', lzma_de_res, lzma_sizes)

