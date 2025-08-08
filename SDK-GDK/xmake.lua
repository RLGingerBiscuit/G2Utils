target("G2Utils-SDK-GDK", function()
    set_kind("static")
    set_languages("cxx23")

    add_includedirs(".", { public = true })
    add_headerfiles("SDK/**.hpp")

    add_files("SDK/**.cpp")
end)

target("G2Utils-SDK-GDK-Headers", function()
    set_kind("headeronly")
    set_languages("cxx23")

    add_includedirs(".", { public = true })
    add_headerfiles("SDK/**.hpp")
end)
