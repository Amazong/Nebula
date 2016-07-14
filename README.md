# iOS - Instrument Outlet Simulator
## The one place for all your instrument needs.


### Introdução
O iOS - Instrument Outlet Simulator é um jogo de simulação. Nele, o jogador toma o controlo de um conjunto de lojas de música. Comprando e vendendo instrumentos, e gerindo o pessoal, o objetivo é não ir à falência!

### Funcionamento
Quando o utilizador inicia o jogo pela primeira vez, será conduzido a um menu. Aqui, poderá tomar uma das seguintes decisões:

1. **Criar um novo jogo** - neste caso, o utilizador irá configurar o seu perfil, e será levado a um ecrã onde poderá configurar a sua loja inicial e comprar instrumentos, para que possa então dar início ao seu jogo.
2. **Carregar um jogo** - carrega um ficheiro gravado.
3. **Opções** - permitem controlar a música.
4. **Sair** - fecha o jogo

Foram ainda configurados os seguintes atalhos:
- alt+m - **Mute** - silencia a música
- alt+s - **Skip** - salta a música
- alt+j - baixa o volume
- alt+k - aumenta o volume

### Estruturas de Dados
No desenvolvimento deste jogo, utilizámos vectors, lists, stacks, deques e arrays. Existe uma entitade que gere os estados do jogo. Nela existe uma pilha, onde se encontram os diferentes estados actualmente carregados. Estes fazem a ligação com a interface gráfica.

Internamente, existe um perfil de utilizador, que contém uma lista de lojas. Uma destas é a principal, mas cada uma delas tem uma lista de instrumentos, uma lista de funcionários. A classe "instrumento" é uma classe abstrata que possui duas classes derivadas - a classe guitarra e a classe piano.

Todas estas estruturas são dinamicamente alocadas, e possuem propriedades que as tentam tornar tão diversificadas como possível.

### Algoritmos
Vários algoritmos para geração aleatória de classes foram implementados. Adicionalmente, o algoritmo de compra também é de interesse, uma vez que tira partido das propriedades das restantes estruturas de dados.

O algoritmo de gravação de jogo é modular, permitindo guardar para cada loja os seus instrumentos e empregados em ficheiros distintos. Isto permite um processo de gravação mais rápido, eficaz e prático.

Foi desenvolvido um algoritmo de encriptação, que se utilizou para encriptar os ficheiros de gravação de jogo.

### Listas de texto
As listas foram extraídas de sites por um procedimento automático desenvolvido por nós, que optámos por não incluir no resultado final.

### Música
O Elvis gravou a música que se pode ouvir durante o jogo. O João editou. Esta música corre ao longo de todo o jogo, através de um player que implementámos numa thread separada.

### Conclusão
Apesar de infelizmente não termos conseguido entregar o projeto na data final sem erros na parte gráfica, o funcionamento do jogo está maioritariamente completo. Adicionalmente, a implementação das partes que não conseguimos incluir nesta versão que enviamos seria em grande medida análoga a implementações que foram já feitas.
Independentemente da avaliação, tencionamos continuar o desenvolvimento deste projecto, que se revelou muito enriquecedor para ambos os membros que o desenvolveram.


### Tecnologias Utilizadas

##### Para o código:
- **SFML**.
- **GitHub**, para controlo de versões e monitorização de bugs.
- **Visual Studio 2015** como IDE de C++.
 
##### Para colaboração:
- **Google Docs/Drawings** para colaboração em tempo real e planeamento.
- **Slack** para brainstorming, arquivo, comunicação geral.

##### Para design:
- **Photoshop** para desenhar mockups e edição de fotos.
- **Marvel** para sincronização de mockups e experimentação de UX.

##### Para edição de música:
- **Adobe Audition**, para edição de música.
 

### Fontes:
##### Código:
- C++ e STL - http://www.cplusplus.com/reference/
- Documentação oficial do SFML - http://www.sfml-dev.org/documentation/2.3.2/
- Conceitos introdutórios para desenho de jogos 2D:
  - http://www.binpress.com/tutorial/creating-a-city-building-game-with-sfml-part-1-state-manager/123
  - http://www.gamefromscratch.com/page/Game-Development-Tutorial-Series.aspx
- StackOverflow

##### Design:
- Esquemas de cores - https://material.google.com/style/color.html

##### Guitarras:
- http://burnerguitars.com/portfolio/011-ms-black-ebony-black-sitka-spruce/
- http://www.fayguitars.com/Guitars/Gallery/recent.html
- http://www.hatcherguitars.com/gallery/nggallery/page/2

##### Tipos de letra:
- Roboto
