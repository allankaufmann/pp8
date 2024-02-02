'''
Displays image in CSV or binary RGB format. Pass file name as command line argument.
'''


import sys
#import csv
import numpy as np
import matplotlib.pyplot as plt


IMGWIDTH = 2300
IMGHEIGHT = 1848


################################################################
# For binary, assuming 8 bit unsigned integers in RGB order
################################################################
img = np.fromfile(sys.argv[1], np.uint8).reshape(IMGHEIGHT, IMGWIDTH, 3)
################################################################
# For csv format
################################################################
# img = np.zeros((IMGHEIGHT,IMGWIDTH,3), np.uint8)
# with open(sys.argv[1], 'r') as imgfile:
#     csvrd = csv.reader(imgfile)
#     row = 0
#     for line in csvrd:
#         for pix in range(IMGWIDTH):
#             img[row,pix] = [int(line[pix*3]), int(line[pix*3+1]), int(line[pix*3+2])]
#         row += 1
################################################################
plt.imshow(img)
plt.show()
