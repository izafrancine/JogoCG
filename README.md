# Jogo 3D - Computação Gráfica (UFCA)

Trabalho da disciplina Computação Gráfica — Profa. Luana Batista da Cruz.
Feito em C + OpenGL/FreeGLUT.

## Requisitos mínimos (do enunciado)
- [ ] Objetos 3D com cores distintas + materiais (propriedades ópticas)
- [ ] Iluminação: ambiente, especular, difusa
- [ ] Câmera para navegação/visualização
- [ ] Projeção perspectiva e/ou ortogonal
- [ ] Remoção de superfícies ocultas (depth test)

## Pontos extras (0,5 cada)
- [ ] Importação de objeto (ex: .obj)
- [ ] Texturas nos objetos
- [ ] Projeção de sombra dos objetos

## Como compilar (Git Bash + MinGW + FreeGLUT)

Antes de tudo, abra o arquivo `build.sh` e ajuste os caminhos do FreeGLUT para onde ele está instalado na sua máquina:

```
FREEGLUT_INCLUDE="C:/freeglut/include"
FREEGLUT_LIB="C:/freeglut/lib"
```

Depois, no Git Bash, dê permissão de execução (só precisa fazer isso uma vez):

```
chmod +x build.sh
```

### Compilar

```
./build.sh
```

### Compilar e rodar

```
./build.sh run
```

### Limpar o executável gerado

```
./build.sh clean
```

### Compilando manualmente (sem o script)

```
gcc src/main.c -o jogo.exe -IC:/freeglut/include -LC:/freeglut/lib -lfreeglut -lopengl32 -lglu32 -lwinmm -mwindows
./jogo.exe

```

## Estrutura

```
jogo-cg/
├── src/main.c       -> código principal
├── include/          -> headers extras (ainda não tem nada aqui)

```
