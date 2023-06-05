import sys

# Funcoes do codigo
def declare_variable(name: str, escopo: dict):
    escopo.setdefault(name, "Nao definido.")

# Esta funcao retorna um tuple (nome_var, valor, escopo)
def find_var(name: str, escopo: list):
    for i in escopo.__reversed__():
        value = i.get(name)
        if value == None: continue
        return name, value, i
    return None, None, None

def command_print(name: str, escopo: list):
    name = name.split(",")
    for var in name:
        var = find_var(var, escopo)
        print(var[1])

def expression(expr: list):
    acc = 0
    for i in expr:
        try:
            acc += int(i)
        except:
            if i == "+": continue
            var = find_var(i, escopo)
            acc += var[1]
    return acc

def atrib(name: str, expr: list, escopo: list):
    var = find_var(name, escopo)

    if var[0] != None:
        var[2][var[0]] = expression(expr)

def declare_function(cmd: str, escopo: list):
    name = cmd[0]
    var = find_var(name, escopo)

    # Body
    inputs = []
    exprs = []

    for i in cmd:
        parenthesis = False
        if i == "(" or i == ")":
            parenthesis = not parenthesis
        if parenthesis:
            inputs += i


    if var[0] != None:
        var[2][var[0]] = lambda x: 2+2

# Leitura do arquivo a ser interpretado
try:
    input_file = sys.argv[1]
except:
    exit("Nenhum arquivo inserido.")
with open(input_file, "r") as code:
    code = code.readlines()

# Escopos e armazenamento de dados do programa
escopo = [{}]

# Processamento do codigo
reading_func = False
for line in code:
    # Pre processamento das instrucoes
    line = line.replace("\n", "")
    line = line.split(" ")

    # Processamento da instrucao dada
    if reading_func == False:
        cmd = ""

    if line[0] == "func" or reading_func:
        # print(line)
        for i in line:
            if i != "func":
                for j in i:
                    cmd += j

        if line[-1][-1] == "{":
            reading_func = True
        if line[-1][-1] == "}":
            reading_func = False
            declare_variable(cmd[0], escopo[-1])
            declare_function(cmd, escopo)
        continue

    if line[0] == "var": declare_variable(line[1], escopo[-1])

    if line[0] == "print": command_print(line[1], escopo)

    if line[1] == "=": atrib(line[0], line[2:], escopo)


    # Log
    # print(line)