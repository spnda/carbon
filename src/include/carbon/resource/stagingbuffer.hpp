#pragma once

#include <carbon/resource/buffer.hpp>

namespace carbon {
    // A StagingBuffer is a buffer specific to CPU memory that
    // is copied into VRAM using a command buffer. Instead of
    // having to write all of that manually each time, we abstract
    // the copies and management in a single staging buffer.
    // TODO: Check if device is using ReBAR and skip the staging buffer if so.
    class StagingBuffer : public Buffer {
        constexpr static const VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
        constexpr static const VmaAllocationCreateFlags srcAllocationFlags = 0;
        constexpr static const VkBufferUsageFlags srcBufferUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        constexpr static const VkBufferUsageFlags dstBufferUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        constexpr static const VkMemoryPropertyFlags srcMemoryProperties = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

        std::unique_ptr<carbon::Buffer> gpuBuffer;

    public:
        explicit StagingBuffer(carbon::Device* device, VmaAllocator allocator, const std::string& name = "stagingBuffer");
        StagingBuffer(const StagingBuffer& buffer) = default;
        virtual ~StagingBuffer() override = default;

        /**
         * It is very likely that the source buffer should be mappable and most likely be
         * persistently mapped too. To achieve this, the buffer should be allocated with
         * VMA_ALLOCATION_CREATE_MAPPED_BIT to have it persistently mapped. We also have to know
         * whether reads and writes are sequential or random. In most cases, we use staging
         * buffers to sequentially write into a big block of memory, which is why most of the time
         * VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT should be used.
         */
        void create(VkDeviceSize bufferSize, VkBufferUsageFlags bufferUsage = 0, VmaAllocationCreateFlags allocationFlags = 0);
        void createDestinationBuffer(VkBufferUsageFlags usage);
        void destroy() override;
        void copyIntoVram(carbon::CommandBuffer* cmdBuffer);
        auto getDestinationHandle() const -> VkBuffer;
        [[nodiscard]] auto getDescriptorInfo(uint64_t rangeSize, uint64_t offset) const -> VkDescriptorBufferInfo override;

        // We override this function as the device address of a local buffer
        // is never interesting and only the device buffer address matters.
        [[nodiscard]] auto getDeviceAddress() const -> VkDeviceAddress override;
        void resize(VkDeviceSize newSize) override;
    };
} // namespace carbon
