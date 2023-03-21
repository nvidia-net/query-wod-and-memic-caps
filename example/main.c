#include "get_cap.h"

int main()
{
    struct ibv_context *ctx = NULL;
    int num, i;
    struct ibv_device **list = ibv_get_device_list(&num);

    for (i = 0; i < num; i++)
    {
        printf("\n\n> %s:\n", ibv_get_device_name(list[i]));
        struct mlx5dv_context_attr attr = {.flags = MLX5DV_CONTEXT_FLAGS_DEVX};
        ctx = mlx5dv_open_device(list[i], &attr);
        if (ctx == NULL)
        {
            printf("Could not open the device!\n");
            continue;
        }
        print_memic_and_wod_capabilities(ctx, MLX5_HCA_CAP_OP_MOD_GET_CUR);
        print_memic_and_wod_capabilities(ctx, MLX5_HCA_CAP_OP_MOD_GET_MAX);
        if (ibv_close_device(ctx) != 0)
        {
            perror("ibv_close_device failed");
            return 1;
        }
    }
    ibv_free_device_list(list);
    return 0;
}