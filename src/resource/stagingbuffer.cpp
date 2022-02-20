#include <carbon/base/command_buffer.hpp>
#include <carbon/resource/stagingbuffer.hpp>

carbon::StagingBuffer::StagingBuffer(std::shared_ptr<carbon::Device> device, VmaAllocator allocator, std::string name)
    : Buffer(device, allocator, "staging_" + name) {
    gpuBuffer = std::make_unique<carbon::Buffer>(device, allocator, name);
}

void carbon::StagingBuffer::create(uint64_t bufferSize, VkBufferUsageFlags additionalBufferUsage) {
    Buffer::create(bufferSize, bufferUsage | additionalBufferUsage, memoryUsage, memoryProperties);
}

void carbon::StagingBuffer::createDestinationBuffer(VkBufferUsageFlags usage) {
    if (size != 0 && handle != nullptr) {
        gpuBuffer->create(size, dstBufferUsage | usage, VMA_MEMORY_USAGE_GPU_ONLY);
    }
}

void carbon::StagingBuffer::copyIntoVram(carbon::CommandBuffer* cmdBuffer) { copyToBuffer(cmdBuffer, gpuBuffer.get()); }

void carbon::StagingBuffer::destroy() {
    gpuBuffer->destroy();
    carbon::Buffer::destroy();
}

VkBuffer carbon::StagingBuffer::getDestinationHandle() const { return gpuBuffer->getHandle(); }

VkDescriptorBufferInfo carbon::StagingBuffer::getDescriptorInfo(uint64_t size, uint64_t offset) const {
    return {
        .buffer = gpuBuffer->getHandle(),
        .offset = offset,
        .range = size,
    };
}

const VkDeviceAddress carbon::StagingBuffer::getDeviceAddress() const { return gpuBuffer->getDeviceAddress(); }

void carbon::StagingBuffer::resize(VkDeviceSize newSize) {
    carbon::Buffer::resize(newSize);
    gpuBuffer->resize(newSize);
}
