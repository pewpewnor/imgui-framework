add_rules("mode.debug", "mode.release")

set_languages("c++23")
set_warnings("allextra", "pedantic", "error")

if is_mode("debug") then
	set_policy("build.sanitizer.address", true)
	set_policy("build.sanitizer.leak", true)
	set_policy("build.sanitizer.undefined", true)
elseif is_mode("tsan") then
	set_policy("build.sanitizer.thread", true)
end

add_requires("imgui", { configs = { glfw_opengl3 = true } })
add_requires("glfw")
add_requires("opengl")

target("project-name")
    set_kind("binary")
    add_files("src/engine/*.cpp")
    add_files("src/**.cpp")
    add_packages("imgui", "glfw", "opengl")
