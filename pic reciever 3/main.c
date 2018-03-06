#include <16F877A.h>
#use delay(clock = 4000000)
#use rs232 (uart1)
#use fast_io(b)



void main() {
setup_uart(9600);
set_tris_b(0xff);
int A = 0;
int B = 0;
while (input_B())
{
A++;
delay_ms(1);
}
while (!input_B())
{
B++;
delay_ms(1);
}




while(1) { // Foreground loop


printf("so a la: %d  ", A);
printf("so B la: %d  ", B);

delay_ms(1000);
}
}
