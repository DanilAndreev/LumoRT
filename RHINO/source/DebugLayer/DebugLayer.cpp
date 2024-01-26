#include "DebugLayer.h"

#ifdef WIN32
#include <windows.h>
#endif

namespace RHINO::DebugLayer {
    using namespace std::string_literals;
    DebugLayer::DebugLayer(RHINOInterface* wrapped) noexcept : m_Wrapped(wrapped) {}

    void DebugLayer::Initialize() noexcept {}

    void DebugLayer::Release() noexcept {}

    RTPSO* DebugLayer::CompileRTPSO(const RTPSODesc& desc) noexcept {

        auto* result = m_Wrapped->CompileRTPSO(desc);
        return result;
    }

    void DebugLayer::ReleaseRTPSO(RTPSO* pso) noexcept {
    }

    ComputePSO* DebugLayer::CompileComputePSO(const ComputePSODesc& desc) noexcept {
        auto* result = m_Wrapped->CompileComputePSO(desc);

        std::set<size_t> usedSpaces{};
        for (size_t space = 0; space <= desc.spacesCount; ++space) {
            if (usedSpaces.contains(desc.spacesDescs[space].space)) {
                DB("Redefined descrioptor space ["s + std::to_string(desc.spacesDescs[space].space) + "]."s);
            }
            usedSpaces.insert(desc.spacesDescs[space].space);
            if (desc.spacesDescs[space].rangeDescCount) {
                bool isSampler = desc.spacesDescs[space].rangeDescs[0].rangeType == DescriptorRangeType::Sampler;
                for (size_t range = 0; range < desc.spacesDescs[space].rangeDescCount; ++range) {
                    if (!(desc.spacesDescs[space].rangeDescs[0].rangeType == DescriptorRangeType::Sampler &&  isSampler)) {
                        DB("Invalid descriptor range type. You can either SRV/UAV/CBV or Sampler per descriptor space. Space ["s + std::to_string(space) + "] range ["s + std::to_string(range) + "]");
                    }
                }
            } else {
                DB("Invalid descriptor space descriptor ranges count. At least one descriptor range per descriptor space is mandatory. Space ["s + std::to_string(space) + "]");
            }
        }

        return result;
    }

    void DebugLayer::ReleaseComputePSO(ComputePSO* pso) noexcept {
    }

    Buffer* DebugLayer::CreateBuffer(size_t size, ResourceHeapType heapType, ResourceUsage usage, size_t structuredStride, const char* name) noexcept {
        switch (heapType) {
            case ResourceHeapType::Default:
                break;
            case ResourceHeapType::Upload:
                switch (usage) {
                    case ResourceUsage::CopySource:
                        break;
                    default:
                        DB("Invalid usage + "s + EtoS(usage) + " +  flag with ResourceHeapType::Upload of buffer '"s + name + "'. Valid values: [ResourceUsage::CopySource].");
                }
                break;
            case ResourceHeapType::Readback:
                switch (usage) {
                    case ResourceUsage::CopyDest:
                        break;
                    default:
                        DB("Invalid usage + "s + EtoS(usage) + " +  flag with ResourceHeapType::Readback of buffer '"s + name + "'. Valid values: [ResourceUsage::CopyDest].");
                }
                break;
        }

        auto* result = m_Wrapped->CreateBuffer(size, heapType, usage, structuredStride, name);
        return result;
    }

    void DebugLayer::ReleaseBuffer(Buffer* buffer) noexcept {
    }

    Texture2D* DebugLayer::CreateTexture2D() noexcept {
        return nullptr;
    }

    void DebugLayer::ReleaseTexture2D(Texture2D* texture) noexcept {
    }

    DescriptorHeap* DebugLayer::CreateDescriptorHeap(DescriptorHeapType type, size_t descriptorsCount, const char* name) noexcept {
        return nullptr;
    }

    void DebugLayer::ReleaseDescriptorHeap(DescriptorHeap* heap) noexcept {
    }

    CommandList* DebugLayer::AllocateCommandList(const char* name) noexcept {
        return nullptr;
    }

    void DebugLayer::ReleaseCommandList(CommandList* commandList) noexcept {
    }

    void DebugLayer::SubmitCommandList(CommandList* cmd) noexcept {
    }

    void DebugLayer::DB(const std::string& text) noexcept {
#ifdef WIN32
        OutputDebugStringA(text.c_str());
        __debugbreak();
#endif// WIN32
    }

#define RHINO_ENUM_SWITCH_CASE(e) case e: return #e;
    const char* DebugLayer::EtoS(ResourceUsage usage) noexcept {
        switch (usage) {
            RHINO_ENUM_SWITCH_CASE(ResourceUsage::None)
            RHINO_ENUM_SWITCH_CASE(ResourceUsage::VertexBuffer)
            RHINO_ENUM_SWITCH_CASE(ResourceUsage::IndexBuffer)
            RHINO_ENUM_SWITCH_CASE(ResourceUsage::ConstantBuffer)
            RHINO_ENUM_SWITCH_CASE(ResourceUsage::ShaderResource)
            RHINO_ENUM_SWITCH_CASE(ResourceUsage::UnorderedAccess)
            RHINO_ENUM_SWITCH_CASE(ResourceUsage::Indirect)
            RHINO_ENUM_SWITCH_CASE(ResourceUsage::CopySource)
            RHINO_ENUM_SWITCH_CASE(ResourceUsage::CopyDest)
            RHINO_ENUM_SWITCH_CASE(ResourceUsage::ValidMask)
        }
    }
}// namespace RHINO::DebugLayer
