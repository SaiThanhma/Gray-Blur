# Image-Processing
Simple image processing which includes grayscaling, gaussian blur and Sobel operator.
These filters as well as the kernel convolution are fully implemented in header files.

## Kernel Convolution
The function first runs a convolution without edges and processes them in the next step depending on option. The methods for edge handling can be selected from "Extend", "Wrap", "Mirror" or without handling the edges.

## Gaussian Blur
There two gaussian blur functions: 
```c++
template <typename T>
void gaussianBlur(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, size_t ksize, Border border);

template <typename T>
void gaussianBlurSeparate(const T *img_in, size_t width, size_t height, size_t channels, T *img_out, size_t ksize, Border border);
```
which differ in perfemance. The first one applies the kernel on an image as usual. The second function runs more effient by separating the kernel in two and applying convolution twice.   
