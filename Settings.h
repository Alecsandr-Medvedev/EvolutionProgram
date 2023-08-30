#include <iostream>

#include "window.h"
#include "dynamicdata.h"
#include <chrono>
#include <thread>
#include <time.h>
#include "neironnet.h"
#include "field.h"
#include <stdlib.h>




std::srand(time(NULL));
Color color(40, 40, 40);
Color color2(255, 255, 40);
int countFrames = 100;
int countTicks = 20;
int FPS = 1000 / 100;
int TPS = 1000 / 20;
std::vector<int> StandartNeiron1 = {7, 8, 8, 1}, StandartNeiron2 = {2, 4, 4, 2};
double kEnergyGrow = 0.1, kMutate = 2, kPhotosintes = 2.5, kPredotor = 0.1;
int kFind = 10, kEnemy = 20;
int WorldWidth = 700, WorldHeight = 700;
Rect WorldRect(0, 0, WorldWidth, WorldHeight);

DynamicData dd;

zone BaseSettings(100, 100, 100, WorldRect, dd);

Field field(WorldWidth, WorldHeight, BaseSettings, dd);

double getRandNum(double minNum, double maxNum, double step){
    double num = std::rand();
    num = int num % int ((maxNum - minNum) / step);
    num = num * step;
    num = num - minNum;
    return num;
}
//int kNearbly = 3;

//#include <iostream>
//#include <vector>
//#include <cmath>
//#include <cassert>
//#include <ctime>
//#include <cstdlib>
//#include <functional>
//
//
//class NeuronNet {
//public:
//    // Перечисление для определения доступных функций активации
//    enum class ActivationFunctionType {
//        Sigmoid,
//        ReLU,
//        Tanh
//    };
//
//    NeuronNet(const std::vector<int>& layer_sizes, ActivationFunctionType activation_function_type = ActivationFunctionType::Sigmoid, const std::vector<std::vector<std::vector<double>>>& initial_weights = {}) {
//        assert(layer_sizes.size() >= 2);
//        assert(layer_sizes.size() == initial_weights.size() + 1 || !initial_weights.size());
//
//        std::srand(std::time(0));
//
//        // Устанавливаем функцию активации в соответствии с выбранным типом
//        setActivationFunction(activation_function_type);
//
//        // Инициализируем веса и смещения для каждого слоя
//        for (size_t i = 1; i < layer_sizes.size(); ++i) {
//            int input_size = layer_sizes[i - 1];
//            int output_size = layer_sizes[i];
//            weights_.emplace_back(std::vector<std::vector<double>>(output_size, std::vector<double>(input_size, 0.0)));
//
//            // Если переданы инициализационные веса, используем их
//            if (i - 1 < initial_weights.size()) {
//                weights_[i - 1] = initial_weights[i - 1];
//            }
//            // Иначе генерируем случайные веса
//            else {
//                for (int j = 0; j < output_size; ++j) {
//                    for (int k = 0; k < input_size; ++k) {
//                        weights_[i - 1][j][k] = getRandomWeight();
//                    }
//                }
//            }
//
//            biases_.emplace_back(std::vector<double>(output_size, 0.0));
//        }
//
//        // Инициализируем активации для каждого слоя
//        for (size_t i = 0; i < layer_sizes.size(); ++i) {
//            activations_.emplace_back(std::vector<double>(layer_sizes[i], 0.0));
//        }
//    }
//
//    // Функция активации
//    double activationFunction(double x) {
//        return activation_function_(x);
//    }
//
//    // Прямое распространение (forward propagation)
//    std::vector<double> forward(const std::vector<double>& input) {
//        assert(input.size() == activations_[0].size());
//
//        // Копируем вход в активации входного слоя
//        for (size_t i = 0; i < input.size(); ++i) {
//            activations_[0][i] = input[i];
//        }
//
//        // Вычисляем активации для каждого слоя
//        for (size_t i = 1; i < activations_.size(); ++i) {
//            const auto& prev_layer = activations_[i - 1];
//            auto& current_layer = activations_[i];
//            auto& current_weights = weights_[i - 1];
//            auto& current_biases = biases_[i - 1];
//
//            for (size_t j = 0; j < current_layer.size(); ++j) {
//                double sum = 0.0;
//                for (size_t k = 0; k < prev_layer.size(); ++k) {
//                    sum += current_weights[j][k] * prev_layer[k];
//                }
//                sum += current_biases[j];
//                current_layer[j] = activationFunction(sum);
//            }
//        }
//
//        return activations_.back();
//    }
//
//    // Мутация весов с заданным шансом
//    void Mutate(double mutation_chance, double max_mutation_amount) {
//        for (auto& weight_matrix : weights_) {
//            for (auto& row : weight_matrix) {
//                for (double& weight : row) {
//                    if (shouldMutate(mutation_chance)) {
//                        weight += getRandomMutation(max_mutation_amount);
//                    }
//                }
//            }
//        }
//    }
//
//    // Установка функции активации
//    void setActivationFunction(ActivationFunctionType activation_function_type) {
//        switch (activation_function_type) {
//            case ActivationFunctionType::Sigmoid:
//                activation_function_ = [](double x) { return 1.0 / (1.0 + std::exp(-x)); };
//                break;
//            case ActivationFunctionType::ReLU:
//                activation_function_ = [](double x) { return std::max(0.0, x); };
//                break;
//            case ActivationFunctionType::Tanh:
//                activation_function_ = [](double x) { return std::tanh(x); };
//                break;
//            default:
//                activation_function_ = [](double x) { return 1.0 / (1.0 + std::exp(-x)); }; // По умолчанию - сигмоид
//        }
//    }
//
//    std::vector<std::vector<std::vector<double>>> printWeights() const {
//        std::cout << "Weights of the neural network:" << std::endl;
//        for (size_t layer = 0; layer < weights_.size(); ++layer) {
//            std::cout << "Layer " << layer + 1 << ":" << std::endl;
//            const auto& weight_matrix = weights_[layer];
//            for (size_t neuron = 0; neuron < weight_matrix.size(); ++neuron) {
//                const auto& neuron_weights = weight_matrix[neuron];
//                std::cout << "  Neuron " << neuron + 1 << " weights: ";
//                for (size_t i = 0; i < neuron_weights.size(); ++i) {
//                    std::cout << neuron_weights[i] << " ";
//                }
//                std::cout << std::endl;
//            }
//            std::cout << std::endl;
//        }
//    }
//
//private:
//    std::vector<std::vector<std::vector<double>>> weights_;
//    std::vector<std::vector<double>> biases_;
//    std::vector<std::vector<double>> activations_;
//
//    std::function<double(double)> activation_function_;
//
//    double getRandomWeight() {
//        return (2.0 * std::rand() / RAND_MAX) - 1.0;
//    }
//
//    bool shouldMutate(double mutation_chance) {
//        double random_value = static_cast<double>(std::rand()) / RAND_MAX;
//        return random_value < mutation_chance;
//    }
//
//    double getRandomMutation(double max_mutation_amount) {
//        double random_value = static_cast<double>(std::rand()) / RAND_MAX;
//        return (2.0 * random_value - 1.0) * max_mutation_amount;
//    }
//};
//
//int main() {
//    // Пример создания нейронной сети с 2 входами, 3 нейронами в скрытом слое и 1 выходом
//    std::vector<int> layer_sizes = {5, 4, 3, 4, 2};
//    NeuronNet neuralNet(layer_sizes, NeuronNet::ActivationFunctionType::Sigmoid);
//
//    // Пример прямого распространения (forward propagation) с некоторым входом
//    std::vector<double> input = {0.5, 0.8, 12, 1.23, -3.4};
//    std::vector<double> output = neuralNet.forward(input);
//
//    double c = 0;
//    while(c < 5){
//            c = 0;
//            neuralNet.Mutate(1, 0.1);
//
//        // Прямое распространение после мутации
//        output = neuralNet.forward(input);
//
//        // Выводим результат после мутации
//        std::cout << "Output: ";
//        for (double value : output) {
//            std::cout << value << " ";
//            c +=value;
//        }
//        std::cout << std::endl;
//        std::this_thread::sleep_for(std::chrono::milliseconds(500));
//
//    }
//
//    return 0;
//
//}

