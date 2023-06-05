import sys

file_name = sys.argv[1]
usr_file = open(file_name, "r")
out_file = open(f'{file_name.removesuffix(".c")}-preproc.c', "w")

content = usr_file.readlines()

def remove_comments(line):
    aux = ''
    try:
        comment_beginning = line.index("//")
        i = 0
        while i < comment_beginning and i < len(line):
            aux += line[i]
            i += 1
        return aux
    except:
        aux = line
        return aux

def remove_endlines(line):
    aux = ''
    try:
        line.index("#include")
        return line
    except:
        aux = line.removesuffix("\n")
        return aux
    
def expand_lib(line):
    if line[:8] == "#include":
        lib_name = line.split(" ")[1]
        lib_name = lib_name.replace('"', '')
        lib_name = lib_name.replace('\n', '')

        try:
            with open(lib_name, "r") as lib:
                lib = lib.read()
                return lib
        except:
            exit("Biblioteca nao encontrada.")
    return line

def is_variable(i, aux):
    if (i == ' ' and aux[-3:] == "int"):
        return True
    elif (i == ' ' and aux[-5:] == "float"):
        return True
    elif (i == ' ' and aux[-4:] == "char"):
        return True
    elif (i == ' ' and aux[-6:] == "return"):
        return True
    else:
        return False

def remove_spaces(line):
    aux = ''
    allowed_space = False
    for i in line:
        if line[0] == '#':
            aux += i
            continue
        if is_variable(i, aux):
            aux += i
            continue
        if (i == r'"' or i == r"'") and allowed_space == False:
            allowed_space = True
        elif (i == "\"" or i == "\'" and allowed_space == True):
            allowed_space = False
        elif i == ' ' and allowed_space == False:
            continue            
        aux += i
    return aux

for line in content:
    if line == '\n':
        continue
    line = remove_comments(line)
    line = remove_spaces(line)
    line = remove_endlines(line)
    line = expand_lib(line)
    out_file.write(line)

usr_file.close()
out_file.close()