#include "mylib.h"

static int idouble(lua_State *L){              /* Internal name of func */
	float rtrn = lua_tonumber(L, -1);      /* Get the single number arg */
	//printf("Top of double(), nbr=%f\n",rtrn);
	/*code here*/
	lua_pushnumber(L,rtrn*rtrn);           /* Push the return */
	return 1;                              /* One return value */
}

static const struct luaL_Reg mylib [] = { 
	{"double", idouble}, 
	{NULL, NULL} /* sentinel */ 
}; 

//name of this function is not flexible 
int luaopen_libmylib (lua_State *L){     
   	//luaL_newlibtable(L, mylib);
   	//luaL_setfuncs(L, mylib, 0);
	luaL_newlib(L, mylib); 
	return 1; 
}
