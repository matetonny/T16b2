from sys import *

def lexer(contents):
    lines = contents.split('\n')
    
    for line in lines:
        chars = list(line)
        
        tokens = []
        temp_str = ''
        qoute_count = 0
        for char in chars:
            if char == '"':
                qoute_count += 1
            if qoute_count % 2 == 0:
                in_qoutes = False
            else:
                in_qoutes = True
                
            if char == ' ' and not in_qoutes:
                tokens.append(temp_str)
                temp_str = ''
            else:
                temp_str += char
        tokens.append(temp_str)
                
        print(tokens)

def parse(file):
    contents = open(file, 'r').read()
    tokens = lexer(contents)
    return tokens

if __name__ == '__main__':
    print(parse(argv[1]))