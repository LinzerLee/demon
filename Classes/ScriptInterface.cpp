#include "ScriptInterface.h"

using std::string;
USING_NS_CC;

const char *ScriptInterface::LIB_NAME = "ScriptInterface";
const int ScriptInterface::LIB_VERSION = GEN_VERSION(0, 1);
const luaL_Reg ScriptInterface::LIB_API[] = {
    { "getFullPathForFileName",ScriptInterface::_getFullPathForFileName },
	{ NULL, NULL }
};

int ScriptInterface::_getFullPathForFileName(lua_State *luaState)
{
    XLUA_CALL_C_API_INIT(arg_num, ret_num, id);
    arg_num = 0;
    
    string filename = XLUA_CHECK_STRING(luaState, arg_num);
    string fullpath = FileUtils::getInstance()->fullPathForFilename(filename);
    
   // 不用XLUA_PUSH_STRING是因为路径可能存在中文，lua代码io.open需要的路径是utf-8编码的路径
    lua_pushstring(luaState, fullpath.c_str());
    ++ret_num;
    return ret_num;
}

ScriptInterface::ScriptInterface()
{
}

ScriptInterface::~ScriptInterface()
{
}
