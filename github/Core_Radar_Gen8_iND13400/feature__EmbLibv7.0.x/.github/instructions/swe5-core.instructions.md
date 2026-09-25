---
applyTo: "software/**/integration_test/**,software/**/ipc_data.h"
---

# SWE5 Core — Hooks, Enums, Interface Patterns (Repo-Verified)

## Centralized Hook Headers (ACTUAL REPO)

- **BBE32**: `software/bbe32/integration_test/dsp_it_macros_test.h`
- **R52**: `software/r52/integration_test/master_it_macros_test.h`
- Include UNCONDITIONALLY — headers have internal `#ifdef` guards
- Macros expand to `/* No operation */` when `Integration_Testing` undefined

### Real Hook Call Pattern (from `doppler_process_ifc.c:734-735`)
```c
#include "dsp_it_macros_test.h"
/* ... in function body ... */
   /* Providing d2m_msg_ptr info, for Integration Testing */
   SIT_D2M_MSG_BUFFER(d2m_msg_ptr);
```

### Real Hook Call Pattern (from `anglefinding_project_interface.c:603-604`)
```c
   /* Providing af_input_data_ptr info, for Integration Testing */
   SIT_AF_INPUT(af_input_data_ptr);
```

### Real Hook Call Pattern (from `dsp_setup.c:293-294`)
```c
      /* Providing m2d_one_time_init_ptr info, for Integration Testing */
      SIT_M2D_ONETIME((M2D_One_Time_Msg_T *)m2d_one_time_init_ptr);
```

## Available Hook Macros

### BBE32 (`dsp_it_macros_test.h`)
| Macro | Guard | Function |
|-------|-------|----------|
| `SIT_AF_INPUT(ptr)` | IT + AF_IT | `SIT_AF_Input_Receiver` |
| `SIT_D2M_MSG_BUFFER(ptr)` | IT + AF_IT | `SIT_D2M_Msg_Buffer_Provider` |
| `SIT_IPC_TRANSFER_M2D_RECEIVE(ptr)` | IT | `SIT_IPC_Transfer_M2D_Receive` |
| `SIT_IPC_TRANSFER_D2M_SEND(ptr)` | IT | `SIT_IPC_Transfer_D2M_Send` |

### R52 (`master_it_macros_test.h`)
| Macro | Guard | Function |
|-------|-------|----------|
| `SIT_WAIT_FOR_DEBUGGER()` | IT | spin-wait loop |
| `SIT_M2D_ONETIME(ptr)` | IT + AF_IT | `SIT_M2D_Onetime_Provider` |
| `SIT_M2D_PERLOOK(ptr)` | IT + AF_IT | `SIT_M2D_Perlook_Provider` |
| `SIT_IPC_TRANSFER_M2D_SEND(ptr)` | IT | `SIT_IPC_Transfer_M2D_Send` |
| `SIT_IPC_TRANSFER_D2M_RECEIVE(ptr)` | IT | `SIT_IPC_Transfer_D2M_Receive` |

## Interface Enum Pattern (from `dsp_integration_test.h`)

```c
#if defined(Anglefinding_IT)
   #define IF_ANGLEFINDING(x) x
#else
   #define IF_ANGLEFINDING(x)
#endif

#define INTERFACE_ENUM_LIST \
   IF_ANGLEFINDING(DCS_X(Ri_beam_vector, 392701U))   /* WI-392701: rdd1_bv[det][TX][RX] */ \
   IF_ANGLEFINDING(DCS_X(Ri_rdd_snr, 362404U))       /* WI-362404: rdd2_snr[det] */

typedef enum Test_Interface_Name_Tag {
   Test_Interface_None = 0U,
   #define DCS_X(name, value) name = value,
   INTERFACE_ENUM_LIST
   #undef DCS_X
} Test_Interface_Name_T;
```

## Provider Pattern (from `dsp_integration_test.c`)
```c
case Ri_beam_vector: /* WI-392701 */
   if (num_elem > MAX_DETS_FIRST_PASS * NUM_TX * NUM_RX)
   { num_elem = MAX_DETS_FIRST_PASS * NUM_TX * NUM_RX; }
   Set_Shared_Ptr(&DSP_Shared_Pi, d2m_msg_ptr->rdd1_bv, sizeof(uint16_t),
                  num_elem, Ri_beam_vector, DATATYPE_UINT16);
   Stub_3D_Array_Values(d2m_msg_ptr->rdd1_bv, DATATYPE_UINT16, DSP_Stub_Val, dim1, dim2, dim3);
   break;
```

## Receiver Pattern (from `dsp_integration_test.c`)
```c
case Ri_host_vehicle_speed:
{
   is_valid = Validate_Scalar_Values(af_input_ptr->p_veh_speed, DATATYPE_FLOAT32, DSP_Stub_Val);
   Set_Shared_Ptr(&DSP_Shared_Ri, af_input_ptr->p_veh_speed, sizeof(float),
                  1, Ri_host_vehicle_speed, DATATYPE_FLOAT32);
}
break;
```

## Key Data Types (from `dsp_integration_test.h`)
```c
typedef union Data_Ptr_Tag {
   void *ptr; uint8_t *u8; uint16_t *u16; uint32_t *u32;
   int16_t *i16; int32_t *i32; float *f32;
} Data_Ptr_T;

typedef struct G_Pointer_Tag {
   Data_Ptr_T data; size_t elem_size; size_t no_of_elements;
   Test_Interface_Name_T name; DataType_T data_type;
} G_Pointer_T;
```

## IPC Structure (from `ipc_data.h`)
```c
typedef struct SIT_Data_Tag {
   double stub_value;
   uint32_t test_interface_number;
   uint32_t no_of_elements_in_stub_array;
   SIT_Test_Result_T test_result;
} __attribute__((aligned(32))) SIT_Data_T;
```

## Rules
- Enum values = unique Work Item IDs (NOT sequential)
- `Set_Shared_Ptr()` takes enum, NOT string
- `G_Pointer_T.name` type = `Test_Interface_Name_T`
- `SWE5_VALIDATION_TOLERANCE = 1e-5F`
- Always boundary-check arrays before stub/validate
- NULL-check pointers at function entry
- `AF_CAL_SEC_N = 2` on Gen8 (calmat3/4 → TEST_SKIPPED)
