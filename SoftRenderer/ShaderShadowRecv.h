#pragma once

#include "Shader.h"

namespace SoftRenderer
{
    class ShaderShadowRecv : public Shader
    {
    public:
        ShaderV2F vert(const Vertex& v) override
        {
            ShaderV2F o;
            o.position = v.position * mvp;
            // lightSpacePos
            o.texcoord3 = v.position * uniformM0;
            return o;
        }

        Vec4 frag(const ShaderV2F& f) override
        {
            float shadow = shadowCalculation(f.texcoord3);
            if (shadow > 0)
            {
                return Vec4(0, 0.5f, 0, 1);
            }
            return Vec4(0, 1, 0, 1);
        }

        float shadowCalculation(Vec4 fragPosLightSpace)
        {
            // ִ��͸�ӳ���
            Vec3 projCoords;
            projCoords.x = fragPosLightSpace.x / fragPosLightSpace.w;
            projCoords.y = fragPosLightSpace.y / fragPosLightSpace.w;
            projCoords.z = fragPosLightSpace.z / fragPosLightSpace.w;
            // �任��[0,1]�ķ�Χ
            Vec2 uv = Vec2(projCoords.x * 0.5f + 0.5f, projCoords.y * 0.5f + 0.5f);
            // ȡ�����������(ʹ��[0,1]��Χ�µ�fragPosLight������)
            float closestDepth = texture0->getColor(uv).x;
            // ȡ�õ�ǰƬԪ�ڹ�Դ�ӽ��µ����
            float currentDepth = projCoords.z;// *0.5f + 0.5f;
            // ��鵱ǰƬԪ�Ƿ�����Ӱ��
            float bias = 0.0002f;
            //Vec3 lightDir = lighting.dir.normalize();
            //float bias = max(0.002 * (1.0 - dot(v_normal, lightDir)), 0.0002);
            float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
            //float shadow = currentDepth > closestDepth ? 1.0f : 0.0f;

            if (projCoords.z > 1.0f)
            {
                shadow = 0.0f;
            }
            if (shadow != 0.0f)
            {
                int x = 6;
            }
            return shadow;
        }
    };
}
