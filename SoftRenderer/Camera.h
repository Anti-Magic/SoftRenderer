#pragma once

#include "Entity.h"

namespace SoftRenderer
{
	class Camera : public Entity
	{
	public:
        Camera() 
            : Entity()
            , viewProjDirty(true)
        {
        }

        void setProjMatrix(const Mat4& v)
        {
            projMatrix = v;
            viewProjDirty = true;
        }

        virtual Mat4 getModelMatrix() override
        {
            if (transformDirty)
            {
                transformDirty = false;
                viewProjDirty = true;
                modelMatrix = Mat4::scale(scale) * Mat4::rotate(rot) * Mat4::translate(pos);
                viewMatrix = modelMatrix.inverse();
            }
            return modelMatrix;
        }

        Mat4 getViewProjMatrix()
        {
            if (transformDirty)
            {
                getModelMatrix();
            }
            if (viewProjDirty)
            {
                viewProjMatrix = viewMatrix * projMatrix;
            }
            return viewProjMatrix;
        }

    protected:
        Mat4 viewMatrix;
        Mat4 projMatrix;
        Mat4 viewProjMatrix;
        bool viewProjDirty;
	};
}
