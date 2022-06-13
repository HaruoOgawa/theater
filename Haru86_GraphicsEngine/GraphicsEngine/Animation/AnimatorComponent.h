#pragma once
#include "GraphicsEngine/Component/Component.h"

class Object;

namespace animation {
    class Skeleton;
    class Pose;
    class Clip;

    class AnimatorComponent :
        public Component
    {
        std::shared_ptr<Skeleton> m_Skeleton;
        std::vector<glm::mat4> m_PoseMatrix;
        std::shared_ptr<Pose> m_CurrentPose;

        std::vector<std::shared_ptr<animation::Clip>> m_AnimationClipList;

        float m_PlaybackTime;
    public:
        AnimatorComponent(Object* o);
        virtual ~AnimatorComponent();

        virtual void Update() override;
        virtual void Draw();
        virtual void ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) override;

        void Set(const std::shared_ptr<Skeleton>& Skeleton, const std::vector<std::shared_ptr<animation::Clip>>& AnimationClipList);

        const std::shared_ptr<Skeleton>& GetSkeleton()const;
        const std::vector<glm::mat4>& GetPoseMatrix()const;
        const std::shared_ptr<Pose>& GetCurrentPose()const;
    };
}