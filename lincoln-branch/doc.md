## Complete Documentation

### Network Functions

- **`forward_pass(x, W, b, f)`**
  - Forward propagation through the network.
  - **Parameters**:
    - `x`: Input row vector array.
    - `W`: Network weight matrix array.
    - `b`: Network bias row vector array.
    - `f`: Network activation function array.
  - **Returns**:
    - `Z`: Array of pre-activated neuron row vectors.
    - `A`: Array of activated neuron row vectors.

- **`backward_pass(C, fp, Z, A, W, b, lr)`**
  - Backpropagation to update weights and biases.
  - **Parameters**:
    - `C`: Cost of network performance on a training set.
    - `fp`: Derivatives of activation functions used.
    - `Z`: Array of pre-activated neuron row vectors.
    - `A`: Array of activated neuron row vectors.
    - `W`: Network weight matrix array.
    - `b`: Network bias row vector array.
    - `lr`: Learning rate.
  - **Returns**:
    - `W_new`: New network weight matrix array.
    - `b_new`: New network bias row vector array.

- **`log_loss(Y_hat, Y)`**
  - Applies the cross-entropy log loss function.
  - **Parameters**:
    - `Y_hat`: Labeled true values.
    - `Y`: Predicted values.
  - **Returns**:
    - Log loss result.

- **`sel_loss(Y_hat, Y)`**
  - Applies the squared error loss function.
  - **Parameters**:
    - `Y_hat`: Labeled true values.
    - `Y`: Predicted values.
  - **Returns**:
    - Squared error loss result.

- **`loss(Y_hat, Y_pred, func=sel_loss)`**
  - Applies the specified loss function.
  - **Parameters**:
    - `Y_hat`: Labeled true values.
    - `Y_pred`: Predicted values.
    - `func`: Loss function (default is `sel_loss`).

### Network Class

- **`Network(size, activations, activationsp)`**
  - Represents the architecture of a neural network.
  - **Parameters**:
    - `size`: Array of layer sizes.
    - `activations`: Array of lambda functions of activations.
    - `activationsp`: Array of lambda functions of activation derivatives.

- **`train(x, y, epochs=10, lr=0.01)`**
  - Trains the neural network.
  - **Parameters**:
    - `x`: Array of input data.
    - `y`: Array of labeled output data.
    - `epochs`: Number of iterations to train the network (default is 10).
    - `lr`: Initial learning rate (default is 0.01).

- **`predict(x)`**
  - Performs inference on input data.
  - **Parameters**:
    - `x`: Input row vector.
  - **Returns**:
    - Output row vector.
