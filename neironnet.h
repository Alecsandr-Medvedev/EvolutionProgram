#ifndef NEIRONNET_H
#define NEIRONNET_H
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <functional>


class NeironNet
{

    public:
        enum class ActivationFunctionType {
        Sigmoid,
        ReLU,
        Tanh
    };
        NeironNet(const std::vector<int>& layer_sizes, ActivationFunctionType activation_function_type = ActivationFunctionType::Sigmoid, const std::vector<std::vector<std::vector<double>>>& initial_weights = {});
        virtual ~NeironNet();

        double activationFunction(double x);
        std::vector<double> forward(const std::vector<double>& input);
        void Mutate(double mutation_chance, double max_mutation_amount);
        void setActivationFunction(ActivationFunctionType activation_function_type);
        std::vector<std::vector<std::vector<double>>> printWeights() const;

    protected:

    private:
        std::vector<std::vector<std::vector<double>>> _weights;
        std::vector<std::vector<double>> _biases;
        std::vector<std::vector<double>> _activations;
        std::function<double(double)> _activation_function;

        double getRandomWeight(int seed = -1);
        bool shouldMutate(double mutation_chance);
        double getRandomMutation(double max_mutation_amount);

};

#endif // NEIRONNET_H
