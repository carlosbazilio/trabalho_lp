// funcao de potencia para c
int pow(int x, int y){
    for(int i=0; i<y; i++) x *= x;
    return x;
}