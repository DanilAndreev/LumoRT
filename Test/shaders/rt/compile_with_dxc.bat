start dxc.exe -T lib_6_3 -Fo rt.dxil rt.hlsl
start dxc.exe -T lib_6_3 -Fo rt.spirv rt.hlsl -spirv -fspv-target-env="vulkan1.3"
