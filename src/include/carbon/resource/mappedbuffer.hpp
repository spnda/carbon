#pragma once

#include <carbon/resource/buffer.hpp>

namespace carbon {
    // A MappedBuffer is a constantly mapped buffer which is accessible by both the host and the
    // device. This BAR region is usually very small and only useful for small amounts of data,
    // usually up to 256MB.
    class MappedBuffer : public Buffer {
    public:
        explicit MappedBuffer(std::shared_ptr<carbon::Device> device, VmaAllocator allocator, std::string name = "mappedbuffer");
        MappedBuffer(const MappedBuffer& buffer) = default;

        /**
         * additionalAllocationFlags should contain a allocation flag describing the memory read
         * and write usage. Either specify *_SEQUENTIAL_WRITE_BIT or *_RANDOM_BIT, so that the
         * used memory can be optimized and be more fitting for the use case.
         */
        void create(uint64_t bufferSize, VkBufferUsageFlags bufferUsage = 0, VmaAllocationCreateFlags additionalAllocationFlags = 0);
    };
}
