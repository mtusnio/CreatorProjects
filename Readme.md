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
5. Once the image's been flashed create a symbolic link to work around the interpreter not working: ln -s /lib/ld-musl-mipsel-sf.so.1 /lib/ld-musl-mips.so.1

## Projects

### Fan

A simple C application which enables PWM fan control using a joystick

[Video](https://drive.google.com/file/d/0B2-TjqGUlGKvSmNfLUF5TjRaNDQ/view?usp=sharing)

### Temperature server

Python HTTP server which reports temperature read from the Thermo3 click

[Video](https://drive.google.com/file/d/0B2-TjqGUlGKvYkxfVW9fX2tWcGs/view?usp=sharing)

### Snake

A game of snake implemented using a joystick and 8x8 LED display

[Video](https://www.youtube.com/watch?v=A7ed3Y3s2E8)
