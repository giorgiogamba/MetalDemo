//
//  Shaders.metal
//  MetalDemo
//
//  Created by Giorgio Gamba on 14/12/24.
//

#include <metal_stdlib>
using namespace metal;

// Vertex shader
// Executed for each vertex in parallel in order to transform its position

// "constant" makes the buffer a read only struct
[[vertex]] float4 vertexShader(const constant packed_float3* triangleVertices [[buffer(5)]], const ushort vertexIndex [[vertex_id]])
{
    // Since the script is executed for each vetex in parallel, we need to index the buffer in order
    // to parse different vertices
    
    // There's generally a problem when passing from CPU to GPU which is the alignment between different data structures
    // Each vertex will start in an address divisible by 16 -> we get dirty values from the end of the vertex to the next one
    // thus we draw wrong coordinates (because 3 integers occupy 12 bytes)
    // We need to do an clean up operation by telling Metal to use a different DataStructure -> packed_float
    
    // Classic implementation
//    const float x = triangleVertices[vertexIndex];
//    const float y = triangleVertices[vertexIndex + 1];
//    const float z = triangleVertices[vertexIndex + 2];
//    return float4(x, y, z, 1.f);
    
    // Fixed implementation
    return float4(triangleVertices[vertexIndex], 1.f);
}


// Fragment shader
// Executed for each pixel to choose the color to set, Executed after rasterization, which creates the 2D from the 3D

[[fragment]] float4 fragmentShader()
{
    // Each pixel inside the triangle is blue
    return float4(0.f, 1.f, 0.f, 1.f);
    
}

