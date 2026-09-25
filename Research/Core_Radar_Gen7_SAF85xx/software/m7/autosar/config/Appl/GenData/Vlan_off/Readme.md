To generate the EthIF and MKA configuration for VLAN OFF package:
1- Apply the No_Vlan_changes.patch on the repo using command "git apply No_Vlan_Changes.patch"
2- Reload DaVinci project as popped up
3- regenerate EthIf and MKA
4- Copy the generated untracked files out of GenData and into Vlan_Off folder.
5- Undo the ARXML changes.