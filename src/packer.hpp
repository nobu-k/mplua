#ifndef MSGPACK_LUA_PACKER_HPP_
#define MSGPACK_LUA_PACKER_HPP_

#include <lua.hpp>
#include <msgpack.hpp>

namespace msgpack {
namespace lua {

class Buffer;

/**
 * Metatable for this class:
 * callback = nil or function (serialized string)
 */
class Packer {
private:
  Packer(Buffer* buf);
  Packer(const Packer&);
  Packer& operator =(const Packer&);

public:
  static const char* const MetatableName;
  static void registerUserdata(lua_State* L);
  static int create(lua_State* L);

private:
  static int finalizer(lua_State* L);

public:
  ~Packer();

  /**
   * @brief Pack arbitrary data.
   *
   * This function has a variable number of arguments. The type of each
   * argument is automatically detected and packed in an appropriate way.
   * This function also correctly packs tables or arrays which recursively
   * have tables or arrays.
   */
  int pack(lua_State* L);

  /**
   * @brief Pack tables.
   *
   * This function packs tables. If arguments are actually arrays (having only
   * integral keys), this function packs them as tables.
   *
   * If arguments recursively have an array, it will be packed as an array,
   * not as a table.
   */
  int packTable(lua_State* L);

  /**
   * @brief Pack arrays.
   *
   * This function packs arrays. If arguments have keys other than integers,
   * this function ignores those keys and only packs integral keys and their
   * values.
   *
   * In addition, if arguments recursively have an table, it will be packed
   * as an table, not as an array.
   */
  int packArray(lua_State* L);

  Buffer* buffer() { return buffer_; }
  const Buffer* buffer() const { return buffer_; }
  lua_State* state() { return tmp_lua_state_; }

private:
  void pack(lua_State* L, int index);
  void packNumber(lua_State* L, int index);
  void packBoolean(lua_State* L, int index);
  void packString(lua_State* L, int index);
  void packTable(lua_State* L, int index);
  void packTableAsTable(lua_State* L, int index);
  void packTableAsArray(lua_State* L, int index);
  void packTableAsClass(lua_State* L, int index);
  void packUserdata(lua_State* L, int index);
  
private:
  msgpack_packer packer_;
  Buffer* buffer_;

  lua_State* tmp_lua_state_;
};

} // namespace lua
} // namespace msgpack

#endif
