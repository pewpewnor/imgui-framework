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
add_requires("lua")

includes("core")

target("tuesdays-core")
    set_kind("static")
    add_includedirs("core", { public = true })
    add_deps(
        "tuesdays-core-utils",
        "tuesdays-core-components",
        "tuesdays-core-engine",
        "tuesdays-core-luaren",
        "tuesdays-core-globals",
        "tuesdays-core-lifetimes",
        "tuesdays-core-keys",
        "tuesdays-core-tasks"
    )

target("tuesdays-app")
    set_kind("binary")
    add_files("app/**.cpp")
    add_deps("tuesdays-core")
    add_packages("spdlog")
