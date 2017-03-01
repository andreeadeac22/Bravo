#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Perform multiple compression benchmarks.

Usage: ./geotiff2postgres.py <tiff file> 
"""
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

snappy_res = []
gz_res = []
bz2_res = []
lzma_res = []

snappy_sizes = []
gz_sizes = []
bz2_sizes = []
lzma_sizes = []

snappy_de_res = []
gz_de_res = []
bz2_de_res = []
lzma_de_res = []

def benchmark_one(data, algo, comp_var, decomp_var, size_var):
    t1 = time.time()
    comp = algo.compress(data)
    t2 = time.time()
    size_var.append(len(comp))
    comp_var.append(t2 - t1)

    t1 = time.time()
    decomp = algo.decompress(comp)
    t2 = time.time()
    decomp_var.append(t2 - t1)

def benchmark(data):
    global snappy_res, gz_res, bz2_res, lzma_res
    global snappy_de_res, gz_de_res, bz2_de_res, lzma_de_res
    global snappy_size, gz_size, bz2_size, lzma_size

    data = data.tobytes()

    benchmark_one(data, snappy, snappy_res, snappy_de_res, snappy_sizes)
    benchmark_one(data, zlib, gz_res, gz_de_res, gz_sizes)
    benchmark_one(data, bz2, bz2_res, bz2_de_res, bz2_sizes)
    benchmark_one(data, lzma, lzma_res, lzma_de_res, lzma_sizes)

# avoid a DecompressionBombWarning
Image.MAX_IMAGE_PIXELS = 1000000000

img = Image.open(sys.argv[1])
data = np.asarray(img)

width, height = data.shape
print(len(data.tobytes()))
sys.exit(0)
box_size = 500


# we'll break down the image into (up to) 500px*500px chunks
for x in range(0, width, box_size):
    for y in range(0, height, box_size):
        frame_width = min(width - x, 500)
        frame_height = min(height - y, 500)

        frame_data = data[x:x+frame_width,y:y+frame_height]
        benchmark(frame_data)

def print_results(name, var, sizes):
    print(name)
    print('min time: %f s' % min(var))
    print('max time: %f s' % max(var))
    print('ave time: %f s' % (sum(var)/len(var)))
    print('file size: %d' % sum(sizes))

print_results('SNAPPY COMP', snappy_res, snappy_sizes)
print_results('SNAPPY DECOMP', snappy_de_res, snappy_sizes)
print_results('GZ COMP', gz_res, gz_sizes)
print_results('GZ DECOMP', gz_de_res, gz_sizes)
print_results('BZ2 COMP', bz2_res, bz2_sizes)
print_results('BZ2 DECOMP', bz2_de_res, bz2_sizes)
print_results('LZMA COMP', lzma_res, lzma_sizes)
print_results('LZMA DECOMP', lzma_de_res, lzma_sizes)
