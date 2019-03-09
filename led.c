#define LEDCON (*(volatile unsigned long *)0x110002e0)
#define LEDDAT (*(volatile unsigned long *)0x110002e4)

#define KEYCON (*(volatile unsigned long *)0x11000c60)
#define KEYDAT (*(volatile unsigned long *)0x11000c64)

#define BEEPCON (*(volatile unsigned long *)0x114000A0)
#define BEEPDAT (*(volatile unsigned long *)0x114000A4)

#define BEEP_ON BEEPDAT |= 0x01
#define BEEP_OFF BEEPDAT &= 0xFE

int main(void)
{
	LEDCON &= 0xffff0000;
	LEDCON |= 0x00001111;
	LEDDAT |= 0x0f;
	KEYCON &= 0xff0000ff;
	BEEPCON &= 0xFFFFFFF0;
    BEEPCON |= 0x00000001;
    BEEPDAT &= 0xFE;
	while(1)
	{
        if((KEYDAT & (0x04)) == 0)
        {
            BEEP_ON;
            LEDDAT &= 0xfe;
        }
        if((KEYDAT & (0x08)) == 0)
        {
            BEEP_OFF;
            LEDDAT &= 0xfd;
        }
	// if((KEYDAT & (0x10)) == 0)
	// {
	// 	LEDDAT &= 0xfb;
	// }
	// if((KEYDAT & (0x20)) == 0)
	// {
	// 	LEDDAT &= 0xf7;
	// }
	}
	return 0;
}

