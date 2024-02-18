import os

registers = {
    '000': 0,
    '001': 0,
    '010': 0,
    '011': 0,
    '100': 0,
    '101': 0,
    '110': 0,
    '111': 0
}

ram = {}
stack = []
halt = False


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

def read_instruction(rom_addr, bytes) -> None:
    return bytes[rom_addr * 4:rom_addr * 4 + 4]

def run_instruction(instruction: Instruction) -> None:
    global halt
    
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
            if instruction.reg_or_diri == '0':
                registers['011'] = instruction.er1
            else:
                registers['011'] = int(instruction.diri, 2)
                
            registers[instruction.er2] = ram[registers['011']]
        case '00000111':
            if instruction.reg_or_diri == '0':
                registers['011'] = instruction.er1
            else:
                registers['011'] = int(instruction.diri, 2)
                
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
        case '00001001':
            if instruction.reg_or_diri == '0':
                if '-' in bin(registers[instruction.er1] - registers[instruction.er2]):
                    result = bin(registers[instruction.er1] - registers[instruction.er2])[3:].rjust(16, '0')
                    registers['010'] = int(result[2:], 2)
                    registers['110'] = 1
                else:
                    result = bin(registers[instruction.er1] - registers[instruction.er2])[2:].rjust(16, '0')
                    registers['010'] = int(result[1:], 2)
                    registers['110'] = 0
            else:
                if '-' in bin(int(instruction.diri, 2) - registers[instruction.er2]):
                    result = bin(int(instruction.diri, 2) - registers[instruction.er2])[3:].rjust(16, '0')
                    registers['010'] = int(result[2:], 2)
                    registers['110'] = 1
                else:
                    result = bin(int(instruction.diri, 2) - registers[instruction.er2])[2:].rjust(16, '0')
                    registers['010'] = int(result[1:], 2)
                    registers['110'] = 0
        case '00001010':
            if instruction.reg_or_diri == '0':
                result = bin(registers[instruction.er1] * registers[instruction.er2])[2:].rjust(32, '0')
                registers['010'] = int(result[16:], 2)
                registers['110'] = int(result[:16], 2)
            else:
                result = bin(int(instruction.diri, 2) * registers[instruction.er2])[2:].rjust(32, '0')
                registers['010'] = int(result[16:], 2)
                registers['110'] = int(result[:16], 2)
        case '00001011':
            if instruction.reg_or_diri == '0':
                result = bin(registers[instruction.er1] // registers[instruction.er2])[2:].rjust(16, '0')
                registers['010'] = int(result, 2)
                registers['110'] = registers[instruction.er1] % registers[instruction.er2]
            else:
                result = bin(int(instruction.diri, 2) // registers[instruction.er2])[2:].rjust(16, '0')
                registers['010'] = int(result, 2)
                registers['110'] = int(instruction.diri, 2) % registers[instruction.er2]
        case '00001100':
            if instruction.reg_or_diri == '0':
                registers['010'] = registers[instruction.er1][2:] & registers[instruction.er2]
            else:
                registers['010'] = int(instruction.diri, 2) & registers[instruction.er2]
        case '00001101':
            if instruction.reg_or_diri == '0':
                registers['010'] = registers[instruction.er1][2:] | registers[instruction.er2]
            else:
                registers['010'] = int(instruction.diri, 2) | registers[instruction.er2]
        case '00001110':
            if instruction.reg_or_diri == '0':
                registers['010'] = ~ registers[instruction.er2]
            else:
                registers['010'] = ~ int(instruction.diri, 2)
        case '00001111':
            if instruction.reg_or_diri == '0':
                if registers[instruction.er1] < registers[instruction.er2]:
                    registers['110'] = 0
                elif registers[instruction.er1] == registers[instruction.er2]:
                    registers['110'] = 1
                else:
                    registers['110'] = 65535
            else:
                if int(instruction.diri, 2) < registers[instruction.er2]:
                    registers['110'] = 0
                elif int(instruction.diri, 2) == registers[instruction.er2]:
                    registers['110'] = 1
                else:
                    registers['110'] = 65535
        case '00010000':
            stack.append(registers['100'])
            stack.append(registers['101'])
            
            if instruction.reg_or_diri == '0':
                registers['100'] = registers[instruction.er1]
            else:
                registers['100'] = int(instruction.diri, 2)
                
            registers['101'] = registers[instruction.er2]
        
        case '00010001':
            if registers['110'] == 0:
                stack.append(registers['100'])
                stack.append(registers['101'])
                
                if instruction.reg_or_diri == '0':
                    registers['100'] = registers[instruction.er1]
                else:
                    registers['100'] = int(instruction.diri, 2)
                    
                registers['101'] = registers[instruction.er2]
        
        case '00010010':
            if registers['000'] == 0:
                stack.append(registers['100'])
                stack.append(registers['101'])
                
                if instruction.reg_or_diri == '0':
                    registers['100'] = registers[instruction.er1]
                else:
                    registers['100'] = int(instruction.diri, 2)
                    
                registers['101'] = registers[instruction.er2]
                
        case '00010011':
            if instruction.reg_or_diri == '0':
                stack.append(registers[instruction.er1])
            else:
                stack.append(int(instruction.diri, 2))
        
        case '00010100':
            registers[instruction.er1] = stack.pop()
            
        case '00010101':
            halt = True
                
def run_program(program: bytes):
    global halt
    
    program_counter = int(bin(registers['100']) + bin(registers['101']), 2)
    while not halt:
        current_instruction = Instruction(read_instruction(program_counter, program))
        run_instruction(current_instruction)
        
cwd = os.getcwd()
boot_program_file = open(cwd + '/programs/boot.hex', 'rb')
boot_program = boot_program_file.read()

run_instruction(Instruction(read_instruction(0, boot_program)))
run_instruction(Instruction(read_instruction(1, boot_program)))

boot_program_file.close()