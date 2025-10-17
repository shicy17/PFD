# Polarity-Focused Denoising for Event Cameras
This is an official implementation of Polarity-Focused Denoising (PFD) method for event cameras.

Event cameras, which are highly sensitive to light intensity changes, often generate substantial noise during imaging. Existing denoising methods either lack the speed for real-time processing or struggle with dynamic scenes, mistakenly discarding valid events. To address these issues, we propose a novel dual-stage polarity-focused denoising (PFD) method that leverages the consistency of polarity and its changes within local pixel areas. Whether due to camera motion or dynamic scene changes, the polarity and its changes in triggered events are highly correlated with these movements, allowing for effective noise handling. We introduce two versions: PFD-A, which excels at reducing background activity (BA) noise, and PFD-B, which is designed to address both BA and flicker noise. Both versions are lightweight and computationally efficient. The experimental results show that PFD outperforms benchmark methods in terms of the SNR and ESR metrics, achieving state-of-the-art performance across various datasets. Additionally, we propose an FPGA implementation of PFD processes that handles each event in just 7 clock cycles, ensuring real-time performance.

****
# 👉Citation   

Citations are welcome, and if you use all or part of our codes in a published article or project, please cite: 

C. Shi et al., "Polarity-Focused Denoising for Event Cameras," in IEEE Transactions on Circuits and Systems for Video Technology, doi: 10.1109/TCSVT.2024.3519430.

BibTeX of the paper:  
```
@ARTICLE{10804847,
  author={Shi, Chenyang and Wei, Boyi and Wang, Xiucheng and Liu, Hanxiao and Zhang, Yibo and Li, Wenzhuo and Song, Ningfang and Jin, Jing},
  journal={IEEE Transactions on Circuits and Systems for Video Technology}, 
  title={Polarity-Focused Denoising for Event Cameras}, 
  year={2025},
  volume={35},
  number={5},
  pages={4370-4383},
  keywords={Noise reduction;Noise;Cameras;Spatiotemporal phenomena;Phase frequency detectors;Real-time systems;Circuits and systems;Hardware;1/f noise;Field programmable gate arrays;Event camera;denoising method;BA noise;flicker noise},
  doi={10.1109/TCSVT.2024.3519430}}
```

****

# Installation
The PFD refers to the group of event processing version, while the PFDs refers to the event-by-event processing version.
Install the dependencies with

```
mkdir build
cd build
cmake ..
make
./PFD
./PFDs
```

****

# :clap: Acknowledgement
We gratefully acknowledge Professor Tobi Delbruck for implementing [PFD](https://github.com/SensorsINI/jaer/blob/master/src/net/sf/jaer/eventprocessing/filter/SpatioTemporalCorrelationFilter.java) algorithm in the [jaer](https://github.com/SensorsINI/jaer) project.
