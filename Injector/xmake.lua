target("Injector", function()
    set_kind("binary")
    set_languages("cxx23")
    set_exceptions("cxx")

    add_includedirs("include", { public = true })
    add_headerfiles("include/**.hpp", { install = true })

    add_files("src/**.cpp")

    add_packages("spdlog", { public = true })
end)
