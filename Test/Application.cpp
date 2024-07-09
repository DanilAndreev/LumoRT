#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

#include <Math3D.h>
#include "Application.h"

#include "FrameDebuggerIntegration.h"
#include "Utils/Common.h"

#include <RaytracingHlslCompat.hlsli>


static std::vector<uint8_t> ReadBinary(std::istream& stream) noexcept {
    std::streamsize size = stream.tellg();
    stream.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    if (stream.read(reinterpret_cast<char*>(buffer.data()), size))
    {
        /* worked! */
    }
    return buffer;
}

void Application::Init() noexcept {
    using namespace RHINO;
    m_RHI = CreateRHINO(BackendAPI::D3D12);
    m_RHI->Initialize();
}

void Application::Logic() noexcept {
    using namespace Math3D;
    using namespace RHINO;
    RDOCIntegration::StartCapture();

    DescriptorHeap* heap = m_RHI->CreateDescriptorHeap(DescriptorHeapType::SRV_CBV_UAV, 2, "Heap");
    Texture2D* backbuffer = m_RHI->CreateTexture2D({800, 600}, 1, TextureFormat::R32G32B32A32_FLOAT, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, "BackBuffer");

    WriteTexture2DDescriptorDesc textureDescriptorDesc{};
    textureDescriptorDesc.texture = backbuffer;
    textureDescriptorDesc.offsetInHeap = 1;
    heap->WriteUAV(textureDescriptorDesc);

    Index indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };

    Vertex vertices[] =
    {
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },

        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },

        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },

        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },

        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
    };
    Buffer* vertexStaging = m_RHI->CreateBuffer(sizeof(vertices), ResourceHeapType::Upload, ResourceUsage::CopySource, 0, "VertexB");
    void* mappedVertexStaging = m_RHI->MapMemory(vertexStaging, 0, sizeof(vertices));
    memcpy(mappedVertexStaging, vertices, sizeof(vertices));
    m_RHI->UnmapMemory(vertexStaging);

    Buffer* indexStaging = m_RHI->CreateBuffer(sizeof(indices), ResourceHeapType::Upload, ResourceUsage::CopySource, 0, "IndexB");
    void* mappedIndexStaging = m_RHI->MapMemory(indexStaging, 0, sizeof(indices));
    memcpy(mappedIndexStaging, indices, sizeof(indices));
    m_RHI->UnmapMemory(indexStaging);

    Buffer* vertexBuffer = m_RHI->CreateBuffer(sizeof(vertices), ResourceHeapType::Default, ResourceUsage::VertexBuffer | ResourceUsage::CopyDest, 0, "VertexB");
    Buffer* indexBuffer = m_RHI->CreateBuffer(sizeof(indices), ResourceHeapType::Default, ResourceUsage::IndexBuffer | ResourceUsage::CopyDest, 0, "IndexB");

    CommandList* uploadCMD = m_RHI->AllocateCommandList("UploadCMD");
    uploadCMD->CopyBuffer(vertexStaging, vertexBuffer, 0, 0, sizeof(vertices));
    uploadCMD->CopyBuffer(indexStaging, indexBuffer, 0, 0, sizeof(indices));
    m_RHI->SubmitCommandList(uploadCMD);
    m_RHI->ReleaseCommandList(uploadCMD);
    m_RHI->ReleaseBuffer(vertexStaging);
    m_RHI->ReleaseBuffer(indexStaging);

    // -------------------------------------------------------------------------------------------------------------------------------------------

    BLASDesc blasDesc{};
    blasDesc.indexBuffer = indexBuffer;
    blasDesc.indexCount = std::size(indices);
    blasDesc.indexFormat = IndexFormat::R16_UINT;
    blasDesc.vertexBuffer = vertexBuffer;
    blasDesc.vertexCount = std::size(vertices);
    blasDesc.vertexStride = sizeof(Vertex);
    blasDesc.vertexFormat = TextureFormat::R32G32B32_FLOAT;
    blasDesc.transformBuffer = nullptr;
    blasDesc.transformBufferStartOffset = 0;

    ASPrebuildInfo blasPrebuild = m_RHI->GetBLASPrebuildInfo(blasDesc);
    Buffer* blasScratch = m_RHI->CreateBuffer(blasPrebuild.scratchBufferSizeInBytes, ResourceHeapType::Default, ResourceUsage::UnorderedAccess, 0, "BLAS Scratch");

    CommandList* blasCMD = m_RHI->AllocateCommandList("BLAS CMD");
    BLAS* blas = blasCMD->BuildBLAS(blasDesc, blasScratch, 0, "BLAS");
    m_RHI->SubmitCommandList(blasCMD);
    m_RHI->ReleaseCommandList(blasCMD);
    m_RHI->ReleaseBuffer(blasScratch);

    // -------------------------------------------------------------------------------------------------------------------------------------------
    BLASInstanceDesc blasInstanceDesc{};
    blasInstanceDesc.blas = blas;
    // blasInstanceDesc.transform = ;
    blasInstanceDesc.instanceID = 0;

    TLASDesc tlasDesc{};
    tlasDesc.blasInstances = &blasInstanceDesc;
    tlasDesc.blasInstancesCount = 1;

    ASPrebuildInfo tlasPrebuild = m_RHI->GetBLASPrebuildInfo(blasDesc);
    Buffer* tlasScratch = m_RHI->CreateBuffer(tlasPrebuild.scratchBufferSizeInBytes, ResourceHeapType::Default, ResourceUsage::UnorderedAccess, 0, "TLAS Scratch");

    CommandList* tlasCMD = m_RHI->AllocateCommandList("TLAS CMD");
    TLAS* tlas = tlasCMD->BuildTLAS(tlasDesc, tlasScratch, 0, "TLAS");
    m_RHI->SubmitCommandList(tlasCMD);
    m_RHI->ReleaseCommandList(tlasCMD);
    m_RHI->ReleaseBuffer(tlasScratch);

    WriteTLASDescriptorDesc tlasDescriptorDesc{};
    tlasDescriptorDesc.tlas = tlas;
    tlasDescriptorDesc.offsetInHeap = 0;
    heap->WriteSRV(tlasDescriptorDesc);

    // -------------------------------------------------------------------------------------------------------------------------------------------

    std::ifstream smSourceFile("rt/rt.dxil", std::ios::binary | std::ios::ate);
    assert(smSourceFile.is_open());
    auto smSource = ReadBinary(smSourceFile);
    smSourceFile.close();

    ShaderModule raygenSM{smSource.size(), smSource.data(), "MyRaygenShader"};
    ShaderModule missSM{smSource.size(), smSource.data(), "MyMissShader"};
    ShaderModule closestHitSM{smSource.size(), smSource.data(), "MyClosestHitShader"};
    ShaderModule anyHitSM{smSource.size(), smSource.data(), ""};
    ShaderModule intersectionSM{smSource.size(), smSource.data(), ""};

    ShaderModule shaderModules[] = {raygenSM, missSM, closestHitSM, /* anyHitSM, intersectionSM */};

    RTShaderTableRecord records[3] = {};
    records[0].recordType = RTShaderTableRecordType::RayGeneration;
    records[0].rayGeneration.rayGenerationShaderIndex = 0;
    records[1].recordType = RTShaderTableRecordType::Miss;
    records[1].miss.missShaderIndex = 1;
    records[2].recordType = RTShaderTableRecordType::HitGroup;
    records[2].hitGroup.closestHitShaderIndex = 2;
    records[2].hitGroup.clothestHitShaderEnabled = true;
    // records[2].hitGroup.anyHitShaderIndex = 3;
    // records[2].hitGroup.intersectionShaderIndex = 4;

    DescriptorRangeDesc descriptorRangeSRV{};
    descriptorRangeSRV.descriptorsCount = 1;
    descriptorRangeSRV.rangeType = DescriptorRangeType::SRV;
    descriptorRangeSRV.baseRegisterSlot = 0;

    DescriptorRangeDesc descriptorRangeUAV{};
    descriptorRangeUAV.descriptorsCount = 1;
    descriptorRangeUAV.rangeType = DescriptorRangeType::UAV;
    descriptorRangeUAV.baseRegisterSlot = 1;

    DescriptorRangeDesc descriptorRanges[] = {descriptorRangeSRV, descriptorRangeUAV};

    DescriptorSpaceDesc spaceDesc{};
    spaceDesc.space = 0;
    spaceDesc.rangeDescs = descriptorRanges;
    spaceDesc.rangeDescCount = std::size(descriptorRanges);
    spaceDesc.offsetInDescriptorsFromTableStart = 0;

    RTPSODesc rtpsoDesc{};
    rtpsoDesc.shaderModules = shaderModules;
    rtpsoDesc.shaderModulesCount = std::size(shaderModules);
    rtpsoDesc.records = records;
    rtpsoDesc.recordsCount = std::size(records);
    rtpsoDesc.spacesDescs = &spaceDesc;
    rtpsoDesc.spacesCount = 1;
    rtpsoDesc.maxTraceRecursionDepth = 1;
    rtpsoDesc.maxPayloadSizeInBytes = 32;
    rtpsoDesc.maxAttributeSizeInBytes = 32;
    rtpsoDesc.debugName = "RTPSO";
    RTPSO* pso = m_RHI->CreateRTPSO(rtpsoDesc);

    CommandList* rtpsoCMD = m_RHI->AllocateCommandList("RTPSO CMD");
    rtpsoCMD->BuildRTPSO(pso);
    m_RHI->SubmitCommandList(rtpsoCMD);
    m_RHI->ReleaseCommandList(rtpsoCMD);

    // -------------------------------------------------------------------------------------------------------------------------------------------
    CommandList* traceCMD = m_RHI->AllocateCommandList("TraceCMD");

    DispatchRaysDesc dispatchRaysDesc{};
    dispatchRaysDesc.width = 800;
    dispatchRaysDesc.height = 600;
    dispatchRaysDesc.pso = pso;
    dispatchRaysDesc.rayGenerationShaderRecordIndex = 0;
    dispatchRaysDesc.missShaderStartRecordIndex = 1;
    dispatchRaysDesc.hitGroupStartRecordIndex = 2;
    dispatchRaysDesc.CDBSRVUAVHeap = heap;
    dispatchRaysDesc.samplerHeap = nullptr;
    traceCMD->DispatchRays(dispatchRaysDesc);

    m_RHI->SubmitCommandList(traceCMD);
    // m_RHI->ReleaseCommandList(traceCMD);

    /*
    Buffer* bufCBV = m_RHI->CreateBuffer(64, ResourceHeapType::Default, ResourceUsage::ConstantBuffer, 0, "ConstantB");
    Buffer* destUAV1 = m_RHI->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Default, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, 0, "DestUAV1");
    Buffer* destUAV2 = m_RHI->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Default, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, 0, "DestUAV2");
    Buffer* destUAV3 = m_RHI->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Default, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, 0, "DestUAV3");
    Buffer* rbkUAV1 = m_RHI->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Readback, ResourceUsage::CopyDest, 0, "DestUAV1");
    Buffer* rbkUAV2 = m_RHI->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Readback, ResourceUsage::CopyDest, 0, "DestUAV2");
    Buffer* rbkUAV3 = m_RHI->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Readback, ResourceUsage::CopyDest, 0, "DestUAV3");

    WriteBufferDescriptorDesc desc{};
    desc.offsetInHeap = 0;
    desc.bufferOffset = 0;
    // desc.buffer = bufCBV;
    // heap->WriteCBV(desc);


    desc.buffer = destUAV1;
    desc.size = sizeof(int) * 64;
    desc.bufferStructuredStride = sizeof(int);
    desc.offsetInHeap = 0;
    heap->WriteUAV(desc);
    desc.buffer = destUAV2;
    desc.size = sizeof(int) * 64;
    desc.bufferStructuredStride = sizeof(int);
    desc.offsetInHeap = 1;
    heap->WriteUAV(desc);
    desc.buffer = destUAV3;
    desc.size = sizeof(int) * 64;
    desc.bufferStructuredStride = sizeof(int);
    desc.offsetInHeap = 5;
    heap->WriteUAV(desc);

    std::ifstream shaderFile{"out.scar", std::ios::binary | std::ios::ate};
    assert(shaderFile.is_open());
    auto bytecode = ReadBinary(shaderFile);
    shaderFile.close();
    // assert(bytecode.size() % 4 == 0);

    // const DescriptorRangeDesc space0rd[] = {
    //     DescriptorRangeDesc{DescriptorRangeType::UAV, 0, 2},
    // };
    //
    // const DescriptorRangeDesc space1rd[] = {
    //     DescriptorRangeDesc{DescriptorRangeType::UAV, 1, 1},
    // };
    //
    // const DescriptorSpaceDesc spaces[] = {
    //     DescriptorSpaceDesc{0, 0, RHINO_ARR_SIZE(space0rd), space0rd},
    //     DescriptorSpaceDesc{1, 4, RHINO_ARR_SIZE(space1rd), space1rd},
    // };
    //
    //
    // ComputePSODesc psoDesc{};
    // psoDesc.CS.entrypoint = "main";
    // psoDesc.CS.bytecodeSize = bytecode.size();
    // psoDesc.CS.bytecode = bytecode.data();
    // psoDesc.debugName = "TestCPSO";
    // psoDesc.spacesCount = RHINO_ARR_SIZE(spaces);
    // psoDesc.spacesDescs = spaces;
    // ComputePSO* pso = m_RHI->CompileComputePSO(psoDesc);

    ComputePSO* pso = m_RHI->CompileSCARComputePSO(bytecode.data(), bytecode.size(), "TestCPSO");

    // RDOCIntegration::StartCapture();

    cmd->SetHeap(heap, nullptr);
    cmd->SetComputePSO(pso);
    cmd->Dispatch({1, 1, 1});

    m_RHI->SubmitCommandList(cmd);
    m_RHI->ReleaseCommandList(cmd);

    CommandList* cmd2 = m_RHI->AllocateCommandList("CMDList");
    cmd2->CopyBuffer(destUAV1, rbkUAV1, 0, 0, sizeof(int) * 64);
    cmd2->CopyBuffer(destUAV2, rbkUAV2, 0, 0, sizeof(int) * 64);
    cmd2->CopyBuffer(destUAV3, rbkUAV3, 0, 0, sizeof(int) * 64);
    m_RHI->SubmitCommandList(cmd2);
    m_RHI->ReleaseCommandList(cmd2);


    auto* data1 = static_cast<int*>(m_RHI->MapMemory(rbkUAV1, 0, sizeof(int) * 64));
    auto* data2 = static_cast<int*>(m_RHI->MapMemory(rbkUAV2, 0, sizeof(int) * 64));
    auto* data3 = static_cast<int*>(m_RHI->MapMemory(rbkUAV3, 0, sizeof(int) * 64));

    std::vector<int> vdata1{};
    vdata1.resize(64);
    std::vector<int> vdata2{};
    vdata2.resize(64);
    std::vector<int> vdata3{};
    vdata3.resize(64);

    memcpy(vdata1.data(), data1, sizeof(int) * 64);
    memcpy(vdata2.data(), data2, sizeof(int) * 64);
    memcpy(vdata3.data(), data3, sizeof(int) * 64);

    m_RHI->ReleaseComputePSO(pso);
    m_RHI->ReleaseDescriptorHeap(heap);
    m_RHI->ReleaseBuffer(bufCBV);
    m_RHI->ReleaseBuffer(rbkUAV1);
    m_RHI->ReleaseBuffer(rbkUAV2);
    m_RHI->ReleaseBuffer(rbkUAV3);
    m_RHI->ReleaseBuffer(destUAV1);
    m_RHI->ReleaseBuffer(destUAV2);
    m_RHI->ReleaseBuffer(destUAV3);
    */


    RDOCIntegration::EndCapture();
}
void Application::Release() noexcept {
    using namespace RHINO;
    m_RHI->Release();
    delete m_RHI;
}