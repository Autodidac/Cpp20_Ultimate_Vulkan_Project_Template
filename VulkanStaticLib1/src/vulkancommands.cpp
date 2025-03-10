#include "../include/vulkancommands.hpp"
#include <array>
#include <stdexcept>

namespace VulkanCube {

    CommandPool CommandPool::create(const Context& ctx, uint32_t bufferCount) {
        CommandPool cp;

        vk::CommandPoolCreateInfo poolInfo(
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            ctx.queueIndices.graphicsFamily.value()
        );
        cp.pool = ctx.device->createCommandPoolUnique(poolInfo).value;

        vk::CommandBufferAllocateInfo allocInfo(
            *cp.pool,
            vk::CommandBufferLevel::ePrimary,
            bufferCount
        );
        cp.buffers = ctx.device->allocateCommandBuffersUnique(allocInfo).value;

        return cp;
    }

    void CommandPool::recordFrame(
        const Context& ctx,
        const GraphicsPipeline& pipeline,
        const BufferPackage& vertexBuffer,
        const BufferPackage& indexBuffer,
        vk::Framebuffer framebuffer,
        vk::DescriptorSet descriptorSet,
        const std::vector<uint16_t>& indices,
        uint32_t currentFrame
    ) {
        auto& cmdBuffer = buffers[currentFrame];

        // Reset and begin the command buffer.
        cmdBuffer->reset();
        vk::CommandBufferBeginInfo beginInfo;
        cmdBuffer->begin(beginInfo);

        // Set clear values for color and depth.
        std::array<vk::ClearValue, 2> clearValues = { {
            vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}),
            vk::ClearDepthStencilValue(1.0f, 0)
        } };

        vk::RenderPassBeginInfo renderPassInfo(
            *pipeline.renderPass,
            framebuffer,
            vk::Rect2D({ 0, 0 }, ctx.swapchainExtent),
            static_cast<uint32_t>(clearValues.size()),
            clearValues.data()
        );

        cmdBuffer->beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
        cmdBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, *pipeline.pipeline);

        // Bind vertex and index buffers.
        cmdBuffer->bindVertexBuffers(0, { *vertexBuffer.buffer }, { 0 });
        cmdBuffer->bindIndexBuffer(*indexBuffer.buffer, 0, vk::IndexType::eUint16);

        // Bind descriptor sets.
        cmdBuffer->bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            *pipeline.layout,
            0, { descriptorSet }, {}
        );

        // Record the draw command.
        cmdBuffer->drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
        cmdBuffer->endRenderPass();
        cmdBuffer->end();
    }

    vk::UniqueCommandBuffer beginSingleTimeCommands(const Context& ctx, const CommandPool& pool) {
        vk::CommandBufferAllocateInfo allocInfo(
            *pool.pool,
            vk::CommandBufferLevel::ePrimary,
            1
        );

        auto cmdBuffers = ctx.device->allocateCommandBuffersUnique(allocInfo).value;
        auto cmdBuffer = std::move(cmdBuffers[0]);

        vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        cmdBuffer->begin(beginInfo);

        return cmdBuffer;
    }

    void endSingleTimeCommands(const Context& ctx, const CommandPool& pool, vk::CommandBuffer commandBuffer) {
        commandBuffer.end();

        vk::SubmitInfo submitInfo;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        // Use a fence for synchronization rather than waiting for the entire queue to idle.
        vk::FenceCreateInfo fenceInfo{};
        auto fence = ctx.device->createFenceUnique(fenceInfo).value;

        ctx.graphicsQueue.submit(submitInfo, *fence);
        ctx.device->waitForFences(*fence, VK_TRUE, UINT64_MAX);
    }

} // namespace VulkanCube
