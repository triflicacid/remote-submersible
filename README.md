# Remote Control Submersible

This repository holds the custom-written software for a remote control submersible.
There is software for both the on- and off-board controllers.
Hardware overview:

- STM32 L476R chip.
- RFM98W radio module.

Both chips are configured using the STM32CubeMX IDE.

## Folder Layout

Source code is located under `code/`.
The `shared` folder contains software which is identical to both controllers.
The `x-board` directories contain software specific to each controller, as dictated by `x`.

## Structure

The aim is to maximise separation between custom and generated code.
The entry point to a controller's code is in `main.c`, which provides `setup` and `loop` functions.
In the generated `main.c`, there will be a single call to `setup` with `loop` called inside an infinite loop.

There will also be files for both controllers to specify the exact configuration achieved inside STM32CubeMX.
