add_rules("mode.debug", "mode.release")

set_allowedplats("windows")
set_allowedarchs("x64")

includes("G2Utils")
includes("Injector")
includes("SDK-GDK")
includes("SDK-GRTS")

add_requires("spdlog 1.15.*")
