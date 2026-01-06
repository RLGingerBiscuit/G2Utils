add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")

set_allowedplats("windows")
set_allowedarchs("x64")

includes("G2Utils")
includes("Injector")
-- includes("SDK-GDK")
includes("SDK-GRTS")

includes("vendor")

add_requires("opengl")
add_requires("glfw 3.4")
add_requires("imgui v1.92.1-docking", { configs = { opengl3 = true, glfw = true } })

add_requires("spdlog 1.15.*")
