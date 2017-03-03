import numpy as np

f = open("try.txt", "r")
contents = f.readlines()
f.close()

contents.insert(2, "00000")

f = open("try.txt", "w")
contents = "".join(contents)
f.write(contents)
f.close()


f=open("decomp_data/(0,18001)","r")
data = np.fromfile(f, dtype=np.uint32)   #put data from file in array
print data[0]
print data[1]
print data[199]
print data[200]
print data[201]
print len(data)
f.close()

data = np.insert(data,200,111)

data = np.delete(data,0)

f=open("decomp_data/(0,18001)","w")
print data[0]
print data[1]
print data[199]
print data[200]
print data[201]
print len(data)
f.write(data)
f.close()
