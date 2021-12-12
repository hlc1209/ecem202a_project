# Table of Contents
* Abstract
* [Introduction](#1-introduction)
* [Related Work](#2-related-work)
* [Technical Approach](#3-technical-approach)
* [Evaluation and Results](#4-evaluation-and-results)
* [Discussion and Conclusions](#5-discussion-and-conclusions)
* [References](#6-references)

# Abstract

Provide a brief overview of the project objhectives, approach, and results.

# 1. Introduction

This section should cover the following items:

* Motivation & Objective: What are you trying to do and why? (plain English without jargon)
* State of the Art & Its Limitations: How is it done today, and what are the limits of current practice?
* Novelty & Rationale: What is new in your approach and why do you think it will be successful?
* Potential Impact: If the project is successful, what difference will it make, both technically and broadly?
* Challenges: What are the challenges and risks?
* Requirements for Success: What skills and resources are necessary to perform the project?
* Metrics of Success: What are metrics by which you would check for success?

# 2. Related Work

# 3. Technical Approach

## Character recognition using Deep Learning

For the handwritten character recognition, the traditional method is to use OCR algorithms. However, for the Arduino Nano 33 BLE Sense we are using, most OCR algorithms require well over 1MB of flash space, and demand more computational capacity than a microcontroller can provide for real-time processing.

To develop real-time character recognition algorithms that can be used on microcontrollers, we use deep learning to build and train a model that can achieve about 85% accuracy on 0-9 and a-z handwritten characters. To make real-time processing possible on the Arduino Nano 33, we utilize a variety of  optimization skills to build a tiny convolutional neural network, which has only 50k parameters in total, and the inference time on the device is ~100ms.


### Dataset & Data Preprocessing

The dataset we used is Extended MNIST (EMNIST), which contains 814,255 characters in 62 classes. Compared to the MNIST dataset, it's about 12 times larger and includes a-z characters. Due to its huge scale, we didn't use other datasets to train the model.

We observed that when user draws strokes on the touch screen, there are four things that will cause a severe performance degradation in the real world: 

1. Handwritten characters have an arbitrary angle with respect to the touch screen. However, the dataset we used does not consider this situation.
2. Handwritten characters have an arbitrary size relative to the touch screen, but the training characters from the dataset all have the same size.
3. Handwritten characters have an arbitrary position relative to the touch screen, but the training characters in the dataset are all located in the center.
4. The strokes we obtain from the touch screen are a series of sampled points, which we then convert into a 1-bit character image for the deep learning model's input. But, the characters images in the dataset are all 8-bit grayscale. 

To address the issues listed above, as well as to improve the performance of the model in the real world, we make use of data augmentation. The preprocessing and augmentation sequence we used are:

1. Rescale the images to the float datatype of 0.0~1.0.
2. Randomly rotate the image within an angle from -15 degree to 15 degree. This is to solve the first problem mentioned above.
3. Randomly resize the original 28\*28 image to a new square image of size from 16\*16 to 34\*34. Therefore the training characters will have different size. This is to solve the second problem mentioned above.
4. Crop the resized image into a square image of size 32\*32 with random cropping positions. Thus, the training characters will be positioned at different position. This is to solve the third problem mentioned above.
5. Binarize image to 1-bit image. The threshold we use is 0.3. This will solve the final problem listed above.


The figures below are some of the training images after pre-processing and augmentation.

![Example of Training Inputs of Character e](media/input_e.png)

![Example of Training Inputs of Character H](media/input_H.png)


### Design Model Architecture
Due to the limitation of Arduino Nano 33 BLE, there are strict limitations on model architecture. The following requirements should be met for the success of this project:

1. The inference time should be equal or less than 120ms.

# 4. Evaluation and Results

# 5. Discussion and Conclusions

# 6. References
