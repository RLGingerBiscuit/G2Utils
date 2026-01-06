option("dumper7", { default = false, description = "Setup the injector for Dumper7" })
option("ue4ss", { default = false, description = "Setup the injector for UE4SS" })
option("helper", { default = false, description = "Setup the injector for Sparrow's Grounded Helper" })
option_end()

target("Injector", function()
    set_kind("binary")
    set_languages("cxx23")
    set_exceptions("cxx")

    add_includedirs("include", { public = true })
    add_headerfiles("include/**.hpp", { install = true })

    add_files("src/**.cpp")

    if has_config("dumper7") then
        add_defines("INJECT_DUMPER_7")
    elseif has_config("ue4ss") then
        add_defines("INJECT_UE4SS")
    elseif has_config("helper") then
        add_defines("INJECT_HELPER")
    end

    add_packages("spdlog", { public = true })
end)
