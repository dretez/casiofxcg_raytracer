# Raytracing on the Casio fx-CG50

This project aims to implement and optimize a working Raytracing engine for the
Casio fx-CG50 graphing calculator.

## Building

This project depends on the fxsdk toolchain to be build and developed.
The full list of tools required is as follows:

- [fxsdk](https://git.planet-casio.com/Lephenixnoir/fxsdk/src/branch/dev)
- [sh-elf-binutils](https://git.planet-casio.com/Lephenixnoir/sh-elf-binutils/src/branch/master)
- [sh-elf-gcc](https://git.planet-casio.com/Lephenixnoir/sh-elf-gcc/src/branch/master)
- [OpenLibm](https://git.planet-casio.com/Lephenixnoir/OpenLibm/src/branch/dev)
- [fxlibc](https://git.planet-casio.com/Vhex-Kernel-Core/fxlibc/src/branch/dev)
- [gint](https://git.planet-casio.com/Lephenixnoir/gint/src/branch/dev)

Follow the instructions on each of the repositories to install the respective
tool.

Once the toolchain is setup, the project can be built with:

```sh
fxsdk build-cg
```

To upload the compiled binary to the calculator follow these instructions:

1. Inside the calculator's menu, go to the `Link` program, or simply press
   `ALPHA` -> `cos`
2. Make sure the cable type is set to `USB`
3. Once the calculator is connected to the computer via USB, a menu will appear.
   Press `F1` on the calculator. If for any reason the menu does not appear or
   if it is accidentally closed, inside the `Link` program, press `F2` on the calculator.
4. Once the calculator's drive is mounted to the computer, run the following
   command

```sh
fxlink -s *.g3a
```
