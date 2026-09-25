![](data:image/jpeg;base64...)

***ConnX BBE32EP DSP User's Guide***

Cadence Design Systems, Inc.

2655 Seely Ave.

San Jose, CA 95134 www.cadence.com

Copyright © 2019 Cadence Design Systems,

Inc. All Rights Reserved

This publication is provided “AS IS.” Cadence Design Systems, Inc. (hereafter “Cadence") does not make any warranty of any kind, either expressed or implied, including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose. Information in this document is provided solely to enable system and software developers to use our processors. Unless specifically set forth herein, there are no express or implied patent, copyright or any other intellectual property rights or licenses granted hereunder to design or fabricate Cadence integrated circuits or integrated circuits based on the information in this document. Cadence does not warrant that the contents of this publication, whether individually or as one or more groups, meets your requirements or that the publication is error-free. This publication could include technical inaccuracies or typographical errors. Changes may be made to the information herein, and these changes may be incorporated in new editions of this publication.

© 2019 Cadence, the Cadence logo, Allegro, Assura, Broadband Spice, CDNLIVE!, Celtic, Chipestimate.com, Conformal, Connections, Denali, Diva, Dracula, Encounter, Flashpoint, FLIX, First Encounter, Incisive, Incyte, InstallScape, NanoRoute, NC-

Verilog, OrCAD, OSKit, Palladium, PowerForward, PowerSI, PSpice, Purespec, Puresuite, Quickcycles, SignalStorm, Sigrity,

SKILL, SoC Encounter, SourceLink, Spectre, Specman, Specman-Elite, SpeedBridge, Stars & Strikes, Tensilica, TripleCheck, TurboXim, Virtuoso, VoltageStorm, Xplorer, Xtensa, and Xtreme are either trademarks or registered trademarks of Cadence Design Systems, Inc. in the United States and/or other jurisdictions.

OSCI, SystemC, Open SystemC, Open SystemC Initiative, and SystemC Initiative are registered trademarks of Open SystemC Initiative, Inc. in the United States and other countries and are used with permission. All other trademarks are the property of their respective holders.

Xtensa Release: RI-2019.1 Issue Date: 5/2019

Modification: 477813

Cadence Design Systems, Inc.

2655 Seely Ave. San Jose, CA 95134 www.cadence.com

# Contents

List of Tables............................................................................................................................vii

List of Figures...........................................................................................................................ix

1. Changes from the Previous Version ...................................................................................11
2. Introduction......................................................................................................................... 13
   1. Purpose of this User's Guide..................................................................................... 15
   2. Installation Overview..................................................................................................15
   3. ConnX BBE32EP DSP Architecture Overview.......................................................... 15 2.4 ConnX BBE32EP DSP Instruction Set Overview.......................................................18

2.5 Programming Model and XCC Vectorization............................................................. 19

1. ConnX BBE32EP DSP Features.........................................................................................21
   1. ConnX BBE32EP DSP Register Files........................................................................22
   2. ConnX BBE32EP DSP Architecture Behavior...........................................................25
   3. Operation Naming Conventions.................................................................................26
   4. Fixed Point Values and Fixed Point Arithmetic.......................................................... 35
   5. Data Types Mapped to the Vector Register File........................................................ 37
   6. Data Typing................................................................................................................39
   7. Multiplication Operation............................................................................................. 41
   8. Vector Select Operations...........................................................................................443.9 Vector Shuffle Operations..........................................................................................45
   9. Block Floating Point.................................................................................................46
   10. Complex Conjugate Operations...............................................................................46
   11. FLIX Slots and Formats...........................................................................................47
2. Programming a ConnX BBE32EP DSP..............................................................................49
   1. Programming in Prototypes....................................................................................... 52
   2. Xtensa Xplorer Display Format Support.................................................................... 54
   3. Operator Overloading and Vectorization....................................................................55
   4. Programming Styles...................................................................................................56
   5. Conditional Code....................................................................................................... 63
   6. Using the Two Local Data RAMs and Two Load/Store Units.....................................64
   7. Other Compiler Switches...........................................................................................66
   8. TI C6x Instrinsics Porting Assistance Library.............................................................66
3. Configurable Options.......................................................................................................... 71
   1. FFT............................................................................................................................ 72
   2. Symmetric FIR...........................................................................................................73
   3. Packed Complex Matrix Multiply................................................................................76
   4. LFSR and Convolutional Encoding............................................................................78
   5. Linear Block Decoder.................................................................................................80
   6. 1D Despread..............................................................................................................80
   7. Soft-bit Demapping....................................................................................................82
   8. Comparison of Divide Related Options......................................................................84
      1. Vector Divide ...................................................................................................87
      2. Fast Vector Reciprocal & Reciprocal Square Root..........................................88
      3. Advanced Vector Reciprocal & Reciprocal Square Root.................................89
   9. Advanced Precision Multiply/Add...............................................................................91 5.10 Inverse Log-likelihood Ratio (LLR)...........................................................................95 5.11 Single and Dual Peak Search.................................................................................. 97

5.12 Single-precision Vector Floating-Point.....................................................................99

1. Floating-Point Operations................................................................................................. 101
   1. ConnX BBE32EP DSP Floating-Point Features......................................................103 ConnX BBE32EP DSP Register Files Related to Floating-Point............................103

ConnX BBE32EP DSP Architecture Behavior Related to Floating-Point............... 103

Binary Floating-point Values...................................................................................104 Signed Zero Number.......................................................................................104

Subnormal Number......................................................................................... 104

Normal Number...............................................................................................104

Biased and Unbiased Exponents.................................................................... 104 Half Precision Data.................................................................................................105

Single Precision Data............................................................................................. 105

Maximum Possible Error in the Unit in the Last Place (ULP)................................. 106

Encodings of Infinity and Not a Number (NaN).......................................................106

Scalar Float Data Types Mapped to the Vector Register File.................................107

Vector Float Data Types Mapped to the Vector Register File.................................107

Floating-Point Data Typing..................................................................................... 108

Floating-Point Rounding and Exception Controls...................................................108

Floating-Point Status/Exception Flags....................................................................109

6.2 ConnX BBE32EP DSP Floating-Point Operations...................................................110 Arithmetic Operations..............................................................................................111 Floating-point Conversion Operations.....................................................................111 Integer to/from Float Conversion Operations..........................................................111

Float to Integral Value Rounding Operations..........................................................112

Classification, Comparison, and Min/Max Operations............................................ 112

Division and Square Root Operations.....................................................................113

Complex Arithmetic Operations.............................................................................. 114

Reciprocal, Reciprocal Square Root, and Fast Square Root Operations...............114

Notes on Not a Number (NaN) Propagation........................................................... 115

6.3 ConnX BBE32EP DSP Floating-Point Programming...............................................115 General Guidelines for Floating-Point Programming.............................................. 115

Invalid Floating-Point Operation Examples.............................................................116 Exception, and Exception Reduction...................................................................... 116

Associative and Distributive Rules, as well as Expression Transformations.......... 117

Fused-multiply-add (FMA), and Parallel-Add-Sub (ADDSUB)................................117

Division, Square Root, Reciprocal, and Reciprocal Square Root Intrinsic

Functions........................................................................................................... 118

Sources of NaN, and Potential Changes of NaN Sign and Payload.......................118

Auto-Vectorizing Scalar Floating-Point Programs...................................................118

Compiler Switches Related to Floating-point Programs..........................................118

Domain Expertise, Vector Data-types, C Instrinsics, and Libraries........................ 120

6.4 Accuracy and Robustness Optimizations on ConnX BBE32EP DSP......................120

Historical Floating Point Units.................................................................................120

Comparisons and Measurements against Infinitely Precise Results......................121

Depending on Rounding Mode vs. Not; Standards Conforming vs. Beyond..........121

Computer Algebra, Algorithm Selections, Default Substitutions, and Exception

Flags..................................................................................................................122

Error Bounds, Interval Arithmetic, and Rounding Mode......................................... 123

FMA, Split Number................................................................................................. 123

6.5 Implementing the Floating Point FFT/IFFT on ConnX BBE32EP DSP Vector

Floating Point Unit (VFPU)........................................................................................124

Radix-4 FFT implementation.................................................................................. 124

Intrinsic Code for Vectroized Radix-4 Butterfly and Twiddle Multiplication .....125 Optimizations of Implementation of IFFT.........................................................125

Performance of FFT on ConnX BBE32EP DSP-VFPU................................... 126

Twiddle Size.................................................................................................... 127

6.6 Floating Point References........................................................................................128

1. Special Operations............................................................................................................129
   1. Polynomial Evaluation..............................................................................................130
   2. Matrix Computation..................................................................................................131
   3. Pairwise Real Multiply Operation.............................................................................132
   4. Descramble Operations...........................................................................................133
   5. Vector Compression and Expansion........................................................................134
   6. Predicated Vector Operations..................................................................................135
2. Load & Store Operations...................................................................................................139
   1. ConnX BBE32EP DSP Addressing Modes..............................................................140
   2. Aligning Loads and Stores.......................................................................................140
   3. Circular Addressing..................................................................................................142
   4. Variable Element Vector Aligning Loads and Stores................................................143
   5. Update Post-increment in Loads and Stores...........................................................143
3. Nature DSP Signal Library................................................................................................145
4. Implementation Methodology..........................................................................................147
   1. Configuring a ConnX BBE32EP DSP....................................................................148
   2. XPG Estimation for Size, Performance and Power................................................150
   3. Basic ConnX BBE32EP DSP Characteristics........................................................150
   4. Extending a ConnX BBE32EP DSP with User TIE................................................150

Compiling User TIE.................................................................................................152

Name Space Restrictions for User TIE...................................................................152

* 1. XPG Configuration Options and Capabilities.........................................................154
  2. Sample Configuration Templates for the ConnX BBE32EP DSP ......................... 157
  3. Synthesis and Place-and-Route............................................................................ 163
  4. ConnX BBE32EP DSP Memory Floor-planning Suggestions................................163
  5. Mapping the ConnX BBE32EP DSP to FPGA.......................................................165 11 On-Line ISA, Protos and Configuration Information........................................................167

# List of Tables

Table 1: Basic Instruction Formats..........................................................................................17 Table 2: ConnX BBE32EP DSP Multiply Performance........................................................... 19 Table 3: Sample Categories of Operations............................................................................. 28 Table 4: Types of Load/Store Operations................................................................................34 Table 5: Vector Data Types Mapped to Vector Register Files.................................................38 Table 6: Scalar Memory Data Types.......................................................................................39 Table 7: Scalar Register Data Types.......................................................................................40 Table 8: Vector Memory Data Types.......................................................................................40 Table 9: Vector Register Data Types.......................................................................................40 Table 10: Types of ConnX BBE32EP DSP Multiplication Operations.....................................41 Table 11: Vector Initialization Operation Arguments................................................................44 Table 12: Decoding Complex Codes for Despreading............................................................82 Table 13: Set of Symbol Constellations Supported.................................................................83

Table 14: Comparison of Divide Related Options................................................................... 84 Table 15: Advanced Precision Multiply/Add Operations Overview..........................................92 Table 16: Use cases of peak search operations..................................................................... 99 Table 17: Components of a Binary Floating-point Value....................................................... 104 Table 18: Half Precision Data................................................................................................105 Table 19: Single Precision Data............................................................................................105 Table 20: Encodings of Infinity and Not a Number (NaN)..................................................... 107 Table 21: Complex Single Precision Value............................................................................107 Table 22: Vector Float/Double Data Types Mapped to the Vector Register File................... 107

Table 23: FCR Fields.............................................................................................................108 Table 24: FCR Fields and Meaning.......................................................................................109 Table 25: FSR Fields.............................................................................................................110 Table 26: FSR Fields and Meaning....................................................................................... 110 Table 27: Protos for Descrambling........................................................................................134 Table 28: Predicated Vector Operations................................................................................136 Table 29: ConnX BBE32EP DSP Addressing Modes........................................................... 140 Table 30: Circular Buffer State Registers..............................................................................142 Table 31: State and Register File Names..............................................................................152 Table 32: User Register Entries............................................................................................ 153 Table 33: Simulation Modeling Capabilities...........................................................................155 Table 34: Instruction Extension.............................................................................................155 Table 35: Allowed Architecture Definition..............................................................................155 Table 36: Instruction Width....................................................................................................155 Table 37: Coprocessor Configuration Options...................................................................... 156

Table 38: Local Memories.....................................................................................................156 Table 39: TIE Option Packages.............................................................................................156 Table 40: ConnX BBE32EP DSP Base Configuration...........................................................157 Table 41: ConnX BBE32EP DSP Configuration Options...................................................... 162

Table 42: Xilinx Synthesis Results (RE-2014.0)....................................................................166

# List of Figures

Figure 1: ConnX BBE32EP DSP Architecture........................................................................ 16

Figure 2: ConnX BBE32EP DSP Register Files......................................................................23 Figure 3: Base Xtensa ISA Register Files...............................................................................24 Figure 4: Radix4 FFT Pass.....................................................................................................72 Figure 5: 16-tap Real Symmetric FIR with Complex Data...................................................... 76 Figure 6: Inverse LLR Calculation...........................................................................................96 Figure 7: Radix - 4 Butterfly Implementation.........................................................................124 Figure 8: Performance of Complex Float FFT.......................................................................126

Figure 9: ConnX BBE32EP DSP Configuration Options in Xtensa Xplorer..........................149

# 1. Changes from the Previous Version

The following changes were made to this document for the RI-2019.1 release:

• Removed references to discontinued DSPs

# 2. Introduction

**Topics:**

* *Purpose of this User's Guide*
* *Installation Overview*
* *ConnX BBE32EP DSP*

*Architecture Overview*

* *ConnX BBE32EP DSP*

*Instruction Set Overview*

* *Programming Model and*

*XCC Vectorization*

The Cadence® Tensilica® ConnX BBE32EP DSP (32-MAC

Baseband Engine) is based on a ultra-high performance DSP architecture designed for use in next-generation baseband processors for LTE Advanced, other 4G cellular radios and multi-standard broadcast receivers. The high computational requirements of such applications require new and innovative architectures with a high degree of parallelism and efficient I/Os. The ConnX BBE32EP DSP meets these needs by combining a 16-way Single Instruction, Multiple Data (SIMD), 32 multiplieraccumulators (MAC) and up to 5-issue Very Long Instruction Word (VLIW) processing pipeline with a rich and extensible set of interfaces.

The ConnX BBE family natively supports both real and complex arithmetic operations. For digital signal processing developers, this greatly simplifies development of algorithms dominated by complex arithmetic. In addition to having the SIMD/VLIW DSP core, the ConnX BBE32EP DSP contains a 32-bit scalar processor, ideal for efficient execution of control code. This combined SIMD/VLIW/ Scalar design makes the ConnX BBE32EP DSP ideal for building real systems where high computational throughput is combined with complex decision making.

The ConnX BBE32EP DSP is built around a core vector pipeline consisting of thirty-two 16bx16b MACs along with a set of versatile pipelined execution units. These units support flexible precision real and complex multiply-add; bit manipulation; data shift and normalization; data select, shuffle and interleave. The ConnX BBE32EP DSP multipliers and its associated adder and multiplexer trees enable execution of complex multiply operations and signal processing filter structures in parallel. The results of these operations can be extended up to a precision of 40bits per element, truncated/rounded/saturated or shifted/ packed to meet the needs of different algorithms and implementations. The ConnX BBE32EP DSP instruction set is optimized for several DSP kernel operations and matrix multiplies with added acceleration for a wide range of key wireless functions. In addition, the instruction set

supports signed\*unsigned multiplies for emulation of 32bit wide multiplication operations.

The ConnX BBE32EP DSP supports programming in C/C ++ with a vectorizing compiler. Automatic vectorization of scalar C and full support for vector data types allows software development of algorithms without the need for programming at assembly level. Native C operator overloading is supported for natural programming with standard C operators on real and complex vector datatypes. The ConnX BBE32EP DSP has a Boolean predication architecture that supports a large number of predicated operations. This enables the ConnX BBE32EP DSP compiler to achieve a high vectorization throughput even with complicated functions that have conditional operations embedded in their inner loops.

The BBE32EP and its larger cousin, the BBE64EP share a common architecture, providing a high degree of code portability between the two cores. Both cores share a common set of check box options that allow capabilities to be added/subtracted from the core. This permits the system designer to optimize the core for a particular application space reducing both area and power consumption.

## 2.1 Purpose of this User's Guide

The ConnX BBE32EP DSP User’s Guide provides an overview of the ConnX BBE32EP DSP architecture and its instruction set. It will help ConnX BBE32EP DSP programmers identify commonly used techniques to vectorize algorithms. It provides guidelines to improve software performance through the use of appropriate ConnX BBE32EP DSP instructions, intrinsics, protos and primitives. It also serves as a reference for programming the ConnX BBE32EP DSP in a C/C++ software development environment using the Xtensa Xplorer (XX)

Integrated Development Environment (IDE). Additionally, this guide will assist those ConnX BBE32EP DSP users who wish to add custom operations (more hardware) to the ConnX BBE32EP DSP instruction set using Tensilica® Instruction Extension (TIE) language.

To use this guide most effectively, a basic level of familiarity with the Xtensa software development flow is highly recommended. For more details, refer to the *Xtensa Software Development Toolkit User’s Guide.*

Throughout this guide, the symbol <xtensa\_root> refers to the installation directory of the user's Xtensa configuration. For example, <xtensa\_root> might refer to the directory /usr/ xtensa/<user>/<s1> if <user> is the username and <s1> is the name of the user’s Xtensa configuration. For all examples in this guide, replace <xtensa\_root> with the path to the installation directory of the user’s Xtensa distribution.

## 2.2 Installation Overview

To install a ConnX BBE32EP DSP configuration, follow the same procedures described in the *Xtensa Development Tools Installation Guide*. The ConnX BBE32EP DSP comes with a library of examples provided in the XX workspace called bbe32ep\_examples\_re\_v<version\_num>.xws.

The ConnX BBE32EP DSP include-files are in the following directories and files:

<xtensa\_root>/xtensa-elf/arch/include/xtensa/config/defs.h

<xtensa\_root>/xtensa-elf/arch/include/xtensa/tie/xt\_bben.h

![](data:image/png;base64...)**Note:** There is an additional include header file for TI C6x code compatibility, which is discussed in *TI C6x Instrinsics Porting Assistance Library* on page 66. This include file maps TI C6x intrinsics into standard C code and is meant to assist porting only.

## 2.3 ConnX BBE32EP DSP Architecture Overview

ConnX BBE32EP DSP, a 16-way SIMD processor, has the ability to work on several data elements in parallel at the same time. The ConnX BBE32EP DSP executes a single operation simultaneously across a stream of data elements by means of vector processing. For example, it allows for vector additions through a narrow vector ADD (sum of two 16element 16-bits/element vectors) or wide vector (sum of two 16-element 40-bits/element vectors) ADD, in parallel. These operations include optimized instructions for complex multiplication and multiply-accumulation, matrix computation, vector division (optional), vector reciprocal & reciprocal square root (optional) and other performance critical kernels.

ConnX BBE32EP DSP has a 5-slot VLIW architecture, in which up to five operations can be scheduled and dispatched in parallel every cycle. This allows the processor to support multiply-accumulate operations of two narrow vectors of eight 16-bit complex (32-bit realimaginary pair) elements in parallel, equivalently thirty-two 16-bit real elements in total, with a load of eight complex operands and a store of eight complex results in every cycle. To sustain such high memory bandwidth requirements, the ConnX BBE32EP DSP has two asymmetric Load/Store Units (LSUs) which can independently communicate with two local data memories.

For higher efficiency, the ConnX BBE32EP DSP fetches instructions out of a 128-bit wide access to local instruction memory (IRAM). The instruction fetch interface supports a mix of 16/24-bit single instructions and 48/96-bit FLIX (up to 5-way) instructions. The processor can also read from and write to system memory and devices attached to the standard system buses. Other processors or DMA engines can transfer data in and out of the local memories in parallel with the processor pipeline. The processor can also have an instruction cache. The ConnX BBE32EP DSP can also be supplemented with any number of wide, high-speed I/O interfaces (data cache, TIE ports and queues) to directly control devices or hardware blocks, and to move data directly into and out of the processor register files.

![](data:image/jpeg;base64...)

### Figure 1: ConnX BBE32EP DSP Architecture

The ConnX BBE32EP DSP architecture uses variable length instructions, with encodings of

16/24-bits for its baseline Xtensa RISC instructions, and 48/96-bits for up to five operations in VLIW that may be issued in parallel. The Xtensa compiler schedules different operations into up to five VLIW slots available. The VLIW instruction Slot-0 is used to issue mostly loads and/or store operations. The instruction Slot-1 is used primarily for load operations using the second LSU while the instruction Slot-4 schedules most move operations. The instruction Slot-2 mostly allows ALU with multiply operations while the instruction Slot-3 is purely for ALU operations present in the ConnX BBE32EP DSP instruction set. However, it is important to note that the positions of these slots are all interleaved in an actual instruction word much differently than the software view.

The table below illustrates all the basic instruction formats supported by the different operation slots available in the ConnX BBE32EP DSP VLIW architecture. The Xtensa C Compiler (XCC) automatically picks an instruction format that offers the best schedule for an application. When possible, XCC will attempt to pick a 48-bit FLIX format or 16/24-bit standard instruction format to reduce code size.

![](data:image/png;base64...) **Note:**

* Users may optionally add additional 48/96-bit instructions formats as user TIE; see *Extending a ConnX BBE32EP DSP with User TIE* on page 150.
* Format 8 is available only when the *Advanced Precision Multiply/Add* option is present in a ConnX BBE32EP DSP configuration.
* Format 13 and Format 14 are available only when the *Single-precision Vector Floating-point* option is present in a ConnX BBE32EP DSP configuration.

### Table 1: Basic Instruction Formats

|  |  |  |  |  |  |
| --- | --- | --- | --- | --- | --- |
|  | **Slot 0** | **Slot 1** | **Slot 2** | **Slot 3** | **Slot 4** |
| Format0:96b | F0\_S0\_LdStALU | F0\_S1\_LdPk | F0\_S2\_Mul | F0\_S3\_ALU | - |
| Format1:96b | F1\_S0\_St | F1\_S1\_Base | F1\_S2\_WALUMul | F1\_S3\_ALU | F1\_S4\_Mov e |
| Format2:96b | F2\_S0\_LdSt | F2\_S1\_Ld | F2\_S2\_WALUMul | F2\_S3\_ALU | F2\_S4\_Mov e |
| Format3:96b | F3\_S0\_St | F3\_S1\_Ld | F3\_S2\_ALUMul | F3\_S3\_ALU | F3\_S4\_Mov e |
| Format4:96b | F4\_S0\_LdSt | F4\_S1\_LdPkDiv | F4\_S2\_Mul | F4\_S3\_ALU | - |
| Format5:96b | F5\_S0\_St | F5\_S1\_LdPk | F5\_S2\_Mul | F5\_S3\_ALU | F5\_S4\_Shfl |
| Format6:96b | F6\_S0\_St | F6\_S1\_LdPk | F6\_S2\_Mul | F6\_S3\_ALUFIRFFT | F6\_S4\_Mov e |
| Format7:96b | F7\_S0\_St | F7\_S1\_Base | F7\_S2\_ALUMul | F7\_S3\_ALU | F7\_S4\_Shfl |
| Format8\*:96b | F8\_S0\_LdSt | F8\_S1\_LdPk | F8\_S2\_MulM | F8\_S3\_ALU | F8\_S4\_Wac c |
|  | **Slot 0** | **Slot 1** | **Slot 2** | **Slot 3** | **Slot 4** |
| Format9:48b | F9\_S0\_LdStALU | F9\_S1\_None | F9\_S2\_None | F9\_S3\_ALU | - |
| Format10:48b | F10\_S0\_LdStALU | F10\_S1\_None | F10\_S2\_Mul | - | - |
| Format11:48b | F11\_S0\_LdStALU | F11\_S1\_LdPk | - | - | - |
| Format12:96b | F12\_S0\_St | F12\_S1\_Ld | F12\_S2\_Mul | F12\_S3\_ALUFIRFFT | F12\_S4\_Mo ve |
| Format13:96b | F13\_S0\_LdSt | F13\_S1\_Ld | F13\_S2\_FPMul | F13\_S3\_FPALU | - |
| Format14:96b | F14\_S0\_St | F14\_S1\_Ld | F14\_S2\_FPMul | F14\_S3\_FPAddSub | - |

## 2.4 ConnX BBE32EP DSP Instruction Set Overview

The ConnX BBE32EP DSP is built around the baseline Xtensa RISC architecture which implements a rich set of generic instructions optimized for efficient embedded processing. The power of the ConnX BBE32EP DSP comes from a comprehensive set of over 500 DSP and baseband optimized operations excluding the baseline Xtensa RISC operations. A variety of load/store operations support five basic and two special addressing modes for 16/32-bit scalar and 16-bit narrow vector data-types; see *Load & Store Operations* on page

139. A special addressing mode for circular addressing is also available. Additionally, the ConnX BBE32EP DSP supports aligning load/store operations to deliver high bandwidth loads and stores for unaligned data.

Vector data management in the ConnX BBE32EP DSP is supported through operations designed for element-level data selection, shuffle or shift. Further, to easily manage precision in vector data there are packing operations specific to each data-type supported. Additionally, there is an enhanced ISA support for predicated vector operations. Vector level predication allows a vectorizing compiler to exploit deeper levels of inherent parallelism in a program; see *Predicated Vector Operations* on page 135.

Multiply operations supported by the ConnX BBE32EP DSP include real and complex 16bx16b multiply, multiply-round and multiply-add operations. Multiply operations for complex data provide support for conjugate arithmetic, full-precision arithmetic, magnitude computation with saturated/rounded outputs. The ConnX BBE32EP DSP is capable of eight complex multiplies per cycle where each complex product involves four real multiplies. The architecture supports extended precision with guard bits on all 40-bit wide vector register data, full support for double precision data and 40-bit accumulation on all MAC operations without any performance penalty. A wide variety of arithmetic, logical, and shift operations are supported for up to sixteen 40-bit data-words per cycle. The architecture provides special operations that assist matrix-multiply operations.

For algorithm and application specific acceleration, the ConnX BBE32EP DSP can be configured with a number of options:

* FFT
* Symmetrical FIR
* Packed complex matrix multiply
* LFSR & convolutional encoding
* Linear block decoder
* 1D Despreader
* Soft-bit demapping
* Vector divide
* Fast reciprocal & reciprocal square root
* Advanced precision reciprocal & reciprocal square root
* Advanced precision multiply/add (see note below)
* Inverse log-likelihood ratio (LLR)
* Single and dual peak search
* Single-precision vector floating-point (see note below)

![](data:image/png;base64...) **Restriction:** The following pair of options are mutually exclusive:

* Advanced precision multiply/add
* Single-precision vector floating-point

As an example, by configuring a ConnX BBE32EP DSP with the symmetric FIR option and using pairwise real multiply operations, the configured core offers very high performance for a wide range of FIR kernels. The performance, in terms of MACs/cycle, on a ConnX BBE32EP DSP configured for FIR operations is highlighted in the table below.

### Table 2: ConnX BBE32EP DSP Multiply Performance

|  |  |  |  |  |
| --- | --- | --- | --- | --- |
| **Data** |  | **Coefficients** | **Type** | **MACs/cycle** |
| Complex | Real |  | Symmetric | 64 |
| Complex | Real |  | Asymmetric | 32 |
| Real | Real |  | Symmetric | 64 |
| Real | Real |  | Asymmetric | 32 |

The ConnX BBE32EP DSP instruction set is described in further detail later in *ConnX BBE32EP DSP Features* on page 21.

## 2.5 Programming Model and XCC Vectorization

The ConnX BBE32EP DSP supports a number of programming models -- including standard C/C++, the ConnX BBE32EP DSP-specific integer and fixed-point data types with operator overloads and a level of automated vectorization, scalar intrinsics and vector intrinsics.

The ConnX BBE32EP DSP contains integer and fixed-point data types that can be used explicitly by a programmer to write code. These data types can be used with built-in C/C++ operators or with protos (also called intrinsics) as described later in *Programming a ConnX BBE32EP DSP* on page 49.

Vectorization, which can be manual or automatic, analyzes an application program for possible vector parallelism and restructures it to run efficiently on a given ConnX BBE32EP DSP configuration. Manual vectorization using the ConnX BBE32EP DSP data types and protos is discussed later in *Programming a ConnX BBE32EP DSP* on page 49. The Xtensa

C and C++ compiler (XCC) contains a feature to perform automatic vectorization on many ConnX BBE32EP DSP supported data types. The compiler analyzes and vectorizes a program with little or no user intervention. It generates code for loops by using the ConnX BBE32EP DSP operations. It also provides compiler flags and pragmas for users to guide this process. This feature and its related flags and pragmas are documented in the Xtensa C and C++ Compiler User’s Guide.

The current generation ConnX cores introduce a new N-way programming model for vector processing using data-types in memory and registers. The N-way model consists of Nelement data groups to facilitate portable vector programming. N-way refers to the natural SIMD size of a ConnX machine. The ConnX BBE32EP DSP supports N=16. A ConnX BBE32EP DSP FLIX instruction can bundle up to five SIMD operations in parallel and each operation is capable of producing up to ‘N’ results in an independent FLIX lane. Programmers can adopt the N-way abstraction model by using Ctypes, operations and protos in their code that are either N-way or ‘N/2’-way (denoted by ‘N\_2’ in Ctypes, operation and proto names). N-way model on the ConnX BBE32EP DSP supports N and N\_2 as an abstract representation of 16 and 8 respectively. This makes code written using the N-way model easy to port to other architectures with a different SIMD size or example, other SIMD variants in the BBE EP Cores family -BBE64EP. Although not recommended, users can also use equivalent ctypes and protos whose names explicitly have ‘16’ or ‘8’ in place of ‘N’ or ‘N\_2’ respectively.

For automatic vectorization in an N-way environment, however, it is required that all vector types inside a loop have the same SIMD width. In the ConnX BBE32EP DSP, this exposes an important distinction between real and complex vector types. While the real vector types are supported by a SIMD width of N by the core, the complex vector types are supported by two natural SIMD widths – N (16) and ‘N\_2’ (8). For instance, xb\_vecN\_2xc16 is an 8-way complex vector type stored in a single register while xb\_vecNxc16 is a 16-way complex vector type stored in a pair of registers. It is recommended to use the xb\_vecN\_2xc16 type when programming only complex vector types, but when programming real and complex vector types together, the xb\_vecNxc16 vector type is suggested for complex data.

# 3. ConnX BBE32EP DSP Features

**Topics:**

•

*ConnX BBE32EP DSP*

*Register Files*

•

*ConnX BBE32EP DSP*

*Architecture Behavior*

•

*Operation Naming*

*Conventions*

•

*Fixed Point V*

*alues and*

*Fixed Point Arithmetic*

•

*Data Types Mapped to*

*the Vector Register File*

•

*Data Typing*

•

*Multiplication Operation*

•

*Vector Select Operations*

•

*Vector Shuffle Operations*

•

*Block Floating Point*

•

*Complex Conjugate*

*Operations*

•

*FLIX Slots and Formats*

## 3.1 ConnX BBE32EP DSP Register Files

The ConnX BBE32EP DSP has a partitioned set of register files to provide high bandwidth with less register bloat. Larger register files permit deeper software pipelining and reduced memory traffic. The first partition consists of a set of sixteen 256-bit general purpose narrow vector registers (vec) that can hold operands and results of SIMD operations. Each vec register can hold either sixteen 16-bit real or eight 32-bit complex elements, depending on how the register is used by the software. The second partition consists of a set of four 640-bit wide vector registers (wvec) each of which can hold sixteen 40-bit elements. The wvec registers can hold either 32-bit elements with eight guard bits or 40-bit elements. The interface to each data memory is 256-bits wide and this makes all loads/stores for wide wvec registers take place through intermediate moves into the narrow 256-bit vec registers.

The ConnX BBE32EP DSP register file organization also has four 256-bit Alignment registers (4x16N - where N = SIMD width; for ConnX BBE32EP DSP, N = 16) and eight 112-bit specialized variable Shift/Select registers (8x7N) for use with the select category of operations that can manipulate the contents of the vec register file. There are also eight 16bit Boolean registers (16xN) for flexible SIMD and VLIW predication.

Lastly, an optional register file - a two entry mvec - is added when the *Advanced Precision Multiply/Add* (advprec) option is configured in a ConnX BBE32EP DSP core. The 16-way 32bit mvec registers are used to hold results of only those operations belonging to the advprec option. Furthermore, floating-point operands (23-bit element vectors) for advanced precision operations are held as vectors of 7-bit exponents in vsa registers paired with 16-bit mantissa in narrow vec registers.

![](data:image/png;base64...)

### Figure 2: ConnX BBE32EP DSP Register Files

![](data:image/jpeg;base64...)

### Figure 3: Base Xtensa ISA Register Files

On configuring a ConnX BBE32EP DSP core with certain options, additional state registers are built into the processor core. These special state registers can also be used to hold contents of the vector registers temporarily without going to and from memory. The RUR\_<state\_name> and WUR\_<state\_name> operations are used to read from and write to these state registers. Once initialized, users are advised against using these states in order

not to inadvertently overwrite the contents of these states. Following is a list of fixed and configuration dependent states in ConnX BBE32EP DSP

* BBE\_STATE<A|B|C|D> - 256-bit states added by the *FFT* or *symmetric FIR* options. And, they are shared when both the options are present in a ConnX BBE32EP DSP configuration.
* BBE\_RANGE - A 4-bit state added by the *FFT* option.
* BBE\_MODE - A 5-bit state added by the *FFT* option.
* BBE\_BMUL\_STATE - A 1024-bit state added by the *LFSR & Convolutional Encoding* option.
* BBE\_BMUL\_ACC - A 32-bit state added by the *LFSR & Convolutional Encoding* option.
* BBE\_PQUO<0|1> & BBE\_PREM<0|1> - 128-bit states added by the *Vector Divide* option.
* BBE\_FLUSH\_TO\_ZERO - A 1-bit state added by the *Advanced Precision Multiply/Add* option.
* CBEGIN & CEND - These two states are present in the base ConnX BBE32EP DSP core to support circular addressing; they are used to initialize begin and end addresses in a circular buffer. More details about circular addressing are provided in *Load & Store Operations* on page 139.

More details on the configurable options for ConnX BBE32EP DSP can be found in *Configurable Options* on page 71 and in the ISA HTML of the corresponding operations.

Since the ConnX BBE32EP DSP supports 256-bit loads and stores between registers and memory, at 800MHz, this provides over 50GBps of combined data memory bandwidth between ConnX BBE32EP DSP and the two load/store units.

For data transfers from memory to registers during Loads:

* 16-bit elements in memory are loaded as 16-bit elements in narrow vec registers
* 32-bit elements in memory are loaded to narrow vec registers first and then expanded to 40-bits in wvec wide registers after a move
* Signed loads sign-extend the elements while unsigned loads zero-fill inside vector registers

On the other hand, for data transfers from registers to memory during Stores:

* 16-bit elements in registers are stored as 16-bit elements in memory
* 40-bit elements in registers are saturated to 32-bit elements first and then moved to the narrow vec registers for a store
* Signed stores are saturated to signed 16/32-bit MAX and MIN, while unsigned stores are saturated to unsigned 16/32-bit MAX.

The ConnX BBE32EP DSP provides protos, also called intrinsics, to store and restore register spills. The names of protos that store data element(s) to memory on register spills have a ‘\_storei’ suffix, for example xb\_vecN\_2xc40\_storei, xb\_vecNxcq9\_30\_storei, xb\_c16\_storei, etc. These protos use either appropriate store operations in the case of 16-bit data spills from narrow vec registers or use an appropriate combination of move and store operations in the case of 40-bit data spills from wide wvec registers or other special registers. The 40-bit data element(s) in the wvec registers are first zero-extended to 64-bit data element(s) before a move to the narrow vec register and then followed by four 16-bit stores, per data element, from the narrow vec register to memory.

Conversely, the ConnX BBE32EP DSP has complementary protos to restore data elements from memory back into corresponding registers. The names of these protos have a ‘\_loadi’ suffix, for example, uint32\_loadi, xb\_vecN\_2xcq19\_20\_loadi, xb\_vecNx32U\_loadi, etc.. To restore spills back into the narrow vec register, the restoring spill protos use appropriate load operations, for the 16-bit data-types. However, to restore spills back into the wide wvec register, the restoring spill protos use an appropriate combination of load and move operations, for the 40-bit data-types. In the latter case, the restoring process is through four loads of narrow vectors. Once loaded, these are concatenated, and for each element, 40-bits out of 64-bits are written into the wide wvec vector registers. It can be noticed that the previously zero-extended 40-bit elements are now truncated back from 64-bits prior to move from the narrow vec register to the wide wvec register. Note that In general, these special protos are meant to be used by the compiler, and not by regular programmers, who in general should not need them.

## 3.2 ConnX BBE32EP DSP Architecture Behavior

The ConnX BBE32EP DSP architecture provides guard bits in its data path and wide register file to avoid overflow on ALU and MAC operations. The typical data flow on this machine is:

1. Load data into a narrow unguarded vector register file.
2. Compute and accumulate into the wide vector register file with guard bits.
3. Store data in narrow format by packing down with truncation or saturation (via the vec register file).

The ConnX BBE32EP DSP has thirty-two 16bx16b multipliers, each of which produces a 32bit result to be stored in the guarded 40-bit register elements. This allows up to eight guard bits for accumulation in the 40-bit register elements. For real multiplication, only 16 multipliers are used while for complex multiplication, all 32 are used. For pairwise multiply all 32 are used even for real multiplies

The first load/store unit supports all load and store operations present in the ConnX BBE32EP DSP ISA and the baseline Xtensa RISC ISA. The second load unit supports a limited set of commonly used vector load and aligning vector load operations present in the ConnX BBE32EP DSP ISA. This second load unit does not support any store operations.

Additionally, the ConnX BBE32EP DSP offers operations that pack results. For instance, the ‘PACK’ category of operations extract 16-bits from 40-bit wide data in wvec register elements and saturate results to the range [ -215 .. 215-1]. Most ‘MOVE’ operations that move 16-bit or 32-bit results from high-precision to low-precision data widths saturate.

Unlike some architectures, the ConnX BBE32EP DSP does not set flags or take exceptions when operations overflow their 40-bit range, or if saturation or truncation occurs on packing down from a 40-bit wide vector to a 16-bit vector in registers.

## 3.3 Operation Naming Conventions

The ConnX BBE32EP DSP uses certain naming conventions for greater consistency and predictability in determining names of Ctypes, operations and protos. In general, ConnX BBE32EP DSP operation names follow the pattern below:

<prefix>**\_**<op\_class>[<num\_SIMD>**[X**<element\_width>]][<other\_identifiers>] where,

* prefix := BBE\_
* op\_class := one or more letters identifying a simple or compound class of operation(s)
* num\_SIMD := SIMD width of the core, here 'N' (=16) and 'N\_2' (=8)
* element\_width := bits in each SIMD element of an operand, here 8/16/32/40 bits
* other\_identifiers := one or more letters specifying a sub-class of operation(s) in the context of the op\_class
* C := Complex or circular
* R := Real
* U := Unsigned
* S := Signed or saturation
* I := Immediate
* X := Indexed
* P := Post-increment or pairwise
* T := True (predicate)
* F := False (predicate)
* J := Conjugate
* H := High
* L := Low
* B := Boolean
* U := Unsigned
* A := Address register ar
* V := Narrow vector register vec
* W := Wide vector register wvec
* BR := Boolean register br
* BV := Boolean vector register vbool
* VS := Vector shift/select register vsa
* SF := Spread Factor
* CS := Code sets
* INT := Integer
* ALIGN := Vector alignment register valign

Following are a few specific examples to illustrate the naming conventions listed above.

|  |  |
| --- | --- |
| BBE\_LVNX16\_I | **L**oads a **v**ector of sixteen **16**-bit signed elements from memory into a narrow vector register. The base address used for the load is contained in address register ars. This base address is added to an offset. The \_I extension represents an **i**mmediate offset such that the memory address is a multiple of 32 bytes. |
| BBE\_MOVVA16C | This operation performs a single replicating **mov**e of a 32-bit **c**omplex element, consisting of two 16-bit data elements, from the address register **A**R to a narrow vector register **V**EC. The destination register, source register, size and nature (complex) of the data are identified by V, A, 16 and C respectively. |
| BBE\_LSNX16\_IP | This operation performs a 1-way signed **s**calar **l**oad of a **16**-bit element from memory into a narrow vector register. The rest of the output narrow vector register is zero filled. An address register (AR) holds the base address used for the load and this base address is updated using an immediate as an offset after the load is done. The \_IP extension indicates a **p**ost-operation update for the address register after the sum of base address and the **i**mmediate offset. |
| BBE\_MULNX16PACKL | 16-way signed real **mul**tiply of two narrow **16**-bit vectors to produce a 256-bit combined narrow vector product. The PACKL variant of operations **pack** the sixteen full-precision 32-bit results, which would otherwise be stored in a 16x40-bit wide vector register, back into sixteen 16-bit integer results stored in a 256-bit narrow vector register. PACKL grabs the **l**ow order bits of the results — thus a presumed integer and truncates the high order bits of the result without using a vector shift/select (vsa) register. This kind |
|  | of pack extracts the lowest 16-bits of each of the sixteen intermediate result elements and writes them out without shifting, rounding, or saturation. |

The following table broadly categorizes a set of commonly used ConnX BBE32EP DSP operations. It also provides a brief description to show the specific naming conventions used in each of the categories.

### Table 3: Sample Categories of Operations

|  |  |  |  |  |
| --- | --- | --- | --- | --- |
| **Category** | **Mnemonic** |  | **Type** | **Description** |
| **L**OAD | BBE\_L | V |  | Load vector of 16b elements |
|  | P |  | Load complex pair of 16b elements |
|  | S |  | Load scalar 16b element |
|  | A |  | Load unaligned vector of 16 elements |
|  | B |  | Load vector of 1b elements |
| **MOV**E | BBE\_MOV |  |  | Move between core, vector and state registers.  Predicated versions available for some. |
|  | A |  | Move to AR register |
|  | BR |  | Move to BR (core boolean register) |
|  | BV |  | Move to vbool (vector boolean register) |
|  | IDX |  | Indexed move to vector register. |
|  | PA |  | Move from AR to vector register as fractional  Q5.10 with saturation and replication |
|  | QA |  | Move from AR to vector register as fractional  Q15 with saturation and replication |
|  | PINT |  | Move immediate to vector register as fractional  Q5.10 with saturation, replication |
|  | QINT |  | Move immediate to vector register as fractional  Q15 with saturation, replication |
|  | QUO |  | Move quotients from input vector register to vector-divide quotient state-register |
|  | REM |  | Move remainders from input vector register to vector-divide remainder state-register |
|  | S |  | Move to state-register |

|  |  |  |  |
| --- | --- | --- | --- |
| **Category** | **Mnemonic** | **Type** | **Description** |
|  |  | SV | Move to vec (narrow vector register) with saturation |
|  | SW | Move to wvec (wide vector register) with signextension |
|  | V | Move to vec (narrow vector register) |
|  | VS | Move to vector shift/select register (vsa) |
|  | W | Move to wvec (wide vector register) |
| BBE\_MALIGN |  | Move alignment register |
| BBE\_MB |  | Move between two vbool (vector boolean) registers |
| **MUL**TIPLY | BBE\_MUL |  | Multiply operation. Predicated versions available for some. |
|  | A | Multiply-accumulate operation |
|  | C | Multiply complex, high precision |
|  | J | Multiply complex conjugate, high precision |
|  | PACKQ | Multiply signed real, Q15 fractional results with high-order 16-bits, with saturation |
|  | PACKP | Multiply signed real, results converted to Q5.10 fractional form, with saturation |
|  | PACKL | Multiply signed real, integer results with lowerorder 16-bits |
|  | CPACKQ, CPACKP  & CPACKL | Similar to PACKQ, PACKP & PACKL respectively, but with complex signed operands |
|  | JCPACKQ,  JCPACKP &  JCPACKL | Similar to PACKQ, PACKP & PACKL respectively, but with complex and complexconjugate signed operands |
|  | PR | Multiply with pairwise sum and accumulation |
|  | R | Multiply with variable round producing wide (40bit) results with sign-extension |
|  | SGN | Multiply multiplicand by the sign of the multiplier element-wise |

|  |  |  |  |
| --- | --- | --- | --- |
| **Category** | **Mnemonic** | **Type** | **Description** |
|  |  | S | Multiply producing wide (40-bit) results with sign-extension |
|  | UU | Multiply (unsigned\*unsigned) producing wide  (40-bit) results with sign-extension |
|  | US | Multiply (unsigned(first input)\*signed(second input)) producing wide (40-bit) results with signextension |
| **PACK** | BBE\_PACK | L | Packs 40-bit signed real vector to 16-bit elements with truncation to keep lower bits |
|  | P | Packs 40-bit Q19.20 signed fractional vector to  16-bit Q5.10 elements |
|  | S | Packs low-precision integer vector |
|  | Q | Packs vector of Q9.30 in wvec into Q15 results in vec |
|  | V | Packs 40-bit wvec to low-precision vec based on signed shift amount in vector shift/select register (vsa) |
| BBE\_UNPK | P, Q, S, U | 16-way unpack of 16-bit data in vec to 40-bit data in wvec |
| **SEL**ECT | BBE\_SEL |  | Select sixteen 16-bit elements from two input vectors using vector shift/select register (vsa) |
|  | I | Select sixteen 16/40-bit vector from two input vectors using an immediate value |
|  | PR | Real element-wise right shift with shift count using an immediate value |
|  | PC | Complex element-wise right shift with shift count using an immediate value |
| BBE\_SELS |  | Single real element select from vec/wvec register into element-0 of vec/wvec register |
|  | C | Single complex element select from vec/wvec register into complex element-0 of vec/wvec register |

|  |  |  |  |  |
| --- | --- | --- | --- | --- |
| **Category** | **Mnemonic** |  | **Type** | **Description** |
|  | BBE\_DSEL | I |  | Interleave or de-interleave real/complex elements from two input narrow vectors into two output narrow vector using an immediate value to specify a select pattern |
| **SH**UF**FL**E | BBE\_SHFL |  |  | Shuffle 16-element narrow (16-bit) vector using vector selection register |
|  | I |  | Shuffle 16-element 16/40-bit vector using an immediate value to specify a shuffle pattern |
|  | VS |  | Shuffle elements from a vector shift/select (vsa) register to an output vsa register using an immediate value to specify a shuffle pattern |
| **S**TORE | BBE\_S | V |  | Store vector of 16b elements |
|  | P |  | Store pair (complex) of 16b elements |
|  | S |  | Store scalar 16b element |
|  | A |  | Store and align vector of 16b elements |
|  | B |  | Store vector of 1b Boolean elements |
| **ABS**OLUTE | BBE\_ABS |  |  | Absolute value |
| **ADD** | BBE\_ADD |  |  | Vector add |
| **AND** | BBE\_AND |  |  | Vector bitwise Boolean AND |
| **CONJ**UGATE | BBE\_CONJ |  |  | Complex Conjugate |
| **DIV**ISION  (optional) | BBE\_DIV | U |  | Unsigned vector divide |
|  | S |  | Signed vector divide |
| **S**OFT-BIT **D**E**MAP**  (optional) | BBE\_SDMAP |  |  | 3GPP and IEEE constellation soft-bit demap |
| **EQ**UALITY | BBE\_EQ |  |  | Vector equality check |
| BBE\_NEQ |  |  | Vector inequality check |
| **EXTR**ACT | BBE\_EXTR |  |  | Extract one real/complex element into AR  (address register) |
|  | B |  | Extract one real/complex element into BR (core  Boolean register) |

|  |  |  |  |
| --- | --- | --- | --- |
| **Category** | **Mnemonic** | **Type** | **Description** |
|  | BBE\_EXTRACTB |  | Extract elements of a single vbool register into two vbool registers |
| **FFT** (optional) | BBE\_FFT |  | FFT type operations |
| **F**LOATING-**P**OINT  **RECIP**ROCAL  (optional) | BBE\_FP | RECIP | Signed 16-bit mantissa + 7-bit exponent pseudo-floating point reciprocal approximation |
| **F**LOATING-**P**OINT  **R**ECIPROCAL  **SQ**UARE-**R**OO**T**  (optional) | BBE\_FP | RSQRT | 16-bit mantissa + 7-bit exponent pseudo-floating point reciprocal square-root approximation |
| **NEG**ATE | BBE\_NEG |  | Signed negate |
|  | S | Signed saturating negate |
| **I**N**T**ER**L**EA**V**E | BBE\_ITLV |  | Bit-by-bit interleave of two narrow vectors into one narrow vector |
| **JOIN** | BBE\_JOIN |  | Join boolean vectors |
| **MAG**NITUDE | BBE\_MAGI |  | Interleaved magnitude of complex vectors high precision |
|  | PACKQ | Interleaved magnitude of complex fractional vectors, Q15 results with the high-order 16-bits |
|  | PACKL | Interleaved magnitude of complex integer vectors, integer results with the lower-order 16bits |
|  | PACKP | Interleaved magnitude of complex integer vectors, results packed to 16-bits in Q5.10 format after shifting |
| BBE\_MAGIA |  | Interleaved magnitude of complex integer vectors, result multiply-accumulated |
| BBE\_MAGIR |  | Interleaved magnitude of complex integer vectors, variable rounded results |
| **MAX** | BBE\_MAX |  | Max of vector |
| BBE\_MAXU |  | Max of unsigned vector |
| BBE\_BMAX |  | Max of vector generating Boolean mask |

|  |  |  |  |  |
| --- | --- | --- | --- | --- |
| **Category** | **Mnemonic** | **Type** | | **Description** |
| **MIN** | BBE\_MIN |  | | Min of vector |
| BBE\_BMIN |  | | Min of vector generating Boolean mask |
| **NAND** | BBE\_NAND |  | | NAND of vec/wvec vectors |
| **NSA** | BBE\_NSA |  | | Normalize shift amount |
|  | E | | Normalise shift amount truncated to even value |
|  | C | | Complex normalise shift amount |
|  | U | | Unsigned normalise shift amount |
| **OR** | BBE\_OR |  | | OR of vec/wvec/vbool vectors |
| **POLY**NOMIAL | BBE\_POLY |  | | Polynomial evaluation |
| **RECIP**ROCAL  (optional) | BBE\_RECIP |  | | 16-bit vector reciprocal approximation |
| **R**ECIPROCAL  **SQ**UARE-**R**OO**T**  (optional) | BBE\_RSQRT |  | | Compute normalization and table lookup factors for advanced reciprocal square root |
| **R**EDUCTION | BBE\_R | ADD | | Vector sum reduction |
| BBE\_R | MAX, MIN | | Vector signed reduction maximum/minimum |
| BBE\_RB | MAX, MIN | | Vector signed reduction maximum/minimum along with boolean vector indicating location of maximum/minimum |
| **REP**LICATE | BBE\_REP |  | | Replicate elements |
| **R**OU**ND** | BBE\_RND | ADJ | | Rounding add, using variable round amounts from vector shift/select register (vsa) |
|  | SADJ | | Symmetric rounding add, using variable round amounts from vector shift/select register (vsa) |
| **SAT**URATE | BBE\_SAT | S | | Saturate signed vector |
|  | U | | Saturate unsigned vector |
| **SEQ**UENCE | BBE\_SEQ |  | | Create sequence of integer values from 0 to  (32-1) in output vec/wvec register |
| **Category** | **Mnemonic** |  | **Type** | **Description** |
| **S**HIFT **L**EFT/  **R**IGHT | BBE\_SLL |  |  | Logical left shift, amount of signed shift as input from vsa register |
|  | I |  | Logical left shift, amount of unsigned shift as immediate input |
| BBE\_SLS |  |  | Saturating shift, amount of signed shift as input from vsa register |
| I |  | Saturating shift, amount of unsigned shift as immediate input |
| BBE\_SLA |  |  | Arithmetic left shift, amount of signed shift as input from vsa register |
| BBE\_SRA |  |  | Arithmetic right shift, amount of signed shift as input from vsa register |
| I |  | Arithmetic right shift, amount of unsigned shift as immediate input |

Within each class, sub-conventions are used to describe types of operations, data type layouts, signed/unsigned, data formatting, addressing modes, etc., depending on the operation or its class. *Table 4: Types of Load/Store Operations* on page 34 has an abbreviated list of this information; for detailed information about the operations, see the HTML Instruction Set Architecture page. The easiest method to access this page is from the configuration overview in Xtensa Xplorer:

1. Double-click on the ConnX BBE32EP DSP configuration in the System Overview to open the Configuration Summary window.
2. Click View Details button in the rightmost column for the installed build to open a Configuration Overview window.
3. Select All Instructions to open a complete list of instruction descriptions.

### Table 4: Types of Load/Store Operations

|  |  |  |  |
| --- | --- | --- | --- |
| **Type** | **Operation Prefix** | **Proto Support for Ctypes** | **Addressing** |
| **L**oad **V**ector | BBE\_LV | NX{16, 16U, Q15, Q5\_10} | N\_2XC{16, Q15, Q5\_10} | (I|IP|X|XP|  IC) |
| **L**oad **A**ligning  Vector | BBE\_LA | NX{16, 16U, Q15, Q5\_10} | N\_2XC{16, Q15, Q5\_10} | (IP|IC) |
| **L**oad **A**ligning  **V**ariable | BBE\_LAV | NX{16, 16U, Q15, Q5\_10} | N\_2XC{16, Q15, Q5\_10} | (XP) |
| **Type** | **Operation Prefix** | **Proto Support for Ctypes** | **Addressing** |
| **L**oad **S**calar | BBE\_LS | NX{16, 16U, Q15, Q5\_10} | N\_2XC{16, Q15, Q5\_10} | (I|IP|X|XP) |
| **L**oad **P**air | BBE\_LP | NX{16, 16U, Q15, Q5\_10} | N\_2XC{16, Q15, Q5\_10} | (I|IP|X|XP) |
| **L**oad **B**oolean | BBE\_LB | {N, N\_2} | (I|IP) |
| **S**tore **V**ector | BBE\_SV | NX{16, 16U, Q15, Q5\_10} | N\_2XC{16, Q15, Q5\_10} | (I|IP|X|XP|  IC) |
| **S**tore **A**ligning | BBE\_SA | NX{16, 16U, Q15, Q5\_10} | N\_2XC{16, Q15, Q5\_10} | (IP|IC) |
| **S**tore **A**ligning Variable | BBE\_SAV | NX{16, 16U, Q15, Q5\_10} | N\_2XC{16, Q15, Q5\_10} | (XP) |
| **S**tore **A**ligning  **V**ariable - FFT  **R**ange Update | BBE\_SAVR | NX{16, 16U, Q15, Q5\_10} | N\_2XC{16, Q15, Q5\_10} | (XP) |
| **S**tore **S**calar | BBE\_SS | NX{16, 16U, Q15, Q5\_10} | N\_2XC{16, Q15, Q5\_10} | (I|IP|X|XP) |
| **S**tore **P**air | BBE\_SP | NX{16, 16U, Q15, Q5\_10} | N\_2XC{16, Q15, Q5\_10} | (I|IP|X|XP) |
| **S**tore **B**oolean | BBE\_SB | {N, N\_2} | (I|IP) |

### Assembly and C Naming Conventions

In the ConnX BBE32EP DSP, all the operation and proto names (excluding the baseline Xtensa RISC ISA) begin with the prefix "BBE\_". The full names are then constructed using a number of logical fields separated by underscores ("\_"). Syntactically, operation names having either underscores ("\_") or periods (“.”) may be used in an assembly-level program. However, the C language does not allow the use of periods (".") in the names of operation identifiers. Therefore, in referring to protos for base Xtensa operations in C, periods in names are substituted by underscores. This document uses the assembly-correct names for protos i.e. with underscores after a prefix and periods in the body of a proto name. However, note that all the programming examples in *Programming a ConnX BBE32EP DSP* on page 49 list protos using the C-correct form with only underscores and no periods. Thus, in the online ISA HTML documentation (see *On-Line ISA, Protos and Configuration Information* on page 167), when searching for information about an operation or a proto having period(s) in its name, it is useful to search for a variation of the name with underscore(s) replacing period(s).

![](data:image/png;base64...)**Note:** Only some base Xtensa operations have periods (".") in their names. ConnX BBE32EP DSP operation names do not have any periods.

## 3.4 Fixed Point Values and Fixed Point Arithmetic

The ConnX BBE32EP DSP contains operations for implementing fixed point arithmetic. This section describes the representation and interpretation of fixed point values as well as some operations on fixed point values.

### Representation of Fixed Point Values

A fixed point data type Qm.n contains a sign bit, some number of bits m, to the left of the decimal and some number of bits n, to the right of the decimal. When expressed as a binary value and stored into a register file, the least significant n bits are the fractional part, and the most significant m+1 bits are the integer part expressed as a signed 2s complement number. If the binary value is interpreted as a 2s complement signed integer, converting from the binary value to a fixed point number requires dividing the integer by 2n.

Thus, for example, the 40-bit Q9.30 number 1.5 is represented as 0x00 6000 0000.

|  |  |  |  |
| --- | --- | --- | --- |
| **Bit Range (Field)** | **39 (Sign)** | **38 (Integer) 30** | **29 (Fraction) 0** |
| **Size** | **(1 bit)** | **(9 bits)** | **(30 bits)** |
| Binary | 0 | 0 0000 0001 | 10 0000 0000 0000 0000 0000 0000 0000 |
| Hex | 0x0 | 0x1 | 0x2000 0000 |

and the 16-bit Q15 number -0.5 is represented as 0xc000

|  |  |  |
| --- | --- | --- |
| **Bit Range (Field)** | **15 (Sign)** | **14 (Fraction) 0** |
| **Size** | **(1 bit)** | **(15 bits)** |
| Binary | 1 | 100 0000 0000 0000 |
| Hex | 0x1 | 0x4000 |

When m = 0, we write Qn. When n=0, the data type is just a signed integer and we call the data type a signed m+1-bit integer or intm+1.

The ConnX BBE32EP DSP operations use Q15, Q5.10, Q1.30, Q11.20, Q19.20 and Q9.30 data types, described in more detail, as follows:

* Q15 - 16-bit fixed point data type with 1 sign bit and 15 bits of fraction, to the right of the binary point. The largest positive value 0x7fff is interpreted as (1.0 - 2-15). The smallest negative value 0x8000 is interpreted as (-1.0). The value 0 is interpreted as (0.0)
* Q5.10 - 16-bit fixed point data type with 1 sign bit, 5 integer bits to the left of the binary point and 10 bits of fraction to the right of the binary point.
* Q1.30 - 32-bit fixed point data type with 1 sign bit, 1 integer bit to the left of the binary point and 30 bits of fraction to the right of the binary point.
* Q11.20 - 32-bit fixed point data type with 1 sign bit, 11 integer bits to the left of the binary point and 20 bits of fraction to the right of the binary point.
* Q19.20 - 40-bit fixed point data type with 1 sign bit, 19 integer bits to the left of the binary point and 20 bits of fraction to the right of the binary point.
* Q9.30 - 40-bit fixed point data type with 1 sign bit, 9 integer bits to the left of the binary point and 30 bits of fraction to the right of the binary point.

### Arithmetic with Fixed Point Values

When multiplying fixed point numbers Qm0.n0 \* Qm1.n1, with a standard signed integer multiplier, the natural result of the multiple will be a Qm.n data type where n = n0+n1 and m = m0+m1+1. So multiplying a Q15 by a Q15 generates a Q1.30. Since the ConnX BBE32EP DSP has 16-bit x 16-bit multipliers, it multiplies two Q15 values to produce a Q1.30 result sign extended to Q9.30 in a 40-bit register element. To convert Q1.30 to Q9.30 requires a sign extension that fills a 40-bit register. Similarly, a multiplication between two Q5.10 types produces a Q11.20 result that is sign extended to Q19.20 to fill a 40-bit register.

## 3.5 Data Types Mapped to the Vector Register File

A number of different data types are defined for the vector register files. These data types are also referred to as ctypes after the name of the TIE construct that creates them.

**Scalar Data Types Mapped to the Vector Register File** The signed integer data types are:

* xb\_int16 - A 16-bit signed integer stored in the 16-bit vector register element.
* xb\_int32 - A 32-bit signed integer stored in the least significant 32 bits of a 40-bit vector register element. The upper 8 bits are sign extended from bit 31.
* xb\_int40 - A 40-bit signed integer stored in a vector register element.

The complex integer data types are:

* xb\_c16 - A signed complex integer value with 16-bit imaginary and 16-bit real parts. The real and imaginary pair is stored in two 16-bit elements of a vector register file, with the real part in the lower significant element.
* xb\_c32 - A signed complex integer value with 32-bit imaginary and 32-bit real parts. The real and imaginary pair are stored in two 40-bit elements of a vector register file, with the real part in the less significant element. The values are sign extended from bit 31.
* xb\_c40 - A signed complex integer value with 40-bit imaginary and 40-bit real parts. The real and imaginary pair occupies two 40-bit elements of a vector register file, with the real part in the lower significant element.

In addition to the integer data types, the ConnX BBE32EP DSP also supports a programming model with explicit fixed-point (fractional) data types. The software programming model provides C intrinsics and operator overloading that use these data types. All the scalar ones that fit in a single vector register are listed below.

The six real fixed-point data types are:

* xb\_q15 - A signed Q15 data type stored in a 16-bit vector register element.
* xb\_q5\_10 - A signed Q5.10 data type that occupies a 16-bit vector register element.
* xb\_q1\_30 - A signed Q1.30 data type is stored in the least significant 32-bits of a 40-bit vector register element. The rest of the bits are sign extended from bit 31.
* xb\_q11\_20 - A signed Q11.20 data type is stored in the least significant 32-bits of a 40-bit vector register element. The rest of the bits are sign extended from bit 31.
* xb\_q19\_20 - A signed Q19.20 data type that uses all 40 bits of a vector register element.
* xb\_q9\_30 - A signed Q9.30 data type that uses all 40 bits of a vector register element.

The six complex fixed-point data types are:

* xb\_cq15 - A signed complex fixed-point value with Q15 imaginary and Q15 real parts. The real and imaginary pair is stored in two 16-bit elements of a vector register file, with the real part in the lower significant element.
* xb\_cq5\_10 - A signed complex fixed-point value with Q5.10 imaginary and Q5.10 real parts. The real and imaginary pair occupy two 16-bit elements of a vector register file, with the real part in the lower significant element.
* xb\_cq1\_30 - A signed complex fixed-point value with Q1.30 imaginary and Q1.30 real parts. The real and imaginary pair is stored in two 40-bit elements of a vector register file, with the real part in the lower significant element. The values are sign extended from bit 31.
* xb\_cq11\_20 - A signed complex fixed-point value with Q11.20 imaginary and Q11.20 real parts. The real and imaginary pair occupy two 40-bit elements of a vector register file, with the real part in the lower significant element. The values are sign extended from bit 31.
* xb\_cq19\_20 - A signed complex fixed-point value with Q19.20 imaginary and Q19.20 real parts. The real and imaginary pair occupies two 40-bit elements of a vector register file, with the real part in the lower significant element.
* xb\_cq9\_30 - A signed complex fixed-point value with Q9.30 imaginary and Q9.30 real parts. The real and imaginary pair occupy two 40-bit elements of a vector register file, with the real part in the lower significant element.

### Vector Data Types Mapped to the Vector Register File

Following is a list of the vector register files and their corresponding vector data types. The double vector data types are physically stored in a pair of registers. **Table 5: Vector Data Types Mapped to Vector Register Files**

|  |  |  |  |
| --- | --- | --- | --- |
| **Register Vector** | **Vector 16** | **Vector 40** | **Double Vector 16/40** |
| Integer | xb\_vecNx16 | xb\_vecNx40 | xb\_vecNx16 |
| Q15 | xb\_vecNxq15 | xb\_vecNxq9\_30 | xb\_vecNxcq15 |
| Q5.10 | xb\_vecNxq5\_10 | xb\_vecNxq19\_20 | xb\_vecNxcq5\_10 |
| Complex Integer | xb\_vecN\_2xc16 | xb\_vecN\_2xc40 | xb\_vecNxc40 |
| Complex Q15 | xb\_vecN\_2xcq15 | xb\_vecN\_2xcq9\_30 | xb\_vecNxcq9\_30 |
| Complex Q5.10 | xb\_vecN\_2xcq5\_10 | xb\_vecN\_2xcq19\_20 | xb\_vecNxcq19\_20 |
| **Memory Vector** | **Vector 16** | **Vector 32** | **Double Vector 16/32** |
| Integer | xb\_vecNx16 | xb\_vecNx32 | xb\_vecNx16 |
| Unsigned Integer | xb\_vecNx16U | xb\_vecNx32U |  |
| Q15 | xb\_vecNxq15 | xb\_vecNxq1\_30 | xb\_vecNxcq15 |
| Q5.10 | xb\_vecNxq5\_10 | xb\_vecNxq11\_20 | xb\_vecNxcq5\_10 |
| Complex Integer | xb\_vecN\_2xc16 | xb\_vecN\_2xc32 | xb\_vecNxc32 |
| Complex Q15 | xb\_vecN\_2xcq15 | xb\_vecN\_2xcq1\_30 | xb\_vecNxcq1\_30 |
| Complex Q5.10 | xb\_vecN\_2xcq5\_10 | xb\_vecN\_2xcq11\_20 | xb\_vecNxcq11\_20 |

## 3.6 Data Typing

The following tables (Table 2 4 through Table 2 7) list the complete data type naming convention for both operations and protos. These basic data types are used to understand the naming convention. All operations can be accessed using intrinsics with the same name as the operation using one of the base data types xb\_vecNx for full vector and xb\_vecN\_2x for half-vector (for use with complex types). Alternatively, intrinsics are provided to give the same functionality of each operation mapped appropriately to the set of data types.

The ConnX BBE32EP DSP supports standard C data-types, referred to as memory datatypes, for 16/32-bit scalar and vector data elements in memory. These scalar and vector data-elements are mapped to the ConnX BBE32EP DSP register file as register data-types. While the 16-bit memory types are mapped to 16-bit register types, the 32-bit memory types are mapped to 40-bit register types along with 8 guard bits.

Following are scalar, mem vector, and vector register data type details defined for C.

### Table 6: Scalar Memory Data Types

|  |  |  |
| --- | --- | --- |
| **Scalar** | **Scalar 16** | **Scalar 32** |
| Integer | xb\_int16 | xb\_int32 |
| Q15 | xb\_q15 | xb\_q1 \_30 |
| Q5.10 | xb\_q5 \_10 | xb\_q11 \_20 |
| Complex int | xb\_c16 | xb\_c32 |
| Complex Q15 | xb\_cq15 | xb\_cq1 \_30 |
| Complex Q5.10 | xb\_cq5 \_10 | xb\_cq11 \_20 |

### Table 7: Scalar Register Data Types

|  |  |  |
| --- | --- | --- |
| **Scalar** | **Scalar 16** | **Scalar 40** |
| Integer | xb\_int16 | xb\_int40 |
| Q15 | xb\_q15 | xb\_q9 \_30 |
| Q5.10 | xb\_q5 \_10 | xb\_q19 \_20 |
| Complex int | xb\_c16 | xb\_c40 |
| Complex Q15 | xb\_cq15 | xb\_cq9 \_30 |
| Complex Q5.10 | xb\_cq5 \_10 | xb\_cq19 \_20 |

### Table 8: Vector Memory Data Types

|  |  |  |  |
| --- | --- | --- | --- |
| **Memory Vector** | **Vector 16** | **Vector 32** | **Double Vector 16/32** |
| Integer | xb\_vecNx16 | xb\_vecNx32 | xb\_vecNxc16 |
| Unsigned int | xb\_vecNx16U | xb\_vecNx32U |  |
| Q15 | xb\_vecNxq15 (Q15) | xb\_vecNxq1 \_30 (Q1.30) | xb\_vecNxcq15 (Q15) |
| Q5.10 | xb\_vecNxq5 \_10 (Q5.10) | xb\_vecNxq11 \_20  (Q11.20) | xb\_vecNxcq5 \_10 (Q5.10) |
| Complex int | xb\_vecN \_2xc16 | xb\_vecN \_2xc32 | xb\_vecNxc32 |
| Complex Q15 | xb\_vecN \_2xcq15 (Q15) | xb\_vecN \_2xcq1 \_30  (Q1.30) | xb\_vecNxcq1 \_30 (Q1.30) |
| Complex Q5.10 | xb\_vecN \_2xcq5 \_10  (Q5.10) | xb\_vecN \_2xcq11 \_20  (Q11.20) | xb\_vecNxcq11 \_20  (Q11.20) |

### Table 9: Vector Register Data Types

|  |  |  |  |
| --- | --- | --- | --- |
| **Temp Vector** | **Vector 16** | **Vector 40** | **Double Vector 40** |
| Integer | xb\_vecNx16 | xb\_vecNx40 | xb\_vecNxc16 |
| Q15 | xb\_vecNxq15 (Q15) | xb\_vecNxq9 \_30 (Q9.30) | xb\_vecNxcq15 (Q15) |
| Q5.10 | xb\_vecNxq5 \_10 (Q5.10) | xb\_vecNxq19 \_20  (Q19.20) | xb\_vecNxcq5 \_10 (Q5.10) |
| Complex int | xb\_vecN \_2xc16 | xb\_vecN \_2xc40 | xb\_vecNxc40 |
| **Temp Vector** | **Vector 16** | **Vector 40** | **Double Vector 40** |
| Complex Q15 | xb\_vecN \_2xcq15 (Q15) | xb\_vecN \_2xcq9 \_30  (Q9.30) | xb\_vecNxcq9 \_30 (Q9.30) |
| Complex Q5.10 | xb\_vecN \_2xcq5 \_10  (Q5.10) | xb\_vecN \_2xcq19 \_20  (Q19.20) | xb\_vecNxcq19 \_20  (Q19.20) |

## 3.7 Multiplication Operation

The ConnX BBE32EP DSP supports a variety of multiplication operations that use a set of thirty-two 16bx16b SIMD multipliers and associated adders provided as computational resources. These multiplier enabled operations can be broadly classified as – multiply, multiply-accumulate, multiply and round, multiply-subtract, unsigned multiply and signwise multiply operation.

The table below lists the various flavors of multiplication supported by the ConnX BBE32EP DSP. Each of these operations may have multiple protos each of which support a specific data-type related to the operation. Refer to the ISA HTML of an operation to find all the protos using that operation.

### Table 10: Types of ConnX BBE32EP DSP Multiplication Operations

|  |  |  |
| --- | --- | --- |
| **Type** | **Operation** | **Description** |
| Real signed multiply | BBE\_MULNX16 | 16 16-bit operands;  40-bit sign-extended results |
| Real signed multiply with low-precision results | BBE\_MULNX16PACKL | 16 16-bit operands;  16-bit low-precision integer results |
| BBE\_MULNX16PACKP | 16 16-bit operands;  16-bit Q5.10 fractional results |
| BBE\_MULNX16PACKQ | 16 16-bit operands;  16-bit Q15 fractional results |
| Complex signed multiply | BBE\_MULNX16C | 8 16-bit complex operands;  40-bit sign-extended results |
| Complex signed multiply with low-precision results | BBE\_MULNX16CPACKL | 8 16-bit complex operands;  16-bit low-precision integer results |
| BBE\_MULNX16CPACKP | 8 16-bit complex operands; |

|  |  |  |
| --- | --- | --- |
| **Type** | **Operation** | **Description** |
|  |  | 16-bit Q5.10 fractional results |
| BBE\_MULNX16CPACKQ | 8 16-bit complex operands;  16-bit Q15 fractional results |
| Complex conjugate signed multiply | BBE\_MULNX16J | 8 16-bit complex operands;  40-bit sign-extended results |
| Complex conjugate signed multiply with low-precision results | BBE\_MULNX16JPACKL | 8 16-bit complex operands;  16-bit low-precision integer results |
| BBE\_MULNX16JPACKP | 8 16-bit complex operands;  16-bit Q5.10 fractional results |
| BBE\_MULNX16JPACKQ | 8 16-bit complex operands;  16-bit Q15 fractional results |
| Complex and complex conjugate signed multiply with low-precision results | BBE\_MULNX16JCPACKL | 8 16-bit complex operands;  16-bit low-precision integer results |
| BBE\_MULNX16JCPACKP | 8 16-bit complex operands;  16-bit Q5.10 fractional results |
| BBE\_MULNX16JCPACKQ | 8 16-bit complex operands;  16-bit Q15 fractional results |
| Special complex signed multiply with pairwise reduction add | BBE\_MULNX16PC\_0 | Used to accelerate matrix multiplication.  Refer to ISA HTML. |
| BBE\_MULNX16PC\_1 |
|  |  |  |
| Real signed multiply-accumulate | BBE\_MULANX16 | 16 16-bit operands;  40-bit sign-extended results |
| Complex signed multiply-accumulate | BBE\_MULANX16C | 8 16-bit complex operands;  40-bit sign-extended results |
| Complex conjugate signed multiply-accumulate | BBE\_MULANX16J | 8 16-bit complex operands;  40-bit sign-extended results |

|  |  |  |
| --- | --- | --- |
| **Type** | **Operation** | **Description** |
| Special complex signed multiply  accumulate with pairwise reduction add | BBE\_MULANX16PC\_0 | Used to accelerate matrix multiplication.  Refer to ISA HTML. |
| BBE\_MULANX16PC\_1 |
|  |  |  |
| Real signed multiply and variable round | BBE\_MULRNX16 | 16 16-bit operands;  40-bit sign-extended results |
| Complex signed multiply and variable round | BBE\_MULRNX16C | 8 16-bit complex operands;  40-bit sign-extended results |
| Complex conjugate signed multiply and variable round | BBE\_MULRNX16J | 8 16-bit complex operands;  40-bit sign-extended results |
| Special complex signed multiply  with pairwise reduction add and rounding | BBE\_MULRNX16PC\_0 | Used to accelerate matrix multiplication.  Refer to ISA HTML. |
| BBE\_MULRNX16PC\_1 |
|  |  |  |
| Real signed multiply-subtract | BBE\_MULSNX16 | 16 16-bit operands;  40-bit sign-extended results |
| Complex signed multiply-subtract | BBE\_MULSNX16C | 8 16-bit complex operands;  40-bit sign-extended results |
| Complex conjugate signed multiply-subtract | BBE\_MULSNX16J | 8 16-bit complex operands;  40-bit sign-extended results |
|  |  |  |
| Unsigned with signed multiply | BBE\_MULUSNX16 | 16 16-bit operands with first input unsigned and second input signed;  40-bit sign-extended results |
| Unsigned with signed multiplyaccumulate | BBE\_MULUSANX16 |
| Unsigned with signed multiply with rounding | BBE\_MULUSRNX16 |
| Unsigned with unsigned multiply | BBE\_MULUUNX16 |
| Unsigned with unsigned multiplyaccumulate | BBE\_MULUUANX16 |
| Unsigned with unsigned multiply with rounding | BBE\_MULUURNX16 |
|  |  |  |
| Special signwise multiply | BBE\_MULSGNNX16 | Refer to ISA HTML. |

## 3.8 Vector Select Operations

The vector select operations (BBE\_SEL) allow elements from two source vectors to be selectively copied into a destination vector. With this general definition of selective element transfer, it is easy to implement replication, rotation, shift, extraction and interleaving with the same basic BBE\_SEL type of operation.

To setup a selective transfer, the BBE\_SELNX16 operation takes two 16x16-bit narrow source vectors and one 16x16-bit narrow target vector; along with the vector select register (vsa). The vector select register contains a user defined 16-valued pattern to define the required transfer. The 16 values in the vector select register can be [0,…,31] referring to the indices of the combination of the two source vectors.

On the other hand, the BBE\_SELNX16I operation allows common selection patterns without having to set the vector select register that is required by the BBE\_SELNX16 operation. The BBE\_SELNX16I operation selects sixteen 16-bit elements from a pair of narrow vector registers and produces a single narrow vector output. The nature of selection pattern is specified through an immediate value iSel. To use selection on 40-bit wide vector data-types, the BBE\_SELNX40I operation provides support for a limited set of pre-defined selections. Eight preset selection patterns can be chosen with an appropriate immediate iSel to rotate or interleave wide vector data-types. Refer to the ISA HTML of BBE\_SELNX40I for a list of all the available types and permitted immediate values iSel.

![](data:image/png;base64...)**Note:** Select patterns for immediate operations BBE\_SELNX16I and BBE\_SELNX40I are not necessarily same across range of BBE-EP cores BBE16EP/32EP/64EP. Programmer should study patterns available for perticular core they are using from ISAHTML in order to determine the best one.

### Vector Initialization and Some Additional Select Patterns

The ConnX BBE32EP DSP also contains a special move operation, BBE\_MOVVINX16, which is used for vector initialization based on an immediate value. The table below illustrates what is possible.

### Table 11: Vector Initialization Operation Arguments

|  |  |  |  |
| --- | --- | --- | --- |
| **Symbolic Name** | **Immediate**  **Argument** | **Value Assigned** | **Description** |
| BBE\_MOVVI\_INT16\_M1 | -1 | 32'hFFFF\_FFFF | int16 -1 |
| BBE\_MOVVI\_ZERO | 0 | 32'h0000\_0000 | zero |
| BBE\_MOVVI\_INT16\_1 | 1 | 32'h0001\_0001 | int16 +1 |
| BBE\_MOVVI\_INT16\_MININT BBE\_MOVVI\_Q15\_M1 | 2 | 32'h8000\_8000 | int16 MININT Q15 -1 |
| **Symbolic Name** | **Immediate**  **Argument** | **Value Assigned** | **Description** |
| BBE\_MOVVI\_INT16\_MAXINT | 3 | 32'h7FFF\_7FFF | int16 MAXINT |
| BBE\_MOVVI\_Q5\_10\_1 | 4 | 32'h0400\_0400 | Q5.10 +1 |
| BBE\_MOVVI\_Q5\_10\_M1 | 5 | 32'hFC00\_FC00 | Q5.10 -1 |
| BBE\_MOVVI\_LOWER\_CHAR | 6 | 32'h00FF\_00FF | Lower Char |
| BBE\_MOVVI\_UPPER\_CHAR | 7 | 32'hFF00\_FF00 | Upper Char |
| BBE\_MOVVI\_C16\_1 | 8 | 32'h0000\_0001 | C16 +1 |
| BBE\_MOVVI\_C16\_I | 9 | 32'h0001\_0000 | C16 +i |
| BBE\_MOVVI\_C16\_M1 BBE\_MOVVI\_EVEN\_SELECT | 10 | 32'h0000\_FFFF | C16 -1 even select |
| BBE\_MOVVI\_C16\_MI BBE\_MOVVI\_ODD\_SELECT | 11 | 32'hFFFF\_0000 | C16 -i odd select |
| BBE\_MOVVI\_CQ15\_M1 | 12 | 32'h0000\_8000 | CQ15 -1 |
| BBE\_MOVVI\_CQ15\_MI | 13 | 32'h8000\_0000 | CQ15 -i |
| BBE\_MOVVI\_CQ5\_10\_1 | 14 | 32'h0000\_0400 | CQ5.10 +1 |
| BBE\_MOVVI\_CQ5\_10\_I | 15 | 32'h0400\_0000 | CQ5.10 +i |
| BBE\_MOVVI\_CQ5\_10\_M1 | 16 | 32'h0000\_FC00 | CQ5.10 -1 |
| BBE\_MOVVI\_CQ5\_10\_MI | 17 | 32'hFC00\_0000 | CQ5.10 -i |

## 3.9 Vector Shuffle Operations

The vector shuffle operations (BBE\_SHFL) in the ConnX BBE32EP DSP can shuffle any 16elements from a source vector register into a target vector register. These shuffle operations are often useful in performing vector compression, expansion, reordering and in preparation for matrix multiplication.

For shuffle-based operations, some examples of the support present in the ConnX BBE32EP DSP are the ability to:

* perform a full SELECT/SHUFFLE and a specialized SELECT/SHUFFLE (pattern specified by an immediate value) in a single cycle
* perform one 2x2 interleave per cycle with two vector inputs and two vector outputs
* perform a 3x3 interleave in multiple steps
* support specialized shuffles for implementing FIR filters with two vector inputs and two vector outputs

The BBE\_SHFLNX16 operation shuffles elements in a narrow source vector based on a shuffle pattern specified by the vector selection register vsa. The shuffle pattern is set by storing desired indices [0,…,N-1] in the N values in the vsa register.

Similar to the select operations, the BBE\_SHFLNX16I and the BBE\_SHFLNX40I operations use an immediate iSel to specify a shuffle pattern while not using the vsa register; and the support for wide vector types are a limited set of shuffle patterns. Refer to the ISA HTML for a list of all available patterns and corresponding immediate values iSel.

![](data:image/png;base64...)**Note:** Shuffle patterns for immediate operations BBE\_SHFLNX16I and

BBE\_SHFLNX40I are not necessarily same across range of BBE-EP cores BBE16EP/ 32EP/64EP. Programmer should study patterns available for perticular core they are using from ISAHTML in order to determine the best one.

## 3.10 Block Floating Point

When applications can operate across a wide numerical range, the programmer may wish to implement "block floating point", the adjustment of an entire data set based on determining the actual range of values, normalizing the data-set to maximize the number of bits of precision, and readjusting the range later in the computation. The operations

BBE\_NSANX16, BBE\_NSANX16C, BBE\_NSANX40, BBE\_NSANX40C, BBE\_NSAUNX16

and BBE\_NSAUNX40 facilitate block floating point. These operations calculate the left shift amount required to normalize each element to a 16-bit or 40-bit value. The result is returned in another register, and can be used with the operation BBE\_SLLNX16 and BBE\_SLLNX40 to normalize the elements of a single vector.

The corresponding operations for xb\_vecN\_2x40 data are BBE\_NSANX40 and

BBE\_NSAUNX40. To implement block floating point, BBE\_NSANX16 or BBE\_NSAUNX16 is applied to the entire data set, and the minimum normalization shift is calculated using BBE\_MINNX16. Then the same shift is applied to the entire data set, typically using BBE\_SLLNX16. This shift amount can be used at a later stage of the computation, with BBE\_SRANX16 to return the data to non-normalized form.

## 3.11 Complex Conjugate Operations

A complex conjugate multiply of two complex numbers a and b is the multiplication of a by the complex conjugate of b and is defined as: result = (a.real + j a.imag) \* (b.real - j b.imag) Where b = (b.real + j \* b.imag) and complex conjugate of b = (b.real - j \* b.imag). We use "J" in instruction names to indicate complex conjugate operations. And JC for regular multiply and complext conjugate multiply as a pair.

There are many variants of complex-conjugate multiply operations:

* BBE\_MULNX16J – high-precision complex conjugate multiply
* BBE\_MULNX16JCPACKL – low-precision integer result after complex and complex conjugate multiply
* BBE\_MULNX16JCPACKP – low-precision fractional (Q5.10) result after complex and complex conjugate multiply
* BBE\_MULNX16JCPACKQ – low-precision fractional (Q15) result after complex and complex conjugate multiply
* BBE\_MULNX16JCPACKQ – low-precision fractional (Q15) result after complex and complex conjugate multiply
* BBE\_MULNX16JPACKL - low-precision integer complex conjugate multiply
* BBE\_MULNX16JPACKP - low-precision fractional (Q5.10) complex conjugate multiply
* BBE\_MULNX16JPACKQ - low-precision fractional (Q15) complex conjugate multiply
* BBE\_MULANX16J – high-precision complex conjugate multiply-add
* BBE\_MULRNX16J – high-precision complex conjugate multiply and variable round
* BBE\_MULSNX16J – high-precision complex conjugate multiply-subtract

For example, BBE\_MULNX16J takes as input two 16X16-bit vectors, in which the real and imaginary portions of eight complex numbers are interleaved. It produces eight complex results in one 16X40-bit register where the real and imaginary parts the complex results are interleaved.

## 3.12 FLIX Slots and Formats

The ConnX BBE32EP DSP can issue up to five operations in a single instruction bundle using Xtensa LX FLIX (VLIW) technology. It contains scalar and vector SIMD operations. The ConnX BBE32EP DSP is implemented with a number of 48/96-bit formats in addition to the standard 24-bit and optional 16-bit instruction formats in the Xtensa LX architecture. Each basic 48/96-bit format can bundle five operations into its five separate FLIX slots respectively.

### Instruction List – Showing Slot Assignments

The instruction slot assignment list, showing operations assigned to the various slots of the various formats, is automatically generated and available for use in the on-line configuration documentation for the ConnX BBE32EP DSP. Consult ISA HTML for information about this on-line information. Since this list is automatically generated from the machine description, it is comprehensive and up to date including the user's choice of configuration options.

# 4. Programming a ConnX BBE32EP DSP

**Topics:**

* *Programming in Prototypes*
* *Xtensa Xplorer Display Format Support*
* *Operator Overloading and Vectorization*
* *Programming Styles*
* *Conditional Code*
* *Using the Two Local Data*

*RAMs and Two Load/*

*Store Units*

* *Other Compiler Switches*
* *TI C6x Instrinsics Porting*

*Assistance Library*

Cadence® recommends two important Xtensa manuals to read and become familiar with before attempting to obtain optimal results by programming the ConnX BBE32EP DSP:

* Xtensa® C Application Programmer’s Guide
* Xtensa® C and C++ Compiler User’s Guide

Note that this chapter does not attempt to duplicate material in either of these guides.

The ConnX BBE32EP DSP is based on SIMD (Single

Instruction/Multiple Data) techniques for parallel processing. It is typical for programmers to do some work to fully exploit the available performance. It may only require recognizing that an existing implementation of an application is already in essentially the right form for vectorization, or it may require completely reordering the algorithm’s computations to bring together those that can be done in parallel.

This chapter describes several approaches to programming the ConnX BBE32EP DSP and explores the capabilities of automated instruction inference and vectorization, and cases where the use of intrinsic-based programming is appropriate.

To use the ConnX BBE32EP DSP data types and

intrinsics in C, please include the appropriate top-level header file by using the following preprocessor directive in the source file:

#include <xtensa/tie/xt\_bben.h>

The xt\_bben.h include file is auto-generated through the

ConnX BBE32EP DSP core build process and contains

#defines for programmers to conditionalize code (see *Conditional Code* on page 63). Furthermore, xt\_bben.h includes the lower-level include file (xt\_bbe32.h) which has more core specific ISA information. It is worth noting that the header file – xt\_bben\_verification.h contains additional #defines but used only for ISA verification

purposes; and thus are not documented for programmers’ use.

The ConnX BBE32EP DSP processes both fixed-point and integer data. The basic data element, xb\_vecNx16, is 16-bits wide, and a vector consists of sixteen such 16-bit elements. Thus, an input vector in memory is 256-bits wide. The ConnX BBE32EP DSP also supports xb\_vecNx32 and xb\_vecNx32U, a wider 32-bit data element with eight such elements stored in a 256-bit vector. This double-width data type is typically generated as the result of a multiply or multiply/accumulate operation on 16-bit data elements.

![](data:image/png;base64...)

As described earlier, during a load operation, the ConnX BBE32EP DSP loads 16-bit scalar values to 16-bits and it expands 32-bit scalar values to 40-bits. The ConnX BBE32EP DSP supports both signed and unsigned data types in memory and provides a symmetrical set of load and store operations for both these data types. For signed data, a load results in sign extension from 32-bits to 40bits. For unsigned data, loads result in zero extension.

The ConnX BBE32EP DSP architecture philosophy follows the data flow of loading data at a lower precision into a narrow unguarded vec register file, computing and accumulating into the 40-bit/element guarded wide wvec register file (8-guard bits), and storing results back at a lower precision via narrow vec register file. The ConnX BBE32EP DSP ALU/MAC operations wrap on overflow and either saturate going from 40-bits to 32-bits or PACK going from 32-bits to 16-bits.

**Note:** There are no direct loads into or stores from the wide wvec register file. All wvec loads/stores take place through narrow vec registers.

Automatic type conversion is also supported between vector types and the associated scalar types, for example between xb\_vecNx16 and short and between xb\_vecNx40 and int. Converting from a scalar to a vector type replicates the scalar into each element of the vector. Converting from a vector to a scalar extracts the first element of the vector.

The C compiler also supports the xb\_vecNx40 type, a vector of sixteen, 40-bit elements. This data type is useful because logically the multiplication of two variables of type xb\_vecNx16 returns a variable of type xb\_vecNx40. The xb\_vecNx40 type is implemented using the 640-bit wide register file.

Note that as well as the basic types discussed above; there are a number of other types, such as complex integer, fractional, complex fractional, etc. And as discussed in *ConnX BBE32EP DSP Features* on page 21, there are alternative prototypes (protos) for the ConnX BBE32EP DSP operations that deal with these types. This allows both a more intuitive programming style and richer capabilities for more automated compiler inference and vectorization using these additional data types.

Some examples of the various types are:

* xb\_cq1\_30: Memory scalar, one complex pair of Q1.30 type, 1-bit for integer and 30-bit fractional, sign bit implied, total 64 bits
* xb\_vecN\_2xcq9\_30: Register vector, eight complex elements of Q9.30 type, each 9-bit integer and 30-bit fractional, sign bit implied, total 640 bits
* xb\_vecNx32U: Memory vector, sixteen unsigned real elements of 32-bit integer type, total 512 bits
* xb\_vecN\_2xcq15: Memory vector, eight complex elements of Q15 type, total 256 bits

## 4.1 Programming in Prototypes

As part of its programming model, the ConnX BBE32EP DSP defines a number of operation protos (prototypes or intrinsics) for use by the compiler in code generation, for use by programmers with data types other than the generic operation support, and to provide compatibility with related programming models and DSPs. For programmers, the most important use of protos is to provide alternative operation mappings for various data types. If the data types are compatible, one operation can support several variations without any extra hardware cost. For example, protos allow vector operations to support all types of real and complex integer and fixed-point data types.

The ConnX BBE32EP DSP contains several thousand protos. Some protos are meant for compiler usage only, although advanced programmers may find a use for them in certain algorithms. Many of these protos are called BBE\_OPERATOR, and in general should not be used as manual intrinsics; look for the simpler protos for the function instead. The complete list of protos can be accessed via the ISA HTML.

In the ISA HTML package contained within the ConnX BBE32EP DSP configuration, the proto\_list.html page categorizes all the protos available for programmer use. The following categories of protos are listed in the easy-to-navigate HTML page:

* BBE PRIMARY PROTOS
* COMPILER USE PROTOS
* DATA MANAGEMENT PROTOS
* OPERATOR OVERLOAD PROTOS
* TYPE CONVERSION PROTOS
* TYPE-CASTING PROTOS
* USER REGISTER PROTOS
* ZERO ASSIGNMENT PROTOS

### Extract Protos

Extract protos are used to convert between types in the ConnX BBE32EP DSP programming model. The extract protos are used to produce new vectors from existing vectors. Note that their naming and prototype argument list follows certain conventions. The destination type follows “BBE\_EXTRACT”, which is followed by the source type. The destination argument is first in the argument list, followed by the source argument, and then any special arguments such as immediates.

There are some special destination-type names used that are not normal ConnX BBE32EP DSP types. For example, “R” means real (extract the real parts from a vector of complex variables), and “I” means imaginary (extract the imaginary parts from a vector of complex variables).

The ConnX BBE32EP DSP also provides protos to extract the real and imaginary parts of a complex vector - BBE\_EXTRACTI\_FROMC16 and BBE\_EXTRACTR\_FROMC16.

Example: The following code shows the use of an Extract proto; in this case, to extract the real values from a complex vector.

|  |
| --- |
| // Complex conjugate multiplies xb\_c40 cxprod = ycin[i] \* ~ hcin[i]; xb\_c40 hcxabs = hcin[i] \* ~ hcin[i];  // Extract real magnitude squared for channel xb\_int40 habs = BBE\_EXTRACTR\_FROMC40(hcxabs); |

Note that the ConnX BBE32EP DSP scalar types are used here, as the compiler does automatic vectorization and inference.

### Combine Protos

Combine protos are also used to convert between types in the ConnX BBE32EP DSP programming model. Note that their naming and prototype argument list follows certain conventions. The destination type follows “BBE\_COMBINE”, which is followed by the source type. The destination arguments are first in the argument list, followed by the source arguments, and then any special arguments such as immediates. The “BBE\_COMBINE” protos are used to produce a single integer or fixed-point complex vector from two noncomplex input vectors of same types.

There are some special source type names used that are not normal ConnX BBE32EP DSP types. For example, “I” means imaginary, “R” means real, and “Z” means zero. Thus, “IR” means imaginary-real (combine a vector of imaginary parts and a vector of real parts into a vector of interleaved complex numbers). “ZR” means zero-real — that is, all imaginary parts are zero, thus this will generate a vector of interleaved complex numbers whose imaginary parts are all zero.

Complementary to the BBE\_EXTRACT set of protos, the ConnX BBE32EP DSP provides a set of “BBE\_JOIN” protos that can be used to produce a wide integer or fixed-point complex vector by joining two narrow complex vectors of same type. While the BBE\_JOIN protos require narrow complex vectors as inputs, the BBE\_COMBINE protos combine non-complex vectors into a complex vector.

Example: In the following code, a combine from a vector of real values and zeroes for the imaginaries is used to create two complex vectors.

// N-way inverse for scaling, note divide is integer division // Also convert to complex multiplicand to use in later steps xb\_vecNx16 hinv = BBE\_DIVNX32(numinv, numinv, hsabs); xb\_vecNxc16 hcxhi, hcxlo; BBE\_COMBINENXC16\_FROMZR(hcxhi, hcxlo, hinv);

### Move Protos

The ConnX BBE32EP DSP additionally provides “BBE\_MOV” category of protos used to cast between data-types. For vector inputs, the BBE\_MOV protos cast from one data-type to another related data-type without changing the size of the bit-stream. These protos don’t have a cycle cost and are helpful in multi-type programming environments. While the data is interpreted differently, the length of the vector remains the same within the register file. This category of protos usually follows the format

BBE\_MOV<after\_type>FROM<before\_type>

Examples: BBE\_MOVNX16\_FROMNXQ5\_10, BBE\_MOVNX40\_FROMN\_2XCQ9\_30, etc.

### Operator Overload Protos

The proto HTML documentation also includes the Operator protos for compiler usage. Note that Cadence® recommends usage by advanced programmers only.

When using intrinsics, you may pass variables of different types than expected as long as there is a defined conversion from the variable type to the type expected by the intrinsic.

Operator overloading is only supported if there is an intrinsic with types that exactly match the variables. Implicit conversion is not allowed since with operator overloading you are not specifying the intrinsic name, and the compiler does not guess which intrinsics might match. The resultant intrinsic is necessary for operator overloading, but there is no advantage in calling it directly.

## 4.2 Xtensa Xplorer Display Format Support

Xtensa Xplorer provides support for a wide variety of display formats, which makes use of these varied data types easier, and also easier to debug. These formats allow memory and vector register data contents to be displayed in a variety of formats. In addition, users can define their own display formats. Variables are displayed by default in a format matching their vector data types. Registers are by default always displayed as xb\_vecNx16 types, but you can change the format to any other format.

Some examples of these display formats for a 256-bit variable are: xb\_vecNx16m displays hex and decimal for each real element of vector

(720, -736, 721, -725, 739, 728, -725, -758, 727, 722, 724, -722, -720, -724, 722, 716) =

(0x02d0, 0xfd20, 0x02d1, 0xfd2b, 0x02e3, 0x02d8, 0xfd2b, 0xfd0a, 0x02d7, 0x02d2, 0x02d4,

0xfd2e, 0xfd30, 0xfd2c, 0x02d2, 0x02cc)

xb\_vecN\_2xc16m displays above as eight complex numbers instead

(720i+-736, 721i+-725, 739i+728, -725i+-758, 727i+722, 724i+-722, -720i+-724, 722i+716) =

(0x02d0i+0xfd20, 0x02d1i+0xfd2b, 0x02e3i+0x02d8, 0xfd2bi+0xfd0a, 0x02d7i+0x02d2, 0x02d4i

+0xfd2e, 0xfd30i+0xfd2c, 0x02d2i+0x02cc)

Note that the complex numbers are displayed as they are laid out in memories and registers, since the ordering of each pair is (imaginary, real).

## 4.3 Operator Overloading and Vectorization

Common ConnX BBE32EP DSP operations can be accessed in C or C++ by applying standard C operators to the ConnX BBE32EP DSP data types. There are many operator overloads defined for the various types, and the compiler will infer the correct ConnX BBE32EP DSP vector operation in many cases depending upon the data-types used.

In addition, if the scalar types are used in loops, the compiler will often be able to automatically vectorize and infer or overload. That is, a loop using the ConnX BBE32EP DSP scalar types may turn into a loop of the ConnX BBE32EP DSP vector operations that is as tightly packed and efficient as manual code using ConnX BBE32EP DSP intrinsics.

For operations that do not map to standard operators, intrinsics can be used. Several intrinsics can map to the same underlying operation by means of one or more operations. There are different intrinsics for different argument types. Intrinsics are particularly useful in special operations such as select and polynomials which cannot be easily inferred by the compiler. It’s often best to use intrinsincs to select a specific load/store flavor for operations where the compiler may not always pick the right load/store operation for the best schedule. When using intrinsics, the compiler still handles type checking and data movement, and schedules operations into available slots of a FLIX format.

To understand the limits of compiler automatic inferencing and overloading and vectorization, the rest of this chapter discusses the various programming styles and provides several examples showing how the ConnX BBE32EP DSP can be programmed, including the example results.

32-bit memory ctypes are treated equivalently to corresponding 40-bit register Ctypes. Even though operator overload protos are not explicity defined for 32-bit Cytpes, the equivalence lets programmers use the corresponding 40-bit ctype protos. This means xb\_vecNx32 = xb\_vecNx32 + xb\_vecNx32 works the exact same way as xb\_vecNx40 = xb\_vecNx40 + xb\_vecNx40. The following tables show the correspondance of 32/40-bit ctypes for the purpose of operator overload.

|  |  |
| --- | --- |
| **32-bit Ctype** | **40-bit Ctype** |
| xb\_q11\_20 | xb\_q19\_20 |
| xb\_vecNxq11\_20 | xb\_vecNxq19\_20 |
| xb\_cq11\_20 | xb\_cq19\_20 |
| xb\_vecN\_2xcq11\_20 | xb\_vecN\_2xcq19\_20 |
| xb\_vecNxcq11\_20 | xb\_vecNxcq19\_20 |
| xb\_c32 | xb\_c40 |
| **32-bit Ctype** | **40-bit Ctype** |
| xb\_cq1\_30 | xb\_cq9\_30 |
| xb\_vecNxc32 | xb\_vecNxc40 |
| xb\_vecNxcq1\_30 | xb\_vecNxcq9\_30 |
| xb\_vecN\_2xc32 | xb\_vecN\_2xc40 |
| xb\_vecN\_2xcq1\_30 | xb\_vecN\_2xcq9\_30 |
| xb\_vecNx32 | xb\_vecNx40 |
| xb\_vecNx32U | xb\_vecNx40 |
| xb\_vecNxq1\_30 | xb\_vecNxq9\_30 |
| xb\_int32 | xb\_int40 |
| xb\_q1\_30 | xb\_q9\_30 |

## 4.4 Programming Styles

It is typical for programmers to have to put in some effort on their code, especially legacy code, to make it run efficiently on a vectorized DSP. For example, there may be changes required for automatic vectorization, or the algorithm may need some work to expose concurrency so vector instructions can be used manually as intrinsics. For efficient access to data items in parallel, or to avoid unaligned loads and stores, which are less efficient than aligned load/stores, some amount of data reorganization (data marshalling) may be necessary.

Four basic programming styles that can be used, in increasing order of manual effort, are:

* Auto-vectorizing scalar C code
* C code with vector data types (manually vectorized)
* Use of C instrinsic functions along with vector data types and manual vectorization
* Use of DSP Nature Library

One strategy is to start with legacy C code or to write the algorithm in a natural style using scalar types (possibly using the ConnX BBE32EP DSP special scalars - xb\_int16, xb\_c16, xb\_int40, xb\_c40; e.g., for Q15 or complex or complex Q15 data). Once the correctness of a fixed-point code using the ConnX BBE32EP DSP scalar data-types is determined the limits of what automatic/manual vectorization with operator overloading achieves can be investigated. By profiling the code, computationally intensive regions of code can be identified and the limits of automated vectorization determined.

These parts of code that could be vectorized further can then be modified manually to improve performance. Finally, the most computationally intensive parts of the code can be improved in performance through the use of C intrinsic functions.

At any point, if the performance goals for the code have been met, the optimization can cease. By starting with what automation can do and refining only the most computationallyintensive portions of code manually, the engineering effort can be directed to where it has the most effect, which is discussed in the next sections.

### Auto-Vectorization

Auto-vectorization of scalar C code using ConnX BBE32EP DSP types can produce effective results on simple loop nests, but has its limits. It can be improved through the use of compiler pragmas and options, and effective data marshalling to make data accesses (loads and stores) regular and aligned.

The xt-xcc compiler provides several options and methods of analysis to assist in vectorization. These are discussed in more detail in the Xtensa C and C++ Compiler User’s Guide, in particular in the SIMD Vectorization section. Tensilica® recommends studying this guide in detail; however, following are some guidelines in summary form:

* Vectorization is triggered with the compiler options O3, -LNO:simd, or by selecting the Enable Automatic Vectorization option in Xplorer. The -LNO:simd\_v and -keep options give feedback on vectorization issues and keeps intermediate results, respectively.
* Data should be aligned to 32-byte boundaries because of the 256-bit load/store interface. The XCC compiler will naturally align arrays to start on 32-byte boundaries. But the compiler cannot assume that pointer arguments are aligned. The compiler needs to be told that data is aligned by one of the following methods:
* Using global or local arrays rather than pointers
* Using #pragma aligned(<pointer>, n)
* Compiling with -LNO:aligned\_pointers=on
* Pointer aliasing causes problems with vectorization. The \_\_restrict attribute for pointer declarations (e.g. short \* \_\_restrict cp;) tells the compiler that the pointer does not alias.
* Compiler alignment options, such as -LNO:aligned\_pointers=on, tell the compiler that it can assume data is always aligned.

There are global compiler aliasing options, but these can sometimes be dangerous.

* Subtle C/C++ semantics in loops may make them impossible to vectorize. The LNO:simd\_v feedback can assist in identifying small changes that allow effective vectorization.
* Irregular or non-unity strides in data array accessing can be a problem for vectorization. Changing data array accesses to regular unity strides can improve results, even if some "unnecessary computation" is necessary.
* Outer loops can be simplified wherever possible to allow inner loops to be more easily vectorized. Sometimes trading outer and inner loops can improve results.
* Loops containing function calls and conditionals may prevent vectorization. It may be better to duplicate code and perform a little "unnecessary computation" to produce better results.
* Array references, rather than pointer dereferencing, can make code (especially mathematical algorithms) both easier to understand and easier to vectorize.

### Operator Overloading and Inferencing

Many basic C operators work in conjunction with both automatic and manual vectorization to infer the right intrinsic:

* + addition
* - subtraction: both unary (additive inverse) and binary
* \* multiplication: real and complex
* & bitwise AND
* ^ bitwise XOR
* | bitwise OR
* << bitwise left shift
* >> bitwise right shift
* ~ for ConnX BBE32EP DSP complex types, a complex conjugate operation is inferred; otherwise, bitwise NOT or one’s complement operator
* < less than
* <= less than or equal to
* > greater than
* >= greater than or equal to
* == equal to

The next section illustrates how they work in conjunction.

### Vectorization and Inferencing Examples

The examples provided in this section for the ConnX BBE32EP DSP include code snippets which illustrate the extent of and limits to automatic compiler capabilities. This includes three very simple algorithms: vector add, vector dot product and matrix multiply, and several scalar data types: int, short, etc.

In all the following examples, the VEC\_SIZE used is 1024 and ARRAY\_SIZE was 16.

Almost all these examples vectorize. For example, int vector add:

|  |
| --- |
| int ai[VEC\_SIZE], bi[VEC\_SIZE], ci[VEC\_SIZE]; void vec\_add\_int()  { int i;  for (i = 0; i < VEC\_SIZE; i++)  {  ci[i] = ai[i] + bi[i];  }  } |

On compilation, the following code is produced:

{ bbe\_lvnx16\_ip v0, a2, 64; bbe\_lvnx16\_ip v2, a3, 64; bbe\_addnx40 wv3, wv3, wv2; nop; nop }

{ bbe\_svnx16\_i v4, a4, -32; bbe\_lvnx16\_i\_n v1, a2, -32; bbe\_addnx40 wv2, wv1, wv0; nop; nop }

{ bbe\_svnx16\_ip v6, a4, 64; bbe\_lvnx16\_ip v4, a2, 64 }

{ bbe\_lvnx16\_i v0, a3, -32; bbe\_lvnx16\_i\_n v1, a2, -32; bbe\_movswv wv0, v1, v0; nop }

{ bbe\_svnx16\_i v5, a4, -32; bbe\_lvnx16\_ip v5, a3, 64; bbe\_movsvwl v3, wv2; nop; nop }

{ bbe\_lvnx16\_i v1, a3, -32; bbe\_movsvwh v4, wv2; bbe\_movswv wv2, v1, v4; nop }

{ nop; bbe\_movsvwl v6, wv3; bbe\_movswv wv1, v0, v2; nop }

{ bbe\_svnx16\_ip v3, a4, 64; bbe\_movsvwh v5, wv3; bbe\_movswv wv3, v1, v5; nop }

Note the use of vectorized NX16 loads and stores, and vectorized NX40 adds. If we look at vector add of complex fractional data:

|  |
| --- |
| xb\_cq15 ac15[VEC\_SIZE], bc15[VEC\_SIZE], cc15[VEC\_SIZE]; void vec\_add\_cq15()  { int i;  for (i = 0; i < VEC\_SIZE; i++)  { cc15[i] = ac15[i] + bc15[i];  }  } |

and then look at the disassembly, we see the following:

{ bbe\_lvnx16\_ip v0, a2, 64; bbe\_lvnx16\_ip v1, a3, 64 }

{ bbe\_svnx16\_i v5, a4, -32; bbe\_lvnx16\_i\_n v4, a3, -32; nop; bbe\_addsnx16 v5, v4, v3 }

{ bbe\_svnx16\_ip v2, a4, 64; bbe\_lvnx16\_i\_n v3, a2, -32; nop; bbe\_addsnx16 v2, v1, v0 }

Note the mapping of complex q15 types into vectorized NX16 loads and stores, and vectorized NX16 adds.

A vector dot product short:

|  |
| --- |
| short vec\_dot\_short(short as[], short bs[])  { int i; int sum, sum1 = 0, sum2 = 0; short \*ptr\_as = &as[VEC\_SIZE/2]; short \*ptr\_bs = &bs[VEC\_SIZE/2];  #pragma aligned(as, 32)  #pragma aligned(ptr\_as, 32)  #pragma aligned(bs, 32)  #pragma aligned(ptr\_bs, 32)  for (i = 0; i < VEC\_SIZE/2; i++)  { sum1 += as[i] \* bs[i];  sum2 += ptr\_as[i] \* ptr\_bs[i];  } |

return sum = sum1 + sum2; }

produces

loopgtz a6, 40000a18 <vec\_dot\_short+0x48>

{ bbe\_lvnx16\_ip v0, a2, 32; bbe\_lvnx16\_ip v1, a3, 32; bbe\_mulanx16 wv0, v1, v0; nop } { bbe\_lvnx16\_ip v2, a4, 32; bbe\_lvnx16\_ip v3, a5, 32; bbe\_mulanx16 wv1, v3, v2; nop }

<vec\_dot\_short+0x48>:

{ nop; nop; bbe\_mulanx16 wv1, v3, v2 }

{ nop; nop; bbe\_mulanx16 wv0, v1, v0 }

{ nop; nop; bbe\_raddnx40 wv1, wv1 }

{ nop; nop; bbe\_raddnx40 wv0, wv0 }

{ nop; bbe\_movaw32 a4, wv1 }

{ nop; bbe\_movaw32 a2, wv0 }

Note the automatic selection of the BBE\_MULANX16 for the short, as well as appropriate loads and stores. In addition, the correct reduction-add is selected to return the result as a short.

Two variants that do not vectorize in these examples are vector dot product and matrix multiply of ints. This is for the simple reason that the ConnX BBE32EP DSP offers 16-bit matrix multiplication (via its 16 16x16 bit multipliers), but not 32-bit matrix multiplication. In this case, ordinary scalar code and scalar instructions are used.

One interesting example of complex vectorization and inference starts with the following source code:

|  |
| --- |
| xb\_cq15 test\_arr\_1[VEC\_SIZE], test\_arr\_2[VEC\_SIZE]; xb\_cq9\_30 test\_global\_red\_0; void test\_MULAJ\_xb\_cq15\_xb\_cq9\_30\_xb\_cq15()  { int i;  xb\_cq9\_30 red\_0 = test\_global\_red\_0;  for (i=0; i < VEC\_SIZE; i++)  {  red\_0 += (test\_arr\_1[i] \* ~test\_arr\_2[i]);  }  test\_global\_red\_0 = red\_0;  } |

This produces the disassembly for the loop:

loopgtz a4, 40000618 <test\_MULAJ\_xb\_cq15\_xb\_cq9\_30\_xb\_cq15+0x58>

{ bbe\_lvnx16\_ip v0, a2, 64; bbe\_lvnx16\_ip v1, a3, 64; bbe\_mulanx16j wv0, v1, v0; nop }

{ bbe\_lvnx16\_i v2, a2, -32; bbe\_lvnx16\_i\_n v3, a3, -32; bbe\_mulanx16j wv0, v3, v2; nop }

<test\_MULAJ\_xb\_cq15\_xb\_cq9\_30\_xb\_cq15+0x58>:

{ nop; nop; bbe\_mulanx16j wv0, v1, v0; nop }

{ nop; nop; bbe\_mulanx16j wv0, v3, v2; nop }

{ nop; nop; bbe\_raddnx40c wv0, wv0 }

{ nop; bbe\_movvwll v6, wv0; nop; nop }

{ nop; nop; nop; bbe\_shflnx16i v5, v6, 29 }

{ bbe\_sv4x16\_i v6, a6, 0; nop }

{ bbe\_sv4x16\_i v5, a6, 8; nop }

Note the automatic inference of a complex conjugate multiply BBE\_MULANX16J from the construct in C X[i] \* ~X[i], where X is defined as a complex Q15 type.

### Manual Vectorization

Of course, even the best compiler cannot automatically vectorize all code and loop nests even if all the guidelines have been followed. In this case, the next step is to move from scalar ConnX BBE32EP DSP types to vector types, and manually vectorize the loops. The compiler may still be able to infer the use of vector instrinsics by using standard C operators.

When you manually vectorize, you reduce the loop count size by the number of elements in the vector instructions, and replace scalar types with the corresponding vector type. For example, replacing shorts by xb\_vecNx16 (memory variables or vector register variables). The compiler deals with the ConnX BBE32EP DSP vector data types as it does with any other type.

Below is an example of manually vectorized code for a vector add function:

|  |
| --- |
| void vector\_add (const short a[ ], const short b[ ], short c[ ], unsigned len)  { int i;  // Cast pointers to short into pointers to vectors const xb\_vecNx16 \*va = (xb\_vecNx16\*)a; const xb\_vecNx16 \*vb = (xb\_vecNx16\*)b;  // Assume no pointer aliasing  xb\_vecNx16 \* \_\_restrict vc = (xb\_vecNx16\*)c;    // Change loop count to work on vector types for (i = 0; i < len/XCHAL\_BBEN\_SIMD\_WIDTH; i += 1)  {  vc[i] = va[i] + vb[i];  }  } |

Here we see the loop count divided by XCHAL\_BBEN\_SIMD\_WIDTH, which is equal to 16 for

ConnX BBE32EP DSP, and the use of xb\_vecNx16 vector variables. Also note the use of the \_\_restrict attribute to allow efficient compilation by telling the compiler there is no pointer aliasing to array c.

The programmer casts short pointers (in this case, array references) to vector pointers. The compiler will automatically generate the correct loads and stores. Note that this example assumes that "len" is a multiple of BBEN SIMD WIDTH which is sixteen. If it is not, then the programmer needs to write extra code for the more general situation. However, if the data is arranged to always be a size multiple of the normal vector size, then the result can be more efficient even if a few unnecessary computations are included. Padding a data structure with a few zeroes to make it a multiple (of sixteen in the case above) is also often easy to do.

### C-Intrinsic-based Programming

The final programming style is to use explicit intrinsics. Interestingly, it may not be necessary to use intrinsics everywhere, as the compiler may, for example, infer the right vector loads and stores. Sometimes adding just a few strategic intrinsics may be sufficient to achieve maximum efficiency. The compiler can still be counted on for efficient scheduling and optimization.

Here is a simple example adding up a vector:

|  |
| --- |
| short addemup(short a[ ], unsigned int n)  { int i; short sum = 0;  for (i = 0; i < n; i += 1)  {  sum += a[i];  }  return sum;  } |

Here is an optimized intrinsic-based version:

|  |
| --- |
| short addemup\_v(short a[ ], unsigned int n)  { int i;  // Set a vector pointer to array a xb\_vecNx16 \*pa = ((xb\_vecNx16 \*) a);  // Declare sum as a vector type and initialize to zero xb\_vecNx16 sum = 0; xb\_vecNx16 avec;  for (i = 0; i < n; i += XCHAL\_BBEN\_SIMD\_WIDTH)  {  sum += (\*pa++); }  // Add vector of intermediate sums and return short result return BBE\_RADDNX16(sum);  } |

Following are several interesting points:

* There is no need to use explicit vector loads.
* Similarly, the efficient vector adds are inferred from the code, which is still "C-like".
* The only explicit intrinsic necessary is the BBE\_RADDNX16.
* This is a simple evolution from a manually vectorized version of this code.
* "sum" is initialized by casting it to a short 0, which initializes the vector "sum" to 0 in each element.
* Note that intrinsics are not assembly operations. They need not be manually scheduled into FLIX bundles; the compiler takes care of all that. And the code still remains quite "Clike".
* Intrinsic based programming can make use of the rich set of the ConnX BBE32EP DSP data types and the right proto can be chosen that maps the data type into the underlying base instruction. Protos are listed in detail in the ISA HTML.
* The compiler will automatically select load/store instructions, but programmers may be able to optimize results using their own selection, by using the correct intrinsic instead of leaving it to the compiler.

## 4.5 Conditional Code

Programmers are encouraged to use the N-way programming model in ConnX BBE32EP DSP. N-way programming is a convenient way to write code that offers ease of portability across cores in the BBE family. However, the assembler still uses operations with specific SIMD number native to the core used, in this case N=16. N-way programming is supported by inclusion of the primary header file - <xtensa/tie/xt\_bben.h>. This header file xt\_bben.h includes a supplementary include file specific to the BBE machine being programmed, here <xtensa/tie/xt\_ bbe32.h>. Within these include files, there are a number of #defines that define “XCHAL” variables describing the machine characteristics, such as:

* SIMD width: #define XCHAL \_BBEN \_SIMD \_WIDTH 16
* FFT: #define XCHAL\_HAVE\_BBEN\_FFT 1
* Advanced Precision Multiply/Add: #define XCHAL\_HAVE\_BBEN\_ADVPRECISION 1

In addition to N-way programming, to make code robust to the wide configurability in ConnX BBE32EP DSP, it is useful to be able to write *conditional* code so that if an optional package is present (say the FFT option), the ISA support from the package can be explicitly used. Otherwise, the code may use a slower emulation for that operation. This is particularly useful in a complicated codebase shared between many cores with no source differences, one can use an appropriate #define to write conditional code specific to the SIMD size or configuration options included, so that all variants can be located in a single source file. The #defines for external, user configurable options are separately listed as a section inside the primary header file (<xtensa/tie/xt\_bben.h>).

The following example illustrates the usage of vector types, XCHAL\_BBEN \_SIMD \_WIDTH and an intrinsic call based on the N-way programming model:

xb\_vecNx16 vin; xb\_vecNx40 vout = 0; for (i=0; i < N/XCHAL\_BBEN\_SIMD\_WIDTH; i++) vout += vin[i] \* vin[i];

\*out\_p = BBE\_RADDNX40(vout);

## 4.6 Using the Two Local Data RAMs and Two Load/Store Units

The ConnX BBE32EP DSP has two load/store units, which are generally used with two local data RAMs. Effective use of these local memories and obtaining the best performance results may require experimentation with several options and pragmas in your program.

In addition, to correctly analyze your specific code performance, it is important to carry out profiling and performance analysis using the right ISS options.

You may have a "CBox" (Xtensa Connection Box) configured with your ConnX BBE32EP

DSP configuration in order to access the two local data RAMs. For example, the two default ConnX BBE32EP DSP templates described in *Implementation Methodology* on page 147.

Using the CBox, if a single instruction issues two loads to the same local memory, the processor will stall for one cycle. Stores are buffered by the hardware so it can often sneak into a cycle that does not access the same memory. For example, use of a CBox with two local data RAMs may cause occasional access contention, depending on the data usage and access patterns of the code. This access contention is not modeled by the ISS unless you select the --mem\_model simulation parameter. Thus, if your code uses the two local data RAMs and your configuration has a CBox, it is important to select memory modeling when studying the code performance.

If you are using the standard set of LSPs (Linker Support Packages) provided with your

ConnX BBE32EP DSP configuration, and do not have your own LSP, use of the "sim-local" LSP will automatically place compiled code and data into local instruction and data memories to the extent that this is possible. Thus, Tensilica® recommends the use of sim-local LSP or your own LSP for finer grained control.

Finer-grained control over the placement of data arrays and items into local memories and assigning specific items to specific data memories can be achieved through using attributes on data definitions. For example, the following declaration might be used in your source code:

short ar[NSAMPLES][ARRAY\_SIZE][ARRAY\_SIZE] \_\_attribute\_\_ (section(".dram1.data"));

This code declares a short 3-dimensional array ar, and places it in data RAM 1. The compiler automatically aligns it to a 32-byte boundary.

Once you have placed arrays into the specific data RAM you wish, there are two further things to control. The first is to tell the compiler that data items are distributed into the two data RAMs, which can be thought of as "X" and "Y" memory as is often discussed with DSPs.

The second one is to tell the compiler you are using a CBox to access the two data RAMs. There are two controls, currently not documented in the *Xtensa® C Application Programmer’s Guide* or *Xtensa C and C++ Compiler User’s Guide*, that provide this further level of control.

These two controls are a compiler flag, -mcbox, and a compiler pragma (placed in your source code) called "ymemory".

The -mcbox compiler flag tells the compiler to never bundle two loads of "x" memory into the same instruction or two loads of "y" memory into the same instruction (stores are exempt as the hardware buffers them until a free slot into the appropriate memory bank is available). Anything marked with the ymemory will be viewed by the compiler as "y" memory. Everything else will be viewed as "x" memory.

There are some subtleties in using these two controls — when they should be used and how. Here are some guidelines:

* If your configuration does not have CBox, you should not use -mcbox as you are constraining the compiler to avoid an effect that does not apply.
* If you are simulating without --mem \_model, -mcbox might seem to degrade performance as the simulator will not account for the bank stalls.
* If you have partitioned your memory into the two data RAMs, but you have not marked half the memory using the ymemory pragma, use of -mcbox may give worse performance. Without it, randomness will avoid half of all load-load stalls. With the flag, you will never get to issue two loads in the same instruction.
* However, also note that there are scenarios where -mcbox will help. If, for example, there are not many loads in the loop, it might be possible to go full speed without ever issuing two loads in one instruction. In that case, -mcbox will give perfect performance, while not having -mcbox might lead to random collisions.
* If you properly mark your dataram1 memory using ymemory, or if all your memory is in one of the data rams, -mcbox should always be used.
* Without any -mcbox flag, but with the ymemory pragma, the compiler will never bundle two "y" loads together but might still bundle together two "x" loads. With the --mcbox flag, it will also not bundle together two "x" loads.

Thus in general, the most effective strategy for optimal performance is to always analyze ISS results that have used memory modeling; to assign data items to the two local data memories using attributes when declaring them; to mark this using the ymemory pragma; and to use mcbox assuming your configuration has a CBox.

Use of the ymemory pragma is illustrated in the following code:

|  |
| --- |
| complex a[ARRAY\_SIZE][ARRAY\_SIZE][NSAMPLES]\_\_attribute\_\_((aligned(32),section(".dram1.data")));  complex b[ARRAY\_SIZE][ARRAY\_SIZE][NSAMPLES] \_\_attribute\_\_((aligned(32),section(".dram0.data")));  xb\_vecN\_2xc16 c\_auto\_opt[ARRAY\_SIZE][ARRAY\_SIZE][NSAMPLES/4] \_\_attribute\_\_((aligned(32),section(".dram0.data")));  void mm\_auto\_opt\_4x4\_stream\_complex (xb\_vecNxc16 (\*\_\_restrict a)[ARRAY\_SIZE][NSAMPLES/4], xb\_vecN\_2xc16 (\*\_\_restrict b) [ARRAY\_SIZE][NSAMPLES/4], xb\_vecN\_2xc16 (\* \_\_restrict cp)  [ARRAY\_SIZE][NSAMPLES/4]){  #pragma ymemory (a) int i,j,h;  for (i=0; i < ARRAY\_SIZE; i+=1) { for (j=0; j < ARRAY\_SIZE; j+=1) { for (h=0; h < NSAMPLES/4; h++) { |
| cp[i][j][h] = a[i][0][h] \* b[0][j][h] + a[i][1][h] \* b[1][j][h] + a[i][2][h] \* b[2][j]  [h] + a[i][3][h] \* b[3][j][h];  }  }  }  }  ..... mm\_auto\_opt\_4x4\_stream\_complex((xb\_vecN\_2xc16 (\*) [ARRAY\_SIZE][NSAMPLES/4])a, (xb\_vecN\_2xc16 (\*) [ARRAY\_SIZE][NSAMPLES/4])b, c\_auto\_opt);.....  ..... |

![](data:image/png;base64...)**Note:** In this code that we place input array a in data RAM 1; b in data RAM 0; and the output array in data RAM 0. We tell the compiler with the ymemory pragma that the *a* array is in ymemory, which effectively tells it the other two arrays are in x memory. Finally, since this is run on a configuration with a cbox, we compile with the mcbox option and run with the memory modeling enabled in the ISS. The combination of the ymemory pragma and the mcbox compiler directive produces better results than if only one was used.

## 4.7 Other Compiler Switches

The following two other compiler switches are important:

* *-mcoproc*: Discussed in the *Xtensa® C Application Programmer’s Guide* and *Xtensa C and C++ Compiler User’s Guide* may give better results to certain program code.
* *O3 and SIMD vectorization*: If you use intrinsic-based code and manually vectorize it, it may not be necessary to use O3 and SIMD options. In fact, this may produce code that takes longer to execute than using O2 (without SIMD, which only has effect at O3). However, if you are relying on the compiler to automatically vectorize, it is essential to use O3 and SIMD to see this happen. As is the case with all compiler controls and switches, experimenting with them is recommended. In general, -O3 (without SIMD) will still be better than -O2.

## 4.8 TI C6x Instrinsics Porting Assistance Library

Tensilica® provides the following include header file for the RG-2016.4 release:

<install\_path>/XtDevTools/install/tools/<*release*>-linux/XtensaTools/xtensa-elf/include/xtensa/ c6x-compat.h

This file is included to help in porting code that uses TI C6x intrinsics to any Tensilica® Xtensa processor as it maps these intrinsics to standard C. Because it maps TI C6x intrinsics to standard C, the performance of the code is not optimized for the ConnX BBE32EP DSP; to optimize the code further, you need to manually modify it using either ConnX BBE32EP DSP data types that the compiler can vectorize and infer from, and/or ConnX BBE32EP DSP intrinsics.

![](data:image/png;base64...)**Note:** The path to this header file has to be modified accordingly for a different release.

Thus, this header file is intended as a porting aid only. One recommended methodology is:

* Include this code in your source files that use TI C6x intrinsics and move them to ConnX BBE32EP DSP. As it handles most intrinsics, the code should, with little manual effort, compile and execute successfully on ConnX BBE32EP DSP.
* Using the command line or Xtensa Xplorer profiling capabilities, profile the code to determine those functions, loops and loop nests which take most of the cycles.
* Rewrite those computationally-intensive functions, loops or loop nests to use ConnX

BBE32EP DSP data types and compiler automatic vectorization, or ConnX BBE32EP DSP intrinsics, to maximize application performance. You could substitute calls to the ConnX BBE32EP DSP library functions in these places.

The TI C6X standard C intrinsics implement 122 of 131 TI C6x intrinsic functions. Those not implemented are: \_gmpy, \_gmpy4, \_xormpy, \_lssub, \_cmpy, \_cmpyr, \_cmpyr1, \_ddotpl2r, and \_ddotph2r.

### Porting TI C6X Code Examples

This section contains some simple examples for using the intrinsic porting assistance file to port TI C6X code.

The first example uses the TI C6X \_mpy intrinsic.

### Example 1: \_mpy

|  |
| --- |
| int a[VEC\_SIZE], b[VEC\_SIZE], c[VEC\_SIZE]; void test\_mpy()  { int i; for (i = 0; i < VEC\_SIZE; i++)  { c[i] = \_mpy(a[i], b[i]);  }  }  int main()  { test\_mpy();  } |

The \_mpy intrinsic, which is used when you include c6x-compat.h, is:

|  |
| --- |
| static inline int \_mpy(int src1, int src2)  { return (short) src1 \* (short) src2;  } |

Note that the TI \_mpy intrinsic is just mapped into a standard C multiply of two short variables. The inner-loop disassembly is:

{ mul16s a10, a8, a9; bbe\_l16si\_s1 a3, a2, 0 }

{ l16si a5, a4, 0; bbe\_l16si\_s1 a9, a4, 4 }

{ s32i.n a6, a7, 0; bbe\_l16si\_s1 a8, a2, 4 } { s32i a10, a7, 4; addi.n a4, a4, 8; nop; addi.n a2, a2, 8 }

{ mul16s a6, a3, a5; addi.n a7, a7, 8 }

However, for a few TI instructions, the compiler can do better with ConnX BBE32EP DSP vectorization and automatic inference of intrinsics. The inner-loop disassembly is:

{ bbe\_svnx16\_ip v6, a8, 64; bbe\_lvnx16\_i\_n v3, a3, 32; bbe\_movswv wv1, v3, v1; bbe\_packsnx40 v4, wv0 }

{ bbe\_lvnx16\_ip v1, a3, 64; bbe\_movsvwl v6, wv2; bbe\_movswv wv0, v2, v0; nop }

{ bbe\_svnx16\_i v7, a8, -32; bbe\_movsvwh v7, wv2; bbe\_mulnx16 wv2, v5, v4; nop }

{ bbe\_lvnx16\_ip v0, a2, 64; bbe\_lvnx16\_i\_n v2, a2, 32; nop; bbe\_packsnx40 v5, wv1 }

Note the compiler automatically uses 16-way multiplies, 16-way loads (for ints which are cast to shorts), packs (to convert 40-bit results to shorts, 16-way stores (for ints), etc. – all from standard C code. **Example 2: \_add2**

Some intrinsics may need some manual code modification in order to make use of compiler automated vectorization. TI has a number of intrinsics that unpack integers into two shorts, and repack shorts back into integers after computation, such as \_add2:

void test\_add2()

{ int i; for (i = 0; i < VEC\_SIZE; i++) { e[i] = \_add2(a[i], b[i]);

}

}

One approach is to do the unpacking and packing in separate loops and then transform \_add2 into two routines, calc and merge, as follows:

|  |
| --- |
| void test\_add2\_transform\_calc()  { int i; for (i = 0; i < VEC\_SIZE; i++)  {  g1[i] = a[i] & 0xffff; g2[i] = a[i] >> 16; h1[i] = b[i] & 0xffff; h2[i] = b[i] >> 16;  }  for (i = 0; i < VEC\_SIZE; i++)  {  r[i] = g1[i] + h1[i]; s[i] = g2[i] + h2[i]; |
| }  }  void test\_add2\_transform\_merge()  { int i; for (i = 0; i < VEC\_SIZE; i++)  { e[i] = ((unsigned int) r[i] << 16) | ((unsigned int) s[i]);  }  } |

The calc routine unpacks the operands and does the computation, using ordinary C code. The merge routine packs the two results back together in the form the TI intrinsic does.

The calc disassembly vectorizes:

{ bbe\_svnx16\_i v1, a4, -32; bbe\_lvnx16\_ip v2, a3, 64; bbe\_addnx40 wv2, wv3, wv2; nop; nop }

{ bbe\_svnx16\_ip v5, a7, 64; bbe\_lvnx16\_i\_n v1, a2, 32; bbe\_addnx40 wv3, wv1, wv0; nop; nop }

{ bbe\_svnx16\_i v3, a5, -32; bbe\_lvnx16\_i\_n v3, a3, -32; bbe\_movsvwh v5, wv0; nop; nop }

{ bbe\_lvnx16\_ip v0, a2, 64; bbe\_movsvwh v6, wv1; nop; nop }

{ bbe\_svnx16\_ip v4, a6, 64; bbe\_movsvwl v2, wv2; bbe\_movswv wv1, v3, v2; nop }

{ bbe\_svnx16\_i v5, a7, -32; bbe\_movsvwl v1, wv3; bbe\_movswv wv0, v1, v0; nop }

{ bbe\_svnx16\_i v6, a6, -32; bbe\_movsvwh v3, wv3; nop; nop }

{ bbe\_svnx16\_ip v1, a9, 64; bbe\_packlnx40 v1, wv1 }

{ bbe\_svnx16\_i\_n v3, a9, -32; nop; bbe\_srainx40 wv3, wv1, 16 }

{ bbe\_svnx16\_ip v2, a8, 64; bbe\_movsvwh v2, wv2; nop; nop }

{ nop; bbe\_packlnx40 v3, wv0 }

{ bbe\_svnx16\_i\_n v2, a8, -32; nop; bbe\_srainx40 wv2, wv0, 16 }

{ nop; bbe\_movsvwl v2, wv3; bbe\_unpkunx16 wv1, v1; nop }

{ nop; bbe\_movsvwh v3, wv3; bbe\_unpkunx16 wv0, v3; nop }

{ bbe\_svnx16\_ip v2, a5, 64; bbe\_movsvwl v2, wv2; nop; nop }

{ nop; bbe\_movsvwl v4, wv1; nop; nop }

{ nop; bbe\_movsvwh v1, wv2; nop; nop }

{ bbe\_svnx16\_ip v2, a4, 64; bbe\_movsvwl v5, wv0; nop; nop }

But the merge disassembly does not. Therefore, try to avoid transforming data to and from packed intrinsic forms in the loops that must be optimized.

### Example 3: \_add2 and \_sub2

Suppose you had a short sequence of a TI \_add2 and then a TI\_sub2 intrinsics. To begin to optimize this sequence, we want to avoid the transformation of intermediates backed into packed form. This can be tried by creating a \_add2\_sub2\_calc routine, followed by a merge routine, which does the pack back into the TI merged form.

If there is a long sequence of calculations, avoiding the packing back allows the xt-xcc compiler to vectorize and infer naturally, which will save considerable cycles.

### Manual Vectorization

There may be times when manual vectorization and intrinsics may be necessary to achieve optimal results. Also, you will need to decide what to do about saturating operations. The ConnX BBE32EP DSP does not saturate on most operations, nor does it have an overflow register. Instead, it uses 40-bit vector registers that includes 8 guard bits and 16-bit vector registers without guard bits. For 32/40-bit data contained in 40-bit vector registers, the data is either packed and saturated to 16-bits or saturated to 32-bits. In either case post-saturation, the data is moved into one or two narrow vec registers respectively before storing as 16-bit elements in memory. Keep this different model in mind when converting code.

If the code with intrinsics is in parts of code which do not take many cycles in execution, (for example, control code, not loop-intensive data code) then you may just leave the intrinsic conversion to standard C and divide the code into one of the following categories:

* Rarely executed, low-cycle count code (that is, do not optimize the code)
* Heavily-executed, high cycle-count (optimize manually where the compiler does not)
* “Middle ground” code, which you must decide whether to optimize based on time and performance goals

# 5. Configurable Options

**Topics:**

•

*FFT*

•

*Symmetric FIR*

•

*Packed Complex Matrix*

*Multiply*

•

*LFSR and Convolutional*

*Encoding*

•

*Linear Block Decoder*

•

*D Despread*

*1*

•

*Soft-bit Demapping*

•

*Comparison of Divide*

*Related Options*

•

*Advanced Precision*

*Multiply/Add*

•

*Inverse Log-likelihood*

*Ratio (LLR)*

•

*Single and Dual Peak*

*Search*

•

*Single-precision V*

*ector*

*Floating-Point*

## 5.1 FFT

*This option provides FFT and DFT support offering significant performance improvements on FFT operations of any size.*

The ConnX BBE32EP DSP FFT package provides special FFT instructions optimized to perform a range of DFT computations. These FFT instructions implement a generic Discrete Fourier Transform in a series of steps. Each step is a pass over the whole input.

The basic sequence for decimation in frequency FFT operation is as follows:

1. Load a vector of inputs,
2. Multiply by constants (also called “rotations”). These are constant throughout all FFT pass sizes and depend on the radix type (1, j, -1 and –j) in *Figure 4: Radix4 FFT Pass* on page 72. Effectively, these rotation constants only modify the sign of the inputs (real or imaginary) and thus paired with the next step.
3. Perform a radix add. The radix add instructions add the inputs to a radix block while applying the correct sign change coming from the rotation constants.
4. Complex multiply by constants (also called “twiddle factors”). These depend on the FFT size and each individual value going through the transform. They are marked as Tw in *Figure 4: Radix4 FFT Pass* on page 72.

![](data:image/png;base64...)

### Figure 4: Radix4 FFT Pass

In the general case, the ConnX BBE32EP DSP architecture allows a vector/state load, a vector store, a radix4 add, a vector-multiply and a shuffle to execute all in parallel. This produces to a full vector worth of radix4 FFT results as output per cycle. As long as the inputs for the butterfly are coming from a distance of at least one vector length away, the following sequence of basic instructions is followed:

1. Load four input vectors,
2. Use four appropriate FFT add instructions,
3. Apply the twiddle factors. The twiddle factor for one of the vectors is ‘1’, the other three require a multiply (see *Figure 4: Radix4 FFT Pass* on page 72),
4. Store four output vectors.

![](data:image/png;base64...) **Note:** In some stages, an additional interleave or shuffle step may be required.

Some FFT operations require control or monitoring of an instruction for, say, normalization and/or scaling. The BBE\_MODE control register state is used to set appropriate mode for such operations.

Depending on the FFT size (N) the usage of FFT instructions in the ConnX BBE32EP DSP can differ slightly. Consequently, the package has built-in support for radix-2, radix-3, radix-4 and radix-5 FFT implementations.

* When N is a power of ‘2’
* Even powers of ‘2’ – radix4
* Odd powers of ‘2’ – radix4 and radix2 (for only the last pass when N=8)
* When N is a non-power of ‘2’
* Radix3 and radix5

When performing an FFT computation, unless the results go into an inverse FFT (with some processing applied), the results are usually needed in natural order. For best performance it's recommended to use auto-sort FFT algorithms, for example Stockham FFT.

## 5.2 Symmetric FIR

*This option inserts a pre-adder function in front of the multiplier tree, effectively doubling the number of taps that can be handled by the core MACs.*

Optimized ISA support to accelerate symmetric FIR operations is available as a separate configurable option. The symmetric FIR operations double the effective MACs per cycle.

Symmetric FIR functions are supported by the following ConnX BBE32EP DSP special operations:

* For real coefficients, real data
* BBE\_ADDPNX16RRU - State-Based Add with Shift Right for Real Data Symmetric FIR with Updates of States by Shifting Elements
* BBE\_ADDPNX16RRUMBC - State-Based Add with Shift Right for Real Data Symmetric FIR with Update of States Partially by Shifting Elements and Partially by using Input Vectors
* BBE\_ADDPNX16RRUMBCIAD - State-Based Add with Shift Right for Real Data Symmetric FIR with Update of States B and C by using Input Vectors
* For real coefficients, complex data
* BBE\_ADDPNX16RCU - State-Based Add with Shift Right for Complex Data Symmetric FIR by Shifting Elements
* BBE\_ADDPNX16RCUMBC - State-Based Add with Shift Right for Complex Data Symmetric FIR with Update of States Partially by Shifting Elements and Partially by using Input Vectors
* BBE\_ADDPNX16RCUMBCIAD - State-Based Add with Shift Right for Complex Data Symmetric FIR with Update of States B and C by using Input Vectors

This package supports complex data , real coefficients and does not support complex data, complex coefficients symmetric FIR filters.

In ConnX BBE32EP DSP, the doubling of effective acceleration of FIR filtering computations occurs when real coefficients are assumed to have a symmetric impulse response while the data can be real/complex. For real data, operation BBE\_ADDPNX16RRU is first used for data summation to produce two results **X**, Y. Four special machine states BBE\_STATE{A,B,C,D} are pre-loaded with Nx16 real data

1. StateA and StateD are first added into Nx16 result X
2. {StateA, StateB} concatenated (1:16) elements are added to {StateD, StateC} concatenated elements (15:30) for Nx16 result Y
3. Concatenated {StateA, StateB} are updated by shifting down 2 real elements and setting 0s at the top. {StateD, StateC} data is updated by shifting up 2 real elements and filling the bottom with the top 2 elements of state D
4. Use BBE\_ADDPNX16RRU{MBC, MBCIAD} variants to input new vector data into states while rotating and shifting

For complex data, BBE\_ADDPNX16RCU is first used for data summation to produce two results

X, Y. Four special states BBE\_STATE{A,B,C,D} are pre-loaded with N/2x16 complex data (see *Figure 5: 16-tap Real Symmetric FIR with Complex Data* on page 76)

1. StateA and StateD are first added into N/2x16 complex result X
2. {StateA, StateB} concatenated (1:8) complex elements are added to {State D, State C} concatenated complex elements (7:14) for complex N/2x16 result Y
3. {StateA, StateB} updated by down 2 complex element shift and 0 setting at the top; {StateD, StateC} shifted up 2 complex elements and filling of the bottom with the top 2 elements of stateD
4. Use BBE\_ADDPNX16RCU{MBC, MBCIAD} variants to input new vector data into states while rotating and shifting

Use BBE\_EXTRNX16C or BBE\_L32XP operations to prepare pair of real 16-bit coefficients.

Results X, Y are then multiplied with coefficients and pairwise added using special operation BBE\_MUL(A)NX16PR as covered earlier

• out = X \* p + Y \* q, where p, and q are real 16-bit coefficients, and X, Y are the results found above.

An Example of Symmetric FIR Operations with Complex Data is shown below.

For Symmetric FIR Operations with Complex Data, there are 4 special states used to help calculation. Follow the procedure to help with the calculations.

1. Load data to {StateA, StateB, StateC, stateD} There are 8 elements in each state.

Each element is 16 bit complex, thus there are a total of 256 bits . E.g. 16 bit complex

* + data[0]-data[7]->stateA
  + data[8]-data[15]->stateB
  + data[7]-data[14]->stateC
  + data[15]-data[22]-->stateD

1. Use Operation: BBE\_ADDPNX16RCU E.g
   * xb\_vecNx16 sel\_2, sel\_1;
   * BBE\_ADDPNX16RCU(sel\_1,sel\_2);

As seen in Figure 5, X=Sel\_1,Y=Sel\_2.

After operation BBE\_ADDPNX16RCU completes, {StateA, StateB, StateC, stateD} will be updated as shown in Figure5.

1. Use operation BBE\_EXTRNX16C to load a 32 bit coefficient from memory to register. As shown in Figure 5, p and q are 16 bit.
2. Use operation BBE\_MUL(A)NX16PR to do MUL/MAC for X/Y and p/q. E.g.
   * result\_vec = BBE\_MULNX16PR(sel\_2, sel\_1, coeff0);
   * BBE\_ADDPNX16RCU(sel\_1,sel\_2);
   * BBE\_MULANX16PR(result\_vec, sel\_2, sel\_1, coeff1);

![](data:image/png;base64...)

### Figure 5: 16-tap Real Symmetric FIR with Complex Data

Refer to *Implementation Methodology* on page 147 on how to configure a ConnX BBE32EP DSP with the symmetric FIR option. The ISA HTML for symmetric FIR instructions describes each operation’s implementation and how the different operands are set up for a symmetric FIR operation.

![](data:image/png;base64...) **Note:**

* *Symmetric FIR* option adds four states - BBE\_STATE{A,B,C,D} - to a ConnX BBE32EP DSP configuration which are otherwise not included.
* *FFT* & *Symmetric FIR* options share significant amounts of hardware. When configuring in one of these options in your ConnX BBE32EP DSP core, it may be appropriate to add the other as the incremental cost is small.
* As a reference, several code examples are packaged with standared ConnX BBE32EP DSP configurations in Xtensa Xplorer.

## 5.3 Packed Complex Matrix Multiply

*This option adds support for vectors where the matrix elements are ordered by matrix rather than grouped by element.*

ConnX BBE32EP DSP has support for efficient computation of small complex packed matrix multiplies - 2x2, 4x4, and variants.

Complex packed matrix multiplies in ConnX BBE32EP DSP are accomplished by decomposing the problem into smaller tasks:

1. Load matrix data into machine vector registers
2. Use *special shuffle instructions* in multi-step execution to reorganize order of data depending on matrix size
3. Perform multistep MAC operations and store result
   * For square matrices, use regular multiplies after shuffle
   * For rectangular matrices use special multiplies with replication

The *special shuffle instructions* (patterns for matrix element reordering) are available only as a configuration option to enhance regular shuffle operation BBE\_SHFLNX16I and select operation BBE\_SELNX16I. The shuffle/select patterns reorder N complex elements of two source vectors into a N/2 complex elements of destination output vector for processing.

The immediate value argument in these operations selects one of many patterns to select/ shuffle from inputs to outputs. As for notations, even though operations target complex values, elements are designated as real and imaginary pairs, so (0, 1, 2, 3, ) elements means 0th real, 0th imaginary, 1st real, 1st imaginary elements for the first two complex numbers and so on.

Here's an example of a 2x2 complex packed matrix multiply routine in ConnX BBE32EP DSP

|  |
| --- |
| // Using matrix multiply uses special data shuffles for complex 2x2\*2x2 matrix multiply  void matmul\_packed\_complex\_2x2\_2x2(complex\_short in1[][2][2], complex\_short in2[][2][2], complex\_short out[][2][2], int n\_vec)  { int i; xb\_vecN\_2xc16 \* \_\_restrict in1\_p = (xb\_vecN\_2xc16 \*) in1; xb\_vecN\_2xc16 \* \_\_restrict in2\_p = (xb\_vecN\_2xc16 \*) in2; xb\_vecN\_2xc16 \* \_\_restrict out\_p = (xb\_vecN\_2xc16 \*) out; xb\_vecN\_2xc40 vout; xb\_vecN\_2xc16 select\_vec1, select\_vec2; const vsaN\_2C shft = 15; // variable pack shift amount for (i = 0; i < n\_vec;++i)  { select\_vec1 = BBE\_SHFLN\_2XC16I(in1\_p[i], BBE\_SHFLI\_MMC2X2X2X2\_M1\_STEP\_1); // 1st  matrix, step 1  select\_vec2 = BBE\_SHFLN\_2XC16I(in2\_p[i], BBE\_SHFLI\_MMC2X2X2X2\_M2\_STEP\_1); // 2nd  matrix, step 1  vout = select\_vec1 \* select\_vec2; // multiply partial result  select\_vec1 = BBE\_SHFLN\_2XC16I(in1\_p[i], BBE\_SHFLI\_MMC2X2X2X2\_M1\_STEP\_2); // 1st  matrix, step 2  select\_vec2 = BBE\_SHFLN\_2XC16I(in2\_p[i], BBE\_SHFLI\_MMC2X2X2X2\_M2\_STEP\_2); // 2nd  matrix, step 2  vout += (select\_vec1 \* select\_vec2); // multiply complete result out\_p[i] = BBE\_PACKVN\_2XC40(vout, shft); // variable pack and store  } |

return; }

![](data:image/png;base64...)**Note:** Streaming order matrix multiplies can be done without shuffling with regular machine multiplies for real or complex.

## 5.4 LFSR and Convolutional Encoding

*This option adds support for channel coding operations that involve LSFR code generation, such as scrambling and channel spreading. This option also provides support for convolutional encoding.*

The LFSR option adds special operations to accelerate LFSR sequence generation up to 32bits per cycle. This is done by emulating a 32x32-bit matrix by 32x1-bit multiplication. All the multiplication and reduction add operations in this option are in Galois Field (GF2). ConnX BBE32EP DSP LFSR generation is executed in steps:

1. Initialize 32x32-bit matrix state to hold polynomial variants for 32 shifts. Use BBE\_MOVBMULSTATEV operation to load state BBE\_BMUL\_STATE
2. Initialize 32-bit vector to hold initial shift register value. Use BBE\_MOVBMULACCA operation to load state BBE\_BMUL\_ACC. The state is updated with result for the next issue.
3. Output 32-bit result at each issue and shift right old results using the BBE\_BMUL32A operation. Set the second 32-bit register input to 0, not used for LFSR.

The above sequence can be modified to efficient implement various variants - generate Gold31 (3GPP) standard PRBS sequence (see following code sample). This can be done by using two sequence generators and XOR'ing the resulting sequences. You may also need to interleave parallel processed results to avoid interlocks. A similar methodology can be used for XRC processing too - use second input 32-bit register for updating the CRC state by XOR'ing input bits with output.

|  |  |
| --- | --- |
| /\* First block LFSR bits generated outside loop for x and y polynomials along with the current initial states\*/  // Inner loop, generating the next nbits\_256 - 1 blocks for(i=0;i < nbits\_256;++i) {  out\_ptr[i] = v1^v2; // scrambler output as (x XOR y) BBE\_MOVBMULACCA(x1); // initial condition for x  // Compute next 256 LFSR bits, preparing x states  S0 = x\_mat\_ptr[0];  S1 = x\_mat\_ptr[1];  BBE\_MOVBMULSTATEV(S1,S0,0);  S0 = x\_mat\_ptr[2];  S1 = x\_mat\_ptr[3];  BBE\_MOVBMULSTATEV(S1,S0,1);  // Two-vector processing to avoid interlocks v1 = BBE\_BMUL32A(v1, 0); // generate 32 bits x v3 = BBE\_BMUL32A(v3, 0); | |
| v1 = BBE\_BMUL32A(v1, 0); v3 = BBE\_BMUL32A(v3, 0); v1 = BBE\_BMUL32A(v1, 0); v3 = BBE\_BMUL32A(v3, 0); v1 = BBE\_BMUL32A(v1, 0); v3 = BBE\_BMUL32A(v3, 0);  // Pick top half vectors  v1 = BBE\_SELNX16I(v3, v1, BBE\_SELI\_INTERLEAVE\_2\_HI); state  // Repeat for vector y sequence  // Set up y states  BBE\_MOVBMULACCA(x2);  S0 = y\_mat\_ptr[0];  S1 = y\_mat\_ptr[1];  BBE\_MOVBMULSTATEV(S1,S0,0);  S0 = y\_mat\_ptr[2];  S1 = y\_mat\_ptr[3];  BBE\_MOVBMULSTATEV(S1,S0,1);  // Process  v2 = BBE\_BMUL32A(v2, 0); v4 = BBE\_BMUL32A(v4, 0); v2 = BBE\_BMUL32A(v2, 0); v4 = BBE\_BMUL32A(v4, 0); v2 = BBE\_BMUL32A(v2, 0); v4 = BBE\_BMUL32A(v4, 0); v2 = BBE\_BMUL32A(v2, 0); v4 = BBE\_BMUL32A(v4, 0); v2 = BBE\_SELNX16I(v4, v2, BBE\_SELI\_INTERLEAVE\_2\_HI); x2 = BBE\_MOVABMULACC(); // keep y state  }  // Process remaining bits | x1 = BBE\_MOVABMULACC(); // keep x |

onvolutional coding is a form of forward error correction (FEC) coding based on finite state machines - input bit stream is augmented by adding patterns of redundancy data. ConnX BBE32EP DSP version supports up to 16-bit polynomial encoder computing 64 encoded bits at each issue.

The optional BBE\_CC64 convolutional coding operation accepts the following inputs:

* An input Nx16-bit vector, 79 LSBs are used at each cycle (taken from the 5 LSB elements). A shuffle is needed to process 64 new bits at the LSB positions.
* Polynomial definition (16 LSB bits of AR register) to form register state definition
* Inout Nx16-bit vector (contains previous 64 generated bits placed at 4x16 LSB elements)

The operations returns 64 encoded bits placed at 4x16 MSB elements of inout register and shifts right old 64 processed bits to prepare for next cycle.

## 5.5 Linear Block Decoder

*The support from this option is used for decoding block linear error-correction codes such as Hamming codes. In general, it can correlate a set of binary code vectors against a vector of real quantities.*

The support to accelerate linear block decoding in the ConnX BBE32EP DSP comes in the form of two optimized operations:

* BBE\_DSPRMCNRNX16CS8 - 8-Codeset 16-Way 16-bit Real Coded Multiply and Reduce for Linear Block Decoding with No Rotation
* BBE\_DSPRMCANX16CS8 - 8-Codeset 16-Way 16-bit Real Coded Multiply, Reduction and Accumulate for Linear Block Decoding with No Rotation

Both operations operate on 16-bit real data and perform a 16-way multiplication based on a vector of 1-bit codes. The operations support up to eight such sets of 1-bit code vectors. The ISA HTML for these two operations has more information on how a linear block decode operation is set up.

During a linear block decoding operation, reduction-add’s between intermediate results are performed in full precision (32-bits wide) for each of the eight code-words. This is designed considering algorithms needs and hardware optimization. The result after a reduction-add is then sign-extended to 40-bits.

For the accumulating version of a linear block decoding operation, the accumulation is performed only after truncating the 40-bit results to 32-bits first, and then after the accumulation, the 32-bit sums are sign-extended back to 40-bit results.

## 5.6 1D Despread

*This option provides support to correlate a complex-binary vector against a complex 16i+16q or 8i+8q vector. This is particularly useful in despreading operations in 3G standards.*

The despread and descramble functions are needed at different sections of the 3G (WCDMA) receiver chain; these are listed below:

• 1D/2D single/multi-code 16-bit complex despread functions are used for S-SCH inner/ outer code correlations, for coarse frequency-offset estimation (based on P-SCH correlation) as well as coarse channel estimation (based on CPICH correlation) and for CCPCH channels dispreading (through 1D single-code 16-bit complex despread function)

Scrambling by itself, in 3G, only involves complex multiplication and since we usually need to work with a single (primary) scrambling code (as most channels are scrambled using a single scrambling code), 1D single-code type function is required for the descrambling operation.

De-spread function also includes reduction-add following multiplication.

ConnX BBE32EP DSP supports 1D despreading of 16-bit real or 8/16-bit complex input data. The 1D despread operations perform N-way element-wise signed, vector multiplication of real/complex inputs with a vector of real/complex codes at full precision. If n is the spreading factor (SF), n consecutive products are consecutively added (2, 4, 8 or 16). A vector of N/n wide outputs, correspondingly real/complex, is produced every cycle. Say for SF=4, N-way real data and real codes

FOR i in {0...N/n} re\_res[i] = SUM(j=0...n, re\_data[i\*4+j] \* re\_code[i\*4+j])

Inputs to the despreading operation are a Nx16-bit vector of real data to be multiplied by the codes, a Nx16-bit vector of code sets containing sixteen Nx1b codes, immediate(s) that select(s) the code-sets to be processed each issue & the type of code. The accumulating versions of the despreading operations allow accumulating the reduction-add results with previous results into the output wide vector at 40-bit precision to obtain SF beyond 16 .

![](data:image/png;base64...) **Note:** Types of codes can be {1, -1} for real or {+/-1, +/-j, +/-1+/-j} for complex

The ISA support to accelerate applications performing 1D despreading comes in the form of the following special operations:

* BBE\_DSPR1DANX16CSF8 - 8-way 16-bit Complex Coded Multiply, Reduction and Accumulate for Despreading with spreading factor 8
* BBE\_DSPR1DANX16SF16 - 16-way 16-bit Real Coded Multiply, Reduction and Accumulate for Despreading with spreading factor 16
* BBE\_DSPR1DANX8CSF16 - 16-way 8-bit Complex Coded Multiply, Reduction and Accumulate for Despreading with spreading factor 16
* BBE\_DSPR1DNX16CSF4 - 8-way 16-bit Complex Coded Multiply and Reduction for Despreading with spreading factor 4
* BBE\_DSPR1DNX16CSF8 - 8-way 16-bit Complex Coded Multiply and Reduction for Despreading with spreading factor 8
* BBE\_DSPR1DNX16SF16 - 16-way 16-bit Real Coded Multiply and Reduction for Despreading with spreading factor 16
* BBE\_DSPR1DNX16SF4 - 16-way 16-bit Real Coded Multiply and Reduction for Despreading with spreading factor 4
* BBE\_DSPR1DNX16SF8 - 16-way 16-bit Real Coded Multiply and Reduction for Despreading with spreading factor 8
* BBE\_DSPR1DNX8CSF16 - 16-way 8-bit Complex Coded Multiply and Reduction for Despreading with spreading factor 16
* BBE\_DSPR1DNX8CSF4 - 16-way 8-bit Complex Coded Multiply and Reduction for Despreading with spreading factor 4
* BBE\_DSPR1DNX8CSF8 - 16-way 8-bit Complex Coded Multiply and Reduction for Despreading with spreading factor 8

These operations are a part of the 1D despread option configurable in the ConnX BBE32EP DSP. Refer to the operation's ISA HTML page for more details on its implementation and setup.

### Table 12: Decoding Complex Codes for Despreading

|  |  |  |  |  |
| --- | --- | --- | --- | --- |
| **Encoding (2b)** | **Code-type (1b immediate)** | **Decoded value** | **re\_code (1b)** | **im\_code (1b)** |
| 00 | 0 | 1+j | 1 | 1 |
| 01 | 0 | -1+j | -1 | 1 |
| 10 | 0 | 1-j | 1 | -1 |
| 11 | 0 | -1-j | -1 | -1 |
| 00 | 1 | 1 | 1 | 0 |
| 01 | 1 | -j | 0 | -1 |
| 10 | 1 | -1 | -1 | 0 |
| 11 | 1 | j | 0 | -1 |

## 5.7 Soft-bit Demapping

*This option supports up to 256 QAM soft-bit demapping.*

The soft-bit demapping operations are used to convert soft-symbol estimates, outputs of an equalizer, into soft bit estimates, or log-likelihood ratios (LLRs), later to be processed by a soft channel decoder for error correction and detection. The soft-bit demapper typically sits at the interface between complex and soft-bit domains.

The soft-bit demapper accepts as inputs complex-valued soft-symbol estimates x in addition to a scaling factor. Given these inputs, for each bit bi, it calculates the log-likelihood ratio

![](data:image/jpeg;base64...)

according to the mapping of bits to a constellation S. The LLR calculation uses a Max-Log approximation and assumes an unbiased symbol estimate with zero-mean additive white

Gaussian noise (AWGN), i.e. x=s+w where s belongs to S and w is AWGN. Therefore, the SDMAP output is given by

![](data:image/jpeg;base64...)

The scaling factor is used to account for signal-to-noise ratio and any other desired weighting adjustments. Users can negate the LLR values with an additional sign option.

Supported constellations and mappings are summarized in *Table 13: Set of Symbol Constellations Supported* on page 83. Symbol mappings for 3GPP and WiFi use different Gray Encoding formats, both supported by the soft-bit demapper operations.

### Table 13: Set of Symbol Constellations Supported

|  |  |  |  |
| --- | --- | --- | --- |
| **Standard** | **Supported**  **Constellations** | **Gray Encoding** | **Output to Soft Decoding** |
| 3GPP | QPSK  16-QAM  64-QAM | 3GPP | Turbo |
| WiFi (IEEE 802.11) | QPSK  16-QAM  64-QAM  256-QAM | IEEE | Convolutional or LDPC |

ConnX BBE32EP DSP implementation covers cases of 4/16/64/256-QAM soft-demodulation

(BBE\_SDMAP256QAMNX16C, BBE\_SDMAP64QAMNX16C, BBE\_SDMAP16QAMNX16C, BBE\_SDMAPQPSKNX16C).

Inputs to the ConnX BBE32EP DSP soft-bit demap operations are

* Complex constellation points
* Assumed Q5.10 (16 bit resolution), not normalized
* Scale factors per point: 4-bit mantissa and 4-bit exponent in paired vector elements
* Used for SNR and channel weighting adjustments
* Three immediate values to select between various modes
* Pick upper or lower half of input complex vector to operate
* Optionally negate soft-bit LLRs
* Optionally interleave output soft-bit LLRs for real/imaginary parts (IEEE vs. 3GPP standard)

And, as for the outputs of the ConnX BBE32EP DSP soft-bit demap operations,

* Up to 2N soft-bits per half complex vector are computed each cycle, scaled by the scaling factors, with rounding and saturation to 8-bit integer resolution at output.

Scaling before the soft demapper is needed to place onto an integer grid (assumed hardware implementation Q5.10 format). Scaling after the soft-demodulation is optionally applied by the operations.

## 5.8 Comparison of Divide Related Options

ISA support for divide related functions in the ConnX BBE32EP DSP is offered as three configurable options:

* Fast vector reciprocal & reciprocal square root
* Advanced vector reciprocal & reciprocal square root
* Vector divide

Each option provides multiple operations (by adding more hardware) to the ConnX BBE32EP

DSP ISA to accelerate a specific divide related function. *Table 14: Comparison of Divide Related Options* on page 84) briefly outlines the intended use for each operation along with any functional overlap with other operation(s).

For some insight into the usage of above operations, refer to the packaged code example **vector\_divide** (computes 16b by 16b signed vector division) in several different ways.

### Table 14: Comparison of Divide Related Options

|  |  |  |  |  |
| --- | --- | --- | --- | --- |
| ***Class*** | ***Operations*** | ***Function*** | ***Overlaps functionally with*** | ***Configurable***  ***Option*** |
| Advanced  Vector  Reciprocal | BBE\_RECIPLUNX40\_0 | High precision reciprocal approximation: ~23b mantissa accuracy | 32b/16b divide and  BBE\_FPRECIP | Advanced  Vector  Reciprocal &  Reciprocal |
|  |  |  |
|  | BBE\_RECIPLUNX40\_1 | High precision reciprocal approximation: ~23b mantissa accuracy | 32b/16b divide and  BBE\_FPRECIP | Square Root |
| Advanced  Vector  Reciprocal  Square  Root | BBE\_RSQRTLUNX40\_0 | High precision reciprocal square root approximation: ~24b mantissa accuracy | BBE\_FPRSQRT |
| BBE\_RSQRTLUNX40\_1 | High precision reciprocal square root approximation: ~24b mantissa accuracy | BBE\_FPRSQRT |
| Fast Vector  Reciprocal | BBE\_RECIPUNX16\_0 | Unsigned integer reciprocal approximation: results with approximately 15b precision | 16b/16b unsigned divide | Fast Vector  Reciprocal &  Reciprocal  Square Root |
| BBE\_RECIPUNX16\_1 | Unsigned integer reciprocal approximation: results with | 16b/16b unsigned divide |

|  |  |  |  |  |
| --- | --- | --- | --- | --- |
|  |  | approximately 15b precision |  |  |
| BBE\_RECIPNX16\_0 | Signed integer reciprocal approximation: results with approximately 15b precision | 16b/16b signed divide |
| BBE\_RECIPNX16\_1 | Signed integer reciprocal approximation: results with approximately 15b precision | 16b/16b signed divide |
| BBE\_FPRECIPNX16\_0 | Signed floating point reciprocal approximation: results with approximately 10b precision in the typical case, 7.5b in the worst case. | 16b/6b signed divide, 32b/16b divide and advanced precision recip |
| BBE\_FPRECIPNX16\_1 | Signed floating point reciprocal approximation: results with approximately 10b precision in the typical case, 7.5b in the worst case. | 16b/6b signed divide, 32b/16b divide and advanced precision recip |
| BBE\_DIVADJNX16 | Signed divide adjust to allow C-exact 16b/16b integer divide - works with  BBE\_RECIPNX16\* | 16b/16b signed divide |
| BBE\_DIVUADJNX16 | Unsigned divide adjust to allow C-exact 16b/16b integer divide | 16b/16b unsigned divide |
| Fast Vector  Reciprocal  Square  Root | BBE\_FPRSQRTNX16\_0 | Signed floating point reciprocal square root approximation: results with approximately 9.7b precision in the typical case, 7.5b in the worst case. | High precision reciprocal square root |
| BBE\_FPRSQRTNX16\_1 | Signed floating point eciprocol square root approximation: results with approximately 9.7b precision in the typical case, 7.5b in the worst case. | High precision reciprocal square root |
| 16-bit  Vector  Divide First Step | BBE\_DIVNX16S\_5STEP0\_0 | Signed 16b/16b vector divide | BBE\_RECIPNX16 | Vector Divide |
| BBE\_DIVNX16S\_5STEP0\_1 | Signed 16b/16b vector divide | BBE\_RECIPNX16 |

|  |  |  |  |  |
| --- | --- | --- | --- | --- |
|  | BBE\_DIVNX16U\_4STEP0\_0 | Unsigned 16b/16b vector divide | BBE\_RECIPUNX16 |  |
| BBE\_DIVNX16U\_4STEP0\_1 | Unsigned 16b/16b vector divide | BBE\_RECIPUNX16 |
| BBE\_DIVNX16Q\_4STEP0\_0 | Unsigned fractional  16b/16b vector divide | BBE\_FPRECIPNX16 |
| BBE\_DIVNX16Q\_4STEP0\_1 | Unsigned fractional  16b/16b vector divide | BBE\_FPRECIPNX16 |
| Vector  Divide -  Other Steps | BBE\_DIVNX16S\_3STEPN\_0 | Last step of 32b/16b and 16b/16b signed vector divide | BBE\_RECIPNX16 |
| BBE\_DIVNX16S\_3STEPN\_1 | Last step of 32b/16b and 16b/16b signed vector divide | BBE\_RECIPNX16 |
| BBE\_DIVNX16S\_4STEP\_0 | Middle step of 32b/16b and 16b/16b signed vector divide | BBE\_RECIPUNX16,  high precision reciprocal approximation |
| BBE\_DIVNX16S\_4STEP\_1 | Middle step of 32b/16b and 16b/16b signed vector divide | BBE\_RECIPUNX16,  high precision reciprocal approximation |
| BBE\_DIVNX16U\_4STEP\_0 | Middle step of 32b/16b and 16b/16b unsigned vector divide | BBE\_RECIPUNX16,  high precision reciprocal approximation |
| BBE\_DIVNX16U\_4STEP\_1 | Middle step of 32b/16b and 16b/16b unsigned vector divide | BBE\_RECIPUNX16,  high precision reciprocal approximation |
| BBE\_DIVNX16U\_4STEPN\_0 | Last step of 32b/16b and 16b/16b unsigned vector divide | BBE\_RECIPUNX16,  high precision reciprocal approximation |
| BBE\_DIVNX16U\_4STEPN\_1 | Last step of 32b/16b and 16b/16b unsigned vector divide | BBE\_RECIPUNX16,  high precision reciprocal approximation |
| 32-bit  Vector  Divide First Step | BBE\_DIVNX32S\_5STEP0\_0 | Signed 32b/16b vector divide | High precision reciprocal approximation |
| BBE\_DIVNX32S\_5STEP0\_1 | Signed 32b/16b vector divide | High precision reciprocal approximation |
|  | BBE\_DIVNX32U\_4STEP0\_0 | Unsigned 32b/16b vector divide | BBE\_RECIPUNX16 |  |
| BBE\_DIVNX32U\_4STEP0\_1 | Unsigned 32b/16b vector divide | BBE\_RECIPUNX16 |

![](data:image/png;base64...)**Note:** It is recommended to use protos that combine operation sequences appropriate for the type. The packaged code examples may be used as a reference to identify such protos.

### *5.8.1 Vector Divide*

*For 16-bit/16-bit and 32-bit/16-bit vector division with 16-bit outputs.*

The support for 32-bit integer or scalar divide is an option available to supplement the base Xtensa ISA and turning it on upon configuring a ConnX BBE32EP DSP provides users with the following protos:

* Signed integer divide – QUOS() and REMS()
* Unsigned integer divide – QUOU() and REMU()

More significantly, the ConnX BBE32EP DSP can be configured with a vector divide option. Low-precision vector division operations in the ConnX BBE32EP DSP are multiple cycle operations and are executed stepwise – four step operations for N/2 results of 16-bit precision and each step taking one cycle. These steps are pipelined by interleaving even and odd elements of the N-element vector. For convenience of programming, two sets of these four steps, one set each for even and odd elements are bundled into an intrinsic that produces N results of vector division in 8 cycles.

This option also supports high-precision division often useful for dividing arbitrary 16-bit fixedpoint formats by appropriately shifting high-precision dividends. These high-precision vector division operations differ in that the dividend vector is 32-bits/element although stored in a 40-bit/element guarded wvec register.

On inclusion of this option in a ConnX BBE32EP DSP configuration, the core supports the following types of vector division:

* 16-bit by 16-bit unsigned vector divide
* It takes a set of sixteen 16-bit unsigned dividends and sixteen 16-bit unsigned divisors from the vec register file and produces sixteen unsigned quotients and sixteen unsigned remainders, both with 16-bit precision and zero-extended to 16-bits per SIMD element.
* Overflow and divide-by-zero conditions return 0x7FFF.
* Protos used - BBE\_DIVNX16U() / BBE\_DIVNX16U()
* 16-bit by 16-bit signed vector divide
* It takes a set of sixteen 16-bit signed dividends and sixteen 16-bit signed divisors from the vec register file and produces sixteen signed quotients and sixteen signed remainders, both with 16-bit precision and sign-extended to 16-bits per SIMD element.
* Overflow returns 0x7FFF and underflow returns 0x8000 for negative. Similarly, for divide-by-zero, the operation returns either 0x7FFF or 0x8000 appropriately.
* Protos used - BBE\_DIVNX16() / BBE\_DIVNX16()
* 32 -bit by 16-bit unsigned vector divide
* It takes a set of sixteen 32-bit dividends from the wvec register file and sixteen 16-bit unsigned divisors from the vec register file and produces sixteen unsigned quotients and sixteen unsigned remainders, both with 16-bit precision and zero-extended to 16bits per SIMD element.
* Overflow and divide-by-zero conditions return 0x7FFF.
* Protos used - BBE\_DIVNX32U() / BBE\_DIVNX32U()
* 32-bit by 16-bit signed vector divide
* It takes a set of sixteen 32-bit signed dividends from the wvec register file and sixteen 16-bit signed divisors from the vec register file and produces sixteen signed quotients and sixteen signed remainders, both with 16-bit precision and sign-extended to 16-bits per SIMD element.
* Overflow returns 0x7FFF and underflow returns 0x8000 for negative. Similarly, for divide-by-zero, the operation returns either 0x7FFF or 0x8000 appropriately.
* Protos used - BBE\_DIVNX32() / BBE\_DIVNX32()

If users need to write scalar code with the use of scalar data-types like xb\_int16, xb \_int16U, xb \_int40, xb \_c16 and xb \_c40, the Xtensa compiler can auto-vectorize the divide operations in the code by using the following set of ‘compiler-assist’ protos:

* Unsigned scalar divide – BBE\_DIV16U() and BBE\_DIV32U()
* Signed scalar divide – BBE\_DIV16() and BBE\_DIV32()

Refer to the description section of the various divide operations in the ISA HTML for further understanding of stepwise divide operations. Additionally, the vector\_divide example included in the ConnX BBE32EP DSP installation illustrates a sample usage of this option using a 16bit by 16-bit signed vector divide example.

### *5.8.2 Fast Vector Reciprocal & Reciprocal Square Root*

*Reduced cycle counts for 16-bit operations as compared to the vector divide option.*

Refer to the ISA HTML of the following operations for more details on implementation and setup.

* Fast vector reciprocal
* BBE\_RECIPUNX16\_0 - Unsigned 16-bit reciprocal approximation on even elements
* BBE\_RECIPUNX16\_1 - Unsigned 16-bit reciprocal approximation on odd elements
* BBE\_RECIPNX16\_0 - Signed 16-bit reciprocal approximation on even elements
* BBE\_RECIPNX16\_1 - Signed 16-bit reciprocal approximation on odd elements
* BBE\_FPRECIPNX16\_0 - Signed 16-bit + 7-bit pseudo-floating point reciprocal approximation on even elements
* BBE\_FPRECIPNX16\_1 - Signed 16-bit + 7-bit pseudo-floating point reciprocal approximation on odd elements
* BBE\_DIVADJNX16 - Compute adjustment for correction of 16b signed divide based on fast reciprocal
* BBE\_DIVUADJNX16 - Compute adjustment for correction of 16b unsigned divide based on fast reciprocal
* Fast vector reciprocal square root
* BBE\_FPRSQRTNX16\_0 - 16-bit mantissa + 7b exponent pseudo-floating point reciprocal square-root approximation on even elements
* BBE\_FPRSQRTNX16\_1 - 16-bit mantissa + 7b exponent pseudo-floating point reciprocal square-root approximation on odd elements

For more insight into the usage of above operations, refer to the packaged code example **vector\_recip\_fast** (computes reciprocal of input vector using the BBE\_FPRECIP operations) and **vector\_rsqrt\_fast** (computes reciprocal sqrt of input vector using the BBE\_FPRSQRT operations)

### *5.8.3 Advanced Vector Reciprocal & Reciprocal Square Root*

*Increased precision compared to the Fast Vector Reciprocal & Reciprocal Square Root option. Inputs and outputs are generally 16-bit fixed although intermediate results can be stored in higher precision with extra effort.*

The advanced precision - 40-bit mantissa, 7-bit exponent - reciprocal (RECIP) and reciprocal square root (RSQRT) options provide operations that compute lookup table based terms to support Taylor's series expansion. The actual expansion may be performed using the core MAC operations. These options are recommended when more than 16-bit precision is required, particularly for inversions of ill-conditioned matrices. The throughput of this option set is about two 32-bit results per cycle. This estimate also includes the relevant MAC and normalization operations needed in the sequence.

The input to these operations needs to be normalized first. And, the operations compute either even or odd elements within Nx16 or Nx40 vectors. To hide the three cycle latency, they may be issued in pairs and software pipelined. Overflow or saturation conditions resulting from zero valued input elements are designated with the most negative representable number at the 40-bit wide output.

The Taylor's series approximation of y(x) is given as:

y = f(x0) + (x-x0)\*f'(x0) + (x-x0)^2\*f"(x0)/2 = A + (x-x0)\*(B+(x-x0)\*C)

We use lookup tables to approximate A B, and C with x0 being the low end of segment range. The RECIP and RSQRT functions compute N-way 32-bit outputs in multiple steps.

1. Start by finding normalization amounts and then normalize wide precision inputs. Use

BBE\_NSANX40 for RECIP, and BBE\_NSAENX40 for RSQRT, and apply BBE\_SLLNX40 to both.

1. Next, use BBE\_RECIPLUNX40\_{0,1} / BBE\_RSQRTLUNX40\_{0,1} even-odd pairs appropriately. These generate a wide output approximation for term A along with scaled (x-x0) inputs and an adjusted slope term B=(x-x0)\*C. Use the signed MAC operation

BBE\_MULUSANX16 for RECIP and the unsigned MAC operation BBE\_MULUUSNX16 for RSQRT to complete the rest of the Tayalor's series expansion as shown above.

1. Now, to obtain a floating-point output format, use BBE\_PACKVNX40 to pack the Nx40 values into Nx16, and use the normalization amounts to find the appropriate output exponent.
2. Or, to obtain fixed-point output format with desired denormalization into a wide vector register, use BBE\_SRSNX40 with an appropriate shift amount.

In the default use case, the input is assumed to be Q39 - in the range between 0.5 and 1.0 and the output after MAC sequence without any shifts will be Q1.38 - greater than equal to 1.0 but under 2.0. Here's a code sequence illustrating this use case of ConnX BBE32EP DSP RSQRT operations along with the said data formats of its inputs and outputs:

|  |
| --- |
| xb\_vecNx40 inpw, inpnormw; // inputs xb\_vecNx40 a ; // to hold the a's from lookups xb\_vecNx16 inp\_scaled; // to hold scaled input xb\_vecNx16 b; // to hold b's from lookup vsaN norm, renorm; // normalization amounts    // inpw in Q39 format norm = BBE\_NSAENX40(inpw);  inpnormw = BBE\_SLLNX40(inpw,norm);  BBE\_RSQRTLUNX40\_0(a, inp\_scaled, b, inpnormw); BBE\_RSQRTLUNX40\_1(a, inp\_scaled, b, inpnormw);  BBE\_MULUUSNX16(a, inp\_scaled, b);  // a containts mantissa of rsqrt output in Q1.38 format  // Q1.38 format => there is one sign bit, one integer bit, and 38 fractional bits renorm = BBE\_SUBSR1SAVSN(0,norm);  // Output exp = -norm/2  // BBE\_SUBSR1SAVSN(0,norm) calculates 0 - (norm/2) |

If the input data is not in Q39 format, it first needs to be converted to Q39 since the operation requires it. The same shift amount may be used to compute the output exponent.

For ConnX BBE32EP DSP RECIP, the input format is assumed to be normalized Q39. The sequence of operations involved in implementing the Taylor's series approximation will output a result in Q2.37 format. Here's a similar example illustrating the code sequence:

xb\_vecNx40 inpw, inpnormw; // inputs xb\_vecNx40 a ; // to hold the a's from lookups xb\_vecNx16 inp\_scaled; // to hold scaled input xb\_vecNx16 b; // to hold b's from lookup vsaN norm, renorm; // normalization amounts

norm = BBE\_NSANX40(inpw); inpnormw = BBE\_SLLNX40(inpw,norm);

BBE\_RECIPLUNX40\_0(a, inp\_scaled, b, inpnormw);

BBE\_RECIPLUNX40\_1(a, inp\_scaled, b, inpnormw);

BBE\_MULUSANX16(a, b, inp\_scaled); renorm = BBE\_SUBSAVSN(0,norm); // output mantissa in a in Q2.37, output exponent in renorm

Refer to the ISA HTML of the following operations for more details on implementation and setup.

* Advanced precision vector reciprocal (RECIP)
* BBE\_RECIPLUNX40\_0 - Compute normalization and table lookup factors for advanced reciprocal approximation - even elements (...,4,2,0)
* BBE\_RECIPLUNX40\_1 - Compute normalization and table lookup factors for advanced reciprocal approximation - odd elements (...,5,3,1)
* Advanced precision vector reciprocal square root (RSQRT)
* BBE\_RSQRTLUNX40\_0 - Compute normalization and table lookup factors for advanced reciprocal square root approximation - even elements (...,4,2,0)
* BBE\_RSQRTLUNX40\_1 - Compute normalization and table lookup factors for advanced reciprocal square root approximation - odd elements (...,5,3,1)

## 5.9 Advanced Precision Multiply/Add

*This option supports 23-bit (16b mantissa, 7b exponent), and 32-bit fixed point multiplication and addition. Inputs and outputs are 16-bit/32-bit fixed precision although intermediate results can be stored at a higher precision.*

The ConnX BBE32EP DSP advanced precision multiply/add is a configurable option that enables two new classes of data representation and related computations:

* 23-bit floating point - 16-bit mantissa and 7-bit exponent
* 32-bit high precision fixed point

The expect MAC performance of the 23-bit floating point is 1.5 times the cycle cost of 16-bit fixed point. And, the expected MAC performance of 32-bit fixed point is 3 times the cycle cost of 16-bit fixed point.

### 23-bit Floating Point

The mantissas are held in Nx16-bit vector registers (vec) and represent normalized signed Q15 values. For complex numbers, at least one element of the real-imaginary pair is normalized. The exponents are held in Nx7-bit vsa registers (vsa) and denote the amount of right shift needed to convert the mantissa back to fixed Q15 value. The legitimate range of values is -64<exponent<63. The special values 63 and -64 represents zero and BIG NaN respectively. BBE\_FLUSH\_TO\_ZERO state is set when an resulting exponent is 63 (designating a zero). By convention, complex numbers have a common exponent.

### 32-bit Fixed Point

All the 32-bit fixed point data is held in wide vector registers (wvec), or in some cases in a pair of narrow vector registers (vec) as low and high 16-bit parts. The related operations use

16-bit multiply hardware and helper operations to emulate 32-bit multiplication with appropriate shifting and component gathering/accumulation. The intermediate multiply results are stored into new 2-entry Nx32-bit register file (mvec), then shifted appropriately and accumulated into regular Nx40-bit wide vector registers (wvec).

Decoupling of advanced precision multiply/add operations allow multiply-shift-accumulate sequences to be folded into parallel slots with a pipeline depth of 2-3 stages.

*Table 15: Advanced Precision Multiply/Add Operations Overview* on page 92 presents an overview of the various classes of ConnX BBE32EP DSP operations added by the advanced precision multiply/add option.

### Table 15: Advanced Precision Multiply/Add Operations Overview

|  |  |  |
| --- | --- | --- |
| **Operation Type** | **Operations** | **Description** |
| Data organization | BBE\_MOVMVNX32,  BBE\_MOVSVWXL, BBE\_MOVSVWXH,  BBE\_MOVSWVX;  BBE\_UNPKNVNX16,  BBE\_UNPKVENX16,  BBE\_PACKNVNX40,  BBE\_FPPACKNVNX40;  BBE\_FPCVTCRN | Move between vec/wvec/mvec  register files; packing/unpacking from/to wvec; convert real to complex formats |
| Multiply, add, subtract, pack | BBE\_MULUUMNX16,  BBE\_MULUSMNX16,  BBE\_MULMNX16,  BBE\_MULUSMNX16C,  BBE\_MULMNX16C,  BBE\_MULUSMNX16J,  BBE\_MULSUMNX16J,  BBE\_MULMNX16J;  BBE\_FPPACKNX40,  BBE\_FPPACKNX40C,  BBE\_FPNORMNX40,  BBE\_FPNORMNX40C;  BBE\_FPADDNX16,  BBE\_FPSUBNX16 | Signed/unsigned real/complex multiplies into mvec; pack/normalize denormalized values after multiplies; add/subtract vec floating point into wvec |
| Shift, accumulate, normalize | BBE\_SRAMNX40,  BBE\_SRAMADDNX40,  BBE\_SRAMSUBNX40, | Shift and accumulate mvec to wvec with possible rounding; add/subtract |
| **Operation Type** | **Operations** | **Description** |
|  | BBE\_SRAIMNX40,  BBE\_SRAIMRNX40,  BBE\_RNDIMNX40,  BBE\_SRAIMADDNX40,  BBE\_SRAIMADDRNX40,  BBE\_SRAIMSUBNX40,  BBE\_SRAIMSUBRNX40,  BBE\_SRAIWADDMNX40,  BBE\_SRAIWSUBMNX40,  BBE\_SLLIMNX40,  BBE\_SLLIMADDNX40,  BBE\_SLLIMSUBNX40;  BBE\_FPADDMNX40,  BBE\_FPSUBMNX40,  BBE\_FPADDMNX40C,  BBE\_FPSUBMNX40C,  BBE\_FPADDVNX40C,  BBE\_FPSUBVNX40C,  BBE\_FPSUBRVNX40C;  BBE\_NSAZNX16,  BBE\_NSAZNX16C,  BBE\_NSANZNX40,  BBE\_NSANZNX40C,  BBE\_NSANRZNX40,  BBE\_NSANRZNX40C | mvec or vec floating point; find vec/ wvec normalization amount |
| Exponent | BBE\_ADDSAVSN, BBE\_SUBSAVSN,  BBE\_SUBSR1SAVSN,  BBE\_ADDSVSN, BBE\_ADDSTVSN,  BBE\_SUBSVSN; BBE\_MAXVSN,  BBE\_MINVSN, BBE\_SLS1VSN,  BBE\_SRA1VSN, BBE\_SHFLVSNI,  BBE\_SELVSNI,  BBE\_SUBSRA1SVSN,  BBE\_ABSSUBSVSN,  BBE\_DIFFOFFSVSN,  BBE\_UNCPRSVSNC,  BBE\_CPRSVSNC\_0,  BBE\_CPRSVSNC\_1,  BBE\_UNCPRSVSN, BBE\_CPRSVSN | Add, subtract, shift, shuffle,  compare, compress, uncompress exponent values in vsa register |

The following brief examples show the usage of some of the advanced precision multiply/add operations.

### Real floating point addition

Two floating point vectors added. Operation BBE\_FPADDNX16 appropriately aligns mantissas and adjust exponents for proper sum. The result is in 40-bit wvec then packed to output 16-bit mantissa and new exponent using operation BBE\_FPPACKNX40.

|  |
| --- |
| xb\_vecNx16 x0\_mant, y0\_mant, z\_mant; vsaN x0\_exp, y0\_exp, w\_exp, z\_exp; xb\_vecNx40 w\_mant;  BBE\_FPADDNX16(w\_mant, w\_exp, x0\_mant, x0\_exp, y0\_mant, y0\_exp); // add floating point z\_exp = w\_exp;  BBE\_FPPACKNX40(z\_mant, z\_exp, w\_mant, 0); // pack/normalize, update exponents |

### Complex floating point multiply-accumulate

Two sets of products of floating point vectors accumulated. Intermediate products into 32-bit mvec, accumulation into 40-bit wvec using operation BBE\_FPADDMNX40C, then pack with operation BBE\_FPPACKNX40.

|  |
| --- |
| xb\_vecNx16 x0\_mant, y0\_mant, x1\_mant, y1\_mant, z\_mant; vsaN x0\_exp, y0\_exp, w\_exp, x1\_exp, y1\_exp, z\_exp, acc\_exp; xb\_vecNx40 acc\_mant = BBE\_MULNX16C(x0\_mant, y0\_mant); // 1st mantissa set acc\_exp = BBE\_ADDSTVSN(x0\_exp, y0\_exp); // 1st exponent set xb\_mvecNx32 m1\_mant = BBE\_MULMNX16C(x1\_mant, y1\_mant); // 2nd mantissa vsaN m1\_exp = BBE\_ADDSTVSN(x1\_exp, y1\_exp); // 2nd exponent  BBE\_FPADDMNX40C(acc\_mant, acc\_exp, m1\_mant, m1\_exp); // accumulate products z\_exp=acc\_exp;  BBE\_FPPACKNX40(z\_mant, z\_exp, acc\_mant, 0); // normalize pack and update exponents |

### Complex high precision fixed point multiply-accumulate(keep top 32 bits of full precision 64-bit result)

Move each 32-bit number into low/high 16-bit parts. Use appropriate singed/unsigned multiply to compute low/high, high/low, high/high products and accumulate with operations BBE\_SRAIMNX40 with appropriate shifts.

|  |
| --- |
| xb\_vecNx40 x0, y0, x1, y1, wvec; xb\_vecNx16 xl,xh,yl,yh;  // 1st set of complex multiplies xl=BBE\_MOVSVWXL(x0); xh=BBE\_MOVSVWXH(x0); // move saturated 32-bit into low/high 16-bit yl=BBE\_MOVSVWXL(y0); yh=BBE\_MOVSVWXH(y0); // move saturated 32-bit into low/high 16-bit xb\_mvecNx32 mlh = BBE\_MULUSMNX16C(yl, xh); // complex low/high multiply wvec = BBE\_SRAIMNX40(mlh, 15); // accumulate with proper shift adjustment xb\_mvecNx32 mhl = BBE\_MULUSMNX16C(xl,yh); // high/low  BBE\_SRAIMADDNX40(wvec, mhl, 15);  xb\_mvecNx32 mh = BBE\_MULMNX16C(yh, xh); // high/high product BBE\_SRAIMADDNX40(wvec, mh, 0);  // 2nd set of complex multiplies to accumulate |
| xl=BBE\_MOVSVWXL(x1); xh=BBE\_MOVSVWXH(x1); yl=BBE\_MOVSVWXL(y1); yh=BBE\_MOVSVWXH(y1); mlh = BBE\_MULUSMNX16C(yl, xh); //low/low  BBE\_SRAIMADDNX40(wvec, mlh, 15);  mhl = BBE\_MULUSMNX16C(xl, yh); // high/low  BBE\_SRAIMADDNX40(wvec, mhl, 15); mh = BBE\_MULMNX16C(yh,xh); // high/high  BBE\_SRAIMADDNX40(wvec, mh, 0); |

## 5.10 Inverse Log-likelihood Ratio (LLR)

*This options is used to reciprocate the soft-bit demapping operation. It converts a set of LLR values to the mean and variance of the corresponding complex N-QAM symbol. This is useful in SIC and turbo equalization type of operations.*

**Turbo Equalization**: Turbo equalizer is based on the Turbo principle and approaches the performance of MAP (Maximum aposteriori) receiver via iterative message passing between a soft-in soft-out (SISO) equalizer and a SISO decoder. This technique is used in MIMOOFDM and CDMA systems including 3GPP LTE, WCDMA/HSPA, DVB etc. The decoder used is the SISO Turbo decoder, whereas the equalizer algorithm can be LMMSE equalizer using a-priori information based on feedback from Turbo Decoder. This technique can also be used in conjunction with SIC (Sequential interfence cancelation) & PIC(Parallel interfence cancelation).

The inverse LLR calculation converts the extrinsic LLR values output by the Turbo decoder to mean and variance values which can be used by the next iteration of the MMSE algorithm as shown below. The mean and variance calculation needs to be performed per symbol.

![](data:image/jpeg;base64...)

### Figure 6: Inverse LLR Calculation

ConnX BBE32EP DSP includes optional operations to facilitate efficient estimation of softcomplex QAM symbols from log-likelihood ratios (LLR). This process uses a lookup table for the computation of non-linear hyperbolic tangent (tanh). Also, different operations are needed for each size of supported constellations (4/16/64/256-QAM).

The inputs, to the BBE\_INVLLRNX16C operation, represent LLR values for a vector of N/2 complex, QAM symbols. For example, 6 such LLRs are needed for each 64-QAM complex symbol - 3 for real part and 3 for imaginary part.

* Immediate-value inputs select whether the sequence of LLR values, per complex point, are to be considered interleaved (3GGP) or not (IEEE), and whether or not they should be negated before processing further
* Input LLR values are packed in bytes, however, only the 6 LSBs are used in a Q2.3 format

Output of the operation is a vector of N-bit probabilities, in Q5.10 format, computed after a 11-bit lookup table approximation of *tanh* nonlinearity. The bit-probabilities for each complex element are interleaved as real-imaginary pairs. A sequence of operations is needed to obtain all bit-probabilities for any given QAM case, for example 256-QAM requires four issues to obtain the four pairs of bit probabilities. An immediate allows obtaining the value (2-bit probability) for a certain bit to accelerate the estimation process.

Operation BBE\_INVLLRNX16C computes bit probabilities as the *tanh* of input LLRs for different QAM sizes. Once the bit probabilities are available, the mean and variance of the complex constellation point can be estimated using regular ConnX BBE32EP DSP MAC operations.

1. Denote pi the bit probabilities at the output. Then the complex mean is given by
   * 256-QAM mean - real = p0\*(8-p1\*(4-p2\*(2-p3))) & imag = p4\*(8-p5\*(4p6\*(2-p7)))
   * 64-QAM mean - real = p0\*(4-p1\*(2-p2)) & imag = p3\*(4-p4\*(2-p5))
   * 16-QAM mean - real = p0\*(2-p1) & imag = p2\*(2-p3)
   * 4-QAM mean - real = p0 and imag = p1
2. The real-imaginary parts for each bit probability pi are interleaved at the output so that helps in computing SIMD-way the quantities above for N/2 complex element vectors
3. Start with (2-pi) quantity in high QAM cases and successively real multiply-add the next bit probability N/2-way to compute the complex mean as equations dictate
   * For (2-pi) use special immediate setting to enable the factor 2, otherwise disable to produce only the pi value

Some iterative equalization methods require both the mean and variance of the complex constellation point computed (i.e. Turbo equalization). Variance is estimated applying a similar approach and the use of real MAC operations, mean, and bit probabilities already computed above. For example, for 64-QAM, with mean denoted as mean64,

64-QAM variance = 26+(2-p2)\*(4-8p1) + (2-p5)\*(4-8p4) - (mean64)^2

## 5.11 Single and Dual Peak Search

Single and dual peak (value and its index) search of 16/32-bit, signed/unsigned, real/complex inputs. In baseband, this option accelerates peak search of sequences resulting from real and complex correlation processes, for example to detect frame alignment during acquisition, or extracting frequency/phase correction.

The peak search configuration option is based on operations optimized in the use of 32-bit SIMD comparators in conjunction with some helper operations to extract up to two peaks and their indices. The 16-bit peak search operations internally sign extend inputs to 32 bits for use with the 32-bit comparators. In both 32-bit single peak search and 16/32-bit dual peak search, the operations process N input elements per issue. But, with 16-bit single peak search, the 2N elements are processed per issue.

Several state registers are added upon configuring a ConnX BBE32EP DSP with the peak search option in order to not increase register usage.

* BBE\_MAX and BBE\_MAX2 are 256-bit [SIMD] states added to hold first and second peak values respectively.
* BBE\_MAXIDX and BBE\_MAXIDX2 are 96-bit [SIMD] states added to hold first and second peak indices respectively. The indices here are stored with relative SIMD lane offsets and are converted into actual index values at the final stage.
* BBE\_IDX is a 11-bit state added to hold count of the number of peak search passes. This is internally used inside BBE\_MAXIDX/BBE\_MAXIDX2 to compute relative offsets to indices of peaks.

Use model of these peak search operations is a multi-step process involving a setup (initialization), followed by operations that compare, aggregate and extract peaks and values from intermediate vectors. Let's go through the flow:

1. Initialize all index-states with zero and value-states with -infinity using

BBE\_SETDUALMAX().

1. Use one of the following operations to process the loaded vector input, and appropriately save any single/dual peaks and their respective indices into the aforementioned states.
   * For 16-bit signed/unsigned single peak search, use BBE\_DMAX[U]NX16()
   * For 16-bit signed/unsigned dual peak search, use BBE\_DUALMAX[U]NX16()
   * For 32-bit signed/unsigned single/dual peak search, use BBE\_DUALMAX[U]WNX32()
2. Next, for 16-bit single peak search use BBE\_GTMAXNX16() and BBE\_MOVDUALMAXT(), and for all other cases use only BBE\_MOVDUALMAXT() to aggregate peaks and indices held in two states into a single vector.
3. Finally, BBE\_RBDUALMAXR() and BBE\_SELMAXIDX() are used to extract the single/dual peaks and their indices from the single vector obtained in the previous step.

Typically, in implementing complex correlation functions, BBE\_MAGINX16C() is used to compute magnitudes of complex values. This output data of this operation is interleaved. To account for this data reordering, a 1-bit immediate passed to BBE\_SELMAXIDX controls the index value computation of the absolute maximum value; see ISA HTML for more details. Optionally, a right shift may be performed when using BBE\_DUALMAX{U}WNx32() to avoid any potential input overflow. An example code of 32-bit complex dual peak search is provided below:

|  |
| --- |
| void bbe\_dualpeak\_mag\_32b(int16\_t \* data\_in, uint32\_t \* \_\_restrict peaks, int16\_t \*\_\_restrict indices, int32\_t size) {  xb\_vecNx16 \* \_\_restrict data\_ptr = (xb\_vecNx16 \*)data\_in; int32\_t j; int32\_t size\_N = (size/XCHAL\_BBEN\_SIMD\_WIDTH) ; uint32\_t maxpeak; uint32\_t secondpeak; vboolN selector, selector2; BBE\_SETDUALMAX(0); // initialize all dual peak state registers  // Find power of complex data and search for dual peaks/indexes for(j=0;j<size\_N;j=j+1) { xb\_vecNx16 vec0,vec1; xb\_vecNx40 wvec0;  /\* process two complex vectors \*/ vec0 = data\_ptr[2\*j]; |
| vec1 = data\_ptr[2\*j+1]; wvec0 = BBE\_MAGINX16C(vec1,vec0); // this interleaves data, maxindex has to deinterleave below  BBE\_DUALMAXUWNX32(wvec0, 0);  }  // get the maximum and lane flag for max  BBE\_RBDUALMAXUR(maxpeak,selector);  // get corresponding index, note deinterleaving index mode int32\_t maxindex = BBE\_SELMAXIDX(selector,1);  // replace max with max2 for max lane  BBE\_MOVDUALMAXT(selector);  // get the maximum and lane flag for max2  BBE\_RBDUALMAXUR(secondpeak,selector2);  // get corresponding index for max2, deinterleaving mode int32\_t secondindex = BBE\_SELMAXIDX(selector2,1);  peaks[0] = maxpeak; peaks[1] = secondpeak ; // output dual peaks indices[0] = maxindex;  indices[1] = secondindex ; // output dual indexes  } |

Lastly, the table below provides a stage-wise overview of the different use cases supported and the set of operations involved in each of them.

### Table 16: Use cases of peak search operations

|  |  |  |  |  |
| --- | --- | --- | --- | --- |
| **Single peak search** | **SIMD vector search** | **Merge values/ indices vectors** | **Peak/index extraction** |  |
| 16b signed/unsigned | DMAX{U}NX16 | GTMAXNX16 and  MOVDUALMAXT | RBDUALMAXR and  SELMAXIDX |  |
| 32b signed/unsigned | DUALMAX{U}WNX32 | N/A | RBDUALMAXR and  SELMAXIDX |  |
| **Dual peak search** | **SIMD vector search** | **First peak/index extraction** | **Merge values/ indices vectors** | **Second peak/index extraction** |
| 16b signed/unsigned | DUALMAX{U}NX16 | RBDUALMAXR and  SELMAXIDX | MOVDUALMAXT | RBDUALMAXR and  SELMAXIDX |
| 32b signed/unsigned | DUALMAX{U}WNX32 | RBDUALMAXR and  SELMAXIDX | MOVDUALMAXT | RBDUALMAXR and  SELMAXIDX |

## 5.12 Single-precision Vector Floating-Point

Refer to *ConnX BBE32EP DSP Floating-Point Operations*

# 6. Floating-Point Operations

**Topics:**

* *ConnX BBE32EP DSP*

*Floating-Point Features*

* *ConnX BBE32EP DSP*

*Floating-Point Operations*

* *ConnX BBE32EP DSP*

*Floating-Point*

*Programming*

* *Accuracy and*

*Robustness*

*Optimizations on ConnX*

*BBE32EP DSP*

* *Implementing the Floating Point FFT/IFFT on ConnX*

*BBE32EP DSP Vector*

*Floating Point Unit*

*(VFPU)*

* *Floating Point References*

The ConnX BBE32EP DSP is a single instruction multiple data (SIMD) machine. Multiple data is represented with a 256-bit-wide vector register file. Each 256-bit register can represent either 8, 32-bit float data, 4, 64-bit complex float, 4, 64-bit double data, or 2, 128-bit complex double. The ConnX BBE32EP DSP offers 8 copies of single precision hardware to handle either 8 float data or 4 complex float data, with 1 single operation. Float, complex float are available as scalar and vector data-types. For scalar data-types, the ConnX BBE32EP DSP deploys floating-point hardware on data lane 0 and operates on the data of the same data lane. For vector data-types, the ConnX BBE32EP DSP deploys all the floating-point hardware, on all the data lanes. Vector operations are either non-predicated or predicated-true.

This portion of the User’s Guide introduces programmers to the ConnX BBE32EP DSP floating-point operations. Floating-point operations implement the IEEE 754 standards [IEEE], except where noted. *The ConnX BBE32EP DSP Floating-Point Features* section specifies

Half, Single precision formats, Error in Unit in the Last Place (ULP), Signaling and Quiet Not a Number (NaN). This section also advises on how to avoid Signaling NaN (sNaN).

The *Floating-Point Operations* section categorizes floating-point operations according to their functions and details their behaviors, to assist programmers to better utilize these operations. These operations are generally applicable to all combinations of available precision(s), real and complex data-types, as well as scalar and vector.

The *Floating-Point Programming* section is a programming guide on floating-point, including examples of exception-causing operations. Programmers will find essential information on efficient floating-point programming and accurate floating-point computations, without relying on exception or status flags. This section also recommends on the Exception and compiler switches.

The *Accuracy and Robustness Optimizations* on ConnX BBE32EP DSP section clarifies some common misconceptions on floating-point, and gives particular details of accuracy measurements. This section intends to aid programmers on numerical computations especially when an accumulated error of a few Units in the Last Place (ULP) matters. This section also provides references on selecting algorithms and precisions. Some algorithms are more robust, being able to handle a wider domain of input values. All references, algorithms, or examples discussed in this section are provided for explanatory purposes only. Thus, there is no implication that these references, algorithms, or examples are supported or qualified by Cadence.

The *Implementing Floating Point FFT/IFFT* on ConnX BBE32EP DSP VFPU section explains the implementation of FFT/IFFTs of size L=2n on BBENEPVFPU. It provides a description of Radix-4 FFT butterfly implementation along with an example. This section also documents implementation notes and optimization techniques for the Radix-4 FFT.

## 6.1 ConnX BBE32EP DSP Floating-Point Features

### *ConnX BBE32EP DSP Register Files Related to Floating-Point*

ConnX BBE32EP DSP floating-point operations share the same vec registers as the integer operations. These operations share the same load, store, move, select, shuffle and vbool operations with the fixed-point operations. Predicated floating-point operations read vbool registers. Floating-point control and status access operations use AR registers. No other register files are used.

### *ConnX BBE32EP DSP Architecture Behavior Related to Floating-Point*

Floating-point operations may signal five types of exceptions, specified by the IEEE 754 standards *[IEEE]:*

#### • Invalid Exception

An Invalid exception is signaled if the result is invalid and cannot be defined. For example, ADD(+∞, -∞) signals an Invalid Exception since there is no usefully definable result. It returns Not a Number (NaN) since returning a number will mislead follow-up operations.

#### • Division by Zero Exception

Division by Zero exception is signaled if an exact infinite result is produced by an operation on finite operands.

Note: DIV ( ±0.0, ±0.0) is equivalent to MUL ( ±0.0, ±∞), and is an invalid operation. It is not a Division by Zero operation.

#### • Overflow Exception

Overflow exception is signaled if the the rounded result is larger in magnitude than the largest finite number representable in the destination format.

#### • Underflow Exception

Underflow exception is signaled when both tinyness is detected and inexact exception is signaled. The ConnX BBE32EP DSP detects tinyness after rounding, as though the exponent range was unbounded. The ConnX BBE32EP DSP acknowledges tinyness only when the computed result is non-zero and with a magnitude smaller than the minimum normal number. Refer to *Binary Floating-point Values* for the definition of normal numbers.

#### • Inexact Exception

Inexact exception is signaled if the rounded result is different from the exact computed result if both exponent range and precision were unbounded.

Each type of exception always sets a corresponding exception/status flag. Invalid Flag, Division by Zero Flag, Overflow Flag, Underflow Flag, and Inexact Flag are all sticky. The flags remain as set (logic 1) when the exception occurs, until explicitly cleared by the user.

Programmers may enable or disable any number of these floating-point exceptions to generate an Arithmetic Exception signal.

The Arithmetic Exception signal is an external output pin that may optionally signal certain software operating conditions.

It is not a trap for alternate handling as recommended by the IEEE 754 standards *[IEEE]*.

### *Binary Floating-point Values*

The ConnX BBE32EP DSP supports binary floating-point numbers in hardware. A binary floating-point value is a bit-string with three components: a sign, a biased exponent, and a significand. The sign and significand together form a sign-magnitude representation, and the biased exponent allows negative and positive exponents. The significand consists of one implicit leading bit to the left of its implied binary point, and many explicit trailing bits to the right of the binary point.

***Signed Zero Number***

The number is a signed zero when both the biased exponent and the significand are zero.

#### *Subnormal Number*

The number is a subnormal when the biased exponent is zero, the significand is nonzero, and the implicit leading bit of significand (also known as hidden bit) is 0. A subnormal number is also known as denorm. The components of a Binary Floating Point Value are shown in a tabular form below:

#### Table 17: Components of a Binary Floating-point Value

|  |  |  |
| --- | --- | --- |
| **Sign** | **Biased Exponent** | **Significand Field** |

##### Normal Number

The number is a normal when the biased exponent is non-zero, the significand is zero or non-zero, and the implicit leading bit of significand (also known as hidden bit) is 1.

***Biased and Unbiased Exponents***

#### When the biased exponent is all 0’s, The unbiased exponent is equal to (1 - a bias)

Otherwise

The unbiased exponent is the biased exponent - bias.

|  |  |
| --- | --- |
| **When the biased exponent is all 1’s,** | The floating-point represent either infinity or Not a Number (NaN) as described in *Encodings of Infinity and Not a Number (NaN).* |
| **When the biased exponent is not all 1’s** | The floating-point numerical value is the signed product of its significand and 2 raised to the power of its unbiased exponent. |

### *Half Precision Data*

The half precision representation conforms to the binary16 format defined by the IEEE 754 standards. In half precision, the sign is 1 bit. The biased exponent is 5 bits. The bias is 0xf. The significand consists of 1 implicit leading bit to the left of its implied binary point, and 10 explicit trailing bits to the right of the binary point.

Half precision is supported with conversion operations. For applications which are bounded by storage size or by memory bandwidth, data can be computed in single precision but stored in half precision, by using conversion operations. These conversion operations are described in *Floating-point Conversion Operations*.

#### Table 18: Half Precision Data

|  |  |  |  |
| --- | --- | --- | --- |
| **Field** | **From** | **To** | **Length** |
| Sign | 15 | 15 | 1 |
| Biased Exponent | 14 | 10 | 5 |
| Significand Field | 9 | 0 | 10 |

### *Single Precision Data*

The single precision representation conforms to the single or binary32 format defined by the IEEE 754 standards. In single precision, the sign is one bit. The biased exponent is eight bits. The bias is 0x7f. The significand consists of 1 implicit leading bit to the left of its implied binary point, and 23 explicit trailing bits to the right of the binary point.

Single precision is supported with all operations, described in the “*ConnX BBE32EP DSP Floating-Point Operations*” chapter.

#### Table 19: Single Precision Data

|  |  |  |  |
| --- | --- | --- | --- |
| **Field** | **From** | **To** | **Length** |
| Sign | 31 | 31 | 1 |
| Biased Exponent | 30 | 23 | 8 |
| Significand Field | 22 | 0 | 23 |

### *Maximum Possible Error in the Unit in the Last Place (ULP)*

When measured against “correctly rounded” results as specified by the IEEE 754 standards, the following operations generate zero errors in all rounding modes: Arithmetic Operations, Floating-point Conversion Operations, Integer to/from Float Conversion Operations, Float to Integral Value Rounding Operations, and Float Division and Square Root Operations. All these operations are described in *ConnX BBE32EP DSP Floating-Point Operations*.

When measured against an infinitely precise result, these operations may generate an error which may vary from a rounding mode to another. The infinitely precise result is derived from two mathematical calculations; with real numbers instead of floating numbers, or with floating numbers of unbounded range and unbounded precision. An error occurs if the infinitely precise result is unpresentable in a destination format and rounding is necessary to fit the format. A default rounding mode is available after reset, to round to the nearest representable number, and generates an error of [0.0, 0.5] unit in the last place (ULP) in the destination format. Additional rounding modes are also available, to round to various directions, and generates an error of [0.0, 1.0) ULP . All available rounding modes are described in *Floating-Point Rounding and Exception Controls*.

This User’s Guide defines the error as the maximum absolute distance between the infinitely precise result and the operation result given all legal operands. This User’s Guide measures the error with the Unit in the Last Place, also known as ULP, of the destination format.

### *Encodings of Infinity and Not a Number (NaN)*

In all binary float-point formats, the value is a bit-string with three components: a sign, a biased exponent, and a significand. The significand consists of one implicit leading bit to the left of its implied binary point, and many explicit trailing bits to the right of the binary point. The explicit trailing bits of significand is also known as the significand field.

When the biased exponent is all 1’s, and the significand field is all 0’s, the numerical value is either positive infinity or negative infinity depending on the sign bit.

When the biased exponent is all 1’s, and the significand field is non-zero, the floating-point is

Not a Number (NaN). There are two types of NaNs: Signaling Not a Number (sNaN), and Quiet Not a Number (qNaN). sNaN provides representations for uninitialized variables, with the most significant bit (MSB) of the significand field being 0. qNaN provides retrospective diagnostic information inherited from invalid or unavailable data and results, with the MSB of the significand field being 1. The retrospective diagnostic information in the significand field is called the payload. The IEEE 754 standards do not interpret the sign bit of a NaN.

Floating-point instructions can only generate qNaN, when at least one operand causes an operation to be invalid. No floating-point instructions generate sNaN. All floating-point instructions support both sNaN and qNaN, conforming to the IEEE 754 standards.

#### Table 20: Encodings of Infinity and Not a Number (NaN)

|  |  |  |  |
| --- | --- | --- | --- |
| **Encoding** | **Sign** | **Biased Exponent** | **Significand Field** |
| +∞ | 0 | All 1's | All 0's |
| -∞ | 1 | All 1's | All 0's |
| qNaN | 0 or 1 | All 1's | 1 as the MSB, anything as the rest of bits |
| sNaN | 0 or 1 | All 1's | 0 as the MSB, non zero as the rest of bits |

Throughout this User’s Guide, the term NaN include both sNaN and qNaN. For completeness, this User’s Guide provides information pertaining both sNaN and qNaN, even though a well coded program should never read an uninitialized variable and therefore could never encounter sNaN.

### *Scalar Float Data Types Mapped to the Vector Register File*

The ConnX BBE32EP DSP supports both real float and complex float types.

The real float types supported are:

* **float** - A 32-bit single precision value stored in a 32-bit vector register element.

The complex types supported are:

* **complex float**- A complex single precision value, with a 32-bit imaginary part in float format and a 32-bit real part in float format. The real and imaginary pair is stored in two

32-bit elements of a vector register file, with the real part in the lower significant element.

#### Table 21: Complex Single Precision Value

|  |  |  |  |
| --- | --- | --- | --- |
| **Field** | **From** | **To** | **Length** |
| Imaginary Part | 63 | 32 | 32 |
| Real Part | 31 | 0 | 32 |

### *Vector Float Data Types Mapped to the Vector Register File*

Following is a list of the vector register files and their corresponding vector data types.

#### Table 22: Vector Float/Double Data Types Mapped to the Vector Register File

|  |  |  |
| --- | --- | --- |
| **Register Vector** | **Single Precision** | **Double Precision** |
| vec Registers | xb\_vecMxf32 | xb\_vecM2xf64 |
| vec Registers | xb\_vecM2xcf32 |  |

### *Floating-Point Data Typing*

The above data type naming convention applies to both operations and protos (Cprogramming prototypes for operations). Alternatively, intrinsics are provided to give the same functionality of each operation mapped appropriately to the set of data types.

The ConnX BBE32EP DSP supports standard C floating-point data types, referred to as memory data types, for 32-bit float, 64-bit complex float. These data-elements are directly mapped to the ConnX BBE32EP DSP vec registers as register data types.

### *Floating-Point Rounding and Exception Controls*

Programmers may access a Floating-point Control Register (FCR), to read/set rounding mode, and/or to enable/disable any of the five floating-point exceptions as a source of Arithmetic Exception .

The Arithmetic Exception is an external output pin that may optionally signal certain software operating conditions. It is not a trap for alternate handling as recommended by the IEEE 754 standards *[IEEE]*.

The ConnX BBE32EP DSP maintains neither an order nor a count of exceptions.

The enabling or disabling of any exception as a source of Arithmetic Exception changes neither the result nor the floating-point status/exception flags obligated by the operation to complete.

The five floating-point exceptions are described in *ConnX BBE32EP DSP Architecture Behavior Related to Floating-Point*.

RUR.FCR is the instruction for reading the FCR. WUR.FCR is the instruction for writing FCR. FCR is initialized to 0 at reset. Accessing FCR does not trigger Arithmetic Exception .

FCR[1:0] is rounding the control (also known as RM). A value of 0 is rounding to the nearest tied to even. 1, round toward 0. 2, toward positive infinity. 3, toward negative infinity.

FCR[2] is Inexact Exception Enable. FCR[3] is Underflow Exception Enable. FCR[4] is

Overflow Exception Enable. FCR[5] is Division by Zero Exception Enable. FCR[6] is Invalid Exception Enable. A value of 0 disables the corresponding exception and 1 enables correspoding exception.

All other bits are read as 0, and ignored on write. Refer to table below:

#### Table 23: FCR Fields

|  |  |
| --- | --- |
| **Bits** | ***State/unused*** |
| 1:0 | RM |
| 2 | Inexact Exception Enable |
| 3 | Underflow Exception Enable |
| **Bits** | ***State/unused*** |
| 4 | Overflow Exception Enable |
| 5 | Divide by Zero Exception Enable |
| 6 | Invalid Exception Enable |
| 11:7 | Ignore |
| 31:12 | MBZ |

#### Table 24: FCR Fields and Meaning

|  |  |
| --- | --- |
| **FCR Fields** | **Meaning** |
| RM | 0-> round to the nearest tied to even |
|  | 1-> round towards 0 (TRUNC) |
|  | 2-> round towards +Infinity (CEIL) |
|  | 3-> round towards -Infinity (FLOOR) |
| AE | Bits[6:2] are for Arithmetic Exception control bits. |
| MBZ | Reads as 0, Must be written with zeros. |
| Ignore | Reads as 0, ignored on write. Allows a value also containing FSR bits to be written. |

### *Floating-Point Status/Exception Flags*

Programmers may access a Floating-point Status Register (FSR), to read/set/clear each floating-point status/exception flags. Regardless of the state of the FCR, the FSR always records five types of exceptions when detected, as the default response in the non-trapping situations specified by the IEEE 754 standards. The five floating-point exceptions are described in *ConnX BBE32EP DSP Architecture Behavior Related to Floating-Point*.

RUR.FSR is the instruction for reading FSR. WUR.FSR is the instruction for writing FSR. FSR is initialized to 0 at reset.

Accessing FSR does not trigger Arithmetic Exception .

FSR[7] is Inexact Flag. FSR[8] is Underflow Flag. FSR[9] is Overflow Flag. FSR[10] is Division by Zero Flag. FSR[11] is Invalid Flag. A value of 0 indicates that the corresponding floating exception has not occurred. Each flag is sticky and remains as logic 1 once the corresponding exception occurs, until being explicitly cleared.

All other bits are read as 0, and ignored on write.

#### Table 25: FSR Fields

|  |  |
| --- | --- |
| **Bits** | **State/unused** |
| 6:0 | Ignore |
| 7 | InexactFlag |
| 8 | UnderflowFlag |
| 9 | OverflowFlag |
| 10 | DivZeroFlag |
| 11 | InvalidFlag |
| 12:31 | MBZ |

#### Table 26: FSR Fields and Meaning

|  |  |
| --- | --- |
| **FSR Field** | **Meaning** |
| MBZ | Reads as 0, Must be written with zeros |
| Ignore | Reads as 0, ignored on write. Allows a value also containing FCR bits to be written. |

## 6.2 ConnX BBE32EP DSP Floating-Point Operations

The ConnX BBE32EP DSP floating point operations include instructions for essential arithmetic, format conversions, (unsigned) integer to/from float conversions, float to integral value rounding, comparisons and min/max, as well as division and square root instruction sequences. These operations conform to the IEEE 754 standards. Programmers may refer to the IEEE 754 standards for detailed behaviors in terms of exception signaling, status/ exception flag setting, rounding control, as well as treatments of infinity, Not a Number (NaN) and signed zero.

Additional instructions and instruction sequences are available for complex arithmetic, as well as for reciprocal, reciprocal square root, and fast square root. These additional instructions and sequences are optimized for speed but not for accuracy. Though not mandated by the IEEE 754 standards, these operations are provided for optimizing common floating-point applications.

All floating point operations are available for both scalar and vector data-types. Vector operations perform like their scalar counterparts on each predicated-true data lanes. Vectors operations logically OR all the same exceptions from each predicated-true data lanes, to generate the final exception of the operations. For example, all Invalid exceptions from each predicated-true data lanes are OR'ed together as the Invalid exception.

### *Arithmetic Operations*

The ConnX BBE32EP DSP supports arithmetic operations on scalar and vector. These arithmetic operations include absolute (ABS), negation (NEG), addition (ADD), subtraction (SUB), multiplication (MUL), fused-multiplication-addition (MADD), fused-multiplicationsubtraction (MSUB), and parallel add-sub (ADDSUB).

ABS and NEG return exact results, and never signal any exceptions even for sNaN. All other operations normalize an infinitely precise result, round the normalized result according to a currently-set rounding mode in the FCR, and may signal Invalid, Overflow, Underflow and/or Inexact exception(s), conforming to the IEEE 754 standards.

Each of these operations has multiple protos, each of which supports a specific data-type related to the operation. Refer to the ISA HTML of an operation to find all the protos using that operation.

### *Floating-point Conversion Operations*

The ConnX BBE32EP DSP supports conversion operations, including half precision to/from single precision conversions, on scalar and vector. Half-to-Single conversion operations are named CVTF32\_F16. Single-to-Half conversion operations are named CVTF16\_F32. All the conversions from a narrower data-type to a wider one are exact, requiring no rounding, and signaling no exceptions except Invalid exception when the input is sNaN. The conversions from a wider data-type to a narrower one may not be exact, may require rounding to fit into the narrower data-type, and may signal exceptions. Any required rounding is performed according to a currently-set rounding mode in the FCR. The conversions from a wider datatype to a narrower one signal Invalid exception when the input is sNaN; otherwise may signal Overflow, Underflow and/or Inexact flag(s) when such situation(s) occur, conforming to the IEEE 754 standards.

Each of these operations has multiple protos, each of which supports a specific data-type related to the operation. Refer to the ISA HTML of an operation to find all the protos using that operation.

### *Integer to/from Float Conversion Operations*

The ConnX BBE32EP DSP supports conversion operations, including integer to/from float conversions, unsigned integer to/from float conversions, on scalar and vector.

Integer-to-Single conversion operations are named FLOAT.S or FLOATF32. Unsigned-toSingle conversion operations are named UFLOAT.S or UFLOATF32. When the number of significant bits in the (unsigned) integer is less than those available in the significand of the destination float format, the conversions are exact, requiring no rounding, and signal no exceptions. Otherwise, the conversions may not be exact, may require rounding to fit into the narrower significand, and may signal Inexact exception. Any required rounding is performed according to a currently-set rounding mode in the FCR, and Inexact exception is signaled when the conversion is not exact.

Single-to-Integer conversion operations are named TRUNC.S and TRUNC32. Single-toUnsigned conversion operations are named UTRUNC.S and TRUNCU32. All these conversions may not be exact. If rounding is required, these conversions always round to zero or truncate any fraction, regardless of the rounding mode. When the truncated value cannot be represented by the destination type, the operations deliver predetermined constants and signal Invalid exception only. The *Xtensa ISA Reference Manual* defines these constants. When the truncated value can be represented but is not exact, the operations deliver the rounded results and signal Inexact exception only.

Each of these operations has multiple protos, each of which supports a specific data-type related to the operation. Refer to the ISA-HTML of an operation to find all the protos using that operation.

### *Float to Integral Value Rounding Operations*

The ConnX BBE32EP DSP supports integral value rounding operations, including single to integral single rounding, on scalar and vector. Most of these rounding operations do not depend on a currently-set rounding mode in the FCR.

Float-to-Integral rounding operations include FICEIL, FIFLOOR, FIRINT, FIROUND, and FITRUNC. FICEIL operations round the float toward positive infinity, but maintain the original float format. FIFLOOR operations round the float toward negative infinity, but maintain the original float format. FIRINT operations round the float according to a currently-set rounding mode in the FCR, but maintain the original float format. FIROUND operations round the float to the nearest integral value, but round halfway cases away from zero (instead of to the nearest even integral value), and maintain the original float format. FITRUNC operations round the float toward zero, but maintain the original float format. All these rounding operations signal Invalid exception for sNaN input. No operations except FIRINT additionally signal Inexact exception for not exact, conforming to the IEEE 754 standards.

Each of these operations has multiple protos, each of which supports a specific data-type related to the operation. Refer to the ISA HTML of an operation to find all the protos using that operation.

### *Classification, Comparison, and Min/Max Operations*

The ConnX BBE32EP DSP supports comparison as well as minimum and maximum operations, on scalar and vector. These comparison operations include 2 types of comparisons, namely Ordered Comparisons and Unordered Comparisons. Ordered Comparisons return true when neither operand is a NaN and comparison is valid; otherwise return false. Unordered Comparisons return true when either operand is a NaN or comparison is valid; otherwise return false.

Ordered Comparison operations include OEQ, OLE, and OLT. Unordered Comparison operations include ULEQ, ULTQ, UNEQ and UN. Here, “EQ” is equal. “LE” is less or equal. “LT” is less than. “NEQ” is not equal. “UN” is unordered. Positive zero is compared equal to negative zero, conforming to the IEEE 754 standards.

All comparison operations signal Invalid exception when either operand is a sNaN. OLE, OLT, ULEQ and ULTQ additionally signal the Invalid exception when either operand is a qNaN.

Additionally, MIN and MAX operations are available. MIN implements “(a<b)?a:b”; while MAX implements “(a>b)?a:b”. Here, both “<” and “>” are considered as ordered comparisons, just like OLT(a,b) and OLT(b,a) respectively. Both MIN and MAX signal Invalid exception when either operand is a sNaN or qNaN.

Furthermore, MINNUM and MAXNUM operations are available. MINNUM implements “fmin” in C; while MAXNUM implements “fmax” in C. Both MINNUM and MAXNUM return a number whenever at least one operand is a number. Both MINNUM and MAXNUM signal Invalid exception when either operand is a sNaN.

The ConnX BBE32EP DSP also supports non-computational classification operations: CLSFY on scalar and vector. CLSFY never raises any exceptions, even for sNaN. CLSFY conforms to IEEE 754-2008 “class ( )” operation.

CLSFY tells a class of the input operand, by returning an 8-bit enum representation. In binary format, the 8-bit enum is

{isFinite, isSignaling, isNaN, isInfinite, isNormal, isSubnormal, isZero, isSignMinus}

where, isFinite is the MSB.

* isFinite is 1 if and only if the operand is normal, subnormal, or zero.
* isSignaling is 1 if and only if the operand is a sNaN.
* isNaN is 1 if and only if the operand is either a sNaN or a qNaN.
* isInfinite is 1 if and only if the operand is either +inf or –inf.
* isNormal is 1 if and only if the operand is ±normal.
* isSubnormal is 1 if and only if the operand is ±subnormal.
* isZero is 1 if and only if the operand is ±0.
* isSignMinus is 1 if and only if the operand has negative sign.
* isSignMinus applies to zeros and NaNs as well.
* isNaN, isInfinite, isNormal, isSubnormal, and isZero are exactly one-hot.

Each of these operations has multiple protos, each of which supports a specific data-type related to the operation. Refer to the ISA HTML of an operation to find all the protos using that operation

### *Division and Square Root Operations*

The ConnX BBE32EP DSP supports division and square root operations with instruction sequences, on scalar and vector. These division and square root instruction sequences emulate infinitely precise results, round the infinitely precise results according to a currentlyset rounding mode in the FCR, generate correctly rounded results, and always signal any exception(s) according to the IEEE 754 standards. These instruction sequences utilize fusedmultiplication-addition operations to conform to accuracy obligations specified by the IEEE 754 standards. The -mfused-madd or -mno-fused-madd compilation switches do not affect these instruction sequences.

Programmers may instantiate division and square root instruction sequences by using intrinsics, or may compile “/”, “sqrtf()” with -fno-reciprocal-math. The division intrinsics include DIV.S. The square root intrinsics include SQRT.S. Here “.S” indicates single precision. All these operations normalize an infinitely precise result, round the normalized result according to a currently-set rounding mode in the FCR, and signal Invalid, Division by Zero, Overflow, Underflow and/or Inexact exception(s), conforming to the IEEE 754 standards.

Each of these operations has multiple protos, each of which supports a specific data-type related to the operation. Refer to the ISA HTML of an operation to find all the protos using that operation.

### *Complex Arithmetic Operations*

The ConnX BBE32EP DSP supports complex arithmetic operations, on scalar and vector. Some of these operations are implemented with one single instruction; while the others are implemented with instruction sequences. These complex arithmetic operations are not specified by the IEEE 754 standards, and some of these operations only accept a subset of all possible floating-point values.

The complex single arithmetic operations include CONJCF32, ADDCF32, SUBCF32,

ADDSUBCF32, MULCF32, MULACF32, MULSCF32, and DIVCF32. Here, “CF32” indicates complex single. “CONJ” is conjugation. “ADD” is addition. “SUB” is subtraction. “ADDSUB” is addition and subtraction in parallel. “MUL” is multiplication. “MULA” is multiplication followed by addition. “MULS” is multiplication followed by subtraction. “DIV” is division. To utilize these complex arithmetic operations, programmers may instantiate complex arithmetic operations by using intrinsics, or by compiling complex arithmetic operators.

When compiled with -mno-fused-madd, these complex arithmetic operations may not use fused-multiplication-addition instructions. This is preferable when programmers wish to generate results exactly matching popular machines without fused-multiplication-addition hardware.

When compiled with -mfused-madd, these complex arithmetic operations may use fusedmultiplication-addition instructions. This is preferable when programmers wish to generate more accurate results, to minimize code sizes, and/or to shorten computation latencies.

Each of these operations has multiple protos, each of which supports a specific data-type related to the operation. Refer to the ISA HTML of an operation to find all the protos using that operation.

### *Reciprocal, Reciprocal Square Root, and Fast Square Root Operations*

The ConnX BBE32EP DSP supports reciprocal, reciprocal square root, and fast square root operations, on scalar and vector. All these operations are implemented with instruction sequences. For smaller code size and shorter latency, the reciprocal, reciprocal square root, and fast square root instruction sequences generate no infinitely precise results, assumes rounding to the nearest tied to even, may generate results with bigger errors, and may signal false Underflow and/or Inexact exception(s). These instruction sequences utilize fusedmultiplication-addition operations. The -mfused-madd or -mno-fused-madd compilation switches do not affect these instruction sequences. These operations are not mandated by the IEEE 754 standards, and accept a subset of all possible floating-point values.

These operations include RECIP, RSQRT, and FSQRT. Here, “RECIP” is reciprocal. “RSQRT” is reciprocal square root. “FSQRT” is fast square root. To utilize these operations, programmers may instantiate intrinsics, or compile with -freciprocal-math. When compiled with -freciprocal-math, the compiler may turn “/”, “sqrtf()” into reciprocal and multiplication, reciprocal square root and multiplication, or fast square root. When compiled with -fnoreciprocal-math, the compiler will honor any intrinsic instantiations but will not generate any additional reciprocal, reciprocal square root, or fast square root instruction sequences.

Each of these operations has multiple protos, each of which supports a specific data-type related to the operation. Refer to the ISA HTML of an operation to find all the protos using that operation.

### *Notes on Not a Number (NaN) Propagation*

Some floating-point operations have a floating-point datum as an input operand or an output operand, but not both. Some other floating-point operations have both a floating-point input operand, and a floating-point output operand. Most of these floating-point operations, having floating-point data as both input and output operands, propagate a NaN as the output result if an input is a NaN, according to IEEE 754-2008. This propagation assists programmers to trace back to the origin of a numerical exception or NaN, usually an invalid operation such as inf - inf.

However, programmers are reminded not to depend on NaN propagation, payload, or the sign bit, since recompilation may cause the propagation to change or to cease. Additionally, IEEE 754-2008 does specify some floating-point operations, having floating-point data as both input and output operands, that may not propagate a NaN. These operations comprise MINNUM and MAXNUM. In addition, the MIN and MAX operations may or may not propagate a NaN, depending on whether the NaN is the first or the second operand.

## 6.3 ConnX BBE32EP DSP Floating-Point Programming

### *General Guidelines for Floating-Point Programming*

ConnX BBE32EP DSP floating-point operations are accessible in C or C++ by applying standard C operators to standard float types, including float, float complex. Programmers may simply start with legacy C/C++ code or write the floating-point algorithm in a natural style.

All floating operations deliver normalized results with adjusted exponents to maximize the precision. All floating operations are fully pipelined. Some operations may create or propagate NaN to represent invalid or unavailable results. No operations create sNaN.

Some aspects of floating point operations are configurable and are controlled by writing appropriate fields of the floating-point control registers (FCR). These configurable options include the selection of a rounding mode, enabling and disabling floating-point Imprecise Vector Operation Exceptions, and others.

### *Invalid Floating-Point Operation Examples*

Programmers may ignore floating-point status registers (FSR), and rely on any qNaN or predetermined integer values to indicate invalid operations. Invalid operations include ADD(+∞, -∞), SUB (+∞, +∞), MUL( ±0, ±∞), MADD(x, ±0, ±∞), DIV(±0, ±0), DIV( ±∞, ±∞), SQRT(negative non-zero), OLT(x, NaN), and similar operations. Here, x is any floating-point representation. These invalid floating-point operations return qNaN. TRUNC(NaN), TRUNC(±∞), UTRUNC(-1.0), and similar operations are invalid float-to-integer conversions, return predetermined integer numbers since qNaN is not supported in integer types. Upon all the above and similar invalid operations, Invalid exception is signaled and Invalid Flag is raised.

There are other 4 less important flags in FSR, namely: Division by Zero, Overflow, Underflow, and Inexact flags. Each of the 4 flags is always raised once a corresponding exception occurs. In all these 4 exceptions, floating operations continue to commit correctly rounded results. Division by Zero results are correctly signed infinity. Overflow results are correctly signed infinity or maximum normal numbers, depending on a currently-set rounding mode in the FCR. Underflow results are correctly rounded signed zero, subnormal, or minimum normal numbers. Inexact results are correctly rounded signed zero, subnormal or normal numbers, or infinity.

### *Exception, and Exception Reduction*

Programmers can write to floating-point control registers (FCR), with WUR.FCR, to enable Exception based on any floating-point exception(s). Any enabled exception does not stop floating-point operations, and affect neither delivered results nor flag behaviors.

When the Exception is used as an interrupt, programmers may minimize interrupt penalties by enabling only the critical floating point exception(s) such as Invalid, by using operations or macros which cause no or fewer exceptions, and/or by filtering out potential NaN inputs before intensive computations. For example, the normal relation operation “<” may be replaced with the macro “isless”. Alternatively, programmers may handle unordered cases expressively with the source code. For example, “isnan” or other macros may be used to filter out NaN or other undesirable inputs. This kind of optimizations is especially beneficial inside loop bodies.

### *Associative and Distributive Rules, as well as Expression Transformations*

The mathematical associative rules for addition and multiplication, as well as the distributive rule, are not generally valid because of roundoff error, even in the absence of overflow and underflow. Some expressions cannot be transformed into each other, since they are not equivalent. For example, “x/x” is not equivalent to “1.0”, since x can be zero, infinite, or NaN.

Compiled with -fno-associative-math, the compiler neither reorders the operations nor transforms expressions. When the small roundoff error is acceptable and special cases are evaluated, programmers may choose to compile with -fassociative-math, or to optimize programs by manually applying associative, distributive, or transformation rules.

### *Fused-multiply-add (FMA), and Parallel-Add-Sub (ADDSUB)*

Fusing a multiplication and an addition/subtraction into a fused-multiplication-addition/ subtraction is an optimization, which has the potential to affect the floating-point behavior. ConnX BBE32EP DSP provides fused-multiply-add (FMA) hardware, which performs multiplication to infinite precision, adds/subtracts a product to/from an addend, and then finishes with one single rounding on an infinitely precise sum. There is no rounding on the product. The FMA operation is therefore faster and with less rounding error, compared to separate multiplication and addition. In almost all cases, results from FMA differ very slightly, if any, from those of separate multiplication and addition. However, a special case exists when the addend is infinity. For example, 1.0 \* 2120 multiplied by 1.0 \* 297 plus -∞, in single precision. For FMA, 1.0 \* 2120 (0x7b800000) multiplied by 1.0 \* 297(0x70000000) is 1.0 \* 2217. Then, -∞ + 1.0 \* 2217 equals to -∞, which is an accurate result from FMA. However, for separate multiplication and addition, 1.0 \* 2217 overflows and become +∞. Then, -∞ + +∞ becomes qNaN, which is not an accurate result but matching that of popular FPU.

Most commercial floating-point units are without FMA hardware. To match their behavior, programmers may compile with -mno-fused-madd. When compiled with -mno-fused-madd, the compiler may keep multiplication and addition/subtraction separate. When wishing to take advantages of FMA, in terms of execution speed, code size and accuracy, programmers may compile with -mfused-madd. When compiled with -mfused-madd, the compiler may fuse multiplication and addition/subtraction into fused-multiplication-addition/subtraction.

The ConnX BBE32EP DSP provides Parallel-Addition-Subtraction hardware (ADDSUB). The ADDSUB hardware uses one pair of operands to perform both addition and subtraction in parallel, to deliver both sum and difference in a same clock cycle, and to signal any exception(s) by ORing together the same exceptions from both addition and subtraction logics. For example, Invalid exception is identical to the logical OR of Invalid exceptions from both addition and subtraction. Both the sum and the difference are identical to those delivered by separate addition and subtraction respectively.

The ADDSUB hardware accelerates butterfly computations for fast Fourier transforms (FFT) or other calculations where both sum and difference are needed, and saves execution time and power consumptions, without side effects. The compiler automatically combines addition and subtraction into ADDSUB when they share the same pair of operands, regardless of compiler switches. Programmers may pair their additions and subtractions when practical to be automatically combined by the compiler, or may use ADDSUB intrinsic functions.

### *Division, Square Root, Reciprocal, and Reciprocal Square Root Intrinsic Functions*

Division operators, sqrtf() are automatically compiled into division and square root function calls, with -fno-reciprocal-math. When compiled with -freciprocal-math, division operators, sqrtf() may automatically turn into reciprocal, reciprocal square root, and fast square root function calls and maybe with multiplication.

To eliminate the function call overheads, programmers may use the division, square root, reciprocal, reciprocal square root, or fast square root intrinsic explicitly in a few strategic places to replace division operators, sqrtf(). The strategic intrinsic replacements can improve performance very effectively, especially in a loop. The compiler can still be counted on for efficient scheduling, optimization, and automatic selection of load/store instructions.

### *Sources of NaN, and Potential Changes of NaN Sign and Payload*

No floating-point operations generate sNaN. Uninitialized variables should be the only possible source of sNaN. A well coded program should never read uninitialized variables, therefore could never encounter sNaN, and might consider qNaN handlings only.

Invalid floating-point operations, as exemplified in *Invalid Operation Examples*, can generate qNaN. Programmers might avoid qNaN by avoiding invalid operations when possible, might filter out qNaN by using UN or other instructions when practical, or consider qNaN as possible inputs inherited from invalid operations.

Programmers should rely on neither sign nor payload of NaN. Though floating-point operations perform best efforts to generate a default qNaN or to propagate a NaN, the sign and/or the payload of NaN may be altered by compiler optimizations.

### *Auto-Vectorizing Scalar Floating-Point Programs*

When scalar types are used in loops, the compiler will often automatically vectorize and infer or overload. A loop using scalar types may turn into a loop of vector operations that is as tightly packed and efficient as manual code using intrinsics.

The xt-xcc compiler provides several options and methods of analysis to assist in vectorization. These are discussed in more detail in the *Xtensa C and C++ Compiler User’s Guide*.

### *Compiler Switches Related to Floating-point Programs*

The following compiler switches are important for floating-point codes:

-**mfused-madd**: allows compiler to fuse a multiplication and an addition/subtraction into a fused-multiplication-addition/subtraction (FMA). By eliminating a rounding on the intermediate product, the performance and accuracy are both improved. The reduced rounding error, however, may cause a slight result difference if compared to the result from machines without FMA hardware. This switch is off by default at -O2 or lower optimization levels. This switch is on by default at -O3 or higher optimization levels.

-**mno-fused-madd**: disallows compiler to fuse a multiplication and an addition/subtraction into a fused-multiplication-addition/subtraction (FMA). This switch is on by default at -O2 or lower optimization levels. This switch is off by default at -O3 or higher optimization levels.

-**menable-non-exact-imaps**: enables -mfused-madd and other inexact imaps. This switch is off by default at -O2 or lower optimization levels. This switch is on by default at -O3 or higher optimization levels. Please refer to the *Tensilica Instruction Extension (TIE) Language Reference Manual* for information related to imaps.

-**mno-enable-non-exact-imaps**: disables -mfused-madd and other inexact imaps. This switch is on by default at -O2 or lower optimization levels. This switch is off by default at -O3 or higher optimization levels.

-**freciprocal-math**: allows compiler to turn division and square root into reciprocal, reciprocal square root, and fast square root maybe with multiplication. By using reciprocal, reciprocal square root, or fast square root, the performance is improved. However, the input domain is narrowed and the output error is widen. This switch is off by default at -O2 or lower optimization levels. This switch is on by default at -O3 or higher optimization levels.

-**fno-reciprocal-math**: disallows compiler to turn division or square root into reciprocal, reciprocal square root, or fast square root. This switch is on by default at -O2 or lower optimization levels. This switch is off by default at -O3 or higher optimization levels.

-**fassociative-math**: allows compiler to re-associate operands in series of floating-point operations, and to possibly change computation and/or comparison results. This switch is off by default at -O2 or lower optimization levels. This switch is on by default at -O3 or higher optimization levels.

-**fno-associative-math**: disallows compiler to re-associate operands in series of floatingpoint operations. This switch is on by default at -O2 or lower optimization levels. This switch is off by default at -O3 or higher optimization levels.

-**funsafe-math-optimizations**: enables -freciprocal-math and -fassociative-math. This switch is off by default at -O2 or lower optimization levels. This switch is on by default at -O3 or higher optimization levels.

-**fno-unsafe-math-optimizations**: disables -freciprocal-math and -fassociative-math. This switch is on by default at -O2 or lower optimization levels. This switch is off by default at -O3 or higher optimization levels.

Programmers may refer to the *Xtensa C and C++ Compiler User’s Guide* for additional information.

### *Domain Expertise, Vector Data-types, C Instrinsics, and Libraries*

Programmers should exercise their domain expertise when deciding whether to apply any recommendations above, for their specific applications to achieve required performance. Programmers may analyze code performance, by carrying out profiling and performance analysis using the right ISS options. Xtensa Xplorer may assist to determine functions, loops, and loop nests which take most of the cycles. When additional performance is required, programmers may manually vectorize critical functions, loops, or loop nests by using vector data-types, may use C intrinsic functions along with vector data-types, and/or may use libraries. The use of vector data-types, C intrinsics, and libraries is similar to that of fixedpoint. Programmers may refer to fixed-point chapters of this User’s Guide for details.

## 6.4 Accuracy and Robustness Optimizations on ConnX BBE32EP DSP

Programmers may accomplish all their floating-point programming goals, by applying their domain expertise together with information in prior chapters alone. This section provides suggestions concerning highly accurate numeric computations. This section will not duplicate essential information in prior sections, instead will use that as a foundation for further discussion.

From a system prespective, there may be 3 main causes of accuracy issues: measurement accuracy, computation accuracy, and actuator accuracy. For example, a car collision avoidance system may detect another vehicle with a radar, compute a distance and/or a speed with floating-point calculations, and when necessary actuate a brake system in the car. The radar measurement, floating-point computation, and brake actuator may all have their accuracy and/or resolution limitations. Programmers should exercise their domain expertise to account for or otherwise compensate accuracy or resolution issues of measurement and/or actuator(s). This chapter presumes that all measurements are properly compensated to provide accurate inputs and that all actuators are properly compensated to actualize faithfully, and focuses on computation accuracy and robustness.

Programmers should investigate any algorithms, methods, techniques, and references in this chapter for suitability in their specific applications; while Cadence assume no responsibility in any forms for supporting any algorithms, methods, techniques, and references in this chapter.

### *Historical Floating Point Units*

Some observations of historical floating point units are longer valid. Many early floating-point units are without internal representation of infinitely precise results and predated the IEEE 754 standards. Some early studies on these outdated floating-point units report big ranges of errors depending on input operands.

The ConnX BBE32EP DSP FPU emulate infinitely precise results internally, conform to the IEEE 754 standards, and generate correctly rounded results which can be off by an error of [0.0, 0.5] ULP only compared against infinitely precise results given all legal input operands.

### *Comparisons and Measurements against Infinitely Precise Results*

When using comparisons to measure errors, the comparisons should be against infinitely precise results. A first common mistake is to compare against rounded results. A second common mistake is to compare against a different architecture. A third common mistake is to compare using converted results. All these three common mistakes may mischaracterize any FPU, and mislead programmers.

Comparing against rounded results is the first common mistake. Comparing against rounded results may mischaracterize an FPU with fewer errors. For example, an addition operation conforming to the IEEE 754 standards will be with 0 errors, instead of [0.0, 0.5] ULP. Programmers may be mislead to believe an FPU with 0 errors is better than another FPU with 0.5 ULP of error, while these 2 FPU are actually identical.

The second common mistake is to compare against a different architecture. The ConnX BBE32EP DSP FPU differ from many commercial FPU in three architectural characteristics: subnormal supports in hardware, fused-multiplication-addition/subtraction (FMA) in hardware, and uniform float representations. The ConnX BBE32EP DSP FPU support subnormal numbers in hardware to conform to the IEEE 754 standards without latency penalties, while some commercial FPU flush subnormal to zero and do not conform to the standards. The ConnX BBE32EP DSP FPU contains FMA hardware to conform to the IEEE 754 2008 standard with higher accuracy, while most commercial FPUs lack FMA hardware and only conform to the IEEE 754 1985 standard with lower accuracy. The ConnX BBE32EP DSP FPU offers uniform representations of float data-types in both memory and register formats and eliminate a rounding error when moving float data from registers to memory, while some commercial FPU represent float data-types in memory and registers differently and create an extra rounding error when moving float data from registers to memory. Thanks to at least these three architectural advantages, the ConnX BBE32EP DSP FPU better conform to the IEEE 754 standards. Comparing the ConnX BBE32EP DSP FPU against a different architecture with less accuracy may mislead programmers by raising false accuracy issues.

Comparisons in decimal numbers and/or in printed formats may also misguide programmers. The ConnX BBE32EP DSP FPU and most commercial FPU are binary FPU. Programmers should evaluate accuracy in binary, to eliminate binary-to-decimal conversion errors. Evaluations in binary enable ULP calculations, while other evaluations do not.

### *Depending on Rounding Mode vs. Not; Standards Conforming vs. Beyond*

Programmers should distinguish operations depending on rounding mode from other operations independent of the rounding mode. Operations depending on rounding mode may generate 2 different results, 1 ULP away from each other, when rounding mode is changed. When intending to utilize rounding mode to provide different results, programmers may use operations which depend on rounding mode and inherit a currently-set rounding mode in the FCR. Otherwise, programmers may set an intended rounding mode before executing operations which depend on rounding mode.

Additionally, programmers should also distinguish operations conforming to the IEEE 754 standards, and other operations beyond the standards. The operations conforming to the IEEE 754 standards accept all classifications and all possible values of float data-types, and generate correctly rounded results. For application requiring highest possible accuracy, programmers should focus on the operations conforming to the IEEE 754 standards. The operations conforming to the IEEE 754 standards are described in sections of *Arithmetic*

*Operations*, *Floating-point Conversion Operations*, *Integer to/from Float Conversion*

*Operations*, *Float to Integral Value Rounding Operations*, *Comparison and Min/Max Operations*, and *Division and Square Root Operations*, in the chapter of *ConnX BBE32EP DSP Floating-Point Operations*.

*Complex Arithmetic Operations*, and *Reciprocal, Reciprocal Square Root, and Fast Square Root Operations* are beyond the IEEE 754 standards. The ConnX BBE32EP DSP FPU optimizes these operations for execution cycle and code size, but not for accuracy. Programmers should use division operations instead of reciprocal operations for high accuracy applications.

### *Computer Algebra, Algorithm Selections, Default Substitutions, and Exception Flags*

Complex operations are a good example where different algorithms or formulae present various execution cycle, code size, accuracy, and robustness tradeoff points. Complex division described in the *Complex Arithmetic Operations* are optimized for shorter execution cycle, smaller size and with reasonable accuracy, using the following formula:

(a + bi) / (c + di) = (ac + bd) / (c^2 + d^2) + (bc – ad) / (c^2 + d^2) i

This formula is always correct when a, b, c, d are all real numbers. In float data-types, the formula will overflow when c or d is big enough, even though the final result may be still within range. Programmers may evaluate Smith’s method *[Baudin]*, or other improved algorithms, when concerning robustness.

In general, formulae should be evaluated for any spurious behaviors, with signed zero, and signed infinities. Programmers may utilize a computer algebra system (CAS), and/or perform symbolic computation, to enhance mathematical formulae, expressions, or functions.

Programmers may also monitor the exception/status flags closely, as a tool to warn about questionable behaviors. Programmers may investigate any Invalid, Division by Zero, Overflow, and Underflow exceptions in this priority order. These exceptions may cause loss of accuracy. Programmers may solve these exceptions, by selecting algorithms, by rearranging formulae, by recoding functions, or by substituting a default value for what would have been the result of the exceptional operations *[Kahan]*.

### *Error Bounds, Interval Arithmetic, and Rounding Mode*

Programmers may calculate error bounds of mathematical formulae, expressions, and functions. The error bounds may be used to verify numerical correctness. Maximum errors could be subject to formulae weak points around some input values. Average errors could be subject to formulae overall accuracy limitations.

Programmers may take advantage of the ConnX BBE32EP DSP FPU rounding modes, which confirm to the IEEE 754 standards, for interval arithmetic *[Hijazi]*. The interval arithmetic is a powerful approach to bound rounding errors in a mathematical formula, expression, and function. Applying the interval analysis, programmers may consider an output of a mathematical formula to be an interval. The interval is a pair of endpoints. A first endpoint may be generated by executing the formula by rounding toward -∞, and a second endpoint may be generated by executing the same but rounding toward +∞. The interval should contain the exact result of the formula. The span of the interval could be inversely proportional to the accuracy of the formula. If the span is too wide, programmers may optimize the formula, or increase precisions of critical variables inside the formula.

### *FMA, Split Number*

Programmers should deploy their domain expertise when applying any techniques suggested above for their specific applications. After applying all the suitable suggested techniques, programmers may promote critical variables to higher precisions when necessary to achieve an accuracy goal. Programmers should consider precision upgrades in the following order: FMA, split numbers.

The ConnX BBE32EP DSP provides fused-multiplication-addition/subtraction (FMA) hardware conforming to the IEEE 754 2008 standard. The FMA accept all float inputs including subnormal, compute an infinitely precise product, add/subtract the product to/from an addend, round a sum/difference according to a currently-set rounding mode in the FCR, and signal exception(s) if any. A single precision number comprises a 24-bit significand. Inside the FMA, the infinitely precise product is represented with a 48-bit significand, and the addition or subtraction is performed with the 48-bit significand. Programmers should take advantage of these extra significand bits inside the FMA, before considering the following more expensive techniques.

Another technique of precision upgrade is to split 1 value into 2 single precision numbers. When it’s necessary to represent π with more than 24-bit precision, for example, programmers may split π into Pi\_hi and Pi\_lo. Pi\_hi represents a higher 24 bits; while Pi\_lo represents a lower 24 bits. With Pi\_hi and Pi\_lo, programmers are able to represent π with 48-bit precision by replacing π with (Pi\_hi + Pi\_lo).

## 6.5 Implementing the Floating Point FFT/IFFT on ConnX BBE32EP DSP Vector Floating Point Unit (VFPU)

This is a note on the implementation of Fast Fourier Transform/Inverse Fast Fourier Transforms (FFT/IFFTs) of FFT size L=2n on ConnX BBE32EP DSP- Vector Floating Point Unit (VFPU).

ConnX BBE32EP DSP-VFPU provides, 16 8-way float ( 4-way complex float ) vector registers. ConnX BBE32EP DSP-VFPU ISA supports FFT/IFFT by providing

ADDSUBN\_2XF32 and MULMN\_2XF32 operations with 16 floating point multiply accumulate units (FP MACs). The ADDSUBN\_2XF32 operation performs an element-wise simultaneous vector float add and subtract, and the MULMN\_2XF32 operation performs vector lanemultiplexed float multiply useful for complex float multiplication. The next section explains the algorithm implementing the above Floating Point FFT/IFFT.

### *Radix-4 FFT implementation*

With these instructions radix-4 FFT implementation is a good compromise between number of MACs required and the implementation complexity. Figure below shows a radix-4 butterfly structure suitable for implementation with ConnX BBE32EP DSP VFPU ISA.

![](data:image/jpeg;base64...)

#### Figure 7: Radix - 4 Butterfly Implementation

Following snippet of code shows a typical radix-4 butterfly followed by corresponding twiddle multiplication. Load input vectors are assumed as x0, x1, x2, x3, tw1 (=wLn), tw2

(=wL2n) and tw3 (=wL3n) and store output vectors are assumed as y0, y1, y2 and y3 .

Where WL = e -j2π/L, L = size of the FFT, and n = index 0 ...to L/5.

##### Intrinsic Code for Vectroized Radix-4 Butterfly and Twiddle Multiplication

|  |
| --- |
| {  BBE\_ADDSUBN\_4XCF32 (y1, y0, x0, x2);  BBE\_ADDSUBN\_4XCF32 (y3, y2, x1, x3);  // y3 \* (-j)  tmp = BBE\_SHFLN\_4XCF32I (y3, BBE\_SHFLI\_SWAP\_2); y3 = BBE\_CONJN\_4XCF32 (tmp);  BBE\_ADDSUBN\_4XCF32 (y2, y0, y0, y2);  BBE\_ADDSUBN\_4XCF32 (y3, y1, y1 y3);  //Twiddle multiplication  y1 = BBE\_MULN\_4XCF32 (y1, tw1); // BBE\_MULN\_4XCF32 is proto for complex float  multiplication y2 = BBE\_MULN\_4XCF32 (y2, tw2); // It employs two MULM ops.  y3 = BBE\_MULN\_4XCF32 (y3, tw3);  } |

**Implementation of IFFT:**

The implementation is similar for FFT with following two changes:

1. In the radix-4 butterfly implementation, multiplication is performed with j is instead of -j.
2. The resulting elements are scaled by L at the end of the computation.

***Optimizations of Implementation of IFFT***

Three types of optimizations are applied to the above radix-4 implementation.

#### Loop Merging

In the reference example decimation in frequency (DIF) FFT has been implemented. Tensor based implementations are performed to avoid bit-reversing in the end. This involves performing loads/stores in certain complex patterns before/after the processing of each FFT stage.

Two nested loops are needed to implement one stage of radix-4 processing. The inner loop performs loads and stores with uniform strides and the outer loop modifies the pointers to source/destination buffer. Such nested loops incur overheads when inner loop-count is small. These overheads are avoided by merging the two loops and retaining the same schedule as the inner loop. Support for modular addition and conditional AR register moves, in the ISA is exploited to perform complex pattern of memory access as required by tensor based implementation, in single loop for one stage of radix-4. This provides an implementation which scales well with FFT size L and has ideally desired complexity of L\*log(L).

#### Super\_swp Pragma

The compiler uses heuristics to converge quickly to a legitimate schedule of loops. When the schedule is not possible, super\_swp pragma is used to prompt the compiler to perform a thorough search for a resource/recurrence bound schedule.

#### Register Pressure

Register pressure drives FFT implementation decisions. With 8-way addsub and multiplexed multiply operations, available MACs can be fully utilized by performing two, radix-4 butterflies per iteration. The software-pipelined loop, fully utilizing the available MACs, requires more than 16 vector registers. ConnX BBE32EP DSP-VFPU has 16 vector registers, and hence the register pressure does not allow a resource bound schedule.

To avoid register pressure it is advised to write small loops, or loops with small sequences of recurring operations. In case of FFT it translates to avoiding unroll or choosing smaller radix implementation.

1. **Unroll or No unroll:**
   * In the reference example, one radix-4 butterfly is implemented along with corresponding twiddle multiplications per iteration, to avoid register pressure. A recurrence bound schedule of 8 cycles per iteration is achieved in the steady state.
   * The last stage of processing is an exception, which does not require twiddle multiplications. Hence two radix-4 butterflies per iteration are implemented, to get a resource bound schedule of 8 cycles.
2. **Radix-2:**
   * Register pressure can be avoided by implementing a smaller radix-2 implementation.
   * To perform same amount of processing, with radix-2 as radix-4, requires minimum 8 loads and 8 stores, which inturn requires 8 cycles. So radix-2 hits the load-store bound and cannot do better than radix-4 with the given ISA (note the last stage may be an exception).
3. **Fused mul+add:**
   * Fused mul + add usage can be avoided to release register pressure. This does not always help and may cause degradations.

##### Performance of FFT on ConnX BBE32EP DSP-VFPU

FFT is known to have a complexity of L\*log2(L) , where L is the size of FFT. We can achieve the same complexity on ConnX BBE32EP DSP-VFPU. To demonstrate it, cycles consumed by FFT computation are normalized, for size L, by L\*log2(L) . Plots of performance metric for complex float FFT on Connx BBE16EP DSP, Connx BBE32EP DSP and Connx BBE64EP DSP are shown in figure below. The Cadence® Tensilica® ConnX BBE16/32/64EP DSPs (16,32 and 64 MAC Baseband Engine respectively) are a family of DSPs, based on a ultra-high performance architecture designed for use in next-generation baseband processors for LTE Advanced, other 4G cellular radios and multi-standard broadcast receivers.

![](data:image/jpeg;base64...)

#### Figure 8: Performance of Complex Float FFT

Note that the metric initially decreases with FFT size L and then settles around 0.483 on BBE16EP, 0.247 for larger L on BBE32EP, and 0.138 on BBE64EP DSP. It implies that the code scales well with size L. Cycles consumed for computation of large L-size complex float FFT can be approximately given by:

0.130\*L\*log2(L) for BBE16EP-VFPU,

0.247\*L\*log2(L) for BBE32EP-VFPU and 0.485\*L\*log2(L) for BBE64EP-VFPU. Where, L = size of FFT

##### Twiddle Size

Tensor based FFT/IFFT implementation requires repeated twiddles in a vector register for the inner stages. The present reference code has following characteristics:

1. The reference example implementation is designed to achieve best possible cycle performance.
2. It uses tensor based radix-4 DIF FFT implementation
3. It keeps twiddles repeated in the twiddle table for inner stages

• Thus twiddle table size is larger than 3L/4 complex elements and actual size depends on SIMD width. ISA also supports REPN\_4XCF32 operation which repeats any

particular complex float element of a vector in all the lanes of output vector operand. So the twiddle size can be reduced to optimal value of 3L/4 elements at the cost of cycle performance.

## 6.6 Floating Point References

1. M. Baudin and R. L. Smith, “A robust complex division in Scilab,”October 2012, availableat [*http://arxiv.org/abs/1210.4539*](http://arxiv.org/abs/1210.4539).
2. Prof. W. Kahan, Joseph D. Darcy, “How Java’s Floating-Point Hurts Everyone

Everywhere”, Originally presented 1 March 1998 at the invitation of the ACM 1998 Workshop on Java for High–Performance Network Computing (July 2004), Stanford University, Palo Alto, CA. Available at [*http://www.cs.berkeley.edu/~wkahan/JAVAhurt.pdf*](http://www.cs.berkeley.edu/~wkahan/JAVAhurt.pdf).

1. Younis Hijazi, Hans Hagen, Charles Hansen, and Kenneth I. Joy, “Why Interval ArithmeticIs So Useful”.
2. Standards Committee of IEEE Society, "IEEE Standard for Binary Floating Point

Arithmetic", March 1985, IEEE New York, NY, USA. Available at [*http://ieeexplore.ieee.org/xpl/ mostRecentIssue.jsp?punumber=2355*](http://ieeexplore.ieee.org/xpl/mostRecentIssue.jsp?punumber=2355)

1. IEEE Computer Society, "IEEE Standard for Floating-Point Arithmetic", August 2008, IEEENew York, NY, USA. Available at [*http://ieeexplore.ieee.org/xpl/mostRecentIssue.jsp?*](http://ieeexplore.ieee.org/xpl/mostRecentIssue.jsp?punumber=4610933)

[*punumber=4610933*](http://ieeexplore.ieee.org/xpl/mostRecentIssue.jsp?punumber=4610933)

# 7. Special Operations

**Topics:**

•

*Polynomial Evaluation*

•

*Matrix Computation*

•

*Pairwise Real Multiply*

*Operation*

•

*Descramble Operations*

•

*V*

*ector Compression and*

*Expansion*

•

*Predicated Vector*

*Operations*

## 7.1 Polynomial Evaluation

The ConnX BBE32EP DSP has operations to support fast Taylor’s series expansion. These operations are useful to compute approximations of non-linear functions f(x), where f(x) = f(x0) + f’(x0) (x-x0) + … The entire range of f(x) is divided into multiple subdivisions, the center of each being x0. Thus, these operations can be used to evaluate transcendental functions (sines, cosines and such) that can be expressed in the form of a series.

The polynomial evaluation operations are designed to work together to facilitate quick SIMD evaluation of iterative polynomial functions of the form yi = xi \* yi-1 + ci, where xi is a coefficient for the ith step, and ci is a constant for the ith step. Vector polynomial operationsaccelerate N-way real vector multiply-add kernels of the following form:

for (i=0; i<terms; i++)

{

Poly \_out[i] = Lookup[x0] + Poly \_in[i] \* (x[i] - x0);

}

The vector polynomial operations compute N results in parallel and evaluates the function expansion by decomposition:

v = a + b\*(x-x0) + c\*(x-x0)2 + d\*(x-x0)3 v = a + (x-x0)\*(b + (x-x0)\*(c + (x-x0)\*(d + (x-x0)\*0)))

The polynomial acceleration process needs an initial setup, once for every narrow vector (Nx16-bit). The prototype of this operation looks like:

BBE\_POLYNX16\_OFF(xb\_vecNx16 x, immediate sl, immediate sa)

where,

* Input 16-bit vector x to compute vector (x-x0) factors output
* Choose sl (range 12-15) to extract bit-range from input and index/mid-point of its subdivision
* Choose sa for output shift (range 0..3, allows scaling by 1, 2, 4, 8)

Offv = InputData[(sl-1):0] - (2048 << (sl-12));

Offset = (Offv << sa) & 0xFFFF;

After completing the initial setup, execute iteratively until a desired precision is achieved:

* Use (sl-0)/(sl+1) as the shift amount in BBE\_MOVVSV operation to extract the subrange index for BBE\_SHFL16X16/SEL16X16 operation respectively. These are indexes into the lookup table vector values
* Perform vector multiplication of the (x-x0) factors with the previous computed polynomial result and/or the lookup values

It is important to note that the table entries for lookup depend on the SIMD width of the machine. For ConnX BBE32EP DSP, 16 subdivisions are allowed for the range of *f*(x) by using SHFL and 32 subdivisions by using SELECT. One can use SELECT or SHFL depending on the operation needs.

## 7.2 Matrix Computation

ConnX BBE32EP DSP instruction set provides operations optimized for several types of matrix computation tasks. These operations support matrices stored in both packed and streaming order. Let's say we have three 2x2 matrices - A0, A1 and A2; the following figures illustrate this difference of matrix storage in memory.

![](data:image/jpeg;base64...)

Now, in packed or natural order, the elements of each matrix are stored contiguously in

memory

![](data:image/jpeg;base64...)

However, in streaming order, corresponding elements of each matrix form individual streams as they are stored in memory.

![](data:image/jpeg;base64...)

ConnX BBE32EP DSP supports efficient computation of small, complex packed matrix multiplications – 2x2, 4x4, 8x8 and variants. For streaming order matrices, regular multiply operations from the ISA can be used to get efficient code schedules. For packed order matrices, special select/shuffle patterns are required that enhance the regular shuffle operation BBE\_SHFLNX16I and select operation BBE\_SELNX16I; see *Packed Complex Matrix Multiply* on page 76. These complex packed matrix multiplications are accomplished by decomposing computation into smaller tasks:

* Load matrix data into the ConnX BBE32EP DSP vector registers
* In a multi-step execution routine, use special select/shuffle patterns to reorganize the order of data depending on matrix size
* For square matrices, using regular multiplies after shuffle.
* For rectangular matrices, using special multiplies with replication.
* Perform multi-step MAC operations and storing the result

When the matrices are not square or multiplying a matrix by a vector, the

BBE\_MULNX16PC\_{0/1}() pair of protos can be used. The inputs to these protos are reordered outputs of the special shuffle/select operations and the outputs are full-precision results of the shuffled element multiplications. BBE\_MULNX16PC\_{0/1}() pair of protos perform N/2 complex multiplications and add adjacent pairs to form N/2 full-precision complex results in one wide wvec register. For multiply-accumulate type operations, a similar pair of protos is also provided – BBE\_MULANX16PC\_{0/1}().

The following figure illustrates packed matrix multiplication of two 2x2 complex matrices:

![](data:image/jpeg;base64...)

## 7.3 Pairwise Real Multiply Operation

The pairwise real multiply operations in the ConnX BBE32EP DSP support 32 MACs per cycle on real data. The BBE\_MUL[A]NX16PR operation finds natural use in accelerating symmetric FIR operations with real coefficients as described in the previous section. The general form of a pairwise real multiply operation is:

out\_wvec[i] = in\_vecA[i] \* p + in\_vecB[i] \* q

where, in\_vecA and in\_vecB are N-way 16-bit narrow vectors; p & q are 16-bit scalars; and out\_wvec is a N-way 40-bit wide vector. The 32-bit signed product per vector element is pairwise added between the two input narrow vectors. The pairwise sum is then signextended to 40-bits per element and written to the output wide vector. When implementing a symmetric FIR operation, p & q represent real coefficients of the filter.

In addition, the BBE\_MUL[A]NX16PR operation finds use in accelerating real matrix-matrix multiplies, in particular product of 2x2 matrices, real matrix-vector multiplies and in real dot product. Refer to the ISA HTML of BBE\_MUL[A]NX16PR for more information on how the operation is set up.

## 7.4 Descramble Operations

*Use of 1D single-code 16-bit complex despread function for combined descrambling and despreading operation on the downlink control channel DPCCH/FDPCH.*

In 3G, scrambling by itself only involves complex multiplication, and since we usually need to work with a single (primary) scrambling code (as most channels are scrambled using a single scrambling code), 1D single-code type function is required for the descrambling operation.

ConnX BBE32EP DSP provides ISA support for descrambling. A descrambling operation multiplies an input data sample with a code. This type of an operation is suited for vector processing through element-wise signed multiplication of a vector of input data samples by a code vector. The input samples may be real or complex, and accordingly define the nature of code used in the descrambling operation.

An N-way (N = SIMD size of the ConnX BBE32EP DSP) descrambling operation on real data can be described as follows:

FOR i in {0..N}

re\_res[i] = real\_ip[i] \* real\_code[i]);

The BBE\_DESCRNX16{} proto performs an N-way descrambling of 16-bit real data with a real code. A real code is always {1, -1} and is encoded by a 1-bit code as a vector input to the proto - '1' encoded by '0' and '-1' encoded by '1'. Thus, a set of 16-bits of code is required for a full vector of 16-way real inputs. Thus, a 256-bit narrow vec register can hold 8 code sets, 16 -bits each. The result of BBE\_DESCRNX16{} is a vector of sixteen 16- bit real elements every cycle. The resulting output values saturate when the 16-bit range is exceeded.

For complex data samples, however, the descrambling operation in the ConnX BBE32EP DSP is supported by two different protos. The use of these protos depends on the nature of input data samples.

* BBE\_DESCRNX8C{} - 16-way descramble of 8-bit complex samples
* BBE\_DESCRN\_2X16C{} - 8-way descramble of 16-bit complex samples.

Similar to despreading, complex codes for descrambling can also be of two types - {+/-1, +/-j} or {+/-1+/-j}. A one bit immediate to the complex descramble proto selects the type of code to be used. For each sample of complex input data, two bits are required to encode the four possible code values regardless of the code type. *Table 12: Decoding Complex Codes for Despreading* on page 82 can be used again to show how complex codes are decoded within a complex descrambling operation. A vector-form description of a complex descrambling operation can be shown as:

FOR i in {0..N} re\_res[i] = re\_ip[i] \* re\_code[i] + im\_ip[i] \* im\_code[i]; im\_res[i] = re\_ip [i] \* im\_code[i] + im\_ip[i] \* re\_code[i];

The outputs of the complex descramble protos are complex vectors whose values saturate when their range is exceeded. The real and imaginary parts of the complex result(s) in the output narrow vec register are interleaved. Refer to the ISA HTML for data-type information of inputs and outputs, and the manner in which each operation is set up. A summary of all the protos available for descrambling in the ConnX BBE32EP DSP is shown in *Table 27: Protos for Descrambling* on page 134.

### Table 27: Protos for Descrambling

|  |  |  |  |  |  |
| --- | --- | --- | --- | --- | --- |
| **Proto Name** | **SIMD** | **Input Type (vec)** | **Code Values** | **Code Sets (vec)** | **Output Type**  **(vec)** |
| NX16 | 16 | 16b complex | {1, -1} | 16 x (16x1b) | 16b real |
| NX8C | 16 | 8b complex | {+/-1, +/-j} |  {+/-1+/-j} | 8 x (16x2b) | 8b complex |
| N\_2X16C | 8 | 16b complex | {+/-1, +/-j} |  {+/-1+/-j} | 16 x (8x2b) | 16b complex |

## 7.5 Vector Compression and Expansion

ConnX BBE32EP DSP has special operations to help with vector compression and expansion. Vector compression involves extracting elements from a full vector source, indexed by their positions, to form a new vector consisting of the extracted elements. Vector compression can be achieved by combining a squeeze and a shuffle operation. As an example, to compress a source vector vec\_in = {vn-1, vn-2, …, v1, v0} to only its 0th, 1st and 4th elements, follow the steps below:

1. Define a Boolean vector mask in the vbool register vbr. The bit positions in the mask correspond to the position of the desired elements in the source vector. In this case,vbr =

{0,0,...,0,1,0,0,1,1}.

1. Use the proto BBE\_ SQZN() to generate a shuffle pattern in a vsa register st and a count of the number of bytes squeezed in an ar register art. Now, st = {0, 0,..., 0, 4, 1, 0} (n=16, SIMD width of the core) and art = 6 (3 elements x 2 bytes/element).
2. Now, use the shuffle pattern produced in st to shuffle the source vector vec\_in by choosing a BBE\_SHFL proto appropriate to the data-type of vec\_in. The output vec\_shfl

= {v0, v0,…, v0, v4, v1, v0}. Notice that after extracting the specified elements on the lower end, the rest of the output vector is filled with the lowermost element of the source vector (v0).

1. Additionally, the corresponding BBE \_SAVNX\*\_XP() proto can be used on vec\_shfl along with the size computed in art to store only the compressed vector elements.

Vector expansion follows a similar principle by using a combination of a un-squeeze and a shuffle instruction. Assuming the source vector vec\_in = {vn-1, vn-2,…, v1, v0},

1. Define a Boolean vector mask in a vbool register vbr and generate a shuffle pattern in a vsa register st using the BBE\_ UNSQZN() proto. BBE\_ UNSQZN() also tracks the count of logical ones in the mask and computes the number of bytes to be un-squeezed as output in an ar register art.
2. The lowest order element (element zero) in st is always set to zero. Each higher order element (from one up to n-1) will contain a count of the number of ones in the input Boolean vector vbr, starting at the next lower element and going down to element zero. Now, if vbr = 0b1001001 to represent the expansion of 0th, 3rd and 6th elements, then the un-squeeze instruction sets st = 0d32221110 and art = 6.
3. The index values in st correspond to the positions of logical ones in the Boolean mask. Use the generated vector of index values in st within a BBE\_ SHFLNX\*() proto to unsqueeze the vector elements and produce an expanded output which in this case is vec\_shfl = {v3, ..., v3, v2, v2, v2, v1, v1, v1, v0}

## 7.6 Predicated Vector Operations

ConnX BBE32EP DSP offers an enhanced ISA support for predicated vector operations natural to the core SIMD size. Without ISA support for predicated vector operations inherently parallel code cannot be parallelized by a vectorizing compiler. Consider a simple example:

for (i=0; i*<*16; i++) {

if (a[i] != 0)

b[i]++;

}

However, using predicated vector operations – in this case BBE\_ADDNX16T; the above code can be easily vectorized. With a higher number of predicated operations supported for vectors than other Tensilica® Baseband architectures, the ConnX BBE32EP DSP provides an improved performance in the presence of control flow both inside and outside loops. *Table 28: Predicated Vector Operations* on page 136 lists all the predicated operations supported by the ConnX BBE32EP DSP for parallelizing control flow in vector code.

### Table 28: Predicated Vector Operations

|  |  |  |
| --- | --- | --- |
| **Type** | **Predicated TRUE Operation** | **Predicated FALSE Operation** |
| Add | BBE\_ADDNX16T | BBE\_ADDNX16F |
| Signed add | BBE\_ADDSNX16T | BBE\_ADDSNX16F |
| Complex, complex conjugate | BBE\_CONJNX16CT | BBE\_CONJNX16CF |
| Complex, complex conjugate - saturating | BBE\_CONJSNX16CT | BBE\_CONJSNX16CF |
| Signed maximum | BBE\_MAXNX16T | BBE\_MAXNX16F |
| Unsigned maximum | BBE\_MAXUNX16T | BBE\_MAXUNX16F |
| Signed minimum | BBE\_MINNX16T | BBE\_MINNX16F |
| Unsigned minimum | BBE\_MINUNX16T | BBE\_MINUNX16F |
| Narrow signed move | BBE\_MOVNX16T | Use TRUE type |
| Wide signed move | BBE\_MOVNX40T | Use TRUE type |
| Complex signed multiply-accumulate | BBE\_MULANX16CT | Use TRUE type |
| Complex conjugate signed multiply-accumulate | BBE\_MULANX16JT | Use TRUE type |
| Multiply-accumulate | BBE\_MULANX16T | Use TRUE type |
| Real unsigned\*signed  Multiply-accumulate | BBE\_MULUSANX16T | Use TRUE type |
| Real unsigned\*unsigned  Multiply-accumulate | BBE\_MULUUANX16T | Use TRUE type |
| Signed negative | BBE\_NEGNX16T | BBE\_NEGNX16F |
| Signed saturating negative | BBE\_NEGSNX16T | BBE\_NEGSNX16F |
| Complex signed reduction sum | BBE\_RADDNX16CT | BBE\_RADDNX16CF |
| Signed reduction sum | BBE\_RADDNX16T | BBE\_RADDNX16F |
| Complex signed saturating reduction sum | BBE\_RADDSNX16CT | BBE\_RADDSNX16CF |
| Signed saturating reduction sum | BBE\_RADDSNX16T | BBE\_RADDSNX16F |

|  |  |  |
| --- | --- | --- |
| **Type** | **Predicated TRUE Operation** | **Predicated FALSE Operation** |
| Signed reduction maximum | BBE\_RMAXNX16T | BBE\_RMAXNX16F |
| Unsigned reduction maximum | BBE\_RMAXUNX16T | BBE\_RMAXUNX16F |
| Signed reduction minimum | BBE\_RMINNX16T | BBE\_RMINNX16F |
| Unsigned reduction minimum | BBE\_RMINUNX16T | BBE\_RMINUNX16F |
| Signed difference | BBE\_SUBNX16T | BBE\_SUBNX16F |
| Signed saturating difference | BBE\_SUBSNX16T | BBE\_SUBSNX16F |

# 8. Load & Store Operations

**Topics:**

* *ConnX BBE32EP DSP*

*Addressing Modes*

* *Aligning Loads and Stores*
* *Circular Addressing*
* *Variable Element Vector*

*Aligning Loads and*

*Stores*

* *Update Post-increment in*

*Loads and Stores*

The ConnX BBE32EP DSP has an extremely wide variety of load and store operations for vectors, scalars, pairs of scalars (which can represent complex numbers as an (imaginary, real) pair) and a large number of addressing modes. There are over 185 load and store operations which support 16/32-bit scalars, 16x16-bit vectors in memory and 40-bit elements in vector registers. Unaligned vectors are supported by aligning loads and stores. The five addressing modes include a variety of indexed, immediate and updating addressing. There are 256/640-bit spills and restore operations primarily for compiler use. The 640-bit spills are implemented by extending them to 1024-bits and moving into four 256-bit registers for four subsequent stores/restores.

## 8.1 ConnX BBE32EP DSP Addressing Modes

*Table 29: ConnX BBE32EP DSP Addressing Modes* on page 140 provides a description of the ConnX BBE32EP DSP addressing modes .

### Table 29: ConnX BBE32EP DSP Addressing Modes

|  |  |  |
| --- | --- | --- |
| **Name** | **Notation** | **Description** |
| Immediate | \_I | Offset is an immediate operand |
| Immediate with Post-increment | \_IP | Offset is an immediate operand. Update base address postincrement. |
| Indexed | \_X | Offset from a register |
| Indexed with Post-increment | \_XP | Offset from a register. Update base address post-increment. |
| Narrow Immediate | \_I\_N | Offset is a narrow immediate operand. Compiler alias of \_I version. |

![](data:image/png;base64...)**Note:** There are specialized versions of the immediate loads (suffixed with ".N", which you may see in the disassembly. You do not need to use these directly; the compiler automatically chooses between the .N and the normal version depending on the scheduling.

Apart from the above five basic addressing modes, a special addressing modes supported by ConnX BBE32EP DSP is the \_IC notation used with operations that support circular addressing.

## 8.2 Aligning Loads and Stores

In ConnX BBE32EP DSP, the support from aligning operations enables vector load/store of unaligned data. The aligning vector load and store operations move 256-bit vectors between the ConnX BBE32EP DSP registers and memory addresses that may or may not be aligned to 32-byte boundaries. If the address is aligned, these operations perform the same operation as aligned load and store operations. For unaligned addresses, these operations use the ConnX BBE32EP DSP alignment register file to provide a throughput of one aligning load or store operation per operation. The aligning vector load and store operations rely on two mechanisms to do this. One mechanism is the rotation of load and store data based on the least significant address bits of the virtual address. The other mechanism is the appropriate merging of the vector data with the contents of the alignment register.

### Aligning Loads

A special priming operation is used to begin the process of loading an array of unaligned data. This operation conditionally loads the alignment register if the target address is unaligned. If the memory address is not aligned to a 32-byte boundary, this load initializes the contents of the alignment register. The subsequent aligning load operation merges data loaded from the target location with the appropriate data bytes already residing in the alignment register to form the completed vector, which is then written to the vector register. The base address is incremented after the completion of the aligning load operation i.e. postincrementation. Data from this load then overwrites the alignment register, priming it for the next load. Subsequent load operations provide a throughput of one aligning load per operation.

The design of the priming load and aligning load operations is such that they can be used in situations where the alignment of the address is unknown. If the address is aligned to a 32 boundary, the priming load operation does nothing. Subsequent aligning load operations will not use the alignment register and will directly load the memory data into the vector register. Thus, the load sequence works whether the starting address is aligned or not.

Any C code for the ConnX BBE32EP DSP compiled using the Xtensa C Compiler (XCC) always aligns arrays to 2N-bytes, where N is the SIMD size. If the C code development is not in a XCC environment, users may need to explicitly force alignment appropriate to the environment used.

### Aligning Stores

Aligning stores operate in a slightly different manner. Each aligning store operation is sensitive to the value of the flag bit in the alignment register. If the flag bit is 1, appropriate bytes of the alignment register are combined with appropriate bytes of the vector register to form the 256-bit store data written to memory. On the other hand, if the flag bit is 0, then the store is a partial store and only the relevant bytes of the vector register are written to memory. Data from the alignment register is not used. No data will be written to one or more bytes starting at the 32 byte aligned address. This store will only write data starting from the byte corresponding to the memory address of the store.

Each aligning store operation (independent of the value of the flag bit) will also update the appropriate bytes in the alignment register, priming it for the next store operation. Every aligning store operation also sets the alignment register’s flag bit to 1. When the last aligning store operation executes, some data may be left in the alignment register, which must be flushed to memory. A special flush operation copies this data from the alignment register to memory if needed.

Start with the BBE\_ZALIGN operation to store an array of vectors beginning at an aligning memory address. This operation initializes the alignment register’s contents and clears the flag bit. A series of aligning stores following the BBE\_ZALIGN operation will store one vector to memory per operation. Note that the first store operation of this series will perform a partial store because the flag bit was cleared by the BBE\_ZALIGN operation. Each subsequent store will perform a full 256-bit store because the first (and subsequent) store operations set the flag bit. Finally, a flush operation flushes out the last remaining bytes in the alignment register.

Once again, the design of the aligning store and flush operations allows them to work even if the memory address is aligned to a 32 byte boundary. Specifically, if the address is aligned, the store operations store data only from the vector register. The alignment register is not used if the addresses are aligned. Similarly, if the addresses are aligned, the flush operation does nothing.

Note that these operations move data between memory and the ConnX BBE32EP DSP registers if the memory addresses are not aligned to a vector boundary. However, these operations do assume that the addresses are aligned to 16-bit scalar boundaries. If these operations are used with addresses that are not aligned to 16-bit boundaries, the result is undefined. The priming and flush operations support only the immediate addressing mode, while the other operations support all four addressing modes.

## 8.3 Circular Addressing

The ConnX BBE32EP DSP supports circular addressing using two special register states CBEGIN and CEND. There are certain requirements to initialize these states before a circular buffer addressing operation is set up, as follows:

* The circular buffers need to be 32-byte aligned. This requires both CBEGIN and CEND to be 32-byte aligned addresses. Otherwise, circular buffer loads and stores may behave incorrectly.
* CBEGIN should be set to the start address of the circular buffer pointing at the first vector.
* CEND should be set to 32 bytes after the last aligned vector in the buffer.
* (CEND-CBEGIN) must be a multiple of 32-bytes.

![](data:image/png;base64...) **Note:** Comparison for wrap-around is done after the post-increment .

The ConnX BBE32EP DSP only supports a stride of 32 bytes in circular addressing. Other than these considerations, the load and store operations perform as expected, and can be either aligned or aligning.

There are four basic operations to access the circular buffer states as shown in the following table.

### Table 30: Circular Buffer State Registers

|  |  |
| --- | --- |
| **Operation** | **Description** |
| RUR.CBEGIN() | Reads from user register CBEGIN |
| RUR.CEND() | Reads from user register CEND |
| **Operation** | **Description** |
| WUR.CBEGIN(v) | Writes to user register CBEGIN |
| WUR.CEND(v) | Writes to user register CEND |

The load/store operations with the \_IC suffix or the corresponding \_IC C intrinsic functions refer to the circular buffer addressing. Also, the BBE\_LAPOS\_PC priming load operation and the corresponding BBE\_LAPOS\_PC intrinsic function use the circular buffer registers.

## 8.4 Variable Element Vector Aligning Loads and Stores

ConnX BBE32EP DSP contains a set of store operations that compact vectors at the same time as they do the store, according to patterns defined by the user. These operations are BBE\_LAVNX16.XP, BBE\_SAVNX16.XP and BBE\_SAPOS.FP. There are also protos that

define variations for various data types including fractional, complex, fractional complex, etc. as defined in the earlier tables. Further details about the operations, their protos and usage may be found in the ConnX BBE32EP DSP ISA/HTML.

## 8.5 Update Post-increment in Loads and Stores

The ConnX BBE32EP DSP has many loads and stores, as described in earlier sections, which perform an address update as part of their operation. The updating loads and stores do a post-increment of the address after doing the load or store.

# 9. Nature DSP Signal Library

The ConnX BBE32EP DSP comes packaged together with a generic DSP library. The library comes with two Xplorer projects -

bbe32ep\_integrit\_re\_lib\_v<version\_num>.xws and

bbe32ep\_integrit\_re\_demo\_v<version\_num>.xws. The former is the library. The latter is a test application that exercises all the functions in the library. Both are delivered in source form so that the user can use the library as is or use it as a starting point for their own development. The library contains functions for basic math functions, matrix operations, communication operations and others.

From the library project, under doc, is a reference manual,

*NatureDSP Signal Library Reference for Cadence® ConnX BBE32EP DSP DSP*, which describes the library and the test program in depth.

# 10. Implementation Methodology

**Topics:**

* *Configuring a ConnX BBE32EP DSP*
* *XPG Estimation for Size, Performance and Power*
* *Basic ConnX BBE32EP*

*DSP Characteristics*

* *Extending a ConnX*

*BBE32EP DSP with User*

*TIE*

* *XPG Configuration*

*Options and Capabilities*

* *Sample Configuration Templates for the ConnX BBE32EP DSP*
* *Synthesis and Place-andRoute*
* *ConnX BBE32EP DSP*

*Memory Floor-planning*

*Suggestions*

* *Mapping the ConnX BBE32EP DSP to FPGA*

The ConnX BBE32EP DSP is a part of the *ConnX Baseband Engine* family of DSPs specially optimized for infrastructure and user-equipment applications. This family of DSPs is based on Xtensa ISA version LX6.0. The ConnX BBE32EP DSP is included as a check box option in the Xplorer Processor Generator (XPG) interface in Xtensa Xplorer. This option is featured in the RG-2016.4 release of Xtensa Xplorer, XPG, and Xtensa tools. The following section provides guidelines for using XPG to configure a custom ConnX BBE32EP DSP. The last section in this chapter discusses synthesis and placeand-route aspects of the ConnX BBE32EP DSP.

## 10.1 Configuring a ConnX BBE32EP DSP

Creating your own ConnX BBE32EP DSP DSP configuration consists of the following steps using XPG in the Xtensa Xplorer IDE:

1. Open the System Overview tab by navigating to the Show View menu in the Window dropdown item from the menu bar. Right click the Configuration directory and select the New Configuration option.
2. Select the Create new configuration with a new core ISA option and make sure to test your XPG access by entering a valid customer\username and password. Click next after XPG access test is passed.
3. Enter a name for your configuration, an optional description and select the processor ISA version LX6.0 or greater. Click Finish.
4. Open the created configuration by finding it within the Uninstalled Configs sub-directory. In the Configuration Overview pane, click the Edit button in the Workspace Config area under Configuration section.
5. Open the Processors tab in the Configuration Editor and select the ConnX BBE32EP DSP DSP from the ConnX BBE-EP DSP family.

A red cross-mark may appear if there are any required modules to be included in the configuration. Hover over the cross-mark to read any errors and resolve them by selecting appropriate options to include all the necessary modules. Some errors may need to be resolved by selecting correct options from the Interfaces tab.

1. To extend your ConnX BBE32EP DSP configuration with one or more options, select the relevant check box options under the ConnX BBE-EP DSP Options section:
   * FFT
   * Symmetrical FIR
   * Packed complex matrix multiply
   * LFSR & convolutional encoding
   * Linear block decoder
   * 1D Despreader
   * Soft-bit demapping
   * Vector divide
   * Fast reciprocal & reciprocal square root
   * Advanced precision reciprocal & reciprocal square root
   * Advanced precision multiply/add (see note below)
   * Inverse log-likelihood ratio (LLR)
   * Single and dual peak search
   * Single-precision vector floating-point (see note below)

![](data:image/png;base64...)

**Note:** Including any configurable option adds additional operations and associated hardware to the ConnX BBE32EP DSP configuration.

![](data:image/png;base64...) **Restriction:** The following pair of options are mutually exclusive:

* + Advanced precision multiply/add
  + ![](data:image/png;base64...)Single-precision vector floating-point

### Figure 9: ConnX BBE32EP DSP Configuration Options in Xtensa Xplorer

Optionally, there are sample configuration templates provided for the ConnX BBE32EP DSP, called XRC\_B32EP\_AO and which are described in *Sample Configuration Templates for the ConnX BBE32EP DSP* on page 157. They provide useful starting points for users who wish to either use the configuration as described by the template, or create their own variation. It is useful, but not essential, to start with a ConnX BBE32EP DSP template. Following are the steps to create your configuration in the Xtensa Xplorer IDE using a template:

1. Follow steps 1-4 from earlier procedure to create your own ConnX BBE32EP DSP configuration.
2. Click Load a Configuration Template from the Software pane if you wish to use one of the templates as a starting point for your ConnX BBE32EP DSP configuration
3. Click Select from standard templates and pick either XRC\_B32EP\_MIN or XRC\_B32EP\_AO to apply and click OK.

As you are customizing your ConnX BBE32EP DSP, keep in mind the following restrictions. The ConnX BBE32EP DSP has 48/96-bit instruction formats. The following may need to be set during the configuration process:

* The Maximum Instruction Width in bytes to 12 in the ISA Configuration Options section under the Instructions pane;
* Pipeline Width to 7 in the ISA Configuration Options section under the Instructions pane;
* Width of Instruction Fetch Interface to 128 [bits] in the PIF/Memory Interface Widths section under the Interfaces pane; and
* Width of Data Memory/Cache Interface to 256 [bits] in the PIF/Memory Interface Widths section under the Interfaces pane.

Once your ConnX BBE32EP DSP has been configured, save your configuration and open the Configuration Overview pane. You can now click the Upload/Build button to submit your ConnX BBE32EP DSP configuration to XPG. Once your configuration is built, you may download and install it in the Xtensa Xplorer IDE.

## 10.2 XPG Estimation for Size, Performance and Power

In the RG-2016.4 release of the XPG, the estimation of size (area), performance, and power for the ConnX BBE32EP DSP is available.

These estimations are adjusted when users modify their ConnX BBE32EP DSP configuration. Sometimes hardware is shared between one or more configurable options. This makes the combined cost of those options less than the linear sum of their individual costs. In ConnX BBE32EP DSP, for example,the FFT and symmetric FIR options share a significant amount of hardware - including one of them makes it relatively inexpensive to include the other.

## 10.3 Basic ConnX BBE32EP DSP Characteristics

Some of the relevant configuration characteristics of the ConnX BBE32EP DSP include:

* ConnX BBE32EP DSP instruction set
* Vector Boolean registers
* Little-endian byte order
* Two load/store units
* 256-bit local data memory interface
* MUL16 and MUL32 (Pipelined + UH/SH) arithmetic operation options
* 128-bit PIF interface. A 32-bit PIF interface is theoretically possible, but is not advised for ConnX BBE32EP DSP, as it is intended for high performance applications; a 32-bit PIF is likely to be inadequate for optimal ConnX BBE32EP DSP performance.

## 10.4 Extending a ConnX BBE32EP DSP with User TIE

The ConnX BBE32EP DSP can be extended with user TIE defining new operations. These instructions can be assigned to the 24-bit regular instruction format, or can be used with the 48/96-bit FLIX instruction format available for user instruction additions. Due to encoding restrictions, there are fewer bits available in the user-defined instruction format than the length of the instruction.

We illustrate the process of defining user FLIX instruction format, and adding instructions to it using two TIE examples, as follows. The first TIE example defines a new FLIX format, consisting of five FLIX slots. The following TIE template can be used to define a new 96-bit wide FLIX format:

|  |
| --- |
| // Define a new 96-bit wide FLIX format for user instructions in ConnX BBE32EP DSP // Use this format declaration:  format user96 96 { user\_wide\_slot0, user\_wide\_slot1, user\_wide\_slot2, user\_wide\_slot3, user\_wide\_slot4 }  // Assign NOPs to the user slots slot\_opcodes user\_wide\_slot0 {NOP} slot\_opcodes user\_wide\_slot1 {NOP} slot\_opcodes user\_wide\_slot2 {NOP} slot\_opcodes user\_wide\_slot3 {NOP} slot\_opcodes user\_wide\_slot4 {NOP} |

Based on the scheduling requirements in the target application, users may now add one or more operations -

* Either from the ConnX BBE32EP DSP ISA or
* User defined TIE operations - to the slot\_opcodes defined.

Based on the amount of encoding space available, the TIE Compiler will attempt to encode the format and opcodes assigned to its one or more slots.

Similarly, the following TIE template can be used to define a new 48-bit narrow FLIX format:

|  |
| --- |
| // Define a new 48-bit (narrow) FLIX format for user instructions in ConnX BBE32EP DSP // Use this format declaration:  format user48 48 { user\_narrow\_slot0, user\_narrow\_slot1 }  // Assign NOPs to the user slots slot\_opcodes user\_narrow\_slot0 {NOP} slot\_opcodes user\_narrow\_slot1 {NOP} |

![](data:image/png;base64...) **Restriction:**

* Type of Error: TIE Compiler Error
* Error Details: TIE\_FORMAT\_MAX\_SLOT\_COUNT , A total slot count of [n] has been reached which exceeds the maximum slot limit of 64. Each format may contain a maximum of 30 slots, but the total slot count across all formats may not exceed 64.
* How to Avoid:

For a ConnX BBE32EP DSP configuration with the "***Advanced Precision***

***Multiply/Add***" option turned ON: A restriction in the current release provides

four user FLIX slots. No more than four slots may be defined and used regardless of the number of user TIE formats being added.

For a ConnX BBE32EP DSP configuration with the "***Single Precision Vector FP***" option turned ON: A restriction in the current release does not allow any user TIE formats to be specified. Users are allowed to slot ConnX BBE32EP DSP operations or user-defined operations in the DSP's predefined slots.

Users can also write their own operations to extend an Xtensa based machine like a ConnX BBE32EP DSP. The *Tensilica® Instruction Extension (TIE) Language User’s Guide* is a good reference to learn the fundamentals of TIE and write TIE code to extend a core’s ISA.

### *Compiling User TIE*

Following are the instructions to compile an example user TIE for the ConnX BBE32EP DSP:

1. Start with a ConnX BBE32EP DSP configuration in Xtensa Xplorer. The configuration should not already include the desired user TIE.
2. Clone the configuration and attach TIE files. Select **ConnX BBE32EP DSP\_user \_format.tie** and **ConnX BBE32EP DSP\_user \_slots.tie**. Choose a name for the TIE database, TDB.
3. Choose a name for the new configuration.
4. Compile the TDK for the new configuration with user TIE. Note: Generating cstub libraries requires more time than a standard TDK compilation.
5. After compiling the TDK, use the new configuration to compile and run code.

### *Name Space Restrictions for User TIE*

There are restrictions in the name space that can be used in a user TIE file when using the ConnX BBE32EP DSP.

The following TIE elements are reserved and cannot be used in TIE added to a configuration that includes the ConnX BBE32EP DSP. In general, the prefix BBE\_, bbe\_, and xb\_ are reserved for the coprocessor. More specifically, the names of the different TIE features used by the coprocessor are discussed below.

The following tables show state and register file names and user register entries, used by ConnX BBE32EP DSP:

#### Table 31: State and Register File Names

|  |  |  |
| --- | --- | --- |
| **Name** | **Type** | **Description** |
| vec | regfile |  |
| wvec | regfile |  |
| valign | regfile |  |
| **Name** | **Type** | **Description** |
| vsa | regfile |  |
| vbool | regfile |  |
| mvec | regfile | Vector divide option |
| BBE\_BMUL \_STATE | state | LFSR & convolutional encoding option |
| BBE\_BMUL \_ACC | state | LFSR & convolutional encoding option |
| BBE\_STATEA | state | FFT / symmetric FIR option |
| BBE\_STATEB | state | FFT / symmetric FIR option |
| BBE\_STATEC | state | FFT / symmetric FIR option |
| BBE\_STATED | state | FFT / symmetric FIR option |
| CBEGIN | state |  |
| CEND | state |  |
| FFTCTRL | state | FFT option |
| RANGE | state | FFT option |
| FLUSH\_TO\_ZERO | state | Advanced precision multiply/add option |
| BBE\_PQUO0 | state | Vector divide option |
| BBE\_PQUO1 | state | Vector divide option |
| BBE\_PREM0 | state | Vector divide option |
| BBE\_PREM1 | state | Vector divide option |

#### Table 32: User Register Entries

|  |  |
| --- | --- |
| **User Register Name** | **User Register Number** |
| BBE\_STATEA | 0-7 |
| BBE\_STATEB | 8-15 |
| BBE\_STATEC | 16-23 |
| BBE\_STATED | 24-31 |
| **User Register Name** | **User Register Number** |
| RANGE | 96 |
| FLUSH\_TO\_ZERO | 97 |
| BBE\_BMUL\_STATE | 192-223 |
| BBE\_BMUL\_ACC | 224 |
| BBE\_PQUO0 | 64-67 |
| BBE\_PQUO1 | 68-71 |
| BBE\_PREM0 | 72-75 |
| BBE\_PREM1 | 76-79 |
| FFTCTRL | 241 |
| CBEGIN, CEND | 246, 247 |

![](data:image/png;base64...) **Note:** User TIE should not use User Registers above 223)

#### • Coprocessor Number: 1

* **ctype Names:** All ConnX BBE32EP DSP ctype names are reserved names. They are all prefixed with "xb\_". In addition, the ctype starting with “vbool”, “vsa”, and “valign” are also reserved.
* **Operation Names:** All ConnX BBE32EP DSP operation names are reserved names, which are either: Prefixed with BBE\_, Named RUR.<User register name as defined above> or Named WUR.<User register name as defined above>.
* **Proto Names:** All ConnX BBE32EP DSP intrinsic (proto) names are reserved intrinsic names. These all have the prefix “BBE\_”, or the prefix that begins with the ctype names “xb \_”, “vsa”, “vbool”, “valign” (for data type conversions protos).
* **TIE Function Names**: The ConnX BBE32EP DSP uses a number of TIE functions, which you should not use for your own TIE functions. All functions have the prefix “bbe\_”, which should not be used by user TIE.
* **Semantic Names**: The ConnX BBE32EP DSP uses a number of semantic names, which you should not use within your own TIE semantics. All semantics have the prefix “bbe\_”, which should not be used by user TIE.

## 10.5 XPG Configuration Options and Capabilities

The following tables (**Table 8**‑ **4** through **Table 8**‑ **9**) list the configuration options and any constraints on the ConnX BBE32EP DSP .

### Table 33: Simulation Modeling Capabilities

|  |  |
| --- | --- |
| **Capability** | **Allowed?** |
| NGO flow | Yes |
| Pin-level XTSC | Yes |
| XTMP | Yes |
| XTSC | Yes |

### Table 34: Instruction Extension

|  |  |
| --- | --- |
| **Option** | **Allowed?** |
| 16AR | Yes |
| CLAMPS | Mandatory |
| MAC16 | Yes |
| Extended L32R | No |
| MUL32 Implementation Selection | Pipelined with UH/SH |
| MUL16 | Yes |
| DIV32 | Yes |
| NSA (Normalized Shift Amount) | Mandatory |
| MINMAX (min and Max values) | Mandatory |
| SEXT (Sign Extended) | Mandatory |
| BOOLEANS | Mandatory |
| Density instructions supported (16-bit) | Yes |

### Table 35: Allowed Architecture Definition

|  |  |
| --- | --- |
| **Option** | **Constraint** |
| Load/Stores | Two |
| Action when handling an unaligned load/store | Take exception |
| Supported instruction widths | 16, 24, 48, & 96 required |

### Table 36: Instruction Width

|  |  |  |
| --- | --- | --- |
| **Option** | **Allowed?** | **Constraint** |
| Pipeline length | Yes | Only seven-stage allowed. |
| Minimum number of interrupts needed |  | 0 |
| **Option** | **Allowed?** | **Constraint** |
| Minimum number of timers needed |  | 0 |
| Inbound PIF needed? | Not needed |  |
| PIF widths supported |  | 32, 64 and 128. |
| Byte Enables | Mandatory |  |
| **\* (instrwidthbytes = 8**) |  |  |

### Table 37: Coprocessor Configuration Options

|  |  |
| --- | --- |
| **Option** | **Allowed?** |
| Number of coprocessors | At least two |
| Single-precision floating point | Yes |
| Double-precision acceleration HW | Yes |
| HiFi2 | No |

### Table 38: Local Memories

|  |  |
| --- | --- |
| **Option** | **Allowed?** |
| D-Cache supported | Yes |
| I-Cache supported | Yes |
| CAMMU | Yes |
| CAXLT | Yes |
| Full MMU (4KB page) | No |
| XLMI | No |

### Table 39: TIE Option Packages

|  |  |
| --- | --- |
| **Option** | **Allowed?** |
| FLIX3 | No |
| QIF32 | Yes |
| GPIO32 | Yes |
| User-defined TIE ports | Yes |
| User-defined TIE queues | Yes |
| **Option** | **Allowed?** |
| User-defined TIE lookups (with writes) | Yes |

## 10.6 Sample Configuration Templates for the ConnX BBE32EP DSP

Sample configuration templates are provided in XPG for ConnX BBE32EP DSP: XRC\_B32EP\_AO & XRC\_B32EP\_MIN.These templates contain a valid core configuration and different sets of optional ISA packages available for ConnX BBE32EP DSP. Effectively, these reference cores bracket a wide range of configuration possibilities for the ConnX BBE32EP DSP. Users can start with any of these templates and modify configuration options, within the constraints defined by XPG, until a required configuration is built.

XRC\_B32EP\_MIN is a minimal configuration DSP. It represents a configuration that might be well suited for limited scope problems such as matrix operations. XRC\_B32EP\_AO provides all option configuration.

For complete information on the base core that constitutes the XRC\_B32EP\_AO & XRC\_B32EP\_MIN configuration templates, see next section. Users are free to use any of these templates as a starting point, or not use a template at all, and then modify the configuration options for their ConnX BBE32EP DSP configuration within the constraints and assumptions defined in XPG and the build process.

### Base Configuration

*Table 40: ConnX BBE32EP DSP Base Configuration* on page 157 specifies the base core configuration in the XRC\_B32EP\_AO & XRC\_B32EP\_MIN templates, which uses Xtensa ISA version LX6.0 and Xtensa Exception Architecture 2 (XEA2). **Table 40: ConnX BBE32EP DSP Base Configuration**

|  |  |
| --- | --- |
| **Option** | **Available** |
| **Xtensa ISA version** | LX6.0 |
| **Instruction options** |  |
| 16-bit MAC with 40 bit Accumulator | No |
| MUL16 | Yes |
| MUL32 | Pipelined + UH/SH |
| 32 bit integer divider | Yes |
| Single Precision FP (coprocessor id 0) | No |

|  |  |
| --- | --- |
| **Option** | **Available** |
| Double Precision FP Accelerator | No |
| CLAMPS | Yes |
| NSA/NSAU | Yes |
| MIN/MAX and MINU/MAXU | Yes |
| SEXT | Yes |
| Boolean Registers | Yes |
| Number of Coprocessors (NCP) | 3 |
| Enable Density Instructions | Yes |
| Enable Processor ID | Yes |
| Zero-overhead loop instructions | Yes |
| Synchronize instruction | Yes |
| Conditional store synchronize instruction | Yes |
| TIE arbitrary byte enables | Yes |
| Count of Load/Store units | 2 |
| Max instruction width (bytes) | 12 |
| L32R hardware support option | Normal L32R |
| Pipeline length | 7 |
| **Thread Pointer** | No |
| **GPIO32: 32-bit GPIO interface** | No |
| **QIF32: 32-bit Queue Interface** | No |
| **Interrupts enabled?** | Yes |
| **Interrupt count** | 17 |
| Int 0 type / priority level | ExtLevel / 1 |
| Int 1 type / priority level | ExtLevel / 1 |
| Int 2 type / priority level | ExtLevel / 1 |
| Int 3 type / priority level | ExtLevel / 1 |
| Int 4 type / priority level | ExtLevel / 1 |

|  |  |
| --- | --- |
| **Option** | **Available** |
| Int 5 type / priority level | ExtLevel / 1 |
| Int 6 type / priority level | Timer / 1 |
| Int 7 type / priority level | Software / 1 |
| Int 8 type / priority level | ExtLevel / 2 |
| Int 9 type / priority level | ExtLevel / 2 |
| Int 10 type / priority level | Timer / 2 |
| Int 11 type / priority level | Software / 2 |
| Int 12 type / priority level | ExtEdge / 1 |
| Int 13 type / priority level | ExtEdge / 1 |
| Int 14 type / priority level | ExtEdge / 2 |
| Int 15 type / priority level | ExtEdge / 2 |
| Int 16 type / priority level | NMI / 4 |
| **High Priority Interrupts** | Yes |
| Interrupt Level count | 3 |
| **Medium Level Interrupts** | Yes |
| Highest Medium Interrupt Level | 2 |
| **Timer count** | Yes |
| Timer count | 2 |
| Timer 0 | 6 |
| Timer 1 | 10 |
| **Byte ordering (endianness)** | Little Endian |
| **Address registers available for call windows** | 32 |
| **Miscellaneous Special Register count** | 2 |
| **On unaligned load/store address** | Exception |
| **Enable Processor Interface (PIF)** | Yes |
| PIF version 3.2 | No |
| Asynchronous PIF interface | No |

|  |  |
| --- | --- |
| **Option** | **Available** |
| Write buffer entries | 2 |
| Enable PIF Write Responses | Yes |
| Prioritize Load Before Store | No |
| **Widths of Cache and Memory Interfaces** |  |
| Width of Instruction Fetch interface | 128 |
| Width of Data Memory/Cache interface | 256 |
| Width of PIF interface | 128 |
| **Instruction Cache** | Not Selected |
| **Data Cache** | Not Selected |
| **Debug** | Yes |
| Data address breakpoint registers | 2 |
| Instruction address breakpoint registers | 2 |
| Debug interrupt level | 3 |
| On Chip Debug(OCD) | Yes |
| *Use array of 4 Debug Instruction Registers (DIRs)* | Yes |
| *External Debug Interrupt* | Yes |
| Trace port (address trace and pipeline status) | Yes |
| *Add data trace* | No |
| **Xtensa Exception Architecture** | XEA2 |
| **Memory Protection/MMU** | Region Protection |
| System RAM start address / size | 0x60000000 - 0x63ffffff / 64M |
| System ROM start address / size | 0x50000000 - 0x50ffffff / 16M |
| **Inbound PIF request buffer depth** | 2 |
| **Local Memory** |  |
| Instruction RAM start address / size | 0x40000000 - 0x4001ffff / 128K |
| Instruction ROM start address / size | Not selected |
| Data RAM [0] start address / size | 0x3ffc0000 - 0x3ffdffff / 128K |

|  |  |
| --- | --- |
| **Option** | **Available** |
| Data RAM [1] start address / size | 0x3ffe0000 - 0x3fffffff / 128K |
| Data ROM start address / size | Not selected |
| XLMI start address / size | Not selected |
| **Vector configuration** |  |
| Reset Vector start address / size | 0x50000000 / 0x2e0 |
| Kernel (Stacked) Exception Vector start address / size | 0x400001dc / 0x1c |
| User (Program) Exception Vector start address / size | 0x400001fc / 0x1c |
| Double Exception Vector start address / size | 0x4000021c / 0x1c |
| Window Register Overflow Vector start address / size | 0x40000000 / 0x178 |
| Level 2 Interrupt Vector start address / size | 0x4000017c / 0x1c |
| Level 3 Interrupt Vector start address / size | 0x4000019c / 0x1c |
| Level 4 Interrupt Vector (NMI vector) start address / size | 0x400001bc / 0x1c |
| **Relocatable Vectors** | Yes |
| Selected Static Vector set | Primary |
| Primary Static Vector Group Base Address | 0x50000000 |
| Alternate Static Vector Group Base Address | 0x40000240 |
| Alternate Reset Vector Address | 0x40000240 |
| Default Dynamic Vector Group Base Address  (VECBASE) | 0x40000000 |
| **Target & CAD options [Nominal]** |  |
| RTL description | Verilog |
| Synthesis / P&R flow | Physical Synthesis, Route |
| Geometry / Process | 45gs / Worst |
| Core Speed | 976 MHz |
| User Defined Estimator Library | Default |
| *User Area Scaling factor* | 1.0 |
| *User Speed Scaling factor* | 1.0 |
| **Option** | **Available** |
| *User Dynamic Power Scaling factor* | 1.0 |
| *User Leak Power Scaling factor* | 1.0 |
| *User Area To Gate Scaling factor* | 1.0 |
| Functional Unit Clock Gating | Yes |
| Global Clock Gating | Yes |
| Register file implementation block (Latches are deprecated) | Flip-flops |
| Asynchronous Reset | No |
| Full scan | Yes |
| **Software Target Options** |  |
| Xtensa Tools should use Extended L32R | No |
| Software ABI | Windowed |
| C Libraries | Newlib |
| **Compatibility Checking** |  |
| Generic RTOS compatibility | No |
| Target Linux compatibility | No |

### Differences in Reference Configurations

The following table highlights all the differences between the reference configuration templates.

### Table 41: ConnX BBE32EP DSP Configuration Options

|  |  |  |  |
| --- | --- | --- | --- |
| **Option** | **MIN** | **AO** |  |
| Integer and fractional vector divide | No | Yes |
| Fast vector reciprocal & reciprocal square root | Yes | Yes |
| Advanced vector reciprocal & reciprocal square root | No | Yes |
| **Option** | **MIN** | **AO** |  |
| Bit-multiplication support for LFSR applications and convolutional encoding | No | Yes |
| 1D despread | No | Yes |
| Packed complex matrix multiply | No | Yes |
| Symmetric FIR | No | Yes |
| Linear block decoder | No | Yes |
| FFT | No | Yes |
| 3GPP soft-bit demap | No | Yes |
| Advanced precision multiply/add | No | Yes |
| Inverse LLR | No | Yes |
| Single and dual peak search | No | Yes |

## 10.7 Synthesis and Place-and-Route

When the ConnX BBE32EP DSP coprocessor is included in an Xtensa processor configuration, the synthesis and place-and-route scripts that are included with the software build can be used with the usual methodology, which is outlined in the *Xtensa LX Hardware User’s Guide*.

For timing closure between synthesis and place-route, Tensilica® recommends using a layout aware synthesis tool. Because of the data path-intensive nature of the ConnX BBE32EP DSP netlist, the tool may need to be further controlled with some of its parameters depending upon the process technology, the foundry, and the library vendor.

## 10.8 ConnX BBE32EP DSP Memory Floor-planning Suggestions

Tensilica® provides an automated, push-button, standard cells layout flow for ConnX

BBE32EP DSP. Before proceeding with floor-planning ConnX BBE32EP DSP with memory, it is imperative that out-of-the-box push-button flow is exercised for ConnX BBE32EP DSP. The advantage is that it will provide accurate measure of flop-to-flop timing inside ConnX BBE32EP DSP without extraneous factors such as specific memory placement or floor-plan dimensions that may cause layout tool to severely degrade the performance of ConnX BBE32EP DSP. Secondly, it helps identify potential timing issues on memory paths based on only on memory timing budgets. Finally, the flow provides an estimate of ConnX BBE32EP DSP size to determine the dimensions of floor-plan for ConnX BBE32EP DSP with its associated memories.

The ConnX BBE32EP DSP interfaces with instruction and data memories. The instruction memories interface with the Program Counter and Instruction Fetch (PCIF) unit in ConnX BBE32EP DSP. The data memories interface with the Load/Store (LS) unit in ConnX BBE32EP DSP. The layout tool attempts to place standard cells for PCIF and LS units close to instruction and data memories respectively. When all memory instances are placed on one side of the floor-plan, the tendency of layout tools to place standard cells of both PCIF and LS units close to the memory will likely cause congestion. When instruction memories are placed on one side and data memories on other, the congestion at memory interfaces is alleviated. As a general guideline:

* The data memory interfaces must be contiguous,
* Instruction memory interfaces must be contiguous,
* Data and instruction memory interfaces should not be intermixed.

The number of pipeline stages in ConnX BBE32EP DSP is 7. With large memories, the ConnX BBE32EP DSP is configured with 7-pipeline stages so that a full cycle is available to register the memory data. Specifying accurate memory timing budgets in synthesis ensures that netlist generated by synthesis tool right sizes the gates and inserts buffers to address the timing on memory interface paths. This minimizes the risk of congestion causing area increase on memory interface paths during layout.

ConnX BBE32EP DSP muxes data from each instruction cache way, instruction ram 0, and instruction ram 1 on the instruction memory interface. Depending on the instruction memory interface width, each of instruction cache way and instruction rams may be implemented as a bank of memory instances. For example, a 128-bit instruction memory interface may be implemented using four banks, each one with 32-bit interface. As data is muxed inside ConnX BBE32EP DSP, it is useful to interleave the memory banks. For example, consider

128-bit interface for instruction memory configuration of Instruction Ram 0 (IRam0) and Instruction Ram 1 (IRam1), each of which is 64 KB. Each instruction ram is implemented as 4 banks of 16 KB with 32-bit interface (IRam<N>\_0, IRam<N> \_1, IRam<N> \_2, and IRam<N> \_3). In this situation, the floor-plan should interleave IRam0 and IRam1 instances by placing corresponding bank instances next to each other (e.g. IRam0 \_0 next to IRam1 \_0). The same guideline is applicable for data memory interface. The data from ConnX BBE32EP DSP to memory (memory write data) and memory address signals need to be routed to each memory bank. For this 7-stage machine, the address setup path may be timing critical. In this situation, interleaving the memory banks may lead to accentuating the congestion and timing challenge at memory address and write data pins. In summary, interleaving instances for memory banks is one of the alternatives that should be carefully considered for creating ConnX BBE32EP DSP floor-plan.

In addition to considering memory interfaces, it is important to allocate sufficient floor-plan space for ConnX BBE32EP DSP. Our experience suggests that a starting utilization of 60% is a reasonable preliminary estimate for floor-planning experiments. The starting utilization refers to the floor-plan area allocated to ConnX BBE32EP DSP standard cells divided by the post-synthesis cell area. The starting utilization ratio need to be adjusted down if floor-plan dimensions and memory instance dimensions leads to notches and narrow channels. The ratio may be adjusted up if the design easily meets the timing.

As with any floor-plan creation, the design of the ConnX BBE32EP DSP floor-plan is partly art and partly science. The first level consideration must take into account the following:

* Size and aspect ratio of floor-plan space allocated to ConnX BBE32EP DSP and associated memories
* Data and instruction memory configuration
* Relative size of ConnX BBE32EP DSP and its associated memories

At this stage it is important to estimate the memory sizes and floor-plan real estate available to place ConnX BBE32EP DSP standard cells. It is important that sufficient notch-free and channel-free space is available for placement of the ConnX BBE32EP DSP standard cells.

The next level of detail must evaluate banking structure to implement the ConnX BBE32EP DSP memory configuration. The banking may be along "data bits" or "address bits" (for example, top 2 bits to select data from one of the four banks) or both. The memory banking enables use of smaller memory instances instead of a large monolithic instance or may be the only choice when it is not possible to generate a monolithic instance. In addition, the column mux ratio may be employed as a parameter to determine the memory instance dimensions in the context of the floor-plan dimensions.

The logical connectivity of instruction and data memories to ConnX BBE32EP DSP units must be exploited to place the memory instances. It is important to differentiate type of memories from the ConnX BBE32EP DSP perspective and place memory of a specific type contiguously in the floor-plan. The interleaving of memory bank instances must be considered to improve the congestion characteristics of the floor-plan.

For optimal congestion-free ConnX BBE32EP DSP floor-plan that meets the timing, the importance of early, floor-plan exploration experiments cannot be overemphasized.

## 10.9 Mapping the ConnX BBE32EP DSP to FPGA

Experiments have been carried out mapping the ConnX BBE32EP DSP to Xilinx Kintex/ Virtex 7 FPGAs. The characterization runs were carried out on ConnX BBE32EP DSP configurations from a previous release (RE-2014.0). These results, as shown in the tables below, should be similar to a ConnX BBE32EP DSP configuration in the current RG-2016.4 release.

**ConnX BBE32EP DSP Synthesis - Xilinx**

### Table 42: Xilinx Synthesis Results (RE-2014.0)

|  |  |  |  |  |
| --- | --- | --- | --- | --- |
| **Config** | **Device** | **Utilization** | **Target Frequency** | **Achieved**  **Frequency** |
| XRC\_B32EP\_AO | 7vx485t | 90 | 33.0MHz | 78.69MHz |

![](data:image/png;base64...)**Note:** The XRC\_B32EP\_AO configuration used for the FPGA mapping experiment does not include the following configurable options:

* Soft-bit demapping
* Advanced precision multiply/add
* Inverse log-likelihood ratio (LLR)
* Single-precision vector floating-point

# 11. On-Line ISA, Protos and Configuration Information

Xtensa Xplorer offers a number of on-line Instruction Set Architecture (ISA) documentation references in HTML format for Xtensa configurations, including the ConnX BBE32EP DSP. These references are accessed from the Xplorer’s Configuration Overview window by clicking the View Details button from the Installed Builds column.

Note: Be sure to click the View Details button from the

Installed Builds column, and not from the Workspace Config column, which displays configuration information only.

When you click View Details, an HTML page is displayed within Xplorer (on Windows) and in your web browser (on Linux). From this page you can access a number of HTML pages describing the ISA, protos, and other

documentation information. These are listed under the Instruction Set Architecture area and the Hardware Documentation area. The information offered includes:

* Instruction Formats, including a complete operation slot assignment table called the “Operation Slot Compatibility Matrix”
* Instruction Descriptions, sometimes informally called “ISA HTML”. This is an HTML page describing each instruction in some detail
* Instruction Opcodes
* C-Types, Operators, and Instruction Prototypes (the latter also known as “Protos”)
* List of Prototypes (Categorized by Functional Class)
* Description of Prototypes (C Function Protoype, TIE Definitions)
* Instruction Pipelining
* State List for ISA internal state
* A hardware Port List for the configuration, under the Hardware Documentation area

The Instruction Descriptions contain live links to the Instruction Prototypes within which these instructions are used. In addition, the Instruction Prototypes contain live links to the particular instruction descriptions that they use.