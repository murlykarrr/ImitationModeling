#include <iostream>
#include <random>
#include <cmath>
#include <chrono>
using namespace std;
using namespace chrono;


// Генератор случайных чисел через линейный конгруэнтный метод
double* Random(int n) {
    double* randomNumbers = new double[n];

    // Получаем текущее время в миллисекундах
    auto time = high_resolution_clock::now().time_since_epoch();
    unsigned long long milliseconds = duration_cast<std::chrono::milliseconds>(time).count();

    // Параметры линейного конгруэнтного метода
    unsigned long long m = 4294967296; // Модуль
    unsigned long long a = 1664525;    // Множитель
    unsigned long long c = 1013904223; // Приращение

    // Генерируем первое случайное число
    unsigned long long randomNumber = (a * milliseconds + c) % m;
    randomNumbers[0] = static_cast<double>(randomNumber) / m;

    // Генерируем остальные случайные числа
    for (int i = 1; i < n; i++) {
        randomNumber = (a * randomNumber + c) % m;
        randomNumbers[i] = static_cast<double>(randomNumber) / m;
    }

    return randomNumbers; // Возвращаем сгенерированный массив чисел
}

// Генератор случайных чисел из стандартной библиотеки C++ mt19937
double* RandomBiblioteca(int n) {
    mt19937 rng(time(NULL)); // Объявление генератора случайных чисел с помощью текущего времени
    uniform_real_distribution<float> distribution(0.0, 1.0); // Задание равномерного распределения в диапазоне от 0 до 1
    double* randomNumbers = new double[n];

    cout << endl;

    for (int i = 0; i < n; i++) {
        randomNumbers[i] = distribution(rng); // Генерация одного случайного числа и сохранение его в массиве
    }

    return randomNumbers; // Возвращаем сгенерированный массив чисел
}

void generateHistogram(double* randomNumbers, int n) {
    const int intervalCount = 10; // Число интервалов
    const double intervalSize = 1.0 / intervalCount; // Размеры интервалов

    int interval[intervalCount] = { 0 }; // Массив для хранения числа вхождений чисел в интервалы

    // Подсчет числа вхождений в интервалы
    for (int i = 0; i < n; i++) {
        int index = static_cast<int>(randomNumbers[i] / intervalSize);
        if (index >= intervalCount) {
            index = intervalCount - 1;
        }
        interval[index]++;
    }

    // Вывод гистограммы
    for (int i = 0; i < intervalCount; i++) {
        int hits = 0;
        cout << "[" << (i * intervalSize) << ", " << ((i + 1) * intervalSize) << "]: ";
        for (int j = 0; j < interval[i]; j++) {
            hits++; // Количество чисел в диапазоне
            if ((j > 0) && (j % 10 == 0)) {
                cout << "*";
            }
        }
        cout << " " << hits << endl;
      
    }
    

    // Расчёт среднего ожидаемого значения
    double expected_value = static_cast<double>(n) / static_cast<double>(intervalCount);

    // Расчет хи-квадрат
    double chi_square = 0;
    for (int i = 0; i < intervalCount; i++) {
        double difference = interval[i] - expected_value;//кол-во вхождений в интервал и ожидаемым вхождением
        chi_square += (difference * difference) / expected_value;
    }
    cout << " Хи квадрат = " << chi_square << endl << endl;
}


int main() {
    setlocale(LC_ALL, "Russian"); // Установка локали для вывода руского языка в консоль
    const int Numbers = 3000; // Количество генерируемых чисел

    // Генерация чисел с помощью собственной функции
    double* myNumbers = Random(Numbers);

    // Генерация чисел с помощью библиотечного генератора
    double* libraryNumbers = RandomBiblioteca(Numbers);

    cout << " Последовательность чисел сгенерированных линейным конгруэнтным методом " << endl << endl;
    generateHistogram(myNumbers, Numbers);
    cout << " Последовательность чисел сгенерированная из библиотеки " << endl << endl;
    generateHistogram(libraryNumbers, Numbers);
    cout << "Согласно таблицы значений критических точек распределения хи-квадрат критерии Пирсона, будем предпологать, что уровень значимости для нашей задачи 0.05, а число степеней свободы 9, тогда уровень значимости должен быть < или = 16,9 " << endl << endl;
    // Освобождение выделенного места в памяти
    delete[] myNumbers;
    delete[] libraryNumbers;

    return 0;
}