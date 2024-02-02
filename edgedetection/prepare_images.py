'''
Extract images, resize to smallest common size, save as 3-tuples of uint8 values in RGB order.
'''

import os
import zipfile
from PIL import Image
import numpy as np
import shutil


WIDTH = 2300
HEIGHT = 1848
PATH = "./imgzips"


shutil.rmtree("./images", ignore_errors=True)
os.makedirs("./images")
for f in os.listdir(PATH):
    if not f.endswith('.zip'):
        continue
    fname = os.path.splitext(f)[0]
    imname = fname.split('-')[0]
    print("Processing image {}...".format(imname))
    with zipfile.ZipFile(os.path.join(PATH, f),"r") as zip_ref:
        zip_ref.extract("{}/im0.png".format(fname), PATH)
    os.rename(os.path.join(PATH, fname, "im0.png"), os.path.join("./images", "{}.png".format(imname)))
    os.removedirs(os.path.join(PATH, fname))
    img = Image.open(os.path.join("./images", "{}.png".format(imname))).convert('RGB')#.save(os.path.join("./images", "{}.png".format(imname)))
    img = img.resize((WIDTH,HEIGHT), Image.LANCZOS)
    values = np.asarray(img.getdata(), dtype=np.uint8).reshape((img.size[1], img.size[0]*3))
    #np.savetxt(os.path.join("./images", "{}.csv".format(imname)), values, fmt='%u', delimiter=',')
    values.tofile(os.path.join("./images", "{}.bin".format(imname)))
