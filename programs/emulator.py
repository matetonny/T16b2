import os

cwd = os.getcwd()
boot_program_file = open(cwd, 'rb')
boot_program = boot_program_file.read()

print(boot_program)

boot_program_file.close()