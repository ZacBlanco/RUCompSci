# Robotics and Computer Vision - Final Project

## Part 1

In this we use `bundler_sfm` in order to perform reconstruction from a set of images.

Run the following commands in order to properly set up the environment to run the structure-from-motion

```
sudo pip3 install opencv-contrib-python
sudo pip install virtualenv
virtualenv env -p python3
source env/bin/activate
pip install -r requirements.txt
sudo apt-get install gfortran libjpeg-dev libblas-dev liblapack-dev jhead gobjc++ imagemagick
export PROJ_HOME=/mnt/c/Users/zacbl/Documents/GitHub/RUCompSci/rcv/final
export BUNDLER_PATH=$PROJ_HOME/bundler_sfm
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$BUNDLER_PATH/bin
```

You'll also need to create a file called `sift.py` in the current directory (next to `Part1.py`) with the following contents. See Appendix A for information on the file that this python script produces.:

```python
#! /usr/bin/env python3

import sys
import io
import os
import math
import cv2 # requires opencv-contrib-python


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("ERROR")
        print("Must provide at least 2 arguments {INPUT_FILE} {OUTPUT_FILE}")
        sys.exit(1)
    input = sys.argv[1]
    output = sys.argv[2]
    sift = cv2.xfeatures2d.SIFT_create()
    kps, des= sift.detectAndCompute(cv2.imread(input, cv2.IMREAD_GRAYSCALE), None)
    kp_len = len(kps)
    torad = lambda x: math.pi/180*(x if x < 180 else -1*(x-180))
    with open(output, 'w') as f:
        f.write("{} 128\n".format(len(kps)))
        for x in range(len(kps)):
            k = kps[x]
            d = []
            d.append(des[x][:20].tolist())
            d.append(des[x][20:40].tolist())
            d.append(des[x][40:60].tolist())
            d.append(des[x][60:80].tolist())
            d.append(des[x][80:100].tolist())
            d.append(des[x][100:120].tolist())
            d.append(des[x][100:120].tolist())
            f.write(" ".join([str(round(k.pt[0], 3)), str(round(k.pt[1], 3)), str(round(k.size, 3)), str(round(torad(k.angle), 3))]))
            f.write('\n')
            for row in d:
                [f.write(' ' + str(int(h))) for h in row]
                f.write('\n')
```

and finally, a file called `RunBundler.sh` with the folowing contents in the same directory. It is a modified version of the `RunBundler.sh` script which is included with the `bundler_sfm` project.

```sh
#!/bin/bash
#
# RunBundler.sh
#   copyright (c) 2008-2013 Noah Snavely
#
# A script for preparing a set of image for use with the Bundler 
# structure-from-motion system.
#
# Usage: RunBundler.sh [config_file]
#
# config_file describes [optional] configuration options, including:
#
#   IMAGE_LIST=<list.txt> # file with input list of images / focal lengths
#   IMAGE_DIR=<dir> # directory containing the images you'd like to process
#   MATCH_WINDOW_RADIUS=<num> # only match images in a sliding 
#                             # window of size 2rad+1 
#   INIT_FOCAL=<num>   # value to use for initial focal length
#   FOCAL_WEIGHT=<num> # weight used to constrain focal length
#   TRUST_FOCAL=true   # tell bundler to trust the provided focal lengths
#   RAY_ANGLE_THRESHOLD=<num> # used to remove ill-conditioned points
#   USE_CERES=true # enables use of Ceres solver for bundle adjustment
#                  # (if this is enabled at compile time)
#   NUM_MATCHES_ADD_CAMERA=<num> # number of matches above which bundler
#                                # will definitely attempt to add a camera
#                                # (e.g., you might use 500)
#

BASE_PATH=$(dirname $(which $0));
BASE_PATH=./bundler_sfm

cp ./sift.py $BASE_PATH/bin/sift

if [ $BASE_PATH == "TODO" ]
then
    echo "Please modify this script (RunBundler.sh) with the base path of your bundler installation in the BASE_PATH variable.";
    exit;
fi

EXTRACT_FOCAL=$BASE_PATH/bin/extract_focal.pl

OS=`uname -o`

if [ $OS == "Cygwin" ]
then
    MATCHKEYS=$BASE_PATH/bin/KeyMatchFull.exe
    BUNDLER=$BASE_PATH/bin/Bundler.exe
else
    MATCHKEYS=$BASE_PATH/bin/KeyMatchFull
    BUNDLER=$BASE_PATH/bin/bundler
fi

TO_SIFT=$BASE_PATH/bin/ToSift.sh
TO_SIFT_LIST=$BASE_PATH/bin/ToSiftList.sh

# Default config options
IMAGE_DIR="./Images"
MATCH_WINDOW_RADIUS="-1"  # infinite window
FOCAL_WEIGHT="0.0001"
RAY_ANGLE_THRESHOLD="2.0"

# Read any user-provided config options
if [ $# -eq 1 ]
then
    echo "Using config file '$1'"
    source $1
else
    echo "Using default config options"
fi

if [ "$IMAGE_LIST" == "" ]
then
  # Prepare the list of images

  # Rename ".JPG" to ".jpg"
  for d in `ls -1 $IMAGE_DIR | egrep ".JPG$"`
  do 
      mv $IMAGE_DIR/$d $IMAGE_DIR/`echo $d | sed 's/\.JPG/\.jpg/'`
  done

  # Create the list of images
  find $IMAGE_DIR -maxdepth 1 | egrep ".jpg$" | sort > list_tmp.txt

  if [ "$INIT_FOCAL" == "" ]
  then
    # Extract focal lengths using Exif data
    $EXTRACT_FOCAL list_tmp.txt || exit 1
    cp prepare/list.txt .
  else
    # Use the provided focal length
    awk "{print \$1, 0, $INIT_FOCAL}" list_tmp.txt > list.txt
  fi

  IMAGE_LIST=list.txt
fi

# Run the ToSift script to generate a list of SIFT commands
if [ "$SKIP_FEATURES" != "true" ]
then
    echo "[- Extracting keypoints -]"
    rm -f sift.txt
    $TO_SIFT_LIST $IMAGE_LIST > sift.txt || exit 1

    # Execute the SIFT commands
    sh sift.txt
fi

# Match images (can take a while)
echo "[- Matching keypoints (this can take a while) -]"
awk '{print $1}' $IMAGE_LIST | sed 's/\.jpg$/\.key/' > list_keys.txt
sleep 1
echo $MATCHKEYS list_keys.txt matches.init.txt $MATCH_WINDOW_RADIUS
$MATCHKEYS list_keys.txt matches.init.txt $MATCH_WINDOW_RADIUS

# Generate the options file for running bundler 
mkdir -p bundle
rm -f options.txt

echo "--match_table matches.init.txt" >> options.txt
echo "--output bundle.out" >> options.txt
echo "--output_all bundle_" >> options.txt
echo "--output_dir bundle" >> options.txt
echo "--variable_focal_length" >> options.txt
echo "--use_focal_estimate" >> options.txt
echo "--constrain_focal" >> options.txt
echo "--constrain_focal_weight $FOCAL_WEIGHT" >> options.txt

if [ "$TRUST_FOCAL" != "" ]
then
    echo "--trust_focal" >> options.txt
fi

echo "--estimate_distortion" >> options.txt
echo "--ray_angle_threshold $RAY_ANGLE_THRESHOLD" >> options.txt

if [ "$NUM_MATCHES_ADD_CAMERA" != "" ]
then
    echo "--num_matches_add_camera $NUM_MATCHES_ADD_CAMERA" >> options.txt
fi

if [ "$USE_CERES" != "" ]
then
    echo "--use_ceres" >> options.txt
fi

echo "--run_bundle" >> options.txt

# Run Bundler!
echo "[- Running Bundler -]"
rm -f constraints.txt
rm -f pairwise_scores.txt
$BUNDLER $IMAGE_LIST --options_file options.txt > bundle/bundle.log

rm $BASE_PATH/bin/sift
cp -r prepare bundle/
rm -rf prepare
mv *.txt bundle/
rm $IMAGE_DIR/*.gz


echo "[- Done -]"
```

Once all of these files are in place, you can run bundle adjustment on any images located under the `Images/` directory. `bundler_sfm` will only accept jpg files, so all files should be stored in that format.

To run the full bundle adjustment and view the reconstruction simply run

    python Part1.py

To read from a bundle output file and display the reconstruction from the `{output_file}` (Optional argument, if not specified, it will attempt to read from a file `bundle/bundle.out`)

    python Part1.py read {output_file}

If your images are too large or reconstruction and point detection takes a long time you may want to consider downscaling the images to a smaller resolution. Note that this will remove EXIF metadata tags from images which `bundler_sfm` attempts to use for more accurate reconstruction.

    python Part1.py downscale

This will reduce all images underneath `Images/` to have at least 1 resolution dimension less than 800 pixels. It will append `_down` to the filename. The original images can be found under `./Images/orig/`


## Part 2 - Fine Tuning a Neural Network for Cats and Dogs

To set up the environment properly, first make sure you have created the python virtual environment (see directions from part1 on setup)

```
source env/bin/activate
pip install -r requirements.txt
```

Make sure that the images for classification are stored under `Images/keras/{classname}/`



You can run any of the parts from the assignment by passing arguments described in the help message below.

```
    --help                  Display this help message
    
    --imnet                 Uses ResNet50 to classify 20 images. Reports statistics

    --train {weights}       Trains a modified version ResNet50 to recognize cats and dogs
                            weights after training are saved to {weights} if specified.
                            Otherwise, weights are saved to 't1weights.h5'. The starting
                            weights for the network are the original weights from
                            ResNet50. We train on 200 random images from the Cats and
                            Dogs dataset (100 dogs, 100 cats)

    --test {weights} {num}  Loads weights from the {weights} file, then performs
                            predictions on test data. {num} is the number of random
                            samples to predict for each class of cat and dog

    --loadtrain {weights}   Loads weights from the {weights} file, then trains and
                            refines those weights using more training data, then performs
                            predictions on test data. New weights are automatically saved
                            to 't1weights.h5`
```

For example

    python Part2.py --train myweights.h5

Will train the fine-tuned ResNet50 for cats and dogs

### APPENDIX A - David Lowe's SIFT File Format

While prof. Lowe's SIFT algorithm is patented, it is widely used for academic and research purposes by many individuals. One noteworthy use is in [bundler\_sfm](https://github.com/snavely/budler_sfm).

`bundler_sfm` directs us to Lowe's site where he has an executable for a pre-compiled SIFT program which generates a file of keypoints which represents the output of the SIFT algorithm.

The issue which arises is that this executable distribution has not been updated since 2005. The Linux executable was not able to run on my system which effectively made `bundler_sfm` useless.

I was, however, able to obtain a sample file output of the keypoints descriptor file. The purpose of this section is to specify the file format of Lowe's ASCII file which I deciphered in order to get `bundler_sfm` to run on my linux machine.

The file format specification is as follows:

The first line of the file is:

```
{Number of keypoints} {Length of descriptor}
```

Typically there are 128 descriptor bins.

Following the first line is the description of every keypoint.

```
{X coord} {Y coord} {size} {angle}
 {desc0} {desc1} {desc2} {....} {desc19}
 {desc20} {desc1} {desc2} {....} {desc39}
 {desc40} {desc1} {desc2} {....} {desc59}
 {desc60} {desc1} {desc2} {....} {desc79}
 {desc80} {desc1} {desc2} {....} {desc99}
 {desc100} {desc1} {desc2} {....} {desc119}
 {desc120} {desc1} {desc2} {....} {desc127}
```

The `X` and `Y` coordinates should be floating point values rounded to 2 decimal places. The `size` should also be rounded to 2 decimal places. 

`angle` is the angle of the descriptor in **radians**. The value range is `[-pi, pi]` Where angles > 180 are negative values, and angles < 180 are positive.

This section of the file is copied for every single keypointand descriptor and placed one after another sequentially in the file. There are no special file endings or other metadata. The file should only include ASCII characters.

