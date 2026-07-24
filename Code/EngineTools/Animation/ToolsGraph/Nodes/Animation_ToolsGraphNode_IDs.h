#pragma once
#include "Animation_ToolsGraphNode.h"
#include "Animation_ToolsGraphNode_VariationData.h"
#include "Engine/Animation/Graph/Nodes/Animation_RuntimeGraphNode_IDs.h"

//-------------------------------------------------------------------------

namespace EE::Animation
{
    class IDComparisonToolsNode final : public FlowToolsNode
    {
        EE_REFLECT_TYPE( IDComparisonToolsNode );

    public:

        IDComparisonToolsNode();

        virtual char const* GetTypeName() const override { return "ID Comparison"; }
        virtual char const* GetCategory() const override { return "Values/ID"; }
        virtual TBitFlags<GraphType> GetAllowedParentGraphTypes() const override { return TBitFlags<GraphType>( GraphType::BlendTree, GraphType::ValueTree, GraphType::TransitionConduit ); }
        virtual int16_t Compile( GraphCompilationContext& context ) const override;

        virtual void DrawInfoText( NodeGraph::DrawContext const& ctx, NodeGraph::UserContext* pUserContext ) override;
        virtual void GetLogicAndEventIDs( TVector<StringID>& outIDs ) const override;
        virtual void RenameLogicAndEventIDs( StringID oldID, StringID newID ) override;

    private:

        EE_REFLECT();
        IDComparisonNode::Comparison     m_comparison = IDComparisonNode::Comparison::Matches;

        EE_REFLECT( CustomEditor = "AnimGraph_ID" );
        TVector<StringID>                m_IDs;
    };

    //-------------------------------------------------------------------------

    class IDToFloatToolsNode final : public FlowToolsNode
    {
        EE_REFLECT_TYPE( IDToFloatToolsNode );

        struct Mapping : public IReflectedType
        {
            EE_REFLECT_TYPE( Mapping );

            EE_REFLECT( CustomEditor = "AnimGraph_ID" );
            StringID    m_ID;

            EE_REFLECT();
            float       m_value;
        };

    public:

        IDToFloatToolsNode();

        virtual char const* GetTypeName() const override { return "ID to Float"; }
        virtual char const* GetCategory() const override { return "Values/ID"; }
        virtual TBitFlags<GraphType> GetAllowedParentGraphTypes() const override { return TBitFlags<GraphType>( GraphType::BlendTree, GraphType::ValueTree, GraphType::TransitionConduit ); }
        virtual int16_t Compile( GraphCompilationContext& context ) const override;
        virtual void DrawInfoText( NodeGraph::DrawContext const& ctx, NodeGraph::UserContext* pUserContext ) override;
        virtual void GetLogicAndEventIDs( TVector<StringID>& outIDs ) const override;
        virtual void RenameLogicAndEventIDs( StringID oldID, StringID newID ) override;

    private:

        bool ValidateMappings() const;

    private:

        EE_REFLECT();
        float                            m_defaultValue = 0.0f;

        EE_REFLECT();
        TVector<Mapping>                 m_mappings;
    };

    //-------------------------------------------------------------------------

    class IDSwitchToolsNode final : public FlowToolsNode
    {
        EE_REFLECT_TYPE( IDSwitchToolsNode );

    public:

        IDSwitchToolsNode();

        virtual char const* GetTypeName() const override { return "ID Switch"; }
        virtual char const* GetCategory() const override { return "Values/ID"; }
        virtual TBitFlags<GraphType> GetAllowedParentGraphTypes() const override { return TBitFlags<GraphType>( GraphType::BlendTree, GraphType::ValueTree, GraphType::TransitionConduit ); }
        virtual int16_t Compile( GraphCompilationContext& context ) const override;

    private:

        // Valve stores both results on the node instead of taking them from input pins.
        EE_REFLECT();
        StringID                         m_trueValue;

        EE_REFLECT();
        StringID                         m_falseValue;
    };

    //-------------------------------------------------------------------------

    // The ID counterpart of the dynamic float selector: each dynamic bool input pin selects the
    // option at the same index, falling back to the default ID when none of them are set.
    class IDSelectorToolsNode final : public FlowToolsNode
    {
        EE_REFLECT_TYPE( IDSelectorToolsNode );

    public:

        IDSelectorToolsNode();

        virtual bool IsRenameable() const override { return true; }

        virtual char const* GetTypeName() const override { return "Dynamic ID Selector"; }
        virtual char const* GetCategory() const override { return "Values/ID"; }
        virtual TBitFlags<GraphType> GetAllowedParentGraphTypes() const override { return TBitFlags<GraphType>( GraphType::BlendTree, GraphType::ValueTree, GraphType::TransitionConduit ); }
        virtual int16_t Compile( GraphCompilationContext& context ) const override;

        virtual bool SupportsUserEditableDynamicInputPins() const override { return true; }
        virtual TInlineString<100> GetNewDynamicInputPinName() const override { return "Option"; }
        virtual StringID GetDynamicInputPinValueType() const override { return GetPinTypeForValueType( GraphValueType::Bool ); }

    private:

        EE_REFLECT();
        StringID                         m_defaultID;

        EE_REFLECT( ShowAsStaticArray );
        TVector<StringID>                m_options;
    };

    //-------------------------------------------------------------------------

    // Compares an input ID against a per-variation list of IDs.
    class VariationIDComparisonToolsNode final : public VariationDataToolsNode
    {
        EE_REFLECT_TYPE( VariationIDComparisonToolsNode );

    public:

        struct Data final : public VariationDataToolsNode::Data
        {
            EE_REFLECT_TYPE( Data );

        public:

            EE_REFLECT( ShowAsStaticArray );
            TVector<StringID>            m_IDs;
        };

    public:

        VariationIDComparisonToolsNode();

        virtual char const* GetTypeName() const override { return "Variation ID Comparison"; }
        virtual char const* GetCategory() const override { return "Animation/Compatibility"; }
        virtual TBitFlags<GraphType> GetAllowedParentGraphTypes() const override { return TBitFlags<GraphType>( GraphType::BlendTree, GraphType::ValueTree, GraphType::TransitionConduit ); }
        virtual int16_t Compile( GraphCompilationContext& context ) const override;

    private:

        virtual TypeSystem::TypeInfo const* GetVariationDataTypeInfo() const override { return VariationIDComparisonToolsNode::Data::s_pTypeInfo; }

    private:

        // Valve stores this as an enum, but for compatibility loading we only need to preserve the token.
        EE_REFLECT();
        StringID                         m_comparison;
    };
}
