#ifndef GET_CAP_H
#define GET_CAP_H

#include <infiniband/mlx5dv.h>
#include <inttypes.h>
#include <stdio.h>
#include "mlx5_ifc_extended.h"

int get_general_device_cap( struct ibv_context *ctx, 
                            int *is_wod_supported, 
                            int *is_dm_supported, 
                            int *port_type, 
                            const uint16_t maximum_current);
int get_device_memory_cap(  struct ibv_context *ctx, 
                            int *is_memic_supported, 
                            uint64_t *memic_bar_start_addr,
                            uint32_t *memic_bar_size, 
                            uint8_t *memic_atomic_incr, 
                            const uint16_t maximum_current);
void print_memic_and_wod_capabilities(struct ibv_context *ctx, const uint16_t maximum_current);

#endif