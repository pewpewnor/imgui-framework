#include "lua_renderer.hpp"

#include <imgui.h>

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

namespace {

struct GlobalState {
    std::map<std::string, bool*> bools;
    std::map<std::string, int*> ints;
    std::map<std::string, float*> floats;
    std::map<std::string, std::string*> strings;
};

GlobalState gState;

int luaGetGlobalBool(lua_State* l) {
    const char* name = luaL_checkstring(l, 1);
    auto it = gState.bools.find(name);
    if (it != gState.bools.end()) {
        lua_pushboolean(l, *(it->second) ? 1 : 0);
        return 1;
    }
    luaL_error(l, "Global bool '%s' not found", name);
    return 0;
}

int luaSetGlobalBool(lua_State* l) {
    const char* name = luaL_checkstring(l, 1);
    bool value = lua_toboolean(l, 2) != 0;
    auto it = gState.bools.find(name);
    if (it != gState.bools.end()) {
        *(it->second) = value;
        return 0;
    }
    luaL_error(l, "Global bool '%s' not found", name);
    return 0;
}

int luaGetGlobalInt(lua_State* l) {
    const char* name = luaL_checkstring(l, 1);
    auto it = gState.ints.find(name);
    if (it != gState.ints.end()) {
        lua_pushinteger(l, *(it->second));
        return 1;
    }
    luaL_error(l, "Global int '%s' not found", name);
    return 0;
}

int luaSetGlobalInt(lua_State* l) {
    const char* name = luaL_checkstring(l, 1);
    int value = static_cast<int>(luaL_checkinteger(l, 2));
    auto it = gState.ints.find(name);
    if (it != gState.ints.end()) {
        *(it->second) = value;
        return 0;
    }
    luaL_error(l, "Global int '%s' not found", name);
    return 0;
}

int luaGetGlobalFloat(lua_State* l) {
    const char* name = luaL_checkstring(l, 1);
    auto it = gState.floats.find(name);
    if (it != gState.floats.end()) {
        lua_pushnumber(l, static_cast<double>(*(it->second)));
        return 1;
    }
    luaL_error(l, "Global float '%s' not found", name);
    return 0;
}

int luaSetGlobalFloat(lua_State* l) {
    const char* name = luaL_checkstring(l, 1);
    auto value = static_cast<float>(luaL_checknumber(l, 2));
    auto it = gState.floats.find(name);
    if (it != gState.floats.end()) {
        *(it->second) = value;
        return 0;
    }
    luaL_error(l, "Global float '%s' not found", name);
    return 0;
}

int luaGetGlobalString(lua_State* l) {
    const char* name = luaL_checkstring(l, 1);
    auto it = gState.strings.find(name);
    if (it != gState.strings.end()) {
        lua_pushstring(l, it->second->c_str());
        return 1;
    }
    luaL_error(l, "Global string '%s' not found", name);
    return 0;
}

int luaSetGlobalString(lua_State* l) {
    const char* name = luaL_checkstring(l, 1);
    const char* value = luaL_checkstring(l, 2);
    auto it = gState.strings.find(name);
    if (it != gState.strings.end()) {
        *(it->second) = value;
        return 0;
    }
    luaL_error(l, "Global string '%s' not found", name);
    return 0;
}

int luaImguiButton(lua_State* l) {
    const char* label = luaL_checkstring(l, 1);
    bool pressed = ImGui::Button(label);
    lua_pushboolean(l, pressed ? 1 : 0);
    return 1;
}

int luaImguiText(lua_State* l) {
    const char* text = luaL_checkstring(l, 1);
    ImGui::Text("%s", text);
    return 0;
}

int luaImguiBegin(lua_State* l) {
    const char* name = luaL_checkstring(l, 1);
    bool open = ImGui::Begin(name);
    lua_pushboolean(l, open ? 1 : 0);
    return 1;
}

int luaImguiEnd(lua_State* /*l*/) {
    ImGui::End();
    return 0;
}

int luaImguiInputtext(lua_State* l) {
    const char* label = luaL_checkstring(l, 1);
    const char* text = luaL_checkstring(l, 2);

    std::array<char, 256> buffer{};
    const size_t len = std::min(strlen(text), buffer.size() - 1);
    std::copy_n(text, len, buffer.begin());

    bool changed = ImGui::InputText(label, buffer.data(), buffer.size());
    lua_pushstring(l, buffer.data());
    lua_pushboolean(l, changed ? 1 : 0);
    return 2;
}

int luaImguiCheckbox(lua_State* l) {
    const char* label = luaL_checkstring(l, 1);
    bool value = lua_toboolean(l, 2) != 0;

    bool changed = ImGui::Checkbox(label, &value);
    lua_pushboolean(l, value ? 1 : 0);
    lua_pushboolean(l, changed ? 1 : 0);
    return 2;
}

int luaImguiSliderFloat(lua_State* l) {
    const char* label = luaL_checkstring(l, 1);
    auto value = static_cast<float>(luaL_checknumber(l, 2));
    auto min = static_cast<float>(luaL_checknumber(l, 3));
    auto max = static_cast<float>(luaL_checknumber(l, 4));

    bool changed = ImGui::SliderFloat(label, &value, min, max);
    lua_pushnumber(l, static_cast<double>(value));
    lua_pushboolean(l, changed ? 1 : 0);
    return 2;
}

int luaImguiSameline(lua_State* /*l*/) {
    ImGui::SameLine();
    return 0;
}

int luaImguiSeparator(lua_State* /*l*/) {
    ImGui::Separator();
    return 0;
}

int luaImguiSpacing(lua_State* /*l*/) {
    ImGui::Spacing();
    return 0;
}

int luaImguiShowDemoWindow(lua_State* l) {
    bool show = lua_toboolean(l, 1) != 0;
    ImGui::ShowDemoWindow(&show);
    lua_pushboolean(l, show ? 1 : 0);
    return 1;
}

int luaImguiSetNextWindowPos(lua_State* l) {
    auto x = static_cast<float>(luaL_checknumber(l, 1));
    auto y = static_cast<float>(luaL_checknumber(l, 2));
    ImGui::SetNextWindowPos(ImVec2(x, y));
    return 0;
}

int luaImguiSetNextWindowSize(lua_State* l) {
    auto width = static_cast<float>(luaL_checknumber(l, 1));
    auto height = static_cast<float>(luaL_checknumber(l, 2));
    ImGui::SetNextWindowSize(ImVec2(width, height));
    return 0;
}

int luaImguiGetDisplaySize(lua_State* l) {
    ImVec2 size = ImGui::GetIO().DisplaySize;
    lua_pushnumber(l, static_cast<double>(size.x));
    lua_pushnumber(l, static_cast<double>(size.y));
    return 2;
}

int luaImguiGetFramerate(lua_State* l) {
    lua_pushnumber(l, static_cast<double>(ImGui::GetIO().Framerate));
    return 1;
}

void registerImguiFunctions(lua_State* l) {
    lua_newtable(l);

    lua_pushcfunction(l, luaImguiButton);
    lua_setfield(l, -2, "button");

    lua_pushcfunction(l, luaImguiText);
    lua_setfield(l, -2, "text");

    lua_pushcfunction(l, luaImguiBegin);
    lua_setfield(l, -2, "begin_window");

    lua_pushcfunction(l, luaImguiEnd);
    lua_setfield(l, -2, "end_window");

    lua_pushcfunction(l, luaImguiInputtext);
    lua_setfield(l, -2, "input_text");

    lua_pushcfunction(l, luaImguiCheckbox);
    lua_setfield(l, -2, "checkbox");

    lua_pushcfunction(l, luaImguiSliderFloat);
    lua_setfield(l, -2, "slider_float");

    lua_pushcfunction(l, luaImguiSameline);
    lua_setfield(l, -2, "same_line");

    lua_pushcfunction(l, luaImguiSeparator);
    lua_setfield(l, -2, "separator");

    lua_pushcfunction(l, luaImguiSpacing);
    lua_setfield(l, -2, "spacing");

    lua_pushcfunction(l, luaImguiShowDemoWindow);
    lua_setfield(l, -2, "show_demo_window");

    lua_pushcfunction(l, luaImguiSetNextWindowPos);
    lua_setfield(l, -2, "set_next_window_pos");

    lua_pushcfunction(l, luaImguiSetNextWindowSize);
    lua_setfield(l, -2, "set_next_window_size");

    lua_pushcfunction(l, luaImguiGetDisplaySize);
    lua_setfield(l, -2, "get_display_size");

    lua_pushcfunction(l, luaImguiGetFramerate);
    lua_setfield(l, -2, "get_framerate");

    lua_setglobal(l, "imgui");
}

void registerGlobalAPI(lua_State* l) {
    lua_newtable(l);

    lua_pushcfunction(l, luaGetGlobalBool);
    lua_setfield(l, -2, "get_bool");

    lua_pushcfunction(l, luaSetGlobalBool);
    lua_setfield(l, -2, "set_bool");

    lua_pushcfunction(l, luaGetGlobalInt);
    lua_setfield(l, -2, "get_int");

    lua_pushcfunction(l, luaSetGlobalInt);
    lua_setfield(l, -2, "set_int");

    lua_pushcfunction(l, luaGetGlobalFloat);
    lua_setfield(l, -2, "get_float");

    lua_pushcfunction(l, luaSetGlobalFloat);
    lua_setfield(l, -2, "set_float");

    lua_pushcfunction(l, luaGetGlobalString);
    lua_setfield(l, -2, "get_string");

    lua_pushcfunction(l, luaSetGlobalString);
    lua_setfield(l, -2, "set_string");

    lua_setglobal(l, "globals");
}

}

LuaRenderer::LuaRenderer(const std::string& filePath) : filePath_(filePath), L_(luaL_newstate()) {
    if (L_ == nullptr) {
        std::cerr << "Failed to create Lua state" << std::endl;
        return;
    }

    luaL_openlibs(L_);
    registerImguiFunctions(L_);
    registerGlobalAPI(L_);

    std::cout << "LuaRenderer initialized with file: " << filePath_ << std::endl;
}

LuaRenderer::~LuaRenderer() {
    if (L_ != nullptr) {
        lua_close(L_);
    }
}

bool LuaRenderer::render() {
    if (L_ == nullptr) {
        std::cerr << "Lua state not initialized" << std::endl;
        return false;
    }

    std::ifstream file(filePath_);
    if (!file.is_open()) {
        std::cerr << "Failed to open Lua file: " << filePath_ << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string luaCode = buffer.str();
    file.close();

    if (luaL_dostring(L_, luaCode.c_str()) != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(L_, -1) << std::endl;
        lua_pop(L_, 1);
        return false;
    }

    return true;
}

void LuaRenderer::registerGlobalBool(const std::string& name, bool* ptr) {
    gState.bools[name] = ptr;
}

void LuaRenderer::registerGlobalInt(const std::string& name, int* ptr) { gState.ints[name] = ptr; }

void LuaRenderer::registerGlobalFloat(const std::string& name, float* ptr) {
    gState.floats[name] = ptr;
}

void LuaRenderer::registerGlobalString(const std::string& name, std::string* ptr) {
    gState.strings[name] = ptr;
}

void LuaRenderer::registerFunction(const std::string& name, lua_CFunction func) {
    lua_pushcfunction(L_, func);
    lua_setglobal(L_, name.c_str());
}
