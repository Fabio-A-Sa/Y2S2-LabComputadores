# Real Time Clock

## Tópicos

- [Funcionamento](#funcionamento)
- [Interrupções](#interrupções)
- [BCD vs Binário](#bcd-vs-binário)

## Funcionamento

O RTC (*Real Time Clock*) tem um funcionamento semelhante ao [Timer](../lab2/). Possui 6 contadores internos, cada um correspondendo a um parâmetro da data ou hora:

- Ano (0 a 99)
- Mês (1 a 12)
- Dia (1 a 31)
- Hora (0 a 24)
- Minuto (0 a 59)
- Segundo (0 a 59)

<br>
<p align="center">
  <img src="../../Images/RTC.png">
  <p align="center">Real Time Clock</p>
</p><br>

Sempre que quisermos ler algo do RTC (ler configurações ou ler os contadores internos) é preciso primeiro avisá-lo, escrevendo no registo de controlo (`0x70`) o comando adequado de 8 bits. A leitura é feita posteriormente pelo registo de output, `0x71`, também com 8 bits.

```c
int rtc_output(uint8_t command, uint8_t *output) {
    if (sys_outb(0x70, command) != 0) return 1;
	if (util_sys_inb(0x71, output) != 0) return 1;
    return 0;
}
```

A tabela seguinte mostra os comandos relevantes para LCOM:

| Comando | Leitura de...                                                                                                     |
|---------|-------------------------------------------------------------------------------------------------------------------|
| 0       | Segundos                                                                                                          |
| 2       | Minutos                                                                                                           |
| 4       | Horas                                                                                                             |
| 7       | Dias                                                                                                              |
| 8       | Meses                                                                                                             |
| 9       | Anos                                                                                                              |
| 10      | Update status. Se o BIT 7 estiver ativo o RTC está a atualizar algum dos contadores internos. internos                            |
| 11      | Counting status. Se o BIT 2 estiver ativo o RTC possui os contadores em binário, caso contrário conta em formato BCD |

Antes de qualquer leitura de um dos seis contadores internos é necessário confirmar que o RTC não está a atualizar. 

```c
int rtc_is_updating() {
    uint8_t result;
    if (rtc_output(10, &result)) return 1;
	return result & BIT(7);
}
```

Por exemplo, para atualizar o valor dos segundos:

```c

```

## Interrupções

## BCD vs Binário

---

@ Fábio Sá <br>
@ Abril de 2023