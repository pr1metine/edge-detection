#include <iostream>
#include <systemc>

#include "convolution/ImageReader.h"
#include "convolution/ImageWriter.h"
#include "convolution/Layer.h"

int sc_main(int argc, char *argv[]) {
  convolution::ImageReader<char> reader("image_reader", argv[1]);
  convolution::Layer<char> convolution_layer(
      "convolution_layer", reader.get_output_height(),
      reader.get_output_width(), convolution::ConvolutionType::LINE_DIAGONAL);
  convolution::Layer<char> blur_layer(
      "blur_layer", convolution_layer.get_output_height(),
      convolution_layer.get_output_width(), convolution::ConvolutionType::BLUR);
  ImageWriter<char> writer("image_writer", "output.png",
                           blur_layer.get_output_height(),
                           blur_layer.get_output_width());

  sc_core::sc_vector<sc_core::sc_signal<char>> reader_convolution_chan;
  reader_convolution_chan.init(reader.get_output_height() *
                               reader.get_output_width());
  for (size_t i = 0; i < reader_convolution_chan.size(); ++i) {
    reader.output_matrix(reader_convolution_chan[i]);
    convolution_layer.input_matrix(reader_convolution_chan[i]);
  }

  sc_core::sc_vector<sc_core::sc_signal<char>> convolution_blur_chan;
  convolution_blur_chan.init(convolution_layer.get_output_height() *
                             convolution_layer.get_output_width());
  for (size_t i = 0; i < convolution_blur_chan.size(); ++i) {
    convolution_layer.output_matrix(convolution_blur_chan[i]);
    blur_layer.input_matrix(convolution_blur_chan[i]);
  }

  sc_core::sc_vector<sc_core::sc_signal<char>> blur_writer_chan;
  blur_writer_chan.init(blur_layer.get_output_height() *
                        blur_layer.get_output_width());
  for (size_t i = 0; i < blur_writer_chan.size(); ++i) {
    blur_layer.output_matrix(blur_writer_chan[i]);
    writer.input_matrix(blur_writer_chan[i]);
  }

  sc_start(10, sc_core::SC_NS);

  std::cout << "Done!\n";
  return 0;
}