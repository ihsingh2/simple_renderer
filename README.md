## Brief

Implementation of Direct Lighting and Texture Mapping.

## Direct Lighting

- Point Light

```math
L_o (\textbf{x}, \boldsymbol{\omega}_o) = \sum_{\textbf{p} \in \mathcal{P}} f \left( \textbf{x}, \boldsymbol{\omega}_o, \cfrac{\textbf{p} - \textbf{x}}{\lvert \textbf{p} - \textbf{x} \rvert} \right) \cfrac{L_i \left( \textbf{x}, \cfrac{\textbf{p} - \textbf{x}}{\lvert \textbf{p} - \textbf{x} \rvert} \right)}{\lvert \textbf{p} - \textbf{x} \rvert ^ 2} \left( \cfrac{\textbf{p} - \textbf{x}}{\lvert \textbf{p} - \textbf{x} \rvert} \cdot \textbf{n} \right)
```

- Directional Light

```math
L_o (\textbf{x}, \boldsymbol{\omega}_o) = \sum_{\boldsymbol{\omega} \in \mathcal{D}} f(\textbf{x}, \boldsymbol{\omega}_o, \boldsymbol{\omega}) L_i(\textbf{x}, \boldsymbol{\omega}) (\boldsymbol{\omega} \cdot \textbf{n})
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
./build/render <scene_path> <out_path> <interpolation_variant>
```
