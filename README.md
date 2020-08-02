# Temporal Video Denoising for Nonmoving Videos <br/>

This small passion project of mine is designed to tackle one of the problems I have as a hobbyist filmmaker.
Often times, I find myself in a situation where I see something interesting and I want to film it, 
but the only device available to me is my smartphone because I cannot always be carrying around a camera. 
Thus, the combination of a really small sensor with potential low light conditions inevitably results in very noisy footage. 
Even though the subject may be interesting, the results can sometimes be borderline unuseable.
Hence, I have decided to spend a week or two on implementing a denoising algorithm to help me out with this problem.

## The Idea

In photography, there is a technique known as **image stacking** where you take several images of a single scene
and average them, effectively simulating a longer exposure time. Given the assumption of image noise being a Gaussian
distribution, the more images you average the better your signal-to-noise ratio will be. 
Implementing this technique for video is going to be a bit more tricky though,
simply because the scene most likely will contain moving objects and may be changing due to camera movement.
Yet, there are ways of implementing this technique for video with non-moving cameras and it is called **temporal denoising** 

## The Implementation

The graph below shows the RGB values of a single pixel over roughly 450 frames.
As we can see from the graph, the pixel values take a dip every 70 frames, indicating some form of periodic movement.
In between these dips the pixel values seem to be dancing around some mean value they are supposed to represent.
This phenomenon is the image noise we want to get rid off, and the way to do it is by smoothing the RGB curve without
distorting the parts representing real movement too much.

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/pixelvals.png" width="500">
</p>

### Separating Noise from Real Movement

One possible choice for distinguishing between noise and real movement is the rolling variance taken over multiple frames,
in this case four. Variance represents the squared distance between the mean and the individual pixel values.
If we look at the graph below, we can see how the rolling variance reacts to the previously mentioned dips and plateaus:

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/variance_four_frames.png" width="500">
</p>

### Averaging Nonmoving Parts of the Image

As we have seen, the rolling average is quite useful for determining whether something should be considered
noise or legitimate movement. The table below shows a numerical example of how rolling variance is affected
by sudden changes in pixel value. Also, the last row in said table shows us how many of the past frames should
be considered in the image or frame stacking process given the changes in pixel value. <br/>
With that, the next step is to find a way of connecting the variance values with the required number of past frames.

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/pixelvalues.png" width="500">
</p>

The variance values can be used for computing an averaging score, i.e. the probability of a pixel value being
legitimate movement. If this value is zero, then the pixel is guaranteed to be random noise.
Conversely, a value of one represents real movement within the scope of this model. A sigmoid function with hand-tuned
parameters was chosen for this task. Originally, a hard variance threshold was considered but ultimately abandoned
due to remaining noise artifacts. With the sigmoid function, variance edge cases are less jarring in the resulting video.

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/determining_avg_strength.png" width="500">
</p>

Now that we have the averaging score, we can perform a series of steps to determine how much attention we should pay to
the current pixel in comparison to the previous pixels. The higher the score, the less weight the previous pixels get
when computing the new average pixel value:

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/calculation.png" width="500">
</p>

## The Results?
So much for the theory. Here are some actual side by side comaprisons between denoised and original video frames:

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/examples/example01_before.png" width="500">
</p>

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/examples/example01_after.png" width="500">
</p>

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/examples/example02_before.png" width="500">
</p>

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/examples/example02_after.png" width="500">
</p>

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/examples/example03_before.png" width="500">
</p>

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/examples/example03_after.png" width="500">
</p>

## Final Thoughts

The algorithm works reasonably well in most situations where there is enough contrast between moving parts and nonmoving parts of the image. Intuitively, this makes a lot of sense if one considers that we are separating noise from movement by means of variance between the last four pixels. With low contrast situations, this method starts to break down and one starts to see artifacts in the final video.
Additionally, this method only really works if the camera is not moving as mentioned previously. However, the algorithm has no problems whatsoever in dealing with some camera shake or very slow movement. <br/><br/>
Despite these limitations, I was still very satisfied with the results this algorithm gave me. <br/>
Basically, whenever I take out my smartphone at night and hold it steady, I can be 90% sure that I can remove the video noise in post processing. And to me, that is actually good enough. If my goal is to shoot really stunning footage, I will do my preparation beforehand and take my camera and lenses with me. The purpose of the smartphone in this context is its function as a secondary camera that you always have with you no matter what, something that cannot be said about professional camera equipment.


