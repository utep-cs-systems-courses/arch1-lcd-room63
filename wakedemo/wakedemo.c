#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "sound.h"

#define LED BIT6	       

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8
#define SWITCHES 15


int period = 500;
char songT = 0;
static char switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
}

int switches = 0;

void switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}

short redrawScreen = 1;

void wdt_c_handler()
{
  static int secCount = 0;

  secCount ++;
  if (secCount >= 25) {		/* 10/sec */
    secCount = 0;
    redrawScreen = 1;
  }
}
  
void update_shape();

void main()
{
  
  P1DIR |= LED;		/* Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  buzzer_init();
 
  enableWDTInterrupts();      /* enable periodic interrupt */
  or_sr(0x8);	              /* GIE (enable interrupts) */
  u_char width = screenWidth, height = screenHeight;
  clearScreen(COLOR_BLACK);
  while (1) {
    if (redrawScreen) {
      
      drawString5x7(14,5, "First 5x7", COLOR_GREEN, COLOR_BLACK);
      
      drawString5x7(8, 120, "Second 5x7", COLOR_GREEN, COLOR_BLACK);
      
      
      redrawScreen = 0;
      update_shape();
    }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/* CPU OFF */
    P1OUT |= LED;	/* led on */
  }
}

// Color of the shape.
unsigned int SHAPE_COLOR = COLOR_DARK_VIOLE;

void update_shape()
{
  static unsigned char row = screenHeight / 2, col = screenWidth / 2;
  static int colStep = 5;
  static int rowStep = 5;
  static unsigned char step = 0;
  
  if (step <= 10) {
    int startCol = col - step*2;
    int endCol = col*2 + step*2;
    int width = 4 + endCol - startCol;

    // Shape drawing.
    // Shape_color means that it would be violet if not button is pressed
    // if pressed any of the button it would change to the respective colors
    // of the buttons.
    fillRectangle(endCol, row+1-step, width, 1, SHAPE_COLOR);
    fillRectangle(endCol, row+1+step, width, 1, SHAPE_COLOR);
    drawPixel(endCol+3, row, SHAPE_COLOR); 
    drawPixel(endCol+3, row-1, SHAPE_COLOR);
    drawPixel(endCol+3, row+1, SHAPE_COLOR);
    drawPixel(endCol+3, row-2, SHAPE_COLOR);
    drawPixel(endCol+3, row+2, SHAPE_COLOR);
    drawPixel(endCol+3, row-3, SHAPE_COLOR);
    drawPixel(endCol+3, row+3, SHAPE_COLOR);

    // draw triangles on horizontal way 
    fillRectangle(endCol+30, row*step, width, 1, COLOR_BLACK);
   
    /* Make shadow of hot pink color to the violet shape   (violet is the original color of the shape) */
    
    if (switches & SW1) { // first button
      SHAPE_COLOR = COLOR_HOT_PINK;
      buzzer_set_period(0);
    }
        /* Make shadow of gold color to the violet shape   (violet is the original color of the shape) */
    // second button.
    if (switches & SW2) {
      SHAPE_COLOR = COLOR_GOLD;
      buzzer_set_period(0);
    }
        /* Make shadow of aquamarine color to the violet shape   (violet is the original color of the shape) */
    // third button.
    if (switches & SW3) {
      SHAPE_COLOR = COLOR_AQUAMARINE;
      buzzer_set_period(0);
    }
        /* Make shadow of black color to the violet shape   (violet is the original color of the shape) */
    // four button.
    if (switches & SW4) {
      playSong();
    }
    step ++;
    
  } else {

    // move the shape to the left
    col -= colStep;
    
     clearScreen(COLOR_GOLDENROD);
     step = 0;
  }
}

/* Switch on S2 */
void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
