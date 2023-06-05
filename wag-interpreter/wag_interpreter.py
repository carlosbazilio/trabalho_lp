from wag_parser import parse_code
from sys import argv

code = parse_code(argv[1])

# Escopo e gerenciamento
escopo = [{}]
line_counter = 0
main = False

# Funcoes de processamento
def declare_var(name: str, escopo: dict):
    exec(f"{name}=0", None, escopo)

def declare_func(command: list, escopo: dict):
    tempstr = ""
    name = ""
    proc = False
    for i in command:
        for j in i:
            if j == "(" and not name:
                name = tempstr
            if j == "func":
                continue
            if j == "{":
                tempstr += ":"
                proc = True
                continue
            if j == "}":
                continue
            if proc and j.isalpha():
                tempstr += fr'escopo[-1][{j}]'
                continue
            tempstr += j
        tempstr += "\n\t"
    command = "def " + tempstr
    print(command)
    exec(command, None, escopo)

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
    # func_id = escopo[0][name]
    # escopo.append({})
    # local = escopo[-1]
    # for char in code[func_id][3:]:
    #     if char == ")":
    #         break
        # if != ",":
        #     local[char] = 


reading_func = False
tempstr = []
for line in code:
    if line[0] == 'var':
        declare_var(line[1], escopo[-1])
    elif line[0] == 'func' or reading_func:
        reading_func = True
        tempstr += [line]
        if line[1:5] == list("main"):
            main = True
            reading_func = False
            continue
        if line[-1] == "}":
            reading_func = False
            declare_func(tempstr, escopo[-1])
            tempstr = []

    elif line[0] == 'print':
        func_print(line[1:])
    elif line[0].isalpha() and main:
        if line[1] == "(":
            print("call func")
            # templist = []
            # for value in line[2:]:
            #     if value != ',':
            #         templist += [value]
            # call_func(line[0], templist)
        if line[1] == "=":
            print("atrib")
    line_counter += 1

print(escopo)
escopo[0]['g'](2,3)
print(escopo)