# i8042, the PC's Keyboard

## Tópicos

- [O que é o i8042](#i8042)
- [Interrupções](#interrupções)
- [Compilação do código](#compilação-do-código)
- [Testagem do código](#testagem-do-código)

## i8042

Controlador do teclado do computador e do rato com dois botões. Neste lab focaremos apenas na funcionalidade do teclado. Este dispositivo funciona de forma parecida ao timer, havendo interação a partir da escrita e leitura de registos:

<p align="center">
  <img src="../../Images/i8042.png">
  <p align="center">Funcionamento do i8042</p>
</p><br>

Desta vez temos a possibilidade de ler diretamente o status do dispositivo. Esse estado é constituído por 8 bits (uint8_t) e permite obter algumas informações relevantes:
- Se ocorreu um erro de `paridade`, o bit 7 está ativo;
- Se ocorreu um erro de `timeout`, o bit 6 está ativo;
- Se o buffer de entrada (*input buffer*) estiver cheio, o bit 1 está ativo;
- Se o buffer de saída (*output buffer*) estiver cheio, o bit 0 está ativo;

```c
uint8_t status;
util_sys_inb(0x64, &status);
if (status & BIT(1)) {
    printf("Full input buffer\n");
}
```

