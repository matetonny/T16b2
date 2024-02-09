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

ram = {}

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
        case '00000001':
            registers[instruction.er1] = registers['001']
        case '00000010':
            registers[instruction.er1] = registers['010']
        case '00000011':
            registers[instruction.er1] = registers['110']
        case '00000100':
            if instruction.reg_or_diri == '0':
                registers['000'] = registers[instruction.er2]
            else:
                registers['000'] = int(instruction.diri, 2)
        case '00000101':
            if instruction.reg_or_diri == '0':
                registers['001'] = registers[instruction.er2]
            else:
                registers['001'] = int(instruction.diri, 2)
        case '00000110':
            registers['011'] = instruction.diri 
            registers[instruction.er2] = ram[registers['011']]
        case '00000111':
            registers['011'] = instruction.diri 
            ram[registers['011']] = registers[instruction.er2]
        case '00001000':
            if instruction.reg_or_diri == '0':
                result = bin(registers[instruction.er1] + registers[instruction.er2])[2:].rjust(17, '0')
                registers['010'] = int(result[1:], 2)
                registers['110'] = int(result[0])
            else:
                result = bin(int(instruction.diri, 2) + registers[instruction.er2])[2:].rjust(17, '0')
                registers['010'] = int(result[1:], 2)
                registers['110'] = int(result[0])
        case '00001000':
            if instruction.reg_or_diri == '0':
                result = bin(registers[instruction.er1] - registers[instruction.er2]).rjust(17, '0')
                registers['010'] = int(result[2:], 2)
                registers['110'] = int(result[0])
            else:
                result = bin(int(instruction.diri, 2) - registers[instruction.er2]).rjust(17, '0')
                registers['010'] = int(result[2:], 2)
                registers['110'] = int(result[0])
                
        
def run_program(program: bytes):
    ...

cwd = os.getcwd()
boot_program_file = open(cwd + '/programs/boot.hex', 'rb')
boot_program = boot_program_file.read()

my_instruction = Instruction(b'\x00\x00\x00\x00')
my_instruction.instruction = '00001000'
my_instruction.diri = '1100000000000000'
my_instruction.er1 = '000'
my_instruction.er2 = '001'
my_instruction.reg_or_diri = '1'

registers['001'] = int('1100000000000000', 2)
run_instruction(my_instruction)
print(bin(registers['010']), bin(registers['110']))

boot_program_file.close()