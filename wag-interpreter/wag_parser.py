# Type the file name and receive it parsed by token
def parse_code(input_file: str):
    # Leitura do arquivo a ser interpretado
    with open(input_file, "r") as code:
        code = code.read()
        code = code.split('\n')

    # Processamento do codigo
    commands = []

    for line in code:
        tokens = []
        temp_str = ""
        for char in line:
            temp_str += char
            if temp_str == "var":
                tokens += [temp_str]
                temp_str = ""
            if temp_str == " ":
                temp_str = ""
            if temp_str == "func":
                tokens += [temp_str]
                temp_str = ""
            if temp_str == "print":
                tokens += [temp_str]
                temp_str = ""
        tokens += temp_str
        for i in tokens:
            if i == " ":
                tokens.remove(i)
        commands.append(tokens)
        
    return commands