#include "unpacker.hpp"

namespace msgpack {
namespace lua {

const char* const Unpacker::MetatableName = "msgpack.Unpacker";

void Unpacker::registerUserdata(lua_State* L) {
}

int Unpacker::create(lua_State* L) {
  return 0;
}

int Unpacker::finalizer(lua_State* L) {
  return 0;
}

Unpacker::Unpacker() {
}

Unpacker::~Unpacker() {
}

int Unpacker::feed(lua_State* L) {
  return 0;
}

int Unpacker::data(lua_State* L) {
  return 0;
}

} // namespace lua
} // namespace msgpack
