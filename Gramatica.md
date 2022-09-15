<armazem> ::= <instrucao>+
<instrucao> ::= <inicia_armazem> | <new> | <delete> | <lista> | <atribui>
<inicia_armazem> ::='heap' {'first | 'best |'worst |'next }
<new> ::= <id> <number>
<del> ::= 'del <id>
<lista> ::= 'lista
<atribui> ::= atribui <id> <id>