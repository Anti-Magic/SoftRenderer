//#include "TextureCube.h"
//#include "Texture.h"
//#include "Device.h"
//
//TextureCube::TextureCube(std::string paths[6]) : Texture()
//{
//    for (int i = 0; i < 6; i++)
//    {
//        texs[i] = Device::LoadTexture2D(paths[i]);
//    }
//}
//
//Vec4 TextureCube::getColor(Vec4 pos)
//{
//    float absx = abs(pos.x);
//    float absy = abs(pos.y);
//    float absz = abs(pos.z);
//    int texIdx;
//    float sc;
//    float tc;
//    float ma;
//    if (absx >= absy && absx >= absz)
//    {
//        ma = absx;
//        if (pos.x > 0)
//        {
//            texIdx = 0;
//            sc = -pos.z;
//            tc = -pos.y;
//        }
//        else
//        {
//            texIdx = 1;
//            sc = pos.z;
//            tc = -pos.y;
//        }
//    }
//    else if (absy >= absx && absy >= absz)
//    {
//        ma = absy;
//        if (pos.y > 0)
//        {
//            texIdx = 2;
//            sc = pos.x;
//            tc = pos.z;
//        }
//        else
//        {
//            texIdx = 3;
//            sc = pos.x;
//            tc = -pos.z;
//        }
//    }
//    else //if (absz >= absx && absz >= absy)
//    {
//        ma = absz;
//        if (pos.z > 0)
//        {
//            texIdx = 4;
//            sc = pos.x;
//            tc = -pos.y;
//        }
//        else
//        {
//            texIdx = 5;
//            sc = -pos.x;
//            tc = -pos.y;
//        }
//    }
//
//    float s = 0.5f * (sc / ma + 1.0f);
//    float t = 0.5f * (tc / ma + 1.0f);
//    return texs[texIdx].getColor(Vec2(s, t));
//}
