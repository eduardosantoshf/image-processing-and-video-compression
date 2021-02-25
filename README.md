# CSLPProject

**Image Processing** and **Video compression** in C++, using **OpenCV** library

## Course
This project was developed under the [Complements On Programming Languages](https://www.ua.pt/en/uc/12830) course of [University of Aveiro](https://www.ua.pt/).

## Requirements
* **OpenCV** library
* C++ compiling tool (Xcode developer tools for Mac users)

## Milestones/Deliverables

### Deliverable 1

Implemented **video player** able to:
* Display video in **RGB**.
* Include a **watermark** into an image.
* Transform an image into other **color spaces**, namely **YUV** and **HSV** and visualize individually each color channel as grayscale image.
* Calculate and display the color **histograms** of an image.
* Convert a color image to **grayscale** and apply **histogram equalization**.
* Apply **gaussian** and **blurv filters to an image, exploring different **filter kernels**.
* Use **treshold segmentation algorithm**.
* Explore the use of the most common **morphological operators** in the segmented
images.
* Find image gradients (**Sobel, Scharr and Laplacian derivatives**).
* Apply the **Canny edge detection algorithm**, exploring its parameters.
* Convert between **RGB** and **YUV color spaces**. Taking into consideration the planar mode in the three possible subsampling modes of **YUV: 4:4:4, 4:2:2 and 4:2:0**.

### Deliverable 2

Class **WBitStream** and **RBitStream**, to manipulate (read/write) bits from a binary file**.

### Deliverable 3

Class **Golomb**, **entropy encoder** using **Golomb codes**.

### Deliverable 4

Using the Golomb coding algorithm, implemented a video codec for video sequences previously saved in files. The codec relies on block based **motion compensation** and **predictive coding**.

Class **LosslessIntraFrameCodec**, with the following requirements:
* Frames encoded using **spatial predictive coding** based on the
**non-linear predictor of JPEG-LS** or the **7 JPEG linear predictors**.
* **Entropy coding** performed using **Golomb codes**.
* All the information required by the decoder is included in the bit-stream
(video format, frame size, encoder parameters, etc.).

### Deliverable 5

Class **LosslessHybridCodec**, with the following requirements:
* The **block size** and the search area for inter-frame coding is an input parameter of the encoder.
* The **periodicity of the key** (intra) frames is an input parameter of the encoder.
* All the information required by the decoder are included in the bit-stream
(video format, frame size, block size, search area, code parameters, etc.).
* **Entropy coding** performed using **Golomb codes**.

## How to run

### Compile
    
On the **CMakeList.txt** file, change the **add_executable** field to the desired executable filename.
Inside the **/video-player/src/** directory, run the following commands:
```
cmake .
```
```
make
```

### Run

Run the file:
```
./[executable_name] [args]
```

The # of args can be found inside each #.cpp file (#: 1 - 9)