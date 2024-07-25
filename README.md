# T16b2

## DESIGN SHEET

### TERMS

diri          -> direct input\
'b            -> bus\
@             -> registers\
\#             -> address in RAM\
\##            -> the value of the current address in RAM\
@@1           -> the first register given in the insturction\
@@2           -> the second register given in the insturction\
\#st           -> stack\
\>>#st         -> push to stack\
\#st>>         -> pop stack

### RUNNING PROGRAMS

The computer only runs programs ending in .hex file extension.\
When it starts it will try to run the program with the name "boot.hex" in its current directory.

### CLOCK

2048hz or 1024hz depending on the emulator and logisim\
rising edge  -> fetch\
falling edge -> execute

### MEMORY(RAM)

16bit address\
stored in @ma\
\
reserved spaces:\
\#0x0000:#0xfffe -> free space\
\#0xffff         -> output\
\
16 banks. bank number stored in @bk\
\
stack:\
8bit stack pointer\
commands -> push, pop\
not alowed to change pointer\

### PROGRAM MEMORY(ROM)

16bit address\
each insturction is 32bits(4 bytes)\

### REGISTERS

@a   -> accumulator 1                                  -> 0b000\
@b   -> accumulator 2                                  -> 0b001\
@ao  -> ALU output register(integrated with the ALU)   -> 0b010\
@ma  -> RAM memory address                             -> 0b011\
@pm  -> ROM memory address                             -> 0b100\
@fl  -> flags register                                 -> 0b101\
@sp  -> stack pointer(8 bits)                          -> 0b110\
@bk  -> bank register(4 bits)                          -> 0b111

### ALU

can execute addition, subtraction, multiplication, division and bitwise operations and, or, not\
the output is stored in @ao(integrated with the ALU)\
the carry, borrow and remainder are stored in the flags register\

### INSTRUCTIONS

layout:\
00xxxxxx yyyyyyyy yyyyyyyy 0zzzaaab\
\
x -> insturction  y -> direct input(diri)  z -> first enabled register(@@1)  a -> second enabled register(@@2)  b->@@/diri\
\
insturctions:\
lda  ::  @a -> @@2\
ldb  ::  @b -> @@2\
ldo  ::  @ao -> @@2\
ldf  ::  @fl -> @@2\
\
sta :: @@2/diri -> @a\
stb :: @@2/diri -> @b\
\
ldr  ::  diri/@@1 -> @ma, ## -> @@2\
str  ::  diri/@@1 -> @ma, @@2 -> ##\
\
add  ::  @@1/diri + @@2 -> @ao\
sub  ::  @@1/diri - @@2 -> @ao\
mul  ::  @@1/diri * @@2 -> @ao\
div  ::  @@1/diri / @@2 -> @ao\
and  ::  @@1/diri && @@2 -> @ao\
or   ::  @@1/diri || @@2 -> @ao\
not  ::  !@@1/!diri -> @ao\
cmp  ::  @@1/diri<@@2{0x0000 -> @fl}, @@1/diri==@@2{0x0001 -> @fl}, @@1/diri>@@2{0xffff -> @fl}\
\
jmp  ::  @pm >>#st, @@1/diri -> @pm\
jpfz  ::  @fl==0x0000{@pm >>#st, @@1/diri -> @pm }\
jpff  ::  @fl==0xffff{@pm >>#st, @@1/diri -> @pm }\
\
pus  ::  @@1 >>#st, sp ++\
pop  ::  #st>> @@1, @sp --\
\
hlt  ::  halt\

### TOKEN TYPES

instruction :: f\
integer     :: i\
register    :: r\
memory addr :: m\

### FLAGS

-d :: debug\
