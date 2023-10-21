//
// Created by barth on 21/10/23.
//

#ifndef FEATHERGL_WGPUCONTEXT_H
#define FEATHERGL_WGPUCONTEXT_H

#include <webgpu/webgpu.h>
#include <stdexcept>

class WgpuContext {
public:
    WgpuContext() {
        WGPUInstanceDescriptor desc = {};
        desc.nextInChain = nullptr;
        instance = wgpuCreateInstance(&desc);

        if (!instance) {
            throw std::runtime_error("Could not initialize WebGPU!");
        }
    }

    ~WgpuContext() {
        wgpuInstanceRelease(instance);
    }

private:

    WGPUInstance instance;
};

#endif //FEATHERGL_WGPUCONTEXT_H
