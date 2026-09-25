---
mode: agent
description: "Add a new SWE5 integration test interface"
---

# Add SWE5 Interface

## Input Required
- **Interface name**: `Ri_<field>` or `Ro_<field>`
- **Work Item ID**: Unique integer (e.g., 600001U)
- **Data pointer**: `struct->field` or `struct->array`
- **Data type**: `TYPE_UINT8`/`TYPE_UINT16`/`TYPE_UINT32`/`TYPE_INT16`/`TYPE_FLOAT32`
- **Core**: BBE32 or R52
- **Module guard**: `IF_ANGLEFINDING` / `IF_MODULE`
- **Context**: Which function provides/receives this data

## Steps

### 1. Add to INTERFACE_ENUM_LIST
File: `software/bbe32/integration_test/dsp_integration_test.h` (or R52 equivalent)
```c
IF_ANGLEFINDING(DCS_X(Ri_field_name, 600001U))  \
```
Location: inside `#define INTERFACE_ENUM_LIST \` at appropriate module section.

### 2. Add Provider Case (in SIT_Provide_* function)
File: `software/bbe32/integration_test/dsp_integration_test.c`
```c
case Ri_field_name:
    Set_Shared_Ptr(&DSP_Shared_Pi, (void *)&struct_ptr->field, (uint32_t)sizeof(struct_ptr->field), TYPE_XXX);
    Stub_Scalar_Values((void *)&struct_ptr->field, TYPE_XXX, DSP_Stub_Val);
    break;
```

### 3. Add Receiver Case (in SIT_Receive_* function)
```c
case Ri_field_name:
{
    DSP_Res = Validate_Scalar_Values((void *)&struct_ptr->field, TYPE_XXX, DSP_Stub_Val);
    Set_Shared_Ptr(&DSP_Shared_Ri, (void *)&struct_ptr->field, (uint32_t)sizeof(struct_ptr->field), TYPE_XXX);
}
break;
```

### 4. Build Both Configs
```bash
bazelisk build //:gen8 --config=flr8
bazelisk build //:gen8 --config=flr8 --Integration_Testing=true --Anglefinding_IT=true
```

## Notes
- Provider stubs the value INTO the pointer (before algorithm runs)
- Receiver validates the value FROM the pointer (after algorithm runs)
- `Set_Shared_Ptr` takes enum name (NOT string)
- Arrays: use array element sizeof, cast count to `uint32_t`
- `DSP_Stub_Val` is the debugger-injected double value
- Both Pi (provider info) and Ri (receiver info) exposed via shared pointers
