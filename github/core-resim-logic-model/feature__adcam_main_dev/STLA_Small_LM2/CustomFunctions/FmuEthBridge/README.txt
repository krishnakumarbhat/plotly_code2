FmuEthBridge  –  dSPACE SCALEXIO Custom Function
=================================================
Aptiv  |  STLA Small LM2  |  April 2026  |  v5.6


PURPOSE
-------
Bridges the SCALEXIO Ethernet hardware to the LRCF FMU (FMI 2.0) without
putting any socket code inside the FMU.

Due to the FMI 2.0 limitation (no native byte-array variable type), Ethernet
frames are exchanged using a 64-bit pointer-triplet split across three FMI
Integer variables — the same mechanism already proven on SCALEXIO by the ZMQ
Custom Function (in-process pointer sharing).

RX and TX traffic are assigned to separate physical Ethernet channels, each
backed by its own Custom Ethernet Setup CF block.  Both directions can share
a single channel when the application only has one NIC.


FILES IN THIS PACKAGE
---------------------
  FmuEthBridge CF  (single self-contained block — no other CF needed)
  ──────────────────────────────────────────────────────────────────────
  FmuEthBridge.xml            ConfigurationDesk descriptor
  FmuEthBridge_TypeDef.h      Instance/parameter struct
  FmuEthBridge.h              C function declarations
  FmuEthBridge.cpp            Full implementation

  FMU
  ──────────────────────────────────────────────────────────────────────
  LogicModel2.fmu             FMI 2.0 Co-Simulation FMU (linux64)
                              551 variables: CAN RX/TX + 6 ETH Integer vars


CONFIGURATIONDESK SETUP  (step-by-step)
----------------------------------------
  Step 1 — Add FmuEthBridge

    Add one "FmuEthBridge" CF instance to your ConfigurationDesk project.
    No other Custom Function is needed.

  Step 2 — Assign hardware channels

    Open FmuEthBridge → Electrical Interface.  Two channel dropdowns appear:

      RX Ethernet Adapter  →  select the SCALEXIO NIC receiving sensor data
                               (e.g. NET1)
      TX Ethernet Adapter  →  select the SCALEXIO NIC sending LRCF output
                               (e.g. NET2)

    If a single NIC handles both directions, select the same port in both
    dropdowns.

  Step 3 — Set parameters

    ┌────────────────────┬───────────────────┬────────────────────────────────┐
    │ Parameter          │ Default           │ Description                    │
    ├────────────────────┼───────────────────┼────────────────────────────────┤
    │ RX Port            │ 5001              │ UDP port SCALEXIO listens on   │
    │ RX Local Address   │ 192.168.10.1      │ SCALEXIO IP on the RX NIC      │
    │ RX Subnet Mask     │ 255.255.255.0     │ Subnet mask of the RX NIC      │
    │ TX Port            │ 5002              │ UDP port on the receiving host  │
    │ TX Remote IP       │ 192.168.1.10      │ IP of the host receiving frames │
    │ TX Local Address   │ 192.168.20.1      │ SCALEXIO IP on the TX NIC      │
    │ TX Subnet Mask     │ 255.255.255.0     │ Subnet mask of the TX NIC      │
    │ Max Frame Size     │ 1500              │ Max Ethernet payload (bytes)   │
    └────────────────────┴───────────────────┴────────────────────────────────┘

    RX Local Address / TX Local Address must match the IP assigned to each
    physical NIC in the hardware configuration.

  Step 4 — Wire FMU Integer variables

    Each CF port is a scalar Int32 signal.  Wire directly to the FMU Integer
    variable with no demux or type conversion needed.

    ┌──────────────────────────┬────────────┬──────────────────────────────┐
    │ Custom Function port     │ Direction  │ FMU Integer Variable         │
    ├──────────────────────────┼────────────┼──────────────────────────────┤
    │ EthRxLo                  │ CF → FMU   │ EthRxIn.lo        VR 5000    │
    │ EthRxHi                  │ CF → FMU   │ EthRxIn.hi        VR 5001    │
    │ EthRxSize                │ CF → FMU   │ EthRxIn.size      VR 5002    │
    ├──────────────────────────┼────────────┼──────────────────────────────┤
    │ EthTxLo                  │ FMU → CF   │ EthTxOut.lo       VR 5003    │
    │ EthTxHi                  │ FMU → CF   │ EthTxOut.hi       VR 5004    │
    │ EthTxSize                │ FMU → CF   │ EthTxOut.size     VR 5005    │
    └──────────────────────────┴────────────┴──────────────────────────────┘


EXECUTION ORDER
---------------
  Automatically enforced — no action needed from the integrator:
    - RxLo/RxHi/RxSize are Direction="Out" so ConfigurationDesk schedules them BEFORE fmi2DoStep.
    - TxLo/TxHi/TxSize are Direction="In"  so ConfigurationDesk schedules them AFTER  fmi2DoStep.
    - Within the CF block, port order is fixed by the CModule declaration order in the XML.

  For reference:
    1. EthRxLo   — recvfrom() on RX NIC → cache lo/hi/size → output lo to FMU VR 5000
    2. EthRxHi   — output cached hi   to FMU VR 5001
    3. EthRxSize — output cached size to FMU VR 5002
    4. FMU fmi2DoStep
    5. EthTxLo   — cache lo   from FMU VR 5003
    6. EthTxHi   — cache hi   from FMU VR 5004
    7. EthTxSize — cache size from FMU VR 5005 → sendto() on TX NIC


HOW THE POINTER-TRIPLET WORKS
------------------------------
  RX side (EthRxLo / EthRxHi / EthRxSize → FMU):
    The CF receives a UDP frame into its internal rx_buf (heap-allocated,
    address is fixed for the life of the application).  It encodes the
    buffer address as two Int32 values (bit-pattern preserved):

      lo   = (uint32_t)(addr & 0xFFFFFFFF)
      hi   = (uint32_t)(addr >> 32)
      size = bytes received  (0 = no frame this step)

    The FMU reconstructs the pointer:
      uintptr_t addr = ((uintptr_t)hi << 32) | lo;

  TX side (FMU → EthTxLo / EthTxHi / EthTxSize):
    The FMU writes its output buffer address into lo/hi/size after DoStep.
    The CF reads back the pointer and calls sendto() via the TX NIC.
    The FMU's buffer (g_tx_buf inside LogicModel2.so) is in the same
    SCALEXIO process address space — the pointer is valid during EthTxSize.

  NIC binding:
    FmuEthBridge initializes both NIC drivers directly at startup (no
    separate Custom Ethernet Setup block needed).  The RX socket is bound
    to RX Local Address so it only receives packets arriving on that
    physical port.  The TX socket is bound to TX Local Address so sendto()
    routes through the correct NIC regardless of the OS routing table.


SMOKE TEST (algo_adapter.cpp — v5.4 behaviour)
-----------------------------------------------
  The supplied algo_adapter.cpp contains a smoke-test stub that proves the
  full data path without the real LRCF algorithm library:

    ETH loopback   — any frame received on EthRxIn is echoed back on EthTxOut
                     in the same DoStep.  Visible on the network after sendto().

    CAN passthrough — two vehicle-dynamics signals piped through:
      BSM_DATA_4.CARBODY_LONG_ACCEL_CORRECTED  →  LRCF_DATA_2.ADAS_LONGI_ACCEL_REQUEST   [m/s2]
      BSM_DATA_4.CARBODY_YAW_VELOCITY_FILTERED →  LRCF_DATA_2.YAW_RATE_REQUEST_SETPOINT  [Degree/s]

    Moving patterns (driven by step counter, incremented every DoStep):
      LRCF_DATA_2/9.E2E_ALIVE_*     — 4-bit rolling counter 0-15
      LRCF_DATA_9.ACC_FRONT_TARGET_DISTANCE         — sawtooth 0-199 m
      LRCF_DATA_2.ACC_POTENTIAL_ACCELERATION_REQ    — sine ±5 m/s²

  Replace this block with the real LRCF_process() call when the algorithm
  library is available.


DEPENDENCIES (dSPACE framework — not included, present on SCALEXIO build system)
----------------------------------------------------------------------------------
  DsTypes.h, DsMsg.h
  dssimengine_api.h, rtosal_simengineap.h
  DsIoFuncEthernetInterfaceManagement.h
  DsIoFuncEthernetChannel.h
  IOCode_Data.h
  Library: dsethernetinterfacemanagementdrv  (listed in FmuEthBridge.xml MakeConfiguration)
