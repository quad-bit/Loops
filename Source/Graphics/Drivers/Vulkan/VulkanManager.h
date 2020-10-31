#pragma once

namespace Loops::Graphics::Vulkan
{
    class VulkanManager
    {

    private:
        VulkanManager(){};
        VulkanManager(VulkanManager const &) {}
        VulkanManager const & operator= (VulkanManager const &) {}

        static VulkanManager* instance;

    public:
        void Init();
        void DeInit();
        void Update();
        static VulkanManager* GetInstance();
        ~VulkanManager();
    };
}
