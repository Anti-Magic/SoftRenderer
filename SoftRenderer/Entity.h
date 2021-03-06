#pragma once

#include "Vec.h"
#include "Quat.h"
#include "Mat.h"
#include "Mesh.h"
#include "Shader.h"

namespace SoftRenderer
{
    class Entity
    {
    public:
        Mesh mesh;
        std::unique_ptr<Shader> shader;

        Entity() 
            : pos()
            , rot()
            , scale(1, 1, 1) 
            , transformDirty(true)
        {
        }

        Vec3 GetPos()
        {
            return pos;
        }

        void SetPos(const Vec3& v)
        {
            pos = v;
            transformDirty = true;
        }

        Quat GetRot()
        {
            return rot;
        }

        void SetRot(const Quat& v)
        {
            rot = v;
            transformDirty = true;
        }

        Vec3 GetScale()
        {
            return scale;
        }

        void SetScale(const Vec3& v)
        {
            scale = v;
            transformDirty = true;
        }

        void LookAt(const Vec3& center, const Vec3& up)
        {
            Mat4 viewMat = Mat4::lookAt(pos, center, up);
            SetRot(Quat::fromMat4(viewMat.inverse()));
        }

        Vec3 Right()
        {
            return Vec3(1, 0, 0) * rot;
        }

        Vec3 Up()
        {
            return Vec3(0, 1, 0) * rot;
        }

        Vec3 Forward()
        {
            return Vec3(0, 0, 1) * rot;
        }

        virtual Mat4 GetModelMatrix()
        {
            if (transformDirty)
            {
                transformDirty = false;
                modelMatrix = Mat4::scale(scale) * Mat4::rotate(rot) * Mat4::translate(pos);
            }
            return modelMatrix;
        }
        
    protected:
        Vec3 pos;
        Quat rot;
        Vec3 scale;
        bool transformDirty;
        Mat4 modelMatrix;
    };
}
