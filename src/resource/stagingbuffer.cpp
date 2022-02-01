#include <carbon/base/command_buffer.hpp>
#include <carbon/resource/stagingbuffer.hpp>

carbon::StagingBuffer::StagingBuffer(std::shared_ptr<carbon::Device> device, VmaAllocator allocator, std::string name)
    : Buffer(device, allocator, std::move(name)) {
    gpuBuffer = std::make_unique<carbon::Buffer>(device, allocator, "destinationBuffer");
}

void carbon::StagingBuffer::create(uint64_t bufferSize, VkBufferUsageFlags additionalBufferUsage) {
    Buffer::create(bufferSize, bufferUsage | additionalBufferUsage, memoryUsage, memoryProperties);
}

void carbon::StagingBuffer::createDestinationBuffer(VkBufferUsageFlags usage, std::string name) {
    gpuBuffer->create(size, dstBufferUsage | usage, VMA_MEMORY_USAGE_GPU_ONLY);
}

void carbon::StagingBuffer::copyIntoVram(carbon::CommandBuffer* cmdBuffer) { copyToBuffer(cmdBuffer, gpuBuffer.get()); }

void carbon::StagingBuffer::destroy() {
    gpuBuffer->destroy();
    carbon::Buffer::destroy();
}

VkBuffer carbon::StagingBuffer::getDestinationHandle() const { return gpuBuffer->getHandle(); }

const VkDeviceAddress carbon::StagingBuffer::getDeviceAddress() const { return gpuBuffer->getDeviceAddress(); }
