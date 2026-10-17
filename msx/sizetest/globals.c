#include "nova.h"
u8  Vb, N1, N2, Pr, Md, Prm, Region, EFlag, FFlag, TFlag, list, Serum, CodeSet;
u8  NicheCon, MugCon, SatchCon, HolstCon, RobotCon, FoodCon, PyraCon, HingeCon, PodumCon;
u8  x, y, o, VNP, SFlag, Crawl;
i16 T[31], Sc, i;
u8  R[MMax + 1];
u8  L[RMax + 1][17];
u8  Events[32], Inv[4], Wear[4], Socket[4], Brief[14], NounSet[17], PStat[1];
char Input[132], Line[132], Word[30], Identity[30], VStr[30], NStr[30];
char Vb_[VMax + 1][30], Nn[NMax + 1][30], Pp[12][30], Aa[5][54];
char RN[RMax + 1][20];
u8  Mx[4];
