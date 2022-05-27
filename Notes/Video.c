vbe_mode_info_t videoInfo;
uint8_t * videoMemBaseAdd;

int videoInitialize(uint16_t mode){
  reg86_t videoReg;                         // declarar uma struct de um tipo específico
  memset(&videoReg, 0, sizeof(videoReg));   // garantir que ela começa vazia
  videoReg.ax = 0x4F02;                     // carregar 16 bits
  videoReg.bx = mode | BIT(14);             // o registo B fica com o modo e o bit 14 carregados
  videoReg.intno = 0x10;                    // intno fica com valor 2
  if (sys_int86(&videoReg)) return 1;       // carregamos as configurações com uma system call
  return 0;
}

int videoMemInitialize(uint16_t mode){
  memset(&videoInfo, 0, sizeof(videoInfo));                     // garantimos que a informação começa vazia
  if( vbe_get_mode_info(mode, &videoInfo)) return 1;            // vamos buscar a configuração atual com a system call
  struct minix_mem_range memRange;                              // inicializamos um vector de memória
  memRange.mr_base = (phys_bytes) videoInfo.PhysBasePtr;        // a base corresponde ao que está na informação acerca da VBE
  int videoRamSize = videoInfo.XResolution * videoInfo.YResolution * videoInfo.BitsPerPixel / 8;        // cálculo do tamanho
  memRange.mr_limit = memRange.mr_base + videoRamSize;          // o endereço limite = base + size, assim como em sistemas operativos

  if(sys_privctl(SELF,SYS_PRIV_ADD_MEM, &memRange)) return 1;   // é importante obter permissões para manipular a memória da VBE
  videoMemBaseAdd = vm_map_phys(SELF, (void *) memRange.mr_base, videoRamSize);     // mapear o endereço físico para o apontador global
  if(videoMemBaseAdd == MAP_FAILED) return 1;                   // se não correr bem, avisar
  return 0;
}

int drawPixel(uint16_t x, uint16_t y, uint32_t color) {
  if( x > videoInfo.XResolution || y > videoInfo.YResolution) return 0;     // ver se é possível colocar o píxel atendendo aos limites
  unsigned bytesPerPixel = (videoInfo.BitsPerPixel + 7) / 8;                // cálculo dos bytes por pixel
  memcpy( &videoMemBaseAdd[ (videoInfo.XResolution * y + x) * bytesPerPixel], &color, bytesPerPixel);   // usar uma cópia de memória. Coloca naquele
                                                                                                        // sítio do array exatamente a quantidade de 
                                                                                                        // bytes por pixel da cor, em referência
  return 0;
}

int drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for(unsigned column = 0; column < height; column++){
    for(unsigned line = 0; line < width; line++){
      if(drawPixel(x+line,y+column,color)) return 1; // a drawPixel() é chamada exatamente width * height píxeis, a área do objecto a imprimir
    }
  }
  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (videoInitialize(mode)) return 1;              // inicializa o modo
  sleep(delay);                                     // espera o tempo definido                     
  if(vg_exit()) return 1;                           // volta ao modo de texto de default do minix
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

  if(videoMemInitialize(mode)) return 1;                        // inicializa a memória
  if(videoInitialize(mode)) return 1;                           // inicializa o tipo de vídeo
  if(drawRectangle(x, y, width, height, color)) return 1;       // desenha o rectangulo com aquelas características
  sleep(10);                                                    // espera o tempo definido
  if(vg_exit()) return 1;                                       // volta ao modo de texto de default do minix
  return 0;
}

int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    
    xpm_image_t img;                                            // declarar este tipo de struct
    uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);         // em modo indexado -> cada cor tem 1 byte

    for (int h = 0 ; h < img.height ; h++) {              
        for (int w = 0 ; w < img.width ; w++) {
        if (vg_draw_pixel(x + w, y + h, *colors) != 0) return 1; // draw do pixel, passando a cor correspondente
        colors++;                                                // passar para a cor seguinte do vector
        }
    }

    return 0;
}
