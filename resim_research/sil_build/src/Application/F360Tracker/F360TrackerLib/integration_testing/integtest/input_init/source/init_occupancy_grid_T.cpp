#include "ocg_occupancy_grid_types.h"
#include "ocg_underdrivability_enum.h"
#include "init_occupancy_grid_T.h"

namespace f360_variant_A
{

void init_occupancy_grid_T(ocg::OCG_Outputs_T& occupancy_grid)
{
    occupancy_grid.timestamp = 0;
    occupancy_grid.iteration_index = 0;
    occupancy_grid.grid_definition.num_cells_x_far = ocg::NUM_CELLS_X_FAR;
    occupancy_grid.grid_definition.num_cells_x_mid = ocg::NUM_CELLS_X_MID;
    occupancy_grid.grid_definition.num_cells_x_close = ocg::NUM_CELLS_X_CLOSE;
    occupancy_grid.grid_definition.num_cells_y = 1U;

    occupancy_grid.grid_definition.cell_length = 2.0F;
    occupancy_grid.grid_definition.cell_width = 6.0F;
    occupancy_grid.grid_definition.cell_width_extension_factor = 1.666666666F;

    occupancy_grid.underdrivability.grid_curvature = 0.0F;

    occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x = 0.0F;
    occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y = 0.0F;
    occupancy_grid.underdrivability.ogcs_host_rear_axle_position.z = 0.0F;
    occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw = 0.0F;

    
    occupancy_grid.underdrivability.underdrivability_status[0][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[1][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[2][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[3][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[4][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[5][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[6][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[7][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[8][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[9][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[10][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[11][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[12][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[13][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[14][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[15][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[16][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[17][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[18][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[19][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[20][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[21][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[22][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[23][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[24][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[25][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[26][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[27][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[28][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[29][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[30][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[31][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[32][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[33][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[34][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[35][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[36][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[37][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[38][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[39][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[40][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[41][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[42][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[43][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[44][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[45][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[46][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[47][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[48][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

    occupancy_grid.underdrivability.underdrivability_status[49][0] = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
}
}
