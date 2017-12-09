# Robotics and Computer Vision - Final Project

## Part 1

In this we use `bundler_sfm` in order to perform reconstruction from a set of images.

At minimum you must install `gfortran` on your system in order to run the bundler_sfm package.

```
sudo pip install virtualenv
virtualenv env -p python3
source env/bin/activate
pip install -r requirements.txt
sudo apt-get install gfortran libjpeg-dev libblas-dev liblapack-dev jhead gobjc++ imagemagick
export PROJ_HOME=/mnt/c/Users/zacbl/Documents/GitHub/RUCompSci/rcv/final
export BUNDLER_PATH=$PROJ_HOME/bundler_sfm
export VL_PATH=$PROJ_HOME/vlfeat
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$BUNDLER_PATH/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$VL_PATH/bin/glnxa64
git clone https://github.com/vlfeat/vlfeat
cd vlfeat
make
cd ..
```


### APPENDIX A - David Lowe's SIFT File Format

While prof. Lowe's SIFT algorithm is patented, it is widely used for academic and research purposes by many individuals. One noteworthy use is in [bundler\_sfm](https://github.com/snavely/budler_sfm).

Bundler directs us to Lowe's site where he has an executable for a pre-compiled SIFT program which generates a file of keypoints which represents the output of the SIFT algorithm.

The issue which arises is that this executable distribution has not been updated since 2005. The Linux executable was not able to run on my system which effectively made bundler useless.

I was, however, able to obtain a sample file output of the keypoints descriptor file. I am writing here to specify the file format of Lowe's ASCII file which I deciphered in order to get bundler to run on Linux.

The file format specification is implemented as per the following description:

The first line of the file:

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

This section of the file is copied for every single keypointand descriptor.