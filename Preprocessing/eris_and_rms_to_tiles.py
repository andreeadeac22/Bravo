"""
This script reads two .txt files, one corresponding to the ESRI grid values for a map
(look at https://en.wikipedia.org/wiki/Esri_grid ), and the other corresponding to the RMS
(Root Mean Square) values for the same cells. Then, four numbers: the top-left corner of a
rectangle begx,begy, and the length of its sides, lenx and leny. It writes in a file named
output.txt a list of lines, each corresponding to a single cell: its x,y coordinates and
the ESRI value and the RMS value for that cell.


How to call:
python eris_and_rms_to_tiles.py arthern_accumulation_bedmap2_grid.txt
	arthern_accumulation_rms_bedmap2_grid.txt 5 5 7 7
"""

import sys

eris = open (sys.argv[1], 'r')
rms = open (sys.argv[2], 'r')

ncols = int(eris.readline().split()[1])
nrows = int(eris.readline().split()[1])

#read the ncols, nrows lines from the RMS file as well:
rms.readline()
rms.readline()

begx, begy = int(sys.argv[3]), int(sys.argv[4])
lenx, leny = int(sys.argv[5]), int(sys.argv[6])

#read the xllcorner, yllcorner, cellsize value, NODATA value from both ESRI and RMS:
for k in xrange(4):
	eris.readline()
	rms.readline()


output = open("output.txt", "w")

#write a simple header:
output.write("Col\tRow\tESRI\tRMS\n")

for row, line in enumerate(eris):
	rmsline = rms.readline()
	if row<begy:
		continue
	elif row >= (begy + leny):
		break
	else:
		for col, valueS in enumerate(line.split()):
			rmsVals = rmsline.split()
			if col<begx:
				continue
			elif col > (begx + lenx):
				break
			else:
				rmsVal = rmsVals[col]
				output.write(str(col) + "\t" + str(row) + "\t" + valueS +"\t"+ rmsVal + "\n")
output.close()
eris.close()
rms.close()