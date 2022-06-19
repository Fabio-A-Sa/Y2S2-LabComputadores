/**
 * @file graphics
 * @brief This file contains the functions that allows for the configuration of the graphics (gui mode), allows to draw pixels, line, rectangles.
 * It also contains functions to load xmps to the display, and functions to control the secondary and tertiary buffers.
 *
 * @author Fábio Araújo de Sá, Inês Sá Pereira Estêvão Gaspar, Lourenço Alexandre Correia Gonçalves, Marcos William Ferreira Pinto
 *
 * @date 31/05/2022
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>
#include "VBE.h"
#include "BIOS_MACROS.h"
#include "../video/COLORS.h"

/**
 * @brief This is the struct that store the information about the video mode
 */
vbe_mode_info_t info;

/**
 * @brief This is the base addres (pointer) to the beggining of th array where display pixels are stored
 */
uint8_t* vAddr_base;

/**
 * @brief This function prints a pixels to the display
 * @param x the x-axis position of the pixel's position
 * @param y the y-axis position of the pixel's position
 * @param color the color the pixel to be displayed will be
 * @return Return 0 in case of success, 1 if otherwise
 */
int drawPixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief This function prints a xmp image to the display
 * @param xpm The xpm image to be loaded to the display
 * @param x the x-axis position of the top left pixel's position of the xpm
 * @param y the y-axis position of the top left pixel's position of the xpm
 * @return Return 0 in case of success, 1 if otherwise
 */
void drawXpm(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief This funtion initiates graphics mode (GUI instead of CLI)
 * @param mode The VBE mode in which the graphics will be configured
 * @return Return 0 in case of success, 1 if otherwise
 */
int (init_graphics_mode)(uint16_t mode);

/**
 * @brief This function allocates memomry necessary for the graphics and maps it in an array
 * @param mode The VBE mode in which the graphics will be configured
 * @return Return 0 in case of success, 1 if otherwise
 */
int (map_vmem)(uint16_t mode);

/**
 * @brief This functions sets the color of the pixel in the position given to the desired color, in which after it will be,
 * changed on the screen
 * @param x x position of the desired pixel
 * @param y y position  if the desired pixel
 * @param color The color the pixel will be changed to
 * @return Return 0 in case of success, 1 if otherwise
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief This function draws a line on the screen
 * @param [in] x x position of the start of the line
 * @param [in] y y position of the start of the line
 * @param [in] len Length of the line
 * @param [in] color Color of the line
 * @return Return 0 in case of success, 1 if otherwise
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief This function draws a rectagle (or square) of the desired size starting x and y as  the top left corner
 * @param [in] x x position of the top left corner of the rectangle
 * @param [in] y y position of the top left corner of the rectangle
 * @param [in] width the width of the rectangle to be drawn
 * @param [in] height the height of the rectangle to be drawn
 * @param [in] color Color of the rectangle
 * @return Return 0 in case of success, 1 if otherwise
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief This function draws a xpm to the display
 * @param [in] xmp The xmp image to be drawn
 * @param [in] x x position of the top left corner of the rectangle
 * @param [in] y y position of the top left corner of the rectangle
 * @return Return 0 in case of success, 1 if otherwise
 */
int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief This function returns the red component of a color in RGB
 * @param color The original color
 * @return The red component of the original color
 */
uint32_t (R)(uint32_t color);
/**
 * @brief This function returns the green component of a color in RGB
 * @param color The original color
 * @return The green component of the original color
 */
uint32_t (G)(uint32_t color);

/**
 * @brief This function returns the blue component of a color in RGB
 * @param color The original color
 * @return The blue component of the original color
 */
uint32_t (B)(uint32_t color);


/**
 * @brief This function draws a line between two points on the drawing canvas
 * @param [in] xi x position of the start point
 * @param [in] yi y position of the start point
 * @param [in] xf x position of the end point
 * @param [in] yf y position of the end point
 * @param [in] color Color of the circle
 * @return Return 0 in case of success, 1 if otherwise
 */
int drawToCanvas(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color);

/**
 * @brief This function draws a circle on the drawing canvas
 * @param [in] x x position of the center
 * @param [in] y y position of the center
 * @param [in] radius Radius of the circle
 * @param [in] color Color of the circle
 * @return Return 0 in case of success, 1 if otherwise
 */
int drawCircle(uint16_t x, uint16_t y, uint16_t radius, uint32_t color);

/**
 * @brief This function draws a pixel on the drawing canvas
 * @param [in] x x position of pixel
 * @param [in] y y position of pixel
 * @param [in] color Color of the pixel
 * @return Return 0 in case of success, 1 if otherwise
 */
int drawPixelCanvas(uint16_t x, uint16_t y, uint32_t color);

//image buffer

/**
 * @brief This function allocates the necessary memory to the image buffer (the secondary buffer)
 */
void allocateImgBuffer();

/**
 * @brief This function copies anything not transparent of the image buffer to the main buffer to be displayed
 */
void copyBufferToVideo();

/**
 * @brief This function fills the buffer with the color white
 */
void clearImgBuffer();

/**
 * @brief This function frees the memory of the image buffer (the secondary buffer)
 */
void freeImgBuffer();

//draw buffer

/**
 * @brief This function allocates the necessary memory to the draw buffer (the tertiary buffer)
 */
void allocateDrawBuffer();

/**
 * @brief This function copies anything not transparent of the draw buffer to the image buffer
 */
void copyDrawingToBuffer();

/**
 * @brief This function fills the buffer with the color white
 */
void clearDrawBuffer();

/**
 * @brief This function frees the memory of the drawing buffer (the tertiary buffer)
 */
void freeDrawBuffer();

/**
* @brief This function draws the top bar of the game in drawing and guessing modes
*/
void headerCompose();
/**
* @brief This function draws the bottom bar of the game in drawing and guessing modes
*/
int footerCompose();

/**
 * @brief This function draws the static components of the header and the footer in guess mode and draw mode
 * 
 * @param x x position of the top left corner of the component
 * @param y y position of the top left corner of the component
 * @param [in] width the width of the component to be drawn
 * @param [in] height the height of the component to be drawn
 * @param color Color to draw
 */
void drawHeaderFooterColor(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief This function loads a xpm image to the drawBuffer
 * @param xmp The xmp image to be loaded
 * @param x x position of the top left corner of the rectangle
 * @param y y position of the top left corner of the rectangle
 * @return Return 0 in case of success, 1 if otherwise
 */
void drawXpmDrawBuffer(xpm_map_t xpm, uint16_t x, uint16_t y); //xpm to draw buffer

#endif /* __GRAPHICS_H */
