# Anvil 2
Game dev toolkit build on Raylib

### Development

## Building (CMake + Ninja)
```
cmake -S . -B build -G Ninja
cmake --build build
```

## Dependencies

Most dependencies are vendored, but there are some git modules for projects that have not stabilized.

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

