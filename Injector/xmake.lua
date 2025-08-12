option("dumper7", { default = false, description = "Setup the injector for Dumper7" })

target("Injector", function()
    set_kind("binary")
    set_languages("cxx23")
    set_exceptions("cxx")

    add_includedirs("include", { public = true })
    add_headerfiles("include/**.hpp", { install = true })

    add_files("src/**.cpp")

    if has_config("dumper7") then
        add_defines("INJECT_DUMPER_7")
    end

    add_packages("spdlog", { public = true })
end)
