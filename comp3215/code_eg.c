typedef uchar unsigend char;

volatile const uchar * const inreg = (volatile const uchar * const) 0xffffff00;
volatile uchar * const outreg = (volatile uchar * const) 0xffffff01;

#define INREG_MASK ((uchar 0x02);

//...
while(!((*inreg) & INREG_MASK));
*outreg = 'a'; 
//...