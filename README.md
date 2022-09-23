## 3 - Coletor automático de lixo em C por contagem de referências

*malloc2(tamanho)*: realiza a alocação dinâmica, insere ponteiro para a área criada no contador e retorna o endereço.

*atrib2(endereco1,endereco2)*: atribui ao ponteiro de *endereco1* o endereço de *endereco2*, incrementa o contador e libera espaço de memória caso a contagem atinja zero.

*dump()*: imprime a situação da memória e o contador. 
