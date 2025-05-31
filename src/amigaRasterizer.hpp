#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <cstdint>
#include <cmath>
#include "objects/tetrakis.hpp"
#include "objects/torus.hpp"
#include "objects/test.hpp"
#include "objects/cube.hpp"
#include "objects/ascLoader.hpp"
#include "objects/objLoader.hpp"
#include "scene.hpp"
#include "slib.hpp"
#include "smath.hpp"
#include "tri.hpp"

enum class ClipPlane {
    Left, Right, Bottom, Top, Near, Far
};

template<class Effect>
class AmigaRasterizer : Rasterizer<Effect> {
    public:
        AmigaRasterizer() :  fullTransformMat(smath::identity()), 
                             normalTransformMat(smath::identity()),
                             viewMatrix(smath::identity())
          {}

    private:
        
        std::vector<std::pair<int, float>> SortFaces() {

           // Create vector of (index, depth) pairs
            std::vector<std::pair<int, float>> faceIndicesWithDepth;
            faceIndicesWithDepth.reserve(solid->faceData.size());

            for (int i = 0; i < static_cast<int>(solid->faceData.size()); ++i) {
                const auto& face = solid->faceData[i].face;

                float z1 = projectedPoints[face.vertex1]->ndc.z / projectedPoints[face.vertex1]->ndc.w;
                float z2 = projectedPoints[face.vertex2]->ndc.z / projectedPoints[face.vertex2]->ndc.w;
                float z3 = projectedPoints[face.vertex3]->ndc.z / projectedPoints[face.vertex3]->ndc.w;

                float averageZ = (z1 + z2 + z3) / 3.0f;
                faceIndicesWithDepth.emplace_back(i, averageZ);
            }
            
            // Sort the vector by depth (second element of the pair)
            std::sort(faceIndicesWithDepth.begin(), faceIndicesWithDepth.end(),
                [](const auto& a, const auto& b) {
                    return a.second > b.second;
                });

            return faceIndicesWithDepth;
        }


        void DrawFaces() {

            std::vector<std::pair<int, float>> faceIndicesWithDepth = SortFaces();

            //#pragma omp parallel for
            for (int i = 0; i < static_cast<int>(faceIndicesWithDepth.size()); ++i) {
                const auto& faceDataEntry = solid->faceData[faceIndicesWithDepth[i].first];
                const auto& face = faceDataEntry.face;
                slib::vec3 rotatedFaceNormal;
                rotatedFaceNormal = normalTransformMat * slib::vec4(faceDataEntry.faceNormal, 0);

                vertex* p1 = projectedPoints[face.vertex1].get();

                if (Visible(p1->world, rotatedFaceNormal)) {
                    Triangle<vertex> tri(
                        *projectedPoints[face.vertex1],
                        *projectedPoints[face.vertex2],
                        *projectedPoints[face.vertex3],
                        face,
                        rotatedFaceNormal,
                        solid->materials.at(face.materialKey)
                    );

                    ClipCullDrawTriangleSutherlandHodgman(tri); // Must be thread-safe!
                }
            }
        }

        void draw(Triangle<vertex>& tri) {

            auto* pixels = static_cast<uint32_t*>(scene->sdlSurface->pixels);
            effect.vs.viewProjection(*scene, tri.p1);
            effect.vs.viewProjection(*scene, tri.p2);
            effect.vs.viewProjection(*scene, tri.p3);
            orderVertices(&tri.p1, &tri.p2, &tri.p3);
            if(tri.p1.p_y == tri.p3.p_y) return;

            effect.gs(tri, *scene);

            drawAmiga(tri.p1.p_x, tri.p1.p_y, tri.p2.p_x, tri.p2.p_y, tri.p3.p_x, tri.p3.p_y, tri.flatColor, pixels);
        };

        inline void orderVertices(vertex *p1, vertex *p2, vertex *p3) {
            if (p1->p_y > p2->p_y) std::swap(*p1,*p2);
            if (p2->p_y > p3->p_y) std::swap(*p2,*p3);
            if (p1->p_y > p2->p_y) std::swap(*p1,*p2);
        };
        
        void drawAmiga(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color, uint32_t* pixels) {
            int x,y;
            int16_t line[scene->screen.height*2];
            for(y=0;y<scene->screen.height;y++) { line[2*y+0] = scene->screen.width+1; line[2*y+1] = -1; }

            rasterizeAmiga( x0, y0, x1, y1, line);
            rasterizeAmiga( x1, y1, x2, y2, line);
            rasterizeAmiga( x2, y2, x0, y0, line);

            for(y=0;y<scene->screen.height;y++)
                if(line[2*y+1]>line[2*y+0]) for(x=line[2*y+0]; x<=line[2*y+1]; x++) pixels[scene->screen.width*y+x] = color;
        }

        void rasterizeAmiga(int x0, int y0, int x1, int y1, int16_t *line) 
        {
            if((y0<0 && y1<0) || (y0>=scene->screen.height && y1>=scene->screen.height)) return; // exit if line outside rasterized area
            int dx = std::abs(x1 - x0);
            int dy = std::abs(y1 - y0);
            int err2,err = dx-dy;
            int sx = (x0 < x1) ? 1 : -1;
            int sy = (y0 < y1) ? 1 : -1;
            
            while(1)  {
                if(y0>=0 && y0<scene->screen.height) {
                    if(x0<line[2*(y0)+0]) line[2*(y0)+0] = x0>0 ? x0 : 0;
                    if(x0>line[2*(y0)+1]) line[2*(y0)+1] = x0<scene->screen.width ? x0 : scene->screen.width-1;
                }

                if(x0==x1 && y0==y1)  return;
                err2 = err+err;
                if(err2 > -dy) { err -= dy; x0 += sx; }
                if(err2 < dx)  { err += dx; y0 += sy; }
            }
        }

    };
    

