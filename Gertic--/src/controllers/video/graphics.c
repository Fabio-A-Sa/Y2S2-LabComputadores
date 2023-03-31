#include <lcom/lcf.h>
#include "graphics.h"
#include <math.h>
#include "../../game/modes/drawing.h"

uint8_t *imgBuffer; 
uint8_t *drawBuffer;
unsigned bytesPerPixel;
uint16_t xRes, yRes;
extern int drawingSize;


int (init_graphics_mode)(uint16_t mode){

  reg86_t r;
  memset(&r, 0, sizeof(r));

  r.ax = VBE_MODE_SET;
  r.bx = mode | VBE_LINEAR_FB;
  r.intno = BIOS_VIDEOCARD_SERV;

  if(sys_int86(&r) != OK){
    printf("vg_exit(): sys_int86() failed \n");
    return 1;
  }

  return 0;
}

int (map_vmem)(uint16_t mode){

  memset(&info, 0, sizeof(info));
  if(vbe_get_mode_info(mode,&info)) return 1;
  bytesPerPixel = (info.BitsPerPixel + 7) / 8;
  xRes = info.XResolution;
  yRes = info.YResolution;

  int r;
  uint8_t* video_mem;
  struct minix_mem_range mr; /* physical memory range */
  unsigned int vram_base = info.PhysBasePtr; /* VRAM’s physical address */
  unsigned int vram_size = (info.XResolution * info.YResolution * info.BitsPerPixel) / 8; /* VRAM’s size, but you can use the frame-buffer size, instead */

  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return 1;
  }

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED){
    panic("couldn’t map video memory");
    return 1;
  }

  vAddr_base = video_mem;
  return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {

  if(color == 0xfffffe) return 0;
  if(x >= xRes || y >= yRes) return 0;

  memcpy(&imgBuffer[(xRes*y + x) * bytesPerPixel], &color, bytesPerPixel);

  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  for(unsigned i = 0; i < len; i++)
    if (vg_draw_pixel(x+i,y,color)) return 1;

  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if(color == 0xfffffe) return 0;
  for(unsigned i = 0; i < height; i++)
    if (vg_draw_hline(x,y+i,width,color) != 0) {
      vg_exit();
      return 1;
    }
  return 0;
}


/* Funções auxiliares de cores */

uint32_t (R)(uint32_t color){
  return ((1 << info.RedMaskSize) - 1) & (color >> info.RedFieldPosition);
}

uint32_t (G)(uint32_t color){
  return ((1 << info.GreenMaskSize) - 1) & (color >> info.GreenFieldPosition);
}

uint32_t (B)(uint32_t color){
  return ((1 << info.BlueMaskSize) - 1) & (color >> info.BlueFieldPosition);
}

/* draw xpm */
int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  xpm_image_t img;
  uint32_t *colors = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &img);
  for (int h = 0 ; h < img.height ; h++) {
    for (int w = 0 ; w < img.width ; w++) {
      if (vg_draw_pixel(x + w, y + h, colors[w + h*img.width])) return 1;
    }
  }
  return 0;
}


// Image buffer

void allocateImgBuffer(){
    imgBuffer = (uint8_t *)malloc(xRes*yRes*bytesPerPixel); 
}

void copyBufferToVideo(){
    memcpy(vAddr_base, imgBuffer, xRes*yRes*bytesPerPixel); 
}

void clearImgBuffer(){
    memset(imgBuffer, 0, xRes*yRes*bytesPerPixel); 
}

void freeImgBuffer(){
    free(imgBuffer); 
}

int drawToCanvas(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color) {
  float m=0.0,b;
  if(xi == xf && yi == yf) return drawCircle(xi, yi, drawingSize/2, color);
  
  if(xi != xf) m = (yf-yi)/(xf-xi);

  if(xi != xf && abs(m) <= 1.0){
    if(xi > xf){
      //swap start and end points
      int xTemp = xf; int yTemp = yf;
      xf = xi; xi = xTemp; yf = yi; yi = yTemp;
    }

    b = yi - m*xi;

    for(uint16_t nowX = xi; nowX < xf; nowX++){
      const float nowY = m*(float)nowX+b;
      if(drawCircle(nowX, (uint16_t)nowY, drawingSize/2, color)) return 1;
    }
  }
  else{
    if(yi > yf){
      //swap start and end points
      int xTemp = xf; int yTemp = yf;
      xf = xi; xi = xTemp; yf = yi; yi = yTemp;
    }

    m = (xf-xi)/(yf-yi);
    b = xi - m*yi;

    for(uint16_t nowY = yi; nowY < yf; nowY++){
      const float nowX = m*(float)nowY+b;
      if(drawCircle((uint16_t)nowX, nowY, drawingSize/2, color)) return 1;
    }
  }

  return 0;
}

int drawCircle(uint16_t x, uint16_t y, uint16_t radius, uint32_t color){
  for(uint16_t nowX = x - radius; nowX < x + radius; nowX++)
    for(uint16_t nowY = y - radius; nowY < y + radius; nowY++){
      if((x-nowX)*(x-nowX)+(y-nowY)*(y-nowY) <= radius*radius)
        drawPixelCanvas(nowX,nowY,color);
    }
  return 0;
}

int drawPixelCanvas(uint16_t x, uint16_t y, uint32_t color) {

  if(color == 0xfffffe) return 0;
  if(x >= xRes || y >= yRes) return 0;
  if(y < 100 || y > 500) return 0;

  memcpy(&drawBuffer[(xRes*y + x) * bytesPerPixel], &color, bytesPerPixel);

  return 0;
}

//drawing buffer

void allocateDrawBuffer(){
    drawBuffer = (uint8_t *)malloc(xRes*yRes*bytesPerPixel);
    memset(drawBuffer,0xFFFFFF,xRes*yRes*bytesPerPixel);
}

void copyDrawingToBuffer(){
  memcpy(imgBuffer, drawBuffer, xRes*yRes*bytesPerPixel); 
}

void clearDrawBuffer(){
  memset(drawBuffer,0xFFFFFF,xRes*yRes*bytesPerPixel);
}

void freeDrawBuffer(){
    free(drawBuffer); 
}

void headerCompose(){
  uint32_t colors[10] = {COLOR_BLACK,COLOR_GRAY,COLOR_RED,COLOR_YELLOW,
    COLOR_GREEN,COLOR_AQUA,COLOR_DARKBLUE,COLOR_PURPLE, COLOR_WHITE, COLOR_WHITE};
  memset(drawBuffer,0xDDDDDD,xRes*100*bytesPerPixel);
  int counter = 0, x =15;
  for( int i = 0; i<10; i++){
    if (i < 8) drawHeaderFooterColor(x, 10, 50, 50, colors[counter++]);
    else if(i == 8) drawXpmDrawBuffer( (xpm_map_t) brushMinus_xpm, x, 10);
    else if(i == 9) drawXpmDrawBuffer( (xpm_map_t) brushPlus_xpm, x, 10);
    x+=79;
  }
}

int footerCompose(){
  memset(drawBuffer+(xRes*yRes*bytesPerPixel-(xRes*100*bytesPerPixel)),0xDDDDDD,xRes*100*bytesPerPixel);
  drawHeaderFooterColor(15, 520, 500, 65, COLOR_WHITE);
  return 0;
}

void drawHeaderFooterColor(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for (int h = 0 ; h < height; h++) {
    for (int w = 0 ; w < width; w++) {
      if((x+w) >= xRes || (y+h) >= yRes) return;
      if(color == 0xfffffe) return;
      memcpy(&drawBuffer[(xRes*(y+h) + (x+w)) * bytesPerPixel], &color, bytesPerPixel);
    }
  }
}

void drawXpmDrawBuffer(xpm_map_t xpm, uint16_t x, uint16_t y) {

  xpm_image_t img;
  uint32_t *colors = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &img);
  for (int h = 0 ; h < img.height ; h++) {
    for (int w = 0 ; w < img.width ; w++) {
      if( (uint32_t) &colors[w + h*img.width] == 0xfffffe) return;
      if(x >= xRes || y >= yRes) return;
      memcpy(&drawBuffer[(xRes*(y+h) + (x+w) ) * bytesPerPixel], &colors[w + h*img.width], bytesPerPixel);
    }
  }
  return;
}
