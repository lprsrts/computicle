#import <Metal/Metal.h>
#import <Foundation/Foundation.h>
#import "metal_compute_shader.h"

MetalComputeShader::MetalComputeShader(const std::string& fx, const std::string& fy, size_t count)
    : device((void*)MTLCreateSystemDefaultDevice()), particleCount(count) {
    id<MTLDevice> dev = (id<MTLDevice>)device;
    if (!dev) return;
    NSError *error = nil;
    // Build Metal shader source string
    std::string src =
        "#include <metal_stdlib>\n"
        "using namespace metal;\n"
        "kernel void computeShader(device float *data [[ buffer(0) ]], uint id [[ thread_position_in_grid ]]) {\n"
        "    uint i = id * 3;\n"
        "    float2 v = float2(data[i], data[i+1]);\n"
        "    float2 w = v + float2(0.01, 0.01);\n"
        "    data[i]    = w.x;\n"
        "    data[i+1]  = w.y;\n"
        "    data[i+2] += 0.01;\n"
        "}\n";
    id<MTLLibrary> lib = [dev newLibraryWithSource:@(src.c_str()) options:nil error:&error];
    if (!lib) return;
    id<MTLFunction> fn = [lib newFunctionWithName:@"computeShader"];
    id<MTLComputePipelineState> pipe = [dev newComputePipelineStateWithFunction:fn error:&error];
    if (!pipe) return;
    pipeline = (void*)pipe;
    id<MTLCommandQueue> q = [dev newCommandQueue];
    if (!q) return;
    queue = (void*)q;
    _good = true;
}

void MetalComputeShader::workOn(GLuint glBuffer) {
    id<MTLDevice> dev = (id<MTLDevice>)device;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, glBuffer);
    void *ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
    id<MTLBuffer> buf = [dev newBufferWithBytesNoCopy:ptr length:particleCount*3*sizeof(float)
                               options:MTLResourceStorageModeShared
                           deallocator:nil];
    buffer = (void*)buf;
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void MetalComputeShader::dispatch() {
    id<MTLCommandQueue> q = (id<MTLCommandQueue>)queue;
    id<MTLCommandBuffer> cmd = [q commandBuffer];
    id<MTLComputeCommandEncoder> encoder = [cmd computeCommandEncoder];
    [encoder setComputePipelineState:(id<MTLComputePipelineState>)pipeline];
    [encoder setBuffer:(id<MTLBuffer>)buffer offset:0 atIndex:0];
    MTLSize grid = MTLSizeMake(particleCount, 1, 1);
    MTLSize tg = MTLSizeMake(64, 1, 1);
    [encoder dispatchThreads:grid threadsPerThreadgroup:tg];
    [encoder endEncoding];
    [cmd commit];
    [cmd waitUntilCompleted];
}

// Destructor: release Metal objects
MetalComputeShader::~MetalComputeShader() {
    @autoreleasepool {
        [(id<MTLBuffer>)buffer release];
        [(id<MTLComputePipelineState>)pipeline release];
        [(id<MTLCommandQueue>)queue release];
        [(id<MTLDevice>)device release];
    }
}

bool MetalComputeShader::isGood() const {
    return _good;
}
