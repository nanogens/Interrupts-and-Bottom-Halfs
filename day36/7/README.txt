test_hello.c has been verified against the Odroid-XU4 board.

Procedure detailed in our note book.
Will replicate it here when i have time.

Note we use #18 pin (it is NOT the 18th pin, but rather is a pin name) on the Odroid-XU4 board + Shield.


Note:
This test_hello.c is an addition to the one found in day36/5.  (Lec 65)
It checks when the IRQ is Enabled / Disabled during Hard IRQ -> Soft IRQ process.
Basically the interrupt is disabled in Hard IRQ and enabled (at the end of?) Soft IRQ -- which makes sense.
