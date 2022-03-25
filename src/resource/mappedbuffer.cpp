#include <carbon/base/command_buffer.hpp>
#include <carbon/resource/mappedbuffer.hpp>

carbon::MappedBuffer::MappedBuffer(std::shared_ptr<carbon::Device> device, VmaAllocator allocator, std::string name)
    : Buffer(device, allocator, name) {

}

void carbon::MappedBuffer::create(uint64_t bufferSize, VkBufferUsageFlags bufferUsage, VmaAllocationCreateFlags additionalAllocationFlags) {
    Buffer::create(bufferSize, bufferUsage, VMA_ALLOCATION_CREATE_MAPPED_BIT | additionalAllocationFlags, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
}
