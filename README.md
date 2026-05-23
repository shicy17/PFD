# PFD: Polarity-Focused Denoising for Event Cameras

[![Paper](https://img.shields.io/badge/IEEE%20TCSVT-2025-blue)](https://doi.org/10.1109/TCSVT.2024.3519430)
[![FPGA](https://img.shields.io/badge/FPGA-7%20cycles%2Fevent-green)](https://doi.org/10.1109/TCSVT.2024.3519430)

Official implementation of *"Polarity-Focused Denoising for Event Cameras"*, published in **IEEE Transactions on Circuits and Systems for Video Technology**, Vol. 35, No. 5, 2025.

---

## Overview

Event cameras, which are highly sensitive to light intensity changes, often generate substantial noise during imaging. Existing denoising methods either lack the speed for real-time processing or struggle with dynamic scenes, mistakenly discarding valid events. To address these issues, we propose a novel dual-stage **Polarity-Focused Denoising (PFD)** method that leverages the consistency of polarity and its changes within local pixel areas. Whether due to camera motion or dynamic scene changes, the polarity and its changes in triggered events are highly correlated with these movements, allowing for effective noise handling.

We introduce two variants:

- **PFD-A** — excels at reducing background activity (BA) noise.
- **PFD-B** — addresses both BA and flicker noise.

Both variants are lightweight and computationally efficient. Experimental results show that PFD outperforms benchmark methods in terms of SNR and ESR metrics, achieving state-of-the-art performance across various datasets. Additionally, we propose an FPGA implementation that handles each event in just **7 clock cycles**, ensuring real-time performance.

---

## Repository Contents

This repository provides two C++ reference implementations of the PFD algorithm:

| Executable | Mode | Description |
|---|---|---|
| `PFD` | Group processing | Processes events in batches — higher throughput |
| `PFDs` | Event-by-event | Streams events one at a time — lower latency, FPGA-aligned |

The FPGA implementation described in the paper follows the same pipeline as `PFDs`.

---

## Installation

### Dependencies

A C++ compiler with CMake (≥ 3.10) is required.

### Build

```bash
mkdir build
cd build
cmake ..
make
```

### Run

```bash
./PFD     # group-processing version
./PFDs    # event-by-event version
```

---

## Citation

If you use PFD in a published article or project, please cite:

> C. Shi et al., "Polarity-Focused Denoising for Event Cameras," in *IEEE Transactions on Circuits and Systems for Video Technology*, vol. 35, no. 5, pp. 4370–4383, 2025, doi: 10.1109/TCSVT.2024.3519430.

```bibtex
@ARTICLE{10804847,
  author  = {Shi, Chenyang and Wei, Boyi and Wang, Xiucheng and Liu, Hanxiao and Zhang, Yibo and Li, Wenzhuo and Song, Ningfang and Jin, Jing},
  journal = {IEEE Transactions on Circuits and Systems for Video Technology},
  title   = {Polarity-Focused Denoising for Event Cameras},
  year    = {2025},
  volume  = {35},
  number  = {5},
  pages   = {4370-4383},
  doi     = {10.1109/TCSVT.2024.3519430},
  keywords = {Noise reduction; Noise; Cameras; Spatiotemporal phenomena; Phase frequency detectors; Real-time systems; Circuits and systems; Hardware; 1/f noise; Field programmable gate arrays; Event camera; denoising method; BA noise; flicker noise}
}
```

---

## :clap: Acknowledgement

We gratefully acknowledge Professor Tobi Delbruck for implementing the [PFD algorithm](https://github.com/SensorsINI/jaer/blob/master/src/net/sf/jaer/eventprocessing/filter/SpatioTemporalCorrelationFilter.java) in the [jaer](https://github.com/SensorsINI/jaer) project.
