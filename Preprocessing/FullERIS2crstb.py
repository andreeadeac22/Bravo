## @file

"""
This script reads three .txt files, ESRI grids for the Surface (Elevation) of ice,
ice thickness, and the ice bed (hard rock beneath the ice).
(look at https://en.wikipedia.org/wiki/Esri_grid ). Then, a config file.


How to call:
python FullERIS2crstb.py bedmap2_surface.txt
	bedmap2_thickness.txt bedmap2_bed.txt config.txt 

"""

import sys

arguments = sys.argv
## @test Test there are 5 arguments provided
assert len(arguments) == 5

## open the file containing the ESRI grid of surface elevation
surface = open (arguments[1], 'r')
## ESRI grid of ice thickness
thickness = open (arguments[2], 'r')
## ESRI grid of the depth of the bed rock that is under the ice
bed = open (arguments[3], 'r')
## The configuration file specifies the coordinates of the starting point
# As well as the dimensions of the segment that is processed
conf = open (arguments[4], 'r')
begy, begx = int(conf.readline().split()[1]), int(conf.readline().split()[1])
assert begx is not None
assert begy is not None
leny, lenx = int(conf.readline().split()[1]), int(conf.readline().split()[1])
assert lenx is not None
assert leny is not None


# read the xllcorner, yllcorner, cellsize value, NODATA value from both ESRI and RMS:
# these are metadata that we can use if needed
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
	# check if we've reached the segment we want to process
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

# make sure we close all files, especially output -- we're writing there!
output.close()
surface.close()
thickness.close()
bed.close()
conf.close()
