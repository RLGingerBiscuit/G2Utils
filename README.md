# G2Utils

Some utilities for Grounded 2 that I made because I'm bored.

## Building

We use [Xmake](https://xmake.io/) to build, so you'll need to install that with the instructions on their website.

### Acquiring an SDK

You'll need to clone [Dumper-7](https://github.com/Encryqed/Dumper-7) and apply the patch
`misc/patches/dumper7_fix_data_table_row_handle.patch` to work around a known bug for some games. Build a copy using the
instructions in that repo.

Run `xmake config --dumper7=y` to make set up the injector for Dumper-7, and copy Dumper-7.dll to the output folder
(usually `<repo>/build/windows/x64/release/`) before opening the game, and running `xmake run Injector` to run the
injector. If it was successful, a console window should pop up, and after a few seconds, you should see '_Generating SDK
took ..._'.

The output should be, by default, in `C:\Dumper-7`. Go in to the folder that was created, and copy the _contents_ of
`CppSDK` into either `SDK-GRTS/SDK` (if using the steam version), or `SDK-GDK/SDK` (if using the gamepass version).

Finally, depending on the version of your game, apply `misc/patches/sdk-{grts/gdk}_fix_data_table_row_handle.patch` to
fix an issue with the outputted SDK.

### Building G2Utils

Firstly, run `xmake config --dumper7=n` to reset the injector back to normal, then run
`xmake build G2Utils-SDK-{GRTS/GDK}` depending on your version of the game (It will take a while the first time!).

You can now run `xmake run Injector` and launch the game.

You only need `Injector.exe` and `G2Utils-{GRTS/GDK}.dll` if you are repackaging the built files.
