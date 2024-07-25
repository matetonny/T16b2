# T16b2

## DESIGN SHEET

### TERMS

diri          -> direct input
'b            -> bus
@\<reg name>   -> registers
\#\<address>    -> address in RAM
\##            -> the value of the current address in RAM
@@1           -> the first register given in the insturction
@@2           -> the second register given in the insturction
\#st           -> stack
>>#st         -> push to stack
\#st>>         -> pop stack

### RUNNING PROGRAMS

The computer only runs programs ending in .hex file extension.
When it starts it will try to run the program with the name "boot.hex" in its current dir.

### CLOCK

2048hz or 1024hz depending on the emulator and logisim
rising edge  -> fetch
falling edge -> execute

### MEMORY(RAM)

16bit address
stored in @ma

reserved spaces:
\#0x0000:#0xfffe -> free space
\#0xffff         -> output

16 banks. bank number stored in @bk

stack:
8bit stack pointer
commands -> push, pop
not alowed to change pointer

### PROGRAM MEMORY(ROM)

16bit address
each insturction is 32bits(4 bytes)

### REGISTERS

@a   -> accumulator 1                                  -> 0b000
@b   -> accumulator 2                                  -> 0b001
@ao  -> ALU output register(integrated with the ALU)   -> 0b010
@ma  -> RAM memory address                             -> 0b011
@pm  -> ROM memory address                             -> 0b100
@fl  -> flags register                                 -> 0b101
@sp  -> stack pointer(8 bits)                          -> 0b110
@bk  -> bank register(4 bits)                          -> 0b111

### ALU

can execute addition, subtraction, multiplication, division and bitwise operations and, or, not
the output is stored in @ao(integrated with the ALU)
the carry, borrow and remainder are stored in the flags register

### INSTRUCTIONS

layout:
00xxxxxx yyyyyyyy yyyyyyyy 0zzzaaab

x -> insturction  y -> direct input(diri)  z -> first enabled register(@@1)  a -> second enabled register(@@2)  b->@@/diri

insturctions:
lda   ::  x:00 y:y z:0 a:0 b:b ::  @a -> @@2 
ldb   ::  x:01 y:y z:1 a:1 b:b ::  @b -> @@2
ldo   ::  x:02 y:y z:2 a:2 b:b ::  @ao -> @@2
ldf   ::  x:03 y:y z:6 a:6 b:b ::  @fl -> @@2

sta   ::  x:04 y:y z:0 a:0 b:b ::  @@2/diri -> @a
stb   ::  x:05 y:y z:1 a:1 b:b ::  @@2/diri -> @b

ldr   ::  x:06 y:y z:4 a:a b:b ::  diri/@@1 -> @ma, ## -> @@2
str   ::  x:07 y:y z:4 a:a b:b ::  diri/@@1 -> @ma, @@2 -> ##

add   ::  x:08 y:y z:z a:a b:b ::  @@1/diri + @@2 -> @ao
sub   ::  x:09 y:y z:z a:a b:b ::  @@1/diri - @@2 -> @ao
mul   ::  x:0a y:y z:z a:a b:b ::  @@1/diri * @@2 -> @ao
div   ::  x:0b y:y z:z a:a b:b ::  @@1/diri / @@2 -> @ao
and   ::  x:0c y:y z:z a:a b:b ::  @@1/diri && @@2 -> @ao
or    ::  x:0d y:y z:z a:a b:b ::  @@1/diri || @@2 -> @ao
not   ::  x:0e y:y z:z a:a b:b ::  !@@1/!diri -> @ao
cmp   ::  x:0f y:y z:z a:a b:b ::  @@1/diri<@@2{0x0000 -> @fl}, @@1/diri==@@2{0x0001 -> @fl}, @@1/diri>@@2{0xffff -> @fl}

jmp   ::  x:10 y:y z:z a:a b:b ::  @pm >>#st, @@1/diri -> @pm 
jpfz  ::  x:11 y:y z:z a:a b:b ::  @fl==0x0000{@pm >>#st, @@1/diri -> @pm }
jpff  ::  x:12 y:y z:4 a:5 b:b ::  @fl==0xffff{@pm >>#st, @@1/diri -> @pm }

push  ::  x:13 y:y z:z a:a b:b ::  @@1 >>#st, sp ++
pop   ::  x:14 y:y z:0 a:a b:b ::  #st>> @@1, @sp --

hlt   ::  x:15 y:y z:z a:a b:b ::  halt

\## ASSEMBLY LANGUAGE FOR T16b2

\### INSTRUCTIONS

name  :: function                                                                                           :: args

lda   :: load @a into another register(@@2)                                                                 :: @@2
ldb   :: load @b into another register(@@2)                                                                 :: @@2
ldo   :: load @ao into another register(@@2)                                                                :: @@2
ldf   :: load @fl into another register(@@2)                                                                :: @@2

sta   :: store a word(diri) or another register(@@2) in @a                                                  :: diri/@@2
stb   :: store a word(diri) or another register(@@2) in @b                                                  :: diri/@@2

ldr   :: load a word from an address(diri/@@1) in RAM into a register(@@2)                                  :: diri/@@1, @@2
str   :: store a word in an address(diri/@@1) in RAM from a register(@@2)                                   :: diri/@@1, @@2

add   :: add a word/register(diri/@@1) and another register(@@2) and put it into @ao                        :: diri/@@1, @@2
sub   :: subtract a word/register(diri/@@1) and another register(@@2) and put it into @ao                   :: diri/@@1, @@2
mul   :: multiply a word/register(diri/@@1) and another register(@@2) and put it into @ao                   :: diri/@@1, @@2
div   :: divide a word/register(diri/@@1) and another register(@@2) and put it into @ao                     :: diri/@@1, @@2
and   :: bitwise and a word/register(diri/@@1) and another register(@@2) and put it into @ao                :: diri/@@1, @@2
or    :: bitwise or a word/register(diri/@@1) and another register(@@2) and put it into @ao                 :: diri/@@1, @@2
not   :: bitwise not a word/register(diri/@@1) and put it into @ao                                          :: diri/@@1, @@2
cmp   :: compare a word/register(diri/@@1) and another register(@@2) and put th result into @fl             :: diri/@@1, @@2

jmp   :: jump to an address(diri/@@1) in program memory and push curret adress to the stack                 :: diri/@@1, @@2
jpfz  :: if @fl==0x0000: jump to an address(diri/@@1) in program memory and push curret adress to the stack :: diri/@@1, @@2
jpff  :: if @fl==0xffff: jump to an address(diri/@@1) in program memory and push curret adress to the stack :: diri/@@1, @@2

push  :: push a register onto the stack(@@1)                                                                :: @@1
pop   :: pop the stack and store it to a register(@@1)                                                      :: @@1

hlt   :: stop the program                                                                                   :: n/a

---TOKEN TYPES---
instruction :: f
integer     :: i
register    :: r
memory addr :: m

---FLAGS---
-d :: debug
