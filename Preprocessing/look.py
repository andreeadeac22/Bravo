## @file
# Look at the data output from the ESRI preprocessor.
# Usage: python3 look.py [Thickness|Bed|Surface]

import sys

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

MAXDEPTH = -250

def pickColumn(row):
	"""We specify the column we care about in the argv[] and for every row, pick that."""
	if len(sys.argv) >= 2:
		if sys.argv[1] == "Bed":
			return row.Bed
		elif sys.argv[1] == "Surface":
			return row.Surface
	#return Thickness by default
	return row.Thickness

if __name__ == '__main__':
    datafile = pd.read_csv('output.csv')
    ## @test Check that datafile was assigned to the content of output.csv
    assert datafile is not None

    width = datafile.Col.max() - datafile.Col.min() + 1
    height = datafile.Row.max() - datafile.Row.min() + 1
    ## @test Check that the width is positive
    assert width > 0
    ## @test Check that the height is positive
    assert heigth > 0

    offset_width = datafile.Col.min()
    offset_height = datafile.Row.min()
    
    assert offset_width is not None
    assert offset_width is not 0
    assert offset_height is not None
    assert offset_heigth is not 0

    matrix = np.zeros((width, height))

    for i, row in datafile.iterrows():
    	data = pickColumn(row)
    	assert data is not None
        # remove large negatives to increase contrast: we only care about up to MAXDEPTH m
        if data < MAXDEPTH:
            data = 0
        matrix[row.Col - offset_width, row.Row - offset_height] = data

    plt.imshow(matrix,cmap='hot',interpolation='nearest')
    plt.show()


