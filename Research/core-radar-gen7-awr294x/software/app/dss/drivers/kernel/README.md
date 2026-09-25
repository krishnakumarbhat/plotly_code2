# TI Kernel Module
This module was directly copied from TI library. In order to reduce power consumption, Aptiv has implemented Clock gating/switching. This change was suggested by TI. In order to implement these changes successfully, we must make sure C66 ISRs are running at the fast clock speed. That is the only modification to this module. All else remains the same.
