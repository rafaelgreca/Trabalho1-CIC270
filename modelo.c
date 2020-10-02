#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

typedef struct pixel{
  //irá armazenar os valores RGB de cada pixel
  int r, g, b;

}pixel;

/* Variáveis globais */

/** Largura da janela */
int win_width  = 800;
/** Altura da janela */
int win_height = 600;
/** Variável programa */
int program;
/** Vertex array */
unsigned int VAO;
/** Vertex buffer */
unsigned int VBO;

/*Funções*/

/*Lê a imagem que é passada como parâmetro
e armazena seus pixels em uma matriz*/
int le_imagem(char nome_imagem[]){

  char id[2];
  int largura, altura, valor_maximo_pixel;
  pixel **matriz;

  //Abre a imagem ppm
  FILE *imagem = fopen(nome_imagem, "rb");
  if(!imagem){
    printf("Problema com o arquivo. \n");
  }

  //P3 = imagem que está no formato texto (ASCII)
  //P6 = imagem que está no formato binário
  fscanf(imagem, "%s\n", id);
  fscanf(imagem, "%d %d\n", &largura, &altura);
  fscanf(imagem, "%d\n", &valor_maximo_pixel);

  if(largura > 0 && altura > 0 && valor_maximo_pixel <= 255){

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

    if((strcmp(id, "P6") == 0) || (strcmp(id, "P5") == 0)){

      fgetc(imagem);

      while(!feof(imagem)){
        pixel p;
        fread(&p, sizeof(struct pixel), 1, imagem);
        printf("%d %d %d", p.r, p.g, p.b);
      }
    
    }
  }

  //fechar arquivo da imagem
  fclose(imagem);
}

int main(int argc, char *argv[]){

  if (argc != 2){
    printf("Erro ao passar os parâmetros!\n");
    return 0;
  }

  char nome_imagem[250];  

  strcpy(nome_imagem, argv[1]);

  le_imagem(nome_imagem);

  glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(win_width,win_height);
	glutCreateWindow(argv[0]);
	glewExperimental = GL_TRUE; 
	glewInit();

  return 0;
}