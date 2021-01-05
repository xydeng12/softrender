#pragma once
#include "tgaimage.h"
#include "math.h"
#include <vector>
#include <string>

class Model
{
public:
    Model(const std::string filename);
    int nVerts() const;
    int nFaces() const;
    vec3 Normal(const int iface, const int nthvert) const;  // per triangle corner normal vertex
    vec3 Normal(const vec2& uv) const;                      // fetch the normal vector from the normal map texture
    vec3 Vert(const int i) const;
    vec3 Vert(const int iface, const int nthvert) const;
    vec2 UV(const int iface, const int nthvert) const;
    vec3 Diffuse(const vec2& uv) const;
    double Specular(const vec2& uv) const;

    void LoadTextureDiffuse(const std::string filename);
    void LoadTextureSpecular(const std::string filename);
    void LoadTextureNormal(const std::string filename);
    void LoadCubeMap(const std::string filedir);
    vec3 CubeMap(int index, vec2 uv);

private:
    std::vector<vec3> verts_;     // array of vertices
    std::vector<vec2> uv_;        // array of tex coords
    std::vector<vec3> norms_;     // array of normal vectors
    std::vector<int> face_vert_;
    std::vector<int> face_uv_;  // indices in the above arrays per triangle
    std::vector<int> face_norm_;
    TGAImage diffusemap_;         // diffuse color texture
    TGAImage normalmap_;          // normal map texture
    TGAImage specularmap_;        // specular map texture

    TGAImage cubemaps_[6];

    void LoadTexture(const std::string filename, TGAImage& img);
};