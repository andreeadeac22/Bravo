"""
Look at the data output from the ESRI preprocessor.

Usage:

python3 look.py [Thickness|Bed|Surface]
"""
import sys

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

if __name__ == '__main__':
    df = pd.read_csv('output.csv')

    w = df.Col.max() - df.Col.min() + 1
    h = df.Row.max() - df.Row.min() + 1

    offset_w = df.Col.min()
    offset_h = df.Row.min()

    matrix = np.zeros((w, h))

    data = {}

    for i, row in df.iterrows():
        if len(sys.argv) < 2:
            data = row.Thickness
        elif sys.argv[1] == 'Bed':
            data = row.Bed
        elif sys.argv[1] == 'Thickness':
            data = row.Thickness
        elif sys.argv[1] == 'Surface':
            data = row.Surface
        else:
            data = row.Thickness
        matrix[row.Col - offset_w, row.Row - offset_h] = data

    plt.imshow(matrix,cmap='hot',interpolation='nearest')
    plt.show()


