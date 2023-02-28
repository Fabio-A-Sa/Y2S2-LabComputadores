# i8042, the PC's Keyboard

## Tópicos

- [O teclado comum](#teclado)
- [i8042 KBC](#i8042-kbc)
- [Interrupções](#interrupções)
- [Polling](#polling)
- [Compilação do código](#compilação-do-código)
- [Testagem do código](#testagem-do-código)

## Teclado

Como vimos no capítulo anterior há sempre necessidade de generalizar o hardware para poder ser usado com diversas máquinas. O teclado comum não foge da regra. Desta vez o desafio é diferente: há no mundo imensos fabricantes de teclados, com diferentes teclas, diferentes línguas e constituições. Como é possível generalizar todas estas combinações?

Quando pressionamos ou soltamos uma tecla geramos um `scancode`, que é um código geralmente de 8 bits (1 byte) que caracteriza não o significado da tecla mas sim a posição desta no teclado. Independentemente da língua, do fabricante ou do número de teclas, a mesma posição gera sempre o mesmo scancode. <br>
Este código é depois processado pelo i8042/KBC e interpretado pelo CPU. Antes de mostrar o output o CPU consulta a linguagem escolhida no sistema para fazer a tradução. Assim, quando trocamos a língua do teclado com recurso a software estamos na realidade a trocar o `keymap` da língua, implementado recorrendo por exemplo a *hashmaps* com chaves de bytes e valores em ASCII:

<p align="center">
  <img src="../../Images/Teclado.png">
  <p align="center">Tratamento do output do teclado</p>
</p><br>

Um scancode pode ter duas formas:
- `makecode`: código gerado quando pressionamos a tecla;
- `breakcode`: código gerado quando soltamos a tecla;

Geralmente o breakcode difere apenas no bit mais significativo do makecode original. Esta diferença fica mais nítida no caso anterior:

```c
uint8_t A_makecode  = 0x1E // 00011110
uint8_t A_breakcode = 0x9E // 10011110
```

Uma possível implementação de uma função que faz a distinção dos códigos:

```c
void evaluate_scancode(uint8_t scancode) {
    if (scancode & BIT(7)) {
        printf("%02x, breakcode!\n", scancode);
    } else {
        printf("%02x, makecode!\n", scancode);
    }
}
```

Algumas teclas possuem scancode de 2 bytes. Nesse caso é quase certo que o primeiro byte recebido seja `0xE0`.
Na versão que vamos utilizar em LCOM o Minix contém a linguagem portuguesa.

## i8042 KBC

O i8042 ou KBC (*KeyBoard Controller*) é o controlador do teclado do computador e do rato com dois botões. Neste lab focaremos apenas na funcionalidade do teclado. Este dispositivo funciona de forma parecida ao timer, havendo interação a partir da escrita e leitura de registos:

<p align="center">
  <img src="../../Images/i8042.png">
  <p align="center">Funcionamento do i8042</p>
</p><br>

Desta vez temos a possibilidade de ler diretamente o status do dispositivo. Esse estado é constituído por 8 bits (uint8_t) e permite obter algumas informações relevantes:
- Se ocorreu um erro de `paridade`, o bit 7 está ativo;
- Se ocorreu um erro de `timeout`, o bit 6 está ativo;
- Se o buffer de entrada (*input buffer*) estiver cheio, o bit 1 está ativo;
- Se o buffer de saída (*output buffer*) estiver cheio, o bit 0 está ativo;

### Exemplo 1

Queremos saber se num determinado momento o buffer de input está cheio. Uma possível implementação seria a seguinte:

```c
uint8_t status;
util_sys_inb(0x64, &status);
if (status & BIT(1)) {
    printf("Full input buffer\n");
}
```

Dá ainda para enviar comandos a partir do *input buffer* disponível em 0x64 mas de uma forma diferente do timer. Agora há dois problemas a ter em conta:
- O buffer de entrada é finito, poderá estar cheio e nesse caso não é possível inserir um comando de controlo;
- O KBC (*keyboard controller*) é um pouco lento, na ordem dos milissegundos, pelo que é indispensável tentar algumas vezes todas as operações efetuadas com este dispositivo;

Uma boa solução é criar um ciclo de tentativas finitas e esperar alguns milissegundos entre fracassos. Normalmente 10 tentativas e 20 milissegundos entre cada uma é suficiente para ter sucesso na operação. **Só é possível escrever no controlador quando o buffer de entrada não está cheio**:

```c
int write_KBC_command(uint8_t port, uint8_t commandByte) {

    uint8_t status;
    uint8_t attemps = 10;

    while (attemps) {

        if (read_KBC_status(&status) != 0){             // lê o status
            printf("Error: Status not available!\n");
            return 1;
        }

        if ((status & BIT(1)) == 0){                    // o input buffer não está cheio, posso escrever
            if(sys_outb(port, commandByte) != 0){       // tentativa de escrita
                printf("Error: Could not write commandByte!\n");
                return 1;
            }
            return 0; // sucesso: comando inserido no i8042
        }
        tickdelay(micros_to_ticks(20000));              
        attemps--;
    }
    return 1; // se ultrapassar o número de tentativas lança um erro
}
```

A função `tickdelay()` assegura o intervalo correcto entre tentativas de acordo os *ticks* do processador. A função `micros_to_ticks()` traduz um número inteiro de microsseguros em *ticks*.

Da mesma forma, dá para ler os caracteres pressionados no teclado graças ao buffer de saída. Note-se agora que a informação disponibilizada pelo i8042 é só fiável quando estiver completamente no buffer, ou seja, **só deve ser lida quando o output buffer estiver cheio**. O status indica se há algum erro ao nível da paridade ou de timeout. Nesse caso os bytes devem ser lidos para sairem da fila/buffer mas serem descartados:

```c
int read_KBC_output(uint8_t port, uint8_t *output) {

    uint8_t status;
    uint8_t attemps = 10;
    
    while (attemps) {

        if (read_KBC_status(&status) != 0) {                // lê o status
            printf("Error: Status not available!\n");
            return 1;
        }

        if ((status & BIT(0)) != 0) {                       // o output buffer está cheio, posso ler
            if(util_sys_inb(port, output) != 0){            // leitura do buffer de saída
                printf("Error: Could not read output!\n");
                return 1;
            }
            if((status & BIT(7)) != 0){                     // verifica erro de paridade
                printf("Error: Parity error!\n");           // se existir, descarta
                return 1;
            }
            if((status & BIT(6)) != 0){                     // verifica erro de timeout
                printf("Error: Timeout error!\n");          // se existir, descarta
                return 1;
            }
            return 0; // sucesso: output lido sem erros de timeout ou de paridade
        }
        tickdelay(micros_to_ticks(20000));
        attemps--;
    }
    return 1; // se ultrapassar o número de tentativas lança um erro
}
```

## Interrupções

O i8042 também dá origem a interrupções sempre que uma tecla é pressionada. O dispositivo está disponível na IRQ_LINE 1 e as interrupções são ativadas e eliminadas com comandos semelhantes ao Timer:

```c
/* ------ i8042.h ------ */
#define KEYBOARD_IRQ 1;   

/* ------ kbc.c ------ */
int keyboard_hook_id = 1;

// subscribe interrupts
int keyboard_subscribe_int (uint8_t *bit_no) {
  if(bit_no == NULL) return 1;   // o apontador tem de ser válido
  *bit_no = BIT(timer_hook_id);  // a função que chamou esta deve saber qual é a máscara a utilizar
                                 // para detectar as interrupções geradas
  return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id); // subscrição das interrupções em modo exclusivo
}

// unsubscribe interrupts
int keyboard_unsubscribe_int () {
  return sys_irqrmpolicy(&timer_hook_id); // desligar as interrupções
}
```

Note-se a diferença no segundo argumento da *policy*. Como vimos em cima o i8042 controla o teclado e o rato. Como não queremos receber por este canal as interrupções que possam aparecer do rato, então declaramos estas interrupções como **exclusivas**, IRQ_EXCLUSIVE.

## Polling

// com o teclado dá para polling com um simples ciclo while
// para o teste da segunda função do lab3 o Minix3 desativa as interrupções, é necessário voltar a ativá-las antes de terminar, usando o kbc_restore()

## Compilação do código

// semelhante ao lab2

## Testagem do código

// semelhante ao lab2

---

@ Fábio Sá <br>
@ Fevereiro de 2023