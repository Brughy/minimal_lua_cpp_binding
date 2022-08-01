
#ifndef LUABINDING_H
#define LUABINDING_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}
/*
static int report (lua_State *L, int status) {
const char *msg;
if (status) {
msg = lua_tostring(L, -1);
if (msg == NULL) msg = "(error with no message)";
fprintf(stderr, "status=%d, %s\n", status, msg);
lua_pop(L, 1);
}
return status;
}
*/
class c2luabinding {
	/*
		LUASTATE consider a shared pointer
	*/
	public:
		lua_State* LUASTATE;
		
		c2luabinding(const std::string& filename);
		
		~c2luabinding();
		
		void print_info(const std::string& info, const std::string& func,  const std::string& name, const std::string& action);
		
		std::vector<int> getIntVector(const std::string& name);
		
		template<typename T>
		std::vector<T> getVector(const std::string& name) {
			std::vector<T> vec;
			lua_gettostack(name.c_str());
			if(lua_isnil(LUASTATE, -1)) {
				print_info("WARNING", __func__, "", "array is not found" );
				return std::vector<T>();
			}
			lua_pushnil(LUASTATE);
			while(lua_next(LUASTATE, -2)) {
				vec.push_back(c2luabinding::lua_get<T>(name));
				lua_pop(LUASTATE, 1);
			}
			clean();
			return vec;	
		}
/*
		void luaopen_register_lib (std::vector<std::pair <std::string, lua_CFunction>> register_lib){
			for (int i = 0; i < register_lib.size(); i++) {	
				lua_register(
					LUASTATE,                      
					register_lib[i].first.c_str(), 
					register_lib[i].second         
				); 
			}
		}
*/		
		//library to be registered
		// mylib = require 'mylib'
				
		std::vector<std::string> getTableKeys(const std::string& name);
    
		inline void clean() {
			/*
				Lua_gettop: function which returns number of elements in the Stack
			*/		
			int n = lua_gettop(LUASTATE);
			/*
				Empty Lua Stack 
			*/
			lua_pop(LUASTATE, n);
		}

		template<typename T>
		T get(const std::string& name) {
			if(!LUASTATE) {
				print_info("ERROR", __func__, name, "Lua file is not loaded");
				return lua_getdefault<T>();
			}
      
			T result;
			if(lua_gettostack(name)) { // variable succesfully on top of stack
				result = lua_get<T>(name);  
			} else {
				result = lua_getdefault<T>();
			}

     
			clean();
			return result;
		}
		
		template<typename T, typename S>
		std::vector<S> call(const std::string& name, const std::vector<T>& values, const int num) {
			std::vector<S> s;
			
			// Push the add function on the top of the lua stack
				lua_getglobal(LUASTATE, name.c_str());
			
			// Push the first argument on the top of the lua stack
			for(int i = 0; i < values.size(); i++) 
				lua_push<T>(LUASTATE, values[i]);

   			// Call the function with .size() arguments, returning 1 result
				lua_call(LUASTATE, values.size(), num);
			
			// Get the result
			for(int i = 0; i < num; i++) {
				s.push_back( lua_to<S>(LUASTATE) );
				lua_pop(LUASTATE, 1);
			}
			std::reverse(s.begin(), s.end());
			return s;
		}
		
		template<typename T>
		void put(const std::string& name, T value) {
			if(!LUASTATE) {
				print_info("ERROR", __func__, name, "Lua file is not loaded");
			}
			lua_newtable(LUASTATE);
			lua_push<T>(LUASTATE, value);
			lua_setglobal(LUASTATE, name.c_str());
		}
		
		bool lua_gettostack(const std::string& name) {
			level = 0;
			std::string var = "";
			for(unsigned int i = 0; i < name.size(); i++) {
				//print_info("INFO", __func__, name, var);
				if(name.at(i) == '.') {
					if(level == 0) {
						lua_getglobal(LUASTATE, var.c_str());
					} else {
						lua_getfield(LUASTATE, -1, var.c_str());
					}
            
					if(lua_isnil(LUASTATE, -1)) {
						print_info("ERROR", __func__, name, var + " is not defined");
						return false;
					} else {
						var = "";
						level++;
					}
				} else {
					var += name.at(i);
				}
			}
			if(level == 0) {
				lua_getglobal(LUASTATE, var.c_str());
			} else {
				lua_getfield(LUASTATE, -1, var.c_str());
			}
			if(lua_isnil(LUASTATE, -1)) {
				print_info("ERROR", __func__, name, var + " is not defined");
				return false;
			}

			return true;       
		}

		// Generic
		template<typename T>
		T lua_get(const std::string& name) {
			return 0;
		}
		
		template<typename T>
		std::vector<T> lua_getVector(const std::string& name) {
			std::vector<T> vec = {};
			return vec;
		}
		
		template<typename T>
		T lua_getdefault() {
			return 0;
		}
		
		template<typename T>
		void lua_put(const std::string& name, T value) {
		}
		
		template<typename T>
		void lua_push(lua_State* LUASTATE, T value) {
		}
		
		template<typename T>
		T lua_to(lua_State* LUASTATE) {
		}
							
		
	private:
		//lua_State* LUASTATE;
		std::string filename;
		int level;
};

 // Specializations

template <> 
inline bool c2luabinding::lua_get<bool>(const std::string& name) {
	return (bool)lua_toboolean(LUASTATE, -1);
}

template <> 
inline float c2luabinding::lua_get<float>(const std::string& name) {
	if(!lua_isnumber(LUASTATE, -1)) {
		print_info("WARNING", __func__, name, "not a number");
	}
	return (float)lua_tonumber(LUASTATE, -1);
}

template <>
inline int c2luabinding::lua_get<int>(const std::string& name) {
	if(!lua_isnumber(LUASTATE, -1)) {
		print_info("WARNING", __func__, name, "not a number");
	}
	return (int)lua_tonumber(LUASTATE, -1);
}

template <>
inline std::string c2luabinding::lua_get<std::string>(const std::string& name) {
	std::string s = "null";
	if(lua_isstring(LUASTATE, -1)) {
		s = std::string(lua_tostring(LUASTATE, -1));
	} else {
		print_info("WARNING", __func__, name, "not a string");
	}
	return s;
}

 // Specializations
 
template <> 
inline std::vector<bool> c2luabinding::lua_getVector<bool>(const std::string& name) {
	return (std::vector<bool>) c2luabinding::getVector<bool>(name);
}

template <> 
inline std::vector<float> c2luabinding::lua_getVector<float>(const std::string& name) {
	return (std::vector<float>) c2luabinding::getVector<float>(name);
}

template <>
inline std::vector<int> c2luabinding::lua_getVector<int>(const std::string& name) {
	return (std::vector<int>) c2luabinding::getVector<int>(name);
}

template <>
inline std::vector<std::string> c2luabinding::lua_getVector<std::string>(const std::string& name) {
	return (std::vector<std::string>) c2luabinding::getVector<std::string>(name);
} 

 // Specializations
 
template <> 
inline void c2luabinding::lua_put<bool>(const std::string& name, bool value) {
	c2luabinding::put<bool>(name, value);
}

template <> 
inline void c2luabinding::lua_put<float>(const std::string& name, float value) {
	c2luabinding::put<float>(name, value);
}

template <> 
inline void c2luabinding::lua_put<int>(const std::string& name, int value) {
	c2luabinding::put<int>(name, value);
}

template <> 
inline void c2luabinding::lua_put<std::string>(const std::string& name, std::string value) {
	c2luabinding::put<std::string>(name, value);
}

 // Specializations
 
template <> 
inline void c2luabinding::lua_push<bool>(lua_State* LUASTATE, bool value) {
	lua_pushboolean (LUASTATE, (int) value);
}

template <> 
inline void c2luabinding::lua_push<float>(lua_State* LUASTATE, float value) {
	lua_pushinteger(LUASTATE, (int) value);
}

template <> 
inline void c2luabinding::lua_push<int>(lua_State* LUASTATE, int value) {
	lua_pushinteger(LUASTATE, value);
}

template <> 
inline void c2luabinding::lua_push<std::string>(lua_State* LUASTATE, std::string value) {
	lua_pushstring(LUASTATE, value.c_str());
}
 
 // Specializations
 
template <> 
inline bool c2luabinding::lua_to<bool>(lua_State* LUASTATE) {
	return lua_toboolean(LUASTATE, -1);
}

template <> 
inline float c2luabinding::lua_to<float>(lua_State* LUASTATE) {
	return (float)lua_tonumber(LUASTATE, -1);
}

template <>
inline int c2luabinding::lua_to<int>(lua_State* LUASTATE) {
	return lua_tonumber(LUASTATE, -1);
}

template <>
inline std::string c2luabinding::lua_to<std::string>(lua_State* LUASTATE) {
	return lua_tostring(LUASTATE, -1);
} 

 //

template<>
inline std::string c2luabinding::lua_getdefault<std::string>() {
	return "null";
}

#endif
