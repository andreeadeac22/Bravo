"""
This script reads three .txt files, ESRI grids for the Surface (Elevation) of ice,
ice thickness, and the ice bed (hard rock beneath the ice).
(look at https://en.wikipedia.org/wiki/Esri_grid ). Then, a config file.


How to call:
python FullERIS2crstb.py bedmap2_surface.txt
	bedmap2_thickness.txt bedmap2_bed.txt config.txt 

"""

import sys

surface = open (sys.argv[1], 'r')
thickness = open (sys.argv[2], 'r')
bed = open (sys.argv[3], 'r')
conf = open (sys.argv[4], 'r')

begy, begx = int(conf.readline().split()[1]), int(conf.readline().split()[1])
leny, lenx = int(conf.readline().split()[1]), int(conf.readline().split()[1])

#read the xllcorner, yllcorner, cellsize value, NODATA value from both ESRI and RMS:
for k in xrange(6):
	surface.readline()
	thickness.readline()
	bed.readline()

output = open("output.csv", "w")

#write a simple header:
output.write("Col,Row,Surface,Thickness,Bed\n")

for row, line in enumerate(surface):
	thickline = thickness.readline()
	bedline = bed.readline()
	if row<begy:
		continue
	elif row >= (begy + leny):
		break
	else:
		for col, surfVal in enumerate(line.split()):
			thickVals = thickline.split()
			bedVals = bedline.split()
			if col<begx:
				continue
			elif col > (begx + lenx):
				break
			else:
				thickVal = thickVals[col]
				bedVal = bedVals[col]
				output.write(
					str(col) + "," + str(row) + "," +
					surfVal +","+ thickVal + "," + bedVal + "\n")

output.close()
surface.close()
thickness.close()
bed.close()
conf.close()
