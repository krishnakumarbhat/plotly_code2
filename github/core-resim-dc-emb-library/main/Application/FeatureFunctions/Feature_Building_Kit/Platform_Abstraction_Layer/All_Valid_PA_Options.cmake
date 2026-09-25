# ######################################################################################################################
# Define mapping between customers and abstraction layer. Customer variants like Sensor or ECU are allowed to have a
# whole different abstraction layer
# ######################################################################################################################

# Set consumers of GDSR abstraction layer
set(PA_GDSR_Consumer "Generic" "Honda_SRR6")

# Set consumers of F360 abstraction layer
set(PA_F360_Consumer "BMW_SP25" "STLA_Thunder" "Rivian_SRR6")

# Set consumers of Generic
set(PA_Generic "Generic") 
