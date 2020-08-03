# Temporal Video Denoising Algorithm <br/>

This small passion project of mine is designed to tackle one of the problems I have as a hobbyist filmmaker.
Often times, I find myself in a situation where I see something interesting and I want to film it, 
but the only device available to me is my smartphone. While I do own a camera, I cannot always carry it around with me. 
Thus, the combination of a really small smartphone sensor with potential low light conditions inevitably results in very noisy footage. 
Even though the subject may be interesting, the results can sometimes be borderline unuseable.
Hence, I have decided to spend about two weeks on implementing a denoising algorithm to help me out with this problem.

## The Idea

In photography, there is a technique known as **image stacking** where you take several images of a single scene
and average them, effectively simulating a longer exposure time. Given the assumption of image noise being a Gaussian
distribution, the more images you average the better your signal-to-noise ratio will be. However, implementing this technique for video is much more difficult because the scene will most likely contain moving objects or change entirely due to camera movement.
Luckily, there are ways of implementing this technique for video assuming a non-moving camera - it's called **temporal denoising** 

## The Implementation

The graph below shows the RGB values of a single pixel over roughly 450 frames.
As we can see from the graph, the pixel values take a dip every 70 frames, indicating some form of periodic movement.
In between these dips the pixel values seem to be dancing around some mean value they are supposed to represent.
This phenomenon is the image noise we want to get rid off. Essentially, you want to smooth out the RGB curve without
distorting the parts representing real movement.

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
be considered in the image stacking process given the changes in pixel value. <br/>
With that, the next step is to find a way of connecting the variance values with the required number of past frames.

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/pixelvalues.png" width="500">
</p>

The variance values can be used for computing an averaging score, i.e. the probability of a pixel value being real movement. We define this value to be zero if the pixel is stationary and one if the pixel represents movement. Through some trial and error it was ultimately decided to choose a sigmoid function with hand-tuned parameters for this task. 

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/determining_avg_strength.png" width="500">
</p>

Now that we have the averaging score, we can perform a series of steps to determine how much attention we should pay to
the current pixel in comparison to the previous pixels. The higher the score, the less weight the previous pixels get
when computing the new average pixel value. If there is no movement, then up to 256 past frames can be used for producing the new, averaged frame. If the movement score is one, the number of frames drops back to one immediately:

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/graphics/calculation.png" width="500">
</p>

## The Results?
So much for the theory. Here are some actual side by side comaprisons between denoised and original video frames. The frames themselves are cropped to make the differences more pronounced:

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/examples/example01_before.png" width="550">
</p>

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/examples/example01_after.png" width="550">
</p>

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/examples/example02_before.png" width="550">
</p>

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/examples/example02_after.png" width="550">
</p>

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/examples/example03_before.png" width="550">
</p>

<p align="center">
  <img src="https://github.com/AlexRiepenhausen/TemporalVideoDenoising/blob/master/examples/example03_after.png" width="550">
</p>

## Final Thoughts

The algorithm works reasonably well in most situations with high levels of contrast. Intuitively, this makes a lot of sense since we are separating noise from movement by means of variance. On the flip side, this method introduces a lot of movement artifacts in low contrast situations. While I mentioned previously that this method only works if the camera is not moving, it has no problems whatsoever in dealing with some camera shake or very slow movement. <br/><br/>
Despite some limitations, I was still very satisfied with the results. Basically, whenever I take out my smartphone at night and hold it steady, I have the peace of mind knowing that I have tool for removing noise in post processing. If my goal is to shoot really stunning footage, I will do my preparation beforehand and take my camera and lenses with me. Hence, the ultimate purpose of my smartphone is its function as a secondary camera that I always have with me - and if I can expand its technical capabilities that's even better.


