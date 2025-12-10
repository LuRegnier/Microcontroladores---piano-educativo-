# piano educativo

- **programa de um piano educativo feito em python e arduino**

A idéia é criar um teclado educativo para quem não sabe tocar o instrumento. Ele contará com um Display Touch que mostrará uma pequena mão, que em cada dedo da mão terá um número. Ao lado da mão, terá um mini teclado, representando o teclado real. Nele, terão os mesmos números que estão na mão, demonstrando qual (ou quais) tecla(s) deverão ser tocadas no momento.

No teclado real, terão 12 teclas (representando as notas sonoras) que contarão com LED’s RGB posicionadas acima das teclas, representando com cores (Vermelho,Amarelo e Verde) quando serão tocadas.
A cor vermelha irá representar erro. Ou seja, caso a pessoa que esteja aprendendo segure a tecla por tempo demais, não toque a tecla certa no tempo certo ou toque a tecla errada, a luz vermelha irá surgir no LED.

A amarela representa atenção. A pessoa precisa ficar atenta na qual tecla irá piscar a cor amarela, pois será a próxima que deve ser tocada. Caso a pessoa segure por um tempo a tecla certa, ela ficará amarela, representando que deve ser largada. Se ficar tempo demais, o LED ficará vermelho e uma “música de derrota” será tocada, representando que a pessoa falhou na música e deve recomeçar.

Finalmente, a tecla verde representa acerto. Quando a pessoa toca a tecla no tempo certo, na ordem certa, LED’s verdes começarão a aparecer, demonstrando que ela está tocando a música corretamente.


- **Arquitetura geral do projeto:**

O projeto conecta o código em Python ao piano físico (Arduino) através da comunicação via serial, funcionando com prints na serial vindos tanto do arduino (qual tecla foi apertada ou solta) Quanto do python (Qual tecla deve ser apertada no modo aluno, enviar cor vermelha caso não tenha sido apertada dentro de 5 segundos). O Python atua como o cérebro que processa o som, enquanto o Arduino age como a interface física. 

Quando você toca uma tecla, o Arduino detecta os toques nas teclas e envia comandos como "DO_ON" para a serial, enquanto o Python utiliza uma Thread paralela para ler essas mensagens na serial e processando a string recebida para gerar o som MIDI e, quando necessário (no Modo Aluno), enviando comandos de volta (como "DO_YELLOW") para que o Arduino acione os LEDs e desenhe uma bolinha amarela na tela.


- **Drive com o arquivo .fzz mostrando a conexão dos componentes no arduino:**

https://drive.google.com/drive/folders/1IXoEkHow1AXueu0PAG34DisQG5hfJOlb?usp=drive_link

