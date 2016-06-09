# CI40 Projects

## Requirements

OpenWRT - https://github.com/IMGCreator/openwrt

LetMeCreate library - https://github.com/francois-berder/LetMeCreate

GCC available (see instructions below for current workaround to get GCC running)

Make available (Development->Make package)

Check per project requirements.

### Installing GCC

1. Do make menuconfig in the OpenWRT directory. Select the gcc package under Development->GCC
2. Run make V=s until the build fails.
3. Once the build's failed add "libc.so.6" to the end of the following file: "staging_dir/target-mipsel_mips32_musl-1.1.11/pkginfo/libc.provides"
4. Rerun the build.
5. Once the image's been flashed create a symbolic link to work around the interpreter not working: ln -s ld-musl-mipsel-sf.so.1 ld-musl-mips.so.1

## Projects
