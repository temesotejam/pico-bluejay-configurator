# Attribution and implementation status

Source adapted from [BrushlessPower/BlHeli-Passthrough](https://github.com/BrushlessPower/BlHeli-Passthrough), directory `RP2040/`, under GNU GPL version 3. The full GPL license is included as [LICENSE](LICENSE).

Local changes:
- Set RP2040 ESC GPIO sequence to FR=15, FL=14, RR=3, RL=2 to match `temesotejam/pico-copter-fc`.
- Support SiLabs EFM8 Bluejay / BLHeli_S bootloader signatures and 4-way interface mode 1.
- Set SiLabs flash-page addressing to 512-byte pages rather than the ARM 1024-byte layout.
- Validate boot response length and signature before accepting a device (prevents indexing a zero-length response).
- Do not claim unsupported verify commands succeeded.

Implementation has not yet been hardware-tested on the intended ESC. Do not flash ESC firmware until the Pico interface has been validated with physical hardware. See [README](README.md).
