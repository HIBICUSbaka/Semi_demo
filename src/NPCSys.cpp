//
// Created by 蔡元涵 on 2020/8/24.
//

#include "NPCSys.h"
#include "Player.h"
#include "NPChara.h"
#include "Tools.h"
#include "Item.h"
#include "Document.h"
#include "UIObject.h"
#include "TextZone.h"

NPCSys::NPCSys(class Player *player, Json::Value npcInitInfo) : mPlayer(player),
                                                                mIsTalking(false),
                                                                mTalkIndex(-1)
{
    NPChara *npc;
    for (int i = 0; i < npcInitInfo.size(); ++i)
    {
        npc = new NPChara(player->getGameSys(), this, npcInitInfo[i]);
        player->getGameSys()->AddNPCToSys(npc);
    }
    mNPCharas = player->getGameSys()->getNPCharas();

    npcInitInfo.clear();
    npcInitInfo = GetJsonRoot("../Configs/NPCharaConf.json");
    Json::Value value = npcInitInfo["NPCItems"];
    Item *item;
    Document *document;

    for (int i = 0; i < value.size(); ++i)
    {
        item = new Item(player->getGameSys(), player, value[i]);
        mNPCItems.emplace_back(item);
    }

    value = npcInitInfo["NPCDocs"];
    for (int i = 0; i < value.size(); ++i)
    {
        document = new Document(player->getGameSys(), player, value[i]);
        mNPCDocuments.emplace_back(document);
    }

    value.clear();
    value = player->getGameSys()->GetInitObjRoot()["UIObjects"]["DialogUI"];
    mUIO = new UIObject(player->getGameSys(), value["UITexPath"].asString());
    for (int i = 0; i < value["Button"].size(); ++i)
    {
        mUIO->CreateButton(player->getGameSys(), mUIO,
                           value["Button"][i]["TexPath"].asString(),
                           value["Button"][i]["Type"].asInt(),
                           value["Button"][i]["Text"].asString(),
                           {value["Button"][i]["Position"][0].asFloat(),
                            value["Button"][i]["Position"][1].asFloat()},
                           value["Button"][i]["Function"].asInt());
    }
    for (int i = 0; i < value["TextZone"].size(); ++i)
    {
        mUIO->CreateTextZone(player->getGameSys(), mUIO,
                             {value["TextZone"][i]["Position"][0].asFloat(),
                              value["TextZone"][i]["Position"][1].asFloat()},
                             value["TextZone"][i]["Width"].asInt(),
                             value["TextZone"][i]["ID"].asInt());
    }
    mUIO->TurnOff();

    mTalkInfo = GetJsonRoot("../Configs/TalkTextConf.json");
}

NPChara NPCSys::FindChara(int id)
{
    for (int i = 0; i < mNPCharas.size(); ++i)
    {
        if (mNPCharas[i]->getNPCInfo().ID == id)
        {
            return *mNPCharas[i];
        }
    }
    NPChara *npChara = nullptr;
    return *npChara;
}

void NPCSys::SetTalk(int id)
{
    Json::Value talkText;
    for (int i = 0; i < mTalkInfo["TalkTexts"].size(); ++i)
    {
        if (mTalkInfo["TalkTexts"][i]["ID"].asInt() == id)
        {
            talkText = mTalkInfo["TalkTexts"][i]["TalkText"];
        }
    }

    if (mTalkIndex == talkText.size())
    {
        mUIO->TurnOff();
        mTalkIndex = -1;
        mIsTalking = false;
    } else
    {
        TextZone *textZone;
        textZone = mUIO->FindText(
                mPlayer->getGameSys()->GetInitObjRoot()["UIObjects"]["DialogUI"]["TextZone"][0]["ID"].asInt());
        if (textZone != nullptr)
        {
            // 设置人名
            if (talkText[mTalkIndex]["SpeakerID"].asInt() == -1)
            {
                textZone->setText("Player");
            } else
            {
                textZone->setText(FindChara(
                        talkText[mTalkIndex]["SpeakerID"].asInt()).getNPCInfo().Name);
            }
        }
        textZone = mUIO->FindText(
                mPlayer->getGameSys()->GetInitObjRoot()["UIObjects"]["DialogUI"]["TextZone"][1]["ID"].asInt());
        if (textZone != nullptr)
        {
            // 设置对话
            textZone->setText(talkText[mTalkIndex]["Text"].asString());
        }
        mTalkIndex++;
    }
}