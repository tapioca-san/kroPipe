
#include <glm/ext/vector_float3.hpp>
#include <memory>
#include <string>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "kroPipe_Aplication.hpp"

#include "../Utils/terminal/kroPipe_terminal.hpp"
#include "../Utils/gravity/kroPipe_gravity.hpp"
#include "../Utils/object/kroPipe_object.hpp"
#include "../Utils/object/kroPipe_light.hpp"
#include "window/kroPipe_windowSurface.hpp"
#include "../Utils/input/kroPipe_input.hpp"
#include "../Utils/imgui/kroPipe_imgui.hpp"
#include "window/kroPipe_windowSurface.hpp"
#include "buffers/kroPipe_frameBuffer.hpp"
#include "swapchain/kroPipe_swapchain.hpp"
#include "queue/kroPipe_queuFamilies.hpp"
#include "pipeline/kroPipe_pipeline.hpp"
#include "instance/kroPipe_instance.hpp"
#include "instance/kroPipe_instance.hpp"
#include "texture/kroPipe_texture.hpp"
#include "image/kroPipe_imageView.hpp"
#include "command/kroPipe_command.hpp"
#include "pipeline/kroPipe_GPGPU.hpp"
#include "window/kroPipe_window.hpp"
#include "render/kroPipe_render.hpp"
#include "device/kroPipe_device.hpp"
#include "texture/kroPipe_MSAA.hpp"
#include "texture/kroPipe_MSAA.hpp"
#include "depth/kroPipe_depth.hpp"
#include "debug/kroPipe_debug.hpp" 

Aplication OBJECT_aplication;

#define APP_DEBUG_ENABLE


float deltaTime;
float lastTime = glfwGetTime();

void Aplication::init(){

    KP::ENGINE::OBJECT_instance.createInstance();
    if(KP::ENGINE::debug){
        KP::ENGINE::OBJECT_debugger.setupDebugMessenger(KP::ENGINE::VK_instance, debugMessenger);
    }
    KP::ENGINE::OBJECT_windowSurface.createSurface(KP::ENGINE::VK_instance, KP::ENGINE::OBJECT_window.GLFW_window, KP::ENGINE::OBJECT_windowSurface.VK_surface);
    KP::ENGINE::OBJECT_device.pickPhysicalDevice(KP::ENGINE::VK_instance);
    KP::ENGINE::OBJECT_device.createLogicalDevice();


    KP::ENGINE::OBJECT_swapChain.createSwapChain();
    KP::ENGINE::OBJECT_imageView.createImageViews();
    KP::ENGINE::OBJECT_pipeline.createRenderPass();    
    {   
        KP::ENGINE::OBJECT_sceneUBO.createDescriptorLayout(); // scenes

    }
    //KP::ENGINE::OBJECT_GPGPU              -- GPGPU ain't including
    KP::ENGINE::OBJECT_msaa.createColorResources();
    KP::ENGINE::OBJECT_depth.createDepthResources();
    KP::ENGINE::OBJECT_pipeline.createGraphicsPipeline();  
    KP::ENGINE::OBJECT_command.createCommandPool();
    KP::ENGINE::OBJECT_frameBuffer.createFrameBuffers();
    KP::ENGINE::createTextureImage();
    KP::ENGINE::createTextureImageView();
    KP::ENGINE::createTextureSampler();
    for(std::shared_ptr<KP::UTILS::Model> model : *KP::UTILS::OBJECT_objectsManager.getAllModel()){
        model->loadModel();
    }
    
    KP::UTILS::lightTest.createUniformBuffers();
    
    KP::ENGINE::OBJECT_sceneUBO.create();
    KP::ENGINE::OBJECT_command.createCommandBuffers();
    KP::ENGINE::OBJECT_render.createSyncObjects();

    
    KP::UTILS::OBJECT_imguiInterface = new KP::UTILS::Imgui(
        KP::ENGINE::OBJECT_window.GLFW_window, KP::ENGINE::VK_instance, *KP::ENGINE::OBJECT_device.getPointerPhysicalDevice(), *KP::ENGINE::OBJECT_device.getPointerDevice(),
        KP::ENGINE::OBJECT_queuFamilies.presentQueue, KP::ENGINE::OBJECT_queuFamilies.graphicsIndex, KP::ENGINE::PipelineCache,
        KP::ENGINE::VK_renderPass
    );
    
        
    KP::UTILS::createInfo_object obj1;
    obj1.position = glm::vec3(11.0f, 11.0f, 11.0f);
    obj1.ptr_ObjectsManager = &KP::UTILS::OBJECT_objectsManager;
    obj1.object_type.push_back("Object");
    obj1.modelPath = "/home/pipebomb/Downloads/organizado/model3D/jane_doe_blender_release.glb";
    KP::UTILS::Object* a = new KP::UTILS::Object(obj1);

    KP::UTILS::createInfo_object obj2;
    obj2.position = glm::vec3(0.0f, 0.0f, 0.0f);
    obj2.ptr_ObjectsManager = &KP::UTILS::OBJECT_objectsManager;
    obj2.object_type.push_back("Camera");
    KP::UTILS::Object* a2 = new KP::UTILS::Object(obj2);
    
    KP::UTILS::lightTest.definition();

}


void Aplication::run(){
    while(!glfwWindowShouldClose(KP::ENGINE::OBJECT_window.GLFW_window)) {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        KP::UTILS::useGravity(*KP::UTILS::OBJECT_objectsManager.getAllObject(), deltaTime);
        
        KP::UTILS::lightTest.updateLightToShaders();
        
        glfwPollEvents();
        KP::UTILS::processInput(KP::ENGINE::OBJECT_window.getGlfwWindow(), KP::UTILS::OBJECT_objectsManager.getObjectByID(*KP::UTILS::OBJECT_objectsManager.getCamerasID(0)), deltaTime); // ??????????????
        //std::cerr << "position:\nx: " + std::to_string(KP::UTILS::OBJECT_objectsManager.getObjectByID(*KP::UTILS::OBJECT_objectsManager.getCamerasID(0))->getData().Position.x) + "\ny: " + std::to_string(KP::UTILS::OBJECT_objectsManager.getObjectByID(*KP::UTILS::OBJECT_objectsManager.getCamerasID(0))->getData().Position.y) + "\nz: "+ std::to_string(KP::UTILS::OBJECT_objectsManager.getObjectByID(*KP::UTILS::OBJECT_objectsManager.getCamerasID(0))->getData().Position.z) + "\n"; 
        KP::UTILS::OBJECT_imguiInterface->newFrame(); 
        KP::UTILS::OBJECT_imguiInterface->drawWindows();         
        ImGui::Render(); 

        //for(uint16_t i = 0; i < allObjects.size(); i++){
        //    kroPipe::gravityForce(allObjects[sortedID[i]], deltaTime); gravity force
        //}

        KP::ENGINE::OBJECT_render.drawFrame();
        if (glfwGetKey(KP::ENGINE::OBJECT_window.getGlfwWindow(), GLFW_KEY_Z) == GLFW_PRESS){
            break;
        }

    }
}

void Aplication::clean() {
    vkDeviceWaitIdle(*KP::ENGINE::OBJECT_device.getPointerDevice());
     KP::UTILS::lightTest.clean();
    KP::UTILS::OBJECT_imguiInterface->cleanup();
    KP::ENGINE::OBJECT_msaa.clean();
    vkDestroyImageView(*KP::ENGINE::OBJECT_device.getPointerDevice(), KP::ENGINE::depthImageView, KP::ENGINE::VK_Allocator);
    vkDestroyImage(*KP::ENGINE::OBJECT_device.getPointerDevice(), KP::ENGINE::depthImage, KP::ENGINE::VK_Allocator);
    vkFreeMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(), KP::ENGINE::depthImageMemory, KP::ENGINE::VK_Allocator);
    vkDestroySampler(*KP::ENGINE::OBJECT_device.getPointerDevice(), KP::ENGINE::textureSampler, KP::ENGINE::VK_Allocator);
    vkDestroyImageView(*KP::ENGINE::OBJECT_device.getPointerDevice(), KP::ENGINE::textureImageView, KP::ENGINE::VK_Allocator);
    vkDestroyImage(*KP::ENGINE::OBJECT_device.getPointerDevice(), KP::ENGINE::textureImage, KP::ENGINE::VK_Allocator);
    vkFreeMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(), KP::ENGINE::textureImageMemory, KP::ENGINE::VK_Allocator);
    

    KP::ENGINE::OBJECT_sceneUBO.cleanUp();

    for(auto object : *KP::UTILS::OBJECT_objectsManager.getAllObject()){
        object->clean();
    }
    
    
    KP::ENGINE::OBJECT_render.destroyRender();
        
    //KP::LIGHT::OBJECT_light.cleanUp();
    KP::ENGINE::OBJECT_command.destroyCommandPool();
    KP::ENGINE::OBJECT_frameBuffer.CleanUpFramerBuffer();
    KP::ENGINE::OBJECT_pipeline.CleanUpPipeline();
    KP::ENGINE::OBJECT_imageView.DestroyImageview();
    vkDestroySwapchainKHR(*KP::ENGINE::OBJECT_device.getPointerDevice(), KP::ENGINE::swapChain, KP::ENGINE::VK_Allocator);
    if(KP::ENGINE::debug){
        KP::ENGINE::OBJECT_debugger.DestroyDebugUtilsMessengerEXT(KP::ENGINE::VK_instance, debugMessenger, KP::ENGINE::VK_Allocator);
    }
    vkDestroySurfaceKHR(KP::ENGINE::VK_instance, KP::ENGINE::OBJECT_windowSurface.VK_surface, KP::ENGINE::VK_Allocator);
    vkDestroyDevice(*KP::ENGINE::OBJECT_device.getPointerDevice(), KP::ENGINE::VK_Allocator);
    vkDestroyInstance(KP::ENGINE::VK_instance, KP::ENGINE::VK_Allocator);
    //cleanPointers();
}

