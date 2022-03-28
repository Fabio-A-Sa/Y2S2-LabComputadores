# 5 - Keyboard

Sempre que uma tecla do teclado é pressinada, o teclado envia um comando (**scancode**) para o KBC (*Keyboard Controller*). O código pode ser:

1. `Make code`, se a tecla for pressionada. O bit mais significativo está a 0;
2. `Break code`, se a tecla deixar de ser primida. O bit mais significativo está a 1;

Por exemplo, a tecla *ESC* tem Make Code = 0x01 e Break Code = 0x81. Assim, os códigos só diferem no MSB (ou seja, nos Most Significant Bits). O KBC coloca esse código num registo e gera uma interrupção se estiver configurado para isso. 


