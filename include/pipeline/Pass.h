#pragma once

#include "pipeline/Sink.h"
#include "pipeline/Source.h"

namespace pipeline {
template <typename InputType>
struct Pass : public Sink<InputType>, public Source<InputType> {
  Pass(size_t input_height, size_t input_width, size_t output_height,
       size_t output_width)
      : Sink(input_height, input_width), Source(output_height, output_width) {}
};
}  // namespace pipeline
