# PFD
This is an official implementation of Polarity-Focused Denoising (PFD) method for event cameras.

Event cameras, which are highly sensitive to light intensity changes, often generate substantial noise during imaging. Existing denoising methods either lack the speed for real-time processing or struggle with dynamic scenes, mistakenly discarding valid events. To address these issues, we propose a novel dual-stage polarity-focused denoising (PFD) method that leverages the consistency of polarity and its changes within local pixel areas. Whether due to camera motion or dynamic scene changes, the polarity and its changes in triggered events are highly correlated with these movements, allowing for effective noise handling. We introduce two versions: PFD-A, which excels at reducing background activity (BA) noise, and PFD-B, which is designed to address both BA and flicker noise. Both versions are lightweight and computationally efficient. The experimental results show that PFD outperforms benchmark methods in terms of the SNR and ESR metrics, achieving state-of-the-art performance across various datasets. Additionally, we propose an FPGA implementation of PFD processes that handles each event in just 7 clock cycles, ensuring real-time performance.

# Installation
The PFD refers to the group of event processing version, while the PFDs refers to the event-by-event processing version.
Install the dependencies with

```
mkdir build
cd build
cmake ..
./PFD
./PFDs
```

****
