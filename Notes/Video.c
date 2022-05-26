vbe_mode_info_t videoInfo;
uint8_t * videoMemBaseAdd;

int videoInitialize(uint16_t mode){
  reg86_t videoReg;
  memset(&videoReg, 0, sizeof(videoReg));
  videoReg.ax = 0x4F02;
  videoReg.bx = mode | BIT(14);
  videoReg.intno = 0x10;
  if (sys_int86(&videoReg)) return 1;
  return 0;
}

int videoMemInitialize(uint16_t mode){
  memset(&videoInfo, 0, sizeof(videoInfo));
  if( vbe_get_mode_info(mode, &videoInfo)) return 1;
  struct minix_mem_range memRange;
  memRange.mr_base = (phys_bytes) videoInfo.PhysBasePtr;
  int videoRamSize = videoInfo.XResolution * videoInfo.YResolution * videoInfo.BitsPerPixel / 8;
  memRange.mr_limit = memRange.mr_base + videoRamSize;

  if(sys_privctl(SELF,SYS_PRIV_ADD_MEM, &memRange)) return 1;
  videoMemBaseAdd = vm_map_phys(SELF, (void *) memRange.mr_base, videoRamSize);
  if(videoMemBaseAdd == MAP_FAILED) return 1;
  return 0;
}

int drawPixel(uint16_t x, uint16_t y, uint32_t color) {
  if( x > videoInfo.XResolution || y > videoInfo.YResolution) return 0;
  unsigned bytesPerPixel = (videoInfo.BitsPerPixel + 7) / 8;
  memcpy( &videoMemBaseAdd[ (videoInfo.XResolution * y + x) * bytesPerPixel], &color, bytesPerPixel);
  return 0;
}

int drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for(unsigned column = 0; column < height; column++){
    for(unsigned line = 0; line < width; line++){
      if(drawPixel(x+line,y+column,color)) return 1;
    }
  }
  return 0;
}

///////////////////////////

int(video_test_init)(uint16_t mode, uint8_t delay) {
  //graphics Initialize
  if (videoInitialize(mode)) return 1;
  //wait and exit
  sleep(delay);
  if(vg_exit()) return 1;
  //return
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

  if(videoMemInitialize(mode)) return 1;
  if(videoInitialize(mode)) return 1;
  if(drawRectangle(x, y, width, height, color)) return 1;
  sleep(10);
  if(vg_exit()) return 1;
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

  return 1;
}

int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

 return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {

  return 1;
}
