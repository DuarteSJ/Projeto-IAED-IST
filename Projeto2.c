/*
* Ficheiro: Projeto2.c
* Autor: Duarte Rebelo de Sao Jose
* Numero: 103708
* Descricao: Sistema de gestao de voos e reservas entre aeroportos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN_IDAEROPORTO 4           /*numero maximo de caracteres por ID de
                                        aeroporto*/
#define PAIS_MAX_LEN 31             /*numero maximo de caracteres por pais*/
#define CIDADE_MAX_LEN 51           /*numero maximo de caracteres por cidade*/
#define AEROPORTOS_MAX 40           /*numero maximo de aeroportos*/
#define VOOS_MAX 30000              /*numero maximo de voos*/
#define DURACAO_VOO_MAX 12          /*duracao maxima de um voo*/
#define PASSAGEIROS_MIN 10          /*numero minimo de passageiros*/
#define CODIGO_VOO_LEN 7            /*numero maximo de caracteres para o codigo
                                        do voo*/
#define DATA_INICIAL {1, 1, 2022}   /* Data inicial do programa*/
#define DIAS_31 31                  /*mumero de dias dos meses com mais dias*/
#define DIAS_30 30                  /*mumero de dias dos meses com menos dias*/
#define DIAS_FEV 28                 /*mumero de dias de fevereiro*/
#define AEROPORTO "airport"         /*aeroporto*/
#define CODIGO_VOO_INVALIDO "invalid flight code\n"
/*erro de codigo de voo invalido*/
#define VOO_DUPLICADO "flight already exists\n"
/*erro de voo repetido*/
#define ID_NAO_EXISTE ": no such airport ID\n"
/*erro de ID de aeroporto nao existente*/
#define DEMASIADOS_VOOS "too many flights\n"
/*erro de excesso de voos*/
#define DATA_INVALIDA "invalid date\n"
/*erro de data invalida*/
#define DURACAO_INVALIDA "invalid duration\n"
/*erro de duracao de voo invalida*/
#define CAPACIDADE_INVALIDA "invalid capacity\n"
/*erro de capacidade de voo invalida*/
#define ID_INVALIDO "invalid airport ID\n"
/*erro de ID de aeroporto invalido*/
#define DEMASIADOS_AEROPORTOS "too many airports\n"
/*erro de excesso de aeroportos*/
#define AEROPORTO_DUPLICADO "duplicate airport\n"
/*erro de aeroporto repetido*/
#define CODIGO_RESERVA_INVALIDO "invalid reservation code\n"
/*erro de codigo de reserva invalido*/
#define VOO_NAO_EXISTE "flight does not exist\n"
/*erro de voo nao existente*/
#define CODIGO_RESERVA_REPETIDO "flight reservation already used\n"
/*erro de codigo de reserva repetido*/
#define DEMASIADAS_RESERVAS "too many reservations\n"
/*erro de excesso de reservas*/
#define NUMERO_PASSAGEIROS_INVALIDO "invalid passenger number\n"
/*erro de numero invalido de passageiros*/
#define CODIGO_NAO_ENCONTRADO "not found\n"
/*erro de codigo nao encontrado*/
#define MEMORIA_CHEIA "No memory\n"
/*erro de memoria cheia*/
#define MAX_CARACTERES_INSTRUCAO 65535
/*#define PASSAGEIROS_MAX 100 numero maximo de passageiros (deixou de existir)*/
#define COD_AEROPORTO_MAX 6
#define COD_AEROPORTO_MIN 4

/*......................Estruturas......................*/

/*Estrutura correspondente aos aeroportos*/
typedef struct{
    char IDAeroporto[LEN_IDAEROPORTO], pais[PAIS_MAX_LEN],
    cidade[CIDADE_MAX_LEN];
}Aeroporto;

/*Estrutura correspondente as datas*/
typedef struct{
    int dia, mes, ano;
}Data;

/*Estrutura correspondente as horas e minutos*/
typedef struct{
    int hora, min;
}Horario;

/*Estrutura correspondente aos voos*/
typedef struct{
    int capacidade;
    Data dataPartida, dataChegada;
    Horario horaPartida, horaChegada, duracao;
    char IDPartida[LEN_IDAEROPORTO],
    IDChegada[LEN_IDAEROPORTO], codigoVoo[CODIGO_VOO_LEN];
}Voo;

/*Estrutura correspondente as reservas*/
typedef struct Reserva{
    Data data;
    int num_passageiros;
    char codigo_voo[CODIGO_VOO_LEN], *codigo_reserva;
    struct Reserva* next;
}Reserva;



    /*......................Variaveis globais......................*/


Voo voos[VOOS_MAX];  /*inicializacao do vetor que contem todos os voos*/




/*------------------Funcoes adicionadas no primeiro projeto------------------*/


    /*......................funcoes auxiliares......................*/



/*Verifica se o caracter recebido e um digito*/
int eh_digito(char c){
    if (c <= '9' && c >='0'){
        return 1;
    }
    return 0;
}


/*Verifica se o caracter recebido e uma maiuscula*/
int eh_maiuscula(char c){
    if (c <= 'Z' && c >= 'A'){
        return 1;
    }
    return 0;
}

/*Verifica se o caracter recebido e uma maiuscula*/
int eh_minuscula(char c){
    if (c <= 'z' && c >= 'a'){
        return 1;
    }
    return 0;
}


/*Retorna o aeroporto ao qual corresponde o ID ou, no caso de nao existir nenhum
aeroporto com o ID recebido, cria um novo aeroporto com ID '...' e retorna-o*/
Aeroporto encontrar_aeroporto(char ID[], Aeroporto aeroportos[],
                              int numero_de_aeroportos){

    Aeroporto aeroporto;
    int i;
    
    strcpy(aeroporto.IDAeroporto, "..."); 
    /*Se o ID for '...', o aeroporto nao existe*/

    for (i = 0; i < numero_de_aeroportos; i++){
        if (!strcmp(aeroportos[i].IDAeroporto, ID)){
            return aeroportos[i];
        }
    }
    return aeroporto;
}


/*Recebe o dia, mes e ano novos que deverao corresponder a uma nova data
e a data atual. No caso de a nova data nao ser valida, a funcao devolve 0,
caso contrario, a funcao devolve 1*/
int data_valida(int dia_novo, int mes_novo, int ano_novo, Data data){
    if ((ano_novo < data.ano) || (ano_novo == data.ano && mes_novo < data.mes)
    || (ano_novo == data.ano && mes_novo == data.mes && dia_novo < data.dia)
    || (ano_novo - data.ano > 1) || (ano_novo - data.ano == 1 &&
                                                            mes_novo > data.mes)
    || (ano_novo - data.ano == 1 && mes_novo == data.mes &&
                                                          dia_novo > data.dia)){
        return 0;
    }
    else{
        return 1;
    }
}


/*Recebe duas datas e compara-as cronologicamente, devolvendo '>', '=', ou '<'*/
char comparar_datas(Data data1, Data data2){

    if ((data1.ano > data2.ano) ||
    ((data1.ano == data2.ano) && (data1.mes > data2.mes)) ||
    ((data1.ano == data2.ano) && (data1.mes == data2.mes) &&
    (data1.dia > data2.dia))){
        return '>';
    }
    else if ((data1.ano == data2.ano) && (data1.mes == data2.mes) &&
    (data1.dia == data2.dia)){
        return '=';
    }
    else{
        return '<';
    }
}


/*Recebe duas horas e compara-as cronologicamente, devolvendo '>', '=' ou '<'.*/
char comparar_horas(Horario hora1, Horario hora2){

    if ((hora1.hora > hora2.hora) ||
    ((hora1.hora == hora2.hora) && (hora1.min > hora2.min))){
        return '>';
    }
    else if ((hora1.hora == hora2.hora) && (hora1.min == hora2.min)){
        return '=';
    }
    else{
        return '<';
    }
}


/*Recebe uma lista de voos e organiza-a por ordem cronologica de partidas*/
void ordenar_voos_partida(Voo l_voos[], int len){

    int i, j, index_menor = 0;
    Data dataPartida_min = {0, 0, 0};
    Horario horaPartida_min = {0, 0};
    Voo menor, temp;
    for(i = 0; i < len; i++){
        for (j = i; j < len; j++){
            if (j == i){
                menor = l_voos[j];
                index_menor = j;
                dataPartida_min = l_voos[j].dataPartida;
                horaPartida_min = l_voos[j].horaPartida;
            }
            if ((comparar_datas(l_voos[j].dataPartida, dataPartida_min)=='<')
            || ((comparar_datas(l_voos[j].dataPartida, dataPartida_min)=='=')
            && (comparar_horas(l_voos[j].horaPartida, horaPartida_min)=='<'))){
                menor = l_voos[j];
                index_menor = j;
                dataPartida_min = l_voos[j].dataPartida;
                horaPartida_min = l_voos[j].horaPartida;
            }
        }
        temp = l_voos[i];
        l_voos[i] = menor;
        l_voos[index_menor] = temp;
    }
}


/*Recebe uma lista de voos e organiza-a por ordem cronologica de chegadas*/
void ordenar_voos_chegada(Voo l_voos[], int len){
    int i, j, index_menor = 0;
    Data dataChegada_min = {0, 0, 0};
    Horario horaChegada_min = {0, 0};
    Voo menor, temp;
    for(i = 0; i < len; i++){
        for (j = i; j < len; j++){
            if (j == i){
                menor = l_voos[j];
                index_menor = j;
                dataChegada_min = l_voos[j].dataChegada;
                horaChegada_min = l_voos[j].horaChegada;
            }
            if ((comparar_datas(l_voos[j].dataChegada, dataChegada_min)=='<')
            || ((comparar_datas(l_voos[j].dataChegada, dataChegada_min)=='=')
            && (comparar_horas(l_voos[j].horaChegada, horaChegada_min)=='<'))){
                menor = l_voos[j];
                index_menor = j;
                dataChegada_min = l_voos[j].dataChegada;
                horaChegada_min = l_voos[j].horaChegada;
            }
        }
        temp = l_voos[i];
        l_voos[i] = menor;
        l_voos[index_menor] = temp;
    }
}


/*Recebe uma lista de aeroportos e organiza-a por ordem alfabetica de IDs*/
void ordenar_aeroportos(Aeroporto aeroportos[], int numero_de_aeroportos){

    int i, j, x, trocou;
    Aeroporto temporario;

    for (x = numero_de_aeroportos; x > 0; x--){
        trocou = 0;
        for (i = 0, j = 1; j < x; i++, j++){
            if(strcmp(aeroportos[i].IDAeroporto,aeroportos[j].IDAeroporto) > 0){
                temporario = aeroportos[i];
                aeroportos[i] = aeroportos[j];
                aeroportos[j] = temporario;
                trocou = 1;
            }
        }
        if (!trocou){
            break;
        }
    }
}


/*Devolve o numero de dias do mes recebido*/
int numero_de_dias(int mes){
    int n_dias_mes;
     if((mes == 1) || (mes == 3) || (mes == 5) || (mes == 7) || (mes == 8)
     || (mes == 10) || (mes == 12)){
        n_dias_mes = DIAS_31;
    }
    else if (mes == 2){
        n_dias_mes = DIAS_FEV;
    }
    else{
        n_dias_mes = DIAS_30;
    }
    return n_dias_mes;
}


/*Verifica se o voo e valido*/
int validar_voo(Voo voo, int numero_de_voos, Aeroporto aeroportos[], Data data,
                int numero_de_aeroportos){

    int i, len_codigo, count;
    Aeroporto aeroportoP, aeroportoC;
    len_codigo = strlen(voo.codigoVoo);
    
    aeroportoP = encontrar_aeroporto(voo.IDPartida, aeroportos,
                                    numero_de_aeroportos);
    aeroportoC = encontrar_aeroporto(voo.IDChegada, aeroportos,
                                    numero_de_aeroportos);

    if ((len_codigo > 6) || (!eh_maiuscula(voo.codigoVoo[0])
    || !eh_maiuscula(voo.codigoVoo[0]))
    || ((!eh_digito(voo.codigoVoo[2])) || (voo.codigoVoo[2] == '0'))){
        printf("%s", CODIGO_VOO_INVALIDO);
        return 0;
    }

    for (i = 3; i < len_codigo; i++){
        if (!eh_digito(voo.codigoVoo[i])){
            printf("%s", CODIGO_VOO_INVALIDO);
            return 0;
        }
    }

    for (count = 0; count < numero_de_voos; count++){
        if (!strcmp(voos[count].codigoVoo, voo.codigoVoo)){
            if ((voos[count].dataPartida.ano == voo.dataPartida.ano) &&
            (voos[count].dataPartida.mes == voo.dataPartida.mes) &&
            (voos[count].dataPartida.dia == voo.dataPartida.dia)){
                printf("%s", VOO_DUPLICADO);
                return 0;
            }
        }
    }
    if (!strcmp(aeroportoP.IDAeroporto, "...")){
        printf("%s%s", voo.IDPartida, ID_NAO_EXISTE);
        return 0;
    }
    else if (!strcmp(aeroportoC.IDAeroporto, "...")){
        printf("%s%s", voo.IDChegada, ID_NAO_EXISTE);
        return 0;
    }

    if (numero_de_voos >= VOOS_MAX){
        printf("%s", DEMASIADOS_VOOS);
        return 0;
    }

    if(!data_valida(voo.dataPartida.dia, voo.dataPartida.mes,
    voo.dataPartida.ano, data)){
        printf("%s", DATA_INVALIDA);
        return 0;
    }
    if ((voo.duracao.hora > DURACAO_VOO_MAX) ||
    ((voo.duracao.hora == DURACAO_VOO_MAX) && (voo.duracao.min > 0))){
        printf("%s", DURACAO_INVALIDA);
        return 0;
    }
    if (voo.capacidade < PASSAGEIROS_MIN){
        printf("%s", CAPACIDADE_INVALIDA);
        return 0;
    }
    return 1;
}


/*Calcula a data de chegada do voo com base na sua hora de partida e duracao*/
Voo calcular_data_e_hora_chegada(Voo voo){
    int n_dias_mes;

    n_dias_mes = numero_de_dias(voo.dataPartida.mes);

    voo.dataChegada.dia = voo.dataPartida.dia;
    voo.dataChegada.mes = voo.dataPartida.mes;
    voo.dataChegada.ano = voo.dataPartida.ano;

    voo.horaChegada.hora = voo.horaPartida.hora + voo.duracao.hora;
    voo.horaChegada.min = voo.horaPartida.min + voo.duracao.min;

    if (voo.horaChegada.min > 59){
        voo.horaChegada.min -= 60;
        voo.horaChegada.hora += 1;
    }
    if (voo.horaChegada.hora > 23){
        voo.horaChegada.hora -= 24;
        voo.dataChegada.dia += 1;
        if (voo.dataChegada.dia > n_dias_mes){
            voo.dataChegada.dia -= n_dias_mes;
            voo.dataChegada.mes += 1;
            if (voo.dataChegada.mes > 12){
                voo.dataChegada.mes -= 12;
                voo.dataChegada.ano += 1;
            }
        }
    }
    return voo;
}


    /*......................Funcoes principais......................*/



/*Cria um novo aeroporto com base no que o utilizador escreve e retorna-o*/
Aeroporto criar_aeroporto(){

    Aeroporto aeroporto;
    scanf("%s %s %[^\n]", aeroporto.IDAeroporto, aeroporto.pais,
    aeroporto.cidade);
    return aeroporto;
}


/*Verifica se o ID do aeroporto recebido e valido, se ultrapassa o numero
maximo de aeroportos e se ja existe outro aeroporto com o mesmo ID e, no caso de
o aeroporto passar todos os testes, imprime-o no ecra e adiciona-o ao vetor*/
int adicionarAeroporto(Aeroporto aeroporto, Aeroporto aeroportos[],
                       int numero_de_aeroportos){

    int cont1, cont2, len;

    len = strlen(aeroporto.IDAeroporto);
    if (len < COD_AEROPORTO_MIN || len > COD_AEROPORTO_MAX){
        printf("%s1", ID_INVALIDO);
        return 0;
    }

    for (cont1 = 0; cont1 < len; cont1++){
        if (!eh_maiuscula(aeroporto.IDAeroporto[cont1])
        && !eh_minuscula(aeroporto.IDAeroporto[cont1])){
            printf("%s2", ID_INVALIDO);
            return 0;
        }
    }
    if (numero_de_aeroportos >= AEROPORTOS_MAX){
        printf("%s", DEMASIADOS_AEROPORTOS);
        return 0;
    }
    for (cont2 = 0; cont2 < numero_de_aeroportos; cont2++){
        if (!strcmp(aeroportos[cont2].IDAeroporto, aeroporto.IDAeroporto)){
            printf("%s", AEROPORTO_DUPLICADO);
            return 0;
        }
    }
    aeroportos[numero_de_aeroportos] = aeroporto;
    printf("%s %s\n", AEROPORTO, aeroporto.IDAeroporto);
    return 1;
}


/*Lista todos os aeroportos no ecra, seguidos do numero de voos que deles
partem*/
void listar_aeroportos_todos(int numero_de_aeroportos, Aeroporto aeroportos[],
                             int numero_de_voos){

    int cont, i = 0, n_voos = 0;

    for(i = 0; i < numero_de_aeroportos; i++){
            n_voos = 0;
            for (cont = 0; cont < numero_de_voos; cont++){
                if (!strcmp(voos[cont].IDPartida, aeroportos[i].IDAeroporto)){
                    n_voos++;
                }
            }
            printf("%s %s %s %d\n", aeroportos[i].IDAeroporto,
                   aeroportos[i].cidade, aeroportos[i].pais, n_voos);
        }
}


/*Lista os aeroportos fornecidos no ecra seguidos do numero de voos que deles
partem pela ordem pela qual o utilizador os escreveu*/
void listar_aeroportos_dados(int numero_de_aeroportos, Aeroporto aeroportos[],
                                int numero_de_voos){
    
    int cont, i = 0, n_voos = 0;
    char ID[AEROPORTOS_MAX], c = ' ';
    Aeroporto aeroporto;

    while (c != '\n'){
            scanf("%s", ID);
            c = getchar();
            aeroporto = encontrar_aeroporto(ID, aeroportos,
            numero_de_aeroportos);
            if(!strcmp(aeroporto.IDAeroporto, "...")){
                printf("%s%s", ID, ID_NAO_EXISTE);
            }

            else{
                for (cont = 0; cont < numero_de_voos; cont++){
                    if (!strcmp(voos[cont].IDPartida,
                                aeroportos[i].IDAeroporto)){
                        n_voos++;
                    }
                }
                printf("%s %s %s %d\n", aeroporto.IDAeroporto,
                       aeroporto.cidade, aeroporto.pais, n_voos);
                n_voos = 0;
            }
    }
}


/*Ordena os aeroportos todos por ordem alfabetica de ID e, dependendo do
caracter, seguinte, chama uma de duas funcoes*/
void listar_aeroportos(int numero_de_aeroportos, Aeroporto aeroportos[],
                       int numero_de_voos){

    ordenar_aeroportos(aeroportos, numero_de_aeroportos);

    if (getchar() == '\n'){
        listar_aeroportos_todos(numero_de_aeroportos, aeroportos,
                                numero_de_voos);
    }

    else{
        listar_aeroportos_dados(numero_de_aeroportos, aeroportos,
                                numero_de_voos);
    }
}


/*Cria um novo voo e adiciona-o ao vetor dos voos*/
int criar_voo(int numero_de_voos, Aeroporto aeroportos[], Data data,
              int numero_de_aeroportos){
    Voo voo;

    scanf("%s %s %s %d-%d-%d %d:%d %d:%d %d", voo.codigoVoo, voo.IDPartida,
            voo.IDChegada, &voo.dataPartida.dia, &voo.dataPartida.mes,
            &voo.dataPartida.ano, &voo.horaPartida.hora, &voo.horaPartida.min,
            &voo.duracao.hora, &voo.duracao.min, &voo.capacidade);

    if(!validar_voo(voo, numero_de_voos, aeroportos, data,
                    numero_de_aeroportos)){
        return 0;
    }

    voo = calcular_data_e_hora_chegada(voo);

    voos[numero_de_voos] = voo;
    return 1;
}


/*Lista todos os voos no ecra pela ordem pela qual foram criados*/
void listar_voos(int numero_de_voos){
    int i;
    for (i = 0; i < numero_de_voos; i++){
        printf("%s %s %s %02d-%02d-%d %02d:%02d\n", voos[i].codigoVoo,
               voos[i].IDPartida, voos[i].IDChegada, voos[i].dataPartida.dia,
               voos[i].dataPartida.mes, voos[i].dataPartida.ano,
               voos[i].horaPartida.hora, voos[i].horaPartida.min);
    }
}


/*Lista no ecra todos os voos com partida no aeroporto recebido*/
void listar_partidas(int numero_de_voos, Aeroporto aeroportos[],
                     int numero_de_aeroportos, Data data_atual){
    int i, c = 0, count = 0;
    char ID[LEN_IDAEROPORTO];
    Voo voos_a_listar[VOOS_MAX], v;
    Aeroporto aeroporto;

    scanf("%s", ID);

    aeroporto = encontrar_aeroporto(ID, aeroportos, numero_de_aeroportos);

    if (!strcmp(aeroporto.IDAeroporto, "...")){
        printf("%s%s", ID, ID_NAO_EXISTE);
        return;
    }
    
    for (i = 0; i < numero_de_voos; i++){
        if (!strcmp(voos[i].IDPartida, ID)
        && comparar_datas(voos[i].dataPartida, data_atual) == '='){
            voos_a_listar[c] = voos[i];
            c++;
        }
    }
    ordenar_voos_partida(voos_a_listar, c);
    for (count = 0; count < c; count++){
        v = voos_a_listar[count];
        printf("%s %s %02d-%02d-%d %02d:%02d\n", v.codigoVoo, v.IDChegada,
               v.dataPartida.dia, v.dataPartida.mes, v.dataPartida.ano,
               v.horaPartida.hora, v.horaPartida.min);
    }
}


/*Lista no ecra todos os voos com chegada prevista para o aeroporto recebido*/
void listar_chegadas(int numero_de_voos, Aeroporto aeroportos[],
                     int numero_de_aeroportos){
    int i, c = 0, count = 0;
    char ID[LEN_IDAEROPORTO];
    Voo voos_a_listar[VOOS_MAX], v;
    Aeroporto aeroporto;

    scanf("%s", ID);

    aeroporto = encontrar_aeroporto(ID, aeroportos, numero_de_aeroportos);

    if (!strcmp(aeroporto.IDAeroporto, "...")){
        printf("%s%s", ID, ID_NAO_EXISTE);
        return;
    }

    for (i = 0; i < numero_de_voos; i++){
        if (!strcmp(voos[i].IDChegada, ID)){
            voos_a_listar[c] = voos[i];
            c++;
        }
    }

    ordenar_voos_chegada(voos_a_listar, c);
    for (count = 0; count < c; count++){
        v = voos_a_listar[count];
        printf("%s %s %02d-%02d-%d %02d:%02d\n", v.codigoVoo, v.IDPartida,
               v.dataChegada.dia, v.dataChegada.mes, v.dataChegada.ano,
               v.horaChegada.hora, v.horaChegada.min);
    }
}


/*Altera a data atual para a que o utilizador escrever no ecra*/
Data alterar_data(Data data){
    int ano_novo, mes_novo, dia_novo;
    Data nova_data;
    scanf("%d-%d-%d", &dia_novo, &mes_novo, &ano_novo);

    if (!data_valida(dia_novo, mes_novo, ano_novo, data)){
        printf("%s", DATA_INVALIDA);
        return data;
    }
    
    nova_data.dia = dia_novo;
    nova_data.mes = mes_novo;
    nova_data.ano = ano_novo;
    printf("%02d-%02d-%d\n", nova_data.dia, nova_data.mes, nova_data.ano);
    
    return nova_data;
}






/*-------------------Funcoes adicionadas no segundo projeto-------------------*/



    /*........................funcoes auxiliares........................*/


/*Verifica se o codigo de reserva recebido e valido*/
int eh_codigoRes_valido(char codigo[], int len){
    int i;
    if (len < 10){
        return 0;
    }
    for(i = 0; i < len; i++){
        if(!eh_digito(codigo[i]) && !eh_maiuscula(codigo[i])){
            return 0;
        }
    }
    return 1;
}


/*Retorna um ponteiro para o voo ao qual corresponde o codigo ou, no caso de
nao existir nenhum voo com o codigo recebido, retorna NULL*/
Voo* encontrar_voo_codVoo(char codigo_voo[], int numero_de_voos, Data data){
    int i;
    for (i = 0; i < numero_de_voos; i++){
        if (!strcmp(codigo_voo, voos[i].codigoVoo)
        && comparar_datas(data, voos[i].dataPartida) == '='){
            return &(voos[i]);
        }
    }
    return NULL;
}


/*liberta a memoria associada a um ponteiro para uma reserva e ao ponteiro
para o seu codigo de reserva*/
void libertar_res(Reserva* reserva){
    free(reserva->codigo_reserva);
    free(reserva);
}


/*apaga a memória e termina o programa*/
void terminar_prog(Reserva* prim_res, int mem_cheia){
    Reserva* aux;

    if (prim_res != NULL){
        while(prim_res != NULL){
            aux = prim_res;
            prim_res = prim_res->next;
            libertar_res(aux);
        }
    }

    if (mem_cheia){
        printf("%s", MEMORIA_CHEIA);
        exit(1);
    }
    exit(0);
}


/*insere a reserva ordenadamente na linked list das reservas desde que esta
passe nos testes a eroos*/
Reserva *inserir_reserva_ordenada(Reserva* reserva, Reserva* prim_res,
                                    Voo* voo, Data data_atual){
    Reserva* atual;
    if (reserva->num_passageiros <= 0){
        printf("%s", NUMERO_PASSAGEIROS_INVALIDO);
        libertar_res(reserva);
        return prim_res;
    }
    if ((prim_res == NULL)
    || (strcmp(reserva->codigo_reserva, prim_res->codigo_reserva) < 0)){
        voo->capacidade -= reserva->num_passageiros;
        if(voo->capacidade < 0){
            voo->capacidade += reserva->num_passageiros;
            printf("%s", DEMASIADAS_RESERVAS);
            voo->capacidade += reserva->num_passageiros;
            libertar_res(reserva);
            return prim_res;
        }
        if(!data_valida(reserva->data.dia, reserva->data.mes,
                        reserva->data.ano, data_atual)){
            printf("%s", DATA_INVALIDA);
            libertar_res(reserva);
            return prim_res;
        }
        reserva->next = prim_res;
        return reserva;
    }
    atual = prim_res;
    while (atual->next != NULL){
        if(strcmp(reserva->codigo_reserva, atual->next->codigo_reserva) < 0){
            if (!strcmp(reserva->codigo_reserva, atual->codigo_reserva)){
                printf("%s: %s", reserva->codigo_reserva,
                       CODIGO_RESERVA_REPETIDO);
                libertar_res(reserva);
                return prim_res;
            }
            voo->capacidade -= reserva->num_passageiros;
            if(voo->capacidade < 0){
                printf("%s", DEMASIADAS_RESERVAS);
                voo->capacidade += reserva->num_passageiros;
                libertar_res(reserva);
                return prim_res;
            }
            if(!data_valida(reserva->data.dia, reserva->data.mes,
                            reserva->data.ano, data_atual)){
                printf("%s", DATA_INVALIDA);
                libertar_res(reserva);
                return prim_res;
            }
            reserva->next = atual->next;
            atual->next = reserva;
            return prim_res;
        }
        atual = atual->next;
    }

    if (!strcmp(reserva->codigo_reserva, atual->codigo_reserva)){
        printf("%s: %s", reserva->codigo_reserva, CODIGO_RESERVA_REPETIDO);
        libertar_res(reserva);
        return prim_res;
    }
    voo->capacidade -= reserva->num_passageiros;
    if(voo->capacidade < 0){
        printf("%s", DEMASIADAS_RESERVAS);
        voo->capacidade += reserva->num_passageiros;
        libertar_res(reserva);
        return prim_res;
    }

    if(!data_valida(reserva->data.dia, reserva->data.mes,
                    reserva->data.ano, data_atual)){
        printf("%s", DATA_INVALIDA);
        libertar_res(reserva);
        return prim_res;
    }
    atual->next = reserva;
    return prim_res;
}


/*apaga a reserva com o codigo recebido*/
Reserva *apagar_reserva_codigo_res(char* codigo_reserva, Reserva* prim_res,
                                    int numero_de_voos){
    Reserva *atual, *aux;
    Voo* voo;
    if(prim_res == NULL){
        printf("%s", CODIGO_NAO_ENCONTRADO);
        free(codigo_reserva);
        return prim_res;
    }
    if (!strcmp(prim_res->codigo_reserva, codigo_reserva)){
        voo = encontrar_voo_codVoo(prim_res->codigo_voo, numero_de_voos,
                                    prim_res->data);
        voo->capacidade += prim_res->num_passageiros;
        aux = prim_res;
        prim_res = prim_res->next;
        libertar_res(aux);
        free(codigo_reserva);
        return prim_res;
    }

    for(atual = prim_res; atual != NULL && atual->next != NULL;
        atual = atual->next){
        if (!strcmp(atual->next->codigo_reserva, codigo_reserva)){
            voo = encontrar_voo_codVoo(atual->next->codigo_voo, numero_de_voos,
                                        atual->next->data);
            voo->capacidade += atual->next->num_passageiros;
            aux = atual->next;
            atual->next = atual->next->next;
            libertar_res(aux);
            free(codigo_reserva);
            return prim_res;
        }
    }
    printf("%s", CODIGO_NAO_ENCONTRADO);
    free(codigo_reserva);
    return prim_res;
}


/*apaga todas as reservas com o codigo de voo recebido*/
Reserva* apagar_reservas_codigo_voo(char codigo_voo[], Reserva* prim_res){
    Reserva *atual, *aux;
    if(prim_res == NULL){
        free(codigo_voo);
        return prim_res;
    }

    atual = prim_res;
    while(atual != NULL && atual->next != NULL){
        if (!strcmp(atual->next->codigo_voo, codigo_voo)){
            aux = atual->next;
            atual->next = atual->next->next;
            libertar_res(aux);
        }
        else{
            atual = atual->next;
        }
    }

    if (!strcmp(codigo_voo, prim_res->codigo_voo)){
        aux = prim_res;
        prim_res = prim_res->next;
        libertar_res(aux);
    }
    free(codigo_voo);
    return prim_res;
}


/*apaga os voos com o codigo recebido e as reservas a eles associadas*/
Reserva* apagar_voos(char codigo_voo[], int* numero_de_voos,
                        Reserva* prim_res, Data data_atual){
    int i, x = 0, existe = 0;
    for (i = 0; (i + x) < *numero_de_voos; i++){
        voos[i] = voos[i + x];
        if (!strcmp(voos[i].codigoVoo, codigo_voo)
        && comparar_datas(data_atual, voos[i].dataPartida) == '>'){
            voos[i] = voos[i + 1];
            x += 1;
            i -= 1;
            if(existe == 0) existe = 1;
        }
    }
    if (!existe){
        printf("%s", CODIGO_NAO_ENCONTRADO);
        free(codigo_voo);
        return prim_res;
    }
    prim_res = apagar_reservas_codigo_voo(codigo_voo, prim_res);
    *numero_de_voos -= x;
    return prim_res;
}



    /*......................Funcoes principais......................*/


/*Cria uma nova reserva e devolve a reserva da primeira posicao da lista*/
Reserva *criar_reserva(char codigo_voo[], Data data, Reserva* prim_res,
                        Data data_atual, int numero_de_voos){
    int len;
    char codigo_reserva[MAX_CARACTERES_INSTRUCAO];
    Voo* voo;
    Reserva *reserva = malloc(sizeof(Reserva));
    if (reserva == NULL){
        terminar_prog(prim_res, 1);
    }

    voo = encontrar_voo_codVoo(codigo_voo, numero_de_voos, data);

    scanf("%s %d", codigo_reserva, &reserva->num_passageiros);

    len = strlen(codigo_reserva);
    if(!eh_codigoRes_valido(codigo_reserva, len)){
        printf("%s", CODIGO_RESERVA_INVALIDO);
        free(reserva);
        return prim_res;
    }

    if (voo == NULL){
        printf("%s: %s", codigo_voo, VOO_NAO_EXISTE);
        free(reserva);
        return prim_res;
    }

    reserva->codigo_reserva = (char*) malloc(sizeof(char) * (len + 1));
    if (reserva->codigo_reserva == NULL){
        free(reserva);
        terminar_prog(prim_res, 1);
    }

    strcpy(reserva->codigo_voo, codigo_voo);
    strcpy(reserva->codigo_reserva, codigo_reserva);
    reserva->data = data;
    reserva->next = NULL;
    prim_res = inserir_reserva_ordenada(reserva, prim_res,
                                            voo, data_atual);
    return prim_res;
}


/*Lista todas as reservas por ordem alfabetica*/
void listar_reservas(char codigo_voo[], Data data, Reserva* prim_res,
                     int numero_de_voos, Data data_atual){
    Reserva* atual;
    Voo* voo;
    voo = encontrar_voo_codVoo(codigo_voo, numero_de_voos, data);
    if (voo == NULL){
        printf("%s: %s", codigo_voo, VOO_NAO_EXISTE);
        return;
    }

    if(!data_valida(data.dia, data.mes, data.ano, data_atual)){
        printf("%s", DATA_INVALIDA);
        return;
    }

    atual = prim_res;
    while (atual != NULL){
        if (!strcmp(atual->codigo_voo, codigo_voo)
        && comparar_datas(atual->data, data) == '='){
            printf("%s %d\n", atual->codigo_reserva, atual->num_passageiros);
        }
        atual = atual->next;
    }
}


/*Chama uma de duas funcoes consoante o comando 'r' e ou nao chamado com
argumentos extra*/
Reserva* comando_r(int numero_de_voos, Data data_atual, Reserva* prim_res){
    char codigo_voo[CODIGO_VOO_LEN];
    Data data;
    scanf("%s %d-%d-%d", codigo_voo, &data.dia, &data.mes, &data.ano);
    if (getchar() == '\n'){
        listar_reservas(codigo_voo, data, prim_res, numero_de_voos,
                        data_atual);
        return prim_res;
    }
    else{
        prim_res = criar_reserva(codigo_voo, data, prim_res, data_atual,
                                    numero_de_voos);
        return prim_res;
    }
}


/*Chama uma de tres funcoes consoante o comando 'e' e chamado
com um codigo de voo ou com um codigo de reserva*/
Reserva* comando_e(int* numero_de_voos, Reserva* prim_res, Data data_atual){
    char codigo_aux[MAX_CARACTERES_INSTRUCAO], *codigo;
    int len;
    getchar();
    scanf("%s", codigo_aux);
    len = strlen(codigo_aux);
    codigo = (char*) malloc(sizeof(codigo_aux));
    if (codigo == NULL){
        terminar_prog(prim_res, 1);
    }
    strcpy(codigo, codigo_aux);
    if (len < 10){
        return apagar_voos(codigo, numero_de_voos, prim_res, data_atual);
    }
    return apagar_reserva_codigo_res(codigo, prim_res, *numero_de_voos);
}



    /*..........................Funcao main..........................*/


/*Le o comando dado pelo utilizador e chama as funcoes necessarias para o
executar*/
int main(){
    int numero_de_aeroportos = 0, numero_de_voos = 0;
    char comando;
    Data data = DATA_INICIAL;
    Aeroporto aeroportos[AEROPORTOS_MAX];
    Aeroporto aeroporto;
    Reserva* prim_res = NULL;
    while(1){
        comando = getchar();
        switch(comando){

            case 'q' :
                terminar_prog(prim_res, 0);
                break;

            case 'a' :
                aeroporto = criar_aeroporto();
                numero_de_aeroportos += adicionarAeroporto(aeroporto,
                aeroportos, numero_de_aeroportos);
                break;

            case 'l' :
                listar_aeroportos(numero_de_aeroportos, aeroportos,
                numero_de_voos);
                break;

            case 'v' :
                if (getchar() != '\n'){
                    numero_de_voos += criar_voo(numero_de_voos, aeroportos,
                    data, numero_de_aeroportos);
                }
                else{
                    listar_voos(numero_de_voos);
                }
                break;

            case 'p' :
                listar_partidas(numero_de_voos, aeroportos,
                numero_de_aeroportos, data);
                break;

            case 'c' :
                listar_chegadas(numero_de_voos, aeroportos,
                numero_de_aeroportos);
                break;

            case 't' :
                data = alterar_data(data);
                break;

            case 'r' :
                prim_res = comando_r(numero_de_voos, data, prim_res);
                break;

            case 'e' :
                prim_res = comando_e(&numero_de_voos, prim_res, data);
                break;
        }
    }
}
