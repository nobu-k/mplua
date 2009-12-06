#ifndef MSGPACK_LUA_UNPACKER_HPP_
#define MSGPACK_LUA_UNPACKER_HPP_

#include <lua.hpp>
#include <msgpack.hpp>

namespace msgpack {
namespace lua {

/**
 * @todo implement Feeder for Unpacker to use user defined feeding function.
 */
class Feeder;

class Unpacker {
private:
  Unpacker();
  Unpacker(const Unpacker&);
  Unpacker& operator =(const Unpacker&);

public:
  static const char* const MetatableName;
  static void registerUserdata(lua_State* L);
  static int create(lua_State* L);

private:
  static int finalizer(lua_State* L);

public:
  ~Unpacker();

  /**
   * @brief feeding serialized data
   */
  int feed(lua_State* L);

  /**
   * @brief Get deserialized data if exists
   *
   * @note this function can be call in two ways:
   *       unpacker.data() or unpacker().
   *
   * memo:
   * Currently, Unpacker only support manual feeding by calling feed function.
   * Feeding from user defined function should be supported. 
   *
   * example code:
   * p = msgpack.Unpacker([feeder1])
   * for v in p [, feeder2] do
   *   -- v has deserialized data
   * end
   *
   * There can be three type of feeding functions:
   *
   * 1. funciton f (unpacker) unpacker.feed("data") end
   * 2. function f () return "data" end
   * 3. function f (unpacker) unpacker.feed("data1"); return "data2" end
   *
   * 3 may be good. Every function returns nil when no more feed exists.
   */
  int data(lua_State* L);
};

} // namespace lua
} // namespace msgpack

#endif
