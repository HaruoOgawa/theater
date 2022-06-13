#include "AnimatorComponent.h"
#include "GraphicsEngine/Object/Object.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Animation/Skeleton.h"
#include "GraphicsEngine/Animation/Pose.h"
#include "GraphicsEngine/Animation/Clip.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Animation/AnimatorComponent.h"

namespace animation {
    AnimatorComponent::AnimatorComponent(Object* o):
        Component(o),
        m_PlaybackTime(0.0f)
    {
    }

    AnimatorComponent::~AnimatorComponent() {
        m_PoseMatrix.clear();
    }

    void AnimatorComponent::Set(const std::shared_ptr<Skeleton>& Skeleton, const std::vector<std::shared_ptr<animation::Clip>>& AnimationClipList) {
        //
        m_Skeleton = Skeleton;
        m_AnimationClipList = AnimationClipList;

         // ポーズの初期化
        m_CurrentPose = m_Skeleton->GetBindPose();
        //m_CurrentPose = m_Skeleton->GetRestPose();
        m_CurrentPose->GetMatrixPalette(m_PoseMatrix);
    }

    void AnimatorComponent::Update() {
        // アニメーションの更新
        /*if (m_AnimationClipList.size()>0) {
            m_PlaybackTime = m_AnimationClipList[1]->Sample(m_CurrentPose, m_PlaybackTime + GraphicsMain::GetInstance()->deltaTime);
            m_PoseMatrix.clear();
            m_CurrentPose->GetMatrixPalette(m_PoseMatrix);
        }*/
    }

    void AnimatorComponent::Draw() {
    }
    void AnimatorComponent::ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) {

    }

    const std::shared_ptr<Skeleton>& AnimatorComponent::GetSkeleton()const {
        return m_Skeleton;
    }

    const std::vector<glm::mat4>& AnimatorComponent::GetPoseMatrix()const {
        return m_PoseMatrix;
    }

    const std::shared_ptr<Pose>& AnimatorComponent::GetCurrentPose()const {
        return m_CurrentPose;
    }
}