#ifndef _PARAMETROS_H
#define _PARAMETROS_H

#define ALPHA1 3.0
#define ALPHA2 3.0

#define TEMPO_MAX 15000 // milisegundos

#define TEMPO_REF 50 // milisegundos

#define TEMPO_MODELO_REF 30 // milisegundos

#define TEMPO_CONTROLE 30 // milisegundos

#define TEMPO_LINEARIZACAO 20 // milisegundos

#define TEMPO_ROBO 10 // milisegundos

#define TAM_VETOR_REF TEMPO_MAX/TEMPO_REF
#define TAM_VETOR_MODELO_REF TEMPO_MAX/TEMPO_MODELO_REF
#define TAM_VETOR_CONTROLE TEMPO_MAX/TEMPO_CONTROLE
#define TAM_VETOR_LINEARIZACAO TEMPO_MAX/TEMPO_LINEARIZACAO
#define TAM_VETOR_ROBO TEMPO_MAX/TEMPO_ROBO

#define R 0.3

#endif
