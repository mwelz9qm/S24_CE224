# CE 224 Final Project

CE 224, Adv. Algorithms and Data Structures

Lincoln Scheer

Spring 2024

Fort Lewis College

## Overview
This project is a implementation of the backpropagation algorithm in python. To demonstrate this algorithm a fully-connected feed forward neural network is implemented. To view the development log, go to the [development](dev.md) markdown document. 

## Table of Contents

- [CE 224 Final Project](#ce-224-final-project)
  - [Overview](#overview)
  - [Table of Contents](#table-of-contents)
  - [Running](#running)
  - [Features](#features)
  - [Description](#description)
  - [Documentation](#documentation)
    - [Network Functions](#network-functions)
    - [Network Class](#network-class)
  - [Usage](#usage)
    - [Step 1: User-Defined Activation Functions](#step-1-user-defined-activation-functions)
    - [Step 2: Define Network Architecture](#step-2-define-network-architecture)
    - [Step 3: Generate Network](#step-3-generate-network)
    - [Step 4: Define Dataset](#step-4-define-dataset)
    - [Step 5: Train Network](#step-5-train-network)

## Running

To run the project you can download the attached `.py` or `.ipynb` files in the same directory as this README. Dependencies include `numpy` for numerics, and `matplotlib` for plotting in both implementations of the project.


## Features
- Backpropagation Algorithm
- Feed Forward Neural Network
- Training Example

## Description

This Jupyter Notebook / Python Script contains an example implementation of a neural network using Python. It includes functions for forward and backward propagation, various activation functions, loss functions, and a Network class for constructing and training neural networks.

## Documentation

For the complete documentation, please see the [documentation](doc.md) markdown document.

### Network Functions

- `forward_pass(x, W, b, f)`: Performs forward propagation through the network.
- `backward_pass(C, fp, Z, A, W, b, lr)`: Performs backpropagation to update weights and biases.
- `log_loss(Y_hat, Y)`: Computes the cross-entropy log loss.
- `sel_loss(Y_hat, Y)`: Computes the squared error loss.
- `loss(Y_hat, Y_pred, func=sel_loss)`: Applies the specified loss function.
  
### Network Class

- `Network(size, activations, activationsp)`: Represents a neural network with specified architecture.
- `train(x, y, epochs=10, lr=0.01)`: Trains the neural network using input data and labels.
- `predict(x)`: Performs inference using the trained network.

## Usage

### Step 1: User-Defined Activation Functions

- Define activation functions such as sigmoid and linear functions.

### Step 2: Define Network Architecture

- Specify the network architecture including layer sizes and activation functions.

### Step 3: Generate Network

- Create an instance of the Network class with the specified architecture.

### Step 4: Define Dataset

- Define the input data and corresponding labels.

### Step 5: Train Network

- Train the network using the defined dataset and parameters.

