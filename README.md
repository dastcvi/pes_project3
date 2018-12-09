# pes_project3
This is the repository for my solution to the third project for CU Boulder's ECEN 5813 Principles of Embedded Software class.

The project involves setting up a peripheral ADC to run continuously and trigger DMA requests so that an application can do peak-level metering on the ADC data. The target hardware is NXP's FRDM-KL25Z development board, which hosts an MKL25Z128VLK4 MCU. The MCU is based on an ARM Cortex-M0+ running at 48 MHz.

The base for this project is my solution to project 2 (see my repo pes_project2) so that I could bring in the interrupt-driven UART implementation.
