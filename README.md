# Anvil 2
Game dev toolkit build on Raylib

### Development

## Building (CMake + Ninja)
```
cmake -S . -B build -G Ninja
cmake --build build
```

## Dependencies
Dependencies are vendored in `external`.
Header-only libraries are vendored in `external/include`.

Try to find an versioned release. Some projects, like sol2,
do not have recent releases and we have to simply download the latest source.

TODO: make sure we are honoring licenses.

**Note**: Avoid submodules. The below information is probably wrong.

Add a new submodule:
```bash
git submodule add <repository_url> [<destination_path>]
```

Intialize submodules (after cloning this repo):
```bash
git submodule update --init --recursive
```

Update submodules
```bash
git submodule update --remote --recursive
```

## Building on Web
TODO