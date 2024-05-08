## Brief

Implementation of Pixel Subsampling, Area Lighting and Importance Sampling.

## Pixel Subsampling

```math
\textbf{p}_{xy} = \textbf{t}_l + \left( \cfrac{x + \xi}{i_w} \right) \cdot (v_w \textbf{u}) + \left( \cfrac{y + \xi}{i_h} \right) (-v_h \textbf{v})
```

## Importance Sampling

- Uniform Hemisphere Sampling

```math
\langle L_o (\textbf{x}, \boldsymbol{\omega}_o) \rangle = \cfrac{2 \pi}{N} \sum_{i = 1}^N f(\textbf{x}, \boldsymbol{\omega}_o, \boldsymbol{\omega}_i) L_i(\textbf{x}, \boldsymbol{\omega}_i) \cos \theta
```

- Cosine Sampling

```math
\langle L_o (\textbf{x}, \boldsymbol{\omega}_o) \rangle = \cfrac{\pi}{N} \sum_{i = 1}^N f(\textbf{x}, \boldsymbol{\omega}_o, \boldsymbol{\omega}_i) L_i(\textbf{x}, \boldsymbol{\omega}_i)
```

- Area Light Sampling

```math
\langle L_o (\textbf{p}, \boldsymbol{\omega}_o) \rangle = \cfrac{\lvert A \rvert}{N} \sum_{i = 1}^N \cfrac{ f(\textbf{p}, \boldsymbol{\omega}_o, \boldsymbol{\omega}_i) L_i(\textbf{p}, \boldsymbol{\omega}_i) \cos \theta \cos \theta_l }{ r^2 }
```

## Instructions

### Steps to build

```shell
git clone --recursive https://github.com/ihsingh2/simple_renderer
```

```shell
mkdir build
cd build
cmake ..
make -j8
```

### Steps to run

```shell
git clone https://github.com/ihsingh2/scenes
```

```shell
./build/render <scene_path> <out_path> <num_samples> <sampling_strategy>
```
