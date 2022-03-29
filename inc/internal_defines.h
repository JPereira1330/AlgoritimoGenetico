#ifndef _INC_INTERNAL_DEFINES_H_
#define _INC_INTERNAL_DEFINES_H_

//// CONTROLE POPULACIONAL
#define MAX_POPULATION          100
#define MAX_WORST_POP           20
#define MAX_BETTER_POP          20
#define MAX_MUTABLE_POP         20

/// CONTROLE DE STRINGS
#define MAX_LEN_NAME_ITEN       126     // Tamanho de cada item
#define MAX_LEN_ARRAY_ITENS     256     // Quantia que cada DNA pode possuir de itens
#define MAX_LEN_POPULATION      100     // Tamanho maximo de DNA

/// CONTROLE DO BANCO DE DADOS
#define DATABASE_NAME   "data/drugs.db" // Caminho do banco de dados que sera analisado
#define MAX_DATABASE_LEN        10000   // Tamanho maximo de itens no banco de dados

#endif  // _INC_INTERNAL_DEFINES_H_