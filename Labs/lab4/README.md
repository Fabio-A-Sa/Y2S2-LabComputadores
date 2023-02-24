# i8042, the PC's Mouse

## Tópicos

- [Funcionamento do Rato](#rato)
- [i8042 Mouse](#i8042-mouse)
- [Interrupções](#interrupções)
- [Máquinas de Estado em C](#maquinas-de-estado-em-c)
- [Compilação do código](#compilação-do-código)
- [Testagem do código](#testagem-do-código)

## Rato

O Sistema Operativo por padrão atribui umas coordenadas iniciais no ecrã ao cursor, por esse motivo aparece sempre na mesma posição quando ligamos o computador. Depois disso o dispositivo emite bytes descrever o valor absoluto do deslocamento no eixo X, o valor absoluto do deslocamento no eixo Y e um byte extra a indicar o sinal das duas componentes e se houve algum botão (direito ou esquerdo) pressionado no processo. Todas as seguintes posições do rato são calculadas tendo por base a soma de vetores:

<p align="center">
  <img src="../../Images/Mouse.png">
  <p align="center">Interpretação da mudança das coordenadas do cursor</p>
</p><br>

Note-se o sentido dos eixos no ecrã. De P1 para P2 houve um deslocamento positivo nos dois eixos mas de P2 para P3 o deslocamento em Y foi negativo. O deslocamento do cursor para fora do quadrante positivo dos eixos não é permitido nem possível pois as variáveis que representam as coordenadas atuais são dois inteiros sem sinal.

// soon

## i8042

Por motivos de hardware o rato é controlado pelo mesmo dispositivo do teclado: o i8042. Vamos portanto usar as mesmas funções para ler e escrever do controlador:

- A
- B


## Interrupções

// soon

## Máquinas de Estado em C

// soon

## Compilação do código

// soon

## Testagem do código

// soon

---

@ Fábio Sá <br>
@ Fevereiro de 2023