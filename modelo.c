/*
Lista 1 - Computação Gráfica (CIC270)

Rafael Greca Vieira - 2018000434
Vitor Siqueira Lobão - 2018004809
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

//irá armazenar os valores RGB de cada pixel
typedef struct pixel{
  float r, g, b;

}pixel;

/* Variáveis globais */

int type_primitive = GL_POINTS;
int largura_imagem;
int altura_imagem;
int win_width  = 800;
int win_height = 600;
int program;
unsigned int VAO;
unsigned int VBO;

/* Vertex shader */
const char *vertex_code = "\n"
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"\n"
"out vec3 vColor;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position, 1.0);\n"
"    vColor = color;\n"
"}\0";

/* Fragment shader. */
const char *fragment_code = "\n"
"#version 330 core\n"
"\n"
"in vec3 vColor;\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(vColor, 1.0f);\n"
"}\0";

/* Funções */

//ignora os comentários das imagens (máximo uma linha por vez)
void le_comentarios(FILE *imagem){

  char comentario;

  comentario = fgetc(imagem);

  if(comentario == '#'){
    
    //le até terminar a linha do comentário
    do{
      comentario = fgetc(imagem);
    }while(comentario != '\n');

  }

  //volta uma posição
  fseek(imagem, -1, SEEK_CUR);
}

/*Lê a imagem que é passada como parâmetro e retorna sua matriz de pixels*/
pixel **le_imagem(char nome_imagem[]){ 

  char id[2];
  int valor_maximo_pixel;
  pixel **matriz;

  //Abre a imagem
  FILE *imagem = fopen(nome_imagem, "rb");
  if(!imagem){
    printf("Problema com o arquivo. \n");
    exit(-1);
  }

  //P2 e P3 = imagem que está no formato texto (ASCII)
  //P5 e P6 = imagem que está no formato binário
  //PBM (preto e branco)
  //PGM (colorida)
  fscanf(imagem, "%s\n", id);
  le_comentarios(imagem);
  fscanf(imagem, "%d %d\n", &largura_imagem, &altura_imagem);
  printf("%d %d\n", largura_imagem, altura_imagem);
  le_comentarios(imagem);
  fscanf(imagem, "%d\n", &valor_maximo_pixel);
  le_comentarios(imagem);

  //verifica se a imagem é maior que a janela
  if((largura_imagem > win_width) || (altura_imagem > win_height)){

    printf("A resolucao da imagem eh maior que a da janela!\n");
    exit(-1);
  }

  //verifica se a imagem possui um cabeçalho válido
  if(largura_imagem > 0 && altura_imagem > 0 && valor_maximo_pixel <= 255){

    int tamanho = strlen(nome_imagem);

    matriz = malloc (altura_imagem * sizeof(pixel));

    //aloca a matriz
    for(int i=0; i<altura_imagem; i++){
      matriz[i] = malloc (largura_imagem * sizeof(pixel));
    }
    
    //verifica se o identificador é p2
    if(strcmp(id, "P2") == 0){

      for(int i=0; i<altura_imagem; i++){
        for(int j=0; j<largura_imagem; j++){
          float rgb;
          fscanf(imagem, "%f", &rgb);

          //insere os valores rgb já normalizados (com valor entre 0 e 1)
          matriz[i][j].r = rgb/valor_maximo_pixel;
          matriz[i][j].g = rgb/valor_maximo_pixel;
          matriz[i][j].b = rgb/valor_maximo_pixel;
        }
      }
      
      return matriz;   
    }

    //verfica se o identificador é p3
    if(strcmp(id, "P3") == 0){
      for(int i=0; i<altura_imagem; i++){
        for(int j=0; j<largura_imagem; j++){
          float r, g, b;
          fscanf(imagem, "%f %f %f", &r, &g, &b);

          //insere os valores rgb já normalizados (com valor entre 0 e 1)
          matriz[i][j].r = r/valor_maximo_pixel;
          matriz[i][j].g = g/valor_maximo_pixel;
          matriz[i][j].b = b/valor_maximo_pixel;
        }
      }
      
      return matriz;
    }

    //verifica se o identificador é p5
    if(strcmp(id, "P5") == 0){
      for(int i=0; i<altura_imagem; i++){
        for(int j=0; j<largura_imagem; j++){
          int rgb;
          fread(&rgb, 1, 1, imagem);

          //insere os valores rgb já normalizados (com valor entre 0 e 1)
          matriz[i][j].r = rgb/(1.0 * valor_maximo_pixel);
          matriz[i][j].g = rgb/(1.0 * valor_maximo_pixel);
          matriz[i][j].b = rgb/(1.0 * valor_maximo_pixel);
        }
      }
      
      return matriz;
    }

    //verifica se o identificador é p6
    if(strcmp(id, "P6") == 0){
      for(int i=0; i<altura_imagem; i++){
        for(int j=0; j<largura_imagem; j++){
          unsigned char currentPixel[3];
          fread(currentPixel, 3, 1, imagem);

          //insere os valores rgb já normalizados (com valor entre 0 e 1)
          matriz[i][j].r = currentPixel[0]/(1.0 * valor_maximo_pixel);
          matriz[i][j].g = currentPixel[1]/(1.0 * valor_maximo_pixel);
          matriz[i][j].b = currentPixel[2]/(1.0 * valor_maximo_pixel);
        }
      }
      
      return matriz;
    }

  }else{
    printf("Imagem invalida!\n");
    exit(-1);
  }

  //fecha o arquivo da imagem
  fclose(imagem);
}

//normaliza a coordenada do eixo x
float normaliza_eixo_x(float coordenada){

  return ((coordenada/(largura_imagem - 1.0f)) * 2.0f - 1.0f);
}

//normaliza a coordenada do eixo y
float normaliza_eixo_y(float coordenada){

  return (((altura_imagem - 1.0f - coordenada)/(altura_imagem - 1.0f)) * 2.0f - 1.0f);
}

void initData(pixel **matriz){

  //cada pixel terá 6 vértices
  //como cada vértice terá seis valores, como mostrado abaixo:
  //ordem dos valores do vértice: X, Y, Z (0.0), R, G, B
  //o tamanho do vértice será: altura_imagem * largura_imagem * 36
  float *vertices;
  int quantidade_vet = 0;

  vertices = (float *) malloc(altura_imagem * largura_imagem * 36 * sizeof(float));

  if(!vertices){
    printf("Erro ao alocar memória!\n");
    exit(-1);
  }

  for(int i=0; i<altura_imagem-1; i++){
    for(int j=0; j<largura_imagem-1; j++){

      //para cada pixel teremos 6 vértices
      //um para cada canto do quadrado (pixel) e dois repetidos para a diagonal secundária

      //vértice 1
      float vet_aux[6] = {normaliza_eixo_x(j), normaliza_eixo_y(i), 0.0, matriz[i][j].r, matriz[i][j].g, matriz[i][j].b};
      
      for(int k=0; k<6; k++){
        vertices[quantidade_vet++] = vet_aux[k];
      }

      //vértice 2
      float vet_aux2[6] = {normaliza_eixo_x(j), normaliza_eixo_y(i+1), 0.0, matriz[i+1][j].r, matriz[i+1][j].g, matriz[i+1][j].b};

      for(int k=0; k<6; k++){
        vertices[quantidade_vet++] = vet_aux2[k];
      }

      //vértice 3
      float vet_aux3[6] = {normaliza_eixo_x(j+1), normaliza_eixo_y(i), 0.0, matriz[i][j+1].r, matriz[i][j+1].g, matriz[i][j+1].b};

      for(int k=0; k<6; k++){
        vertices[quantidade_vet++] = vet_aux3[k];
      }

      //vértice 4
      float vet_aux4[6] = {normaliza_eixo_x(j), normaliza_eixo_y(i+1), 0.0, matriz[i+1][j].r, matriz[i+1][j].g, matriz[i+1][j].b};

      for(int k=0; k<6; k++){
        vertices[quantidade_vet++] = vet_aux4[k];
      }

      //vértice 5
      float vet_aux5[6] = {normaliza_eixo_x(j+1), normaliza_eixo_y(i+1), 0.0, matriz[i+1][j+1].r, matriz[i+1][j+1].g, matriz[i+1][j+1].b};

      for(int k=0; k<6; k++){
        vertices[quantidade_vet++] = vet_aux5[k];
      }

      //vértice 6
      float vet_aux6[6] = {normaliza_eixo_x(j+1), normaliza_eixo_y(i), 0.0, matriz[i][j+1].r, matriz[i][j+1].g, matriz[i][j+1].b};

      for(int k=0; k<6; k++){
        vertices[quantidade_vet++] = vet_aux6[k];
      }
    }
  }
    
  //Vertex array.
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  //Vertex buffer
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * altura_imagem * largura_imagem * 36 , vertices, GL_STATIC_DRAW);
    
  //Set attributes.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  //Unbind Vertex Array Object.
  glBindVertexArray(0);
}

void display(){

  glClearColor(0.2, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glBindVertexArray(VAO);

  //passa a quantidade de vértices que serão desenhados
	glDrawArrays(type_primitive, 0, altura_imagem * largura_imagem * 6);

	glutSwapBuffers();
}

void reshape(int width, int height){
  win_width = width;
  win_height = height;
  glViewport(0, 0, width, height);
  glutPostRedisplay();
}

//configura as teclas necessárias do teclado
void keyboard(unsigned char key, int x, int y){

  switch(key){

    case 27:
      exit(0);
    case 'q':
    case 'Q':
      exit(0);
      break;
    case 'v':
    case 'V':
      if(type_primitive == GL_POINTS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        type_primitive = GL_TRIANGLES;
      }else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        type_primitive = GL_POINTS;
      }

      break;
  }
    
	glutPostRedisplay();
}

void initShaders(){
  
  // Request a program and shader slots from GPU
  program  = glCreateProgram();
  int vertex   = glCreateShader(GL_VERTEX_SHADER);
  int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    
  // Set shaders source
  glShaderSource(vertex, 1, &vertex_code, NULL);
  glShaderSource(fragment, 1, &fragment_code, NULL);

  // Compile shaders
  glCompileShader(vertex);
  glCompileShader(fragment);

  // Attach shader objects to the program
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);

  // Link the program
  glLinkProgram(program);

  // Get rid of shaders (not needed anymore)
  glDetachShader(program, vertex);
  glDetachShader(program, fragment);
  glDeleteShader(vertex);
  glDeleteShader(fragment);
    
  // Set the program to be used.
  glUseProgram(program);
}

int main(int argc, char *argv[]){

  pixel **matriz_pixels;

  if (argc != 2){
    printf("Erro ao passar os parâmetros!\n");
    printf("Exemplo: ./modelo images/nome_da_imagem.pgm(ou ppm)");
    return 0;
  }

  char nome_imagem[250];  

  strcpy(nome_imagem, argv[1]);

  matriz_pixels = le_imagem(nome_imagem);

  glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(win_width, win_height);
	glutCreateWindow("Lista 1 - Computacao Grafica");
	glewExperimental = GL_TRUE; 
	glewInit();

  //Init vertex data for the triangle
  initData(matriz_pixels);

  //Create shaders
  initShaders();

  glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

  //libera o espaço alocado pela matriz de pixels
  free(matriz_pixels);
  return 0;
}