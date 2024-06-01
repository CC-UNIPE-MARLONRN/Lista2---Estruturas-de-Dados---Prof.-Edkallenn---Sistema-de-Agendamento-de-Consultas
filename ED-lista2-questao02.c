/*
** Função : Criar um Sistema de agendamento de consultas para uma clinica médica.
** Autor : Marlon Rufino do Nascimento 2º período turma A 2024.1
** Data : 26/05/2024
** Observações:
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#define MAX_CONSULTA_MEDICO_DIA 5

typedef struct {
    int primeiro, ultimo; // Índice do primeiro e do último atendimento
    int quantidade; // Quantidade máxima de atendimentos
    int numero_atendimento[MAX_CONSULTA_MEDICO_DIA]; // Números dos atendimentos agendados
} Fila_Consulta_Medico;

typedef struct No_Consulta_enfermagem {
    int numero_atendimento;
    struct No_Consulta_enfermagem *prox;
} No_Consulta_enfermagem;

typedef struct {
    No_Consulta_enfermagem *primeiro, *ultimo;
} Fila_Consulta_Enfermagem;

// Função para criar uma fila de consultas com médico
void criar_fila_Consulta_Medico(Fila_Consulta_Medico *fila) {
    fila->primeiro = -1;
    fila->ultimo = -1;
    fila->quantidade = MAX_CONSULTA_MEDICO_DIA;
}

// Função para criar uma fila de consultas com enfermagem
void criar_fila_Consulta_enfermagem(Fila_Consulta_Enfermagem *fila) {
    fila->primeiro = NULL;
    fila->ultimo = NULL;
}

// Função para verificar se a fila de consultas com médico está vazia
int verifica_fila_Medico_vazia(Fila_Consulta_Medico *fila) {
    return fila->primeiro == -1;
}

// Função para verificar se a fila de consultas com enfermagem está vazia
int verifica_fila_Enfermagem_vazia(Fila_Consulta_Enfermagem *fila) {
    return fila->primeiro == NULL;
}

// Função para verificar se a fila de consultas com médico está cheia
int verifica_fila_Medico_cheia(Fila_Consulta_Medico *fila) {
    return (fila->ultimo + 1) % fila->quantidade == fila->primeiro;
}

// Função para agendar uma consulta com médico
int agendar_Consulta_Medico(Fila_Consulta_Medico *fila) {
    if (verifica_fila_Medico_cheia(fila)) {
        printf("\nNão é possível agendar mais consultas: Máximo de consultas para o dia alcançadas!\n");
        return -1;
    }
    static int numero_consulta_medico = 1; // Número da consulta começando de 1
    if (fila->primeiro == -1) {
        fila->primeiro = 0;
    }
    fila->ultimo = (fila->ultimo + 1) % fila->quantidade;
    fila->numero_atendimento[fila->ultimo] = numero_consulta_medico;
    return numero_consulta_medico++;
}

// Função para agendar uma consulta com enfermagem
int agendar_Consulta_Enfermagem(Fila_Consulta_Enfermagem *fila) {
    No_Consulta_enfermagem *novo_no = (No_Consulta_enfermagem *)malloc(sizeof(No_Consulta_enfermagem));
    if (novo_no == NULL) {
        printf("\nFalha na alocação de memória!\n");
        return -1;
    }
    static int numero_consulta_enfermagem = 1; // Número da consulta começando de 1
    novo_no->numero_atendimento = numero_consulta_enfermagem++;
    novo_no->prox = NULL;
    if (verifica_fila_Enfermagem_vazia(fila)) {
        fila->primeiro = novo_no;
        fila->ultimo = novo_no;
    } else {
        fila->ultimo->prox = novo_no;
        fila->ultimo = novo_no;
    }
    return novo_no->numero_atendimento;
}

// Função para imprimir a fila de consultas com médico
void imprimir_fila_Consulta_Medico(Fila_Consulta_Medico *fila) {
    if (verifica_fila_Medico_vazia(fila)) {
        printf("\nNão há consultas agendadas com o médico!\n");
        return;
    }
    printf("\nConsultas agendadas com o médico:\n");
    int i = fila->primeiro;
    while (i != fila->ultimo) {
        printf(" - Consulta %d\n", fila->numero_atendimento[i]);
        i = (i + 1) % fila->quantidade;
    }
    printf("- Consulta %d\n", fila->numero_atendimento[i]);
    int total_consultas = (fila->ultimo - fila->primeiro + 1 + fila->quantidade) % fila->quantidade;
    printf("Total de consultas marcadas: %d\n", total_consultas);
    printf("Total de vagas restantes: %d\n", fila->quantidade - total_consultas);
}

// Função para imprimir a fila de consultas com enfermagem
void imprimir_fila_Consulta_Enfermagem(Fila_Consulta_Enfermagem *fila) {
    if (verifica_fila_Enfermagem_vazia(fila)) {
        printf("\nNão há consultas agendadas com a enfermagem!\n");
        return;
    }
    printf("\nConsultas agendadas com a enfermagem:\n");
    No_Consulta_enfermagem *atual = fila->primeiro;
    while (atual != NULL) {
        printf(" - Consulta %d\n", atual->numero_atendimento);
        atual = atual->prox;
    }
    // Calcula o total de consultas
    int total_consultas = 0;
    atual = fila->primeiro;
    while (atual != NULL) {
        total_consultas++;
        atual = atual->prox;
    }
    printf("\nTotal de consultas marcadas: %d\n", total_consultas);
}

// Função para cancelar uma consulta com médico
int cancelar_consulta_Medico(Fila_Consulta_Medico *fila, int numero_consulta) {
    if (verifica_fila_Medico_vazia(fila)) {
        printf("\nNão há consultas agendadas com o médico!\n");
        return -1;
    }
    
    int i = fila->primeiro;
    int consultas_encontradas = 0;
    
    while (i != fila->ultimo) {
        if (fila->numero_atendimento[i] == numero_consulta) {
            consultas_encontradas = 1;
            break;
        }
        i = (i + 1) % fila->quantidade;
    }

    if (!consultas_encontradas && fila->numero_atendimento[i] != numero_consulta) {
        printf("\nConsulta %d não encontrada.\n", numero_consulta);
        return -1;
    }

    printf("\nConsulta %d cancelada.\n", numero_consulta);

    if (fila->primeiro == fila->ultimo) {
        fila->primeiro = -1;
        fila->ultimo = -1;
    } else {
        fila->primeiro = (fila->primeiro + 1) % fila->quantidade;
    }

    return numero_consulta;
}

// Função para cancelar uma consulta com enfermagem
int cancelar_consulta_Enfermagem(Fila_Consulta_Enfermagem *fila, int numero_consulta) {
    if (verifica_fila_Enfermagem_vazia(fila)) {
        printf("\nNão há consultas agendadas com a enfermagem!\n");
        return -1;
    }

    No_Consulta_enfermagem *atual = fila->primeiro;
    No_Consulta_enfermagem *anterior = NULL;
    while (atual != NULL && atual->numero_atendimento != numero_consulta) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("\nConsulta %d não encontrada.\n", numero_consulta);
        return -1;
    }

    if (anterior == NULL) {
        fila->primeiro = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    if (atual == fila->ultimo) {
        fila->ultimo = anterior;
    }

    free(atual);
    printf("\nConsulta %d cancelada.\n", numero_consulta);
    return numero_consulta;
}

// Função principal
int main() {
    setlocale(LC_ALL, "");

    Fila_Consulta_Medico fila_medico;
    criar_fila_Consulta_Medico(&fila_medico);

    Fila_Consulta_Enfermagem fila_enfermagem;
    criar_fila_Consulta_enfermagem(&fila_enfermagem);

    int opcao_principal = -1;
    int opcao_submenu = -1;

    while (opcao_principal != 3) {
        printf("\n----- Menu Principal -----\n");
        printf("1. Atendimento Médico\n");
        printf("2. Atendimento Enfermagem\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao_principal);

        switch (opcao_principal) {
            case 1:
                while (opcao_submenu != 4) {
                    printf("\n--- Atendimento Médico ---\n");
                    printf("1. Agendar consulta\n");
                    printf("2. Cancelar consulta\n");
                    printf("3. Verificar consultas\n");
                    printf("4. Voltar ao menu principal\n");
                    printf("Escolha uma opção: ");
                    scanf("%d", &opcao_submenu);

                    switch (opcao_submenu) {
                        case 1: {
                            int consulta = agendar_Consulta_Medico(&fila_medico);
                            if (consulta != -1) {
                                printf("\n Consulta %d agendada com o médico\n", consulta);
                            }
                            break;
                        }
                        case 2: {
                            int num_consulta;
                            printf("Digite o número da consulta a ser cancelada: ");
                            scanf("%d", &num_consulta);
                            cancelar_consulta_Medico(&fila_medico, num_consulta);
                            break;
                        }
                        case 3: {
                            imprimir_fila_Consulta_Medico(&fila_medico);
                            break;
                        }
                        case 4:
                            printf("\nVoltando ao menu principal...\n");
                            break;
                        default:
                            printf("\nOpção inválida! Tente novamente.\n");
                            break;
                    }
                }
                opcao_submenu = -1; 
                break;
            case 2:
                while (opcao_submenu != 4) {
                    printf("\n--- Atendimento Enfermagem ---\n");
                    printf("1. Agendar consulta\n");
                    printf("2. Cancelar consulta\n");
                    printf("3. Verificar consultas\n");
                    printf("4. Voltar ao menu principal\n");
                    printf("Escolha uma opção: ");
                    scanf("%d", &opcao_submenu);

                    switch (opcao_submenu) {
                        case 1: {
                            int consulta = agendar_Consulta_Enfermagem(&fila_enfermagem);
                            if (consulta != -1) {
                                printf("\nConsulta %d agendada com a enfermagem\n", consulta);
                            }
                            break;
                        }
                        case 2: {
                            int num_consulta;
                            printf("Digite o número da consulta a ser cancelada: ");
                            scanf("%d", &num_consulta);
                            cancelar_consulta_Enfermagem(&fila_enfermagem, num_consulta);
                            break;
                        }
                        case 3: {
                            imprimir_fila_Consulta_Enfermagem(&fila_enfermagem);
                            break;
                        }
                        case 4:
                            printf("\nVoltando ao menu principal...\n");
                            break;
                        default:
                            printf("\nOpção inválida! Tente novamente.\n");
                            break;
                    }
                }
                opcao_submenu = -1; // Resetar para o próximo uso do submenu
                break;

            case 3:
                printf("\nSaindo do sistema...\n");
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
                break;
        }
    }

    return 0;
}

