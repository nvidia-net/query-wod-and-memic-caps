#include "../include/query_wod_memic_cap.h"

int get_general_device_cap( struct ibv_context *ctx, 
                            int *is_wod_supported,
                            int *is_device_memory_supported, 
                            int *port_type, 
                            const uint16_t maximum_current)
{
    uint32_t in[DEVX_ST_SZ_DW(query_hca_cap_in)] = {0};
    uint32_t out[DEVX_ST_SZ_DW(query_hca_cap_out)] = {0};

    DEVX_SET(query_hca_cap_in, in, opcode, MLX5_CMD_OP_QUERY_HCA_CAP);
    // op_mod - 16 bit
    // op_mod[0] -      0   = Maximum capabilities
    //                  1   = Current capabilities
    // op_mod[1:15] -   0xF = Device Memory Capabilities
    uint16_t op_mod_0 = maximum_current & 0x1;
    uint16_t op_mod_1_15 = MLX5_SET_HCA_CAP_OP_MOD_GENERAL_DEVICE;
    uint16_t op_mod = op_mod_1_15 | op_mod_0;
    DEVX_SET(query_hca_cap_in, in, op_mod, op_mod);
    if (mlx5dv_devx_general_cmd(ctx, in, sizeof(in), out, sizeof(out)))
    {
        perror("mlx5dv_devx_general_cmd  general device function failed");
        return -1;
    }
    *is_wod_supported = DEVX_GET(query_hca_cap_out, out, capability.cmd_hca_cap.wait_on_data);
    *is_device_memory_supported = DEVX_GET(query_hca_cap_out, out, capability.cmd_hca_cap.device_memory);
    //port_type= true:"Ethernet", false:"InfiniBand"
    *port_type = DEVX_GET(query_hca_cap_out, out, capability.cmd_hca_cap.port_type);
    return 0;
}

int get_device_memory_cap(  struct ibv_context *ctx, 
                            int *is_memic_supported, 
                            uint64_t *memic_bar_start_addr,
                            uint32_t *memic_bar_size, 
                            uint8_t *memic_atomic_incr, 
                            const uint16_t maximum_current)
{
    uint32_t in[DEVX_ST_SZ_DW(query_hca_cap_in)] = {0};
    uint32_t out[DEVX_ST_SZ_DW(query_hca_cap_out)] = {0};

    DEVX_SET(query_hca_cap_in, in, opcode, MLX5_CMD_OP_QUERY_HCA_CAP);

    // op_mod - 16 bit
    // op_mod[0] -      0   = Maximum capabilities
    //                  1   = Current capabilities
    // op_mod[1:15] -   0xF = Device Memory Capabilities
    uint16_t op_mod_0 = maximum_current & 0x1;
    uint16_t op_mod_1_15 = 0xF << 1;
    uint16_t op_mod = op_mod_1_15 | op_mod_0;
    DEVX_SET(query_hca_cap_in, in, op_mod, op_mod);
    if (mlx5dv_devx_general_cmd(ctx, in, sizeof(in), out, sizeof(out)))
    {
        perror("mlx5dv_devx_general_cmd device memory function failed");
        return 1;
    }
    *is_memic_supported = DEVX_GET(query_hca_cap_out, out, capability.device_mem_cap.memic);
    *memic_bar_start_addr = DEVX_GET64(query_hca_cap_out, out, capability.device_mem_cap.memic_bar_start_addr);
    *memic_bar_size = DEVX_GET(query_hca_cap_out, out, capability.device_mem_cap.memic_bar_size);
    *memic_atomic_incr = DEVX_GET(query_hca_cap_out, out, capability.device_mem_cap.memic_atomic_increment);
    return 0;
}

void print_memic_and_wod_capabilities(struct ibv_context *ctx, const uint16_t maximum_current)
{
    int is_memic_supported, is_wod_supported, is_device_memory_supported, port_type;
    uint8_t memic_atomic_incr;
    uint64_t memic_bar_start_addr;
    uint32_t memic_bar_size;

    if (get_general_device_cap(ctx, &is_wod_supported, &is_device_memory_supported,&port_type, maximum_current) != 0)
    {
        printf("Error at get_general_device_cap function!\n");
        return;
    }
        
    if (get_device_memory_cap(ctx, &is_memic_supported, &memic_bar_start_addr, &memic_bar_size, &memic_atomic_incr, maximum_current) != 0){
         printf("Error at get_device_memory_cap function!\n");
         return;
    }

    printf("-------  %s Capabilities  -------\n", maximum_current?"Current":"Maximum");
    printf("  > wait-on-data WQE support:           %s\n", is_wod_supported ? "True" : "False");
    printf("  > Device memory support:              %s\n", is_device_memory_supported ? "True" : "False");
    printf("  > MEMIC support:                      %s\n", is_memic_supported ? "True" : "False");
    printf("       > memic_bar_start_addr (64-bit): 0x%" PRIx64 " (%" PRIu64 ")\n", memic_bar_start_addr, memic_bar_start_addr);
    printf("       > memic_bar_size (32-bit):       0x%" PRIx32 " (%" PRIu32 ")\n", memic_bar_size, memic_bar_size);
    printf("  > Atomic Increment support:           %s (%d)\n", memic_atomic_incr ? "True" : "False", (int)memic_atomic_incr);
    printf("       >  1 Byte support:               %s\n", (memic_atomic_incr & 0x1) ? "True" : "False");
    printf("       >  2 Byte support:               %s\n", (memic_atomic_incr & 0x2) ? "True" : "False");
    printf("       >  4 Byte support:               %s\n", (memic_atomic_incr & 0x4) ? "True" : "False");
    printf("       >  8 Byte support:               %s\n", (memic_atomic_incr & 0x8) ? "True" : "False");
    printf("       > 16 Byte support:               %s\n", (memic_atomic_incr & 0x10) ? "True" : "False");
}
