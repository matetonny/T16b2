from sys import argv, exit

instruction_set = [
    'lda',
    'ldb',
    'ldo',
    'ldf',
    'sta',
    'stb',
    'ldr',
    'str',
    'add',
    'sub',
    'mul',
    'div',
    'and',
    'or',
    'not',
    'comp'
]

with open(argv[1], 'r') as f:
    raw_code = f.readlines()
    file_name = argv[1].split('/n')[-1].split('.')[0]
