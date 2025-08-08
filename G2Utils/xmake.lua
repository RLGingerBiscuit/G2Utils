local function common_stuff()
    set_kind("shared")
    set_languages("cxxlatest")
    set_exceptions("cxx")

    add_includedirs("include", { public = true })
    add_headerfiles("include/**.hpp", { install = true })

    add_files("src/**.cpp")

    add_links("user32.lib")

    add_packages("spdlog")
end

target("G2Utils-GRTS", function()
    common_stuff()

    -- add_deps("G2Utils-SDK-GRTS", { private = true })
    -- add_defines("G2_SDK=GRTS")
end)

target("G2Utils-GDK", function()
    common_stuff()

    -- add_deps("G2Utils-SDK-GDK", { private = true })
    -- add_defines("G2_SDK=GDK")
end)
