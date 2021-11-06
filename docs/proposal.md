## M202A Course Project

# 1. Motivation & Objective

Designers have long recognized that handwriting with a pen offers more fluidity compared with typing. After the keyboard has been the dominant input device for electronics for years, stylus pen comes into play as another commonly-used input device for electronics. While stylus pen becomes increasingly popular over the years, problems remain in this type of device, which could be turned into sizable market if properly solved. 

![](media/ipad_scribble.jpg)

First, the cost of handwritten-enabled devices with a smooth experience is high. Currently, multiple solutions exist for scribble digitization. For example, Microsoft Surface Pro has a built-in Wacom-made digitier layer and a pen designed for it to achieve such functionality. Apple iPad introduced the scribble feature in iOS 14 with an extremely enhanced seamless handwriting input experience, but it requires the powerful computing power of the A14 chip and a separately sold apple pencil. Other solutions like capacitive and bluetooth stylus have readily lower cost. Still, they can only work with devices with touchscreen enabled and rely on hardware-specific optimized software to deliver a good experience. It is undeniably true that the vast majority of low-end devices offer a very poor handwriting experience.

Secondly, hardware support is relatively stringent. Most of the handwritten solutions for electronics does not generalize well to different devices and mainly focus on the need of high-end electronics. Also, if handwritten characters are used as input, the recognition of handwritten characters becomes the burden on the interfaced device, placing demands on the system's software and hardware performance, increasing the system's complexity and reducing its reliability and real-time performance.

To tackle the aforementioned problems, we propose to develop a low-cost and low-power device with a capacitive touchscreen that is able to offers a seamless handwriting input experience and send commands/keyboard strokes to bluetooth low energy (BLE) enabled devices as an alternative to conventional keyboard input. Such device is essentially a new type of human interface devices (HID) and able to work universally with any BLE enabled devices with minimum change in firmware. Since the handwritten recognition is done on this embedded device, there would be little hardware constraints for the interfaced devices except BLE capability. 

# 2. State of the Art & Its Limitations

In 2018, Fernández et. al developed a real-time handwritten letter recognition system based on Raspberry Pi 3 using ConvNets which is able to achieve an accuracy of 93.4% and an average response time of 21.9 ms. This is the best performance of all similar systems that run the machine learning model offline considering accuracy, response, power consumption, and size. 

However, limitations remain on this system and major improvement can be achieved on this type of system. First, the machine learning model used in this system can be further compressed with proper pruning, quantization, and other model compression techniques. Second, by replacing the Raspberry Pi 3 with a low-power microcontroller (i.e., the Arduino Nano 33 BLE Sense in our case), the power consumption and size of the system can be further reduced. Finally and most importantly, this article only demonstrates the feasibility of individual character recognition using deep learning networks in embedded systems, and does not illustrate the performance of handwriting recognition in real-world applications where efficiency, usability, and accuracy are extremely important.

In 2019, Google Research uses recurrent neural network to enable seamless handwriting input on Android devices. Unlike traditional hand-designed heuristics to cut the handwritten input into single characters, Google build an RNN model that operates on the whole input. They convert a series of handwritten inputs into a sequence of Bessel curves that are fed into the RNN to get the actual written characters.

Nevertheless, this powerful approach relies on strong chips on Android devices and even requires specially designed chips like Google Tensor with TPU cores to deliver smooth interactions, which is unacceptable in our project.

# 3. Novelty & Rationale

Nowadays, seamless handwritten recognition solutions either work on specialized devices or touchscreen enabled high-end electronics, e.g., BMW iDrive infotainment system and Apple iPad. Also, the handwritten recognition algorithm of these devices typical requires the support of powerful processors. We envision a system with a small touch screen built in to collect handwritten inputs and infer the written characters on chip before sending out the commands/keyboard strokes. 

Therefore, high compatibility and seamless input experience are the main aspects that makes our proposed device stand out from other touchscreen solutions available in the market. 

Moreover, given the flexibility of handwritten input, our device enables fast execution of complex commands by binding a custom stroke to a sequence of instructions. Simultaneously, the device has unlimited possibilities for functional expansion. For example, performing calculations on handwritten numbers and symbols and transmitting the final results to the terminal equipment via BLE.

# 4. Potential Impact

If successfully made, this device could become a new type of human interface device that is compatible to any BLE enable devices not limited to high-end electronics like laptops and tablets. For instance, it could be used to recognize and convert user's writings to keyboard strokes to replace the functionality of a keyboard upon user's desire. Similarly, it could also become an input device of speech-synthesizer for speech impaired people. 

In short, such device brings a new way of electronic interface with high performance, great compatibility and low cost. 

# 5. Challenges

First of all, the experience of handwriting recognition input is critical, i.e., low latency, high accuracy and high efficiency. After we decided to use on-device deep learning inference for handwriting character recognition, it became pivotal to implement fast and accurate model inference while handling GUI drawing, user stroke input and BLE transmission on an ultra-low power device. Secondly, how to achieve seamless handwriting input on ultra-low power devices is to be overcome. We need a strong collaboration of deep learning models and GUI to achieve this. Likewise, data transmission via BLE is very important and needs to be user friendly, low latency and highly reliable. 

# 6. Requirement for Success

To build such a system, the following skillsets and resources are needed.

## Skillsets
* Embedded system
* Real-time operating system
* Bluetooth Low Energy (BLE)
* SPI and I2C
* Electronics
* Machine Learning and model compression

## Skillsets potentially in need
* PCB design
* Mechenical Design

## Hardware
* Arduino Nano 33 BLE Sense
* Adafruit 2.8" TFT Touch Shield v2 (capacitive touch)
* Breadboard

## Hardware potentially in need
* Boost converter
* 3d Printer

# 7. Metrics of Success

The successfulness of the project will be evaluated through the following metrics. 

1. Accuracy:
   Successful rate of recognitzing the written characters.

2. Response time:
   The time from a character is written to the inference is done.

3. Power consumption:
   The power consumption of the entire system.

4. Weight:
   The weight of the overall system.


# 8. Execution Plan

The project can be partitioned into 5 mandatory tasks and 1 optional task:

1. Development of the embedded system.
   Interface the touchscreen to obtain handwritten characters properly. Develop BLE connection between Arduino Nano 33 BLE Sense and laptop. 

2. Machine learning model development:
   Develop and compress machine learning model to meet the accuracy and response time criteria with respect to the hardware constraint in static settings.

3. System Integration:
   Fully integrate the machine learning model with touchscreen and BLE function enabled. Ensure the real-time feature of the system. 

4. Model Optimization:
   Optimize the machine learning model to improve accuracy and reduce the run-time of the model. 

5. System optimization:
   Reduce the latency of the system and improve the user experience of the system.

6. System packaging (Optional):
   Migrate the system from breadboard to a perfboard/PCB with proper power system if time allowed. Design a 3D-printed package to minimize the size of the entire system. 

Yu Nong will be mainly responsible for task 1, 3, 5, 6 and Hanlong Chen will be mainly responsible for task 2, 3, 4, 6.

The proposed timeline is the following.

## Week 1-2 (Oct. 4-17)

* Research for project ideas with focus on human interface device (HID)
* Form a team of 2 considering the scope and difficulty of the project.
  
## Week 3 (Oct. 18-24)

* Create project website on Github and finish the section of abstract.
* Discuss project idea and validate the feasibility.
* Place order for necessary components.
* Research into the user Manual and tutorial of the purchased components.

## Week 4 (Oct. 25-31)

* Finalize detailed project ideas and create timeline for the project. 
* Analyze applications and review literatures that are similar to this project.
* Start wiring and testing of the touchscreen.
* Start collecting datasets for DL model training and building the model architecture using PC.

## Week 5 (Nov. 1-7)

* Finish wiring and interfacing the touchscreen with Arduino Nano 33 BLE Sense.
* Train the DL model using GPU and keep adjusting the architecture to obtain better performance with smaller model sizes.

## Week 6 (Nov. 8-14)

* Finish BLE connection between Arduino Nano 33 BLE Sense and PC with HID keystroke input.
* Attempt to run trained models on Arduino Nano 33 BLE Sense and profiling the performance to adjust the architecture.

## Week 7-8 (Nov. 15-21)

* Finish pruning, digitization, and model compression on ML model.
* Implement the compressed ML model in Arduino Nano 33 BLE Sense. 
* Achieve real-time DL model inference on Arduino Nano 33 BLE Sense with decent recognition accuracy.
* Achieve a basic graphical user interface, including handwriting input area and candidate words area.
* Resolve issues on timing and ensure the system meets the minimum requirement of real-time response.
* Prepare a demo video and report as a progress update.

## Week 9-10 (Nov. 22- Dec. 5)

* Tune the and further compress the ML model to improve the performance on both accuracy and timing.
* Add more functions to the GUI to enable more interesting interactions.
* Debug issues found through testing.
* Consider to make a 3D-printed case to package the system.

## Week 11 (Dec. 6-11)
* Evaluate the performance of the furnished system and compose the final report.
* Update the project website with detail testing results, source code, and thorough documentation.
* Prepare Zoom presentation. 

# 9. Related Work

## 9.a. Papers

## 9.b. Datasets

## 9.c. Software

# 10. References

D. Núñez Fernández and S. Hosseini, "Real-Time Handwritten Letters Recognition on an Embedded Computer Using ConvNets," 2018 IEEE Sciences and Humanities International Research Conference (SHIRCON), 2018, pp. 1-4, doi: 10.1109/SHIRCON.2018.8592981.

Saldanha, Luca Bochi and Christophe Bobda. “An embedded system for handwritten digit recognition.” J. Syst. Archit. 61 (2015): 693-699.

Carbune, Victor, et al. "Fast multi-language LSTM-based online handwriting recognition." International Journal on Document Analysis and Recognition (IJDAR) 23.2 (2020): 89-102.

Feuz, Sandro, and Pedro Gonnet. “RNN-Based Handwriting Recognition in Gboard.” Google AI Blog, 7 Mar. 2019, http://ai.googleblog.com/2019/03/rnn-based-handwriting-recognition-in.html. 

# Literature Analysis

The key points to the success for our project are
1. High accuracy real-time written character recognition using deep learning on a lower-cost and low-power embedded system.
2. High-reliability low-latency HID devices emulation via BLE.
3. Effective user interface design with continuous input and behavior customization capabilities.

## Real-Time Handwritten Letters Recognition on an Embedded Computer Using ConvNets
An article closely related to our project is "Real-Time Handwritten Letters Recognition on an Embedded Computer Using ConvNets" by D. Núñez Fernández and S. Hosseini. 

While this article demonstrates the basic feasibility of our project, the plan for our project goes beyond the level demonstrated in this article.

In this article, the described method is implemented using the Raspberry Pi 3 which comes with 1GB RAM and 1.2 GHz main clock speed, we are aiming to reproduce a system with similar performance using Arduino Nano 33 BLE Sense which has only 256KB RAM and 64 MHz main clock speed. In this paper, authors were able to achieve an accuracy of 93.4% with a response time of 21.9 ms. Such high performance in both accuracy and response time is at the cost of a more expensive and power-consuming device. However, we notice that the machine learning models implemented in Raspberry Pi 3 are not optimized using any of the emerging model compression techniques such as pruning and quantization. Therefore, it is feasible to achieve similar performance on a less power device with a much more compressed model with reasonable cost in accuracy and response time. 

The proposed method in the article performance only one task, which simply outputs the inference of the handwritten character. Therefore, scheduling of different tasks would not be a problem in this desgin. In our anticipated system, we propose to extend the funtionality of the system to interact with external device throught BLE based on the inferenced of handwritten characters. In this context, two tasks exist simutaneously. One is the recognition of handwritten characters and the other one is the BLE service for the interface between devices. With the sporadic nature of the first task, the scheduling of these two events needs to be carefully evaluated from other sources as this article did not cover this topic.

### Character recognition

### Hardware 


