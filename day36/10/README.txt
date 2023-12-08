test_hello.c has been verified against the Odroid-XU4 board.

Procedure detailed in our note book.
Will replicate it here when i have time.

Note we use #18 pin (it is NOT the 18th pin, but rather is a pin name) on the Odroid-XU4 board + Shield.


Note:
This test_hello.c is an addition to the one found in day36/7.  (Lec 65)
It introduces a delay in the Soft IRQ of 4 secs.  The Soft IRQ is triggered by the Hard IRQ after it fires.

The conclusion is that the Hard IRQs will be ignored until the Soft IRQ has completed processing the initial 
interrupt raised by the Hard IRQ.

Also, note how :

sudo /proc/interrupts  (can be used to see how many times the hard irq has fired)

and :

sudo /proc/softirq  (can be used to see how many times the soft irq has fired)
