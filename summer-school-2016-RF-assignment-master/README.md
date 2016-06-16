# Random Forests (RFs) Trainer
### Instructors: Otmar Hilliges, Fabrizio Pece, Benjamin Hepp
### Code Credits: Benjamin Hepp


Setting-up the Environment 
==========================

In this repository, you will find a sub-folder named “cpp”, with a fully
functional Random Forest framework that will help you during this
exercise. Please work on these files, but **DO NOT** push to this
repository. Keep your changes either locally or in a seperate repository. Additionally, **please DO
NOT push any data used to produce your results, nor your trained forests (these will be large).**

You can get the data required for training and testing from
[here](https://ait.inf.ethz.ch/public-data/train_test_7gestures_RF.zip).
Once you have downloaded the dataset, unzip it somewhere on
your hard drive, then run the MATLAB script
`convert_mat_file_to_images.m` that you can find in the `.\data`
sub-folder. When prompted, select the mat file you want to process, and
then choose the destination folder. To keep things
organised, and assuming you are processing the training data, you should
have a folder structure that resembles the following:

	.\data\train\images\

	.\data\train\images\images.csv

Compiling the Code
------------------

The framework was tested under *nix systems (Linux/Mac OS X), and builds
via CMake. 
If you do not have access to a Unix environment, we have
created a virtual machine image to run via [Oracle VM
VirtualBox](http://www.oracle.com/technetwork/server-storage/virtualbox/downloads/index.html).
You can find the image
[here](https://ait.inf.ethz.ch/public-data/UIE16_Ubuntu_VM.ova) (~
2GB).

On your own system you will need to install the following dependencies:

-   CMake (3.1.3 or later)
-   Eigen3 (3.1.2 or later)
-   Boost (1.4.0 or later)

You can install these packages via `apt-get` on Linux and [`Homebrew`](https://github.com/Homebrew/brew/) on Mac
OS X. 
To ensure you install the correct version of CMake, follow the
instructions
[here](http://askubuntu.com/questions/610291/how-to-install-cmake-3-2-on-ubuntu-14-04).
The virtual machine image should already have all the required packages
installed. The root password for it is *uie2016*. 

Once you have installed the required libraries, you can build the source
via the usual CMake work flow:

      cd PATH_TO_THE_CPP_FOLDER
      mkdir build
      cd build
      cmake ../.
      make 
      

The last command will compile the source and will produce two binaries.
Alternatively, you can run `cmake-gui` for a graphical version of CMake.
On OSX, use the `-G Xcode` to generate an XCode project. More
information on CMake generators can be found
[here](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html).

Training
--------

The framework comes with two applications: one for train time one for test time. 
*Note:* For these applications to be useful you will need to complete the assignment outlined in ASSIGNMENT.md first.

Assuming all steps in Section \[coding\] have succeeded, you can train your model by
issuing the following command in a shell:

     ./depth_forest_trainer -f TRAIN_PATH/images.csv \
     -j TRAIN_PATH/forest_name.json \
     -l 7 -n 7 \
     -c TRAIN_PATH/config.yaml
     

Here, the flag `-l` specifies the number of classes, while the flag `-n`
the label assigned to background pixels. Please do not change these
flags when using the provided dataset.

Testing 
-------

The framework also comes with a testing application to evaluate your
model and produce performance metrics (i.e., per-pixel and per-frame
confusion matrix and accuracy). You can run the evaluation application
via the command:

      ./forest_predictor -f TEST_PATH/images.csv \
      -j TRAIN_PATH/forest_name.json \
      -l 7 -n 7
       
