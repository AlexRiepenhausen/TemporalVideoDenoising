# Temporal Video Denoising for Nonmoving Smartphone Videos <br/>

This small passion project of mine is designed to tackle one of the problems I have as a hobbyist filmmaker.<br/>
Often times, I find myself in a situation where I see something interesting and I want to film it, <br/>
but the only device available to me is my smartphone because I cannot always be carrying around a camera. <br/>
Thus, the combination of a really small sensor with potential low light conditions inevitably results
in very noisy footage. Even though the subject may be interesting, the results can sometimes be borderline unuseable.<br/>
Hence, I have decided to spend a week or two on implementing a denoising algorithm to help me out with this problem <br/>

## The Idea

In photography, there is a technique known as **image stacking** where you take several images of a single scene <br/>
and average them, effectively simulating a longer exposure time. Given the assumption of image noise being a Gaussian <br/>
distribution, the more images you average the better your signal-to-noise ratio will be. <br/>
Implementing this technique for video is going to be a bit more tricky though, <br/>
simply because the scene most likely will contain moving objects and may be changing due to camera movement. <br/>
Yet, there are ways of implementing this technique for video with non-moving cameras and it is called **temporal denoising** <br/>

## The Implementation

The graph below shows the RGB values of a single pixel over roughly 450 frames. <br/>
As we can see from the graph, the pixel values take a dip every 70 frames, indicating some form of periodic movement. <br/>
In between these dips the pixel values seem to be dancing around some mean value they are supposed to represent. <br/>
This phenomenon is the image noise we want to get rid off, and the way to do it is by smoothing the RGB curve without
distorting the parts representing real movement too much. <br/>

![alt text](https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/pixelvals.png)

### Separating Noise from Real Movement

One possible choice for distinguishing between noise and real movement is the rolling variance taken over multiple frames, <br/>
in this case four. Variance represents the squared distance between the mean and the individual pixel values. <br/>
If we look at the graph below, we can see how the rolling variance reacts to the previously mentioned dips and plateaus: <br/>

![alt text](https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/variance_four_frames.png)

### Averaging Nonmoving Parts of the Image

As we have seen, the rolling average is quite useful in establishing

![alt text](https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/pixelvalues.png)
![alt text](https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/examples/example03_after.png)


