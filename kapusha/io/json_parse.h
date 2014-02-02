#pragma once

#include <kapusha/core/String.h>
#include "buffered_stream.h"

namespace kapusha {
namespace io {

struct json_t {
  struct consumer_t {
    typedef bool (*value_string_f)(consumer_t *_this, core::string_t &&string);
    typedef bool (*value_int_f)(consumer_t *_this, int);
    typedef bool (*value_float_f)(consumer_t *_this, float);
    typedef bool (*value_bool_f)(consumer_t *_this, bool);
    typedef bool (*value_f)(consumer_t *_this);

    value_string_f value_string;
    value_int_f value_int;
    value_float_f value_float;
    value_bool_f value_bool;
    value_f value_null;
    value_bool_f value_array;
    value_bool_f value_object;
  };

  static bool parse(buffered_stream_t &stream, consumer_t *consumer);
};

} // namespace io
} // namespace kapusha
