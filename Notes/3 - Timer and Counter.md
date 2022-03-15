# 3 - Timer / Counter

O temporizador do computador, também conhecido como i8254, é um dos mais básicos tipos de harware que podemos programar na linguagem C. <br>
Cada computador tem a funcionalidade de medir um tempo preciso, por exemplo, um processo e a rapidez do processador. <br>

#### Existem assim 3 contadores de 16 bits. Cada um:

- pode contar de forma binária (com zeros e uns somente), ou usando o sistema BCD (*binary-coded decimal*);
- tem 6 modos de contagem, todos eles começam com um valor inicial e decrescem até chegar a zero. O modo a adoptar para as aulas laboratoriais e para a cadeira num todo é o Mode 3 (0b101), que é "Square Wave Generator", uma vez que até metade do tempo está a 1 e na outra metade está a 0, recuperando o valor original no final de cada período;
- os contadores têm portas 0x40, 0x41 e 0x42 correspondentemente;
- tem um registo de controlo na porta 0x43;

## Registo de Controlo (0x43)


