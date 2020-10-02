#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pixel{
  //irá armazenar os valores RGB de cada pixel
  int r, g, b;

}pixel;

void le_imagem_ppm(char nome_imagem[]){

  char id[2];
  int largura, altura, valor_maximo_pixel;
  pixel **matriz;

  //Abre a imagem ppm
  FILE *imagem = fopen(nome_imagem, "r");
  if(!imagem){
    printf("Problema com o arquivo. \n");
  }

  //P3 = imagem que está no formato texto (ASCII)
  //P6 = imagem que está no formato binário
  fscanf(imagem, "%s\n", id);
  fscanf(imagem, "%d %d\n", &largura, &altura);
  fscanf(imagem, "%d\n", &valor_maximo_pixel);

  matriz = malloc (largura * sizeof(pixel));

  if(strcmp(id, "P3") == 0){

    for(int i=0; i<largura; i++){
      matriz[i] = malloc (altura * sizeof(pixel));
    }

    for(int k=0; k<altura; k++){
      for(int l=0; l<largura; l++){
        fscanf(imagem, "%d %d %d", &matriz[l][k].r, &matriz[l][k].g, &matriz[l][k].b);
      }
    }
  
  }

  if(strcmp(id, "P6") == 0){

    while(!feof(imagem)){
      pixel p;
      fread(&p, sizeof(struct pixel), 1, imagem);
      printf("%d %d %d\n", p.r, p.g, p.b);
    }
  
  }

  //fechar arquivos
  fclose(imagem);
}

int main(int argc, char *argv[]){

  if (argc != 2){
    printf("Erro ao passar os parâmetros!\n");
    return 0;
  }

  char nome_imagem[250];  

  strcpy(nome_imagem, argv[1]);

  le_imagem_ppm(nome_imagem);

  return 0;
}