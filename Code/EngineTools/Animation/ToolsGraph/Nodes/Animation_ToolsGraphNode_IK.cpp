#include "Animation_ToolsGraphNode_IK.h"
#include "EngineTools/Animation/ToolsGraph/Animation_ToolsGraph_Compilation.h"
#include "Engine/Animation/Graph/Nodes/Animation_RuntimeGraphNode_IK.h"

//-------------------------------------------------------------------------

namespace EE::Animation
{
    IKRigToolsNode::IKRigToolsNode()
        : VariationDataToolsNode()
    {
        CreateOutputPin( "Result", GraphValueType::Pose );
        CreateInputPin( "Input", GraphValueType::Pose );
        CreateDynamicInputPin( "Effector 0", GetPinTypeForValueType( GraphValueType::Target ) );
        CreateDynamicInputPin( "Effector 1", GetPinTypeForValueType( GraphValueType::Target ) );

        m_defaultVariationData.CreateInstance( GetVariationDataTypeInfo() );
    }

    int16_t IKRigToolsNode::Compile( GraphCompilationContext& context ) const
    {
        IKRigNode::Definition* pDefinition = nullptr;
        NodeCompilationState const state = context.GetDefinition<IKRigNode>( this, pDefinition );
        if ( state == NodeCompilationState::NeedCompilation )
        {
            auto pInputNode = GetConnectedInputNode<FlowToolsNode>( 0 );
            if ( pInputNode != nullptr )
            {
                int16_t const compiledNodeIdx = pInputNode->Compile( context );
                if ( compiledNodeIdx != InvalidIndex )
                {
                    pDefinition->m_childNodeIdx = compiledNodeIdx;
                }
                else
                {
                    return InvalidIndex;
                }
            }
            else
            {
                context.LogError( this, "Disconnected input pin!" );
                return InvalidIndex;
            }

            //-------------------------------------------------------------------------

            for ( auto i = 1; i < GetNumInputPins(); i++ )
            {
                auto pEffectorTargetNode = GetConnectedInputNode<FlowToolsNode>( i );
                if ( pEffectorTargetNode != nullptr )
                {
                    int16_t const compiledEffectorTargetNodeIdx = pEffectorTargetNode->Compile( context );
                    if ( compiledEffectorTargetNodeIdx != InvalidIndex )
                    {
                        pDefinition->m_effectorTargetIndices.emplace_back( compiledEffectorTargetNodeIdx );
                    }
                    else
                    {
                        return InvalidIndex;
                    }
                }
                else
                {
                    pDefinition->m_effectorTargetIndices.emplace_back( (int16_t) InvalidIndex );
                    return InvalidIndex;
                }
            }
        }

        auto pData = GetResolvedVariationDataAs<Data>( context.GetVariationHierarchy(), context.GetVariationID() );
        pDefinition->m_dataSlotIdx = context.RegisterResource( pData->m_rigDefinition.GetResourceID() );

        return pDefinition->m_nodeIdx;
    }

    TInlineString<100> IKRigToolsNode::GetNewDynamicInputPinName() const
    {
        int32_t const numInputPins = GetNumInputPins();
        TInlineString<100> pinName;
        pinName.sprintf( "Effector %d", numInputPins - 1 );
        return pinName;
    }

    void IKRigToolsNode::PostDynamicPinDestruction()
    {
        FlowToolsNode::PostDynamicPinDestruction();

        int32_t const numInputPins = GetNumInputPins();
        for ( auto i = 1; i < numInputPins; i++ )
        {
            TInlineString<100> newPinName;
            newPinName.sprintf( "Effector %d", i - 1 );
            GetInputPin( i )->m_name = newPinName;
        }
    }

    //-------------------------------------------------------------------------

    AimCSToolsNode::AimCSToolsNode()
        : FlowToolsNode()
    {
        CreateOutputPin( "Result", GraphValueType::Pose, true );
        CreateInputPin( "Input", GraphValueType::Pose );
        CreateInputPin( "Horizontal Aim Angle", GraphValueType::Float );
        CreateInputPin( "Vertical Aim Angle", GraphValueType::Float );
        CreateInputPin( "Weapon Category", GraphValueType::ID );
        CreateInputPin( "Weapon Type", GraphValueType::ID );
        CreateInputPin( "Weapon Action", GraphValueType::ID );
        CreateInputPin( "Weapon Drop", GraphValueType::Float );
        CreateInputPin( "Crouch Weight", GraphValueType::Float );
        CreateInputPin( "Is Defusing", GraphValueType::Bool );
    }

    int16_t AimCSToolsNode::Compile( GraphCompilationContext& context ) const
    {
        auto pInputNode = GetConnectedInputNode<FlowToolsNode>( 0 );
        if ( pInputNode != nullptr )
        {
            return pInputNode->Compile( context );
        }

        context.LogError( this, "Disconnected input pin on compatibility Aim CS node!" );
        return InvalidIndex;
    }

    //-------------------------------------------------------------------------

    FootIKToolsNode::FootIKToolsNode()
        : VariationDataToolsNode()
    {
        CreateOutputPin( "Result", GraphValueType::Pose, true );
        CreateInputPin( "Input", GraphValueType::Pose );
        CreateInputPin( "Left Foot Target", GraphValueType::Target );
        CreateInputPin( "Right Foot Target", GraphValueType::Target );
        CreateInputPin( "Enabled", GraphValueType::Bool );

        m_defaultVariationData.CreateInstance( GetVariationDataTypeInfo() );
    }

    int16_t FootIKToolsNode::Compile( GraphCompilationContext& context ) const
    {
        auto pInputNode = GetConnectedInputNode<FlowToolsNode>( 0 );
        if ( pInputNode != nullptr )
        {
            return pInputNode->Compile( context );
        }

        context.LogError( this, "Disconnected input pin on compatibility Foot IK node!" );
        return InvalidIndex;
    }

    //-------------------------------------------------------------------------

    FollowBoneToolsNode::FollowBoneToolsNode()
        : VariationDataToolsNode()
    {
        CreateOutputPin( "Result", GraphValueType::Pose, true );
        CreateInputPin( "Input", GraphValueType::Pose );
        CreateInputPin( "Enabled", GraphValueType::Bool );

        m_defaultVariationData.CreateInstance( GetVariationDataTypeInfo() );
    }

    int16_t FollowBoneToolsNode::Compile( GraphCompilationContext& context ) const
    {
        auto pInputNode = GetConnectedInputNode<FlowToolsNode>( 0 );
        if ( pInputNode != nullptr )
        {
            return pInputNode->Compile( context );
        }

        context.LogError( this, "Disconnected input pin on compatibility Follow Bone node!" );
        return InvalidIndex;
    }

    //-------------------------------------------------------------------------

    SnapWeaponToolsNode::SnapWeaponToolsNode()
        : FlowToolsNode()
    {
        CreateOutputPin( "Result", GraphValueType::Pose, true );
        CreateInputPin( "Input", GraphValueType::Pose );
        CreateInputPin( "Flashed Amount", GraphValueType::Float );
        CreateInputPin( "Weapon Category", GraphValueType::ID );
        CreateInputPin( "Weapon Type", GraphValueType::ID );
    }

    int16_t SnapWeaponToolsNode::Compile( GraphCompilationContext& context ) const
    {
        auto pInputNode = GetConnectedInputNode<FlowToolsNode>( 0 );
        if ( pInputNode != nullptr )
        {
            return pInputNode->Compile( context );
        }

        context.LogError( this, "Disconnected input pin on compatibility Snap Weapon node!" );
        return InvalidIndex;
    }

    //-------------------------------------------------------------------------

    ChainLookatToolsNode::ChainLookatToolsNode()
        : VariationDataToolsNode()
    {
        CreateOutputPin( "Result", GraphValueType::Pose, true );
        CreateInputPin( "Input", GraphValueType::Pose );
        CreateInputPin( "Target", GraphValueType::Target );
        CreateInputPin( "Weight", GraphValueType::Float );
    }

    int16_t ChainLookatToolsNode::Compile( GraphCompilationContext& context ) const
    {
        auto pInputNode = GetConnectedInputNode<FlowToolsNode>( 0 );
        if ( pInputNode != nullptr )
        {
            return pInputNode->Compile( context );
        }

        context.LogError( this, "Disconnected input pin on compatibility Chain Lookat node!" );
        return InvalidIndex;
    }
}
