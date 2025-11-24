#pragma once

#include <string>

#include "lua.hpp"

class LuaRenderer {
public:
    LuaRenderer(const std::string& filePath);
    LuaRenderer(const LuaRenderer&) = default;
    LuaRenderer(LuaRenderer&&) = delete;
    LuaRenderer& operator=(const LuaRenderer&) = default;
    LuaRenderer& operator=(LuaRenderer&&) = delete;
    ~LuaRenderer();

    bool render();

    static void registerGlobalBool(const std::string& name, bool* ptr);
    static void registerGlobalInt(const std::string& name, int* ptr);
    static void registerGlobalFloat(const std::string& name, float* ptr);
    static void registerGlobalString(const std::string& name, std::string* ptr);
    void registerFunction(const std::string& name, lua_CFunction func);

private:
    std::string filePath_;
    lua_State* L_;
};
