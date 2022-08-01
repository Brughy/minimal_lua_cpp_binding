#include "luabinding.h"
		
c2luabinding::c2luabinding(const std::string& filename) {
    LUASTATE = luaL_newstate();
    
    if (luaL_loadfile(LUASTATE, filename.c_str())) {
        std::cout<<"## Error: failed to load [luaL_loadfile]("<<filename<<")"<<std::endl;
		LUASTATE = 0;
    }
    
    int error = lua_pcall(LUASTATE, 0, 0, 0);
    while (error && lua_gettop(LUASTATE))
    {
        std::cout << "## Error: stack = " << lua_gettop(LUASTATE) << "\n";
        std::cout << "## Error: error = " << error << "\n";
        std::cout << "## Error: message = " << lua_tostring(LUASTATE, -1) << "\n";
	std::cout << "## Error: failed to load [lua_pcall] ("<<filename<<")"<<std::endl;
        lua_pop(LUASTATE, 1);
        error = lua_pcall(LUASTATE, 0, 0, 0);       
    }    
    
    if(LUASTATE) luaL_openlibs(LUASTATE);
}

c2luabinding::~c2luabinding() {
	if(LUASTATE) lua_close(LUASTATE);
}

void c2luabinding::print_info(const std::string& info, 
			    const std::string& func, 
			    const std::string& name, 
			    const std::string& action) {
	std::cout<<info<<": "<<"["<<func<<"] "<<"["<<name<<"] "<<action<<std::endl;
}

std::vector<int> c2luabinding::getIntVector(const std::string& name) {
    std::vector<int> vec;
    lua_gettostack(name.c_str());
    if(lua_isnil(LUASTATE, -1)) {
        print_info("WARNING", __func__, "", "array is not found" );
        return std::vector<int>();
    }
    lua_pushnil(LUASTATE);
    while(lua_next(LUASTATE, -2)) {
        vec.push_back((int)lua_tonumber(LUASTATE, -1));
        lua_pop(LUASTATE, 1);
    }
    clean();
    return vec;
}

std::vector<std::string> c2luabinding::getTableKeys(const std::string& name) {
    std::string code = 
        "function getKeys(name) "
        "s = \"\""
        "for k, v in pairs(_G[name]) do "
        "    s = s..k..\",\" "
        "    end "
        "return s "
        "end"; // function for getting table keys
    luaL_loadstring(LUASTATE, code.c_str()); // execute code
    lua_pcall(LUASTATE,0,0,0);
    lua_getglobal(LUASTATE, "getKeys"); // get function
    lua_pushstring(LUASTATE, name.c_str());
    lua_pcall(LUASTATE, 1 , 1, 0); // execute function
    std::string test = lua_tostring(LUASTATE, -1);
    std::vector<std::string> strings;
    std::string temp = "";
    for(unsigned int i = 0; i < test.size(); i++) {     
        if(test.at(i) != ',') {
            temp += test.at(i);
        } else {
            strings.push_back(temp);
            temp= "";
        }
    }
    clean();
    return strings;
}
