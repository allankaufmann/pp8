'''
Displays image in CSV or binary greyscale format. Pass file name as command line argument.
'''


import sys
#import os
#import csv
import numpy as np
import matplotlib.pyplot as plt


IMGWIDTH = 2300
IMGHEIGHT = 1848


################################################################
# For binary, assuming 8 bit unsigned integers
################################################################
img = np.fromfile(sys.argv[1], np.uint8).reshape(IMGHEIGHT, IMGWIDTH)
################################################################
# For csv format
################################################################
# img = np.zeros((IMGHEIGHT,IMGWIDTH), np.uint8)
# with open(sys.argv[1], 'r') as imgfile:
#     csvrd = csv.reader(imgfile)
#     row = 0
#     for line in csvrd:
#         for pix in range(len(line)):
#             img[row,pix] = int(line[pix])
#         row += 1
#print(np.shape(img))
################################################################
plt.imshow(img, cmap='gray')
#plt.savefig(os.path.splitext(sys.argv[1])[0] + '.png')
plt.show()
