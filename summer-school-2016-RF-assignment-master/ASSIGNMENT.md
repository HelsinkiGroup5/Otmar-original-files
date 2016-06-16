Exercise
========

RF Training
-----------

In this exercise, you will experiment with several aspects pertaining
Random Forests (RFs) training, in a multi-class classification task.
We will work with static hand gestures data.
To do so, we are going to extract, from a dataset of real
hand-gestures images, custom-engineered features, as described in [Song
et al.](http://ait.inf.ethz.ch/projects/2014/InAirGesture/).
Before starting working on the code, we strongly recommend to read, understand, and get familiar with the paper.

In the repository you will find skeletal code that you are required to fill.
There’s also a `README` file that summarises the files you will need to
modify. The following files have to be adapted to complete the various
tasks:

	weak_learner.h
	image_weak_learner.h
	histogram_statistics.h

Places where code should be placed are marked with a `TODO UIE A2` comment block.

### Step 1: Information Gain
----------------------------

In this first step, you are required to implement the information gain
function in order to perform and asses node splits of your tree. You
will need to implement:

1.  The information gain metric as described in [Song
et al.](http://ait.inf.ethz.ch/projects/2014/InAirGesture/).
See section *Hand State Classification Method* in the paper and class `WeakLearner` in the code;

2.  The [Shannon Entropy](https://en.wikipedia.org/wiki/Entropy_(information_theory)). See the `HistogramStatistics` class in
    the code.

Please refer to the comments in code for more details.

### Step 2: Feature Response and Selection
------------------------------------------

In this second step, you are required to implement the feature response
and selection introduced by [Song
et al.](http://ait.inf.ethz.ch/projects/2014/InAirGesture/).
You will need to implement:

1.  The feature response as per [Song
et al.](http://ait.inf.ethz.ch/projects/2014/InAirGesture/).
    See section *Hand State Classification Method* in the paper and the
    struct `ImageFeature` and class `ImageSplitPoint` in the code.

2.  The random sampling of the features and thresholds. See the class
    `ImageWeakLearner` in the code.

Please refer to the comments in code for more details.

### Step 3: Fine Tuning
-----------------------

In this step, you will learn how to fine-tune your RF model. The trainer
application allows you to specify a variety of parameters at runtime and
through a configuration file. For this step, we are interested in
exploring only the effect of forest size (i.e., number of trees) and
individual tree depth on the model accuracy. These parameters can be set
via the attributes `num_of_trees` and `tree_depth` in the
`training_parameters` block of the `.yaml` config file (see
`./data/config/test.yaml `for an example).

Plot performance-vs-depth and performance-vs-forest size. What is the
most impacting factor for this task? Why do you think this is the case?

Additionally, you can also experiment with number of random features
used for training. To do this, you will need to change the value
assigned to the variable `samples_per_image_fraction` in
`image_weak_learner.h`.

In order to fine-tune your model, we suggest you write a script (using
the language of your choice) that:

-   Iterates over sensible ranges for tree depth and forest size;
-   Creates a configuration file with the current parameters and pass it
    to the trainer application to generate a model. **Make sure the new
    configuration file has all the values which are in the template, or
    the application will not run correctly**.
-   Evaluates the model and saves on disk the accuracy and
    parameters set. As the test application already computes and prints
    the accuracy, you can either modify the source to save this on disk,
    or you can grab it from the console directly in your script.

Please be aware that with increasing tree depth, training time (and
memory requirement) will grow. Here’s some stub Python code to
illustrate this process:

    ...

    #specify the parameters to train the forest
    the_tree_depth = [16, 18, 20, 22]
    the_forest_size = [1, 2, 3, 4, 5]

    ...

    for _forest_size in the_forest_size:
        for _depth in the_tree_depth:

            maximum_depth = _depth;
            num_of_trees = _forest_size;

            config_file_name = base_directory + "/configuration_" + \
            "_D" + str(maximum_depth) + \
            "_T" + str(num_of_trees) + ".yaml"

            forest_file_name = base_directory + "/forest_" +  \
            "_D" + str(maximum_depth) + \
            "_T" + str(num_of_trees) + ".json"

            print("Writing config file...")
            write_config_file(config_file_name, \
            maximum_depth, \
            num_of_trees)

            print("Training...")
            run_train( train_data_path, config_file_name, \
            forest_file_name, \
            num_gestures, \
            num_gestures)

            print("Testing...")
            pix_acc, frame_acc = run_test(  train_data_path, \
            forest_file_name, \
            num_gestures, \
            num_gestures)

            #write out the accuracy
            ...
