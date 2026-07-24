#pragma once
#include "Animation_ToolsGraphNode_Result.h"
#include "EngineTools/NodeGraph/NodeGraph_StateMachineGraph.h"

//-------------------------------------------------------------------------

namespace EE::Animation
{
    class ResultToolsNode;

    //-------------------------------------------------------------------------

    // The result node for a state's layer settings
    class StateLayerDataToolsNode final : public ResultToolsNode
    {
        EE_REFLECT_TYPE( StateLayerDataToolsNode );

        StateLayerDataToolsNode();

        virtual char const* GetTypeName() const override { return "State Layer Data"; }
        virtual char const* GetCategory() const override { return "State Machine"; }
        virtual bool IsUserCreatable() const override { return false; }
        virtual TBitFlags<GraphType> GetAllowedParentGraphTypes() const override { return TBitFlags<GraphType>( GraphType::ValueTree ); }
        virtual int16_t Compile( GraphCompilationContext& context ) const override { EE_UNREACHABLE_CODE(); return InvalidIndex; }
    };

    //-------------------------------------------------------------------------

    class StateToolsNode final : public NodeGraph::StateNode
    {
        friend class StateMachineToolsNode;
        EE_REFLECT_TYPE( StateToolsNode );

        constexpr static float const s_minimumStateNodeUnscaledWidth = 30;
        constexpr static float const s_minimumStateNodeUnscaledHeight = 30;

    public:

        struct TimedStateEvent : public IReflectedType
        {
            EE_REFLECT_TYPE( TimedStateEvent );

            EE_REFLECT( CustomEditor = "AnimGraph_ID" );
            StringID                 m_ID;

            EE_REFLECT();
            Seconds                  m_timeValue;
        };

        // Valve keeps one list of state events with per-phase flags instead of a list per phase.
        struct StateEvent : public IReflectedType
        {
            EE_REFLECT_TYPE( StateEvent );

            EE_REFLECT( CustomEditor = "AnimGraph_ID" );
            StringID                 m_ID;

            EE_REFLECT();
            bool                     m_bIsEntry = false;

            EE_REFLECT();
            bool                     m_bIsExit = false;

            EE_REFLECT();
            bool                     m_bIsFullyInState = false;
        };

        // Valve's timed events carry their own comparison and phase instead of living in two lists.
        struct ValveTimedStateEvent : public IReflectedType
        {
            EE_REFLECT_TYPE( ValveTimedStateEvent );

            EE_REFLECT( CustomEditor = "AnimGraph_ID" );
            StringID                 m_ID;

            // Valve stores these as enums, but for compatibility loading we only need the tokens.
            EE_REFLECT();
            StringID                 m_comparisonOperator;

            EE_REFLECT();
            StringID                 m_type;

            EE_REFLECT();
            Seconds                  m_flTimeValueSeconds = 0.0f;
        };

    public:

        enum class StateType
        {
            EE_REFLECT_ENUM

            OffState,
            BlendTreeState,
            StateMachineState
        };

    public:

        StateToolsNode( DefaultInstanceCtor_t ) : NodeGraph::StateNode() {}
        StateToolsNode();
        StateToolsNode( StateType type );

        virtual bool IsRenameable() const override final { return true; }
        virtual bool RequiresUniqueName() const override final { return true; }

        inline bool IsOffState() const { return m_type == StateType::OffState; }
        inline bool IsBlendTreeState() const { return m_type == StateType::BlendTreeState; }
        inline bool IsStateMachineState() const { return m_type == StateType::StateMachineState; }

        // Return any logic or event IDs entered into this node (things like event IDs, parameter ID values, etc...)
        virtual void GetLogicAndEventIDs( TVector<StringID>& outIDs ) const;

        // Rename any logic or event IDs entered into this node (things like event IDs, parameter ID values, etc...)
        virtual void RenameLogicAndEventIDs( StringID oldID, StringID newID );

    private:

        // Valve compatibility state data, kept so a load/save cycle preserves it.
        EE_REFLECT( Hidden );
        UUID                         m_cloneSourceStateID;

        EE_REFLECT( Hidden );
        TVector<StateEvent>          m_stateEvents;

        EE_REFLECT( Hidden );
        TVector<ValveTimedStateEvent> m_timedStateEvents;

        EE_REFLECT( Hidden );
        bool                         m_bUseActualElapsedTimeInStateForTimedEvents = false;

    private:

        virtual char const* GetTypeName() const override { return "State"; }
        virtual Color GetTitleBarColor() const override;
        virtual void DrawContextMenuOptions( NodeGraph::DrawContext const& ctx, NodeGraph::UserContext* pUserContext, Float2 const& mouseCanvasPos ) override;
        virtual NodeGraph::BaseGraph* GetNavigationTarget() override;
        virtual void DrawExtraControls( NodeGraph::DrawContext const& ctx, NodeGraph::UserContext* pUserContext ) override;
        virtual bool IsActive( NodeGraph::UserContext* pUserContext ) const override;
        virtual void OnShowNode() override;

        bool CanConvertToBlendTreeState();
        void ConvertToBlendTreeState();

        bool CanConvertToStateMachineState();
        void ConvertToStateMachineState();

    private:

        EE_REFLECT( ReadOnly );
        StateType                       m_type = StateType::BlendTreeState;

        //-------------------------------------------------------------------------

        // These events are emitted in all cases (entry/execute/exit)
        EE_REFLECT( CustomEditor = "AnimGraph_ID" );
        TVector<StringID>               m_events;

        //-------------------------------------------------------------------------

        // Only emitted when entering the state
        EE_REFLECT( Category = "Phase Events", CustomEditor = "AnimGraph_ID" );
        TVector<StringID>               m_entryEvents;

        // Only emitted when fully in (no transition occurring) the state
        EE_REFLECT( Category = "Phase Events", CustomEditor = "AnimGraph_ID" );
        TVector<StringID>               m_executeEvents;

        // Only emitted when exiting the state
        EE_REFLECT( Category = "Phase Events", CustomEditor = "AnimGraph_ID" );
        TVector<StringID>               m_exitEvents;

        //-------------------------------------------------------------------------

        // Only emitted when a time remaining condition is met
        EE_REFLECT( Category = "Timed Events" );
        TVector<TimedStateEvent>        m_timeRemainingEvents;

        // Only emitted when a time elapsed condition is met
        EE_REFLECT( Category = "Timed Events" );
        TVector<TimedStateEvent>        m_timeElapsedEvents;
    };
}