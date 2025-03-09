//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#ifndef RAYTRACINGHLSLCOMPAT_H
#define RAYTRACINGHLSLCOMPAT_H

#ifdef HLSL
#include "HlslCompat.hlsli"
#else
using namespace Math3D;

// Shader will use byte encoding to access indices.
using Index = uint16_t;
#endif

struct SceneConstantBuffer
{
    Float4x4 projectionToWorld;
    Float4 cameraPosition;
};

struct CubeConstantBuffer
{
    Float4 albedo;
};

struct Vertex
{
    Float3 position;
    Float3 normal;
};

#endif // RAYTRACINGHLSLCOMPAT_H