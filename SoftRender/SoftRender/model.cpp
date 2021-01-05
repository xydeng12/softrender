#include "model.h"
#include <fstream>
#include <sstream>
#include <iostream>

Model::Model(const std::string filename)
{
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    int lin = 0;
    while (!in.eof()) {
        ++lin;
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            vec3 v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            verts_.push_back(v);
        }
        else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            vec3 n;
            for (int i = 0; i < 3; i++) iss >> n[i];
            norms_.push_back(normalize(n));
        }
        else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            vec2 uv;
            for (int i = 0; i < 2; i++) iss >> uv[i];
            uv_.push_back(uv);
        }
        else if (!line.compare(0, 2, "f ")) {
            int f, t, n;
            iss >> trash;
            int cnt = 0;
            while (iss >> f >> trash >> t >> trash >> n) {
                face_vert_.push_back(--f);
                face_uv_.push_back(--t);
                face_norm_.push_back(--n);
                cnt++;
            }
            if (3 != cnt) {
                std::cerr << "Error: the obj file is supposed to be triangulated" << std::endl;
                in.close();
                return;
            }
        }
    }
    in.close();
    std::cerr << "# v# " << nVerts() << " f# " << nFaces() << " vt# " << uv_.size() << " vn# " << norms_.size() << std::endl;
}

int Model::nVerts() const
{
    return verts_.size();
}

int Model::nFaces() const
{
    return face_vert_.size() / 3;
}

vec3 Model::Normal(const int iface, const int nthvert) const
{
    return norms_[face_norm_[iface * 3 + nthvert]];
}

vec3 Model::Normal(const vec2& uv) const
{
    TGAColor c = normalmap_.get(uv[0] * normalmap_.get_width(), uv[1] * normalmap_.get_height());
    vec3 res;
    for (int i = 0; i < 3; i++)
        res[2 - i] = c[i] / 255. * 2 - 1;
    return res;
}

vec3 Model::Vert(const int i) const
{
    return verts_[i];
}

vec3 Model::Vert(const int iface, const int nthvert) const
{
    return verts_[face_vert_[iface * 3 + nthvert]];
}

vec2 Model::UV(const int iface, const int nthvert) const
{
    return uv_[face_uv_[iface * 3 + nthvert]];
}

vec3 Model::Diffuse(const vec2& uv) const
{
    TGAColor c = diffusemap_.get(uv[0] * diffusemap_.get_width(), uv[1] * diffusemap_.get_height());
    vec3 res;
    for (int i = 0; i < 3; i++)
        res[2 - i] = c[i] / 255.;
    return res;
}

double Model::Specular(const vec2& uv) const
{
    return specularmap_.get(uv[0] * specularmap_.get_width(), uv[1] * specularmap_.get_height())[0];
}

void Model::LoadTextureDiffuse(const std::string filename)
{
    LoadTexture(filename, diffusemap_);
}

void Model::LoadTextureSpecular(const std::string filename)
{
    LoadTexture(filename, specularmap_);
}

void Model::LoadTextureNormal(const std::string filename)
{
    LoadTexture(filename, normalmap_);
}

void Model::LoadCubeMap(const std::string filedir)
{
    std::string filename = filedir + "_top.tga";
    std::cerr << "texture file " << filename << " loading " << (cubemaps_[0].read_tga_file(filename.c_str()) ? "ok" : "failed") << std::endl;
    filename = filedir + "_bottom.tga";
    std::cerr << "texture file " << filename << " loading " << (cubemaps_[1].read_tga_file(filename.c_str()) ? "ok" : "failed") << std::endl;
    filename = filedir + "_left.tga";
    std::cerr << "texture file " << filename << " loading " << (cubemaps_[2].read_tga_file(filename.c_str()) ? "ok" : "failed") << std::endl;
    filename = filedir + "_right.tga";
    std::cerr << "texture file " << filename << " loading " << (cubemaps_[3].read_tga_file(filename.c_str()) ? "ok" : "failed") << std::endl;
    filename = filedir + "_front.tga";
    std::cerr << "texture file " << filename << " loading " << (cubemaps_[4].read_tga_file(filename.c_str()) ? "ok" : "failed") << std::endl;
    filename = filedir + "_back.tga";
    std::cerr << "texture file " << filename << " loading " << (cubemaps_[5].read_tga_file(filename.c_str()) ? "ok" : "failed") << std::endl;
    for(int i=0;i<6;++i)
        cubemaps_[i].flip_vertically();
}

vec3 Model::CubeMap(int index, vec2 uv)
{
    TGAColor c = cubemaps_[index].get(uv.x * cubemaps_[index].get_width(), uv.y * cubemaps_[index].get_height());
    vec3 res;
    for (int i = 0; i < 3; i++)
        res[2 - i] = c[i] / 255.;
    return res;
}

void Model::LoadTexture(const std::string filename, TGAImage& img)
{
    std::cerr << "texture file " << filename << " loading " << (img.read_tga_file(filename.c_str()) ? "ok" : "failed") << std::endl;
    //img.flip_vertically();
}
