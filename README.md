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

Various circuit schematics are available under `schematics/`.
These are all KiCAD schematics.

## Structure

The aim is to maximise separation between custom and generated code.
The entry point to a controller's code is in `main.c`, which provides `setup` and `loop` functions.
In the generated `main.c`, there will be a single call to `setup` with `loop` called inside an infinite loop.

There will also be files for both controllers to specify the exact configuration achieved inside STM32CubeMX.

### Shared

These files are copied on both controllers.

- `7-segment` - *library* for arbitrary-digit 7-segment displays.
- `action-mgr` - *library* for creating actions, storing as pending, and executing them.
- `communications` - types for various actions' payloads, and functions to receive payload packet.
- `lora` - *library* for sending and receivig data using LoRa on an RFM95/96/97/98 radio module.
- `stepper-event` - *library* for driving a motor between three fixed positions.
- `stepper-motor` - *library* for driving a 4-pin stepper motor.
- `stored-code` - functions to save and fetch a stored code.
- `timed-events` - *library* for managing events on a timeout.

### Common

These files are present on both controllers but contain different code (but the maintains the same semantic meaning).

- `actions` - action handlers.
- `constants` - contain implementation constants, such as pin and port configurations and names of peripherals. Creates further separation between custom and generated code.
- `main` - 'entry point' of custom code; supplies `setup` and `loop` functions, and declares global variables.
