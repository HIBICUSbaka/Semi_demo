//
// Created by 蔡元涵 on 2020/8/24.
//

#ifndef SEMI_DEMO_NPCHARA_H
#define SEMI_DEMO_NPCHARA_H

#include "Object.h"
#include "NPCSys.h"

struct NPCInfo
{
    int ID;
    std::string Name;
};

class NPChara : public Object
{
public:
    NPChara(class GameSys *gameSys, class NPCSys *npcSys, Json::Value npcInfo);

    void UpdateObject(float deltatime) override;

    void TalkWithPlayer();

    const glm::ivec2 &getPlayerLatePosition() const
    {
        return mPlayerLatePosition;
    }

    void setPlayerLatePosition(const glm::ivec2 &mPlayerLatePosition)
    {
        NPChara::mPlayerLatePosition = mPlayerLatePosition;
    }

    NPCSys *getNPCSys() const
    {
        return mNPCSys;
    }

    bool isPlayerTrigged() const
    {
        return mPlayerTrigged;
    }

    const NPCInfo &getNPCInfo() const
    {
        return mNPCInfo;
    }

private:
    NPCInfo mNPCInfo;

    bool mPlayerTrigged;

    class NPCSys *mNPCSys;

    class SpriteComponent *mTexSC;

    class CollisionComponent *mCC;

    glm::ivec2 mPlayerLatePosition;

    std::vector<class BorderDecider *> mBorderDeciders;
};


#endif //SEMI_DEMO_NPCHARA_H