//
// Created by barth on 21/10/23.
//

#ifndef FEATHERGL_WGPUCONTEXT_H
#define FEATHERGL_WGPUCONTEXT_H

#include <webgpu/webgpu.h>
#include <glfw3webgpu.h>
#include <stdexcept>
#include "WgpuUtils.h"

class WgpuContext {
public:
    WgpuContext() = default;

    void init(GLFWwindow *window) {
        WGPUInstanceDescriptor desc = {};
        desc.nextInChain = nullptr;
        instance = wgpuCreateInstance(&desc);

        if (!instance) {
            throw std::runtime_error("Could not initialize WebGPU!");
        }

        std::cout << "Requesting adapter..." << std::endl;

        surface = glfwGetWGPUSurface(instance, window);

        WGPURequestAdapterOptions adapterOpts = {};
        adapterOpts.nextInChain = nullptr;
        adapterOpts.compatibleSurface = surface;

        adapter = requestAdapter(instance, &adapterOpts);

        std::cout << "Got adapter: " << adapter << std::endl;

        logAdapter();

        std::cout << "Requesting device..." << std::endl;

        WGPUDeviceDescriptor deviceDesc = {};
        deviceDesc.nextInChain = nullptr;
        deviceDesc.label = "My Device"; // anything works here, that's your call
        deviceDesc.requiredFeaturesCount = 0; // we do not require any specific feature
        deviceDesc.requiredLimits = nullptr; // we do not require any specific limit
        deviceDesc.defaultQueue.nextInChain = nullptr;
        deviceDesc.defaultQueue.label = "The default queue";
        device = requestDevice(adapter, &deviceDesc);

        auto onDeviceError = [](WGPUErrorType type, char const *message, void * /* pUserData */) {
            std::cout << "Uncaptured device error: type " << type;
            if (message) std::cout << " (" << message << ")";
            std::cout << std::endl;
        };
        wgpuDeviceSetUncapturedErrorCallback(device, onDeviceError, nullptr /* pUserData */);

        std::cout << "Got device: " << device << std::endl;

        queue = wgpuDeviceGetQueue(device);

        auto onQueueWorkDone = [](WGPUQueueWorkDoneStatus status, void * /* pUserData */) {
            std::cout << "Queued work finished with status: " << status << std::endl;
        };
        wgpuQueueOnSubmittedWorkDone(queue, onQueueWorkDone, nullptr /* pUserData */);

        WGPUCommandEncoderDescriptor encoderDesc = {};
        encoderDesc.nextInChain = nullptr;
        encoderDesc.label = "My command encoder";
        encoder = wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

        WGPUSwapChainDescriptor swapChainDesc = {};
        swapChainDesc.nextInChain = nullptr;
        /*glfwGetWindowSize(window, &swapChainDesc.width, &swapChainDesc.height);
        swapChainDesc.width = window.;
        swapChainDesc.height = window.;*/

    }

    void resize(int width, int height) {

    }

    ~WgpuContext() {
        wgpuInstanceRelease(instance);
        wgpuAdapterRelease(adapter);
        wgpuSurfaceRelease(surface);
        wgpuDeviceRelease(device);
        wgpuQueueRelease(queue);
        wgpuCommandEncoderRelease(encoder);
    }

private:
    WGPUInstance instance;
    WGPUSurface surface;
    WGPUAdapter adapter;
    WGPUDevice device;
    WGPUQueue queue;
    WGPUCommandEncoder encoder;

    void logAdapter() {
        std::vector<WGPUFeatureName> features;

        // Call the function a first time with a null return address, just to get
        // the entry count.
        size_t featureCount = wgpuAdapterEnumerateFeatures(adapter, nullptr);

        // Allocate memory (could be a new, or a malloc() if this were a C program)
        features.resize(featureCount);

        // Call the function a second time, with a non-null return address
        wgpuAdapterEnumerateFeatures(adapter, features.data());

        std::cout << "Adapter features:" << std::endl;
        for (auto f: features) {
            std::cout << " - " << f << std::endl;
        }
    }
};

#endif //FEATHERGL_WGPUCONTEXT_H
