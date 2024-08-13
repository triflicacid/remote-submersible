# Remote Control Submersible

This repository holds the custom-written software for a remote control submersible.
There is software for both the on- and off-board controllers.
Hardware overview:

- STM32 L476R chip.
- RFM98W radio module.

Both chips are configured using the STM32CubeMX IDE.

## Folder Layout

- `code` - contains various source code files.

  The `shared` folder contains software which is identical to both controllers.
  The `x-board` directories contain software specific to each controller, as dictated by `x`.
- `schematics` - contains schematics of various items, where `schematics/x` contains the KiCAD schematic sources for item `x`.
- `simulations` - contains circuit simulation files.
- `stm32cubemx` - contains STM32CubeMX projects, where `stm32cubemx/x` contains the project file `x.ioc` for project `x`.

## Code Structure

The aim is to maximise separation between custom and generated code.
The entry point to a controller's code is in `main.c`, which provides `setup` and `loop` functions.
In the generated `main.c`, there will be a single call to `setup` with `loop` called inside an infinite loop.

There will also be files for both controllers to specify the exact configuration achieved inside STM32CubeMX.

### Shared Common

These files are copied on both controllers, although all of them are not necessarily required for a given controller.

- `action-mgr` - library for creating actions, storing as pending, and executing them.
- `communications` - types for various actions' payloads, and functions to receive payload packet.
- `counter` - library for implementing a counter with a callback.
- `dc-motor` - library for driving a DC motor.
- `lora` - library for sending and receivig data using LoRa on an RFM95/96/97/98 radio module.7
- `mcp23s17` - library for interfacing with the MCP23S17 pin extender.
- `stepper-motor` - library for driving a 4-pin stepper motor.
- `stepper-motor-event` - library for driving a stepper motor between three fixed positions.
- `stored-code` - functions to save and fetch a stored code.
- [*UNUSED*] `timed-events` - library for managing events on a timeout.
- `tm1637` - library for controlling the TM1637 segment display module.
- `util` - contains utility functions and macros.

### Common Distinct

These files are present on both controllers but contain different code (but the maintains the same semantic meaning).

- `actions` - action handlers.
- `constants` - contain implementation constants, such as pin and port configurations and names of peripherals. Creates further separation between custom and generated code.
- `globals.h` - contains global variables and types.
- `main.c` - 'entry point' of custom code; supplies `setup` and `loop` functions, and declares global variables.

### Off-Board Distinct

These files are distinct to and present only on the off-board controller.

- `7-segment` - library for arbitrary-digit full-pin 7-segment displays.
- `depth` - functions for estimating the depth of the platform.

### On-Board Distinct

These files are distinct to and present only on the on-board controller.

*None*
