#include <lua.h>

/**
 * 
 */
int CreatePacker(lua_State* L) {
  
  return 1;
}

/**
 * class Unpacker {
 *   object get()
 *   feed()
 * }
 */
int CreateUnpacker(lua_State* L) {
  return 1;
}

extern "C" {
  /**
   * pack: automatically detect types
   * pack_#{TypeName}: pack with the specific type
   */
  int luaopen_msgpack(lua_State* L) {
    
  }
}
