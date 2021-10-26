## Jogo da Memória (Projeto 1) - Computação Gráfica 2021.1

Made by: **Gabriel Zolla Juarez- RA 11201721446*

**Link do repositório do código-fonte:** https://github.com/EduardaMeirinhos/abcg/edit/main/examples/memorygame/
**Link para uma página Web com a aplicação rodando em WebAssembly**: https://eduardameirinhos.github.io/play-memory-game/memory-game/

Este repositório contém o código-fonte da implementação do Jogo da Memória, desenvolvido como Projeto 1 para a disciplina de Computação Gráfica, em 2021.1, ministrada pelo Prof. Harlen Batagelo.

O projeto foi desenvolvido utilizando a biblioteca **abcg** e o **ImGui**. O código está baseado nas implementações dos jogos TicTacToe e Asteroids, disponibilizados na página da disciplina. Para tanto, foi criado um grid 4x4 compostos por botões opacos, representando o tabuleiro e compondo a base de interação do usuário com a interface. Tais botões também são feedbacks visuais para informar quais posições do tabuleiro ainda são interativos.

A lógica do jogo se baseia em encontrar os pares de cards (figuras), dois a dois, até que todas as figuras estejam visíveis. As figuras são triângulos renderizados através do pipeline do OpenGL, criando as primitivas a partir dos Vertex Shaders e diferenciando-os pela cor, definindo os dados da textura. 

O jogo implementado oferece três níveis de dificuldade:
- **Fácil:** Possui triângulos de cor sólida (os três vértices possuem o mesmo vetor RGBA).
- **Médio:** Possui triângulos degradê, utilizando duas cores (dois dos vértices possuem o mesmo vetor RGBA).
- **Difícil:** Possui triângulos degradê, utilizando três cores (todos os vértices possuem vetores RGBA distintos).

Ao vencer o jogo, o contador de tempo é interrompido e demonstra o tempo decorrido naquela partida. Para iniciar um novo jogo, basta selecionar a dificuldade através do menu presente na topbar da janela.

*A cada novo jogo, as cores e a posição dos cards são gerados automaticamente.*