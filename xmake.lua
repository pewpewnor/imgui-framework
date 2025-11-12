add_rules("mode.debug", "mode.release")
if is_mode("debug") then
	set_policy("build.sanitizer.address", true)
	set_policy("build.sanitizer.undefined", true)
	set_policy("build.sanitizer.leak", true)
elseif is_mode("debug_tsan") then
	set_policy("build.sanitizer.thread", true)
end

set_languages("c++23")
set_warnings("allextra", "pedantic", "error")

add_requires("imgui 1.91.8", { configs = { backend = "none" } })
add_requires("sfml 3.0.1")

target("imgui-sfml")
    set_kind("static")
    add_files("vendor/imgui-sfml/imgui-SFML.cpp")
    add_includedirs("vendor/imgui-sfml", { public = true })
    add_packages("imgui", "sfml", { public = true })
    add_syslinks("GL")

target("engine")
    set_kind("static")
    add_packages("imgui", "sfml")
    add_deps("imgui-sfml")

target("app")
    set_kind("binary")
    add_files("src/app/**.cpp")
    add_includedirs("src")
    add_deps("engine")
    add_packages("imgui", "sfml")
    add_deps("imgui-sfml")
