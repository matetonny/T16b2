import os

registers = {
    '000': 0,
    '001': 0,
    '010': 0,
    '011': 0,
    '100': 0,
    '101': 0,
    '110': 0,
    '111': 0,
}

class Instruction:
    def __init__(self, instruction: bytearray) -> None:
        self.instruction = bin(instruction[0]).replace('0b', '').rjust(8, '0')
        diri1 = bin(instruction[1]).replace('0b', '').rjust(8, '0')
        diri2 = bin(instruction[2]).replace('0b', '').rjust(8, '0')
        self.diri = diri1 + diri2
        extra = bin(instruction[3])[2:] + '00000000000000'
        self.er1 = extra[0:3]
        self.er2 = extra[3:6]
        self.reg_or_diri = extra[6]
        
        print(self.instruction, self.diri, self.er1, self.er2, self.reg_or_diri)

def read_instruction(rom_addr, bytes):
    return bytes[rom_addr * 4:rom_addr * 4 + 4]

def run_instruction(instruction: Instruction):
    match instruction.instruction:
        case '00000000':
            registers[instruction.er1] = registers['000']

def run_program(program: bytes):
    ...

cwd = os.getcwd()
boot_program_file = open(cwd + '/programs/boot.hex', 'rb')
boot_program = boot_program_file.read()

Instruction(read_instruction(0, boot_program))

boot_program_file.close()