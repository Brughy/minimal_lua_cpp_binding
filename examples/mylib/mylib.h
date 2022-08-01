
#ifndef MYLIB_H
#define MYLIB_H

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"

 static int idouble(lua_State *L);
 int luaopen_libmylib(lua_State *L);
}

#endif
