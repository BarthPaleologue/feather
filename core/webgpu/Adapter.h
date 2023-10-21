//
// Created by barth on 21/10/23.
//

#ifndef FEATHERGL_ADAPTER_H
#define FEATHERGL_ADAPTER_H

#include <webgpu/webgpu.h>
#include <iostream>
#include <cassert>

/**
 * Utility function to get a WebGPU adapter, so that
 *     WGPUAdapter adapter = requestAdapter(options);
 * is roughly equivalent to
 *     const adapter = await navigator.gpu.requestAdapter(options);
 */
WGPUAdapter requestAdapter(WGPUInstance instance, WGPURequestAdapterOptions const *options);

#endif //FEATHERGL_ADAPTER_H
