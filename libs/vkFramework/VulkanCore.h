/*
Copyright 2022-2022 Stephane Cuillerdier (aka aiekick)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

#include <vulkan/vulkan.hpp>
#include "vk_mem_alloc.h"
#include <glm/glm.hpp>

#include <ctools/cTools.h>

#include <vkFramework/vkFramework.h>

#include "VulkanWindow.h"
#include "VulkanSwapChain.h"
#include "VulkanDevice.h"

#include <string>
#include <functional>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <array>

#include <vkProfiler/Profiler.h> // for TracyVkCtx


class VulkanImGuiRenderer;
class VulkanShader;
struct GLFWwindow;
namespace vkApi
{
	class VulkanCore
	{
	public:
		static VmaAllocator sAllocator;
		static VulkanShaderPtr sVulkanShader;
		static VulkanCorePtr Create(
			VulkanWindowPtr vVulkanWindow,
			const std::string& vAppName,
			const int& vAppVersion,
			const std::string& vEngineName,
			const int& vEngineVersion,
			const bool& vCreateSwapChain,
			const bool& vUseRTX);
		static void check_error(vk::Result result);
		static void check_error(VkResult result);

	protected:
		VulkanCoreWeak m_This;
		VulkanSwapChainPtr m_VulkanSwapChainPtr = nullptr;
		VulkanDevicePtr m_VulkanDevicePtr = nullptr;
		TracyVkCtx m_TracyContext = nullptr;
		VulkanImGuiRendererWeak m_VulkanImGuiRendererWeak;
		Texture2DPtr m_EmptyTexture2DPtr = nullptr;
		TextureCubePtr m_EmptyTextureCubePtr = nullptr;
		vk::DescriptorBufferInfo m_EmptyDescriptorBufferInfo =
			vk::DescriptorBufferInfo{ VK_NULL_HANDLE, 0, VK_WHOLE_SIZE };
		vk::BufferView m_EmptyBufferView = VK_NULL_HANDLE;

		std::vector<vk::CommandBuffer> m_CommandBuffers;
		std::vector<vk::Semaphore> m_ComputeCompleteSemaphores;
		std::vector<vk::Fence> m_ComputeWaitFences;
		std::vector<vk::CommandBuffer> m_ComputeCommandBuffers;
		vk::DescriptorPool m_DescriptorPool;
		vk::PipelineCache m_PipelineCache = nullptr;
		bool m_CreateSwapChain = false;

	protected: // extentions
		struct SupportedFeatures {
			bool is_RTX_Supported = false;
		} m_SupportedFeatures;

	public:
		bool Init(
			VulkanWindowPtr vVulkanWindow,
			const std::string& vAppName,
			const int& vAppVersion,
			const std::string& vEngineName,
			const int& vEngineVersion,
			const bool& vCreateSwapChain,
			const bool& vUseRTX);
		void Unit();

	public: // get / set
		vk::Instance getInstance() const;
		vk::PhysicalDevice getPhysicalDevice() const;
		vk::Device getDevice() const;
		VulkanDeviceWeak getFrameworkDevice();
		vk::DispatchLoaderDynamic GetDynamicLoader() const;
		vk::DescriptorPool getDescriptorPool() const;
		vk::RenderPass getMainRenderPass() const;
		vk::CommandBuffer getGraphicCommandBuffer() const;
		vk::CommandBuffer getComputeCommandBuffer() const;
		vk::SurfaceKHR getSurface() const;
		VulkanSwapChainWeak getSwapchain() const;
		std::array<vk::Semaphore, SWAPCHAIN_IMAGES_COUNT> getPresentSemaphores();
		std::array<vk::Semaphore, SWAPCHAIN_IMAGES_COUNT> getRenderSemaphores();
		vk::Viewport getViewport() const;
		vk::Rect2D getRenderArea() const;
		VulkanQueue getQueue(vk::QueueFlagBits vQueueType);
		TracyVkCtx getTracyContext();
		vk::SampleCountFlagBits getSwapchainFrameBufferSampleCount() const;

		Texture2DWeak getEmptyTexture2D() const;
		TextureCubeWeak getEmptyTextureCube() const;
		vk::DescriptorImageInfo* getEmptyTexture2DDescriptorImageInfo() const;
		vk::DescriptorImageInfo* getEmptyTextureCubeDescriptorImageInfo() const;

		// when the NullDescriptor Feature is enabled
		vk::DescriptorBufferInfo* getEmptyDescriptorBufferInfo();
		vk::BufferView* getEmptyBufferView();

		void SetVulkanImGuiRenderer(VulkanImGuiRendererWeak vVulkanShader);
		VulkanImGuiRendererWeak GetVulkanImGuiRenderer();

		// compute delta time for this frame
		// of return the last computed time
		float GetDeltaTime(const uint32_t& vCurrentFrame = 0U);

		// from device
		vk::SampleCountFlagBits GetMaxUsableSampleCount();

		const SupportedFeatures& GetSupportedFeatures() const { return m_SupportedFeatures; }

	public:
		void setupMemoryAllocator();

	public:
		void resize();

	public:// graphic
		bool frameBegin();
		void beginMainRenderPass();
		void endMainRenderPass();
		void frameEnd();

	public:// compute
		bool resetComputeFence();
		bool computeBegin();
		bool computeEnd();
		bool submitComputeCmd(vk::CommandBuffer vCmd);

	public: // KHR
		bool AcquireNextImage(VulkanWindowPtr vVulkanWindow);
		void Present();
		uint32_t getSwapchainFrameBuffers() const;
		bool justGainFocus();
		ct::frect* getDisplayRect();

	public: // reset
		void ResetCommandPools();

	protected:
		void setupGraphicCommandsAndSynchronization();
		void destroyGraphicCommandsAndSynchronization();

		void setupComputeCommandsAndSynchronization();
		void destroyComputeCommandsAndSynchronization();

		void setupDescriptorPool();

	public:
		VulkanCore() = default; // Prevent construction
		VulkanCore(const VulkanCore&) = default; // Prevent construction by copying
		VulkanCore& operator =(const VulkanCore&) { return *this; }; // Prevent assignment
		~VulkanCore() = default; // Prevent unwanted destruction
	};
}
