#ifndef KROPIPE_MSAA_H
#define KROPIPE_MSAA_H

#include "../device/kroPipe_device.hpp"
#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace ENGINE {

struct ImageMSAA{
    private:

    VkImage colorImage;
    VkDeviceMemory colorDeviceMemory;
    VkImageView colorImageColorView;
    
    public:
    
    VkDeviceMemory* getPointerColorDeviceMemory();
    VkImage* getPointerColorImage();
    VkImageView* getPointerImageColorView();

};

class MSAA{

    private:

    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

    ImageMSAA dataImage;

    VkSampleCountFlagBits getMaxUsableSampleCount(); // use to test KP::ENGINE::Device::pickPhysicalDevice()

    ImageMSAA* getPointerDataImage();
    
    public:

    VkSampleCountFlagBits* getPointerMsaaSamples();

    void createColorResources();

    void clean();

};


extern MSAA OBJECT_msaa;

}//ENGINE
}//KP

#endif //KROPIPE_MSAA_H