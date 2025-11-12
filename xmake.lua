includes("scripts/xmake_utils.lua")

add_rules("mode.debug", "mode.release")
if is_mode("debug") then
	set_policy("build.sanitizer.address", true)
	set_policy("build.sanitizer.undefined", true)
	set_policy("build.sanitizer.leak", true)
    SuppressLsan()
elseif is_mode("debug_tsan") then
	set_policy("build.sanitizer.thread", true)
end

add_requires("imgui", { configs = { glfw_opengl3 = true } })
add_requires("glfw")
add_requires("opengl")
set_languages("c++23")
set_warnings("allextra", "pedantic", "error")

target("engine")
    set_kind("static")
    add_files("src/engine/**.cpp")
    add_packages("imgui", "glfw", "opengl")

target("app")
    set_kind("binary")
    add_files("src/app/**.cpp")
    add_includedirs("src")
    add_deps("engine")
    add_packages("imgui", "glfw", "opengl")
