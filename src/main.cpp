#include <systemc.h>

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <memory>

#include "convolution/Layer.h"

boost::gil::gray8_image_t get_gray_scale_image(const std::string& png_path) {
  using namespace boost::gil;

  //   rgb8_image_t tmp;
  gray8_image_t out;
  //   read_and_convert_image(png_path, tmp, png_tag());
  read_and_convert_image(png_path, out, png_tag());

  //   gray8_image_t out(view(tmp).dimensions());
  //   copy_and_convert_pixels(const_view(tmp), view(out));

  return out;
}

int my_main(int argc, char* argv[]) {
  auto gray_scale = get_gray_scale_image(argv[1]);
  auto view = boost::gil::view(gray_scale);

  convolution::Layer<5, 5, 3, 3> huh("test_layer");

  char input_value[5][5] = {{127, 127, 127, 127, 127},
                            {127, 127, 127, 127, 127},
                            {127, 127, 127, 127, 127},
                            {127, 127, 127, 127, 127},
                            {127, 127, 127, 127, 127}};
  sc_signal<char[5][5]> input("input", input_value);
  float op_value[3][3] = {
      {1.0, 0.0, -1.0},
      {2.0, 0.0, -2.0},
      {1.0, 0.0, -1.0},
  };
  sc_signal<float[3][3]> op("op", op_value);
  sc_signal<char> offset;
  char output_value[3][3] = {
      {0, 0, 0},
      {0, 0, 0},
      {0, 0, 0},
  };
  sc_signal<char[3][3]> output("output", output_value);
  huh.input(input);
  huh.op(op);
  huh.offset(offset);
  huh.output(output);

  input = {
      {127, 127, 127, 127, 127}, {127, 127, 127, 127, 127},
      {127, 127, 127, 127, 127}, {127, 127, 127, 127, 127},
      {127, 127, 127, 127, 127},
  };

  op = {
      {1.0, 0.0, -1.0},
      {2.0, 0.0, -2.0},
      {1.0, 0.0, -1.0},
  };
  offset = 127;

  sc_start(1, SC_NS);

  std::cout << "Done!\n";
  return 0;
}

int sc_main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "Usage: edge-detection <path/to/png>"
              << "\n";
    return 1;
  }

  try {
    my_main(argc, argv);
    std::cout << "Successfully imported image!\n";
  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
