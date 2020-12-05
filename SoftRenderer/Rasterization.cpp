//#include "Rasterization.h"
//#include "FrameBuffer.h"
//#include "Shader.h"
//#include "Mesh.h"
//
//namespace SoftRenderer
//{
//    void Rasterization::drawLine(FrameBuffer& fbo, const Vec4& begin, const Vec4& end)
//    {
//        int dx = floor(end.x) - floor(begin.x);
//        int dy = floor(end.y) - floor(begin.y);
//        int ux = (dx >= 0) ? 1 : -1;
//        int uy = (dy >= 0) ? 1 : -1;
//        int x = floor(begin.x);
//        int y = floor(begin.y);
//        int endx = floor(end.x);
//        int endy = floor(end.y);
//        int eps = 0;
//        dx = abs(dx);
//        dy = abs(dy);
//        if (dx > dy)
//        {
//            for (; x != endx; x += ux)
//            {
//                if (x >= 0 && x < fbo.size.x && y >= 0 && y < fbo.size.y)
//                {
//                    fbo.setColor(x, y, Vec4(1, 0, 0, 1));
//                }
//
//                eps += dy;
//                if ((eps << 1) >= dx)
//                {
//                    y += uy;
//                    eps -= dx;
//                }
//            }
//        }
//        else
//        {
//            for (; y != endy; y += uy)
//            {
//                if (x >= 0 && x < fbo.size.x && y >= 0 && y < fbo.size.y)
//                {
//                    fbo.setColor(x, y, Vec4(1, 0, 0, 1));
//                }
//
//                eps += dx;
//                if ((eps << 1) >= dy)
//                {
//                    x += ux;
//                    eps -= dy;
//                }
//            }
//        }
//    }
//
//    void Rasterization::drawTriangles(FrameBuffer& fbo, std::unique_ptr<Shader>& shader, const Mesh& mesh)
//    {
//        RasterizationParam param(fbo, shader);
//        if (mesh.indices.size() > 0)
//        {
//            for (size_t i = 0; i < mesh.indices.size(); i += 3)
//            {
//                param.v[0] = shader->vert(mesh.vertices[mesh.indices[i]]);
//                param.v[1] = shader->vert(mesh.vertices[mesh.indices[i + 1]]);
//                param.v[2] = shader->vert(mesh.vertices[mesh.indices[i + 2]]);
//                for (int j = 0; j < 3; j++)
//                {
//                    ShaderV2F& v = param.v[j];
//                    v.position.x /= v.position.w;
//                    v.position.y /= v.position.w;
//                    v.position.z /= v.position.w;
//                    v.mul(1.0f / v.position.w);
//                    // viewport
//                    v.position.x = (v.position.x + 1) * 0.5f * fbo.size.x;
//                    v.position.y = (v.position.y + 1) * 0.5f * fbo.size.y;
//                }
//                drawTriangle(param);
//            }
//        }
//        else
//        {
//            for (size_t i = 0; i < mesh.vertices.size(); i += 3)
//            {
//                param.v[0] = shader->vert(mesh.vertices[i]);
//                param.v[1] = shader->vert(mesh.vertices[i + 1]);
//                param.v[2] = shader->vert(mesh.vertices[i + 2]);
//                for (int j = 0; j < 3; j++)
//                {
//                    ShaderV2F& v = param.v[j];
//                    v.position.x /= v.position.w;
//                    v.position.y /= v.position.w;
//                    v.position.z /= v.position.w;
//                    v.mul(1.0f / v.position.w);
//                    // viewport
//                    v.position.x = (v.position.x + 1) * 0.5f * fbo.size.x;
//                    v.position.y = (v.position.y + 1) * 0.5f * fbo.size.y;
//                }
//                drawTriangle(param);
//            }
//        }
//    }
//
//    void Rasterization::drawTriangle(RasterizationParam& param)
//    {
//        // sort y
//        if (param.v[0].position.y > param.v[1].position.y)
//        {
//            std::swap(param.v[0], param.v[1]);
//        }
//        if (param.v[1].position.y > param.v[2].position.y)
//        {
//            std::swap(param.v[1], param.v[2]);
//        }
//        if (param.v[0].position.y > param.v[1].position.y)
//        {
//            std::swap(param.v[0], param.v[1]);
//        }
//
//        if (param.v[0].position.y == param.v[1].position.y)
//        {
//            if (param.v[0].position.x > param.v[1].position.x)
//            {
//                std::swap(param.v[0], param.v[1]);
//            }
//            drawFlatBotTriangle(param);
//        }
//        else if (param.v[1].position.y == param.v[2].position.y)
//        {
//            if (param.v[1].position.x > param.v[2].position.x)
//            {
//                std::swap(param.v[1], param.v[2]);
//            }
//            drawFlatTopTriangle(param);
//        }
//        else
//        {
//            float t = (param.v[1].position.y - param.v[0].position.y) / (param.v[2].position.y - param.v[0].position.y);
//            Vec4 pEx = lerp(param.v[0].position, param.v[2].position, t);
//
//            if (param.v[1].position.x > pEx.x)
//            {
//                std::swap(param.v[1], param.v[2]);
//                drawFlatTopTriangle(param);
//                std::swap(param.v[1], param.v[2]);
//            }
//            else
//            {
//                drawFlatTopTriangle(param);
//            }
//            
//            if (pEx.x > param.v[1].position.x)
//            {
//                std::swap(param.v[0], param.v[1]);
//            }
//            drawFlatBotTriangle(param);
//        }
//    }
//
//    void Rasterization::drawFlatTopTriangle(RasterizationParam& param)
//    {
//        //  1-------2
//        //   \     /
//        //    \   /
//        //     \ /
//        //      0
//        float yel = floor(param.v[1].position.y);
//        float yer = floor(param.v[2].position.y);
//        float ys = floor(param.v[0].position.y);
//        float ye = std::min(yel, yer);
//        int loopLow = clamp(ys, 0.0f, param.fbo.size.y);
//        int loopHigh = clamp(ye, 0.0f, param.fbo.size.y);
//        for (int y = loopLow; y < loopHigh; y++)
//        {
//            param.vfl.fromLerp(param.v[0], param.v[1], (y - ys) / (yel - ys));
//            param.vfr.fromLerp(param.v[0], param.v[2], (y - ys) / (yer - ys));
//            drawScanLine(param, y);
//        }
//
//        //drawLine(param.fbo, param.v[0].position, param.v[1].position);
//        //drawLine(param.fbo, param.v[1].position, param.v[2].position);
//        //drawLine(param.fbo, param.v[2].position, param.v[0].position);
//    }
//
//    void Rasterization::drawFlatBotTriangle(RasterizationParam& param)
//    {
//        //      2
//        //     / \
//        //    /   \
//        //   /     \
//        //  0-------1
//        float ysl = floor(param.v[0].position.y);
//        float ysr = floor(param.v[1].position.y);
//        float ys = std::max(ysl, ysr);
//        float ye = floor(param.v[2].position.y);
//        int loopLow = clamp(ys, 0.0f, param.fbo.size.y);
//        int loopHigh = clamp(ye, 0.0f, param.fbo.size.y);
//        for (int y = loopLow; y < loopHigh; y++)
//        {
//            param.vfl.fromLerp(param.v[2], param.v[0], (ye - y) / (ye - ysl));
//            param.vfr.fromLerp(param.v[2], param.v[1], (ye - y) / (ye - ysr));
//            drawScanLine(param, y);
//        }
//
//        //drawLine(param.fbo, param.v[0].position, param.v[1].position);
//        //drawLine(param.fbo, param.v[1].position, param.v[2].position);
//        //drawLine(param.fbo, param.v[2].position, param.v[0].position);
//    }
//
//    void Rasterization::drawScanLine(RasterizationParam& param, int y)
//    {
//        float xs = floor(param.vfl.position.x);
//        float xe = floor(param.vfr.position.x);
//        int loopLow = clamp(xs, 0.0f, param.fbo.size.x);
//        int loopHigh = clamp(xe, 0.0f, param.fbo.size.x);
//        for (int x = loopLow; x < loopHigh; x++)
//        {
//            float t = (x - xs) / (xe - xs);
//            param.vfm.fromLerp(param.vfl, param.vfr, t);
//            if (depthTest(param.vfm.position.z, param.fbo.getDepth(x, y)))
//            {
//                param.vfm.mul(param.vfm.position.w);
//                Vec4 color = param.shader->frag(param.vfm);
//                color = alphaBlend(color, param.fbo.getColor(x, y));
//                param.fbo.setColor(x, y, color);
//                param.fbo.setDepth(x, y, param.vfm.position.z);
//            }
//        }
//    }
//
//    bool Rasterization::depthTest(float src, float dst)
//    {
//        //return src <= 1 && src >= dst;
//        return src <= dst;
//    }
//
//    Vec4 Rasterization::alphaBlend(const Vec4& src, const Vec4& dst)
//    {
//        float alpha = src.w;
//        return Vec4(src.x * alpha + dst.x * (1 - alpha), src.y * alpha + dst.y * (1 - alpha), src.z * alpha + dst.z * (1 - alpha), 1);
//    }
//}
