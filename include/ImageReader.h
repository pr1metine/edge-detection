#ifndef __PROJECTS_SYSTEMC_EDGE_DETECTION_INCLUDE_IMAGEREADER_H_
#define __PROJECTS_SYSTEMC_EDGE_DETECTION_INCLUDE_IMAGEREADER_H_

#include <systemc.h>

template <unsigned int Height, unsigned int Width>
SC_MODULE(ImageReader) {
  sc_in<char[Height][Width]> grayscale_matrix_input;
  sc_out<char[Height][Width]> output;

  SC_CTOR(ImageReader) { output(grayscale_matrix_input); }
};

#endif  // __PROJECTS_SYSTEMC_EDGE_DETECTION_INCLUDE_IMAGEREADER_H_