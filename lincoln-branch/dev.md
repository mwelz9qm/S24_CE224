# Development Log

This file is comprised of the development markdown document used to implement this project. Because this project was initially planned in C++ some of the information in this file is meaningless in the context of the actual program but shows the though process. A separate matrix library was developed to aid in the C++ development project, that file was scrapped, but is included in this repository.

# Homework 8 Update

##### Core Classes

**Matrix**
`Mat`-Matrix Base Class

- `Identity` - Identity Matrix Implementation
- `Random` - Random Matrix Implementation
- `Ones` -  1's Matrix Implementation
- `Zeros` -  0's Matrix Implementation

**Network**
`MLP` - Base Multi-Layer Perceptron Class

- `fit()`
- `score()`
- `predict()`
  `RegressionMLP` - Regression Specific MLP Implementation
  `ClassificationMLP` - Regression Specific MLP Implementation
  `Layer`- Layer of Neurons Class

**Utility**
`namespace activations`- Implementation of Some Common Activation Functions and Their Derivatives

- `identity(x)`
- `unitStep(x)`
- `sigmoid(x)`
- `tanh`

`Utility`- Base Utility Class

- `trainTestSplit()`
- `normalize()`
- `augment()`

##### Update

**Items Accomplished**

- Programed Matrix Library
- Beginning Implementing Neural Network Class
- Devised Backpropagation Strategy

**Challenges**

- Faced issues with indexing with matrix library.
- Error propagation through network is still a struggle.

# Homework 9 Update

For this past two weeks I have been working on wrapping my head around the backpropagation algorithm, and different ways to implement it. It looks like the most common way for perceptron is to calculate the finite differences to determine the error contribution from all the weights and biases. The backpropagation algorithm allows us to not compute finite differences by instead applying the chain rule to account for these error factors.

The next two sections are my plan for implementing both the forward and backward propagation of the network. **Very little code has been implemented on the neural network side as I was more concerned with implementing the right thing.** I am now on the right track for implementing this and will stop by Matt's office hours during the last week of classes if I run into roadblocks with this process.

## Neural Network Implementation

I will focus on a multi-layer perceptron for this project. Generally we write a $L$-layer neural network in the form of $R-S^1-S^2-S^3-\cdots-S^l-\cdots-S^L$ to denote the structure of the network. In this form, $R$ is the number of inputs to the network, and $S^l$ denotes the number of neurons in the $l$ layer of the neural network. Also to note, $S^L$ is the number of neurons in the output layer which traditionally has a linear activation applied.

### Forward Propagation

#### Definitions

The superscripts denote the layer of neural network, not exponentiation.

$X$ - the $[R\times 1]$ input row vector.
$W^1$ - the $[S^1\times R]$ base case weight matrix of the first layer of neurons.
$W^l$ - the $[S^l \times S^{l-1}]$ generalized weight matrix for arbitrary layer $l$.
$B^l$ - the $[S^l\times 1]$ generalized bias row vector for arbitrary layer $l$.
$Z^l$ - the $[S^l \times 1]$ generalized pre-activated neuron row vector for arbitrary layer $l$.
$A^l$ - the $[S^l \times 1]$ generalized activated neuron row vector for arbitrary layer $l$.
$x_i$ - $i$th input to the network.
$w^{1}_{jk}$ - weight from $k$th input to the $j$th neuron in layer $2$.
**$w^{l}_{jk}$** - weight from $k$th neuron in layer $l-1$ to the $j$th neuron in layer $l$.
$b_j^l$ - bias of $j$th neuron in layer $l$.
$a_k^{l-1}$ - activation of the $k$th neuron in layer $l-1$.
$a_j^{l}$ - activation of the $j$th neuron in layer $l$.

The pre-activation of a neuron row vector $Z^l$ for an arbitrary layer $l$ is defined by,

$$
A^l=\begin{cases} 
      W^1X+B^1 & l = 1 \\
      W^lA^{l-1}+B^l & l < 1
   \end{cases}
$$

And the activation is defined as

$$
A^l=f^l(Z^l)
$$

This is computed through simple matrix multiplication and addition.

### Backpropagation

#### Definitions

$\delta _j^l$ - $\delta$ of $j$th neuron in layer $l$.
$p_q$ - input row vector of predictors.
$t_q$ - output row vector of desired $A^L$ given inputs $p_q$.
$\partial C/\partial w_{jk}^l$ - cost of network w.r.t. the weight $w^{l}_{jk}$.
$\partial C/\partial b_{j}^l$ - cost of network w.r.t. the bias $b^{l}_{j}$.
$\eta$ = learning rate of network

#### Algorithm

1. Initialize the neural network with random weights and biases.
2. For each set of $\{ p_q, t_q \}$ predictor and target training pairs in $\{P_Q, T_Q\}$
   1. Complete Forward Propagation Given $p_q$ as an input row vector $X$ for the neural network.
   2. Compute for all $j$ in layer $L$
      $\delta_j^L=2(t_j^L-a^L_j)a^L_j(1-a^L_j)$
   3. Compute for all $j$ in non output layers $l$
      $\delta^l_j=\sum_k \delta_k^{l-1}w^{l-1}_{kj}a^l_j(1-a^l_j)$
   4. Compute costs w.r.t all weights and biases
      $\frac{\partial C_{p_q}}{\partial w_{jk}^l}=\delta^l_ja^{l-1}_j$
      $\frac{\partial C_{p_q}}{\partial b_{j}^l}=\delta^l_j$
   5. Compute average costs across all training sets  $\{P_Q, T_Q\}$.
      $\frac{\partial C_{p_Q}}{\partial w_{jk}^l}=\frac{1}{Q}\sum_{q=1}^Q\frac{\partial C_{p_q}}{\partial w_{jk}^l}$
      $\frac{\partial C_{p_Q}}{\partial b_{j}^l}=\frac{1}{Q}\sum_{q=1}^Q\frac{\partial C_{p_q}}{\partial b_{j}^l}$
   6. Update weights and biases
      $w_{jk}^l\rightarrow w_{jk}^l=w_{jk}^l-\eta \frac{\partial C_{p_Q}}{\partial w_{jk}^l}$
      $b_{j}^l\rightarrow b_{j}^l=b_{j}^l-\eta \frac{\partial C_{p_Q}}{\partial b_{j}^l}$
