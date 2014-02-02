#include <kapusha/core/log.h>
#include <kapusha/viewport.h>
#include <kapusha/io/IFilesystem.h>
#include <kapusha/io/IFile.h>
#include <kapusha/io/json_parse.h>

using namespace kapusha;

bool jv_string(io::json_t::consumer_t*, core::string_t &&string) {
  L("string: \"%s\"", string.str());
  return true;
}

bool jv_int(io::json_t::consumer_t*, int i) {
  L("int: %d", i);
  return true;
}

bool jv_float(io::json_t::consumer_t*, float f) {
  L("float: %f", f);
  return true;
}

bool jv_bool(io::json_t::consumer_t*, bool b) {
  L("bool: %s", b ? "true" : "false");
  return true;
}

bool jv_array(io::json_t::consumer_t*, bool b) {
  L("array: %s", b ? "[" : "]");
  return true;
}

bool jv_object(io::json_t::consumer_t*, bool b) {
  L("object: %s", b ? "{" : "}");
  return true;
}

bool jv_null(io::json_t::consumer_t*) {
  L("null");
  return true;
}

const IViewportFactory *kapusha_main(core::StringArray *args) {
  io::IFilesystem *fs = io::IFilesystem::create_native(".");
  io::IFile *f = fs->open_file("test_00.json");
  io::json_t::consumer_t cons;
  cons.value_string = jv_string;
  cons.value_int = jv_int;
  cons.value_float = jv_float;
  cons.value_bool = jv_bool;
  cons.value_null = jv_null;
  cons.value_array = jv_array;
  cons.value_object = jv_object;
  io::buffered_stream_t fstream;
  f->stream_chunk(fstream, 0);
  bool r = io::json_t::parse(fstream, &cons);
  L("res: %d", r);
  return nullptr;
}
