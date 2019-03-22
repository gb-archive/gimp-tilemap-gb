//
// lib_gbr_import.h
//

#include "lib_gbr.h"

int32_t gbr_object_producer_decode(gbr_record * p_gbr, pascal_file_object * p_obj);
int32_t gbr_object_tile_data_decode(gbr_record * p_gbr, pascal_file_object * p_obj);
int32_t gbr_object_tile_settings_decode(gbr_record * p_gbr, pascal_file_object * p_obj);
int32_t gbr_object_tile_export_decode(gbr_record * p_gbr, pascal_file_object * p_obj);
int32_t gbr_object_tile_import_decode(gbr_record * p_gbr, pascal_file_object * p_obj);
int32_t gbr_object_palettes_decode(gbr_record * p_gbr, pascal_file_object * p_obj);
int32_t gbr_object_tile_pal_decode(gbr_record * p_gbr, pascal_file_object * p_obj);
