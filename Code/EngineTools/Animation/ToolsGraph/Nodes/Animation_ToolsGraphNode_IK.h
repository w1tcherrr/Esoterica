#pragma once

#include "EngineTools/Animation/ToolsGraph/Nodes/Animation_ToolsGraphNode_VariationData.h"
#include "Engine/Animation/IK/IKRig.h"

//-------------------------------------------------------------------------

namespace EE::Animation
{
    class IKRigToolsNode final : public VariationDataToolsNode
    {
        EE_REFLECT_TYPE( IKRigToolsNode );

    public:

        struct Data final : public VariationDataToolsNode::Data
        {
            EE_REFLECT_TYPE( Data );

            virtual void GetReferencedResources( TInlineVector<ResourceID, 2>& outReferencedResources ) const override { outReferencedResources.emplace_back( m_rigDefinition.GetResourceID() ); }

        public:

            EE_REFLECT();
            TResourcePtr<IKRigDefinition>     m_rigDefinition;
        };

    public:

        IKRigToolsNode();

        virtual char const* GetTypeName() const override { return "IK Rig Solve"; }
        virtual char const* GetCategory() const override { return "Animation/IK"; }
        virtual TBitFlags<GraphType> GetAllowedParentGraphTypes() const override { return TBitFlags<GraphType>( GraphType::BlendTree ); }
        virtual int16_t Compile( GraphCompilationContext& context ) const override;

        virtual bool SupportsUserEditableDynamicInputPins() const override { return true; }
        virtual TInlineString<100> GetNewDynamicInputPinName() const override;
        virtual StringID GetDynamicInputPinValueType() const override { return GetPinTypeForValueType( GraphValueType::Target ); }
        virtual void PostDynamicPinDestruction() override;

    private:

        virtual TypeSystem::TypeInfo const* GetVariationDataTypeInfo() const override { return IKRigToolsNode::Data::s_pTypeInfo; }
    };

    //-------------------------------------------------------------------------

    class AimCSToolsNode final : public FlowToolsNode
    {
        EE_REFLECT_TYPE( AimCSToolsNode );

    public:

        AimCSToolsNode();

        virtual char const* GetTypeName() const override { return "Aim CS"; }
        virtual char const* GetCategory() const override { return "Animation/Compatibility"; }
        virtual TBitFlags<GraphType> GetAllowedParentGraphTypes() const override { return TBitFlags<GraphType>( GraphType::BlendTree ); }
        virtual int16_t Compile( GraphCompilationContext& context ) const override;

    private:

        EE_REFLECT();
        Seconds                                m_flActionBlendTimeSeconds = 0.4f;

        EE_REFLECT();
        Seconds                                m_flHandIKBlendInTimeSeconds = 0.3f;

        EE_REFLECT();
        Seconds                                m_flPlantingBlendTimeSeconds = 0.2f;
    };

    //-------------------------------------------------------------------------

    class FootIKToolsNode final : public VariationDataToolsNode
    {
        EE_REFLECT_TYPE( FootIKToolsNode );

    public:

        struct Data final : public VariationDataToolsNode::Data
        {
            EE_REFLECT_TYPE( Data );

        public:

            EE_REFLECT();
            StringID                            m_leftEffectorBoneName;

            EE_REFLECT();
            StringID                            m_rightEffectorBoneName;

            EE_REFLECT();
            Seconds                             m_flBlendTimeSeconds = 0.0f;
        };

    public:

        FootIKToolsNode();

        virtual char const* GetTypeName() const override { return "Foot IK"; }
        virtual char const* GetCategory() const override { return "Animation/Compatibility"; }
        virtual TBitFlags<GraphType> GetAllowedParentGraphTypes() const override { return TBitFlags<GraphType>( GraphType::BlendTree ); }
        virtual int16_t Compile( GraphCompilationContext& context ) const override;

    private:

        virtual TypeSystem::TypeInfo const* GetVariationDataTypeInfo() const override { return FootIKToolsNode::Data::s_pTypeInfo; }

    private:

        EE_REFLECT();
        bool                                   m_bIsTargetInWorldSpace = false;

        // Valve stores this as an enum, but for compatibility loading we only need to preserve the token.
        EE_REFLECT();
        StringID                               m_blendMode;
    };

    //-------------------------------------------------------------------------

    class FollowBoneToolsNode final : public VariationDataToolsNode
    {
        EE_REFLECT_TYPE( FollowBoneToolsNode );

    public:

        struct Data final : public VariationDataToolsNode::Data
        {
            EE_REFLECT_TYPE( Data );

        public:

            EE_REFLECT();
            StringID                            m_boneName;

            EE_REFLECT();
            StringID                            m_followTargetBoneName;
        };

    public:

        FollowBoneToolsNode();

        virtual char const* GetTypeName() const override { return "Follow Bone"; }
        virtual char const* GetCategory() const override { return "Animation/Compatibility"; }
        virtual TBitFlags<GraphType> GetAllowedParentGraphTypes() const override { return TBitFlags<GraphType>( GraphType::BlendTree ); }
        virtual int16_t Compile( GraphCompilationContext& context ) const override;

    private:

        virtual TypeSystem::TypeInfo const* GetVariationDataTypeInfo() const override { return FollowBoneToolsNode::Data::s_pTypeInfo; }

    private:

        // Valve stores this as an enum, but for compatibility loading we only need to preserve the token.
        EE_REFLECT();
        StringID                               m_mode;
    };

    //-------------------------------------------------------------------------

    class SnapWeaponToolsNode final : public FlowToolsNode
    {
        EE_REFLECT_TYPE( SnapWeaponToolsNode );

    public:

        SnapWeaponToolsNode();

        virtual char const* GetTypeName() const override { return "Snap Weapon"; }
        virtual char const* GetCategory() const override { return "Animation/Compatibility"; }
        virtual TBitFlags<GraphType> GetAllowedParentGraphTypes() const override { return TBitFlags<GraphType>( GraphType::BlendTree ); }
        virtual int16_t Compile( GraphCompilationContext& context ) const override;
    };

    //-------------------------------------------------------------------------

    class ChainLookatToolsNode final : public VariationDataToolsNode
    {
        EE_REFLECT_TYPE( ChainLookatToolsNode );

    public:

        struct Data final : public VariationDataToolsNode::Data
        {
            EE_REFLECT_TYPE( Data );

        public:

            EE_REFLECT();
            StringID                            m_chainEndBoneName;

            EE_REFLECT();
            Float3                              m_chainForwardDir = Float3( 1.0f, 0.0f, 0.0f );

            EE_REFLECT();
            int32_t                             m_nChainLength = 0;

            EE_REFLECT();
            Seconds                             m_flBlendTimeSeconds = 0.0f;
        };

    public:

        ChainLookatToolsNode();

        virtual char const* GetTypeName() const override { return "Chain Lookat"; }
        virtual char const* GetCategory() const override { return "Animation/Compatibility"; }
        virtual TBitFlags<GraphType> GetAllowedParentGraphTypes() const override { return TBitFlags<GraphType>( GraphType::BlendTree ); }
        virtual int16_t Compile( GraphCompilationContext& context ) const override;

    private:

        virtual TypeSystem::TypeInfo const* GetVariationDataTypeInfo() const override { return ChainLookatToolsNode::Data::s_pTypeInfo; }

    private:

        EE_REFLECT();
        bool                                   m_isTargetInWorldSpace = false;
    };
}
