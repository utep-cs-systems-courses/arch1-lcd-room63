# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:wakedemo.elf

#additional rules for files
wakedemo.elf: ${COMMON_OBJECTS} wakedemo.o wdt_handler.o buzzer.o sound.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 

wakedemoAssem.elf: ${COMMON_OBJECTS} wakedemo.o wdt_handler.o buzzer.o sounds.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 


load: wakedemo.elf
	msp430loader.sh $^
# assembly load.
load2: wakedemoAssem.elf
	msp430loader.sh $^


clean:
	rm -f *.o *.elf 

buzzer.o: buzzer.h

sound.o: buzzer.h sound.h

sounds.o: buzzer.h sound.h
