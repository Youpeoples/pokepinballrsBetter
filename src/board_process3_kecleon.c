#include "global.h"
#include "m4a.h"
#include "main.h"
#include "constants/bg_music.h"

#define BONUS_KECLEON_COMPLETE_POINTS 30000000

extern struct SpriteGroup gMain_spriteGroups_9;
extern struct SpriteGroup gMain_spriteGroups_31;

extern void UpdateKecleonEntityLogic(void);
extern void RenderKecleonSprites(void);
extern void UpdateKecleonScopeItem(void);
extern void UpdateKecleonScopeVision(void);
extern void RenderKecleonBoardElements(void);
extern void SortKecleonSpritesByY(void);

extern const struct SpriteSet *gKecleonSpriteSets[];

struct KecleonMoveNode
{
    u8 adjacentStates[4];
    u16 velocity[2];
};
struct KecleonSpriteSortEntry
{
    const struct SpriteSet *spriteSet;
    u8 ySortKey;
    u8 spriteIndex;
};

extern const u8 gKecleonBonusClear_Gfx[];
extern const u16 gKecleonBerryOverlayTilemap[];
extern const u8 gKecleonStageKecleon_Gfx[][0x280];
extern const u8 gKecleonStageKecleonFx_Gfx[][0x100];
extern struct SongHeader se_unk_fc;
extern struct SongHeader se_unk_fd;
extern struct SongHeader se_unk_fe;
extern struct SongHeader se_unk_ff;
extern struct SongHeader se_unk_100;
extern struct SongHeader se_unk_101;
extern struct SongHeader se_unk_102;
extern struct SongHeader se_unk_103;
extern const u16 gKecleonAnimFramesetTable[][3];
extern const s16 gKecleonOverlayTileAnimIndices[];
extern const u16 gKecleonVisibleWalkDirectionMap[];
extern const u16 gKecleonInvisibleDashDirectionMap[];
extern const struct KecleonMoveNode gKecleonMovementTargetGraph[];
extern const struct Vector16 gKecleonScopeFallTargets[];
extern const u16 *gKecleonOverlayTilemapPointers[];
extern const s16 gKecleonBerryFlashAnimIndices[];
extern const struct KecleonSpriteSortEntry gKecleonSpriteYSortData[];
extern const u16 gKecleonLowerBodyOamData[126][2][3];
extern const u16 gKecleonUpperBodyOamData[126][2][3];
extern const struct SpriteSet *gKecleonDefaultSpriteSets[];

void KecleonBoardProcess_3A_35860(void)
{
    s16 i;

    gCurrentPinballGame->stageTimer = 0;
    gCurrentPinballGame->boardSubState = 0;
    gCurrentPinballGame->boardState = 0;
    gCurrentPinballGame->boardModeType = 1;
    gCurrentPinballGame->eventTimer = gCurrentPinballGame->timerBonus + 120 * 60; // 120 seconds
    gCurrentPinballGame->timerBonus = 0;
    gCurrentPinballGame->gravityFrozen = 0;
    gCurrentPinballGame->ballRespawnState = 3;
    gCurrentPinballGame->ballRespawnTimer = 0;
    gCurrentPinballGame->ball->ballHidden = 1;
    gCurrentPinballGame->returnToMainBoardFlag = 0;
    gCurrentPinballGame->kecleonTargetVisible = 1;
    gCurrentPinballGame->bossEntityState = 0;
    gCurrentPinballGame->bossPositionX = 750;
    gCurrentPinballGame->bossPositionY = 360;
    gCurrentPinballGame->bossVulnerable = 3;
    gCurrentPinballGame->bonusModeHitCount = 0;
    gCurrentPinballGame->boardEntityCollisionMode = 0;
    gCurrentPinballGame->bannerSlideYOffset = 0;
    gCurrentPinballGame->bossColorState = 0;
    gCurrentPinballGame->bossSubEntityState = 0;
    gCurrentPinballGame->bossFramesetIndex = 0;
    gCurrentPinballGame->bossFrameTimer = 0;
    gCurrentPinballGame->kecleonTargetState = 0;
    gCurrentPinballGame->kecleonTargetFrameIndex = 0;
    gCurrentPinballGame->kecleonTargetColorMatch = 0;
    gCurrentPinballGame->kecleonCollisionEnabled = 0;
    gCurrentPinballGame->kecleonTargetX = 0;
    gCurrentPinballGame->kecleonTargetY = 0;
    gCurrentPinballGame->kecleonHitState = 0;
    gCurrentPinballGame->kecleonHitFrameIndex = 0;
    gCurrentPinballGame->kecleonTargetActive = 0;
    gCurrentPinballGame->kecleonAnimTimer = 0;
    gCurrentPinballGame->kecleonCollisionX = 0;
    gCurrentPinballGame->kecleonCollisionY = 0;
    gCurrentPinballGame->kecleonFrameTimer = 0;
    gCurrentPinballGame->kecleonHitboxX = 0;
    gCurrentPinballGame->kecleonHitboxY = 0;
    for (i = 0; i < 4; i++)
        gCurrentPinballGame->kecleonBerryLargeFlashTimer[i] = 0;

    for (i = 0; i < 10; i++)
        gCurrentPinballGame->kecleonBerrySmallFlashTimer[i] = 0;

    for (i = 0; i < 7; i++)
        gCurrentPinballGame->kecleonHitSparkTimer[i] = 0;

    gCurrentPinballGame->flippersDisabled = 0;
    gCurrentPinballGame->kecleonCollisionPos.y = gCurrentPinballGame->bossPositionY / 10 + 58;
    SortKecleonSpritesByY();
    gCurrentPinballGame->boardEntityCollisionMode = 1;
    UpdateKecleonEntityLogic();
    RenderKecleonSprites();
    m4aSongNumStart(MUS_BONUS_FIELD_KECLEON);
    UpdateKecleonScopeItem();
    UpdateKecleonScopeVision();
    RenderKecleonBoardElements();
    DmaCopy16(3, gBonusStageObjPal, (void *)0x05000320, 0x20);
}

void KecleonBoardProcess_3B_35AA4(void)
{
    SortKecleonSpritesByY();
    switch (gCurrentPinballGame->boardState)
    {
    case 0:
        gCurrentPinballGame->ballUpgradeTimerFrozen = 1;
        if (gCurrentPinballGame->stageTimer < 120)
        {
            gCurrentPinballGame->cameraYAdjust = (gCurrentPinballGame->stageTimer / 5) + 0xFFE8;
            gCurrentPinballGame->stageTimer++;
        }
        else
        {
            gCurrentPinballGame->cameraYAdjust = 0;
            gCurrentPinballGame->boardState = 1;
            gCurrentPinballGame->stageTimer = 0;
        }

        if (gCurrentPinballGame->returnToMainBoardFlag == 0)
        {
            gMain.blendControl = 0x1C10;
            gMain.blendAlpha = BLDALPHA_BLEND(16, 0);
        }
        break;
    case 1:
        if (gCurrentPinballGame->returnToMainBoardFlag == 0)
        {
            if (gCurrentPinballGame->kecleonFrameTimer == 0)
            {
                u16 var0 = 16 - gCurrentPinballGame->kecleonTargetX;
                u16 var1 = gCurrentPinballGame->kecleonTargetX;
                gMain.blendControl = 0x1C10;
                gMain.blendAlpha = BLDALPHA_BLEND(var0, var1);
            }
            else
            {
                gMain.blendControl = 0x1C42;
                gMain.blendAlpha = BLDALPHA_BLEND(7, 9);
            }
        }
        break;
    case 2:
        if (gCurrentPinballGame->stageTimer < 10)
        {
            gCurrentPinballGame->stageTimer++;
        }
        else
        {
            gCurrentPinballGame->boardState = 3;
            gCurrentPinballGame->stageTimer = 0;
            gMain.spriteGroups[6].available = 1;
            gMain.spriteGroups[5].available = 1;
            DmaCopy16(3, gKecleonBonusClear_Gfx, (void *)0x6015800, 0x2000);
            gCurrentPinballGame->bannerSlideYOffset = 0x88;
            gMain.modeChangeFlags = MODE_CHANGE_BONUS_BANNER;
        }
        break;
    case 3:
        ProcessBonusBannerAndScoring();
        if (gCurrentPinballGame->scoreCounterAnimationEnabled)
            gCurrentPinballGame->stageTimer = 181;

        if (gCurrentPinballGame->stageTimer == 180)
        {
            gCurrentPinballGame->scoreCounterAnimationEnabled = TRUE;
            gCurrentPinballGame->scoreAddStepSize = 400000;
            gCurrentPinballGame->scoreAddedInFrame = BONUS_KECLEON_COMPLETE_POINTS;
        }

        if (gCurrentPinballGame->stageTimer < 240)
        {
            if (gCurrentPinballGame->stageTimer == 20)
            {
                m4aMPlayAllStop();
                m4aSongNumStart(MUS_SUCCESS3);
            }

            gCurrentPinballGame->stageTimer++;
        }
        else
        {
            gCurrentPinballGame->stageTimer = 0;
            gCurrentPinballGame->boardState = 4;
            gCurrentPinballGame->numCompletedBonusStages++;
        }
        break;
    case 4:
        ProcessBonusBannerAndScoring();
        gCurrentPinballGame->returnToMainBoardFlag = 1;
        break;
    }

    UpdateKecleonEntityLogic();
    RenderKecleonSprites();
    UpdateKecleonScopeItem();
    UpdateKecleonScopeVision();
    if (gCurrentPinballGame->boardModeType && gCurrentPinballGame->eventTimer < 2 && gMain.modeChangeFlags == MODE_CHANGE_NONE)
    {
        m4aMPlayAllStop();
        m4aSongNumStart(MUS_END_OF_BALL3);
        gCurrentPinballGame->kecleonHitState = 0;
        gCurrentPinballGame->kecleonAnimTimer = 0;
        gCurrentPinballGame->kecleonTargetActive = 0;
        gMain.modeChangeFlags |= MODE_CHANGE_EXPIRED_BONUS;
    }

    if (gCurrentPinballGame->returnToMainBoardFlag)
        FadeToMainBoard();

    BonusStage_HandleModeChangeFlags();
    RenderKecleonBoardElements();
}

void UpdateKecleonEntityLogic(void)
{
    const u16 VECTORSCALEDOWN = 20000;
    int deltaX;
    int deltaY;
    struct Vector32 tempVec;
    struct Vector32 deltaVec;
    u16 angle;
    int xx, yy;
    int deltaMagSquared;

    switch (gCurrentPinballGame->bossEntityState)
    {
    case 0:
        gCurrentPinballGame->bossEntityState = 1;
        gCurrentPinballGame->bossFramesetIndex = 3;
        gCurrentPinballGame->bossFrameTimer = 0;
        gCurrentPinballGame->bossPositionX = 750;
        gCurrentPinballGame->bossPositionY = 360;
        gCurrentPinballGame->kecleonTargetState = gKecleonMovementTargetGraph[10].adjacentStates[(Random() * gMain.systemFrameCount) & 0x3];
        break;
    case 1:
        if (gKecleonAnimFramesetTable[gCurrentPinballGame->bossFramesetIndex][1] > gCurrentPinballGame->bossFrameTimer)
        {
            gCurrentPinballGame->bossFrameTimer++;
        }
        else
        {
            gCurrentPinballGame->bossFrameTimer = 0;
            if (++gCurrentPinballGame->bossFramesetIndex == 9)
                gCurrentPinballGame->bossFramesetIndex = 3;
        }

        if (gCurrentPinballGame->bossPositionX > 0)
        {
            gCurrentPinballGame->bossPositionX -= 6;
        }
        else
        {
            gCurrentPinballGame->bossEntityState = 2;
            gCurrentPinballGame->bossFramesetIndex = 59;
            gCurrentPinballGame->bossFrameTimer = 0;
        }

        if (gCurrentPinballGame->ballRespawnTimer > 2)
            gCurrentPinballGame->ballRespawnTimer--;
        break;
    case 2:
        if (gKecleonAnimFramesetTable[gCurrentPinballGame->bossFramesetIndex][1] > gCurrentPinballGame->bossFrameTimer)
        {
            gCurrentPinballGame->bossFrameTimer++;
        }
        else
        {
            gCurrentPinballGame->bossFrameTimer = 0;
            if (++gCurrentPinballGame->bossFramesetIndex == 66)
            {
                gCurrentPinballGame->bossFramesetIndex = 65;
                if (gCurrentPinballGame->kecleonFrameTimer < 8)
                {
                    gCurrentPinballGame->bossEntityState = 3;
                }
                else
                {
                    int rand, mask;

                    if (gCurrentPinballGame->kecleonTargetFrameIndex)
                    {
                        gCurrentPinballGame->bossFrameTimer = 20;
                    }
                    else
                    {
                        gCurrentPinballGame->bossFrameTimer = 0;
                        gCurrentPinballGame->kecleonTargetFrameIndex = 1;
                    }

                    gCurrentPinballGame->bossEntityState = 5;

                    rand = Random() + gMain.systemFrameCount;
                    mask = 0x3;
                    gCurrentPinballGame->kecleonTargetState = gKecleonMovementTargetGraph[gCurrentPinballGame->kecleonTargetState].adjacentStates[rand & mask];
                }
            }

            if (gCurrentPinballGame->bossFramesetIndex == 60 || gCurrentPinballGame->bossFramesetIndex == 61)
                MPlayStart(&gMPlayInfo_SE1, &se_unk_fc);
        }

        if (gCurrentPinballGame->bossFramesetIndex == 64)
        {
            gCurrentPinballGame->kecleonTargetX = gCurrentPinballGame->bossFrameTimer / 2;
            if (gCurrentPinballGame->bossFrameTimer == 4)
                MPlayStart(&gMPlayInfo_SE1, &se_unk_fd);
        }
        break;
    case 3:
        gCurrentPinballGame->bossVelocityX = gKecleonMovementTargetGraph[gCurrentPinballGame->kecleonTargetState].velocity[0];
        gCurrentPinballGame->bossVelocityY = gKecleonMovementTargetGraph[gCurrentPinballGame->kecleonTargetState].velocity[1];
        deltaVec.x = gCurrentPinballGame->bossVelocityX - gCurrentPinballGame->bossPositionX;
        deltaVec.y = gCurrentPinballGame->bossVelocityY - gCurrentPinballGame->bossPositionY;
        angle = ArcTan2(deltaVec.x, -deltaVec.y);
        gCurrentPinballGame->bossColorState = gKecleonVisibleWalkDirectionMap[angle / 0x2000];
        gCurrentPinballGame->bossFramesetIndex = gCurrentPinballGame->bossColorState;
        gCurrentPinballGame->bossFrameTimer = 0;
        gCurrentPinballGame->bossEntityState = 4;
        break;
    case 4:
        deltaVec.x = gCurrentPinballGame->bossVelocityX - gCurrentPinballGame->bossPositionX;
        deltaVec.y = gCurrentPinballGame->bossVelocityY - gCurrentPinballGame->bossPositionY;
        xx = deltaVec.x * deltaVec.x;
        yy = deltaVec.y * deltaVec.y;
        deltaMagSquared = xx + yy;
        angle = ArcTan2(deltaVec.x, -deltaVec.y);
        tempVec.x = 6 * Cos(angle) / VECTORSCALEDOWN;
        tempVec.y = -6 * Sin(angle) / VECTORSCALEDOWN;
        gCurrentPinballGame->bossPositionX += tempVec.x;
        gCurrentPinballGame->bossPositionY += tempVec.y;
        if (deltaMagSquared < 2500)
        {
            gCurrentPinballGame->bossEntityState = 8;
            if (gCurrentPinballGame->bossColorState == 3)
            {
                gCurrentPinballGame->bossColorState = 66;
                gCurrentPinballGame->bossFramesetIndex = 66;
            }
            else
            {
                gCurrentPinballGame->bossColorState = 69;
                gCurrentPinballGame->bossFramesetIndex = 69;
            }

            gCurrentPinballGame->bossFrameTimer = 0;
        }

        if (gKecleonAnimFramesetTable[gCurrentPinballGame->bossFramesetIndex][1] > gCurrentPinballGame->bossFrameTimer)
        {
            gCurrentPinballGame->bossFrameTimer++;
        }
        else
        {
            gCurrentPinballGame->bossFrameTimer = 0;
            gCurrentPinballGame->bossFramesetIndex++;
            if (gCurrentPinballGame->bossFramesetIndex == gCurrentPinballGame->bossColorState + 6)
                gCurrentPinballGame->bossFramesetIndex = gCurrentPinballGame->bossColorState;
        }
        break;
    case 5:
        if (gCurrentPinballGame->bossFrameTimer < 18)
        {
            if (gCurrentPinballGame->bossFrameTimer == 0)
                MPlayStart(&gMPlayInfo_SE1, &se_unk_fe);

            gCurrentPinballGame->bossFrameTimer++;
            if (gCurrentPinballGame->bossColorState == 66)
                gCurrentPinballGame->bossFramesetIndex = 37;
            else
                gCurrentPinballGame->bossFramesetIndex = 48;
        }
        else
        {
            gCurrentPinballGame->bossVelocityX = gKecleonMovementTargetGraph[gCurrentPinballGame->kecleonTargetState].velocity[0];
            gCurrentPinballGame->bossVelocityY = gKecleonMovementTargetGraph[gCurrentPinballGame->kecleonTargetState].velocity[1];
            deltaVec.x = gCurrentPinballGame->bossVelocityX - gCurrentPinballGame->bossPositionX;
            deltaVec.y = gCurrentPinballGame->bossVelocityY - gCurrentPinballGame->bossPositionY;
            angle = ArcTan2(deltaVec.x, -deltaVec.y);
            gCurrentPinballGame->bossColorState = gKecleonInvisibleDashDirectionMap[angle / 0x2000];
            gCurrentPinballGame->bossFramesetIndex = gCurrentPinballGame->bossColorState;
            gCurrentPinballGame->bossEntityState = 6;
        }
        break;
    case 6:
        deltaVec.x = gCurrentPinballGame->bossVelocityX - gCurrentPinballGame->bossPositionX;
        deltaVec.y = gCurrentPinballGame->bossVelocityY - gCurrentPinballGame->bossPositionY;
        xx = deltaVec.x * deltaVec.x;
        yy = deltaVec.y * deltaVec.y;
        deltaMagSquared = xx + yy;
        angle = ArcTan2(deltaVec.x, -deltaVec.y);
        tempVec.x = 10 * Cos(angle) / VECTORSCALEDOWN;
        tempVec.y = -10 * Sin(angle) / VECTORSCALEDOWN;
        gCurrentPinballGame->bossPositionX += tempVec.x;
        gCurrentPinballGame->bossPositionY += tempVec.y;
        if (deltaMagSquared < 2500)
        {
            if (gCurrentPinballGame->bonusModeHitCount > 9)
            {
                if (gCurrentPinballGame->bossColorState == 66)
                {
                    gCurrentPinballGame->bossColorState = 75;
                    gCurrentPinballGame->bossPositionX += 240;
                }
                else
                {
                    gCurrentPinballGame->bossColorState = 95;
                    gCurrentPinballGame->bossPositionX -= 240;
                }

                gCurrentPinballGame->bossEntityState = 14;
                gCurrentPinballGame->bossFramesetIndex = gCurrentPinballGame->bossColorState;
            }
            else
            {
                if (gCurrentPinballGame->kecleonTargetX < 16)
                {
                    gCurrentPinballGame->bossEntityState = 2;
                    gCurrentPinballGame->bossFramesetIndex = 62;
                    gCurrentPinballGame->bossFrameTimer = 0;
                }
                else
                {
                    gCurrentPinballGame->bossEntityState = 7;
                    if (gCurrentPinballGame->bossColorState == 38)
                    {
                        gCurrentPinballGame->bossColorState = 42;
                        gCurrentPinballGame->bossFramesetIndex = 42;
                    }
                    else
                    {
                        gCurrentPinballGame->bossColorState = 53;
                        gCurrentPinballGame->bossFramesetIndex = 53;
                    }

                    gCurrentPinballGame->bossFrameTimer = 0;
                }
            }
        }

        if (gKecleonAnimFramesetTable[gCurrentPinballGame->bossFramesetIndex][1] > gCurrentPinballGame->bossFrameTimer)
        {
            gCurrentPinballGame->bossFrameTimer++;
        }
        else
        {
            gCurrentPinballGame->bossFrameTimer = 0;
            gCurrentPinballGame->bossFramesetIndex++;
            if (gCurrentPinballGame->bossFramesetIndex == gCurrentPinballGame->bossColorState + 4)
                gCurrentPinballGame->bossFramesetIndex = gCurrentPinballGame->bossColorState;

            if (gCurrentPinballGame->bossFramesetIndex == gCurrentPinballGame->bossColorState + 1)
                MPlayStart(&gMPlayInfo_SE1, &se_unk_ff);
            if (gCurrentPinballGame->bossFramesetIndex == gCurrentPinballGame->bossColorState + 3)
                MPlayStart(&gMPlayInfo_SE1, &se_unk_ff);
        }
        break;
    case 7:
        if (gKecleonAnimFramesetTable[gCurrentPinballGame->bossFramesetIndex][1] > gCurrentPinballGame->bossFrameTimer)
        {
            gCurrentPinballGame->bossFrameTimer++;
        }
        else
        {
            gCurrentPinballGame->bossFrameTimer = 0;
            gCurrentPinballGame->bossFramesetIndex++;
            if (gCurrentPinballGame->bossFramesetIndex == gCurrentPinballGame->bossColorState + 6)
            {
                int rand, mask;

                if (gCurrentPinballGame->kecleonFrameTimer < 8)
                {
                    gCurrentPinballGame->bossEntityState = 3;
                }
                else
                {
                    if (gCurrentPinballGame->kecleonTargetFrameIndex)
                    {
                        gCurrentPinballGame->bossFrameTimer = 20;
                    }
                    else
                    {
                        gCurrentPinballGame->bossFrameTimer = 0;
                        gCurrentPinballGame->kecleonTargetFrameIndex = 1;
                    }

                    gCurrentPinballGame->bossEntityState = 5;
                }

                rand = Random() + gMain.systemFrameCount;
                mask = 0x3;
                gCurrentPinballGame->kecleonTargetState = gKecleonMovementTargetGraph[gCurrentPinballGame->kecleonTargetState].adjacentStates[rand & mask];
                gCurrentPinballGame->bossFramesetIndex = gCurrentPinballGame->bossColorState + 5;
            }
        }
        break;
    case 8:
        if (gKecleonAnimFramesetTable[gCurrentPinballGame->bossFramesetIndex][1] > gCurrentPinballGame->bossFrameTimer)
        {
            gCurrentPinballGame->bossFrameTimer++;
        }
        else
        {
            int rand, mask;

            gCurrentPinballGame->bossFrameTimer = 0;
            gCurrentPinballGame->bossFramesetIndex++;
            if (gCurrentPinballGame->bossFramesetIndex == gCurrentPinballGame->bossColorState + 2)
            {
                if (gCurrentPinballGame->kecleonFrameTimer < 8)
                {
                    gCurrentPinballGame->bossEntityState = 3;
                }
                else
                {
                    if (gCurrentPinballGame->kecleonTargetFrameIndex)
                    {
                        gCurrentPinballGame->bossFrameTimer = 20;
                    }
                    else
                    {
                        gCurrentPinballGame->bossFrameTimer = 0;
                        gCurrentPinballGame->kecleonTargetFrameIndex = 1;
                    }

                    gCurrentPinballGame->bossEntityState = 5;
                }

                rand = Random() + gMain.systemFrameCount;
                mask = 0x3;
                gCurrentPinballGame->kecleonTargetState = gKecleonMovementTargetGraph[gCurrentPinballGame->kecleonTargetState].adjacentStates[rand & mask];
                gCurrentPinballGame->bossFramesetIndex = gCurrentPinballGame->bossColorState + 1;
            }
        }
        break;
    case 9:
        gCurrentPinballGame->bossEntityState = 10;
        if (gMain.systemFrameCount & 1)
        {
            if (gCurrentPinballGame->bossPositionX < -640)
            {
                gCurrentPinballGame->kecleonTargetColorMatch = 1;
                gCurrentPinballGame->bossColorState = 32;
            }
            else
            {
                gCurrentPinballGame->kecleonTargetColorMatch = 0;
                gCurrentPinballGame->bossColorState = 27;
            }
        }
        else
        {
            if (gCurrentPinballGame->bossPositionX > 640)
            {
                gCurrentPinballGame->kecleonTargetColorMatch = 0;
                gCurrentPinballGame->bossColorState = 27;
            }
            else
            {
                gCurrentPinballGame->kecleonTargetColorMatch = 1;
                gCurrentPinballGame->bossColorState = 32;
            }
        }

        if (gCurrentPinballGame->kecleonTargetX < 16)
            gCurrentPinballGame->kecleonTargetX = 0;

        gCurrentPinballGame->bossFramesetIndex = gCurrentPinballGame->bossColorState;
        gCurrentPinballGame->bossFrameTimer = 0;
        MPlayStart(&gMPlayInfo_SE1, &se_unk_100);
        break;
    case 10:
        if (gKecleonAnimFramesetTable[gCurrentPinballGame->bossFramesetIndex][1] > gCurrentPinballGame->bossFrameTimer)
        {
            gCurrentPinballGame->bossFrameTimer++;
        }
        else
        {
            gCurrentPinballGame->bossFrameTimer = 0;
            gCurrentPinballGame->bossFramesetIndex++;
            if (gCurrentPinballGame->bossFramesetIndex == gCurrentPinballGame->bossColorState + 1)
                gCurrentPinballGame->kecleonTargetY = 23;

            if (gCurrentPinballGame->bossFramesetIndex == gCurrentPinballGame->bossColorState + 2)
            {
                gCurrentPinballGame->bossEntityState = 13;
                gCurrentPinballGame->bossFramesetIndex = gCurrentPinballGame->bossColorState + 1;
            }
        }
        break;
    case 11:
        gCurrentPinballGame->bossEntityState = 12;
        gCurrentPinballGame->bossFramesetIndex = gCurrentPinballGame->bossColorState + 2;
        gCurrentPinballGame->bossFrameTimer = 0;
        gCurrentPinballGame->kecleonTargetX = 0;
        gCurrentPinballGame->scoreAddedInFrame = 500000;
        gCurrentPinballGame->bonusModeHitCount++;
        MPlayStart(&gMPlayInfo_SE1, &se_unk_101);
        PlayRumble(7);
        break;
    case 12:
        if (gKecleonAnimFramesetTable[gCurrentPinballGame->bossFramesetIndex][1] > gCurrentPinballGame->bossFrameTimer)
        {
            gCurrentPinballGame->bossFrameTimer++;
        }
        else
        {
            gCurrentPinballGame->bossFrameTimer = 0;
            gCurrentPinballGame->bossFramesetIndex++;
            if (gCurrentPinballGame->bossFramesetIndex == gCurrentPinballGame->bossColorState + 4)
            {
                gCurrentPinballGame->bossFramesetIndex = gCurrentPinballGame->bossColorState + 3;
                if (gCurrentPinballGame->bonusModeHitCount < 10)
                {
                    gCurrentPinballGame->bossEntityState = 13;
                }
                else
                {
                    gCurrentPinballGame->boardModeType = 3;
                    gMain.modeChangeFlags = MODE_CHANGE_BONUS_BANNER;
                    gCurrentPinballGame->ballRespawnState = 2;
                    gCurrentPinballGame->ballRespawnTimer = 0;
                    gCurrentPinballGame->bossEntityState = 13;
                    gCurrentPinballGame->kecleonHitState = 0;
                    gCurrentPinballGame->kecleonAnimTimer = 0;
                    gCurrentPinballGame->kecleonTargetActive = 0;
                    gCurrentPinballGame->boardEntityCollisionMode = 0;
                }
            }
        }
        break;
    case 13:
        gCurrentPinballGame->bossFramesetIndex = gCurrentPinballGame->bossColorState + 4;
        if (gCurrentPinballGame->bossFrameTimer < 8)
        {
            gCurrentPinballGame->bossFrameTimer++;
        }
        else
        {
            if (gCurrentPinballGame->bonusModeHitCount > 9)
            {
                if (gCurrentPinballGame->kecleonTargetColorMatch == 0)
                {
                    gCurrentPinballGame->bossFramesetIndex = 72;
                    gCurrentPinballGame->bossPositionX -= 240;
                }
                else
                {
                    gCurrentPinballGame->bossFramesetIndex = 73;
                    gCurrentPinballGame->bossPositionX += 240;
                }

                gCurrentPinballGame->bossEntityState = 5;
                gCurrentPinballGame->bossFrameTimer = 20;
                gCurrentPinballGame->kecleonTargetState = 3 + ((Random() * gMain.systemFrameCount) & 0x3);
            }
            else
            {
                if (gCurrentPinballGame->kecleonTargetX < 16)
                {
                    if (gCurrentPinballGame->kecleonTargetColorMatch == 0)
                    {
                        gCurrentPinballGame->bossFramesetIndex = 72;
                        gCurrentPinballGame->bossPositionX -= 240;
                    }
                    else
                    {
                        gCurrentPinballGame->bossFramesetIndex = 73;
                        gCurrentPinballGame->bossPositionX += 240;
                    }

                    gCurrentPinballGame->bossEntityState = 5;
                    gCurrentPinballGame->bossFrameTimer = 20;
                    gCurrentPinballGame->kecleonTargetState = 3 + ((Random() * gMain.systemFrameCount) & 0x3);
                }
                else
                {
                    if (gCurrentPinballGame->kecleonTargetColorMatch == 0)
                    {
                        gCurrentPinballGame->bossFramesetIndex = 72;
                        gCurrentPinballGame->bossPositionX -= 240;
                    }
                    else
                    {
                        gCurrentPinballGame->bossFramesetIndex = 73;
                        gCurrentPinballGame->bossPositionX += 240;
                    }

                    gCurrentPinballGame->bossFrameTimer = 0;
                    if (gCurrentPinballGame->kecleonFrameTimer < 8)
                    {
                        gCurrentPinballGame->bossEntityState = 3;
                    }
                    else
                    {
                        if (gCurrentPinballGame->kecleonTargetFrameIndex)
                        {
                            gCurrentPinballGame->bossFrameTimer = 20;
                        }
                        else
                        {
                            gCurrentPinballGame->bossFrameTimer = 0;
                            gCurrentPinballGame->kecleonTargetFrameIndex = 1;
                        }

                        gCurrentPinballGame->bossEntityState = 5;
                    }
                }
            }
        }
        break;
    case 14:
        gCurrentPinballGame->kecleonTargetX = 0;
        if (gKecleonAnimFramesetTable[gCurrentPinballGame->bossFramesetIndex][1] > gCurrentPinballGame->bossFrameTimer)
        {
            gCurrentPinballGame->bossFrameTimer++;
        }
        else
        {
            gCurrentPinballGame->bossFrameTimer = 0;
            gCurrentPinballGame->bossFramesetIndex++;
            if (gCurrentPinballGame->bossFramesetIndex == gCurrentPinballGame->bossColorState + 19)
            {
                gCurrentPinballGame->bossFramesetIndex = gCurrentPinballGame->bossColorState + 18;
                gCurrentPinballGame->bossEntityState = 15;
            }

            if (gCurrentPinballGame->bossFramesetIndex == gCurrentPinballGame->bossColorState + 6)
                MPlayStart(&gMPlayInfo_SE1, &se_unk_102);

            if (gCurrentPinballGame->bossFramesetIndex == gCurrentPinballGame->bossColorState + 8)
                MPlayStart(&gMPlayInfo_SE1, &se_unk_103);
        }
        break;
    case 15:
        if (gCurrentPinballGame->bossFrameTimer < 100)
        {
            if (gCurrentPinballGame->bossFrameTimer == 0)
            {
                gCurrentPinballGame->boardState = 2;
                gCurrentPinballGame->stageTimer = 0;
            }

            gCurrentPinballGame->bossFrameTimer++;
        }

        gCurrentPinballGame->bossFramesetIndex = gCurrentPinballGame->bossColorState + 15;
        break;
    }
}

void RenderKecleonSprites(void)
{
    s16 i;
    struct SpriteGroup *spriteGroup;
    s16 sp0;
    s16 sp4;
    struct OamDataSimple *oamSimple;
    u16 *dst;
    const u16 *src;
    int var0;
    int var1;
    int baseX = 104;
    int baseY = 28;

    spriteGroup = &gMain_spriteGroups[gKecleonSpriteOrderMap[23]];
    if (!spriteGroup->available)
        return;

    sp0 = gCurrentPinballGame->bossVulnerable;
    gCurrentPinballGame->bossVulnerable = gKecleonAnimFramesetTable[gCurrentPinballGame->bossFramesetIndex][2];
    DmaCopy16(3, gKecleonStageKecleon_Gfx[sp0], (void *)0x6010920, 0x280);
    sp4 = gKecleonAnimFramesetTable[gCurrentPinballGame->bossFramesetIndex][0];
    spriteGroup->baseX = gCurrentPinballGame->bossPositionX / 10 + baseX - gCurrentPinballGame->cameraXOffset;
    spriteGroup->baseY = gCurrentPinballGame->bossPositionY / 10 + baseY - gCurrentPinballGame->cameraYOffset;
    if (gCurrentPinballGame->bossFramesetIndex >= 28 && gCurrentPinballGame->bossFramesetIndex < 31)
    {
        gCurrentPinballGame->bossCollisionX = 2 * (gCurrentPinballGame->bossPositionX / 10) + 144;
        gCurrentPinballGame->bossCollisionY = 2 * (gCurrentPinballGame->bossPositionY / 10) + 84;
        if (gCurrentPinballGame->bonusModeHitCount < 10)
            gCurrentPinballGame->boardEntityCollisionMode = 2;
    }
    else if (gCurrentPinballGame->bossFramesetIndex >= 33 && gCurrentPinballGame->bossFramesetIndex < 36)
    {
        gCurrentPinballGame->bossCollisionX = 2 * (gCurrentPinballGame->bossPositionX / 10) + 240;
        gCurrentPinballGame->bossCollisionY = 2 * (gCurrentPinballGame->bossPositionY / 10) + 84;
        if (gCurrentPinballGame->bonusModeHitCount < 10)
            gCurrentPinballGame->boardEntityCollisionMode = 2;
    }
    else
    {
        gCurrentPinballGame->bossCollisionX = 2 * (gCurrentPinballGame->bossPositionX / 10) + 208;
        gCurrentPinballGame->bossCollisionY = 2 * (gCurrentPinballGame->bossPositionY / 10) + 56;
        if (gCurrentPinballGame->bonusModeHitCount < 10)
            gCurrentPinballGame->boardEntityCollisionMode = 1;
    }

    gCurrentPinballGame->kecleonCollisionPos.x = (gCurrentPinballGame->bossPositionX / 10) + 120;
    gCurrentPinballGame->kecleonCollisionPos.y = (gCurrentPinballGame->bossPositionY / 10) + 58;
    if (gCurrentPinballGame->kecleonFrameTimer > 7)
        gCurrentPinballGame->kecleonTargetVisible = 1;
    else if (gCurrentPinballGame->kecleonTargetX > 15)
        gCurrentPinballGame->kecleonTargetVisible = 0;
    else
        gCurrentPinballGame->kecleonTargetVisible = 1;

    for (i = 0; i < 2; i++)
    {
        oamSimple = &spriteGroup->oam[i];
        dst = (u16*)&gOamBuffer[oamSimple->oamId];
        src = gKecleonUpperBodyOamData[sp4][i];
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;

        if (gCurrentPinballGame->kecleonTargetVisible)
        {
            gOamBuffer[oamSimple->oamId].x += spriteGroup->baseX;
            gOamBuffer[oamSimple->oamId].y += spriteGroup->baseY;
            if (gCurrentPinballGame->bossVulnerable == 20) {
                if(gCurrentPinballGame->bossFrameTimer < 10)
                    gOamBuffer[oamSimple->oamId].paletteNum = 10;
                else
                    gOamBuffer[oamSimple->oamId].paletteNum = 2;
            }
            else
            {
                gOamBuffer[oamSimple->oamId].paletteNum = 2;
            }
        }
        else
        {
            gOamBuffer[oamSimple->oamId].x = 240;
            gOamBuffer[oamSimple->oamId].y = 180;
        }
    }

    baseX = 104;
    baseY = 60;
    spriteGroup = &gMain_spriteGroups_31;
    spriteGroup->baseX = gCurrentPinballGame->bossPositionX / 10 + baseX - gCurrentPinballGame->cameraXOffset;
    spriteGroup->baseY = gCurrentPinballGame->bossPositionY / 10 + baseY - gCurrentPinballGame->cameraYOffset;

    for (i = 0; i < 2; i++)
    {
        oamSimple = &spriteGroup->oam[i];
        dst = (u16*)&gOamBuffer[oamSimple->oamId];
        src = gKecleonLowerBodyOamData[sp4][i];
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;

        if (gCurrentPinballGame->kecleonTargetVisible)
        {
            gOamBuffer[oamSimple->oamId].x += spriteGroup->baseX;
            gOamBuffer[oamSimple->oamId].y += spriteGroup->baseY;
            if (sp0 == 20)
            {
                if (gCurrentPinballGame->bossFrameTimer < 10)
                    gOamBuffer[oamSimple->oamId].paletteNum = 11;
                else
                    gOamBuffer[oamSimple->oamId].paletteNum = 4;
            }
            else
            {
                gOamBuffer[oamSimple->oamId].paletteNum = 4;
            }
        }
        else
        {
            gOamBuffer[oamSimple->oamId].x = 240;
            gOamBuffer[oamSimple->oamId].y = 180;
        }
    }

    baseX = 104;
    baseY = 26;
    spriteGroup = &gMain_spriteGroups_9;
    var0 = gCurrentPinballGame->kecleonCollisionEnabled * 3;
    var1 = gCurrentPinballGame->kecleonTargetY / 8 - 2;

    DmaCopy16(3, gKecleonStageKecleonFx_Gfx[var0 - var1], (void *)0x6010BA0, 0x100);
    if (gCurrentPinballGame->kecleonTargetY > 0)
    {
        gCurrentPinballGame->kecleonTargetY--;
        spriteGroup->baseX = gCurrentPinballGame->bossPositionX / 10 + baseX - gCurrentPinballGame->cameraXOffset;
        spriteGroup->baseY = gCurrentPinballGame->bossPositionY / 10 + - gCurrentPinballGame->cameraYOffset + baseY;
    }
    else
    {
        spriteGroup->baseX = 0;
        spriteGroup->baseY = 180;
    }

    oamSimple = &spriteGroup->oam[0];
    gOamBuffer[oamSimple->oamId].x = oamSimple->xOffset + spriteGroup->baseX;
    gOamBuffer[oamSimple->oamId].y = oamSimple->yOffset + spriteGroup->baseY;
    if (gCurrentPinballGame->kecleonTargetColorMatch == 0)
        gOamBuffer[oamSimple->oamId].hFlip = 0;
    else
        gOamBuffer[oamSimple->oamId].hFlip = 1;
}


void UpdateKecleonScopeItem(void)
{
    s16 i;
    struct SpriteGroup *group;
    struct OamDataSimple *oamSimple;
    struct Vector32 tempVector;
    int xx, yy;
    int squaredMagnitude;

    group = &gMain.spriteGroups[24];
    if (!group->available)
        return;

    if (gCurrentPinballGame->kecleonHitState)
    {
        if (gCurrentPinballGame->kecleonCollisionX == 0)
        {
            gCurrentPinballGame->kecleonHitFrameIndex = (Random() * gMain.systemFrameCount) % 5;
            gCurrentPinballGame->orbLogicPosition.y = 0;
            gCurrentPinballGame->orbCollisionPosition.y = 0;
        }

        if (gCurrentPinballGame->kecleonCollisionX < 40)
            gCurrentPinballGame->kecleonCollisionX++;

        if (gCurrentPinballGame->kecleonCollisionX == 20)
            m4aMPlayStop(&gMPlayInfo_SE2);

        if (gCurrentPinballGame->kecleonCollisionX == 22)
            m4aSongNumStart(SE_KECLEON_SCOPE_FALL);

        if (gCurrentPinballGame->kecleonCollisionX > 20)
        {
            gCurrentPinballGame->orbLogicPosition.x = 4;
            gCurrentPinballGame->orbLogicPosition.y += 3;
            gCurrentPinballGame->orbCollisionPosition.x = gKecleonScopeFallTargets[gCurrentPinballGame->kecleonHitFrameIndex].x;
            gCurrentPinballGame->orbCollisionPosition.y += gCurrentPinballGame->orbLogicPosition.y;
            if (gCurrentPinballGame->orbCollisionPosition.y > gKecleonScopeFallTargets[gCurrentPinballGame->kecleonHitFrameIndex].y)
            {
                gCurrentPinballGame->orbCollisionPosition.y = gKecleonScopeFallTargets[gCurrentPinballGame->kecleonHitFrameIndex].y;
                gCurrentPinballGame->orbLogicPosition.y = -(gCurrentPinballGame->orbLogicPosition.y / 6);
            }

            tempVector.x = gCurrentPinballGame->ball->positionQ0.x - (gCurrentPinballGame->orbCollisionPosition.x / 10 + 8);
            tempVector.y = gCurrentPinballGame->ball->positionQ0.y - (gCurrentPinballGame->orbCollisionPosition.y / 10 + 8);
            xx = tempVector.x * tempVector.x;
            yy = tempVector.y * tempVector.y;
            squaredMagnitude = xx + yy;
            if (squaredMagnitude <= 120)
            {
                gCurrentPinballGame->kecleonHitState = 0;
                gCurrentPinballGame->kecleonTargetActive = 1;
                gCurrentPinballGame->kecleonAnimTimer = 0;
                gCurrentPinballGame->scoreAddedInFrame = 10000;
                m4aSongNumStart(SE_KECLEON_SCOPE_ACTIVATED);
            }

            group->baseX = -gCurrentPinballGame->cameraXOffset + gCurrentPinballGame->orbCollisionPosition.x / 10;
            group->baseY = -gCurrentPinballGame->cameraYOffset + gCurrentPinballGame->orbCollisionPosition.y / 10;
        }
        else
        {
            group->baseX = 0;
            group->baseY = 180;
        }
    }
    else
    {
        group->baseX = 0;
        group->baseY = 180;
    }

    oamSimple = &group->oam[0];
    gOamBuffer[oamSimple->oamId].x = oamSimple->xOffset + group->baseX;
    gOamBuffer[oamSimple->oamId].y = oamSimple->yOffset + group->baseY;
}


void UpdateKecleonScopeVision(void)
{
    s16 i, j;
    s16 var0;
    s16 var1;
    s16 var2;
    s16 index;
    s16 var3;
    const u16 *ptr;

    if (gCurrentPinballGame->kecleonTargetActive)
    {
        if (gCurrentPinballGame->kecleonAnimTimer < 600)
        {
            gCurrentPinballGame->kecleonAnimTimer++;
        }
        else
        {
            gCurrentPinballGame->kecleonAnimTimer = 0;
            gCurrentPinballGame->kecleonTargetActive = 0;
        }
    }

    if (gCurrentPinballGame->kecleonTargetActive)
    {
        if (gCurrentPinballGame->kecleonFrameTimer < 10)
            gCurrentPinballGame->kecleonFrameTimer++;
    }
    else
    {
        if (gCurrentPinballGame->kecleonFrameTimer)
            gCurrentPinballGame->kecleonFrameTimer--;
        else
            gCurrentPinballGame->kecleonTargetFrameIndex = 0;
    }

    gMain.kecleonOverlayHeight = gCurrentPinballGame->kecleonFrameTimer * 16;
    var0 = gCurrentPinballGame->kecleonFrameTimer * 64;
    var1 = (20 - gCurrentPinballGame->kecleonFrameTimer * 2) * 32;
    index = (gCurrentPinballGame->kecleonAnimTimer % 36) / 9;
    var3 = gKecleonOverlayTileAnimIndices[index];
    ptr = gKecleonOverlayTilemapPointers[index];
    for (j = 0; j < var0; j++)
        gBG0TilemapBuffer[0x400 + j] = ptr[var1 + j];
    for (j = var0; j < 0x280; j++)
        gBG0TilemapBuffer[0x400 + j] = 0x3FF;

    index = gCurrentPinballGame->kecleonAnimTimer % 150;
    if (index >= 10 && index < 80)
    {
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 4; j++)
            {
                gBG0TilemapBuffer[0x408 + ((i + 6) * 0x20) + j] = gKecleonBerryOverlayTilemap[((i + 21) * 0x20) + (j + var3 * 4)];
            }
        }

        var2 = index / 10;
        if (var2 > 5)
            var2 = 5;

        for (j = 0; j < var2; j++)
        {
            gBG0TilemapBuffer[0x4C3 + j] = gKecleonBerryOverlayTilemap[0x2EC + j];
        }
    }

    if (index >= 80 && index < 150)
    {
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 6; j++)
            {
                gBG0TilemapBuffer[0x411 + ((i + 14) * 0x20) + j] = gKecleonBerryOverlayTilemap[((i + 21) * 0x20) + (j + 0xC + var3 * 6)];
            }
        }

        var2 = (index - 70) / 10;
        if (var2 > 5)
            var2 = 5;

        for (j = 0; j < var2; j++)
        {
            gBG0TilemapBuffer[0x5F7 + j] = gKecleonBerryOverlayTilemap[0x2EC + j];
        }
    }

    DmaCopy16(3, &gBG0TilemapBuffer[0x400], (void *)0x06001000, 0x500);
}

void RenderKecleonBoardElements(void)
{
    s16 i, j;
    struct SpriteGroup *group;
    struct OamDataSimple *oamSimple;
    struct OamData *oamData;
    struct Vector16 tempVector;
    s16 var0;
    s16 var1;
    s16 var2;

    tempVector.x = gCurrentPinballGame->kecleonCollisionPos.x * 2;
    tempVector.y = gCurrentPinballGame->kecleonCollisionPos.y * 2;
    CheckKecleonProjectileCollision(&tempVector);

    group = &gMain.spriteGroups[gKecleonSpriteOrderMap[16]];
    group->baseX = 120 - gCurrentPinballGame->cameraXOffset;
    group->baseY = -gCurrentPinballGame->cameraYOffset - 128;
    for (j = 0; j < 4; j++)
    {
        oamSimple = &group->oam[j];
        gOamBuffer[oamSimple->oamId].x = oamSimple->xOffset + group->baseX;
        gOamBuffer[oamSimple->oamId].y = oamSimple->yOffset + group->baseY;
    }

    group = &gMain.spriteGroups[gKecleonSpriteOrderMap[17]];
    group->baseX = 120 - gCurrentPinballGame->cameraXOffset;
    group->baseY = -gCurrentPinballGame->cameraYOffset - 128;
    oamSimple = &group->oam[0];
    gOamBuffer[oamSimple->oamId].x = oamSimple->xOffset + group->baseX;
    gOamBuffer[oamSimple->oamId].y = oamSimple->yOffset + group->baseY;

    group = &gMain.spriteGroups[7];
    var0 = 5 - gCurrentPinballGame->kecleonCollisionY % 10;
    if (gCurrentPinballGame->kecleonCollisionY > 30)
        var1 = 20;
    else if (gCurrentPinballGame->kecleonCollisionY > 10)
        var1 = 10;
    else
        var1 = 5;

    if (gCurrentPinballGame->kecleonCollisionY == 40)
    {
        m4aSongNumStart(SE_KECLEON_TREE_HIT);
        PlayRumble(7);
    }

    if (var0 < 0)
        var0 = -var0;

    group->baseX = (var1 * (5 - var0)) / 10 + 120u - gCurrentPinballGame->cameraXOffset;
    group->baseY = -gCurrentPinballGame->cameraYOffset - 128;
    for  (j = 0; j < 5; j++)
    {
        oamSimple = &group->oam[j];
        gOamBuffer[oamSimple->oamId].x = oamSimple->xOffset + group->baseX;
        gOamBuffer[oamSimple->oamId].y = oamSimple->yOffset + group->baseY;
    }

    if (gCurrentPinballGame->kecleonCollisionY)
        gCurrentPinballGame->kecleonCollisionY--;

    for (i = 0; i < 6; i++)
    {
        group = &gMain.spriteGroups[gKecleonSpriteOrderMap[10 + i]];
        group->baseX = 120 - gCurrentPinballGame->cameraXOffset;
        group->baseY = -gCurrentPinballGame->cameraYOffset - 128;
        var2 = 0;
        if (gCurrentPinballGame->kecleonBerrySmallFlashTimer[i])
        {
            var2 = gKecleonBerryFlashAnimIndices[(gCurrentPinballGame->kecleonBerrySmallFlashTimer[i] % 24) / 6];
            gCurrentPinballGame->kecleonBerrySmallFlashTimer[i]--;
        }

        for (j = 0; j < 2; j++)
        {
            oamSimple = &group->oam[j];
            gOamBuffer[oamSimple->oamId].x = oamSimple->xOffset + group->baseX;
            gOamBuffer[oamSimple->oamId].y = oamSimple->yOffset + group->baseY;
            gOamBuffer[oamSimple->oamId].tileNum = 0xD2 + var2 * 12 + j * 8;
        }
    }

    for (i = 6; i < 10; i++)
    {
        group = &gMain.spriteGroups[gKecleonSpriteOrderMap[12 + i]];
        group->baseX = 120 - gCurrentPinballGame->cameraXOffset;
        group->baseY = -gCurrentPinballGame->cameraYOffset - 128;
        var2 = 0;
        if (gCurrentPinballGame->kecleonBerrySmallFlashTimer[i])
        {
            var2 = gKecleonBerryFlashAnimIndices[(gCurrentPinballGame->kecleonBerrySmallFlashTimer[i] % 24) / 6];
            gCurrentPinballGame->kecleonBerrySmallFlashTimer[i]--;
        }

        for (j = 0; j < 2; j++)
        {
            oamSimple = &group->oam[j];
            gOamBuffer[oamSimple->oamId].x = oamSimple->xOffset + group->baseX;
            gOamBuffer[oamSimple->oamId].y = oamSimple->yOffset + group->baseY;
            gOamBuffer[oamSimple->oamId].tileNum = 0xD2 + var2 * 12 + j * 8;
        }
    }

    group = &gMain.spriteGroups[25];
    group->baseX = 120 - gCurrentPinballGame->cameraXOffset;
    group->baseY = -gCurrentPinballGame->cameraYOffset - 128;
    var2 = 0;
    if (gCurrentPinballGame->kecleonBerryLargeFlashTimer[0])
    {
        var2 = gKecleonBerryFlashAnimIndices[(gCurrentPinballGame->kecleonBerryLargeFlashTimer[0] % 24) / 6];
        gCurrentPinballGame->kecleonBerryLargeFlashTimer[0]--;
    }

    for (j = 0; j < 3; j++)
    {
        oamSimple = &group->oam[j];
        gOamBuffer[oamSimple->oamId].x = oamSimple->xOffset + group->baseX;
        gOamBuffer[oamSimple->oamId].y = oamSimple->yOffset + group->baseY;
        gOamBuffer[oamSimple->oamId].tileNum = 0xF6 + var2 * 8 + (j * 4 - (j / 2) * 2);
    }

    for (i = 0; i < 2; i++)
    {
        group = &gMain.spriteGroups[26 + i];
        group->baseX = 120 - gCurrentPinballGame->cameraXOffset;
        group->baseY = -gCurrentPinballGame->cameraYOffset - 128;

        var2 = 0;
        if (gCurrentPinballGame->kecleonBerryLargeFlashTimer[1 + i])
        {
            var2 = gKecleonBerryFlashAnimIndices[(gCurrentPinballGame->kecleonBerryLargeFlashTimer[1 + i] % 24) / 6];
            gCurrentPinballGame->kecleonBerryLargeFlashTimer[1 + i]--;
        }

        for (j = 0; j < 2; j++)
        {
            oamSimple = &group->oam[j];
            gOamBuffer[oamSimple->oamId].x = oamSimple->xOffset + group->baseX;
            gOamBuffer[oamSimple->oamId].y = oamSimple->yOffset + group->baseY;
            gOamBuffer[oamSimple->oamId].tileNum = 0x10E + var2 * 12 + j * 8;
        }
    }

    group = &gMain.spriteGroups[28];
    group->baseX = 120 - gCurrentPinballGame->cameraXOffset;
    group->baseY = -gCurrentPinballGame->cameraYOffset - 128;
    var2 = 0;
    if (gCurrentPinballGame->kecleonBerryLargeFlashTimer[3])
    {
        var2 = gKecleonBerryFlashAnimIndices[(gCurrentPinballGame->kecleonBerryLargeFlashTimer[3] % 24) / 6];
        gCurrentPinballGame->kecleonBerryLargeFlashTimer[3]--;
    }

    for (j = 0; j < 2; j++)
    {
        oamSimple = &group->oam[j];
        gOamBuffer[oamSimple->oamId].x = oamSimple->xOffset + group->baseX;
        gOamBuffer[oamSimple->oamId].y = oamSimple->yOffset + group->baseY;
        gOamBuffer[oamSimple->oamId].tileNum = 0x132 + var2 * 12 + j * 8;
    }

    group = &gMain.spriteGroups[32];
    group->baseX = gCurrentPinballGame->ball->screenPosition.x;
    group->baseY = gCurrentPinballGame->ball->screenPosition.y + 14;
    oamSimple = &group->oam[0];
    gOamBuffer[oamSimple->oamId].x = oamSimple->xOffset + group->baseX;
    if (gCurrentPinballGame->ball->ballHidden)
        gOamBuffer[oamSimple->oamId].y = 200;
    else
        gOamBuffer[oamSimple->oamId].y = oamSimple->yOffset + group->baseY;

    group = &gMain.spriteGroups[29];
    if (gCurrentPinballGame->kecleonHitboxX < 12)
        gCurrentPinballGame->kecleonHitboxX++;
    else
        gCurrentPinballGame->kecleonHitboxX = 0;

    for (i = 0; i < 4; i++)
    {
        if (gCurrentPinballGame->kecleonHitSparkTimer[i])
        {
            var2 = 2 - gCurrentPinballGame->kecleonHitSparkTimer[i] / 4;
            gCurrentPinballGame->kecleonHitSparkTimer[i]--;
            group->baseX = gCurrentPinballGame->ballTrailPosition[i].x - gCurrentPinballGame->cameraBaseX + 4;
            group->baseY = gCurrentPinballGame->ballTrailPosition[i].y - gCurrentPinballGame->cameraBaseY- gCurrentPinballGame->cameraScrollOffset - gCurrentPinballGame->cameraYAdjust + 4;
        }
        else
        {
            group->baseX = 240;
            group->baseY = 180;
        }

        oamSimple = &group->oam[i];
        gOamBuffer[oamSimple->oamId].x = oamSimple->xOffset + group->baseX;
        gOamBuffer[oamSimple->oamId].y = oamSimple->yOffset + group->baseY;
        gOamBuffer[oamSimple->oamId].tileNum = 0x16E + var2;
    }

    group = &gMain.spriteGroups[30];
    if (gCurrentPinballGame->kecleonHitboxY < 24)
        gCurrentPinballGame->kecleonHitboxY++;
    else
        gCurrentPinballGame->kecleonHitboxY = 0;

    for (i = 0; i < 3; i++)
    {
        if (gCurrentPinballGame->kecleonHitSparkTimer[4 + i])
        {
            var2 = 2 - gCurrentPinballGame->kecleonHitSparkTimer[4 + i] / 6;
            gCurrentPinballGame->kecleonHitSparkTimer[4 + i]--;
            group->baseX = gCurrentPinballGame->kecleonBerryHitPosition[i].x - gCurrentPinballGame->cameraXOffset - 4;
            group->baseY = gCurrentPinballGame->kecleonBerryHitPosition[i].y - gCurrentPinballGame->cameraYOffset + 2;
        }
        else
        {
            group->baseX = 240;
            group->baseY = 180;
        }

        oamSimple = &group->oam[i];
        gOamBuffer[oamSimple->oamId].x = oamSimple->xOffset + group->baseX;
        gOamBuffer[oamSimple->oamId].y = oamSimple->yOffset + group->baseY;
        gOamBuffer[oamSimple->oamId].tileNum = 0x16E + var2;
    }
}

void SortKecleonSpritesByY(void)
{
    s16 i, j;
    struct KecleonSpriteSortEntry sp0[14];

    for (i = 0; i < 33; i++)
        gKecleonSpriteSets[i] = gKecleonDefaultSpriteSets[i];

    sp0[12] = gKecleonSpriteYSortData[12];
    sp0[12].ySortKey = gCurrentPinballGame->ball->positionQ0.y;
    for (i = 0; i < 12; i++)
    {
        if (gKecleonSpriteYSortData[i].ySortKey < gCurrentPinballGame->ball->positionQ0.y)
        {
            for (j = 11; j >= i; j--)
                sp0[j + 1] = gKecleonSpriteYSortData[j];

            sp0[i] = gKecleonSpriteYSortData[12];
            sp0[i].ySortKey = gCurrentPinballGame->ball->positionQ0.y;
            break;
        }
        else
        {
            sp0[i] = gKecleonSpriteYSortData[i];
        }
    }

    sp0[13] = gKecleonSpriteYSortData[13];
    for (i = 0; i < 13; i++)
    {
        if (sp0[i].ySortKey < gCurrentPinballGame->kecleonCollisionPos.y)
        {
            for (j = 12; j >= i; j--)
                sp0[j + 1] = sp0[j];

            sp0[i] = gKecleonSpriteYSortData[13];
            sp0[i].ySortKey = gCurrentPinballGame->kecleonCollisionPos.y;
            break;
        }
    }

    for (i = 0; i < 14; i++)
    {
        gKecleonSpriteOrderMap[sp0[i].spriteIndex + 10] = i + 10;
        gKecleonSpriteSets[i + 10] = sp0[i].spriteSet;
    }

    LoadSpriteSets(gKecleonSpriteSets, 33, gMain.spriteGroups);
}
