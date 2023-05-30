# Image-Processing
A simple image processing program, which includes grayscaling, gaussian blur, and Sobel operator.
These filters as well as the kernel convolution are fully implemented in the header files. The code uses [stb](https://github.com/nothings/stb) for image manipulation. 

## Kernel Convolution
First the function convolutes the image without the edges. Then it processes them depending on following options:
```c++
enum Border
{
    WO, //ignore border
    EXTEND,
    MIRROR,
    WRAP,
};
```
## Gaussian Blur
There are two gaussian blur functions: 
```c++
template <typename T>
void gaussianBlur(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, size_t ksize, Border border);

template <typename T>
void gaussianBlurSeparate(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, size_t ksize, Border border);
```
They differ in performance. The first one applies the kernel on an image as usual. The second function runs more effient by separating the kernel in two and applying the convolution twice.   

## Sobel operator
In order to get a proper result the image is prepcoessed with a gray filter and a gaussian blur. Afterwards, the function applies a convolution with the Sobel kernel in the x nad the y Direction. The magnitude can be calculated by the pythagoras theorem and the gradient by the arctan; this will show edges with their corresponding angel/orientation.

The following image represents the gradient angle from this function:

<img src="data/gradientcircle.png" width="150" height="150">

# Running the code
To run the code, use the provided CMakeLists file. First, create a `build` folder. Afterwards, use the command `cmake ..`, then`make` to get the executable.
Run the program with the following command:

`./main input output [-gray"r g b"] | [-gaussian"seperate kSize border"] | [-sobel"kSize gausskSize threshold gradient"]`

- input: input file with [.jpg, | .png | .bmp] as possible extension.
- output: output file with [.jpg, | .png | .bmp] as possible extension.
- gaussian: gaussian filter with "seperate" "kSize" "border" as parameter. If seperate = 0 then the image will be processed once with a kSize x kSize Kernel, otherwise the image will be processed twice with a 1 x kSize and a kSize x 1 kernel. Border can be chosen between 0 | 1 | 2 | 3, where 0 = WO, 1 = EXTEND 2 = MIRROR, 3 = WRAP. 
- sobel: sobel operator takes a kSize x kSize Kernel. The image will be preprocessed with a grayfilter and a gausskSize x gausskSize gaussian Filter. The threshold is a scalable number that reduces the amount of noise in the image. The range of the threshold can be set between 0 <= threshold <= 255 (ideally about 150). If gradient = 1 then the gradient (orientation) will be also shown in colors.

If get you the following error: `constexpr function's return type 'Kernel' (aka 'vector<std::vector<float>>') is not a literal type` you should use a different compiler like gcc 12

## Results

### Gray
|Original|Luminance|Original|Luminance|
|:---:|:---:|:---:|:---:|
![](data/lena.bmp)|![](data/lena_gray.bmp)|![](data/berries.bmp)|![](data/berries_gray.bmp)|

### Gaussian blur
|Original|ksize = 5|ksize = 17|ksize = 51|
|:---:|:---:|:---:|:---:|
![](data/lena.bmp)|![](data/lena_blur5x5.bmp)|![](data/lena_blur17x17.bmp)|![](data/lena_blur51x51.bmp)|

|Original|ksize = 5|ksize = 17|ksize = 51|
|:---:|:---:|:---:|:---:|
![](data/berries.bmp)|![](data/berries_blur5x5.bmp)|![](data/berries_blur17x17.bmp)|![](data/berries_blur51x51.bmp)|

### Sobel operator
|Original|ksize = 5|ksize = 7|Gradient|
|:---:|:---:|:---:|:---:|
![](data/lena.bmp)|![](data/lena_sobel5x5.bmp)|![](data/lena_sobel7x7.bmp)|![](data/lena_sobel_5x5_gradient.bmp)|

|Original|ksize = 5|ksize = 7|Gradient|
|:---:|:---:|:---:|:---:|
![](data/berries.bmp)|![](data/berries_sobel5x5.bmp)|![](data/berries_sobel7x7.bmp)|![](data/berries_sobel5x5_gradient.bmp)|
