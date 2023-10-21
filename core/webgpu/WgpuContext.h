//
// Created by barth on 21/10/23.
//

#ifndef FEATHERGL_WGPUCONTEXT_H
#define FEATHERGL_WGPUCONTEXT_H

#include <webgpu/webgpu.h>
#include <glfw3webgpu.h>
#include <stdexcept>
#include "Adapter.h"

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
    }

    ~WgpuContext() {
        wgpuInstanceRelease(instance);
        wgpuAdapterRelease(adapter);
        wgpuSurfaceRelease(surface);
    }

private:
    WGPUInstance instance;
    WGPUSurface surface;
    WGPUAdapter adapter;
};

#endif //FEATHERGL_WGPUCONTEXT_H
