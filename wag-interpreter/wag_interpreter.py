from wag_parser import parse_code
from sys import argv

code = parse_code(argv[1])

# Escopo e gerenciamento
escopo = [{}]
line_counter = 0
main = False

# Funcoes de processamento
def declare_var(name: str, escopo: dict):
    escopo[name] = "Vazio"

def declare_func(name: str, line: int, escopo: dict):
    escopo[name] = line

def func_print(list_var):
    for i in list_var:
        for j in escopo.__reversed__():
            try:
                print(j[i], end=" ")
                break
            except:
                continue
    print()

def call_func(name: str, list_values: list):
    print(list_values)
    func_id = escopo[0][name]
    escopo.append({})
    local = escopo[-1]
    for char in code[func_id][3:]:
        if char == ")":
            break
        # if != ",":
        #     local[char] = 

for line in code:
    if line[0] == 'var':
        declare_var(line[1], escopo[-1])
    elif line[0] == 'func':
        tempstr = ""
        tempstr = tempstr.join(line[1:5])
        if tempstr == "main":
            main = True

        declare_func(line[1], line_counter, escopo[-1])
    elif line[0] == 'print':
        func_print(line[1:])
    elif line[0].isalpha() and main:
        if line[1] == "(":
            templist = []
            for value in line[2:]:
                if value != ',':
                    templist += [value]
            call_func(line[0], templist)
        if line[1] == "=":
            print("atrib")
    line_counter += 1
    
print(escopo)