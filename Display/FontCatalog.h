/**
 * @file FontCatalog.h
 * @author David del Val
 * @brief Catalog with the different fonts
 * @date 2019-12-11
 * 
 * This file includes the functionality required
 * to fetch a font.
 * The fonts will be automatically loaded when a font
 * is requested and the catalog has not been created yet.
 * However, if a font has been fetched using the function
 * 'fcat_lookup', the function fcat_free has to be called
 * before the program exits.
 * 
 */
#ifndef FONT_CATALOG_H
#define FONT_CATALOG_H
#include "Font.h"
#define FSIZE 3
typedef enum {M4,M6,M8} fname;



/**
 * @brief Frees all the memory allocated for the catalog
 * 
 */
void fcat_free();

/**
 * @brief Returns a constant reference to a font
 * 
 * @param f name of the font
 * @return  Refence to the font in the catalog
 */
Font* fcat_lookup(fname f);

#endif