#pragma once

#include "playerbot/strategy/Action.h"

namespace ai
{
    // Occasionally has a bot start small talk (a question/joke/comment) in its party/raid,
    // instead of only reacting to messages from others. Only ever considered when a real
    // player is present in the group (see PlayerbotAI::GroupHasRealPlayer) and is fully gated
    // behind AiPlayerbot.LLMPartyBotToBotChatEnabled.
    class AiChatInitiateAction : public Action
    {
    public:
        AiChatInitiateAction(PlayerbotAI* ai) : Action(ai, "ai chat initiate") {}
        virtual bool Execute(Event& event) override;
        virtual bool isUseful() override;
        virtual bool isUsefulWhenStunned() override { return false; }

#ifdef GenerateBotHelp
        virtual std::string GetHelpName() { return "ai chat initiate"; }
        virtual std::string GetHelpDescription() {
            return "Has the bot start small talk in its party/raid using the LLM system, when a real player is present in the group.";
        }
#endif
    };
}
