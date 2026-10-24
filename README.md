# Anvil 2
Game dev toolkit build on Raylib

## Using in your game
Ideally vendor or submodule this repository.
[test]

## Development

### Building (CMake + Ninja)
```
cmake -S . -B build -G Ninja
cmake --build build
```

### Dependencies
Dependencies are vendored in `external`.

Header-only libraries are vendored (with custom namespace) in `external/include`.

Try to find an versioned release. Some projects, like sol2,
do not have recent releases and we have to simply download the latest source.

TODO: make sure we are honoring licenses.

**Note**: Avoid submodules. I don't understand how recursive submodules work. One layer is more than enough added complexity.

### Windows Development
TODO

### Building on Web
TODO
