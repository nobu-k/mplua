#include "buffer.hpp"

#include "packer.hpp"

namespace msgpack {
namespace lua {

Buffer::~Buffer() {
}

// DirectBuffer
DirectBuffer::DirectBuffer() {
}

DirectBuffer::~DirectBuffer() {
}

void DirectBuffer::initPacker(Packer* p, msgpack_packer* packer) {
  msgpack_packer_init(packer, p, &DirectBuffer::callback);
}

int DirectBuffer::initUpvalues(lua_State* L) {
  // no upvalues
  return 0;
}

int DirectBuffer::returnValues(lua_State* L) {
  lua_pushlstring(L, &retval_[0], retval_.size());
  retval_.clear();
  return 1;
}

int DirectBuffer::callback(void* data, const char* buf, unsigned int len) {
  Packer* packer = static_cast<Packer*>(data);
  DirectBuffer* buffer = static_cast<DirectBuffer*>(packer->buffer());
  buffer->retval_.insert(buffer->retval_.end(), buf, buf + len);
  return 0;
}

// LuaBuffer
LuaBuffer::LuaBuffer() {
}

LuaBuffer::~LuaBuffer() {
}

void LuaBuffer::initPacker(Packer* p, msgpack_packer* packer) {
  msgpack_packer_init(packer, p, &LuaBuffer::callback);
}

int LuaBuffer::initUpvalues(lua_State* L) {
  // TODO: check stack[1] == Lua function
  // TODO: push stack[1] as an upvalue
  return 1;
}

int LuaBuffer::returnValues(lua_State* L) {
  // no return value
  return 0;
}

int LuaBuffer::callback(void* data, const char* buf, unsigned int len) {
  Packer* packer = static_cast<Packer*>(data);
  lua_State* L = packer->state();

  // TODO: get Lua function from upvalue(1)
  // TODO: get Lua function with string(buf, len)
  return 0;
}

} // namespace lua
} // namespace msgpack
