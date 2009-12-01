#ifndef MSGPACK_LUA_BUFFER_HPP_
#define MSGPACK_LUA_BUFFER_HPP_

#include <vector>

namespace msgpack {
namespace lua {

class Packer;

class Buffer {
public:
  virtual ~Buffer();

  /**
   * @param p Packer* whose buffer() == this
   * @param packer msgpack_packer to be used
   */
  virtual void initPacker(Packer* p, msgpack_packer* packer) = 0;

  /**
   * @return The number of upvalues
   *
   * L has arguments of msgpack.Packer() on its stack.
   */
  virtual int initUpvalues(lua_State* L) = 0;

  /**
   * @return The number of return values
   */
  virtual int returnValue(lua_State* L) = 0;
};

/**
 * @brief A buffer class which directly returns the serialized data
 */
class DirectBuffer : public Buffer {
public:
  DirectBuffer();
  virtual ~DirectBuffer();

  virtual void initPacker(Packer* p, msgpack_packer* packer);
  virtual int initUpvalues(lua_State* L);
  virtual int returnValue(lua_State* L);

private:
  static int callback(void* data, const char* buf, unsigned int len);
  
private:
  std::vector<char> retval;
};

/**
 * @brief A buffer class which redirects the serialized data to
 * the callback function defined in Lua.
 */
class LuaBuffer : public Buffer {
public:
  LuaBuffer();
  virtual ~LuaBuffer();

  virtual void initPacker(Packer* p, msgpack_packer* packer);
  virtual int initUpvalues(lua_State* L);
  virtual int returnValue(lua_State* L);

private:
  static int callback(void* data, const char* buf, unsigned int len);
};

} // namespace lua
} // namespace msgpack

#endif
