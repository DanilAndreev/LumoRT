# LumoRT - Rendering Engine with Ray Tracing support
> For now - a sandbox for development and testing RHINO render interface

## RHINO render interface
> Look for README.md in ```/RHINO``` dir  

## Building
```bash
git clone git@github.com:DanilAndreev/LumoRT.git --recurse-submodules
cd LumoRT
cmake -G "Visual Studio 17 2022" -S Test -B build
cmake --build build --target Test --config Debug
```
