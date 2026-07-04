#include "playerbot/playerbot.h"
#include "AiChatInitiateAction.h"
#include "SayAction.h"
#include "playerbot/PlayerbotAIConfig.h"
#include "playerbot/ServerFacade.h"

using namespace ai;

bool AiChatInitiateAction::isUseful()
{
    if (!sPlayerbotAIConfig.llmPartyBotToBotChatEnabled)
        return false;

    if (!sPlayerbotAIConfig.llmPartyBotToBotInitiateChance)
        return false;

    Player* bot = ai->GetBot();

    if (sServerFacade.IsInCombat(bot))
        return false;

    Group* group = bot->GetGroup();
    if (!group)
        return false;

    if (!ai->GroupHasRealPlayer())
        return false;

    // Don't start talking over an ongoing exchange.
    if (PlayerbotAI::IsGroupChatOnCooldown(group->GetId()))
        return false;

    AiObjectContext* context = ai->GetAiObjectContext();
    time_t lastInitiated = AI_VALUE2(time_t, "last said", "ai chat initiate");
    if (time(0) < lastInitiated)
        return false;

    return (uint32)urand(1, 100) <= sPlayerbotAIConfig.llmPartyBotToBotInitiateChance;
}

bool AiChatInitiateAction::Execute(Event& event)
{
    Player* bot = ai->GetBot();

    AiObjectContext* context = ai->GetAiObjectContext();
    context->GetValue<time_t>("last said", "ai chat initiate")->Set(
        time(0) + urand(sPlayerbotAIConfig.llmPartyBotToBotInitiateCooldownMin, sPlayerbotAIConfig.llmPartyBotToBotInitiateCooldownMax));

    if (Group* group = bot->GetGroup())
        PlayerbotAI::PauseGroupChat(group->GetId(), time(0) + urand(sPlayerbotAIConfig.llmPartyBotToBotDelayMin, sPlayerbotAIConfig.llmPartyBotToBotDelayMax));

    return ChatReplyAction::InitiateGroupChat(bot);
}
