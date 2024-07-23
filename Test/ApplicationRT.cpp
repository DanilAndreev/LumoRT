#ifdef WIN32

#include <cassert>
#include <fstream>
#include <iostream>

#include <Math3D.h>
#include "ApplicationRT.h"

#include "FrameDebuggerIntegration.h"

#include <rt/RaytracingHlslCompat.hlsli>
#include "Utils.h"

constexpr size_t BACKBUFFER_WIDTH = 800;
constexpr size_t BACKBUFFER_HEIGHT = 600;

static SceneConstantBuffer CreateCameraConstants() noexcept {
    FXMVECTOR eye = {4.0f, 4.0f, 4.0f};
    FXMVECTOR at = {0.0f, 0.0f, 0.0f};
    FXMVECTOR up = {0.0f, 1.0f, 0.0f};

    float aspectRatio = static_cast<float>(BACKBUFFER_HEIGHT) / static_cast<float>(BACKBUFFER_WIDTH);

    float fovAngleY = 45.0f;
    XMMATRIX view = XMMatrixLookAtLH(eye, at, up);
    XMMATRIX proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(fovAngleY), aspectRatio, 1.0f, 125.0f);
    XMMATRIX viewProj = view * proj;

    SceneConstantBuffer result{};

    result.projectionToWorld = XMMatrixTranspose(XMMatrixInverse(nullptr, viewProj));
    result.cameraPosition = eye;
    result.cameraPosition = {4.0f, 4.0f, 4.0f, 1.0f};
    return result;
}

void ApplicationRT::Init(RHINO::BackendAPI api) noexcept {
    using namespace RHINO;
    m_RHI = CreateRHINO(api);
    m_RHI->Initialize();
}

void ApplicationRT::Logic() noexcept {
    using namespace Math3D;
    using namespace RHINO;
    RDOCIntegration::StartCapture();

    Semaphore* semaphore = m_RHI->CreateSyncSemaphore(0);

    DescriptorHeap* heap = m_RHI->CreateDescriptorHeap(DescriptorHeapType::SRV_CBV_UAV, 3, "Heap");
    Texture2D* backbuffer = m_RHI->CreateTexture2D({BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT}, 1, TextureFormat::R32G32B32A32_FLOAT, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, "BackBuffer");

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
    Buffer* vertexStaging = m_RHI->CreateBuffer(sizeof(vertices), ResourceHeapType::Upload, ResourceUsage::CopySource, 0, "VertexBStaging");
    void* mappedVertexStaging = m_RHI->MapMemory(vertexStaging, 0, sizeof(vertices));
    memcpy(mappedVertexStaging, vertices, sizeof(vertices));
    m_RHI->UnmapMemory(vertexStaging);

    Buffer* indexStaging = m_RHI->CreateBuffer(sizeof(indices), ResourceHeapType::Upload, ResourceUsage::CopySource, 0, "IndexBStaging");
    void* mappedIndexStaging = m_RHI->MapMemory(indexStaging, 0, sizeof(indices));
    memcpy(mappedIndexStaging, indices, sizeof(indices));
    m_RHI->UnmapMemory(indexStaging);

    SceneConstantBuffer sceneConstantPayload = CreateCameraConstants();
    Buffer* cameraConstantStaging = m_RHI->CreateBuffer(sizeof(sceneConstantPayload), ResourceHeapType::Upload, ResourceUsage::CopySource, 0, "CameraConstantBStaging");
    void* mappedCameraConstantsStaging = m_RHI->MapMemory(cameraConstantStaging, 0, sizeof(sceneConstantPayload));
    memcpy(mappedCameraConstantsStaging, &sceneConstantPayload, sizeof(sceneConstantPayload));
    m_RHI->UnmapMemory(cameraConstantStaging);


    Buffer* vertexBuffer = m_RHI->CreateBuffer(sizeof(vertices), ResourceHeapType::Default, ResourceUsage::VertexBuffer | ResourceUsage::CopyDest, 0, "VertexB");
    Buffer* indexBuffer = m_RHI->CreateBuffer(sizeof(indices), ResourceHeapType::Default, ResourceUsage::IndexBuffer | ResourceUsage::CopyDest, 0, "IndexB");
    Buffer* cameraConstantBuffer = m_RHI->CreateBuffer(sizeof(sceneConstantPayload), ResourceHeapType::Default, ResourceUsage::ConstantBuffer | ResourceUsage::CopyDest, 0, "CameraConstantB");

    CommandList* uploadCMD = m_RHI->AllocateCommandList("UploadCMD");
    uploadCMD->CopyBuffer(vertexStaging, vertexBuffer, 0, 0, sizeof(vertices));
    uploadCMD->CopyBuffer(indexStaging, indexBuffer, 0, 0, sizeof(indices));
    uploadCMD->CopyBuffer(cameraConstantStaging, cameraConstantBuffer, 0, 0, sizeof(sceneConstantPayload));
    m_RHI->SubmitCommandList(uploadCMD);
    m_RHI->SignalFromQueue(semaphore, 1);
    m_RHI->SemaphoreWaitFromHost(semaphore, 1, ~0);
    uploadCMD->Release();
    vertexStaging->Release();
    indexStaging->Release();

    WriteBufferDescriptorDesc cbDescriptorDesc{};
    cbDescriptorDesc.buffer = cameraConstantBuffer;
    cbDescriptorDesc.size = sizeof(sceneConstantPayload);
    cbDescriptorDesc.bufferOffset = 0;
    cbDescriptorDesc.offsetInHeap = 2;
    heap->WriteCBV(cbDescriptorDesc);

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
    m_RHI->SignalFromQueue(semaphore, 2);
    m_RHI->SemaphoreWaitFromHost(semaphore, 2, ~0);
    blasCMD->Release();
    blasScratch->Release();

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
    m_RHI->SignalFromQueue(semaphore, 3);
    m_RHI->SemaphoreWaitFromHost(semaphore, 3, ~0);
    tlasCMD->Release();
    tlasScratch->Release();

    WriteTLASDescriptorDesc tlasDescriptorDesc{};
    tlasDescriptorDesc.tlas = tlas;
    tlasDescriptorDesc.offsetInHeap = 0;
    heap->WriteSRV(tlasDescriptorDesc);

    // -------------------------------------------------------------------------------------------------------------------------------------------

    DescriptorRangeDesc descriptorRangeSRV{};
    descriptorRangeSRV.descriptorsCount = 1;
    descriptorRangeSRV.rangeType = DescriptorRangeType::SRV;
    descriptorRangeSRV.baseRegisterSlot = 0;

    DescriptorRangeDesc descriptorRangeUAV{};
    descriptorRangeUAV.descriptorsCount = 1;
    descriptorRangeUAV.rangeType = DescriptorRangeType::UAV;
    descriptorRangeUAV.baseRegisterSlot = 1;

    DescriptorRangeDesc descriptorRangeCBV{};
    descriptorRangeCBV.descriptorsCount = 1;
    descriptorRangeCBV.rangeType = DescriptorRangeType::CBV;
    descriptorRangeCBV.baseRegisterSlot = 2;

    DescriptorRangeDesc descriptorRanges[] = {descriptorRangeSRV, descriptorRangeUAV, descriptorRangeCBV};

    DescriptorSpaceDesc spaceDesc{};
    spaceDesc.space = 0;
    spaceDesc.rangeDescs = descriptorRanges;
    spaceDesc.rangeDescCount = std::size(descriptorRanges);
    spaceDesc.offsetInDescriptorsFromTableStart = 0;

    RootSignatureDesc rootSignatureDesc{};
    rootSignatureDesc.spacesCount = 1;
    rootSignatureDesc.spacesDescs = &spaceDesc;
    rootSignatureDesc.debugName = "RootSignature";
    RootSignature* rootSignature = m_RHI->SerializeRootSignature(rootSignatureDesc);

    std::ifstream smSourceFile("rt.scar", std::ios::binary | std::ios::ate);
    assert(smSourceFile.is_open());
    auto smSource = ReadBinary(smSourceFile);
    smSourceFile.close();

    // ShaderModule raygenSM{smSource.size(), smSource.data(), "MyRaygenShader"};
    // ShaderModule missSM{smSource.size(), smSource.data(), "MyMissShader"};
    // ShaderModule closestHitSM{smSource.size(), smSource.data(), "MyClosestHitShader"};
    // ShaderModule anyHitSM{smSource.size(), smSource.data(), ""};
    // ShaderModule intersectionSM{smSource.size(), smSource.data(), ""};
    //
    // ShaderModule shaderModules[] = {raygenSM, missSM, closestHitSM, /* anyHitSM, intersectionSM */};

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

    RTPSODesc rtpsoDesc{};
    // rtpsoDesc.shaderModules = shaderModules;
    // rtpsoDesc.shaderModulesCount = std::size(shaderModules);
    rtpsoDesc.rootSignature = rootSignature;
    rtpsoDesc.records = records;
    rtpsoDesc.recordsCount = std::size(records);
    rtpsoDesc.maxTraceRecursionDepth = 1;
    rtpsoDesc.maxPayloadSizeInBytes = 32;
    rtpsoDesc.maxAttributeSizeInBytes = 32;
    rtpsoDesc.debugName = "RTPSO";
    RTPSO* pso = m_RHI->CreateSCARRTPSO(smSource.data(), smSource.size(), rtpsoDesc);

    CommandList* rtpsoCMD = m_RHI->AllocateCommandList("RTPSO CMD");
    rtpsoCMD->BuildRTPSO(pso);
    m_RHI->SubmitCommandList(rtpsoCMD);
    m_RHI->SignalFromQueue(semaphore, 4);
    m_RHI->SemaphoreWaitFromHost(semaphore, 4, ~0);
    rtpsoCMD->Release();

    // -------------------------------------------------------------------------------------------------------------------------------------------
    CommandList* traceCMD = m_RHI->AllocateCommandList("TraceCMD");

    traceCMD->SetRootSignature(rootSignature);

    DispatchRaysDesc dispatchRaysDesc{};
    dispatchRaysDesc.width = BACKBUFFER_WIDTH;
    dispatchRaysDesc.height = BACKBUFFER_HEIGHT;
    dispatchRaysDesc.pso = pso;
    dispatchRaysDesc.rayGenerationShaderRecordIndex = 0;
    dispatchRaysDesc.missShaderStartRecordIndex = 1;
    dispatchRaysDesc.hitGroupStartRecordIndex = 2;
    dispatchRaysDesc.CDBSRVUAVHeap = heap;
    dispatchRaysDesc.samplerHeap = nullptr;
    traceCMD->DispatchRays(dispatchRaysDesc);

    m_RHI->SubmitCommandList(traceCMD);
    m_RHI->SignalFromQueue(semaphore, 5);
    m_RHI->SemaphoreWaitFromHost(semaphore, 5, ~0);
    traceCMD->Release();
    RDOCIntegration::EndCapture();
}

void ApplicationRT::Release() noexcept {
    using namespace RHINO;
    m_RHI->Release();
    delete m_RHI;
}

#endif // WIN32
