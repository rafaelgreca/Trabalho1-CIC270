#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

typedef struct coordenadas{
  //irá armazenar os valores RGB de cada pixel
  int x, y, z;

}coordenadas;

typedef struct pixel{
  //irá armazenar os valores RGB de cada pixel
  int r, g, b;

}pixel;

/* Variáveis globais */

char img[168];

/* Largura da janela */
int win_width  = 800;
/* Altura da janela */
int win_height = 600;
/* Variável programa */
int programa;
/* Vertex array */
unsigned int VAO;
/* Vertex buffer */
unsigned int VBO;

/* Variável tipo primitivo */
int primitivo = GL_POINTS;

/* Vertex shader */
const char *vertex_code = "\n"
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

/* Fragment shader. */
const char *fragment_code = "\n"
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\0";

/* Funções */
 
void display(){

  //glClearColor(0.2, 0.3, 0.3, 1.0);
  //glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(programa);
  //glBindVertexArray(VAO);
  //glDrawArrays(primitivo, 0, 4);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 24, 7, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
  //glutSwapBuffers();
}

void reshape(int largura, int altura){

  glViewport(0, 0, largura, altura);
  glutPostRedisplay();

}

//configura as teclas necessárias do teclado
void keyboard(unsigned char key, int x, int y){

  switch (key){

    case 'q':
    case 'Q':
      exit(0);
      break;
    case 'v':
    case 'V':
      primitivo = GL_LINES;
      break;
    default:
      break;
  }
    
	glutPostRedisplay();
}

/*Lê a imagem que é passada como parâmetro
e armazena seus pixels em uma matriz*/
int le_imagem(char nome_imagem[]){

  char id[2];
  int largura, altura, valor_maximo_pixel;
  pixel **matriz;
  coordenadas **mat;

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

    int tamanho = strlen(nome_imagem);
    const char *extensao = &nome_imagem[tamanho-3];

    if(strcmp(extensao, "pgm") == 0){
    
      matriz = malloc (largura * sizeof(pixel));
      mat = malloc (largura * sizeof(coordenadas));

      if((strcmp(id, "P2") == 0) || (strcmp(id, "P3") == 0)){

        //aloca as matrizes
        for(int i=0; i<largura; i++){
          matriz[i] = malloc (altura * sizeof(pixel));
          mat[i] = malloc (altura * sizeof(coordenadas));
        }

        for(int k=0; k<altura; k++){

          for(int l=0; l<largura; l++){
            fscanf(imagem, "%d", &matriz[l][k].r);
            img[largura * k + l] = matriz[l][k].r/valor_maximo_pixel;
          }

        }
      
      }

      if((strcmp(id, "P6") == 0) || (strcmp(id, "P5") == 0)){

        for(int k=0; k<altura; k++){

          for(int l=0; l<largura; l++){
            pixel p;
            fread(&p, 1, 1, imagem);
          }
        
        }

      }

    }else{

      if(strcmp(extensao, "ppm") == 0){
    
        matriz = malloc (largura * sizeof(pixel));
        mat = malloc (largura * sizeof(coordenadas));

        if((strcmp(id, "P2") == 0) || (strcmp(id, "P3") == 0)){

          //aloca as matrizes
          for(int i=0; i<largura; i++){
            matriz[i] = malloc (altura * sizeof(pixel));
            mat[i] = malloc (altura * sizeof(coordenadas));
          }

          for(int k=0; k<altura; k++){

            for(int l=0; l<largura; l++){
              fscanf(imagem, "%d %d %d", &matriz[l][k].r, &matriz[l][k].g, &matriz[l][k].b);
            }

          }
        
        }

        if((strcmp(id, "P6") == 0) || (strcmp(id, "P5") == 0)){

          for(int k=0; k<altura; k++){

            for(int l=0; l<largura; l++){
              pixel p;
              fread(&p, 1, 1, imagem);
            }

          }

        }

     }else{

       printf("Extensão invalida\n");
       exit(-1);

     }

    }

  }

  //fecha o arquivo da imagem
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
	glutInitWindowSize(win_width, win_height);
	glutCreateWindow("Lista 1 - Computacao Grafica");
	glewExperimental = GL_TRUE; 
	glewInit();

  glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
  return 0;
}