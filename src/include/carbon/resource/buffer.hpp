#pragma once

#include <memory>
#include <mutex>
#include <string>

#include <carbon/vulkan.hpp>

namespace carbon {
    class CommandBuffer;
    class Device;
    class Image;

    /**
     * A basic data buffer, managed using vma.
     */
    class Buffer {
        friend class carbon::CommandBuffer;

        carbon::Device* device;
        std::string name;

        mutable std::mutex memoryMutex;

        VmaAllocator allocator = nullptr;
        VmaAllocation allocation = nullptr;

        VkBufferUsageFlags bufferUsage = 0;
        VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_AUTO;
        VkMemoryPropertyFlags memoryProperties = 0;
        VmaAllocationCreateFlags allocationFlags = 0;

        auto getCreateInfo() const -> VkBufferCreateInfo;
        static auto getBufferAddressInfo(VkBuffer handle) -> VkBufferDeviceAddressInfoKHR;
        static auto getBufferDeviceAddress(carbon::Device* device, VkBufferDeviceAddressInfoKHR* addressInfo) -> VkDeviceAddress;

    protected:
        VkDeviceSize size = 0;
        VkDeviceAddress address = 0;
        VkBuffer handle = nullptr;

    public:
        explicit Buffer(carbon::Device* device, VmaAllocator allocator);
        explicit Buffer(carbon::Device* device, VmaAllocator allocator, std::string name);
        Buffer(const Buffer& buffer);
        virtual ~Buffer() = default;

        Buffer& operator=(const Buffer& buffer);

        [[nodiscard]] static constexpr auto alignedSize(uint64_t value, uint64_t alignment) -> uint64_t {
            return (value + alignment - 1) & ~(alignment - 1);
        }

        [[nodiscard]] static constexpr auto alignedSize(uint64_t value, uint32_t alignment) -> uint64_t {
            return (value + alignment - 1) & ~(static_cast<size_t>(alignment) - 1);
        }

        [[nodiscard]] static constexpr auto alignedSize(uint32_t value, uint32_t alignment) -> uint32_t {
            return (value + alignment - 1) & ~(alignment - 1);
        }

        // Mapping memory will only be allowed on buffers where
        // VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT or
        // VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT was set.
        void create(VkDeviceSize newSize, VkBufferUsageFlags bufferUsage, VmaAllocationCreateFlags flags, VkMemoryPropertyFlags properties = 0);
        virtual void destroy();
        void lock() const;
        // Resizes the buffer to a new size. Note that this zeros the memory.
        virtual void resize(VkDeviceSize newSize);
        void unlock() const;

        [[nodiscard]] virtual auto getDeviceAddress() const -> VkDeviceAddress;
        /** Gets a basic descriptor buffer info, with given size and given offset, or 0 if omitted. */
        [[nodiscard]] virtual auto getDescriptorInfo(VkDeviceSize size, VkDeviceSize offset) const -> VkDescriptorBufferInfo;
        [[nodiscard]] auto getHandle() const -> VkBuffer;
        [[nodiscard]] auto getDeviceOrHostConstAddress() const -> const VkDeviceOrHostAddressConstKHR;
        [[nodiscard]] auto getDeviceOrHostAddress() const -> const VkDeviceOrHostAddressKHR;
        [[nodiscard]] auto getMemoryBarrier(VkAccessFlags srcAccess, VkAccessFlags dstAccess) const -> VkBufferMemoryBarrier;
        [[nodiscard]] auto getSize() const -> VkDeviceSize;

        /**
         * Copies the memory of size from source into the
         * mapped memory for this buffer.
         */
        void memoryCopy(const void* source, uint64_t size, uint64_t offset = 0) const;

        void mapMemory(void** destination) const;
        void unmapMemory() const;

        void copyToBuffer(carbon::CommandBuffer* cmdBuffer, const carbon::Buffer* destination);
        void copyToImage(carbon::CommandBuffer* cmdBuffer, const carbon::Image* destination, VkImageLayout imageLayout,
                         VkBufferImageCopy* copy);
    };
} // namespace carbon
