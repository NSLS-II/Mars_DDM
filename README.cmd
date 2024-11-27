This is the EPICS IOC for MARS ASIC based Germanium detector with 96, 192, and 384 elements.

Firmware is included in `boot/`. Please follow `boot/Firmware Update for Germanium Detector.docx` to update.

For UDP data streaming to work properly, please install:

- The auxiliary IOC that provides extra PVs:

  - https://github.com/NSLS-II/germDaemon

- The UDP agent program that writes UDP data files:

  - https://github.com/NSLS-II/germ_agent

