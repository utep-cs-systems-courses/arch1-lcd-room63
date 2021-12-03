#include <msp430.h>
#include "buzzer.h"
#include "sound.h"

void playSong()
{
switch (songT) {
 case 0:
 period = 250;
 songT = 1;
 break;
 
 case 1:
 period = 500;
 songT = 2;
 break;
 
 case 2:
 period = 750;
 songT = 0;
 break;
 default:
 period = 1000;
 songT = 1;
 }
buzzer_set_period(period);
}
