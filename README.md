# SystemC Module for Image Edge Detection

![Overview](./edge-detection-overview.drawio-4.png)

As part of a Hardware-Software-Codesign lecture, TSL21 is tasked with achieving edge detection on a PNG picture using convolution and Sobel / Edge kernels.

This is the example picture:

![Input](./Input.png)

This is the output when using various kernel filters:

| Filter argument    | Output image                                              |
| ------------------ | --------------------------------------------------------- |
| `sobel_horizontal` | ![output_sobel_horizontal](./output_sobel_horizontal.png) |
| `sobel_vertical`   | ![output_sobel_vertical](./output_sobel_vertical.png)     |
| `sobel_diagonal`   | ![output_sobel_diagonal](./output_sobel_diagonal.png)     |
| `edge_horizontal`  | ![output_edge_horizontal](./output_edge_horizontal.png)   |
| `edge_vertical`    | ![output_edge_vertical](./output_edge_vertical.png)       |
| `edge_diagonal`    | ![output_edge_diagonal](./output_edge_diagonal.png)       |

## Resources

- https://deeplizard.com/learn/video/ZjM_XQa5s6s
- https://github.com/prateeksawhney97/Canny-Edge-Detection-Lane-Lines/blob/master/Canny.py
