# PSPCalc

A homebrew calculator for the PlayStation Portable (PSP).

PSPCalc is designed specifically for the PSP's 480×272 display and controller input, with a Material Design 3-inspired visual style adapted for a small screen rather than directly copying a mobile UI.

## Current Status

**Version: v0.2.3**

The current development focus is the input and UI interaction framework.

Implemented:

* 480×272 PSP display
* Analog stick virtual cursor
* X button activation
* ○ button back/cancel event
* START button exit event
* Edge detection for button presses
* D-pad Focus navigation
* Visual Focus feedback
* Unified input event system

Current test layout:

```text
┌────┬────┬────┐
│  0 │  1 │  2 │
├────┼────┼────┤
│  3 │  4 │  5 │
└────┴────┴────┘
```

The analog cursor and D-pad Focus are currently independent interaction systems. They will eventually share the same activation mechanism.

## Development Roadmap

### v0.2.x — Input & Interaction

* [x] D-pad Focus navigation
* [ ] Focus + X activation
* [ ] Unified cursor / Focus activation
* [ ] UI Element abstraction
* [ ] UI Container / Focus manager

### v0.3.x — UI Framework

* UI modularization
* Button
* Label / Text
* Panel / Container
* Automatic Focus layout
* Cursor hit testing
* Input / UI decoupling

### v0.4.x — Calculator UI

* Calculator layout
* Display
* Number input
* Decimal input
* Basic operators
* Clear / Delete

### v0.5.x — Calculator Engine

* Calculator engine
* Expression/state handling
* Continuous calculation
* Error handling
* Number formatting
* Precision and edge cases

Later versions will focus on visual design, UX, testing, architecture cleanup, and the final 1.0 release.

## Releases

Release builds are stored under:

```text
releases/
```

Current release:

* `v0.2.3` — D-pad Focus navigation

## Building

PSPCalc is built using the PSP development toolchain and CMake.

Build directory:

```bash
mkdir build
cd build
cmake ..
make
```

The generated PSP executable is packaged as `EBOOT.PBP`.

## Project Structure

```text
PSPCalc/
├── src/
│   ├── main.c
│   ├── input.c
│   └── input.h
├── releases/
├── build/
├── CMakeLists.txt
└── README.md
```

## License

License information has not been specified yet.

