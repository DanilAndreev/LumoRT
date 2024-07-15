# LumoRT - Rendering Engine with Ray Tracing support
> For now - a sandbox for development and testing RHINO render interface

## RHINO render interface
> Look for README.md in ```/RHINO``` dir  

## Building
```bash
git clone git@github.com:DanilAndreev/LumoRT.git --recurse-submodules
cd LumoRT
cmake -G "Visual Studio 17 2022" -S Test -B build [EXAMPLE SETTINGS DEFINES]
cmake --build build --target Test --config Debug
```

### Example settings
> Attention: for now D3D12 is just one stable backend. It is enabled by default.
#### Ray Tracing
Add following to ```[EXAMPLE SETTINGS DEFINES]``` section:
```
-DEXAMPLE_ID:STRING=RT
```
Example sources: ```Test/Application.cpp```, ```shaders/compute```

#### Compute
Add following to ```[EXAMPLE SETTINGS DEFINES]``` section:
```
-DEXAMPLE_ID:STRING=Compute
```
Example sources: ```Test/ApplicationRT.cpp```, ```shaders/rt```
