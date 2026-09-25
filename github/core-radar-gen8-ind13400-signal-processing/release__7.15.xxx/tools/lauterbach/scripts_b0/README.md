# Testing scripts for Lauterbach

Indie provided a framework for testing the various portions of code. This test setup is based on that delivery and enables more generic programming of the BBE for testing.

An R52 image is provided that sets up the Chandra in a known state for testing. This includes the PLL setup that is required for the BBE to be enabled. The ELF image for the R52 is provided, and it is intentional that no R52 code is included in this repository for rebuilding this application.

The _start_powerview.bat script should be used to start the Trace32 application, as it will set the configuration based on the config.t32 and will also call the start_powerview.cmm script to enable the menus and setup for the Trace32 environment.

There are two buttons added to the Toolbar in Trace32, LR and Power. The LR button will bring up a dialog box that will allow selection of elf images for the R52 core and the BBE core. Clicking OK will load the images to RAM, and initialize the R52 and BBE instances of the Trace32 program.

# File list and uses

## _start_powerview.bat
Script used to start Trace32 with the appropriate configuration and startup script for menu configuration.

## chandra.men
Provides the menu interface to select the Chandra special register definitions.

## chandra.per
The peripheral file for the Chandra device that defines the peripheral register addresses.

## chandra_bbe32_init.cmm
Script from Indie that configures the Trace32 instance for the BBE interface, loads the peripheral addresses, and loads the ELF image to the BBE.

## chandra_r52_init.cmm
Script from Indie that configures the Trace32 instance for the R52 interface, loads the peripheral addresses, and sets up the ARM for running from RAM.

## config.t32
Configures the TCP access settings for use with the Python driver module for controlling T32.

## default_win_bbe.cmm
Sets up the BBE instance of Trace32 child windows in a default configuration.

## default_win_r52.cmm
Sets up the R52 instance of Trace32 child windows in a default configuration.

## last_flash_session.ini
Created by the load_chandra_elfs.cmm to store the last selected files for RAM initialization for the R52 and BBE.

## load_bbe32_elf.cmm
Calls the chandra_bbe32_init.cmm script and loads the BBE32 elf file.

## load_chandra_elfs.cmm
Calls the chandra_bbe32_init.cmm script and loads the BBE32 elf file.
Loads the dialog for selecting the images to load to the R52 and BBE cores. Starts the R52 core upon successful loading of the R52 image, validates that the PLL lock has happened (part of the R52 image), calls the load_bbe32_elf.cmm script to load the BBE and then starts the BBE core. Delays added to allow the StopAndGo debug mode to show updates to the variables upon starting the session.

## load_r52_elf.cmm
Calls the chandra_r52_init.cmm script and loads the R52 elf file.

## mainmenu.men
Adds the toolbar items for "LR" and Power Button.
   LR = Load Images To RAM
   Power Button = Closes both instances of Trace32

## start_powerview.cmm
Names the Trace32 instances, maximizes the windows, resets the cores, and initializes the windows for the two instances. Calls the "Load Images To RAM" script from mainmenu.men.
