/*
 * commands_small.h
 *    Defines data structure and populates command array with small number of
 *    commands.
 */

#ifndef COMMANDS_SMALL_H
#define COMMANDS_SMALL_H

#ifdef COMMANDS_BIG_H
#error "Headers commands_big.h and commands_small.h are mutually exclusive"
#endif

#include <stdio.h>

typedef struct {
  char *cmd;
  char *key;
  char *value;
} cmdNode; 

cmdNode CommandList[] =
{
    { "INS", "BPVHN", "F]Yi|`2fHA}E7" }
   ,{ "INS", "VWVWM", "O?ySvhnKq5*K|'-/jSNhub.h;z]+l.f=K`nCI]k;psdmwnzcBIM9-Y~FUOCh6]4sMT=+AVy5;h.+cnK.<" }
   ,{ "INS", "RQKHC", "'?H83rjN{,%u?>^JFC:oNTQdH3@nNIXSe~ivqTEm^GdzcDF*d]w32JtWZ6G*]}ZD|E;ov^^U'CQgduoITgZc2O<jcarA_Nb]p{" }
   ,{ "INS", "CHQLV", "2d7X9|3z0`7xDvlcV;G4-C^r{h2[5}$Ec8zy6/tCm-=3$*sXB<i" }
   ,{ "INS", "IPEMF", "O8g3Nc.ap$&^/@o0Gceg|P5ZwS5*$ILQ^5b-umlfnoFz06,U" }
   ,{ "INS", "XIMMG", "c@" }
   ,{ "SEL", "IPEMF", NULL }
   ,{ "SEL", "CHQLV", NULL }
   ,{ "INS", "XWARP", "R~m`6w6.gQ$*~R@#7^&BJX.Hl#=pcuc6uQt-J+83Z::YjWZ~7^B^7M(%NBr29VF0);:QCPa{gxURQ1RelqE%?J'jjwz$OA1VYI(zvgv_`M33[b" }
   ,{ "INS", "HLWSM", "OHXQuv`MQ*RL~:C^xnn,JQ$p'><u#jSP4-~*$_TSg(|f?AE905CXcDIg_c^`N21`<1g>m=nVBl=_.`u<r" }
   ,{ "UPD", "IPEMF", "?<}{zM.,.H:sd)1T]QAw1MX(gK?Z#xt@6s<1" }
   ,{ "SEL", "IPEMF", NULL }
   //,{ "DEL", "CHQLV", NULL }
   ,{ "SEL", "IPEMF", NULL }
   ,{ "INS", "NPNBF", "G" }
   ,{ "SEL", "XWARP", NULL }
   ,{ "SEL", "IPEMF", NULL }
   //,{ "DEL", "IPEMF", NULL }
   ,{ "SEL", "BPVHN", NULL }
   ,{ "SEL", "FOONS", NULL }
   ,{ "INS", "UVHFC", "$Z+U5Z[JZ$Wis>~NNT`_#h1adzQ#0Zh26qeHLBp(CIo7eoc4DEqD.#'p{Vp-1Y<DL#juB{b&kwh[[z|~lB.mG{izls-G1Oq1" }
   ,{ "SEL", "FOONS", "**^&*^*&^FDJFGDJHGDJ>DFHDK)#$" }
   ,{ "SEL", "RQKHC", NULL }
   ,{ "SEL", "NPNBF", NULL }
   ,{ "SEL", "VWVWM", NULL }
   ,{ "UPD", "RQKHC", "@#*$&@(#$&*@(#$&(*@#$&@(*#$&@(*#$&@(*#$&*(#@$&@#(*$&@#" }
   ,{ "UPD", "NPNBF", "<<SDADSD>><<SDADSADSADSADWQ>>" }
   ,{ "SEL", "UVHFC", NULL }
   ,{ "SEL", "RQKHC", NULL }
   ,{ "SEL", "NPNBF", NULL }
};

#endif /* #ifndef COMMANDS_SMALL_H */
