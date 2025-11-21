set_languages("c++23")
set_warnings("allextra", "pedantic", "error")
set_toolchains("clang")
set_runtimes("c++_static")

add_rules("mode.debug", "mode.release")
if is_mode("debug") then
	set_policy("build.sanitizer.address", true)
	set_policy("build.sanitizer.undefined", true)
	set_policy("build.sanitizer.leak", true)
elseif is_mode("debug_tsan") then
	set_policy("build.sanitizer.thread", true)
end

add_requires("imgui-sfml 3.0.0")
add_requires("spdlog")

target("internal-utils")
    set_kind("static")
    add_files("internal/utils/**.cpp")
    add_packages("imgui-sfml", "spdlog", { public = true })

target("internal-engine")
    set_kind("static")
    add_files("internal/engine/**.cpp")
    add_includedirs("internal")
    add_deps("internal-utils")
    add_packages("imgui-sfml", { public = true })

target("app")
    set_kind("binary")
    add_files("app/**.cpp")
    add_includedirs("app")
    add_includedirs("internal")
    add_deps("internal-engine", "internal-utils")
    add_packages("imgui", "sfml", "spdlog")
