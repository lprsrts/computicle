// MetalComputeShader provides a GPU compute pass via Metal on macOS. ObjC++ implementation in .mm
#pragma once
#include <cstddef>
#include <string>
#include <vector>
#include <GL/glew.h>

class MetalComputeShader {
private:
    // Opaque pointers to Metal objects (stored in ObjC++ file)
    void* device;
    void* pipeline;
    void* queue;
    void* buffer;
    size_t particleCount;
    bool _good;
public:
    MetalComputeShader(const std::string& fx, const std::string& fy, size_t count);
    ~MetalComputeShader();
    bool isGood() const;
    void workOn(GLuint glBuffer);
    void dispatch();
};
