#pragma once

#include <carbon/resource/buffer.hpp>

namespace carbon {
    // A StagingBuffer is a buffer specific to CPU memory that
    // is copied into VRAM using a command buffer. Instead of
    // having to write all of that manually each time, we abstract
    // the copies and management in a single staging buffer.
    // TODO: Check if device is using ReBAR and skip the staging buffer if so.
    class StagingBuffer : public Buffer {
        static const VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_CPU_TO_GPU;
        static const VkBufferUsageFlags bufferUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        static const VkBufferUsageFlags dstBufferUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        static const VkMemoryPropertyFlags memoryProperties = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

        std::unique_ptr<carbon::Buffer> gpuBuffer;

    public:
        explicit StagingBuffer(std::shared_ptr<carbon::Device> device, VmaAllocator allocator, std::string name = "stagingBuffer");
        StagingBuffer(const StagingBuffer& buffer) = default;

        void create(uint64_t bufferSize, VkBufferUsageFlags bufferUsage = 0);
        void createDestinationBuffer(VkBufferUsageFlags usage);
        void destroy() override;
        void copyIntoVram(carbon::CommandBuffer* cmdBuffer);
        auto getDestinationHandle() const -> VkBuffer;
        [[nodiscard]] auto getDescriptorInfo(uint64_t rangeSize, uint64_t offset) const -> VkDescriptorBufferInfo override;

        // We override this function as the device address of a local buffer
        // is never interesting and only the device buffer address matters.
        [[nodiscard]] auto getDeviceAddress() const -> const VkDeviceAddress;

        void resize(VkDeviceSize newSize) override;
    };
} // namespace carbon
