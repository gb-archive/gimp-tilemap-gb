//
// lib_gbr_export.c
//

#include "lib_gbr_export.h"
#include "lib_gbr_file_utils.h"
#include "lib_gbr_ops.h"



static uint8_t CGB_DEFAULT_PAL_SETS[0x80] = {
  0xE0, 0xEF, 0x29, 0x00, 0x39, 0xB9, 0x42, 0x00, 0x20, 0x75, 0x31, 0x00, 0x07, 0x39, 0x2E, 0x00, 0xE0, 0xEF, 0x29, 0x00, 0x39, 0xB9, 0x42, 0x00, 0x20, 0x75,
  0x31, 0x00, 0x07, 0x39, 0x2E, 0x00, 0xE0, 0xEF, 0x29, 0x00, 0x39, 0xB9, 0x42, 0x00, 0x20, 0x75, 0x31, 0x00, 0x07, 0x39, 0x2E, 0x00, 0xE0, 0xEF, 0x29, 0x00,
  0x39, 0xB9, 0x42, 0x00, 0x20, 0x75, 0x31, 0x00, 0x07, 0x39, 0x2E, 0x00, 0xE0, 0xEF, 0x29, 0x00, 0x39, 0xB9, 0x42, 0x00, 0x20, 0x75, 0x31, 0x00, 0x07, 0x39,
  0x2E, 0x00, 0xE0, 0xEF, 0x29, 0x00, 0x39, 0xB9, 0x42, 0x00, 0x20, 0x75, 0x31, 0x00, 0x07, 0x39, 0x2E, 0x00, 0xE0, 0xEF, 0x29, 0x00, 0x39, 0xB9, 0x42, 0x00,
  0x20, 0x75, 0x31, 0x00, 0x07, 0x39, 0x2E, 0x00, 0xE0, 0xEF, 0x29, 0x00, 0x39, 0xB9, 0x42, 0x00, 0x20, 0x75, 0x31, 0x00, 0x07, 0x39, 0x2E, 0x00 };

static uint8_t SGB_DEFAULT_PAL_SETS[0x80] = {
  0xE0, 0xEF, 0x29, 0x00, 0x39, 0xB9, 0x42, 0x00, 0x20, 0x75, 0x31, 0x00, 0x07, 0x39, 0x2E, 0x00,
  0xE0, 0xEF, 0x29, 0x00, 0x39, 0xB9, 0x42, 0x00, 0x20, 0x75, 0x31, 0x00, 0x07, 0x39, 0x2E, 0x00,
  0xE0, 0xEF, 0x29, 0x00, 0x39, 0xB9, 0x42, 0x00, 0x20, 0x75, 0x31, 0x00, 0x07, 0x39, 0x2E, 0x00,
  0xE0, 0xEF, 0x29, 0x00, 0x39, 0xB9, 0x42, 0x00, 0x20, 0x75, 0x31, 0x00, 0x07, 0x39, 0x2E, 0x00,
  0xE0, 0xEF, 0x29, 0x00, 0x39, 0xB9, 0x42, 0x00, 0x20, 0x75, 0x31, 0x00, 0x07, 0x39, 0x2E, 0x00,
  0xE0, 0xEF, 0x29, 0x00, 0x39, 0xB9, 0x42, 0x00, 0x20, 0x75, 0x31, 0x00, 0x07, 0x39, 0x2E, 0x00,
  0xE0, 0xEF, 0x29, 0x00, 0x39, 0xB9, 0x42, 0x00, 0x20, 0x75, 0x31, 0x00, 0x07, 0x39, 0x2E, 0x00,
  0xE0, 0xEF, 0x29, 0x00, 0x39, 0xB9, 0x42, 0x00, 0x20, 0x75, 0x31, 0x00, 0x07, 0x39, 0x2E, 0x00 };


static uint8_t GBR_PRODUCER_VERSION_STR[0x7] = {0x32, 0x2E, 0x32, 0x00, 0x49, 0x00, 0x01 };



// Clamp a value to within a max range
uint8_t clamp(uint8_t in_val, uint8_t max_val) {
    if (in_val < max_val)
        return in_val;
    else
        return max_val;
}



// TODO: shim values for all of THE GBR EXPORT SETTINGS???

int32_t gbr_object_producer_encode(gbr_record * p_gbr, gbr_file_object * p_obj) {

//    if (p_obj->length_bytes > GBR_PRODUCER_SIZE)
//        return false;

    p_obj->offset = 0;
    p_obj->type   = gbr_obj_producer;
    p_obj->id     = 0; // TODO: this should probably increment on write instead of being hardwired

    gbr_write_str(p_gbr->producer.name,    p_obj, GBR_PRODUCER_NAME_SIZE);
    gbr_write_str(p_gbr->producer.version, p_obj, GBR_PRODUCER_VERSION_SIZE);
    gbr_write_str(p_gbr->producer.info,    p_obj, GBR_PRODUCER_INFO_SIZE);

    printf("PRODUCER:\n%s\n%s\n%s\n", p_gbr->producer.name,
                                      p_gbr->producer.version,
                                      p_gbr->producer.info);

    return true;
}



// Tile: x,y,id = .tile_list[ .height
//                            + (y * .width)
//                            + (id * width * height) ]
// TODO: change to int16_t? read about what is most efficient on 64 bit architectures
int32_t gbr_object_tile_data_encode(gbr_record * p_gbr, gbr_file_object * p_obj) {

//    if (p_obj->length_bytes < GBR_TILE_DATA_SIZE_MIN)
//        return false;

    p_obj->offset = 0;
    p_obj->type   = gbr_obj_tile_data;
    p_obj->id     = 1;

    gbr_write_str   ( p_gbr->tile_data.name,      p_obj, GBR_TILE_DATA_NAME_SIZE);
    gbr_write_uint16(&p_gbr->tile_data.width,     p_obj);
    gbr_write_uint16(&p_gbr->tile_data.height,    p_obj);
    gbr_write_uint16(&p_gbr->tile_data.count,     p_obj);

//TODO: fix buffer write size here
    gbr_write_buf   (p_gbr->tile_data.color_set, p_obj, p_gbr->tile_data.pal_data_size);

    // Write N Tiles of x Width x Height array of bytes, one byte per tile pixel
    gbr_write_buf   (p_gbr->tile_data.tile_list, p_obj, p_gbr->tile_data.tile_data_size);

    // If tile count/data size was less than 8192, pad it out
    // Since GBTD seems to write a record of that size
    // regardless of how many actual tiles are stored
    #define GBR_TILE_DATA_RECORD_MAX 8192
    /*
    gbr_write_buf   (p_gbr->tile_data.tile_list[p_gbr->tile_data.tile_data_size],
                     p_obj,
                     (GBR_TILE_DATA_RECORD_MAX - p_gbr->tile_data.tile_data_size));
    */
    gbr_write_padding(p_obj, (GBR_TILE_DATA_RECORD_MAX - p_gbr->tile_data.tile_data_size));



printf("TILE_DATA:\n%s\n %d\n %d\n %d\n %x\n %x\n %x\n %x\n%d\n%d\n",
                                 p_gbr->tile_data.name,
                                 p_gbr->tile_data.width,
                                 p_gbr->tile_data.height,
                                 p_gbr->tile_data.count,
                                 p_gbr->tile_data.color_set[0],
                                 p_gbr->tile_data.color_set[1],
                                 p_gbr->tile_data.color_set[2],
                                 p_gbr->tile_data.color_set[3],
                                 p_gbr->tile_data.pal_data_size,
                                 p_gbr->tile_data.tile_data_size);

  return true;
}



int32_t gbr_object_tile_settings_encode(gbr_record * p_gbr, gbr_file_object * p_obj) {

//    if (p_obj->length_bytes != GBR_TILE_SETTINGS_SIZE)
//        return false;

    p_obj->offset = 0;
    p_obj->type   = gbr_obj_tile_settings;
    p_obj->id     = 2;

    gbr_write_uint16(&p_gbr->tile_settings.tile_id,      p_obj);
    gbr_write_bool  (&p_gbr->tile_settings.simple,       p_obj);
    gbr_write_uint8 (&p_gbr->tile_settings.flags,        p_obj);
    gbr_write_uint8 (&p_gbr->tile_settings.left_color,   p_obj);
    gbr_write_uint8 (&p_gbr->tile_settings.right_color,  p_obj);
    gbr_write_uint16(&p_gbr->tile_settings.split_width,  p_obj);
    gbr_write_uint16(&p_gbr->tile_settings.split_height, p_obj);
    gbr_write_uint8 (&p_gbr->tile_settings.split_order,  p_obj);  // spec/source shows int32, but file parsing indicates int8
    gbr_write_uint8 (&p_gbr->tile_settings.color_set,    p_obj);
    gbr_write_buf   ( (uint8_t *)p_gbr->tile_settings.bookmarks,
                      p_obj,
                      sizeof(uint16_t) * GBR_TILE_SETTINGS_BOOKMARK_COUNT);
    gbr_write_uint8 (&p_gbr->tile_settings.auto_update,  p_obj);


printf("TILE_SETTINGS:\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
                                 p_gbr->tile_settings.tile_id,
                                 p_gbr->tile_settings.simple,
                                 p_gbr->tile_settings.flags,
                                 p_gbr->tile_settings.left_color,
                                 p_gbr->tile_settings.right_color,
                                 p_gbr->tile_settings.split_width,
                                 p_gbr->tile_settings.split_height,
                                 p_gbr->tile_settings.split_order,
                                 p_gbr->tile_settings.color_set);

  return true;
}


int32_t gbr_object_tile_export_encode(gbr_record * p_gbr, gbr_file_object * p_obj) {

//    if (p_obj->length_bytes != GBR_TILE_EXPORT_SIZE)
//        return false;

    p_obj->offset = 0;
    p_obj->type   = gbr_obj_tile_export;
    p_obj->id     = 3;

    gbr_write_uint16(&p_gbr->tile_export.tile_id,        p_obj);
    gbr_write_str    (p_gbr->tile_export.file_name,      p_obj, GBR_TILE_EXPORT_FILE_NAME_SIZE);
    gbr_write_uint8 (&p_gbr->tile_export.file_type,      p_obj);
    gbr_write_str    (p_gbr->tile_export.section_name,   p_obj, GBR_TILE_EXPORT_SECTION_NAME_SIZE);
    gbr_write_str    (p_gbr->tile_export.label_name,     p_obj, GBR_TILE_EXPORT_LABEL_NAME_SIZE);
    gbr_write_uint8 (&p_gbr->tile_export.bank,           p_obj);
    gbr_write_uint8 (&p_gbr->tile_export.tile_array,     p_obj);
    gbr_write_uint8 (&p_gbr->tile_export.format,         p_obj);
    gbr_write_uint8 (&p_gbr->tile_export.counter,        p_obj);
    gbr_write_uint16(&p_gbr->tile_export.from,           p_obj);
    gbr_write_uint16(&p_gbr->tile_export.upto,           p_obj);
    gbr_write_uint8 (&p_gbr->tile_export.compression,    p_obj);
    gbr_write_uint8 (&p_gbr->tile_export.include_colors, p_obj);
    gbr_write_uint8 (&p_gbr->tile_export.sgb_palettes,   p_obj);
    gbr_write_uint8 (&p_gbr->tile_export.gbc_palettes,   p_obj);
    gbr_write_uint8 (&p_gbr->tile_export.make_meta_tiles,p_obj);
    gbr_write_uint32(&p_gbr->tile_export.meta_offset,    p_obj);
    gbr_write_uint8 (&p_gbr->tile_export.meta_counter,   p_obj);
    gbr_write_uint8 (&p_gbr->tile_export.split,          p_obj);
    gbr_write_uint32(&p_gbr->tile_export.block_size,     p_obj);
    gbr_write_uint8 (&p_gbr->tile_export.sel_tab,        p_obj);


printf("TILE_EXPORT:\n%d\n%s\n%d\n%s\n%s\n",
                                 p_gbr->tile_export.tile_id,
                                 p_gbr->tile_export.file_name,
                                 p_gbr->tile_export.file_type,
                                 p_gbr->tile_export.section_name,
                                 p_gbr->tile_export.section_name);

  return true;
}



int32_t gbr_object_tile_import_encode(gbr_record * p_gbr, gbr_file_object * p_obj) {

//    if (p_obj->length_bytes != GBR_TILE_IMPORT_SIZE)
//        return false;

    p_obj->offset = 0;
    p_obj->type   = gbr_obj_tile_import;
    p_obj->id     = 4;

    gbr_write_uint16(&p_gbr->tile_import.tile_id,          p_obj);
    gbr_write_str    (p_gbr->tile_import.file_name,        p_obj, GBR_TILE_IMPORT_FILE_NAME_SIZE);
    gbr_write_uint8 (&p_gbr->tile_import.file_type,        p_obj);
    gbr_write_uint16(&p_gbr->tile_import.from_tile,        p_obj);
    gbr_write_uint16(&p_gbr->tile_import.to_tile,          p_obj);
    gbr_write_uint16(&p_gbr->tile_import.tile_count,       p_obj);
    gbr_write_uint8 (&p_gbr->tile_import.color_conversion, p_obj);
    gbr_write_uint32(&p_gbr->tile_import.first_byte,       p_obj);
    gbr_write_uint8 (&p_gbr->tile_import.binary_file_type, p_obj);


printf("TILE_import:\n%d\n%s\n%d\n%d\n%d\n",
                                 p_gbr->tile_import.tile_id,
                                 p_gbr->tile_import.file_name,
                                 p_gbr->tile_import.file_type,
                                 p_gbr->tile_import.from_tile,
                                 p_gbr->tile_import.to_tile);

  return true;
}





int32_t gbr_object_palettes_encode(gbr_record * p_gbr, gbr_file_object * p_obj) {

//    if (p_obj->length_bytes < GBR_PALETTES_SIZE_MIN)
//        return false;

    p_obj->offset = 0;
    p_obj->type   = gbr_obj_palettes;
    p_obj->id     = 5;

    gbr_write_uint16(&p_gbr->palettes.id,         p_obj);
    gbr_write_uint16(&p_gbr->palettes.count,      p_obj);
    gbr_write_buf   ( p_gbr->palettes.colors,     p_obj, GBR_PALETTE_CGB_SETS_SIZE);
    gbr_write_uint16(&p_gbr->palettes.sgb_count,  p_obj);
    gbr_write_buf   ( p_gbr->palettes.sgb_colors, p_obj, GBR_PALETTE_SGB_SETS_ACTUAL_SIZE);


printf("PALEttes:\n%d\n%d\n%d\n",
                                 p_gbr->palettes.id,
                                 p_gbr->palettes.count,
                                 p_gbr->palettes.sgb_count);

  return true;
}


int32_t gbr_object_tile_pal_encode(gbr_record * p_gbr, gbr_file_object * p_obj) {

//    if (p_obj->length_bytes < GBR_TILE_PAL_SIZE_MIN)
//        return false;

    p_obj->offset = 0;
    p_obj->type   = gbr_obj_tile_pal;
    p_obj->id     = 6;

    gbr_write_uint16(&p_gbr->tile_pal.id,            p_obj);
    // It's supposed to be tile_pal_count * array of 16 bit ints
    // but instead it seems to be * array of 32 bit ints x count
    gbr_write_uint16(&p_gbr->tile_pal.count,         p_obj);
    // TODO: Is this padding actually necessary? It only shows up in some versions
    gbr_write_buf   ( p_gbr->tile_pal.color_set,     p_obj, p_gbr->tile_pal.count * (sizeof(uint16_t) * 2));
    gbr_write_padding(p_obj, GBR_TILE_PAL_ACTUAL_SIZE
                             - (p_gbr->tile_pal.count * (sizeof(uint16_t) * 2)) );

    gbr_write_uint16(&p_gbr->tile_pal.sgb_count,     p_obj);
    gbr_write_buf   ( p_gbr->tile_pal.sgb_color_set, p_obj, p_gbr->tile_pal.sgb_count * (sizeof(uint16_t) * 2));
    gbr_write_padding(p_obj, GBR_TILE_PAL_ACTUAL_SIZE
                             - (p_gbr->tile_pal.sgb_count * (sizeof(uint16_t) * 2)) );

    // Add the padding amount to fill out the size
    // Not sure where this comes from
    // gbr_write_padding(p_obj, GBR_TILE_PAL_UNKNOWN_PADDING);

printf("tile pal:\n%d\n%d\n%d\n",
                                 p_gbr->tile_pal.id,
                                 p_gbr->tile_pal.count,
                                 p_gbr->tile_pal.sgb_count);

  return true;
}


// Expects tile_size to be an even multiple of 1024 (which it should be)
int32_t gbr_export_tileset_calc_tile_count_padding(gbr_record * p_gbr) {

    uint16_t tile_size;
    uint16_t tile_total_size;
    uint16_t padding_bytes;

    tile_size       = p_gbr->tile_data.width * p_gbr->tile_data.width;
    tile_total_size = tile_size * p_gbr->tile_data.count;

    if (GBR_TILE_DATA_MIN_TILE_BYTES > tile_total_size) {

        padding_bytes = (GBR_TILE_DATA_MIN_TILE_BYTES - tile_total_size);
        p_gbr->tile_data.padding_tile_count = (padding_bytes / tile_size);
    }
    else
        p_gbr->tile_data.padding_tile_count = 0; // No padding required

      return true;
}



int32_t gbr_export_tileset_calc_dimensions(gbr_record * p_gbr, image_data * p_image) {

    // TODO: make tile size an export setting : 8x8, 8,x16, 16x16, 32x32 -- AND/OR SENSE FROM LOADED GBR INFO
    if (p_image->width == 32) {
            // 32x32 tiles
            p_gbr->tile_data.width  = 32;
            p_gbr->tile_data.height = 32;
            p_gbr->tile_data.count  = p_image->height / 32;

            if ((p_image->height % 32) != 0)
                return false; // FAILED: export image must be even multiple of tile size
    }
    else if (p_image->width == 16) {
            // 16x16 tiles
            p_gbr->tile_data.width  = 16;
            p_gbr->tile_data.height = 16;
            p_gbr->tile_data.count  = p_image->height / 16;

            if ((p_image->height % 16) != 0)
                return false; // FAILED: export image must be even multiple of tile size
    }
    else if (p_image->width == 8) {

            p_gbr->tile_data.width  = 8;

            // TODO: re-enable 8x16 tiles
            /*
            if ((p_image->height % 16) == 0) {
                // 8x16 tiles
                p_gbr->tile_data.height = 16;
                p_gbr->tile_data.count  = p_image->height / 16;
            }
            else if ((p_image->height % 8) == 0) {
                // 8x8 tiles
                p_gbr->tile_data.height = 8;
                p_gbr->tile_data.count  = p_image->height / 8;
            }
            */
            if ((p_image->height % 8) == 0) {
                // 8x8 tiles
                p_gbr->tile_data.height = 8;
                p_gbr->tile_data.count  = p_image->height / 8;
            }
            else
                return false; // FAILED: export image must be even multiple of tile size
    }

    return true;
}



// Convert loaded .GBR data to an image
int32_t gbr_convert_image_to_tileset(gbr_record * p_gbr, image_data * p_image, color_data * p_colors) {

    int16_t tile_id;
    int32_t offset;

    // p_image->bytes_per_pixel = 1; // TODO: IS THIS T

    if (!gbr_export_tileset_calc_dimensions(p_gbr, p_image))
        return false;

    gbr_export_tileset_calc_tile_count_padding(p_gbr);

    if (p_image->p_img_data) {

        offset = 0;

        // SAVE IMAGE
        // Extract tiles from buffer
        // TODO: FIXME this is linear for now and assumes an 8 x N dest image

        for (tile_id=0; tile_id < p_gbr->tile_data.count; tile_id++) {
            printf("EXPORT Tile:%d, offset=%d\n", tile_id, offset);
            if (!gbr_tile_set_buf(&p_image->p_img_data[offset],
                                  p_gbr,
                                  tile_id))
                return false;

            offset += p_gbr->tile_data.width * p_gbr->tile_data.height * p_image->bytes_per_pixel;
        }


        // Now add padding tiles to achieve the minimum required size
        for (tile_id=p_gbr->tile_data.count; tile_id < (p_gbr->tile_data.count + p_gbr->tile_data.padding_tile_count); tile_id++) {
            printf("PADDING EXPORT Tile:%d\n", tile_id);
            if (!gbr_tile_set_buf_padding(p_gbr,
                                          tile_id))
                return false;
        }

        p_gbr->tile_data.count += p_gbr->tile_data.padding_tile_count;

        // TODO: should match offset?
        // Calculate final size..
        p_gbr->tile_data.tile_data_size = p_gbr->tile_data.width * p_gbr->tile_data.height
                                                                 * p_gbr->tile_data.count;

        printf("image:%d x %d (%d x %d # %d) \n", p_image->width, p_image->height,
                                                  p_gbr->tile_data.width,
                                                  p_gbr->tile_data.height,
                                                  p_gbr->tile_data.count);

        // SAVE COLOR MAP
        if (!gbr_export_tileset_palette(p_colors, p_gbr))
            return false;

        return true;
    }
    else
        return false;

}


int32_t gbr_export_tileset_palette(color_data * p_colors, gbr_record * p_gbr) {

    uint16_t pal_index;
    uint32_t pal_offset;
    uint32_t buf_offset;

    if (p_colors->color_count > 4) {
        printf("Too many colors %d\n", p_colors->color_count);
        return false; // FAILED, too many colors
    }

// int32_t gbr_pal_set_buf(uint8_t * src_buf, gbr_record * p_gbr, uint16_t num_colors) {

    // Make sure there is enough space in destination palette buffer
    if ((p_gbr->tile_data.pal_data_size * COLOR_DATA_BYTES_PER_COLOR) > (COLOR_DATA_PAL_SIZE)) {
        printf("Not enough space in palette %d x 3 vs %d\n", p_gbr->tile_data.pal_data_size, COLOR_DATA_PAL_SIZE);
        return false;
    }

    // Set the destination color palette size based on tile list color set size
    p_gbr->tile_data.pal_data_size = p_colors->color_count;

    // p_colors->size        = p_colors->color_count * COLOR_DATA_BYTES_PER_COLOR;


    printf("COLOR: size=%d\n", p_colors->color_count);

    buf_offset = 0;
    pal_offset = 0;


    // Load the palette colors for each entry in the tile list color set
    for(pal_index = 0; pal_index < p_gbr->tile_data.pal_data_size; pal_index++) {

        // Set tile list color set entry to point to same numbered entry in color palette
        p_gbr->tile_data.color_set[pal_index] = pal_index;

        // Now select matching tile palette entry for the tile list color
        pal_offset = p_gbr->tile_data.color_set[pal_index] * GBR_PALETTE_TCOLOR_SIZE;

        // Load the color data into the destination palette
        // Limit it's range to 248, a multiple of 8
        // (GBTD may handle colors internally as 0..31, then multiply by 8 for export)
        // TODO: figure this out better
        p_gbr->palettes.colors[pal_offset + 0] = clamp(p_colors->pal[buf_offset++], GBR_COL_MAX); // R.0
        p_gbr->palettes.colors[pal_offset + 1] = clamp(p_colors->pal[buf_offset++], GBR_COL_MAX); // G.1
        p_gbr->palettes.colors[pal_offset + 2] = clamp(p_colors->pal[buf_offset++], GBR_COL_MAX); // B.2


        printf("EXPORT --COLORS %d: %d) %2x, %2x, %2x, <- %2x, %2x, %2x, \n", pal_index, pal_offset,
                                                p_gbr->palettes.colors[pal_offset + 0],
                                                p_gbr->palettes.colors[pal_offset + 1],
                                                p_gbr->palettes.colors[pal_offset + 2],
                                                p_colors->pal[buf_offset-3],
                                                p_colors->pal[buf_offset-2],
                                                p_colors->pal[buf_offset-1]);
    }

    return true;
}



int32_t gbr_export_set_defaults(gbr_record * p_gbr) {



    // PRODUCER
    // default string values is filled with trailing zeros
    memset(p_gbr->producer.name,    0x00, GBR_PRODUCER_NAME_SIZE);
    memset(p_gbr->producer.version, 0x00, GBR_PRODUCER_VERSION_SIZE);
    memset(p_gbr->producer.info,    0x00, GBR_PRODUCER_INFO_SIZE);

    snprintf(p_gbr->producer.name,     GBR_PRODUCER_NAME_SIZE_STR,    "Gameboy Tile Designer");
    // snprintf(p_gbr->producer.version,  GBR_PRODUCER_VERSION_SIZE_STR, "2.2");
    memcpy(p_gbr->producer.version,   &GBR_PRODUCER_VERSION_STR[0], sizeof(GBR_PRODUCER_VERSION_STR));
    snprintf(p_gbr->producer.info,     GBR_PRODUCER_INFO_SIZE_STR,    "Home: www.casema.net/~hpmulder");


    // TILE_DATA
    memset(p_gbr->tile_data.name, 0x00, GBR_TILE_DATA_NAME_SIZE);

    // The values below get updated during export
p_gbr->tile_data.width  = 0;
p_gbr->tile_data.height = 0;
p_gbr->tile_data.count  = 0;
p_gbr->tile_data.pal_data_size = GBR_TILE_DATA_COLOR_SET_SIZE;
p_gbr->tile_data.tile_data_size = 0;

memset(p_gbr->tile_data.color_set,  0x00, GBR_TILE_DATA_COLOR_SET_SIZE);
memset(p_gbr->tile_data.color_set,  0x00, PASCAL_OBJECT_MAX_SIZE);


    // TILE_SETTINGS
    p_gbr->tile_settings.tile_id      = 1;
    p_gbr->tile_settings.simple       = 0;
    p_gbr->tile_settings.flags        = 1;
    p_gbr->tile_settings.left_color   = 1;
    p_gbr->tile_settings.right_color  = 3;
    p_gbr->tile_settings.split_width  = 1;
    p_gbr->tile_settings.split_height = 1;
    p_gbr->tile_settings.split_order  = 0;
    p_gbr->tile_settings.color_set    = 1;

    memset(p_gbr->tile_settings.bookmarks, 0xFF, sizeof(uint16_t) * GBR_TILE_SETTINGS_BOOKMARK_COUNT);
    p_gbr->tile_settings.auto_update  = 0;


    // TILE_EXPORT
    // Just leave these strings as NULL by default
    memset(p_gbr->tile_export.file_name,    0x00, GBR_TILE_EXPORT_FILE_NAME_SIZE);
    memset(p_gbr->tile_export.section_name, 0x00, GBR_TILE_EXPORT_SECTION_NAME_SIZE);
    memset(p_gbr->tile_export.label_name,   0x00, GBR_TILE_EXPORT_LABEL_NAME_SIZE);

    p_gbr->tile_export.tile_id        = 1;
    //snprintf(p_gbr->tile_export.file_name,     GBR_TILE_EXPORT_FILE_NAME_SIZE_STR,    "");
    p_gbr->tile_export.file_type       = 0;
    //snprintf(p_gbr->tile_export.section_name,  GBR_TILE_EXPORT_SECTION_NAME_SIZE_STR, "");
    //snprintf(p_gbr->tile_export.label_name,    GBR_TILE_EXPORT_LABEL_NAME_SIZE_STR,   "");
    p_gbr->tile_export.bank            = 0;
    p_gbr->tile_export.tile_array      = 1;
    p_gbr->tile_export.format          = 0;
    p_gbr->tile_export.counter         = 0;
    p_gbr->tile_export.from            = 0;
    p_gbr->tile_export.upto            = 0;
    p_gbr->tile_export.compression     = 0;
    p_gbr->tile_export.include_colors  = 0;
    p_gbr->tile_export.sgb_palettes    = 0;
    p_gbr->tile_export.gbc_palettes    = 0;
    p_gbr->tile_export.make_meta_tiles = 0;
    p_gbr->tile_export.meta_offset     = 0;
    p_gbr->tile_export.meta_counter    = 0;
    p_gbr->tile_export.split           = 0;
    p_gbr->tile_export.block_size      = 0;
    p_gbr->tile_export.sel_tab         = 0;


    // TILE_IMPORT
    // Leave this string as NULL by default
    memset(p_gbr->tile_export.file_name, 0x00, GBR_TILE_IMPORT_FILE_NAME_SIZE);

    p_gbr->tile_import.tile_id          = 01;
    // snprintf(p_gbr->tile_import.file_name,  GBR_TILE_IMPORT_FILE_NAME_SIZE_STR, "");
    p_gbr->tile_import.file_type        = 0;
    p_gbr->tile_import.from_tile        = 0;
    p_gbr->tile_import.to_tile          = 0;
    p_gbr->tile_import.tile_count       = 0x7F; // TODO: why 127? Doesn't match other tile count
    p_gbr->tile_import.color_conversion = 0;
    p_gbr->tile_import.first_byte       = 0;
    p_gbr->tile_import.binary_file_type = 0;


    // PALETTES
    p_gbr->palettes.id    = 1;

    p_gbr->palettes.count  = 8;
    // Size here should be equiv to: GBR_PALETTE_CGB_SETS_SIZE (and smaller than PASCAL_OBJECT_MAX_SIZE)
    memcpy(&(p_gbr->palettes.colors[0]), &CGB_DEFAULT_PAL_SETS[0], sizeof(CGB_DEFAULT_PAL_SETS));

    p_gbr->palettes.sgb_count = 4;
    // Size here should be equiv to: GBR_PALETTE_SGB_SETS_ACTUAL_SIZE
    // vs GBR_PALETTE_SGB_SETS_SIZE
    // Size here should be equiv to: GBR_PALETTE_CGB_SETS_SIZE (and smaller than PASCAL_OBJECT_MAX_SIZE)
    memcpy(&(p_gbr->palettes.sgb_colors[0]), &SGB_DEFAULT_PAL_SETS[0], sizeof(SGB_DEFAULT_PAL_SETS));


    // TILE_PAL
    p_gbr->tile_pal.id = 1;
    p_gbr->tile_pal.count = 0x10;
    // It's supposed to be tile_pal_count * array of 16 bit ints
    // but instead it seems to be an (empty) array of 32 bit ints x count
    memset(p_gbr->tile_pal.color_set,     0x00, GBR_TILE_PAL_ACTUAL_SIZE);

    p_gbr->tile_pal.sgb_count = 0x10;
    memset(p_gbr->tile_pal.sgb_color_set, 0x00, GBR_TILE_PAL_ACTUAL_SIZE);

    return true;
}
