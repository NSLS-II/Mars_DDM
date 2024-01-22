#include <stdio.h>

//struct chipstr{
//     unsigned int pa:10;		/* Threshold dac */
//     unsigned int pb:10;		/* Test pulse dac */
//     char rm:1;		/* Readout mods; 1=synch, 0=asynch */
//     char senfl1:1;	/* Lock on peak found */
//     char senfl2:1;	/* Lock on threshold */
//     char m0:1;		/* 1=channel mon, 0=others */
//     char m1:1;		/* 1=pk det on PD/PN; 0=other mons on PD/PN */
//     char sbn:1;		/* enable buffer on pdn & mon outputs */
//     char sb:1;		/* enable buffer on pd & mon outputs */
//     char sl:1;		/* 0=internal 2pA leakage, 1=disabled */
//     char ts:2;		/* Shaping time */
//     char rt:1;		/* 1=timing ramp duration x 3 */
//     char spur:1;		/* 1=enable pileup rejector */
//     char sse:1;		/* 1=enable multiple-firing suppression */
//     char tr:2;		/* timing ramp adjust */
//     char ss:2;		/* multiple firing time adjust */
//     char c:5;		/* m0=0,Monitor select. m0=1, channel being monitored */
//     char g:2;		/* Gain select */
//     char slh:1;		/* internal leakage adjust */
//     char sp:1;		/* Input polarity; 1=positive, 0=negative */
//    char saux:1;		/* Enable monitor output */
//     char sbm:1;		/* Enable output monitor buffer */
//     char tm:1;		/* Timing mode; 0=ToA, 1=ToT */
//};
struct chipstr{
     unsigned int pa;		/* Threshold dac */
     unsigned int pb;		/* Test pulse dac */
     unsigned char rm;		/* Readout mods; 1=synch, 0=asynch */
     unsigned char senfl1;	/* Lock on peak found */
     unsigned char senfl2;	/* Lock on threshold */
     unsigned char m0;		/* 1=channel mon, 0=others */
     unsigned char m1;		/* 1=pk det on PD/PN; 0=other mons on PD/PN */
     unsigned char sbn;		/* enable buffer on pdn & mon outputs */
     unsigned char sb;		/* enable buffer on pd & mon outputs */
     unsigned char sl;		/* 0=internal 2pA leakage, 1=disabled */
     unsigned char ts;		/* Shaping time */
     unsigned char rt;		/* 1=timing ramp duration x 3 */
     unsigned char spur;		/* 1=enable pileup rejector */
     unsigned char sse;		/* 1=enable multiple-firing suppression */
     unsigned char tr;		/* timing ramp adjust */
     unsigned char ss;		/* multiple firing time adjust */
     unsigned char c;		/* m0=0,Monitor select. m0=1, channel being monitored */
     unsigned char g;		/* Gain select */
     unsigned char slh;		/* internal leakage adjust */
     unsigned char sp;		/* Input polarity; 1=positive, 0=negative */
     unsigned char saux;		/* Enable monitor output */
     unsigned char sbm;		/* Enable output monitor buffer */
     unsigned char tm;		/* Timing mode; 0=ToA, 1=ToT */
};

volatile struct chipstr globalstr[12];

//struct chanstr{
//       char dp:4;	    /* Pileup rejector trim dac */
//       char nc1:1;	    /* no connection, set 0 */
//       char da:3;	    /* Threshold trim dac */
//       char sel:1;    /* 1=leakage current, 0=shaper output */
//       char nc2:1;    /* no connection, set 0 */
//       char sm:1;	    /* 1=channel disable */
//       char st:1;	    /* 1=enable test input (30fF cap) */
//};
struct chanstr{
       unsigned char dp;	    /* Pileup rejector trim dac */
       unsigned char nc1;	    /* no connection, set 0 */
       unsigned char da;	    /* Threshold trim dac */
       unsigned char sel;    /* 1=leakage current, 0=shaper output */
       unsigned char nc2;    /* no connection, set 0 */
       unsigned char sm;	    /* 1=channel disable */
       unsigned char st;	    /* 1=enable test input (30fF cap) */
};

volatile struct chanstr channelstr[384];
unsigned int loads[12][14];

int Bits(int chan){
  int chip, chn, i, j;
  unsigned int tword,tword2;
 	chip=chan/32;
	chn=chan%32;
	/* do globals first */
	j=0;
	tword=0;
	tword=globalstr[chip].tm &1;  /*
/*1 */	j++;	
	tword=tword<<1|globalstr[chip].sbm &1;
/*2 */ 	j++;
	tword=tword<<1|globalstr[chip].saux &1;
/*3 */	j++;
	tword=tword<<1|globalstr[chip].sp &1;
/*4 */	j++;
	tword=tword<<1|globalstr[chip].slh &1;
/*5 */	j++;
	tword=tword<<2|globalstr[chip].g &3;
/*7 */	j+=2;
	tword=tword<<5|globalstr[chip].c &31;
/*12 */	j+=5;
	tword=tword<<2|globalstr[chip].ss &3;
/*14 */	j+=2;
	tword=tword<<2|globalstr[chip].tr &3;
/*16 */	j+=2;
	tword=tword<<1|globalstr[chip].sse &1;
/*17 */	j++;
	tword=tword<<1|globalstr[chip].spur &1;
/*18 */	j++;
	tword=tword<<1|globalstr[chip].rt &1;
/*19 */	j++;
	tword=tword<<2|globalstr[chip].ts &3;
/*21 */	j+=2;
	tword=tword<<1|globalstr[chip].sl &1;
/*22 */	j++;
	tword=tword<<1|globalstr[chip].sb &1;
/*23 */	j++;
	tword=tword<<1|globalstr[chip].sbn &1;
/*24 */	j++;
	tword=tword<<1|globalstr[chip].m1 &1;
/*25 */	j++;
	tword=tword<<1|globalstr[chip].m0 &1;
/*26 */	j++;
	tword=tword<<1|globalstr[chip].senfl2 &1;
/*27 */	j++;
	tword=tword<<1|globalstr[chip].senfl1 &1;
/*28 */	j++;
	tword=tword<<1|globalstr[chip].rm &1;
/*29 */	j++;
	tword=tword<<3|(globalstr[chip].pb>>7) &7;
/*32 */	j+=3;
	
	printf("# bits=%i   tword=%x\n",j, tword);
	loads[chip][0]=tword;
	tword2=0;
	j=0;
	tword2=tword2|(globalstr[chip].pb & 0x7f);
/*7 */	j+=7;
	tword2=tword2<<10|globalstr[chip].pa & 0x3ff;
/*17 */	j+=10;

	chn=31;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*18 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*19 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*20 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*21 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*24 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*25 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*29 */	j+=4;
	
	chn=30;	
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*30 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*31 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*32 */	j++;

	printf("# bits=%i   tword2=%x\n",j, tword2);
	loads[chip][1]=tword2;
	tword2=0;

	j=0;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*1 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*4 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*5 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*9 */	j+=4;

	chn=29;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*10 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*11 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*12 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*13 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*16 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*17 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*21 */	j+=4;

	chn=28;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*22 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*23 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*24 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*25 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*28 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*29 */	j++;
	tword2=tword2<<3|(channelstr[chip*32+chn].dp>>1);
/*32 */	j+=3;
	loads[chip][2]=tword2;
	printf("# bits=%i   tword2=%x\n",j, tword2);

	tword2=0;
	j=0;
	tword2=tword2|(channelstr[chip*32+chn].dp&0x1);
/*1 */	j++;
	chn=27;

	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*2 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*3 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*4 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*5 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*8 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*9 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*13 */	j+=4;

	chn=26;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*14 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*15 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*16 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*17 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*20 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*21 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*25 */	j+=4;

	chn=25;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*26 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*27 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*28 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*29 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*32 */	j+=3;

	printf("# bits=%i   tword2=%x\n",j, tword2);
	loads[chip][3]=tword2;

	j=0;

	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*1 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*5 */	j+=4;

	chn=24;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*6 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*7 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*8 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*9 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*12 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*13 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*17 */	j+=4;

	chn=23;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*18 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*19 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*20 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*21 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*24 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*25 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*29 */	j+=4;

	chn=22;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*30 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*31 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*32 */	j++;

	printf("# bits=%i   tword2=%x\n",j, tword2);
	loads[chip][4]=tword2;
	tword2=0;
	j=0;

	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*1 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*4 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*5 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*9 */	j+=4;

	chn=21;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*10 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*11 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*12 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*13 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*16 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*17 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*21 */	j+=4;

	chn=20;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*22 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*23 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*24 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*25 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*28 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*29 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp>>1;
/*32 */	j+=3;

	printf("# bits=%i   tword2=%x\n",j, tword2);
	loads[chip][5]=tword2;
	tword2=0;
	j=0;
	tword2=tword2|channelstr[chip*32+chn].dp&1;
/*1 */	j++;
	chn=19;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*2 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*3 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*4 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*5 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*8 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*9 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*13 */	j+=4;

	chn=18;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*14 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*15 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*16 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*17 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*20 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*21 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*25 */	j+=4;

	chn=17;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*26 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*27 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*28 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*29 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*32 */	j+=3;

	printf("# bits=%i   tword2=%x\n",j, tword2);
	loads[chip][6]=tword2;
	tword2=0;
	j=0;

	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*1 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*5 */	j+=4;

	chn=16;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*6 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*7 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*8 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*9 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*12 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*13 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*17 */	j+=4;

	chn=15;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*18 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*19 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*20 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*21 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*24 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*25 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*29 */	j+=4;
	
	chn=14;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*30 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*31 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*32 */	j++;

	printf("# bits=%i   tword2=%x\n",j, tword2);
	loads[chip][7]=tword2;
	tword2=0;
	j=0;

	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*1 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*4 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*5 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*9 */	j+=4;

	chn=13;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*10 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*11 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*12 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*13 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*16 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*17 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*21 */	j+=4;

	chn=12;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*22 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*23 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*24 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*25 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*28 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*29 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp>>1;
/*32 */	j+=3;

	printf("# bits=%i   tword2=%x\n",j, tword2);
	loads[chip][8]=tword2;
	tword2=0;
	j=0;

	tword2=tword2|channelstr[chip*32+chn].dp&&1;
/*1 */	j+=1;

	chn=11;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*2 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*3 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*4 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*5 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*8 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*9 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*13 */	j+=4;

	chn=10;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*14 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*15 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*16 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*17 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*20 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*21 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*25 */	j+=4;

	chn=9;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*26 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*27 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*28 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*29 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*32 */	j+=3;

	printf("# bits=%i   tword2=%x\n",j, tword2);
	loads[chip][9]=tword2;
	tword2=0;
	j=0;

	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*1 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*5 */	j+=4;
	
	chn=8;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*6 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*7 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*8 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*9 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*12 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*13 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*17 */	j+=4;

	chn=7;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*18 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*19 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*20 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*21 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*24 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*25 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*29 */	j+=4;

	chn=6;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*30 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*31 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*32 */	j++;


	printf("# bits=%i   tword2=%x\n",j, tword2);
	loads[chip][10]=tword2;
	tword2=0;
	j=0;
	
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*1 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*4 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*5 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*9 */	j+=4;


	chn=5;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*10 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*11 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*12 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*13 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*16 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*17 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*21 */	j+=4;

 	chn=4;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*22 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*23 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*24 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*25 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*28 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*29 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].dp>>1;
/*32 */	j+=3;

 	printf("# bits=%i   tword2=%x\n",j, tword2);
	loads[chip][11]=tword2;
	tword2=0;
	j=0;

/*1 */	tword2=tword2|channelstr[chip*32+chn].dp&&1;

	chn=3;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*2 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*3 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*4 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*5 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*8 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*9 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*13 */	j+=4;

	chn=2;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*14 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*15 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*16 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*17 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*20 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*21 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*25 */	j+=4;

	chn=1;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*26 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*27 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*28 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*29 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*32 */	j+=3;

	printf("# bits=%i   tword2=%x\n",j, tword2);
	loads[chip][12]=tword2;
	tword2=0;
	j=0;

	tword2=tword2|channelstr[chip*32+chn].nc1;
/*1 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*5 */	j+=4;

	chn=0;
	tword2=tword2<<1|channelstr[chip*32+chn].st;
/*6 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sm;
/*7 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].nc2;
/*8 */	j++;
	tword2=tword2<<1|channelstr[chip*32+chn].sel;
/*9 */	j++;
	tword2=tword2<<3|channelstr[chip*32+chn].da;
/*12 */	j+=3;
	tword2=tword2<<1|channelstr[chip*32+chn].nc1;
/*13 */	j++;
	tword2=tword2<<4|channelstr[chip*32+chn].dp;
/*17 */	j+=4;
	tword2=tword2<<15;
/*32 */	j+=15;

	printf("# bits=%i   tword2=%x\n",j, tword2);
	loads[chip][13]=tword2;
	tword2=0;
	j=0;

 
 
  if(1){
	chip=chan/32;
	chn=chan%32;
	printf("Chip = %i\n",chip);
	printf("Chan = %i\n",chn); 

	printf("Settings for channel %i\n\n",chan);
	printf("pa=%x\n",globalstr[chip].pa);   
	printf("pb=%x\n",globalstr[chip].pb);   
	printf("rm=%x\n",globalstr[chip].rm);   
	printf("senfl1=%x\n",globalstr[chip].senfl1);
	printf("senfl2=%x\n",globalstr[chip].senfl2);
	printf("m0=%x\n",globalstr[chip].m0);   
	printf("m1=%x\n",globalstr[chip].m1);   
	printf("sbn=%x\n",globalstr[chip].sbn);  
	printf("sb=%x\n",globalstr[chip].sb);   
	printf("sl=%x\n",globalstr[chip].sl);   
	printf("ts=%x\n",globalstr[chip].ts);   
	printf("rt=%x\n",globalstr[chip].rt);   
	printf("spur=%x\n",globalstr[chip].spur); 
	printf("sse=%x\n",globalstr[chip].sse);  
	printf("tr=%x\n",globalstr[chip].tr);   
	printf("ss=%x\n",globalstr[chip].ss);   
	printf("c=%x\n",globalstr[chip].c);    
	printf("g=%x\n",globalstr[chip].g);    
	printf("slh=%x\n",globalstr[chip].slh);  
	printf("sp=%x\n",globalstr[chip].sp);   
	printf("saux=%x\n",globalstr[chip].saux); 
	printf("sbm=%x\n",globalstr[chip].sbm);  
	printf("tm=%x\n",globalstr[chip].tm);   
	printf("\n");
	printf("dp=%x\n",channelstr[chip*32+chn].dp); 
	printf("nc1=%x\n",0);
	printf("da=%x\n",channelstr[chip*32+chn].da); 
	printf("sel=%x\n",channelstr[chip*32+chn].sel);
	printf("nc2=%x\n",0);
	printf("sm=%x\n",channelstr[chip*32+chn].sm); 
	printf("st=%x\n",channelstr[chip*32+chn].st);  
	}
        printf("m0:\n");
	for(i=0;i<12;i++){
	    printf("%i ",globalstr[i].m0);
	    }
	    printf("\n");	
        printf("m1:\n");
	for(i=0;i<12;i++){
	    printf("%i ",globalstr[i].m1);
	    }
	    printf("\n");	
        printf("saux:\n");
	for(i=0;i<12;i++){
	    printf("%i ",globalstr[i].saux);
	    }
	    printf("\n");	
/*        printf("sm:\n");
	for(i=0;i<384;i++){
	    printf("%i ",channelstr[i].sm);
	    }
	    printf("\n");	
	printf("st:\n");
	for(i=0;i<384;i++){
	    printf("%i ",channelstr[i].st);
	    }
	    printf("\n"); */
		return(0); 
	} 
	
int main(int argc, char *argv){
  int i,j;
  
  for(i=0;i<12;i++){
globalstr[i].pa=1023;		/* Threshold dac */
globalstr[i].pb=0;	      /* Test pulse dac */
globalstr[i].rm=1;	      /* Readout mods; 1=synch, 0=asynch */
globalstr[i].senfl1=0;       /* Lock on peak found */
globalstr[i].senfl2=1;       /* Lock on threshold */
globalstr[i].m0=0;	      /* 1=channel mon, 0=others */
globalstr[i].m1=1;	      /* 1=pk det on PD/PN; 0=other mons on PD/PN */
globalstr[i].sbn=0;	      /* enable buffer on pdn & mon outputs */
globalstr[i].sb=1;	      /* enable buffer on pd & mon outputs */
globalstr[i].sl=0;	      /* 0=internal 2pA leakage, 1=disabled */
globalstr[i].ts=1;	      /* Shaping time */
globalstr[i].rt=0;	      /* 1=timing ramp duration x 3 */
globalstr[i].spur=1; 	      /* 1=enable pileup rejector */
globalstr[i].sse-0;  	      /* 1=enable multiple-firing suppression */
globalstr[i].tr=1;	      /* timing ramp adjust */
globalstr[i].ss=2;	      /* multiple firing time adjust */
globalstr[i].c=31;	      /* m0=0,Monitor select. m0=1, channel being monitored */
globalstr[i].g=1;	      /* Gain select */
globalstr[i].slh=0;  	      /* internal leakage adjust */
globalstr[i].sp=1;	      /* Input polarity; 1=positive, 0=negative */
globalstr[i].saux=0; 	      /* Enable monitor output */
globalstr[i].sbm=1;  	      /* Enable output monitor buffer */
globalstr[i].tm=0;	      /* Timing mode; 0=ToA, 1=ToT */

    for(j=0;j<32;j++){
      
       channelstr[i*32+j].dp=15;	       /* Pileup rejector trim dac */
       channelstr[i*32+j].nc1=0;	       /* no connection, set 0 */
       channelstr[i*32+j].da=5;	       /* Threshold trim dac */
       channelstr[i*32+j].sel=1;	 /* 1=leakage current, 0=shaper output */
       channelstr[i*32+j].nc2=0;	 /* no connection, set 0 */
       channelstr[i*32+j].sm=1;	       /* 1=channel disable */
       channelstr[i*32+j].st=0;	       /* 1=enable test input (30fF cap) */
       }
   }
  
	Bits(0);
	return(0);
}
