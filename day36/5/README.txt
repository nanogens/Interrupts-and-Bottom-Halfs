test_hello.c has been verified against the Odroid-XU4 board.

Procedure detailed in our note book.
Will replicate it here when i have time.

Note we use #18 pin (it is NOT the 18th pin, but rather is a pin name) on the Odroid-XU4 board + Shield.


Note:
This test_hello.c is an addition to the one found in day36/4.
It checks the context (Lec64) for Hard and Soft IRQ.
Conclusion is both Hard and Soft IRQ take place in Interrupt Context not Process Context.
This makes sense since these are interrupts handled in the kernel (low level).
