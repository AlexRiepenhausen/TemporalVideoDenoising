# Temporal Video Denoising for Nonmoving Videos <br/>

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

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/pixelvals.png" width="500">
</p>

### Separating Noise from Real Movement

One possible choice for distinguishing between noise and real movement is the rolling variance taken over multiple frames, <br/>
in this case four. Variance represents the squared distance between the mean and the individual pixel values. <br/>
If we look at the graph below, we can see how the rolling variance reacts to the previously mentioned dips and plateaus: <br/>

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/variance_four_frames.png" width="500">
</p>

### Averaging Nonmoving Parts of the Image

As we have seen, the rolling average is quite useful for determining whether something should be considered <br/>
noise or legitimate movement. The table below shows a numerical example of how rolling variance is affected <br/>
by sudden changes in pixel value. Also, the last row in said table shows us how many of the past frames should <br/>
be considered in the image or frame stacking process given the changes in pixel value. <br/>
With that, the next step is to find a way of connecting the variance values with the required number of past frames. <br/>

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/pixelvalues.png" width="500">
</p>

The variance values can be used for computing an averaging score, i.e. the probability of a pixel value being <br/>
legitimate movement. If this value is zero, then the pixel is guaranteed to be random noise. <br/>
Conversely, a value of one represents real movement within the scope of this model. A sigmoid function with hand-tuned <br/>
parameters was chosen for this task. Originally, a hard variance threshold was considered but ultimately abandoned <br/>
due to remaining noise artifacts. With the sigmoid function, variance edge cases are less jarring in the resulting video. <br/>

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/determining_avg_strength.png" width="500">
</p>

Now that we have the averaging score, we can perform a series of steps to determine how much attention we should pay to <br/>
the current pixel in comparison to the previous pixels. The higher the score, the less weight the previous pixels get <br/>
when computing the new average pixel value: <br/>

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/calculation.png" width="500">
</p>

## The Results?
So much for the theory. What about the actual results? 
<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/examples/example01_before.png" width="500">
</p>

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/examples/example01_after.png" width="500">
</p>
