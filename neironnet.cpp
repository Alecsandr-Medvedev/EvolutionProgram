#include "neironnet.h"

NeironNet::NeironNet(const std::vector<int>& layer_sizes, ActivationFunctionType activation_function_type, const std::vector<std::vector<std::vector<double>>>& initial_weights) {
        assert(layer_sizes.size() >= 2);
        assert(layer_sizes.size() == initial_weights.size() + 1 || !initial_weights.size());

        // Устанавливаем функцию активации в соответствии с выбранным типом
        setActivationFunction(activation_function_type);

        // Инициализируем веса и смещения для каждого слоя
        for (size_t i = 1; i < layer_sizes.size(); ++i) {
            int input_size = layer_sizes[i - 1];
            int output_size = layer_sizes[i];
            _weights.emplace_back(std::vector<std::vector<double>>(output_size, std::vector<double>(input_size, 0.0)));

            // Если переданы инициализационные веса, используем их
            if (i - 1 < initial_weights.size()) {
                _weights[i - 1] = initial_weights[i - 1];
            }
            // Иначе генерируем случайные веса
            else {
                for (int j = 0; j < output_size; ++j) {
                    for (int k = 0; k < input_size; ++k) {
                        _weights[i - 1][j][k] = getRandomWeight();
                    }
                }
            }

            _biases.emplace_back(std::vector<double>(output_size, 0.0));
        }

        // Инициализируем активации для каждого слоя
        for (size_t i = 0; i < layer_sizes.size(); ++i) {
            _activations.emplace_back(std::vector<double>(layer_sizes[i], 0.0));
        }
    }

double NeironNet::activationFunction(double x) {
        return _activation_function(x);
    }

std::vector<double> NeironNet::forward(const std::vector<double>& input) {

        if (input.size() == _activations[0].size()){
            // Копируем вход в активации входного слоя
            for (size_t i = 0; i < input.size(); ++i) {
                _activations[0][i] = input[i];
            }

            // Вычисляем активации для каждого слоя
            for (size_t i = 1; i < _activations.size(); ++i) {
                const auto& prev_layer = _activations[i - 1];
                auto& current_layer = _activations[i];
                auto& current_weights = _weights[i - 1];
                auto& current_biases = _biases[i - 1];

                for (size_t j = 0; j < current_layer.size(); ++j) {
                    double sum = 0.0;
                    for (size_t k = 0; k < prev_layer.size(); ++k) {
                        sum += current_weights[j][k] * prev_layer[k];
                    }
                    sum += current_biases[j];
                    current_layer[j] = activationFunction(sum);
                }
            }

            return _activations.back();
        }
        else{
            std::cout << input.size() << " " << _activations[0].size() << "\n";
            assert(input.size() == _activations[0].size());
        }


    }

void NeironNet::Mutate(double mutation_chance, double max_mutation_amount) {
        for (auto& weight_matrix : _weights) {
            for (auto& row : weight_matrix) {
                for (double& weight : row) {
                    if (shouldMutate(mutation_chance)) {
                        weight += getRandomMutation(max_mutation_amount);
                    }
                }
            }
        }
    }

void NeironNet::setActivationFunction(ActivationFunctionType activation_function_type) {
        switch (activation_function_type) {
            case ActivationFunctionType::Sigmoid:
                _activation_function = [](double x) { return 1.0 / (1.0 + std::exp(-x)); };
                break;
            case ActivationFunctionType::ReLU:
                _activation_function = [](double x) { return std::max(0.0, x); };
                break;
            case ActivationFunctionType::Tanh:
                _activation_function = [](double x) { return std::tanh(x); };
                break;
            default:
                _activation_function = [](double x) { return 1.0 / (1.0 + std::exp(-x)); }; // По умолчанию - сигмоид
        }
    }

std::vector<std::vector<std::vector<double>>> NeironNet::printWeights() const{
    return _weights;
}

double NeironNet::getRandomWeight(int seed) {
        if (seed != -1){
            std::srand(seed);
        }

        return (2.0 * std::rand() / RAND_MAX) - 1.0;
    }

bool NeironNet::shouldMutate(double mutation_chance) {
        double random_value = static_cast<double>(std::rand()) / RAND_MAX;
        return random_value < mutation_chance;
    }

double NeironNet::getRandomMutation(double max_mutation_amount) {
        double random_value = static_cast<double>(std::rand()) / RAND_MAX;
        return (2.0 * random_value - 1.0) * max_mutation_amount;
    }

NeironNet::~NeironNet()
{
    //dtor
}
