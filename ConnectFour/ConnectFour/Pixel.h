/*
#include <winbgi.cpp>
#include <graphics.h>
#include "Minion.h"
#include "Spell.h"
#include "Token.h"
using namespace std;
*/
const int IMGHeight = 100;
const int IMGWidth = 192;
/*
int BLANKCARD[IMGHeight][IMGWidth];
void cpyCard(const int[][IMGWidth]);
void drawCard(int, int);
void drawMinion(Minion,int, int);
void drawSpell(Spell, int, int);
void drawToken(Token, int, int);

void cpyCard(const int tbC[][IMGWidth]) {
for (int i = 0; i < IMGHeight; i++)
for (int j = 0; j < IMGWidth; j++)
BLANKCARD[i][j] = tbC[i][j];
}
void drawCard(int x, int y) {
for (int i = 0; i < IMGHeight; i++)
for (int j = 0; j < IMGWidth; j++)
if (BLANKCARD[i][j] != 99)
putpixel(x + j, y + i, BLANKCARD[i][j]);
}
void drawMinion(Minion card, int x, int y) { // 38
switch (card) {
case Antasma:
cpyCard(Antasma_ARRY);
break;
case Bill_Blaster:
cpyCard(BillBlaster_ARRY);
break;
case Block:
cpyCard(block_ARRY);
break;
case Boo:
cpyCard(Boo_ARRY);
break;
case Boomarang_Bro:
cpyCard(BoomarangBro_ARRY);
break;
case Bowser_Jr:
cpyCard(BowserJr_ARRY);
break;
case Bowser_Statue:
cpyCard(BowserStatue_ARRY);
break;
case Captain_Toad:
cpyCard(CaptainToad_ARRY);
break;
case Cataquak:
cpyCard(CheepCheep_ARRY);
break;
case Cheep_Cheep:
cpyCard(CheepCheep_ARRY);
break;
case Dry_Bones:
cpyCard(DryBowser_ARRY);
break;
case Dry_Bowser:
cpyCard(DryBowser_ARRY);
break;
case Fawful:
cpyCard(Fawful_ARRY);
break;
case FireBro:
cpyCard(FireBro_ARRY);
break;
case Fuzzy:
cpyCard(fuzzy_ARRY);
break;
case Goomba:
cpyCard(Goomba_ARRY);
break;
case Goomba_King:
cpyCard(GoombaKing_ARRY);
break;
case Goomba_Tower:
cpyCard(GoombaTower_ARRY);
break;
case Green_Koopa:
cpyCard(GreenKoopa_ARRY);
break;
case Hammer_Bro:
cpyCard(hammerbro_ARRY);
break;
case Honey_Queen:
cpyCard(HoneyQueen_ARRY);
break;
case King_Boo:
cpyCard(KingBoo_ARRY);
break;
case Koopatrol:
cpyCard(Koopatrol_ARRY);
break;
case Lakitu:
cpyCard(Lakitu_ARRY);
break;
case Luma:
cpyCard(Luma_ARRY);
break;
case Magikoopa:
cpyCard(Magikoopa_ARRY);
break;
case Mini_Goomba:
cpyCard(MiniGoomba_ARRY);
break;
case Para_Goomba:
cpyCard(Paragoomba_ARRY);
break;
case Pipe_Pirahna_Plant:
cpyCard(PipePirahnaPlant_ARRY);
break;
case Pokey:
cpyCard(pokey_ARRY);
break;
case Red_Koopa:
cpyCard(RedKoopa_ARRY);
break;
case Rocky_Wench:
cpyCard(RockyWrench_ARRY);
break;
case Shy_Guy:
cpyCard(shyGuy_ARRY);
break;
case Sledge_Bro:
cpyCard(SledgeBro_ARRY);
break;
case Toad:
cpyCard(toad_ARRY);
break;
case Toadette:
cpyCard(Toadette_ARRY);
break;
case Toadsworth:
cpyCard(Toadsworth_ARRY);
break;
case Wiggler:
cpyCard(Wiggler_ARRY);
break;
case Yoshi_Egg:
cpyCard(YoshiEgg_ARRY);
break;
}
drawCard(x, y);
}
void drawToken(Token card, int x, int y) { // 18
switch (card) {
case Ball:
cpyCard(Ball_ARRY);
break;
case Bat:
cpyCard(Bat_ARRY);
break;
case Bullet_Bill:
cpyCard(BulletBill_ARRY);
break;
case Coin:
cpyCard(coin_ARRY);
break;
case Green_Shell:
cpyCard(GreenShell_ARRY);
break;
case Iggy:
cpyCard(Iggy_ARRY);
break;
case Larry:
cpyCard(Larry_ARRY);
break;
case Lava_Bubble:
cpyCard(LavaBubble_ARRY);
break;
case Lemmy:
cpyCard(Lemmy_ARRY);
break;
case Ludwig_Von_Koopa:
cpyCard(LudwigVonKoopa_ARRY);
break;
case Morton:
cpyCard(morton_ARRY);
break;
case Red_Shell:
cpyCard(RedShell_ARRY);
break;
case Ring:
cpyCard(Ring_ARRY);
break;
case Roy:
cpyCard(Roy_ARRY);
break;
case Sonic:
cpyCard(Sonic_ARRY);
break;
case Spiny:
cpyCard(Spiny_ARRY);
break;
case War_Toad:
cpyCard(WarToad_ARRY);
break;
case Wendy:
cpyCard(Wendy_ARRY);
break;
case Yoshi:
cpyCard(Yoshi_ARRY);
break;
}
drawCard(x, y);
}
void drawSpell(Spell card, int x, int y) { // 25
switch (card) {
case OneUp:
cpyCard(ONEup_ARRY);
break;
case Blue_Shell:
cpyCard(BlueShell_ARRY);
break;
case Castle:
cpyCard(Castle_ARRY);
break;
case Damsel_in_Distress:
cpyCard(DamselinDistress_ARRY);
break;
case Dr_Mario_Pills:
cpyCard(DrMarioPills_ARRY);
break;
case Fire_Flower:
cpyCard(FireFlower_ARRY);
break;
case FLUDD:
cpyCard(FLUDD_ARRY);
break;
case Fused_Bombomb:
cpyCard(FusedBombomb_ARRY);
break;
case Fuzzy_Maddness:
cpyCard(FuzzyMadness_ARRY);
break;
case Gotta_Go_Fast:
cpyCard(GottaGoFast_ARRY);
break;
case Hammer:
cpyCard(Hammer_ARRY);
break;
case Haunted_Mansion:
cpyCard(HauntedMansion_ARRY);
break;
case Koopalings_Mischief:
cpyCard(KoopalingsMischief_ARRY);
break;
case LavaPool:
cpyCard(LavaPool_ARRY);
break;
case Lightning:
cpyCard(Lightning_ARRY);
break;
case Marios_Cap:
cpyCard(MariosCap_ARRY);
break;
case Mega_Mushroom:
cpyCard(MegaMushroom_ARRY);
break;
case Metal_Cap:
cpyCard(MetalCap_ARRY);
break;
case POW_Block:
cpyCard(POWBlock_ARRY);
break;
case Question_Block:
cpyCard(QuestionBlock_ARRY);
break;
case Super_Mushroom:
cpyCard(SuperMushroom_ARRY);
break;
case Super_Star:
cpyCard(SuperStar_ARRY);
break;
case Thwomp_Stomp:
cpyCard(ThwompStomp_ARRY);
break;
case Warios_Bike:
cpyCard(WariosBike_ARRY);
break;
case Wassup_Bro:
cpyCard(WassupBro_ARRY);
break;
case Whomp_Plomp:
cpyCard(WhompPlomp_ARRY);
break;
}
drawCard(x, y);
}

*/

