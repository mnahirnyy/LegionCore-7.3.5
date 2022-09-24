#include "ScriptedEscortAI.h"
#include "CreatureTextMgr.h"
#include "MapManager.h"
#include "ScriptMgr.h"
#include "GameObjectAI.h"
#include "SpellAuraEffects.h"
#include <AI/ScriptedAI/ScriptedGossip.h>

enum eMardumQuests
{
    QUEST_INVASION_BEGIN = 40077,
    QUEST_ASHTONGUE_FORCES = 40378,
    QUEST_COILSKAR_FORCES = 40379,
    QUEST_EYE_ON_THE_PRIZE = 39049,
    QUEST_MEETING_WITH_QUEEN = 39050,
    QUEST_SHIVARRA_FORCES = 38765,
    QUEST_BEFORE_OVERRUN = 38766,
    QUEST_HIDDEN_NO_MORE = 39495,
    QUEST_ON_FELBAT_WINGS = 39663,
    QUEST_THE_KEYSTONE = 38728,
    QUEST_CRY_HAVOC = 39516,
    QUEST_THEIR_NUMBERS_ARE_LEGION = 38819
};

class sceneTrigger_dh_init : public SceneTriggerScript
{
public:
    sceneTrigger_dh_init() : SceneTriggerScript("sceneTrigger_dh_init")
    {}

    enum data
    {
        NPC_CONV = 705,
        SPELL_CUEILLIDANTH = 191667,
    };

    bool OnTrigger(Player* player, SpellScene const* /*trigger*/, std::string type) override
    {
        if (type == "CUEILLIDANTH")
        {
            //player->CastSpell(player, CUEILLIDANTH, false);
            Conversation* conversation = new Conversation;
            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), NPC_CONV, player, NULL, *player))
                delete conversation;
            player->CompleteQuest(40076);
        }
        return true;
    }
};

// 244898
class go_q40077 : public GameObjectScript
{
public:
    go_q40077() : GameObjectScript("go_q40077") { }

    struct go_q40077AI : public GameObjectAI
    {
        go_q40077AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            SCENE = 191677,
        };

        bool GossipHello(Player* player) override
        {
            /*

            ServerToClient: SMSG_PLAY_SCENE (0x2651) Length: 34 ConnIdx: 0 Time: 02/22/2016 12:57:13.116 Number: 11210
            SceneID: 1116
            PlaybackFlags: 27
            SceneInstanceID: 2
            SceneScriptPackageID: 1493
            TransportGUID: Full: 0x0
            Pos: X: 1002.885 Y: 2956.695 Z: -10.55672
            Facing: 4.76053
            */
            if (player->GetQuestStatus(40077) == QUEST_STATUS_INCOMPLETE)
            {
                player->CastSpell(player, SCENE, true);
                player->KillCreditGO(go->GetEntry(), go->GetGUID());
                return true;
            } else
                return false;
        }

    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q40077AI(go);
    }
};

//93011
class npc_q40077 : public CreatureScript
{
public:
    npc_q40077() : CreatureScript("npc_q40077") { }


    enum data
    {
        QUEST    = 40077,
        NPC_CONV = 922,
    };

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST)
        {
            /* //ToDo: how load npc for converstation?
            player->GetMap()->LoadGrid(1170.74f, 3204.71f); //voice

            Conversation* conversation = new Conversation;
            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), NPC_CONV, player, NULL, *player))
                delete conversation; */
            player->SummonCreature(93011, creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), 1.64f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
        }

        return true;
    }
    
    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q40077AI(creature);
    }

    struct npc_q40077AI : public ScriptedAI
    {
        npc_q40077AI(Creature* creature) : ScriptedAI(creature), pPlayer(nullptr)
        {
            timer = 0;
            movein = false;
        }

        Player* pPlayer;
        uint32 timer;
        bool movein;
        
        void IsSummonedBy(Unit* summoner) override
        {
            Player* player = summoner->ToPlayer();
            if (!player)
                return;
            timer = 9000;
            movein = true;
            
            me->AddPlayerInPersonnalVisibilityList(summoner->GetGUID());
            
            me->SummonCreature(98228, 1182.35f, 3202.90f, 52.60f, 2.28f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
            me->SummonCreature(98227, 1177.00f, 3203.07f, 52.44f, 1.29f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
            me->SummonCreature(99918, 1172.92f, 3207.82f, 52.47f, 5.89f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
            me->SummonCreature(98290, 1171.48f, 3203.68f, 51.39f, 0.18f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
            me->SummonCreature(98292, 1170.73f, 3204.70f, 52.62f, 0.01f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
            
            player->GetMap()->LoadGrid(1170.74f, 3204.71f); //voice

            Conversation* conversation = new Conversation;
            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), NPC_CONV, player, NULL, *player))
                delete conversation;
        }
        
        void UpdateAI(uint32 diff) override
        {
            if (movein)
            {
                if (timer <= diff)
                {
                    me->GetMotionMaster()->MovePath(10267107, false);
                    movein = false;
                    me->DespawnOrUnsummon(17000);
                } else timer -= diff;
            }
        }
    };

};

class npc_q40077_1 : public CreatureScript
{
public:
    npc_q40077_1() : CreatureScript("npc_q40077_1") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q40077_1AI(creature);
    }

    struct npc_q40077_1AI : public ScriptedAI
    {
        npc_q40077_1AI(Creature* creature) : ScriptedAI(creature)
        {
            timer = 0;
            movein = false;
        }
        
        uint32 timer;
        bool movein;
        
        void IsSummonedBy(Unit* summoner) override
        {
            timer = 9000;
            movein = true;
        }
        
        void UpdateAI(uint32 diff) override
        {
            if (movein)
            {
                if (timer <= diff)
                {
                    uint32 path = 0;
                    switch(me->GetEntry())
                    {
                        case 98228:
                            path = 10267108;
                            break;
                        case 98227:
                            path = 10267109;
                            break;
                        case 99918:
                            path = 10267110;
                            break;
                        case 98292:
                            path = 10267111;
                            break;
                        case 98290:
                            path = 10267112;
                            break;
                        default:
                            break;                        
                    }
                    me->GetMotionMaster()->MovePath(path, false);
                    me->DespawnOrUnsummon(17000);
                    movein = false;
                } else timer -= diff;
            }
        }        
    };

};

// 241751
class go_q40378 : public GameObjectScript
{
public:
    go_q40378() : GameObjectScript("go_q40378") { }

    struct go_q40378AI : public GameObjectAI
    {
        go_q40378AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            SCENE = 189261,
        };

        bool GossipHello(Player* player) override
        {
            /*
            ClientToServer: CMSG_GAME_OBJ_REPORT_USE (0x34DE) Length: 15 ConnIdx: 2 Time: 02/06/2016 22:39:25.012 Number: 16325
            GameObjectGUID: Full: 0x2C2090B920EEB5C00000100001364D15; HighType: GameObject; Low: 20335893; Map: 1481; Entry: 244439;
            */
            player->Dismount();
            player->KilledMonsterCredit(88872);
            player->CastSpell(player, SCENE, true);
            
            player->SummonCreature(99916, 1023.25f, 2849.71f, 5.42f, 1.75f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
            //    sayer->AddPlayerInPersonnalVisibilityList(player->GetGUID());
            if (Creature* mount = player->SummonCreature(101518, 1016.71f, 2849.31f, 5.48f, 1.38f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))   // 16
            {
              //  mount->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                mount->SetVisible(false);
            }
            return true;
        }
 
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q40378AI(go);
    }
};


/*
before OBJECT_FIELD_DYNAMIC_FLAGS: 4294902018
after OBJECT_FIELD_DYNAMIC_FLAGS: 2473853184

*/

//244439, 244440, 244441, 243873
class go_q39279 : public GameObjectScript
{
public:
    go_q39279() : GameObjectScript("go_q39279") { }

    struct go_q39279AI : public GameObjectAI
    {
        go_q39279AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 39279,
            GO_244439 = 244439,
            GO_244440 = 244440,
            GO_244441 = 244441,
            GO_243873 = 243873,
            NPC_CONV_GO_244439 = 558,
            NPC_CONV_GO_244440 = 583,
        };

        bool GossipUse(Player* player) override
        { 
            if (player->GetReqKillOrCastCurrentCount(QUEST, go->GetEntry()))
                return true;
            return false;
        }
        bool GossipHello(Player* player) override
        {
            if (player->GetReqKillOrCastCurrentCount(QUEST, go->GetEntry()))
                return true;

            switch (go->GetEntry())
            {
                case GO_244439:
                {
                    Conversation* conversation = new Conversation;
                    if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), NPC_CONV_GO_244439, player, NULL, *player))
                        delete conversation;
                    break;
                }
                case GO_244440:
                {
                    Conversation* conversation = new Conversation;
                    if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), NPC_CONV_GO_244440, player, NULL, *player))
                        delete conversation;
                    break;
                }
                case GO_244441:
                case GO_243873:
                    break;
            }

            return false;
        }

    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q39279AI(go);
    }
};

/*
ClientToServer: CMSG_SPELL_CLICK (0x348C) Length: 16 ConnIdx: 2 Time: 02/06/2016 22:40:30.961 Number: 17163
SpellClickUnitGUID: Full: 0x202090B9205EDD800000100002B64D15; HighType: Creature; Low: 45501717; Map: 1481; Entry: 97142;
TryAutoDismount: True

conv - 581
*/

// Destroying Fel Spreader - 191827
//class spell_legion_q39279 : public SpellScriptLoader
//{
//public:
//    spell_legion_q39279() : SpellScriptLoader("spell_legion_q39279") { }
//
//    class spell_legion_q39279_SpellScript : public SpellScript
//    {
//        PrepareSpellScript(spell_legion_q39279_SpellScript);
//
//        enum data
//        {
//            QUEST = 39279,
//            NPC_CONV = 581
//        };
//
//        void HandleScriptEffect(SpellEffIndex effIndex)
//        {
//            PreventHitDefaultEffect(EFFECT_0);
//            if (Unit* caster = GetCaster())
//            {
//                Player *player = caster->ToPlayer();
//                if (!player)
//                    return;
//
//                if (Unit * target = GetHitUnit())
//                {
//                    target->AddToHideList(caster->GetGUID());
//                    target->DestroyForPlayer(player);
//                    player->KilledMonsterCredit(target->GetEntry());
//
//                    Conversation* conversation = new Conversation;
//                    if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), NPC_CONV, player, NULL, *player))
//                        delete conversation;
//                }
//            }
//        }
//
//        void Register() override
//        {
//            OnEffectHitTarget += SpellEffectFn(spell_legion_q39279_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
//        }
//    };
//
//    SpellScript* GetSpellScript() const override
//    {
//        return new spell_legion_q39279_SpellScript();
//    }
//};

// Destroying Fel Spreader - 191827
class spell_destroying_fel_spreader : public SpellScriptLoader
{
public:
    spell_destroying_fel_spreader() : SpellScriptLoader("spell_destroying_fel_spreader") { }

    class spell_destroying_fel_spreader_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_destroying_fel_spreader_SpellScript);

        enum eSpellData
        {
            QUEST_ASSAULT_ON_MARDUM = 39279,
            NPC_FIRST_SPREADER = 97154,
            NPC_FEL_SPREADER = 97142,
            KAYN_SPREADER_CONVERSATION = 581,
            SPELL_DESTROYING = 248308,
        };

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(EFFECT_0);
            if (Unit* caster = GetCaster())
            {
                Player* player = caster->ToPlayer();

                if (!player)
                    return;

                if (Unit* target = GetHitUnit())
                {
                    if (player->GetReqKillOrCastCurrentCount(QUEST_ASSAULT_ON_MARDUM, NPC_FIRST_SPREADER) == 0
                        && player->hasQuest(QUEST_ASSAULT_ON_MARDUM)) {
                        player->KilledMonsterCredit(NPC_FIRST_SPREADER, ObjectGuid::Empty);
                        Conversation* conversation = new Conversation;
                        if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), KAYN_SPREADER_CONVERSATION, player, NULL, *player))
                            delete conversation;
                    }
                    else {
                        player->KilledMonsterCredit(target->GetEntry(), ObjectGuid::Empty);
                    }

                    player->CastSpell(target, SPELL_DESTROYING, true);
                    target->ToCreature()->DespawnOrUnsummon(1000);
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_destroying_fel_spreader_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_destroying_fel_spreader_SpellScript();
    }
};

// Destroying Stabilizer - Spell 201112
class spell_destroying_stabilizer : public SpellScriptLoader
{
public:
    spell_destroying_stabilizer() : SpellScriptLoader("spell_destroying_stabilizer") { }

    enum eData {
        FIRST_STABILIZER_DESTROYED_CREDIT = 97409,
        STABILIZER_DESTROYED_CREDIT = 101947,
        QUEST_THEIR_NUMBERS_ARE_LEGION = 38819,
        JACE_CONVERSATION = 4601,
        SPELL_DESTROYING = 248308,
    };

    class spell_destroying_stabilizer_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_destroying_stabilizer_SpellScript);

        enum eSpellData {};

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(EFFECT_0);

            if (Unit* caster = GetCaster())
            {
                Player* player = caster->ToPlayer();

                if (!player)
                    return;

                if (Unit* target = GetHitUnit())
                {
                    if (player->GetReqKillOrCastCurrentCount(QUEST_THEIR_NUMBERS_ARE_LEGION, FIRST_STABILIZER_DESTROYED_CREDIT) == 0
                        && player->GetQuestStatus(QUEST_THEIR_NUMBERS_ARE_LEGION) == QUEST_STATUS_INCOMPLETE)
                    {
                        player->KilledMonsterCredit(FIRST_STABILIZER_DESTROYED_CREDIT, ObjectGuid::Empty);
                        Conversation* conversation = new Conversation;
                        if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), JACE_CONVERSATION, player, NULL, *player))
                            delete conversation;
                    }
                    else {
                        player->KilledMonsterCredit(STABILIZER_DESTROYED_CREDIT, ObjectGuid::Empty);
                    }

                    player->CastSpell(target, SPELL_DESTROYING, true);
                    target->ToCreature()->DespawnOrUnsummon(1000);
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_destroying_stabilizer_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_destroying_stabilizer_SpellScript();
    }
};

// Destroying Soul Harvester - Spell 192252
class spell_destroying_soulharvester : public SpellScriptLoader
{
public:
    spell_destroying_soulharvester() : SpellScriptLoader("spell_destroying_soulharvester") { }

    enum eData {
        FIRST_HARVESTER_DESTROYED_CREDIT = 97383,
        HARVESTER_DESTROYED_CREDIT = 97382,
        QUEST_THEIR_NUMBERS_ARE_LEGION = 38819,
        SPELL_DESTROYING = 248308,
    };

    class spell_destroying_soulharvester_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_destroying_soulharvester_SpellScript);

        enum eSpellData {};

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(EFFECT_0);

            if (Unit* caster = GetCaster())
            {
                Player* player = caster->ToPlayer();

                if (!player)
                    return;

                if (Unit* target = GetHitUnit())
                {
                    if (player->GetReqKillOrCastCurrentCount(QUEST_THEIR_NUMBERS_ARE_LEGION, FIRST_HARVESTER_DESTROYED_CREDIT) == 0
                        && player->GetQuestStatus(QUEST_THEIR_NUMBERS_ARE_LEGION) == QUEST_STATUS_INCOMPLETE)
                    {
                        player->KilledMonsterCredit(FIRST_HARVESTER_DESTROYED_CREDIT, ObjectGuid::Empty);
                    }
                    else {
                        player->KilledMonsterCredit(HARVESTER_DESTROYED_CREDIT, ObjectGuid::Empty);
                    }

                    player->CastSpell(target, SPELL_DESTROYING, true);
                    target->ToCreature()->DespawnOrUnsummon(1000);
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_destroying_soulharvester_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_destroying_soulharvester_SpellScript();
    }
};

//94410
class npc_q40378 : public CreatureScript
{
public:
    npc_q40378() : CreatureScript("npc_q40378") { }
    
    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == 39049) 
            sCreatureTextMgr->SendChat(creature, 1, player->GetGUID());
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q40378AI(creature);
    }

    enum data
    {
        QUEST = 40378,
    };

    struct npc_q40378AI : public ScriptedAI
    {
        npc_q40378AI(Creature* creature) : ScriptedAI(creature)
        {
            timer = 1000;
        }
        
        uint32 timer;

        void Reset() override
        {
            if (Creature* target = me->FindNearestCreature(105316, 40.0f))
                me->CastSpell(target, 188437, true); // visual
        }

        void MoveInLineOfSight(Unit* who) override
        {
            Player *player = who->ToPlayer();
            if (!player || !me->IsWithinDistInMap(who, 100.0f))
                return;

            if (player->GetQuestStatus(QUEST) != QUEST_STATUS_INCOMPLETE)
                return;

            if (player->GetQuestObjectiveData(QUEST, me->GetEntry()))
                return;

            player->KilledMonsterCredit(me->GetEntry());
            sCreatureTextMgr->SendChat(me, 0, player->GetGUID());
            return;
        }
        
        void UpdateAI(uint32 diff) override
        {
            if (timer <= diff)
            {
                if (Creature* target = me->FindNearestCreature(105316, 40.0f))
                    me->CastSpell(target, 188437, true); // visual
                timer = 3000;
            } else
                timer -= diff;
                
        }
    };
};


class conversation_announcer : public CreatureScript
{
public:
    conversation_announcer() : CreatureScript("conversation_announcer") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new conversation_announcerAI(creature);
    }

    struct conversation_announcerAI : public ScriptedAI
    {
        conversation_announcerAI(Creature* creature) : ScriptedAI(creature)
        {
            SetCanSeeEvenInPassiveMode(true);
            if (me->GetEntry() == 101748)
                events.RescheduleEvent(EVENT_3, 6000);
        }

        void Reset() override
        {
            conversationEntry = 0;
            targetGUID.Clear();
            //me->SetReactState(REACT_PASSIVE);
        }

        enum events
        {
            EVENT_1 = 1,
            EVENT_2_ANNOUNCER6 = 2,
            EVENT_CLEAR = 3,
            EVENT_3 = 4,
        };

        enum npcs
        {
            NPC_ANNOUNCER_1 = 101748, //583
            NPC_ANNOUNCER_2 = 101781, //1542
            NPC_ANNOUNCER_3 = 100510, //531
        };

        uint32 conversationEntry;
        ObjectGuid targetGUID;
        GuidSet m_player_for_event;
        EventMap events;
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || who->IsOnVehicle())
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;

            if (!me->IsWithinDistInMap(who, 100.0f))
                return;

            uint32 eTimer = 1000;
            /*
            area 7742
            [1] Object Guid: Full: 0x1C2090B9200084C000109C00004B0C80; HighType: Conversation; Low: 4918400; Map: 1481; Entry: 531;
            [1] Stationary Position: X: 1561.098 Y: 2623.586 Z: 30.56277

            area 7705
            [0] Object Guid: Full: 0x1C2090B92000BAC000109C00004B0CE6; HighType: Conversation; Low: 4918502; Map: 1481; Entry: 747;
            [0] Stationary Position: X: 1429.753 Y: 2362.509 Z: 61.22379
            */
            switch (me->GetEntry())
            {
                case NPC_ANNOUNCER_1:
                    if (me->GetAreaId() == 7742) //Quest 38766
                    {
                        conversationEntry = 531;
                        events.RescheduleEvent(EVENT_1, eTimer);
                    }
                    break;
                case NPC_ANNOUNCER_2:
                    conversationEntry = 1542;
                    events.RescheduleEvent(EVENT_2, eTimer);
                    break;
                default:
                    break;
            }

            if (!conversationEntry)
                return;

            m_player_for_event.insert(who->GetGUID());
            //events.RescheduleEvent(EVENT_CLEAR, 300000);
            targetGUID = who->GetGUID();
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                    {
                        if (Player* player = sObjectAccessor->FindPlayer(targetGUID))
                        {
                            Conversation* conversation = new Conversation;
                            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), conversationEntry, player, NULL, *player))
                                delete conversation;
                        }
                        break;
                    }
                    case EVENT_2:
                    {
                        if (Player* player = sObjectAccessor->FindPlayer(targetGUID))
                        {
                            player->SendSpellScene(conversationEntry, nullptr, true, player);
                        }
                        break;
                    }

                    case EVENT_CLEAR:
                        m_player_for_event.clear();
                        break;
                    case EVENT_3:
                        DoCast(200680);
                        events.RescheduleEvent(EVENT_3, 10000);
                        break;
                }
            }
        }
    };
};

// 93105 - Inquisitor Baleful
class npc_inquisitor_baleful : public CreatureScript
{
public:
    npc_inquisitor_baleful() : CreatureScript("npc_inquisitor_baleful") { }

    enum eBaleful
    {
        SPELL_INCITE_MADNESS = 194529,
        SPELL_INFERNAL_SMASH = 192709,
        SPELL_LEGION_AEGIS = 192665,
        SPELL_LEARN_EYE_BEAM = 195447,
        SAY_AGGRO_INQUISITOR_BALEFUL = 0,
        SAY_60_HP_PCT_INQUISITOR_BALEFUL = 1,
        SAY_DEATH_INQUISITOR_BALEFUL = 2,
        NPC_POWER_QUEST_KILL_CREDIT = 105946,
        NPC_COLOSSAL_INFERNAL = 96159,
        EVENT_INCITE_MADNESS = 1,
        EVENT_INFERNAL_SMASH = 2,
        EVENT_END_AEGIS = 3,
        EVENT_EYE_SPAWNS = 4,
    };

    struct npc_inquisitor_baleful_AI : public ScriptedAI
    {
        npc_inquisitor_baleful_AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        Position const NPCsPos[2] =
        {
            { 523.404f,    2428.41f,     -117.087f,   0.108873f }, // Summmon position for Colossal Infernal
            { 586.843323f, 2433.053955f, -62.977276f, 6.143252f }  // Fly position when Legion Aegis Event (Inquisitor Baleful)
        };

        void Initialize() {
            me->SetCanFly(false);
            me->SetDisableGravity(false);

            // spawn the Colossal Infernal near Inquisitor Baleful
            if (!me->FindNearestCreature(NPC_COLOSSAL_INFERNAL, 150.0f))
            {
                if (Creature* infernal = me->SummonCreature(NPC_COLOSSAL_INFERNAL, NPCsPos[0], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 2000))
                    colossalInfernalGuid = infernal->GetGUID();
            }

            aegisDone = false;
            doingAegis = false;
            elementalCast = 1;
        }

        void Reset() override
        {
            Initialize();
            events.Reset();
        }

        Creature* GetInfernal() const
        {
            return ObjectAccessor::GetCreature(*me, colossalInfernalGuid);
        }

        void EnterCombat(Unit* victim) override
        {
            Talk(SAY_AGGRO_INQUISITOR_BALEFUL);
            events.ScheduleEvent(EVENT_INCITE_MADNESS, urand(8000, 10000));
            events.ScheduleEvent(EVENT_EYE_SPAWNS, 5000);
        }

        void JustDied(Unit* attacker) override
        {
            Talk(SAY_DEATH_INQUISITOR_BALEFUL);

            if (Creature* infernal = GetInfernal())
                infernal->Kill(infernal);

            std::list<Player*> players;
            me->GetPlayerListInGrid(players, 50.0f);

            for (Player* player : players)
            {
                if (player->GetQuestStatus(QUEST_EYE_ON_THE_PRIZE) == QUEST_STATUS_INCOMPLETE)
                {
                    player->KilledMonsterCredit(NPC_POWER_QUEST_KILL_CREDIT);
                    player->KilledMonsterCredit(NPC_COLOSSAL_INFERNAL);

                    if (!player->HasSpell(SPELL_LEARN_EYE_BEAM))
                        player->CastSpell(player, SPELL_LEARN_EYE_BEAM);
                }
            }
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType dmgType) override
        {
            if (me->HealthBelowPctDamaged(60, damage) && !aegisDone)
            {
                aegisDone = true;
                doingAegis = true;

                Talk(SAY_60_HP_PCT_INQUISITOR_BALEFUL);

                if (Unit* target = me->getVictim())
                    me->CastSpell(me, SPELL_LEGION_AEGIS, true);

                me->AttackStop();
                me->SetReactState(REACT_PASSIVE);

                me->SetCanFly(true);
                me->SetDisableGravity(true);

                me->GetMotionMaster()->MoveTakeoff(0, 586.843323f, 2433.053955f, -62.977276f);

                events.CancelEvent(EVENT_EYE_SPAWNS);
                events.ScheduleEvent(EVENT_INFERNAL_SMASH, 1000);
                events.ScheduleEvent(EVENT_END_AEGIS, 15000);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_INCITE_MADNESS:
                    if (!doingAegis) {
                        if (Unit* target = me->getVictim())
                            me->CastSpell(target, SPELL_INCITE_MADNESS);
                    }
                    events.RescheduleEvent(EVENT_INCITE_MADNESS, 15000);
                    break;
                case EVENT_INFERNAL_SMASH:
                    events.CancelEvent(EVENT_EYE_SPAWNS);
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                    {
                        if (Creature* infernal = GetInfernal())
                            infernal->CastSpell(target, SPELL_INFERNAL_SMASH);
                    }

                    elementalCast++;

                    if (elementalCast <= 3) // Only 3 cast of Infernal Smash
                        events.RescheduleEvent(EVENT_INFERNAL_SMASH, 5000);
                    break;
                case EVENT_END_AEGIS:
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->SetDisableGravity(true);

                    if (Creature* infernal = GetInfernal())
                    {
                        infernal->AttackStop();
                        infernal->SetReactState(REACT_PASSIVE);
                    }

                    doingAegis = false;
                    events.RescheduleEvent(EVENT_EYE_SPAWNS, 5000);
                    break;
                case EVENT_EYE_SPAWNS: {
                    float radius = 5.0f; // distance to caster
                    float coneAngle = 0.0f; // angle to spawn mob with AT
                    Position position;
                    me->GetNearPosition(position, radius, coneAngle);
                    me->CastSpell(position.GetPositionX(), position.GetPositionY(), position.GetPositionZ(), 195061, true);
                    coneAngle = 2.0f; // angle to spawn mob with AT
                    me->GetNearPosition(position, radius, coneAngle);
                    me->CastSpell(position.GetPositionX(), position.GetPositionY(), position.GetPositionZ(), 195061, true);
                    coneAngle = 4.0f; // angle to spawn mob with AT
                    me->GetNearPosition(position, radius, coneAngle);
                    me->CastSpell(position.GetPositionX(), position.GetPositionY(), position.GetPositionZ(), 195061, true);
                }   
                    break;
                default:
                    break;
                }
            }

            if (UpdateVictim())
                DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
        ObjectGuid colossalInfernalGuid;
        bool aegisDone;
        bool doingAegis;
        uint8 elementalCast;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_inquisitor_baleful_AI(creature);
    }
};

//242989 244916 242987 242990
class go_q38759 : public GameObjectScript
{
public:
    go_q38759() : GameObjectScript("go_q38759") { }

    struct go_q38759AI : public GameObjectAI
    {
        go_q38759AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 38759,
        };

        bool GossipHello(Player* player) override
        {
            std::map<uint32, uint32> _data;
            _data[242989] = 94400;
            _data[244916] = 94377;
            _data[242987] = 93117;
            _data[242990] = 93230;

            if (player->GetQuestObjectiveData(QUEST, _data[go->GetEntry()]))
                return true;

            if (Creature *c = player->FindNearestCreature(_data[go->GetEntry()], 10.0f))
            {
                player->KilledMonsterCredit(c->GetEntry());
                c->DestroyForPlayer(player);
                if (auto temp = player->SummonCreature(_data[go->GetEntry()], c->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 35000, 0, player->GetGUID()))
                {
                    ObjectGuid playerGuid = player->GetGUID();
                    temp->AddDelayedEvent(500, [temp, playerGuid]()-> void
                    {
                        if (auto player = ObjectAccessor::FindPlayer(playerGuid))
                            temp->AI()->Talk(1, player->GetGUID());

                        temp->AddDelayedEvent(4000, [temp]() -> void
                        {
                            switch (temp->GetEntry())
                            {
                            case 94377:
                                temp->GetMotionMaster()->MovePath(1467061408, false);
                                break;
                            case 94400:
                                temp->GetMotionMaster()->MovePath(1467061409, false);
                                break;
                            case 93230:
                                temp->GetMotionMaster()->MovePath(1467061410, false);
                                break;
                            case 93117:
                                temp->GetMotionMaster()->MovePath(1467061411, false);
                                break;
                            }
                        });
                    });
                }
            }

            return true;
        }

    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q38759AI(go);
    }
};

//99914
class npc_q40379 : public CreatureScript
{
public:
    npc_q40379() : CreatureScript("npc_q40379") { }

    enum Credit
    {
        QUEST = 40379,
        SPELL = 196724,
    };
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == 1)
        {
            player->CLOSE_GOSSIP_MENU();
            if (player->GetQuestStatus(QUEST) == QUEST_STATUS_INCOMPLETE && !player->GetReqKillOrCastCurrentCount(QUEST, creature->GetEntry()))
            {
            /*
            ClientToServer: CMSG_CLOSE_INTERACTION(0x348A) Length : 15 ConnIdx : 2 Time : 02 / 22 / 2016 13 : 10 : 10.127 Number : 17187
            Guid : Full : 0x202090B92061928000109C00004AF10C; HighType: Creature; Low: 4911372; Map: 1481; Entry: 99914;
            */
                player->CastSpell(creature, SPELL, false);
                player->KilledMonsterCredit(creature->GetEntry());
                creature->DestroyForPlayer(player);
            }
        }

        return true;
    }
};

class go_q40379 : public GameObjectScript
{
public:
    go_q40379() : GameObjectScript("go_q40379") { }

    struct go_q40379AI : public GameObjectAI
    {
        go_q40379AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST  = 40379,
            SPELL  = 190793,
            CREDIT = 94406,
            CREDIT_REQUARE = 99914,
        };

        bool GossipUse(Player* player) override
        {
            if (player->GetReqKillOrCastCurrentCount(QUEST, CREDIT) || !player->GetReqKillOrCastCurrentCount(QUEST, CREDIT_REQUARE))
                return true;
            return false;
        }

        bool GossipHello(Player* player) override
        {
            if (player->GetReqKillOrCastCurrentCount(QUEST, CREDIT) || !player->GetReqKillOrCastCurrentCount(QUEST, CREDIT_REQUARE))
                return true;

            player->KilledMonsterCredit(CREDIT);
            player->CastSpell(player, SPELL, false);
            player->SummonCreature(99917, 756.76f, 2401.41f, -60.91f, 1.06f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
            //    targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
            return false;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q40379AI(go);
    }
};

//243335
class go_q39050 : public GameObjectScript
{
public:
    go_q39050() : GameObjectScript("go_q39050") { }

    struct go_q39050AI : public GameObjectAI
    {
        go_q39050AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 39050,
            SPELL = 188539,
            CREDIT = 100722,
        };

        bool GossipHello(Player* player) override
        {
            if (player->GetQuestStatus(QUEST) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(CREDIT);
                player->CastSpell(player, SPELL, false);
            }
            return false;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q39050AI(go);
    }
};

// 99915 Sevis Brightflame (Sacrifice made for Shivarra portal)
class npc_sevis_brightflame_sacrifice : public CreatureScript
{
public:
    npc_sevis_brightflame_sacrifice() : CreatureScript("npc_sevis_brightflame_sacrifice") { }

    bool _sacrificeSevis = false;
    bool _sacrificeMe = false;

    enum eSevis {
        SPELL_SACRIFICING_SEVIS_BRIGHTFLAME = 196731,
        SPELL_SACRIFISING_MYSTIC_SOUL = 196724,
        SPELL_SEVIS_SACRIFICE_ME = 196735,
        SPELL_ANQUISHED_SOUL = 203789, // spell "Well of Souls Visual"
        CREATURE_TEXT_1 = 0, // We have a huge problem here.
        CREATURE_TEXT_2 = 1, // It has been... an honor.
        CREATURE_TEXT_3 = 2, // Your sacrifice will NOT be in vain!
        EVENT_SACRIFICE_SEVIS = 1,
        EVENT_SACRIFICE_PLAYER = 2,
        DATA_SACRIFICE_SEVIS = 10,
        DATA_SACRIFICE_PLAYER = 11,
        NPC_PORTAL_BUNNY = 24021,
        NPC_SEVIS_BRIGHTFLAME = 99915,
    };

    struct npc_sevis_brightflame_sacrifice_AI : public ScriptedAI
    {
        npc_sevis_brightflame_sacrifice_AI(Creature* creature) : ScriptedAI(creature) { }

        GuidSet m_player_for_event;

        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER) {
                me->RemoveAllAuras();
                me->CastSpell(summoner, SPELL_SACRIFISING_MYSTIC_SOUL, false);
                sCreatureTextMgr->SendChat(me, 2, summoner->GetGUID());
                me->DespawnOrUnsummon(2000);
                me->HandleEmoteCommand(16);
            }
            else
                me->DespawnOrUnsummon();
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 20.0f))
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;

            if (who->ToPlayer()->GetQuestStatus(QUEST_SHIVARRA_FORCES) != QUEST_STATUS_INCOMPLETE)
                return;

            m_player_for_event.insert(who->GetGUID());
            sCreatureTextMgr->SendChat(me, CREATURE_TEXT_1, who->GetGUID());
        }

    private:
        EventMap _events;
        ObjectGuid _playerGuid = ObjectGuid::Empty;
    };

    bool SacrificeMade(Player* player, Creature* creature) {
        if (!player->GetQuestObjectiveData(QUEST_SHIVARRA_FORCES, 0)) {
            player->KilledMonsterCredit(creature->GetEntry());
            return true;
        }
        return false;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(QUEST_SHIVARRA_FORCES) == QUEST_STATUS_INCOMPLETE && !player->GetReqKillOrCastCurrentCount(QUEST_SHIVARRA_FORCES, creature->GetEntry()))
        {
            player->ADD_GOSSIP_ITEM_DB(19016, 0, GOSSIP_SENDER_MAIN, 2);
            player->ADD_GOSSIP_ITEM_DB(19016, 1, GOSSIP_SENDER_MAIN, 1);
            player->SEND_GOSSIP_MENU(27770, creature->GetGUID());
        }
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {   
        if (!player || !creature)
            return false;

        player->PlayerTalkClass->ClearMenus();

        switch (action)
        {
        case 1:
            player->ADD_GOSSIP_ITEM_DB(19132, 0, GOSSIP_SENDER_MAIN, 3);  // sacrifice Player
            player->ADD_GOSSIP_ITEM_DB(19132, 1, GOSSIP_SENDER_MAIN, 4);
            player->SEND_GOSSIP_MENU(27999, creature->GetGUID());
            break;
        case 2:
            player->ADD_GOSSIP_ITEM_DB(19133, 0, GOSSIP_SENDER_MAIN, 5);  // sacrifice Sevis
            player->ADD_GOSSIP_ITEM_DB(19133, 1, GOSSIP_SENDER_MAIN, 4);
            player->SEND_GOSSIP_MENU(28000, creature->GetGUID());
            break;
        case 4:
            player->ADD_GOSSIP_ITEM_DB(19016, 0, GOSSIP_SENDER_MAIN, 2);
            player->ADD_GOSSIP_ITEM_DB(19016, 1, GOSSIP_SENDER_MAIN, 1);
            player->SEND_GOSSIP_MENU(27770, creature->GetGUID());
            break;
        case 3:
            // Sacrificing Player
            sCreatureTextMgr->SendChat(creature, CREATURE_TEXT_3, player->GetGUID());
            creature->CastSpell(player, SPELL_SACRIFISING_MYSTIC_SOUL, false);
            creature->Kill(player);
            // ------------
            if (Creature* bunny = creature->FindNearestCreature(NPC_PORTAL_BUNNY, creature->GetVisibilityRange(), true))
                creature->CastSpell(bunny, SPELL_ANQUISHED_SOUL, true);
            // -----------
            player->KilledMonsterCredit(creature->GetEntry());
            player->SummonCreature(NPC_SEVIS_BRIGHTFLAME, creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), creature->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
            // Talk
            player->CLOSE_GOSSIP_MENU();
            break;
        case 5:
            // Sacrificing Sevis Brightflame
            sCreatureTextMgr->SendChat(creature, CREATURE_TEXT_2, player->GetGUID());
            player->CastSpell(creature, SPELL_SACRIFICING_SEVIS_BRIGHTFLAME, false);
            // Talk
            player->KilledMonsterCredit(creature->GetEntry());
            // ----------
            if (Creature* bunny = creature->FindNearestCreature(NPC_PORTAL_BUNNY, creature->GetVisibilityRange(), true))
                player->CastSpell(bunny, SPELL_ANQUISHED_SOUL, true);
            // ---------
            creature->DestroyForPlayer(player);
            player->CLOSE_GOSSIP_MENU();
            break;
        }
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_sevis_brightflame_sacrifice_AI(creature);
    }
};

//241757
class go_q38765 : public GameObjectScript
{
public:
    go_q38765() : GameObjectScript("go_q38765") { }

    struct go_q38765AI : public GameObjectAI
    {
        go_q38765AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 38765,
            SPELL = 190851,
            CREDIT = 94407,
        };

        bool GossipHello(Player* player) override
        {
            if (player->GetReqKillOrCastCurrentCount(QUEST, CREDIT))
                return true;

            player->KilledMonsterCredit(CREDIT);
            player->CastSpell(player, SPELL, false);
            return false;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q38765AI(go);
    }
};

// 100161 - Mardum Devastator
class npc_mardum_devastator : public CreatureScript
{
public:
    npc_mardum_devastator() : CreatureScript("npc_mardum_devastator") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_mardum_devastator_AI(creature);
    }

    enum eDevastatorData {
        SPELL_DEVASTATOR_BOMBARDMENT = 191669,
        AREA_MARDUM = 7705,
    };

    struct npc_mardum_devastator_AI : public ScriptedAI
    {
        npc_mardum_devastator_AI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetReactState(REACT_PASSIVE);
            me->setActive(true);
        }

        uint32 timer = 1000;

        void UpdateAI(uint32 diff) override
        {
            if (timer <= diff)
            {
                Position pos = me->GetPosition();
                me->GetMap()->LoadGrid(pos.GetPositionX(), pos.GetPositionY());
                Map::PlayerList const& players = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    if (Player* player = itr->getSource())
                        if ((player->GetAreaId() == AREA_MARDUM || player->GetAreaId() == 7713) && me->GetDistance2d(player) <= 280.0f)
                            me->CastSpell(player, SPELL_DEVASTATOR_BOMBARDMENT, false);
                }
                timer = urand(3000, 6000);
            }
            else
                timer -= diff;
        }
    };
};

//! 96675
class npc_questgiver_93759 : public CreatureScript
{
public:
    npc_questgiver_93759() : CreatureScript("npc_questgiver_93759") {}
    enum data
    {
        QUEST = 40379,
        QUEST2 = 39050,
        QUEST_03 = 38766,
        CONVERSATION = 735
    };

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_03) // La lecon du parchemin brulant
        {
            Conversation* conversation = new Conversation;
            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), CONVERSATION, player, NULL, *player))
                delete conversation;
        }
        else if (quest->GetQuestId() == QUEST2)
        {
            sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_1, player->GetGUID());
        }

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_questgiver_93759AI(creature);
    }
    struct npc_questgiver_93759AI : public ScriptedAI
    {
        npc_questgiver_93759AI(Creature* creature) : ScriptedAI(creature)
        {
            DoCast(188485);
        }
        GuidSet m_player_for_event;
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 20.0f))
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;

            if (who->ToPlayer()->GetQuestStatus(QUEST) != QUEST_STATUS_COMPLETE)
                return;

            m_player_for_event.insert(who->GetGUID());
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, who->GetGUID());
        }
    };
};

// 93221 Doom Commander Beliash
class npc_doom_commander_beliash : public CreatureScript
{
public:
    npc_doom_commander_beliash() : CreatureScript("npc_doom_commander_beliash") { }

    enum eBeliash
    {
        EVENT_SAY_ON_COMBAT = 1,
        EVENT_HIDE_TYRANNA = 2,
        EVENT_SHADOW_RETREAT = 3,
        EVENT_SHADOW_BOLT_VALLEY = 4,
        SPELL_SHADOW_BOLT_VOLLEY = 207422,
        SPELL_SHADOW_RETREAT = 195402,
        SPELL_LEARN_CONSUME_MAGIC = 195439,
        SPELL_TAKING_POWER = 210093,
        AURA_HIDE = 188658,
        DOOM_COMMANDER_BELIASH_DIES_01 = 106003,
        SAY_ON_COMBAT = 0,
        SAY_ON_DEATH = 1,
        SAY_TYRANNA_ON_COMBAT = 0,
        NPC_TYRANNA = 95048,
    };

    struct npc_doom_commander_beliash_AI : public ScriptedAI
    {
        npc_doom_commander_beliash_AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize() {
            me->SetCorpseDelay(30);
            me->SetRespawnDelay(60);

            // spawn or uncover the Brood Queen Tyranna near Beliash
            if (Creature* Tyranna = me->FindNearestCreature(NPC_TYRANNA, 95.0f)) {
                TyrannaGuid = Tyranna->GetGUID();
                if (Tyranna->HasAura(AURA_HIDE))
                    Tyranna->RemoveAura(AURA_HIDE);
            }
        }

        void Reset() override
        {
            Initialize();
            events.Reset();
            me->RemoveAllAreaObjects();
            me->setActive(true);
        }

        void EnterEvadeMode() override
        {
            me->RemoveAllAreaObjects();
        }

        void JustDied(Unit* killer) override
        {
            // Hide the Brood Queen if it wasn't happened during the event
            if (Creature* Tyranna = ObjectAccessor::GetCreature(*me, TyrannaGuid)) {
                Tyranna->AddAura(AURA_HIDE, Tyranna);
                TyrannaGuid = ObjectGuid::Empty;
            }

            Player* player = killer->ToPlayer();
            if (!player)
                return;

            Talk(SAY_ON_DEATH, player->GetGUID());

            std::list<Player*> players;
            me->GetPlayerListInGrid(players, 50.0f);

            for (Player* player : players) {
                player->KilledMonsterCredit(DOOM_COMMANDER_BELIASH_DIES_01);
                player->CastSpell(player, SPELL_TAKING_POWER, true);
            }
        }

        void EnterCombat(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER)
                return;

            if (Creature* Tyranna = ObjectAccessor::GetCreature(*me, TyrannaGuid)) {
                if (Tyranna->AI())
                    Tyranna->AI()->Talk(SAY_TYRANNA_ON_COMBAT);
            }

            events.ScheduleEvent(EVENT_SAY_ON_COMBAT, 1750); // Say: "They will die!"
            events.ScheduleEvent(EVENT_HIDE_TYRANNA, 5000); // Hide the Brood Queen
            events.ScheduleEvent(EVENT_SHADOW_RETREAT, 2500); // Cast "Shadow Retreat"
            events.ScheduleEvent(EVENT_SHADOW_BOLT_VALLEY, urand(4000, 10000)); // Cast "Shadow Bolt Volley"
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_SAY_ON_COMBAT:
                    Talk(SAY_ON_COMBAT);
                    break;
                case EVENT_HIDE_TYRANNA:
                    if (Creature* Tyranna = ObjectAccessor::GetCreature(*me, TyrannaGuid)) {
                        Tyranna->AddAura(AURA_HIDE, Tyranna);
                        TyrannaGuid = ObjectGuid::Empty;
                    }
                    break;
                case EVENT_SHADOW_RETREAT:
                    DoCast(SPELL_SHADOW_RETREAT);
                    events.RescheduleEvent(EVENT_SHADOW_RETREAT, 10000);
                    break;
                case EVENT_SHADOW_BOLT_VALLEY:
                    DoCast(SPELL_SHADOW_BOLT_VOLLEY);
                    events.RescheduleEvent(EVENT_SHADOW_BOLT_VALLEY, 12000);
                    break;
                default:
                    break;
                }
            }

            if (UpdateVictim())
                DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
        ObjectGuid playerGuid;
        ObjectGuid TyrannaGuid;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_doom_commander_beliash_AI(creature);
    }
};

//96441 Fel Lord Caza
class npc_q39495_caza : public CreatureScript
{
public:
    npc_q39495_caza() : CreatureScript("npc_q39495_caza") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q39495_cazaAI(creature);
    }

    struct npc_q39495_cazaAI : public ScriptedAI
    {
        EventMap events;
        ObjectGuid playerGuid;

        npc_q39495_cazaAI(Creature* creature) : ScriptedAI(creature)
        {
            creature->SetCorpseDelay(30);
            creature->SetRespawnDelay(15);
        }

        enum data
        {
            QUEST = 39495,
            SPELL_AT_DEATH = 210107,
            CREDIT = 106014,
            SPELL_1 = 197002, // 7
            SPELL_2 = 196876, // 16 (+ talk)
            SPELL_3 = 197180,
            
        };

        void EnterCombat(Unit* victim) override
        {
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, victim->GetGUID());
            events.RescheduleEvent(EVENT_1, 7000);
            events.RescheduleEvent(EVENT_2, 15000);
            events.RescheduleEvent(EVENT_3, 9000);

            if (auto dh = me->FindNearestCreature(101790, 20.0f, true))
                dh->AI()->AttackStart(me);
        }

        void Reset() override
        {
            events.Reset();
        }
        
        void JustDied(Unit* killer) override
        {
            Player *player = killer->ToPlayer();
            if (!player)
                return;
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_2, player->GetGUID());
            player->KilledMonsterCredit(CREDIT);
            player->CastSpell(player, SPELL_AT_DEATH, false);
        }
        
        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_1);
                        events.RescheduleEvent(EVENT_1, 12000);
                        break;
                    case EVENT_2:
                    {
                        std::list<Creature*> targets;
                        GetCreatureListWithEntryInGrid(targets, me, 100061, 50.0f);
                        std::list<Creature*>::const_iterator itr = targets.begin();
                        if (!targets.empty())
                        {
                            std::advance(itr, urand(0, targets.size() - 1));

                            if (Creature* target = (*itr))
                                if (target->IsInWorld())
                                    me->CastSpell(target, SPELL_2);
                        }
                        Talk(1);
                        events.RescheduleEvent(EVENT_2, 15000);
                    }
                        break;   
                    case EVENT_3:
                        DoCast(SPELL_3);
                        events.RescheduleEvent(EVENT_3, 9000);
                        break;
                }                        
            }
            DoMeleeAttackIfReady();
        }
    };
};

// 243965 243968 243967
// Q: 38727
class go_q38727 : public GameObjectScript
{
public:
    go_q38727() : GameObjectScript("go_q38727") { }
    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q38727AI(go);
    }

    struct go_q38727AI : public GameObjectAI
    {
        go_q38727AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 38727,
            CREDIT = 94407,
        };

        bool GossipHello(Player* player) override
        {
            uint32 credit1 = 0;
            uint32 credit2 = 0;

            switch (go->GetEntry())
            {
                case 243965:
                    credit1 = 93762;
                    credit2 = 96692;
                    if (player->GetQuestObjectiveData(QUEST, credit1))
                        return true;
                    
                    if (Creature* t = go->FindNearestCreature(93762, 50.0f))
                    {
                        t->AddToHideList(player->GetGUID());
                        t->DestroyForPlayer(player);
                    }
                    
                    if (Creature* targ = player->SummonCreature(93762, 1800.39f, 1569.82f, 87.04f, 2.61f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                      //  targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                        targ->DespawnOrUnsummon(20000);
                    }
                    
                    if (Creature* targ = go->SummonCreature(96503, 1838.30f, 1527.21f, 87.45f, 2.20f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                      //  targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                        targ->GetMotionMaster()->MovePath(10267118, false);
                        targ->DespawnOrUnsummon(10000);
                    }
                    if (Creature* targ = go->SummonCreature(96561, 1850.74f, 1533.68f, 91.67f, 2.45f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                       // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                        targ->GetMotionMaster()->MovePoint(0, 1832.96f, 1548.18f, 88.65f);
                        targ->DespawnOrUnsummon(10000);
                    }
                    if (Creature* targ = go->SummonCreature(96503, 1856.08f, 1554.44f, 94.00f, 2.88f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                      //  targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                        targ->GetMotionMaster()->MovePath(10267119, false);
                        targ->DespawnOrUnsummon(10000);
                    }
                    if (Creature* targ = go->SummonCreature(96503, 1848.49f, 1579.76f, 89.97f, 3.55f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                       // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                        targ->GetMotionMaster()->MovePath(10267120, false);
                        targ->DespawnOrUnsummon(10000);
                    }
                    break;
                case 243968:
                {
                    credit1 = 96732;
                    credit2 = 96734;
                    if (player->GetQuestObjectiveData(QUEST, credit1))
                        return true;
                    
                    if (Creature* t = go->FindNearestCreature(96732, 50.0f))
                    {
                        t->AddToHideList(player->GetGUID());
                        t->DestroyForPlayer(player);
                    }
                    
                    if (Creature* targ = player->SummonCreature(96732, 1382.39f, 1452.39f, 37.0f, 1.22061f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                       // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                        targ->DespawnOrUnsummon(6000);
                    }
                    if (Creature* targ = player->SummonCreature(96562, 1342.57f, 1421.92f, 39.34f, 0.33f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                       // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                      //  targ->CastSpell(targ, 114943);
                        targ->GetMotionMaster()->MovePath(10267114, false);
                        targ->DespawnOrUnsummon(9000);
                    }
                    if (Creature* targ = player->SummonCreature(96562, 1350.38f, 1410.88f, 39.14f, 0.87f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                      //  targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                     //   targ->CastSpell(targ, 114943);
                        targ->GetMotionMaster()->MovePath(10267115, false);
                        targ->DespawnOrUnsummon(9000);
                    }
                    if (Creature* targ = player->SummonCreature(96562, 1354.43f, 1402.99f, 39.50f, 0.96f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                       // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                      //  targ->CastSpell(targ, 114943);
                        targ->GetMotionMaster()->MovePath(10267116, false);
                        targ->DespawnOrUnsummon(9000);
                    }
                }
                    break;
                case 243967:
                {
                    credit1 = 96731;
                    credit2 = 96733;
                    
                    if (player->GetQuestObjectiveData(QUEST, credit1))
                        return true;
                    
                    if (Creature* t = go->FindNearestCreature(96731, 100.0f))
                    {
                        t->AddToHideList(player->GetGUID());
                        t->DestroyForPlayer(player);
                    }
                    
                    if (Creature* targ = player->SummonCreature(96731, 1524.58f, 1248.48f, 70.8699f, 1.72f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                      //  targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                        targ->GetMotionMaster()->MovePath(10267117, false);
                        targ->DespawnOrUnsummon(12000);
                    }
                    if (Creature* targ = player->SummonCreature(96563, 1538.09f, 1212.63f, 71.13f, 2.07f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                       // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                        targ->DespawnOrUnsummon(12000);
                        targ->CastSpell(targ, 191537);
                    }
                    if (Creature* targ = player->SummonCreature(96564, 1524.91f, 1212.91f, 71.02f, 1.38f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                       // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                        targ->DespawnOrUnsummon(12000);
                        targ->CastSpell(targ, 191537);
                    }
                }
                    break;
            }

            player->KilledMonsterCredit(credit1);
            player->KilledMonsterCredit(credit2); //Hack. Special event.
            return false;
        }
    };
};

//103429
class npc_q40222_progres1 : public CreatureScript
{
public:
    npc_q40222_progres1() : CreatureScript("npc_q40222_progres1") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q40222_progres1AI(creature);
    }

    struct npc_q40222_progres1AI : public ScriptedAI
    {
        npc_q40222_progres1AI(Creature* creature) : ScriptedAI(creature)
        {
            SetCanSeeEvenInPassiveMode(true);
        }

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
        }


        enum data
        {
            CREDIT = 103429,
            QUEST = 40222,
        };
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 50.0f))
                return;

            if (who->ToPlayer()->GetQuestObjectiveData(QUEST, CREDIT))
                return;

            who->ToPlayer()->KilledMonsterCredit(CREDIT);
        }
    };
};
//98986 Prolifica
class npc_q39495_prolifica : public CreatureScript
{
public:
    npc_q39495_prolifica() : CreatureScript("npc_q39495_prolifica") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q39495_prolificaAI(creature);
    }

    //! Scripted_NoMovementAI HACK!
    struct npc_q39495_prolificaAI : public Scripted_NoMovementAI
    {
        EventMap events;
        ObjectGuid playerGuid;

        npc_q39495_prolificaAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            creature->SetCorpseDelay(30);
            creature->SetRespawnDelay(15);
            if(GameObject* gob = me->FindNearestGameObject(GO, 100.0f))
                gob->SetPhaseMask(1, true);  // invis
        }

        enum data
        {
            GO = 245169,
            SPELL_1 = 197217,
            SPELL_2 = 197240,
        };
        
        void Reset() override
        {
            events.Reset();
            if(GameObject* gob = me->FindNearestGameObject(GO, 100.0f))
                gob->SetPhaseMask(1, true);  // invis
        }

        void EnterCombat(Unit* victim) override
        {
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, victim->GetGUID());
            if(GameObject* gob = me->FindNearestGameObject(GO, 100.0f))
                gob->SetPhaseMask(2, true);  // invis
            events.RescheduleEvent(EVENT_1, 13000);
            events.RescheduleEvent(EVENT_2, 5000); // 20
        }

        void JustDied(Unit* killer) override
        {
            Player *pl = killer->ToPlayer();
            if (!pl)
                return;
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_2, pl->GetGUID());
            
            Conversation* conversation = new Conversation;
            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), 585, pl, NULL, *pl))
                delete conversation;
            if (Creature* tr = pl->SummonCreature(103432, 1807.34f, 1245.43f, 87.27f, 4.85f, TEMPSUMMON_MANUAL_DESPAWN, 0, pl->GetGUID(), NULL))
            {
              //  tr->AddPlayerInPersonnalVisibilityList(pl->GetGUID());
                tr->GetMotionMaster()->MovePoint(0, 1814.97f, 1211.79f, 83.19f);
            }
            if (Creature* tr = pl->SummonCreature(103432, 1805.83f, 1241.35f, 87.03f, 4.85f, TEMPSUMMON_MANUAL_DESPAWN, 0, pl->GetGUID(), NULL))
            {
              //  tr->AddPlayerInPersonnalVisibilityList(pl->GetGUID());
                tr->GetMotionMaster()->MovePoint(0, 1808.91f, 1204.31f, 82.97f);
            }
            if (Creature* tr = pl->SummonCreature(96278, 1809.17f, 1238.59f, 85.88f, 4.85f, TEMPSUMMON_MANUAL_DESPAWN, 0, pl->GetGUID(), NULL))
            {
              //  tr->AddPlayerInPersonnalVisibilityList(pl->GetGUID());
                tr->GetMotionMaster()->MovePoint(0, 1811.69f, 1195.42f, 80.83f);
            }

        }
        
        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_1);
                        events.RescheduleEvent(EVENT_1, 13000);
                        break;
                    case EVENT_2:
                        DoCast(SPELL_2);
                        events.RescheduleEvent(EVENT_2, 20000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

//93802 tyranna
class npc_q38728_tyranna : public CreatureScript
{
public:
    npc_q38728_tyranna() : CreatureScript("npc_q38728_tyranna") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q38728_tyrannaAI(creature);
    }

    struct npc_q38728_tyrannaAI : public ScriptedAI
    {
        EventMap events;
        ObjectGuid playerGuid;
        
        uint32 healthPct;
        SummonList summons;
        uint32 count;
        std::unordered_set<uint32> currentPlayers;

        npc_q38728_tyrannaAI(Creature* creature) : ScriptedAI(creature), healthPct(0), summons(me), count(0)
        {
            creature->SetCorpseDelay(30);
            creature->SetRespawnDelay(15);
        }

        enum data
        {
            SPELL_1 = 197627,  // 8
            SPELL_2 = 197414,  // phase -35%
            SPELL_QUEENS_BITE = 197486,
            NPC_ADD = 100333,
            
        };
        
        void Reset() override
        {
            healthPct = 65;
            summons.DespawnAll();
            events.Reset();
            count = 0;
            currentPlayers.clear();
        }
        
        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
            DoZoneInCombat(summon, 30.0f);
        }

        void EnterCombat(Unit* victim) override
        {
            events.RescheduleEvent(EVENT_1, 8000);
            events.RescheduleEvent(EVENT_4, 10000);
        }

        void AttackedBy(Unit* attacker) override
        {
            // for every player in combat with this boss add 800k HP
            if (attacker->IsPlayer() && currentPlayers.count(attacker->GetGUIDLow()) == 0)
            {
                currentPlayers.insert(attacker->GetGUIDLow());
                me->SetMaxHealth(me->GetMaxHealth() + 800000);
                me->SetHealth(me->GetHealth() + 800000);
            }
        }

        void JustDied(Unit* killer) override
        {
            Talk(3);

            // remove Queen's Bite Transformation if any
            me->CastSpell(me, 208121, true);

            //TODO: Threat does not properly reset for the npc demon hunters due to the faction change
        }
        
        void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType dmgType) override
        {
            if (me->HasAura(SPELL_2))
                damage = 0;

            if (me->HealthBelowPct(healthPct))
            {
                if (healthPct == 65)
                    healthPct -= 30;
                else 
                    healthPct = 0;
                
                events.RescheduleEvent(EVENT_2, 500);
                events.CancelEvent(EVENT_1);
                events.CancelEvent(EVENT_4);
            }
        }
        
        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;            

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_1);
                        events.RescheduleEvent(EVENT_1, 8000);
                        break;
                    case EVENT_2:
                        me->SetReactState(REACT_PASSIVE);
                        DoCast(SPELL_2);
                        Talk(1);
                        Talk(2);
                        count = 0;
                        events.RescheduleEvent(EVENT_3, 3000);
                        events.RescheduleEvent(EVENT_1, 27000);
                        events.RescheduleEvent(EVENT_4, 23000);
                        break;
                    case EVENT_3:
                        count++;
                        
                        for (int8 i = 0; i < 6; i++)
                            me->SummonCreature(NPC_ADD, me->GetPositionX()+irand(-10, 10), me->GetPositionY()+irand(-10, 10), me->GetPositionZ());
                        
                        if (count < 3)
                            events.RescheduleEvent(EVENT_3, 5000);
                        else
                            me->SetReactState(REACT_DEFENSIVE);
                        break;
                    case EVENT_4:
                        events.RescheduleEvent(EVENT_4, 55000);
                        Talk(0);

                        std::list<Unit*> targetList;
                        me->GetAttackableUnitListInRange(targetList, 20.0f);
                        targetList.remove_if([](WorldObject* target)
                        {
                            if (target->GetEntry() == 97244)
                                return false;
                            if (target->GetEntry() == 97959)
                                return false;
                            if (target->GetEntry() == 97962)
                                return false;
                            if (target->GetEntry() == 98712)
                                return false;

                            return true;
                        });

                        if (!targetList.empty())
                        {
                            std::vector<Unit*> targetVector(targetList.begin(), targetList.end());
                            targetVector[urand(0, targetVector.size() - 1)]->CastSpell(me, SPELL_QUEENS_BITE, true);
                        }

                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

class spell_legion_197486 : public SpellScriptLoader
{
public:
    spell_legion_197486() : SpellScriptLoader("spell_legion_197486") { }

    class spell_legion_197486_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_legion_197486_SpellScript);

        void RemoveVehicleEnter()
        {
            if (Unit* caster = GetCaster())
                if (Creature* tyranna = caster->FindNearestCreature(93802, 150.0f, true))
                    tyranna->RemoveAura(197493);
        }

        void AddVehicleEnter(SpellEffIndex /*p_EffIndex*/)
        {
            if (Unit* caster = GetCaster())
                if (Creature* tyranna = caster->FindNearestCreature(93802, 150.0f, true))
                    caster->CastSpell(tyranna, 197493, true);
        }

        void Register() override
        {
            //TODO: this is still a mess...
            //OnEffectHitTarget += SpellEffectFn(spell_legion_197486_SpellScript::AddVehicleEnter, EFFECT_0, SPELL_EFFECT_KNOCK_BACK);
            //AfterHit += SpellHitFn(spell_legion_197486_SpellScript::RemoveVehicleEnter);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_legion_197486_SpellScript();
    }
};

class spell_legion_208121 : public SpellScriptLoader
{
public:
    spell_legion_208121() : SpellScriptLoader("spell_legion_208121") { }

    class spell_legion_208121_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_legion_208121_SpellScript);

        //TODO: debug this...
        void CorrectTargetsEff0(std::list<WorldObject*>& targets)
        {
            targets.remove_if([this](WorldObject* target)
            {
                if (Creature* creature = target->ToCreature())
                    if (creature->HasAura(this->GetSpellInfo()->GetEffect(0)->TriggerSpell))
                        return false;

                return true;
            });
        }

        void CorrectTargetsEff1(std::list<WorldObject*>& targets)
        {
            targets.remove_if([this](WorldObject* target)
            {
                if (Creature* creature = target->ToCreature())
                    if (creature->HasAura(this->GetSpellInfo()->GetEffect(1)->TriggerSpell))
                        return false;

                return true;
            });
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_legion_208121_SpellScript::CorrectTargetsEff0, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_legion_208121_SpellScript::CorrectTargetsEff1, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_legion_208121_SpellScript();
    }
};

class spell_legion_197523 : public SpellScriptLoader
{
public:
    spell_legion_197523() : SpellScriptLoader("spell_legion_197523") { }

    class spell_legion_197523_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_legion_197523_SpellScript);

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            if (Unit* unit = GetCaster())
            {
                if (unit->GetEntry() == 97244 || unit->GetEntry() == 97959)
                    unit->CastSpell(unit, 197505);
                else
                    unit->CastSpell(unit, 197598);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_legion_197523_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_legion_197523_SpellScript();
    }
};

class spell_legion_197505_197598 : public SpellScriptLoader
{
public:
    spell_legion_197505_197598() : SpellScriptLoader("spell_legion_197505_197598") { }

    class spell_legion_197505_197598_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_legion_197505_197598_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                Unit* caster = GetCaster();
                if(!caster)
                    return;

                //TODO: use dbc values here instead of hardcoded
                //if (caster->GetHealthPct() < aurEff->GetBaseAmount())
                //    caster->RemoveAura(aurEff->GetBase());
                if (caster->GetHealthPct() < 50.0f)
                {
                    caster->RemoveAura(197505);
                    caster->RemoveAura(197598);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_legion_197505_197598_AuraScript::OnTick, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

    AuraScript* GetAuraScript() const override
    {
        return new spell_legion_197505_197598_AuraScript();
    }
};

//101760
class npc_q38728_progres1 : public CreatureScript
{
public:
    npc_q38728_progres1() : CreatureScript("npc_q38728_progres1") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q38728_progres1AI(creature);
    }

    struct npc_q38728_progres1AI : public ScriptedAI
    {
        npc_q38728_progres1AI(Creature* creature) : ScriptedAI(creature)
        {
            SetCanSeeEvenInPassiveMode(true);
        }

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
        }


        enum data
        {
            CREDIT = 101760,
            QUEST = 38728,
        };
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 20.0f))
                return;

            if (who->ToPlayer()->GetQuestObjectiveData(QUEST, CREDIT))
                return;

            WorldLocation loc;
            loc.m_mapId = 1481;
            loc.m_positionX = 1467.47f;
            loc.m_positionY = 1412.78f;
            loc.m_positionZ = 243.96f;
            loc.SetOrientation(who->GetOrientation());
            who->ToPlayer()->SetHomebind(loc, 7749);
            who->ToPlayer()->SendBindPointUpdate();

            who->ToPlayer()->KilledMonsterCredit(CREDIT);
        }
    };
};

//245728
class go_q38729 : public GameObjectScript
{
public:
    go_q38729() : GameObjectScript("go_q38729") { }

    struct go_q38729AI : public GameObjectAI
    {
        go_q38729AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 38729,
            CREDIT = 100651,
        };

        bool GossipHello(Player* player) override
        {
            if (player->GetReqKillOrCastCurrentCount(QUEST, CREDIT))
                return true;

            player->KilledMonsterCredit(CREDIT);
            return false;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q38729AI(go);
    }
};

// 245045
class go_q39262 : public GameObjectScript
{
public:
    go_q39262() : GameObjectScript("go_q39262") { }

    struct go_q39262AI : public GameObjectAI
    {
        go_q39262AI(GameObject* go) : GameObjectAI(go), ow(nullptr)
        {
            check = false;
            timer = 0;
        }

        bool check;
        uint32 timer;
        Player* ow;
        
        void UpdateAI(uint32 diff) override
        {
            std::list<Player*> list;
            list.clear();
            go->GetPlayerListInGrid(list, 15.0f);
            if (!list.empty())
            {
               for (std::list<Player*>::const_iterator itr = list.begin(); itr != list.end(); ++itr)
               {
                   if ((*itr)->HasAura(188501))
                       (*itr)->KilledMonsterCredit(96437);
               }
            }
            
            if (check)
                if (timer <= diff)
                {
                    check = false;
                    if (ow)
                        go->DestroyForPlayer(ow);
                    go->SetLootState(GO_JUST_DEACTIVATED);
                } else
                    timer -= diff;
        }
        
        void IsSummonedBy(Unit* summoner)
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                ow = summoner->ToPlayer();
                check = true;
                timer = 5500;
            }
            else
                go->SetLootState(GO_JUST_DEACTIVATED);
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q39262AI(go);
    }
};

// 99916
class npc_q40378_1 : public CreatureScript
{
public:
    npc_q40378_1() : CreatureScript("npc_q40378_1") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q40378_1AI(creature);
    }
    struct npc_q40378_1AI : public ScriptedAI
    {
        npc_q40378_1AI(Creature* creature) : ScriptedAI(creature) 
        {
            check = false;
        }
        
        GuidSet m_player_for_event;
        bool check;
        
        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                sCreatureTextMgr->SendChat(me, TEXT_GENERIC_1, summoner->GetGUID());
                check = true;
                me->GetMotionMaster()->MovePoint(1, 913.92f, 2848.63f, -0.69f);
                me->Mount(64385);
                me->DespawnOrUnsummon(10000);
            }
            else 
                me->DespawnOrUnsummon();
        }
        
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 10.0f))
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;

            if (who->ToPlayer()->GetQuestStatus(40378) != QUEST_STATUS_INCOMPLETE)
                return;
            
            if (check)
                return;

            m_player_for_event.insert(who->GetGUID());
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, who->GetGUID());
        }
    };
};


class npc_q40378_2 : public CreatureScript
{
public:
    npc_q40378_2() : CreatureScript("npc_q40378_2") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q40378_2AI(creature);
    }
    struct npc_q40378_2AI : public ScriptedAI
    {
        npc_q40378_2AI(Creature* creature) : ScriptedAI(creature), timer(0)
        {
            check = false;
        }

        uint32 timer;
        bool check;
        
        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                timer = 16000;
                check = true;
            }
            else 
                me->DespawnOrUnsummon();
        }
        
        void UpdateAI(uint32 diff) override
        {
            if (check)
            {
                if (timer <= diff)
                {
                    me->SetVisible(true);
                    check = false;
                } else
                    timer -= diff;
            }
        }
        
        void OnSpellClick(Unit* clicker) override
        {
            me->DespawnOrUnsummon(10000);
        }
    };
};

// 100982
class npc_q40379_1: public CreatureScript
{
public:
    npc_q40379_1() : CreatureScript("npc_q40379_1") {}

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == 40379) 
            player->SummonCreature(100982, 826.90f, 2758.64f, -30.50f, 1.62f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
              //  target->AddPlayerInPersonnalVisibilityList(player->GetGUID());
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q40379_1AI(creature);
    }
    struct npc_q40379_1AI : public ScriptedAI
    {
        npc_q40379_1AI(Creature* creature) : ScriptedAI(creature) {}
        
        
        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                sCreatureTextMgr->SendChat(me, 0, summoner->GetGUID());
                me->Mount(64385);
                me->GetMotionMaster()->MovePath(10267113, false);
                me->DespawnOrUnsummon(10000);
            }
            else 
                me->DespawnOrUnsummon();
        }
    };
};


class npc_q38759_1 : public CreatureScript
{
public:
    npc_q38759_1() : CreatureScript("npc_q38759_1") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q38759_1AI(creature);
    }
    struct npc_q38759_1AI : public ScriptedAI
    {
        npc_q38759_1AI(Creature* creature) : ScriptedAI(creature) { }
        
        GuidSet m_player_for_event;
        
        void MoveInLineOfSight(Unit* who) override
        {
            if (auto owner = me->GetAnyOwner())
                if (owner->IsPlayer())
                    return;

            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 15.0f))
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;

            if (who->ToPlayer()->GetQuestStatus(38759) != QUEST_STATUS_INCOMPLETE)
                return;
           

            m_player_for_event.insert(who->GetGUID());
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, who->GetGUID());
        }
    };
};

// 99917
class npc_q40379_2 : public CreatureScript
{
public:
    npc_q40379_2() : CreatureScript("npc_q40379_2") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q40379_2AI(creature);
    }
    struct npc_q40379_2AI : public ScriptedAI
    {
        npc_q40379_2AI(Creature* creature) : ScriptedAI(creature) { }
        
        GuidSet m_player_for_event;
        
        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                sCreatureTextMgr->SendChat(me, 2, summoner->GetGUID());
                me->Mount(64385);
                me->GetMotionMaster()->MovePoint(0, 825.57f, 2488.19f, -59.78f);
                me->DespawnOrUnsummon(10000);
            }
            else 
                me->DespawnOrUnsummon();
        }
        
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 15.0f))
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;

            if (who->ToPlayer()->GetQuestStatus(40379) != QUEST_STATUS_INCOMPLETE)
                return;
           

            m_player_for_event.insert(who->GetGUID());
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, who->GetGUID());
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_1, who->GetGUID());
        }
    };
};

class npc_96654 : public CreatureScript
{
public:
    npc_96654() : CreatureScript("npc_96654") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_96654AI(creature);
    }
    struct npc_96654AI : public ScriptedAI
    {
        npc_96654AI(Creature* creature) : ScriptedAI(creature) { }
        
        GuidSet m_player_for_event;
                
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 10.0f))
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;
            m_player_for_event.insert(who->GetGUID());
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, who->GetGUID());
        }
    };
};

// 96436
class npc_96436 : public CreatureScript
{
public:
    npc_96436() : CreatureScript("npc_96436") {}
    
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        player->CLOSE_GOSSIP_MENU();

        sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_0, player->GetGUID());
        player->KilledMonsterCredit(96436);      

        return true;
    }
    
    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == 39495)
        {
            if (Creature* Jais = player->SummonCreature(96436, creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), creature->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
            {
              //  Jais->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                Jais->CastSpell(Jais, 194326);
            }
            
            if (Creature* targ = player->SummonCreature(101788, 1267.46f, 1627.08f, 104.10f, 2.66f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
            {
               // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                targ->CastSpell(targ, 194326);
            }
            if (Creature* targ = player->SummonCreature(101787, 1262.82f, 1624.67f, 104.28f, 2.59f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
            {
               // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                targ->CastSpell(targ, 194326);
            }
            if (Creature* targ = player->SummonCreature(101790, 1256.11f, 1623.56f, 103.58f, 2.43f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
            {
              //  targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                targ->CastSpell(targ, 194326);
            }
            if (Creature* targ = player->SummonCreature(101789, 1260.16f, 1641.68f, 100.90f, 3.04f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
            {
              //  targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                targ->CastSpell(targ, 194326);
            }
            if (Creature* targ = player->SummonCreature(101787, 1262.44f, 1646.57f, 100.38f, 3.39f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
            {
               // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                targ->CastSpell(targ, 194326);
                targ->AddDelayedEvent(3800, [targ]()-> void
                {
                    targ->CastSpell(targ, 194366);
                });
            }

            player->SummonCreature(96504, 1270.82f, 1629.88f, 103.63f, 2.80f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);            
            player->SummonCreature(96503, 1260.15f, 1622.65f, 104.51f, 2.54f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);            
            player->SummonCreature(96500, 1264.43f, 1643.56f, 100.36f, 3.09f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);            
            player->SummonCreature(96500, 1262.93f, 1649.17f, 100.10f, 3.39f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);            
            player->SummonCreature(96501, 1265.69f, 1645.30f, 99.90f, 3.22f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
            
            if (GameObject* targ = player->SummonGameObject(245045, 1237.15f, 1642.62f, 103.152f, 5.80559f, 0.0f, 0.0f, 0.0f, 0.0f, DAY, player->GetGUID()))
            {
                targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                ObjectGuid playerGuid = player->GetGUID();
                targ->AddDelayedEvent(3800, [targ, playerGuid]() -> void
                {
                    targ->SetLootState(GO_READY);
                    if (auto player = ObjectAccessor::FindPlayer(playerGuid))
                        targ->UseDoorOrButton(10000, false, player);
                });
            }
            
        }

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_96436AI(creature);
    }
    struct npc_96436AI : public ScriptedAI
    {
        npc_96436AI(Creature* creature) : ScriptedAI(creature) 
        {
            check = false;
            timer = 0;
        }
        
        bool check;
        uint32 timer;
        
        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                timer = 5500;
                check = true;
            }
            else 
                me->DespawnOrUnsummon();
        }
        
        void UpdateAI(uint32 diff) override
        {
            if (check)
                if (timer <= diff)
                {
                    Talk(1);
                    me->GetMotionMaster()->MovePoint(0, 1223.62f, 1649.79f, 101.29f);
                    me->DespawnOrUnsummon(4000);
                    check = false;
                } else timer -= diff;
        }

    };
};

// 96501, 96500, 96503, 96504, 101787, 101789, 101790, 101788
class npc_q39495 : public CreatureScript
{
public:
    npc_q39495() : CreatureScript("npc_q39495") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q39495AI(creature);
    }
    struct npc_q39495AI : public ScriptedAI
    {
        npc_q39495AI(Creature* creature) : ScriptedAI(creature)
        {
            check = false;
            timer = 0;
        }
        
        bool check;
        uint32 timer;
        EventMap events;
        
        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                timer = 5500;
                check = true;
            }
            else 
                me->DespawnOrUnsummon();
        }
        
        void Reset() override
        {
            events.Reset();
        }
        void EnterCombat(Unit* who) override
        {
            if (me->GetEntry() == 96501)
            {
                events.RescheduleEvent(EVENT_1, 3000); // 200880
                events.RescheduleEvent(EVENT_2, 11000); // 197744
            }
            if (me->GetEntry() == 101787)
            {
                events.RescheduleEvent(EVENT_3, 34000); // 197641
                events.RescheduleEvent(EVENT_4, 12000); // 197639
            }
                
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType dmgType) override
        {
            damage /= 20;
        }

        void UpdateAI(uint32 diff) override
        {
            if (check)
                if (timer <= diff)
                {
                    me->GetMotionMaster()->MovePoint(0, 1223.62f, 1649.79f, 101.29f);
                    me->DespawnOrUnsummon(4000);
                    check = false;
                } else timer -= diff;
                
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;            

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(200880);
                        events.RescheduleEvent(EVENT_1, 3000);
                        break;
                    case EVENT_2:
                        DoCast(197744);
                        events.RescheduleEvent(EVENT_2, 11000);
                        break;
                    case EVENT_3:
                        DoCast(197641);
                        events.RescheduleEvent(EVENT_3, 34000); 
                        break;
                    case EVENT_4:
                        DoCast(197639);
                        events.RescheduleEvent(EVENT_4, 12000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
            
        }

    };
};

// 93762 96732 96731
class npc_q38727 : public CreatureScript
{
public:
    npc_q38727() : CreatureScript("npc_q38727") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q38727AI(creature);
    }
    struct npc_q38727AI : public ScriptedAI
    {
        npc_q38727AI(Creature* creature) : ScriptedAI(creature) 
        {
            check1 = false;
            check2 = false;
            timer = 0;
        }
        
        bool check1;
        bool check2;
        uint32 timer;
        
        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                if (me->GetEntry() == 93762)
                {
                    check1 = true;
                    timer = 9000;
                }
                if (me->GetEntry() == 96732)
                {
                    check2 = true;
                    timer = 5000;
                }
            }
            else 
                me->DespawnOrUnsummon();
        }
        
        void UpdateAI(uint32 diff) override
        {
            if (check2)
                if (timer <= diff)
                {
                    DoCast(191454);
                    check2 = false;
                } else timer -= diff;
            if (check1)
                if (timer <= diff)
                {
                    DoCast(191568);
                    DoCast(198908);
                    check1 = false;
                } else timer -= diff;
        }

    };
};

enum eIllidariTaughtSpells {
    SPELL_MANNETHREL_TAUGHT = 195022,
    SPELL_KORVAS_TAUGHT = 195021,
    SPELL_ALLARI_TAUGHT = 194996,
    SPELL_CYANA_TAUGHT = 195019,
    SPELL_KAYN_TAUGHT = 195020,
    SPELL_DH_LEARN_ABILITY_VISUAL = 184444,
};

// 93127
class npc_93127 : public CreatureScript
{
public:
    npc_93127() : CreatureScript("npc_93127") {}
    
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == 1)
        {
            player->CLOSE_GOSSIP_MENU();

            player->CastSpell(player, SPELL_KAYN_TAUGHT, true);
            creature->CastSpell(creature, SPELL_DH_LEARN_ABILITY_VISUAL, false);
            sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_3, player->GetGUID());  
        }

        return true;
    }
    
    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        switch(quest->GetQuestId()) 
        {
            case 38813:
                sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_0, player->GetGUID());  
                break;
            case 38727:
                sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_1, player->GetGUID());  
                break;
            case 39516:
                sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_2, player->GetGUID());  
                break;
            case 39663:
            {
                if (Creature* targ = player->SummonCreature(93127, creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), creature->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                {
                   // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                    sCreatureTextMgr->SendChat(targ, TEXT_GENERIC_4, player->GetGUID());  
                    targ->Mount(64385);
                    targ->GetMotionMaster()->MovePath(10267121, false);
                    targ->DespawnOrUnsummon(25000);
                } 
                if (Creature* targ = player->SummonCreature(99045, 1454.29f, 1763.44f, 54.52f, 5.56f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                {
                  //  targ->AddPlayerInPersonnalVisibilityList(player->GetGUID()); 
                    targ->Mount(64385);
                    targ->GetMotionMaster()->MovePath(10267121, false);
                    targ->DespawnOrUnsummon(25000);
                } 
                if (Creature* targ = player->SummonCreature(96420, 1453.28f, 1762.33f, 54.52f, 0.26f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                {
                  //  targ->AddPlayerInPersonnalVisibilityList(player->GetGUID()); 
                    targ->Mount(64385);
                    targ->GetMotionMaster()->MovePath(10267121, false);
                    targ->DespawnOrUnsummon(25000);
                } 
                if (Creature* targ = player->SummonCreature(96655, 1458.17f, 1763.06f, 54.52f, 3.16f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                {
                  //  targ->AddPlayerInPersonnalVisibilityList(player->GetGUID()); 
                    targ->Mount(64385);
                    targ->GetMotionMaster()->MovePath(10267121, false);
                    targ->DespawnOrUnsummon(25000);
                } 
            }
                break;
            case 39262:
            {
                Conversation* conversation = new Conversation;
                if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), 486, player, NULL, *player))
                    delete conversation;
            }
                break;
            default:
                break;
        }

        return true;
    }
};

// 96655 (Allari the Souleater),
// 99045 (Korvas Bloodthorn),
// 96420 (Cyana Nightglaive),
// 96652 (Mannethrel Darkstar)
class npc_mardum_dh_taught : public CreatureScript
{
public:
    npc_mardum_dh_taught() : CreatureScript("npc_mardum_dh_taught") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 /*action*/) override
    {
        // player->KilledMonsterCredit(creature->GetEntry());
        player->CLOSE_GOSSIP_MENU();

        switch (creature->GetEntry()) {
        case 96652: // Mannethrel
            creature->CastSpell(creature, SPELL_DH_LEARN_ABILITY_VISUAL, false);
            creature->CastSpell(creature, SPELL_MANNETHREL_TAUGHT);
            creature->AddDelayedEvent(1000, [creature, player]() -> void
            {
                sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_0, player->GetGUID());
                // creature->AI()->Talk(0);
            });
            creature->AddDelayedEvent(5000, [creature, player]() -> void
            {
                // creature->AI()->Talk(1);
                sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_1, player->GetGUID());
                if (Creature* kayn = creature->FindNearestCreature(93127, creature->GetVisibilityRange(), true)) {
                    kayn->AI()->Talk(6);
                }
            });
            creature->AddDelayedEvent(6000, [creature]() -> void
            {
                creature->Kill(creature);
            });
            break;
        case 96655: // Allari
            creature->CastSpell(creature, SPELL_DH_LEARN_ABILITY_VISUAL, false);
            creature->CastSpell(creature, SPELL_ALLARI_TAUGHT);
            creature->AddDelayedEvent(1000, [creature, player]() -> void
            {
                sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_1, player->GetGUID());
                // creature->AI()->Talk(0);
            });
            break;
        case 99045: // Korvas
            creature->CastSpell(creature, SPELL_DH_LEARN_ABILITY_VISUAL, false);
            creature->CastSpell(creature, SPELL_KORVAS_TAUGHT);
            creature->AddDelayedEvent(1000, [creature, player]() -> void
            {
                sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_1, player->GetGUID());
                // creature->AI()->Talk(1);
            });
            break;
        case 96420: // Cyana
            creature->CastSpell(creature, SPELL_DH_LEARN_ABILITY_VISUAL, false);
            creature->CastSpell(player, SPELL_CYANA_TAUGHT);
            creature->AddDelayedEvent(1000, [creature, player]() -> void
            {
                sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_1, player->GetGUID());
                // creature->AI()->Talk(1);
            });
            break;
        default:
            break;
        }

        return true;
    }
};

// 97370 - General Volroth
class npc_general_volroth : public CreatureScript
{
public:
    npc_general_volroth() : CreatureScript("npc_general_volroth") { }

    enum GeneralVolroth
    {
        ABILITY_BURN = 197749,
        ABILITY_FEL_BREATH = 197750,
        QUEST_BROOD_QUEEN_COURT_VOLROTH = 40234,
        SAY_ONCOMBAT = 0,
        SAY_ONFIRE = 1,
        SAY_ONDEATH = 2,
        EVENT_BURN = 1,
        EVENT_FEL_BREATH = 2,
        DOOM_SLAYER_CREDIT = 93716,
        CONVERSATION_STHENO = 570,
    };

    struct npc_general_volroth_AI : public ScriptedAI
    {
        npc_general_volroth_AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize() {}

        void Reset() override
        {
            Initialize();
            events.Reset();
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (who->IsPlayer()) {
                if (who->GetDistance(me) < 75.0f)
                    if (who->ToPlayer()->GetQuestStatus(QUEST_BROOD_QUEEN_COURT_VOLROTH) == QUEST_STATE_NONE) {
                        if (const Quest* quest = sObjectMgr->GetQuestTemplate(QUEST_BROOD_QUEEN_COURT_VOLROTH))
                            who->ToPlayer()->AddQuest(quest, nullptr);
                        Conversation* conversation = new Conversation;
                        Player* player = who->ToPlayer();
                        if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), CONVERSATION_STHENO, player, NULL, *player))
                            delete conversation;
                    }
            }
        }

        void JustDied(Unit* killer) override
        {
            Talk(SAY_ONDEATH);
            if (killer->GetTypeId() == TYPEID_PLAYER) {
                killer->ToPlayer()->KilledMonsterCredit(DOOM_SLAYER_CREDIT, ObjectGuid::Empty);

                if (Creature* coilscar_caller = me->FindNearestCreature(96503, 30.0f, true))
                {
                    coilscar_caller->AddDelayedEvent(2000, [coilscar_caller]() -> void
                    {
                        coilscar_caller->AI()->Talk(0);
                    });
                    coilscar_caller->AddDelayedEvent(5000, [coilscar_caller]() -> void
                    {
                        coilscar_caller->GetMotionMaster()->MovePoint(0, 1872.86f, 1497.45f, 103.66f);
                    });
                    coilscar_caller->DespawnOrUnsummon(12000);
                }
                std::list<Creature*> coilscar_harps;
                coilscar_harps = me->FindNearestCreatures(96561, 30.0f);
                for (std::list<Creature*>::const_iterator itr = coilscar_harps.begin(); itr != coilscar_harps.end(); ++itr)
                    if (Creature* coilscar = (*itr)->ToCreature()) {
                        coilscar->AddDelayedEvent(5000, [coilscar]() -> void
                        {
                            coilscar->GetMotionMaster()->MovePoint(0, 1876.74f, 1501.54f, 102.83f);
                        });
                        coilscar->DespawnOrUnsummon(12000);
                    }
                std::list<Creature*> coilscar_myrms;
                coilscar_myrms = me->FindNearestCreatures(96230, 30.0f);
                for (std::list<Creature*>::const_iterator itr = coilscar_myrms.begin(); itr != coilscar_myrms.end(); ++itr)
                    if (Creature* coilscarMyrm = (*itr)->ToCreature()) {
                        coilscarMyrm->AddDelayedEvent(5000, [coilscarMyrm]() -> void
                        {
                            coilscarMyrm->GetMotionMaster()->MovePoint(0, 1874.57f, 1499.027f, 103.504f);
                        });
                        coilscarMyrm->DespawnOrUnsummon(12000);
                    }
            }
        }

        void EnterCombat(Unit* who) override
        {
            Talk(SAY_ONCOMBAT);
            events.ScheduleEvent(EVENT_BURN, 10000);
            events.ScheduleEvent(EVENT_FEL_BREATH, 7000);
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_BURN:
                    Talk(SAY_ONFIRE);
                    DoCast(ABILITY_BURN);
                    events.ScheduleEvent(EVENT_BURN, 20000);
                    break;
                case EVENT_FEL_BREATH:
                    DoCast(ABILITY_FEL_BREATH);
                    events.ScheduleEvent(EVENT_FEL_BREATH, 17000);
                    break;
                default:
                    break;
                }
            }

            if (UpdateVictim())
                DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_general_volroth_AI(creature);
    }
};

// 97058 - Count Nefarious
class npc_count_nefarious : public CreatureScript
{
public:
    npc_count_nefarious() : CreatureScript("npc_count_nefarious") { }

    enum CountNefarious
    {
        ABILITY_CARRION_STORM = 199035,
        ABILITY_VAMPIRIC_CLEAVE = 198894,
        ABILITY_FEL_SHIELD = 199103,
        SPELL_TORRENT_OF_SOULS = 192714,
        SPELL_TORRENT_OF_SOULS_BEAM = 192715,
        QUEST_BROOD_QUEEN_COURT_NEFARIOUS = 40231,
        SAY_ONCOMBAT = 0,
        SAY_ONSWARM = 1,
        SAY_ONDEATH = 2,
        EVENT_STORM = 1,
        EVENT_CLEAVE = 2,
        EVENT_STUN = 3,
        EVENT_CANCEL_STUN = 4,
        EVENT_START_CONVERSATION = 5,
        EVENT_ONDEATH_CONVERSATION = 6,
        DATA_STUN_TARGET = 11,
        TYRANNA_CONVERSATION = 574,
        ALLARI_CONVERSATION = 576,
        DOOM_SLAYER_CREDIT = 93716,
    };

    struct npc_count_nefarious_AI : public ScriptedAI
    {
        npc_count_nefarious_AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize() {}

        void Reset() override {
            Initialize();
            events.Reset();
        }

        void JustDied(Unit* killer) override {
            Talk(SAY_ONDEATH);

            events.CancelEvent(EVENT_STORM);
            events.CancelEvent(EVENT_CLEAVE);

            events.ScheduleEvent(EVENT_ONDEATH_CONVERSATION, 2000);

            if (killer->GetTypeId() == TYPEID_PLAYER) {
                killer->ToPlayer()->KilledMonsterCredit(DOOM_SLAYER_CREDIT, ObjectGuid::Empty);
                Player* killerPlayer = killer->ToPlayer();
                killerPlayer->AddDelayedEvent(2000, [killerPlayer]() -> void
                {
                    Conversation* conversation = new Conversation;
                    if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), TYRANNA_CONVERSATION, killerPlayer, NULL, *killerPlayer))
                        delete conversation;
                });
            }
        }

        void EnterCombat(Unit* who) override {
            Talk(SAY_ONCOMBAT);

            events.ScheduleEvent(EVENT_STORM, 10000);
            events.ScheduleEvent(EVENT_CLEAVE, 6000);

            if (who->GetTypeId() == TYPEID_PLAYER) {
                Player* whoPlayer = who->ToPlayer();
                whoPlayer->ToPlayer()->AddDelayedEvent(2000, [whoPlayer]() -> void
                {
                    Conversation* conversation = new Conversation;
                    if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), ALLARI_CONVERSATION, whoPlayer, NULL, *whoPlayer))
                        delete conversation;
                });
            }
        }

        void SetData(uint32 id, uint32 value) override {
            switch (id) {
            case DATA_STUN_TARGET:
                events.ScheduleEvent(EVENT_STUN, 500);
                break;
            default:
                break;
            }
        }

        void UpdateAI(uint32 diff) override {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent()) {
                switch (eventId) {
                case EVENT_STORM:
                    Talk(SAY_ONSWARM);
                    DoCast(ABILITY_CARRION_STORM);
                    events.RescheduleEvent(EVENT_STORM, 35000);
                    break;
                case EVENT_CLEAVE:
                    DoCast(ABILITY_VAMPIRIC_CLEAVE);
                    events.RescheduleEvent(EVENT_CLEAVE, 12000);
                    break;
                case EVENT_STUN:
                    me->CastStop();
                    events.DelayEvent(EVENT_STORM, 20000);
                    events.DelayEvent(EVENT_CLEAVE, 20000);
                    DoCast(me, SPELL_TORRENT_OF_SOULS);
                    me->AddAura(SPELL_TORRENT_OF_SOULS_BEAM, me);
                    events.ScheduleEvent(EVENT_CANCEL_STUN, 10000);
                    break;
                case EVENT_CANCEL_STUN:
                    if (!me->HasAura(192714))
                        me->RemoveAurasDueToSpell(SPELL_TORRENT_OF_SOULS_BEAM);
                    else
                        events.RescheduleEvent(EVENT_CANCEL_STUN, 1000);
                    break;
                default:
                    break;
                }
            }

            if (UpdateVictim())
                DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_count_nefarious_AI(creature);
    }
};

// Well of Souls - 97704
class go_well_of_souls : public GameObjectScript
{
public:
    go_well_of_souls() : GameObjectScript("go_well_of_souls") { }

    enum WellOfSouls {
        NPC_COUNT_NEFARIOUS = 97058,
        SPELL_TORRENT_OF_SOULS = 192714,
        SPELL_TORRENT_OF_SOULS_BEAM = 192715,
        SPELL_SOUL_BEAM = 39920,
        DATA_STUN_TARGET = 11,
    };

    void OnLootStateChanged(GameObject* go, uint32 state, Unit* unit) override {
        if (state == GO_ACTIVATED && unit)
            if (Player* player = unit->ToPlayer())
                if (Creature* creature = go->FindNearestCreature(NPC_COUNT_NEFARIOUS, 50.0f)) {
                    go->CastSpell(creature, SPELL_SOUL_BEAM);

                    if (!creature->HasAura(SPELL_TORRENT_OF_SOULS)) {
                        creature->CastSpell(creature, SPELL_TORRENT_OF_SOULS);
                        creature->AddAura(SPELL_TORRENT_OF_SOULS_BEAM, creature);
                        creature->CastStop();
                        player->RemoveAurasDueToSpell(199035);
                    }  
                }
    }
};

// 97059 - King Voras
class npc_king_voras : public CreatureScript
{
public:
    npc_king_voras() : CreatureScript("npc_king_voras") { }

    enum eKingVoras
    {
        EVENT_SPIDER_EGGS = 1,
        EVENT_ACID_SPIT = 2,
        SPELL_SPIDER_EGGS = 198235,
        SPELL_ACID_SPIT = 198163,
        NPC_VORAS_CREDIT = 93716,
        SAY_ON_COMBAT = 0,
        SAY_ON_DEATH = 2,
        SAY_ON_SPYDERS = 1,
        NPC_SPIDER_EGG = 100719,
        TYRANNA_CONVERSATION = 567,
    };

    struct npc_king_voras_AI : public ScriptedAI
    {
        npc_king_voras_AI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize() {}

        void Reset() override
        {
            Initialize();
            events.Reset();
        }

        void JustDied(Unit* killer) override
        {
            Talk(SAY_ON_DEATH);

            events.CancelEvent(EVENT_ACID_SPIT);
            events.CancelEvent(EVENT_SPIDER_EGGS);

            // despawn eggs
            std::list<Creature*> summonedEggs;
            me->GetCreatureListWithEntryInGrid(summonedEggs, NPC_SPIDER_EGG, me->GetVisibilityRange());
            for (std::list<Creature*>::const_iterator itr = summonedEggs.begin(); itr != summonedEggs.end(); ++itr)
                (*itr)->ToCreature()->DespawnOrUnsummon(0);
            // killcredit + conversation
            if (killer->GetTypeId() == TYPEID_PLAYER) {
                killer->ToPlayer()->KilledMonsterCredit(NPC_VORAS_CREDIT, ObjectGuid::Empty);
                Player* killerPlayer = killer->ToPlayer();
                killerPlayer->ToPlayer()->AddDelayedEvent(4000, [killerPlayer]() -> void
                {
                    Conversation* conversation = new Conversation;
                    if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), TYRANNA_CONVERSATION, killerPlayer, NULL, *killerPlayer))
                        delete conversation;
                });
            }
        }

        void EnterCombat(Unit* who) override
        {
            Talk(SAY_ON_COMBAT);
            events.ScheduleEvent(EVENT_ACID_SPIT, 8000);
            events.ScheduleEvent(EVENT_SPIDER_EGGS, 15000);
        }

        void SummomSpiderEggs(uint32 duration)
        {
            Position const eggsPositions[] = {
                { 1186.619f, 1289.300f, 108.589f, 1.225f },
                { 1149.352f, 1239.250f, 120.737f, 3.209f },
                { 1151.949f, 1242.238f, 119.464f, 3.476f },
                { 1142.174f, 1249.346f, 119.955f, 4.552f },
                { 1137.942f, 1249.559f, 120.002f, 4.713f },
                { 1140.069f, 1252.097f, 119.464f, 4.792f },
            };
            for (uint8 i = 0; i < 6; i++)
                me->SummonCreature(NPC_SPIDER_EGG, eggsPositions[i], TEMPSUMMON_CORPSE_DESPAWN, duration);
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_ACID_SPIT:
                    DoCast(SPELL_ACID_SPIT);
                    events.RescheduleEvent(EVENT_ACID_SPIT, 16000);
                    break;
                case EVENT_SPIDER_EGGS:
                    Talk(SAY_ON_SPYDERS);
                    SummomSpiderEggs(10000);
                    events.RescheduleEvent(EVENT_SPIDER_EGGS, 29000);
                    break;
                }
            }

            if (UpdateVictim())
                DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_king_voras_AI(creature);
    }
};

// 97459
class npc_mardum_gaardoun : public CreatureScript
{
public:
    npc_mardum_gaardoun() : CreatureScript("npc_mardum_gaardoun") { }

    enum eData {
        ABILITY_BATTLE_SHOUT = 198258,
        ABILITY_BRUTAL_STRIKE = 198259,
        EVENT_SHOUT = 1,
        EVENT_STRIKE = 2,
        EVENT_REMOVE_AURA = 3,
        EVENT_FREE = 4,
        EVENT_GO_OUT = 5,
        SPELL_WEBBED = 191989,
        NPC_KING_VORAS = 97059,
    };

    struct npc_mardum_gaardoun_AI : public ScriptedAI
    {
        npc_mardum_gaardoun_AI(Creature* creature) : ScriptedAI(creature) { Initialize(); }

        void Initialize() {}

        void Reset() override {
            Initialize();
            events.Reset();
        }

        void OnSpellClick(Unit* clicker) override
        {
            events.ScheduleEvent(EVENT_REMOVE_AURA, 2000);
        }

        void EnterCombat(Unit* who) override
        {
            events.ScheduleEvent(EVENT_SHOUT, 2000);
            events.ScheduleEvent(EVENT_STRIKE, 9000);
        }

        void JustReachedHome() override {
            events.ScheduleEvent(EVENT_FREE, 2000);
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_REMOVE_AURA:
                    me->RemoveAurasDueToSpell(SPELL_WEBBED);
                    if (Creature* voras = me->FindNearestCreature(NPC_KING_VORAS, me->GetVisibilityRange(), true))
                        me->GetAI()->AttackStart(voras);
                    else
                        events.ScheduleEvent(EVENT_FREE, 2000);
                    break;
                case EVENT_FREE:
                    Talk(1);
                    events.ScheduleEvent(EVENT_GO_OUT, 3000);
                    break;
                case EVENT_GO_OUT:
                    Talk(2);
                    float x, y, z;
                    me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 50.0f);
                    me->GetMotionMaster()->MovePoint(0, x, y, z);
                    me->DespawnOrUnsummon(5000);
                    break;
                case EVENT_SHOUT:
                    DoCast(ABILITY_BATTLE_SHOUT);
                    events.RescheduleEvent(EVENT_SHOUT, 15000);
                    break;
                case EVENT_STRIKE:
                    DoCast(ABILITY_BRUTAL_STRIKE);
                    events.RescheduleEvent(EVENT_STRIKE, 18000);
                    break;
                }
            }

            if (UpdateVictim())
                DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_mardum_gaardoun_AI(creature);
    }
};


void AddSC_Mardum()
{
    new sceneTrigger_dh_init();
    new go_q40077();
    new npc_q40077();
    new npc_q40077_1();
    new go_q40378();
    new go_q39279();
    new spell_destroying_fel_spreader(); // spell_legion_q39279
    new spell_destroying_stabilizer();
    new spell_destroying_soulharvester();
    new spell_legion_197486();
    new spell_legion_197505_197598();
    new spell_legion_197523();
    new spell_legion_208121();
    new npc_q40378();
    new conversation_announcer();
    new npc_inquisitor_baleful(); // new npc_q39049();
    new go_q38759();
    new npc_q40379();
    new go_q40379();
    new go_q39050();
    new npc_sevis_brightflame_sacrifice();
    new go_q38765();
    new npc_mardum_devastator();
    new npc_questgiver_93759();
    new npc_doom_commander_beliash();
    new npc_q39495_caza();
    new go_q38727();
    new npc_q40222_progres1();
    new npc_q39495_prolifica();
    new npc_q38728_tyranna();
    new npc_q38728_progres1();
    new go_q38729();
    new go_q39262();
    new npc_q40378_1();
    new npc_q40378_2();
    new npc_q40379_1();
    new npc_q38759_1();
    new npc_q40379_2();
    new npc_96654();
    new npc_96436();
    new npc_q39495();
    new npc_q38727();
    new npc_93127();
    new npc_mardum_dh_taught();
    // new npc_c100161();
    new npc_general_volroth();
    new npc_count_nefarious();
    new go_well_of_souls();
    new npc_king_voras();
    new npc_mardum_gaardoun();
}