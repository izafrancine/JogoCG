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

Ajuste os caminhos `C:/freeglut/include` e `C:/freeglut/lib` abaixo para onde o FreeGLUT está instalado na sua máquina.

### Compilar

```
gcc src/main.c -o jogo.exe -IC:/freeglut/include -LC:/freeglut/lib -lfreeglut -lopengl32 -lglu32 -lwinmm -mwindows

```

### Compilar e rodar

```
gcc src/main.c -o jogo.exe -IC:/freeglut/include -LC:/freeglut/lib -lfreeglut -lopengl32 -lglu32 -lwinmm -mwindows && ./jogo.exe

```

### Rodar

```
./jogo.exe

```

### Limpar o executável gerado

```
rm -f jogo.exe

```


## Estrutura

```
jogo-cg/
├── src/main.c       -> código principal
<<<<<<< HEAD
├── include/          -> headers extras (ainda não temos nada aqui)

```



=======
├── include/          -> headers extras (ainda não tem nada aqui)

```
>>>>>>> 98ea9efdc56efb9bc604f62adde1348b5659ac0c
