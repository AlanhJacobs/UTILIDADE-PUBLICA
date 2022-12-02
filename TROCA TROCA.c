#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "titulo.h"

struct dados_foto{
    char tipo_formatoPPM[10];
    int linhas, colunas, cor_max;
};

// GRAY|4| //

void P3_para_P2(int recortar,struct dados_foto imagem, int matriz[imagem.linhas][imagem.colunas][3], int x1, int x2, int y1, int y2){
    int matrizP2[imagem.linhas][imagem.colunas]; 
    char arquivo_de_saida[20];

    // ARQUIVO DE SAIDA
    printf("Por favor, digite o nome do arquivo de saída com sua extensão:\n");
    scanf("%s", arquivo_de_saida);
    FILE *arqout = fopen(arquivo_de_saida, "w");         
    fprintf(arqout, "P2\n");
    fprintf(arqout, "%d %d\n", imagem.linhas, imagem.colunas);

    // PRINTA A MATRIZ COM COR NO TERMINAL E APROVEITA O LACO PARA TRANSFORMAR EM PGM
    printf("\nMatriz P3:\n");
    for(int i = 0; i<imagem.linhas; i++){               
        for(int j = 0; j<imagem.colunas; j++){
            printf("| %3d %3d %3d ", matriz[i][j][0], matriz[i][j][1], matriz[i][j][2]);    
            matrizP2[i][j] = (matriz[i][j][0] + matriz[i][j][1] + matriz[i][j][2])/3;     
        }
        printf("|\n");
    }

    // PRINTA A SESSAO SELECIONADA NO TERMINAL E NO ARQUIVO(PGM)
    printf("\nMatriz P2:\n");
    if(recortar == 1){                               
        for(int i = y1; i<=y2; i++){
            for(int j = x1; j<=x2; j++){
                printf("%3d ", matrizP2[i][j]);
                fprintf(arqout,"%3d ", matrizP2[i][j]);
            }
            printf("\n");
            fprintf(arqout,"\n");
        }

    // PRINTA TUDO (PGM) NO TERMINAL E NO ARQUIVO
    }else if(recortar == 0){                         
        for(int i = 0; i<imagem.linhas; i++){
            for(int j = 0; j<imagem.colunas; j++){
                printf("%3d ", matrizP2[i][j]);
                fprintf(arqout,"%3d ", matrizP2[i][j]);        
            }
            printf("\n");
            fprintf(arqout, "\n");
        }
    }
    fclose(arqout);
}

// GREEN|6| //

void aplica_greenscreen(struct dados_foto arq_frente, struct dados_foto arq_fundo, int cor_mascara, int x1, int x2, int y1, int y2, int matriz_frente[arq_frente.linhas][arq_frente.colunas][3], int matriz_fundo[arq_fundo.linhas][arq_fundo.colunas][3]){

    char arquivo_saida[20];

    // MOSTRA O FUNDO QUANDO A COR ESCOLHIDA FOR ENCONTRADA NA IMAGEM DA FRENTE
    for(int i = 0; i<arq_frente.linhas; i++){
        for(int j = 0; j<arq_frente.colunas; j++){
            if(matriz_frente[i][j][0] != cor_mascara && matriz_frente[i][j][1] != cor_mascara && matriz_frente[i][j][2] != cor_mascara){
                matriz_fundo[i][j][0] = matriz_frente[i][j][0];
                matriz_fundo[i][j][1] = matriz_frente[i][j][1];
                matriz_fundo[i][j][2] = matriz_frente[i][j][2];        
            }
        }
    }

    // CRIA UM ARQUIVO COM A NOVA IMAGEM
    printf("Escreva o nome do arquivo de saída:\n");
    scanf("%s", arquivo_saida);

    FILE *arquivo_novo = fopen(arquivo_saida, "w");
    fprintf(arquivo_novo, "P3\n");
    fprintf(arquivo_novo, "%d %d\n", arq_fundo.linhas, arq_fundo.colunas);
    fprintf(arquivo_novo, "%d\n", arq_fundo.cor_max);

    // PRINTA NO ARQUIVO
    for(int i = 0; i < arq_fundo.linhas; i++){
        for(int j = 0; j < arq_fundo.colunas; j++){
            fprintf(arquivo_novo, "%3d %3d %3d", matriz_fundo[i][j][0], matriz_fundo[i][j][1], matriz_fundo[i][j][2]);
        }
        fprintf(arquivo_novo, "\n");
    }

    // PRINTA NA TELA
    for(int i = 0; i < arq_fundo.linhas; i++){
        for(int j = 0; j < arq_fundo.colunas; j++){
            printf("%d %d %d", matriz_fundo[i][j][0], matriz_fundo[i][j][1], matriz_fundo[i][j][2]);
        }
        printf("\n");
    }
    fclose(arquivo_novo);
}

// GREEN|5| //

void rotaciona_e_redimenciona(int x1, int x2, int y1, int y2, int rotacao, struct dados_foto arq_frente, struct dados_foto arq_fundo, int cor_mascara, int matriz_frente[arq_frente.linhas][arq_frente.colunas][3], int matriz_fundo[arq_fundo.linhas][arq_fundo.colunas][3]){

    int matriz_auxiliar[arq_frente.linhas][arq_frente.colunas][3];

    // FAZ AS ROTACOES NECESSARIAS
    for(int r = 0; r <= rotacao; r++){
        for(int i = 0; i < arq_frente.linhas; i++){
            for(int j = 0; j < arq_frente.colunas; j++){
                matriz_auxiliar[j][arq_frente.linhas - i - 1][0] = matriz_frente[i][j][0]; 
                matriz_auxiliar[j][arq_frente.linhas - i - 1][1] = matriz_frente[i][j][1];
                matriz_auxiliar[j][arq_frente.linhas - i - 1][2] = matriz_frente[i][j][2];
            }
        }
    }

    // PASSA DA MATRIZ AUXILIAR PARA A MATRIZ FRENTE
    for(int i = 0; i < arq_frente.linhas; i++){
        for(int j = 0; j < arq_frente.colunas; j++){
            matriz_frente[i][j][0] = matriz_auxiliar[i][j][0];
            matriz_frente[i][j][1] = matriz_auxiliar[i][j][1];
            matriz_frente[i][j][2] = matriz_auxiliar[i][j][2];
        }
    }
    aplica_greenscreen(arq_frente, arq_fundo, cor_mascara, x1, x2, y1, y2, matriz_frente, matriz_fundo);
}

// GREEN|4| //

void recorte_green(struct dados_foto arq_frente, struct dados_foto arq_fundo, int rotacao, int cor_mascara, int matriz_frente[arq_frente.linhas][arq_frente.colunas][3], int matriz_fundo[arq_fundo.linhas][arq_fundo.colunas][3]){
    int recortar = 2, x1, x2, y1, y2;

    while(recortar != 0){
        printf("Se você deseja escolher uma parte para aplicar o efeito escolhido, digite 1, senão, digite 0 e toda a imagem será selecionada:\n");
        scanf("%d", &recortar);     

        // CASO  O USUARIO QUEIRA RECORTAR
        if(recortar == 1){ 

            // PEGA AS COORDENADAS 'x', SENDO 'x1' MENOR DO QUE 'x2'                                  
            printf("Por favor, digite as coordenadas 'x1' e 'x2', respectivamente, da área que deseja recortar:\n"); 
            printf("Sendo 'x1' >= 0 e 'x2' < %d\n", arq_frente.colunas);
            scanf("%d %d", &x1, &x2);

            // ANTIBURRO
            while(x1 > x2 || x1 < 0 || x2 >= arq_frente.colunas){
                printf("Por favor, digite as coordenadas 'x1' e 'x2', respectivamente, da área que deseja recortar:\n");
                printf("Sendo 'x1' >= 0 e 'x2' < %d\n", arq_frente.colunas);
                scanf("%d %d", &x1, &x2);
            }

            // PEGA AS COORDENADAS 'y', SENDO 'y1' MENOR DO QUE 'y2'
            printf("Por favor, digite as coordenadas 'y1' e 'y2', respectivamente, da área que deseja recortar:\n");
            printf("Sendo 'y1' >= 0 e 'y2' < %d\n", arq_frente.linhas);
            scanf("%d %d", &y1, &y2);

            // ANTIBURRO
            while(y1 > y2 || y1 < 0 || y2 >= arq_frente.linhas){
                printf("Por favor, digite as coordenadas 'y1' e 'y2', respectivamente, da área que deseja recortar:\n");
                printf("Sendo 'y1' >= 0 e 'y2' < %d\n", arq_frente.linhas);
                scanf("%d %d", &y1, &y2);
            }
            arq_frente.linhas = y2 - y1 + 1;
            arq_frente.colunas = x2 - x1 + 1;
            break;
        }
    }
    rotaciona_e_redimenciona(x1, x2, y1, y2, rotacao, arq_frente, arq_fundo, cor_mascara, matriz_frente, matriz_fundo);
}

// GRAY|3| //

// DA AO USUARIO A OPCAO DO RECORTE, CASO NAO QUEIRA, SEGUE PARA A FUNCAO DE MONOCROMATIZACAO
void recorte_gray(struct dados_foto imagem, int matriz[imagem.linhas][imagem.colunas][3]){           
    int recortar = 1, x1,x2,y1,y2;

    while(recortar != 0){
        printf("Se você deseja escolher uma parte para aplicar o efeito escolhido, digite 1, senão, digite 0 e toda a imagem será selecionada:\n");
        scanf("%d", &recortar);     

        // CASO  O USUARIO QUEIRA RECORTAR
        if(recortar == 1){  

            // PEGA AS COORDENADAS 'x', SENDO 'x1' MENOR DO QUE 'x2'                                  
            printf("Por favor, digite as coordenadas 'x1' e 'x2', respectivamente, da área que deseja recortar:\n"); 
            printf("Sendo 'x1' >= 0 e 'x2' < %d\n", imagem.colunas);
            scanf("%d %d", &x1, &x2);

            // ANTIBURRO
            while(x1 > x2 || x1 < 0 || x2 >= imagem.colunas){
                printf("Por favor, digite as coordenadas 'x1' e 'x2', respectivamente, da área que deseja recortar:\n");
                printf("Sendo 'x1' >= 0 e 'x2' < %d\n", imagem.colunas);
                scanf("%d %d", &x1, &x2);
            }

            // PEGA AS COORDENADAS 'y', SENDO 'y1' MENOR DO QUE 'y2'
            printf("Por favor, digite as coordenadas 'y1' e 'y2', respectivamente, da área que deseja recortar:\n");
            printf("Sendo 'y1' >= 0 e 'y2' < %d\n", imagem.linhas);
            scanf("%d %d", &y1, &y2);

            // ANTIBURRO
            while(y1 > y2 || y1 < 0 || y2 >= imagem.linhas){
                printf("Por favor, digite as coordenadas 'y1' e 'y2', respectivamente, da área que deseja recortar:\n");
                printf("Sendo 'y1' >= 0 e 'y2' < %d\n", imagem.linhas);
                scanf("%d %d", &y1, &y2);
            }
            imagem.colunas = x2 - x1 + 1;
            imagem.linhas = y2 - y1 + 1;
            break;
        }
    }
    P3_para_P2(recortar, imagem, matriz, x1, x2, y1, y2);
}

// GRAY|2| //

void pega_dados_grayscale(FILE *arquivo){
    struct dados_foto imagem;
    fscanf(arquivo, "%d %d", &imagem.linhas, &imagem.colunas);
    fscanf(arquivo, "%d", &imagem.cor_max);

    int matriz[imagem.linhas][imagem.colunas][3];

    for(int i = 0; i < imagem.linhas; i++){
        for(int j = 0; j < imagem.colunas; j++){
            fscanf(arquivo, "%d %d %d", &matriz[i][j][0], &matriz[i][j][1], &matriz[i][j][2]);
        }
    }
    recorte_gray(imagem,matriz);
}

// GREEN|3| //

// USUARIO ESCOLHE COR DO CHROMA KEY E A ROTACAO
void escolhe_cor_e_rotacao(struct dados_foto arq_frente, struct dados_foto arq_fundo, int matriz_frente[arq_frente.linhas][arq_frente.colunas][3], int matriz_fundo[arq_fundo.linhas][arq_fundo.colunas][3]){
    int cor_mascara = -1, rotacao = -1;
    while(cor_mascara < 0 || cor_mascara > arq_frente.cor_max){
        printf("Escolha a cor que deve ser usada como máscara na imagem de frente de 0 a %d:\n", arq_frente.cor_max);
        scanf("%d", &cor_mascara);
    }
    while(rotacao < 0 || rotacao > 3){
        printf("Escolha a rotação desejada para a imagem da frente:\n 0- 0º\n 1- 90º\n 2- 180º\n 3- 270\n");
        scanf("%d", &rotacao);
    }
    recorte_green( arq_frente, arq_fundo, rotacao, cor_mascara, matriz_frente, matriz_fundo);
}

// GREEN|2| //

// CHECA SE A IMAGEM DA FRENTE TEM O TAMANHO MENOR OU IGUAL A DE FUNDO E SE E QUADRADA
void checa_tamanho_matrizacao_fotos(struct dados_foto arq_frente, struct dados_foto arq_fundo, FILE *arquivo_frente, FILE *arquivo_fundo){

    ///// ARQUIVO FRENTE /////

    // PEGA TAMANHO E COR MAX ARQUIVO FRENTE 
    fscanf(arquivo_frente, "%d %d", &arq_frente.linhas, &arq_frente.colunas);
    fscanf(arquivo_frente, "%d", &arq_frente.cor_max);

    // CRIA MATRIZ
    int matriz_frente[arq_frente.linhas][arq_frente.colunas][3];
    for(int i = 0; i<arq_frente.linhas; i++){
        for(int j = 0; j<arq_frente.colunas; j++){
            fscanf(arquivo_frente, "%d %d %d", &matriz_frente[i][j][0], &matriz_frente[i][j][1], &matriz_frente[i][j][2]);
        }
    }

    // SE A MATRIZ NAO FOR QUADRADA
    // PEGA O LADO MAIOR E CRIA UMA MATRIZ QUADRADA
    int maior_lado = 0, matrizaux[maior_lado][maior_lado][3];

    if(arq_frente.linhas > arq_frente.colunas){
        maior_lado = arq_frente.linhas;
    }if(arq_frente.colunas > arq_frente.linhas){
        maior_lado = arq_frente.colunas;
    }

    if(maior_lado != 0){

        // CRIA COM NUMERO NEGATIVO PRA DIFERENCIAR
        for(int i = 0; i < maior_lado; i++){
            for(int j = 0; j < maior_lado; j++){
                matrizaux[i][j][0] = -1;
                matrizaux[i][j][1] = -1;
                matrizaux[i][j][2] = -1;
            }
        }

        // PREENCHE A MATRIZ QUADRADA CRIADA COM A MATRIZ FRENTE
        for(int i = 0; i < maior_lado; i++){
            for(int j = 0; j < maior_lado; j++){
                matrizaux[i][j][0] = matriz_frente[i][j][0];
                matrizaux[i][j][1] = matriz_frente[i][j][1];
                matrizaux[i][j][2] = matriz_frente[i][j][2];
            }
        }

        // MATRIZ FRENTE NOVO TAMANHO
        arq_frente.linhas = maior_lado, arq_frente.colunas = maior_lado;

        // AGORA A MATRIZ FRENTE ESTA QUADRADA
        for(int i = 0; i<arq_frente.linhas; i++){
            for(int j = 0; j<arq_frente.colunas; j++){
                matriz_frente[i][j][0] = matrizaux[i][j][0];
                matriz_frente[i][j][1] = matrizaux[i][j][1];
                matriz_frente[i][j][2] = matrizaux[i][j][2];
            }
        }
    }

    ///// ARQUIVO FUNDO /////

    // PEGA TAMANHO E COR MAX ARQUIVO FUNDO
    fscanf(arquivo_fundo, "%d %d", &arq_fundo.linhas, &arq_fundo.colunas);
    fscanf(arquivo_fundo, "%d", &arq_fundo.cor_max);

    // CRIA MATRIZ
    int matriz_fundo[arq_fundo.linhas][arq_fundo.colunas][3];
    for(int i = 0; i<arq_fundo.linhas; i++){
        for(int j = 0; j<arq_fundo.colunas; j++){
            fscanf(arquivo_fundo , "%d %d %d", &matriz_fundo[i][j][0], &matriz_fundo[i][j][1], &matriz_fundo[i][j][2]);
        }
    }

    // SE A MATRIZ NAO FOR QUADRADA
    // PEGA O LADO MAIOR E CRIA UMA MATRIZ QUADRADA
    int maior_lado1 = 0, matrizaux1[maior_lado1][maior_lado1][3];

    if(arq_fundo.linhas > arq_fundo.colunas){
        maior_lado1 = arq_fundo.linhas;
    }if(arq_fundo.colunas > arq_fundo.linhas){
        maior_lado1 = arq_fundo.colunas;
    }

    // SE UM LADO FOR MAIOR DO QUE O OUTRO EXECUTA A QUADRICULARIZACAO
    if(maior_lado1 != 0){

        // CRIA COM NUMERO NEGATIVO PRA DIFERENCIAR
        for(int i = 0; i < maior_lado1; i++){
            for(int j = 0; j < maior_lado1; j++){
                matrizaux1[i][j][0] = -1;
                matrizaux1[i][j][1] = -1;
                matrizaux1[i][j][2] = -1;
            }
        }

        // PREENCHE A MATRIZ QUADRADA CRIADA COM A MATRIZ FUNDO
        for(int i = 0; i < maior_lado1; i++){
            for(int j = 0; j < maior_lado1; j++){
                matrizaux1[i][j][0] = matriz_fundo[i][j][0];
                matrizaux1[i][j][1] = matriz_fundo[i][j][1];
                matrizaux1[i][j][2] = matriz_fundo[i][j][2];
            }
        }

        // MATRIZ FUNDO NOVO TAMANHO
        arq_fundo.linhas = maior_lado1, arq_fundo.colunas = maior_lado1;

        // AGORA A MATRIZ FUNDO ESTA QUADRADA
        for(int i = 0; i<arq_fundo.linhas; i++){
            for(int j = 0; j<arq_fundo.colunas; j++){
                matriz_fundo[i][j][0] = matrizaux1[i][j][0];
                matriz_fundo[i][j][1] = matrizaux1[i][j][1];
                matriz_fundo[i][j][2] = matrizaux1[i][j][2];
            }
        }
    }

    // IMAGEM DA FRENTE MAIOR: ERRO | IMAGEM DA FRENTE MENOR OU IGUAL: SEGUE
    if(arq_frente.linhas > arq_fundo.linhas || arq_frente.colunas > arq_fundo.colunas){
        printf("\nERRO: Imagem da frente maior que a de fundo!");
        return;
    }else{
        escolhe_cor_e_rotacao(arq_frente, arq_fundo, matriz_frente, matriz_fundo);
    }
}

// GREEN|1| //
// GRAY|1| //

// CHECA SE O ARQUIVO DIGITADO EXISTE E, CASO EXISTA, PEGA O TIPO NA PRIMEIRA LINHA E DIRECIONA PARA A FUNCAO NECESSARIA
void le_arquivo(int opcao){ 
    char nome[10];

    // OPCAO GRAYSCALE----------------------------------------------------------------
    if(opcao == 1){
        printf("\nDigite o nome do arquivo P3 que você deseja trocar para P2: ");
        scanf("%s", nome);
        FILE *arquivo = fopen(nome, "r");   

        // GARANTE A EXISTENCIA DO ARQUIVO                
        if(arquivo == NULL) {                       
            printf("Arquivo não encontrado!");              
            return;
        }
        
        // GARANTE O TIPO P3
        struct dados_foto GRAYSCALE;
        fgets(GRAYSCALE.tipo_formatoPPM, 10, arquivo);                
        printf("%s", GRAYSCALE.tipo_formatoPPM);
        if(!strncmp(GRAYSCALE.tipo_formatoPPM, "P3", 2)){
            pega_dados_grayscale(arquivo);
        }  
    
    // OPCAO GREENSCREEN--------------------------------------------------------------                
    }else if(opcao == 2){
        char nome_frente[10], nome_fundo[10];
        struct dados_foto arq_frente, arq_fundo;
        int contador = 0;

        // RECEBE O NOME DA FOTO DA FRENTE E CHECA SE EXISTE
        printf("\nDigite a seguir:\n");
        printf("O nome da foto de frente com seu formato: ");
        scanf("%s", nome_frente);

        // CHECA SE EXISTE FOTO FRENTE
        FILE *arquivo_frente = fopen(nome_frente, "r");
        if(arquivo_frente == NULL){
            printf("Arquivo de frente não encontrado!");  
            return;
        }
        
        // PERMITE APENAS TIPO P3
        fgets(arq_frente.tipo_formatoPPM, 10, arquivo_frente); 
        if(!strncmp(arq_frente.tipo_formatoPPM, "P3", 2)){
            contador += 1;
        }else{
            printf("Formato da imagem da frente não suportado!");
            return;
        }

        // RECEBE O NOME DA FOTO DE FUNDO 
        printf("O nome da foto de fundo com seu formato: ");
        scanf("%s", nome_fundo);

        // CHECA SE EXISTE FOTO FUNDO
        FILE *arquivo_fundo = fopen(nome_fundo, "r");
        if(arquivo_fundo == NULL){
            printf("Arquivo de fundo não encontrado!");  
            return;
        }

        // PERMITE APENAS TIPO P3
        fgets(arq_fundo.tipo_formatoPPM, 10, arquivo_fundo);
        if(!strncmp(arq_fundo.tipo_formatoPPM, "P3", 2)){
            contador += 1;
        }else{
            printf("Formato da imagem de fundo não suportado!");
            return;
        }
        
        // SE ESTIVER TUDO CERTO CHAMA A PROXIMA FUNCAO
        if(contador == 2){
            checa_tamanho_matrizacao_fotos(arq_frente, arq_fundo, arquivo_frente, arquivo_fundo);
        }
    }
}

// GREEN|0| //
// GRAY|0| //

void menu(){
    
    titulo();
    int opcao = 0;

    // DEIXA O USUARIO ESCOLHER A FUNCIONALIDADE DESEJADA
    while(opcao<1 || opcao>2){
        printf("\n    Escolha a opção desejada:\n");
        printf("        1 - GrayScale\n");
        printf("        2 - GreenScreen\n");
        scanf("%d", &opcao);
    }
    le_arquivo(opcao);
}

int main(){
    menu();
    return 0;
}