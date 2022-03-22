#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[]) {

  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {

  uint8_t configuration;                                    // variável que vai conter a configuração do timer
  int ret = timer_get_config(timer, &configuration);q       // chamar a função que preenche a configuração
  if (ret != 0) return ret;                                 // se houve erro, abortar logo a missão

  return timer_display_conf(timer, configuration, field);   // display das cofigurações segundo o field
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
    return (timer >= 0 && timer < 3) ? timer_set_frequency(timer, freq) : 1;    // retorna o valor da função de time
}                                                                               // só se timer tiver valor válido, senão != 0

int(timer_test_int)(uint8_t time) {
  
  if (timer_set_frequency(0, 60) != 0) return 1;            // inicializa o timer 0 a 60 Hz

  

  return 1;
}