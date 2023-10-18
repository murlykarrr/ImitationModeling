#include <iostream>
#include <random>
#include <cmath>
#include <chrono>
using namespace std;
using namespace chrono;


// ��������� ��������� ����� ����� �������� ������������ �����
double* Random(int n) {
    double* randomNumbers = new double[n];

    // �������� ������� ����� � �������������
    auto time = high_resolution_clock::now().time_since_epoch();
    unsigned long long milliseconds = duration_cast<std::chrono::milliseconds>(time).count();

    // ��������� ��������� ������������� ������
    unsigned long long m = 4294967296; // ������
    unsigned long long a = 1664525;    // ���������
    unsigned long long c = 1013904223; // ����������

    // ���������� ������ ��������� �����
    unsigned long long randomNumber = (a * milliseconds + c) % m;
    randomNumbers[0] = static_cast<double>(randomNumber) / m;

    // ���������� ��������� ��������� �����
    for (int i = 1; i < n; i++) {
        randomNumber = (a * randomNumber + c) % m;
        randomNumbers[i] = static_cast<double>(randomNumber) / m;
    }

    return randomNumbers; // ���������� ��������������� ������ �����
}

// ��������� ��������� ����� �� ����������� ���������� C++ mt19937
double* RandomBiblioteca(int n) {
    mt19937 rng(time(NULL)); // ���������� ���������� ��������� ����� � ������� �������� �������
    uniform_real_distribution<float> distribution(0.0, 1.0); // ������� ������������ ������������� � ��������� �� 0 �� 1
    double* randomNumbers = new double[n];

    cout << endl;

    for (int i = 0; i < n; i++) {
        randomNumbers[i] = distribution(rng); // ��������� ������ ���������� ����� � ���������� ��� � �������
    }

    return randomNumbers; // ���������� ��������������� ������ �����
}

void generateHistogram(double* randomNumbers, int n) {
    const int intervalCount = 10; // ����� ����������
    const double intervalSize = 1.0 / intervalCount; // ������� ����������

    int interval[intervalCount] = { 0 }; // ������ ��� �������� ����� ��������� ����� � ���������

    // ������� ����� ��������� � ���������
    for (int i = 0; i < n; i++) {
        int index = static_cast<int>(randomNumbers[i] / intervalSize);
        if (index >= intervalCount) {
            index = intervalCount - 1;
        }
        interval[index]++;
    }

    // ����� �����������
    for (int i = 0; i < intervalCount; i++) {
        int hits = 0;
        cout << "[" << (i * intervalSize) << ", " << ((i + 1) * intervalSize) << "]: ";
        for (int j = 0; j < interval[i]; j++) {
            hits++; // ���������� ����� � ���������
            if ((j > 0) && (j % 10 == 0)) {
                cout << "*";
            }
        }
        cout << " " << hits << endl;
      
    }
    

    // ������ �������� ���������� ��������
    double expected_value = static_cast<double>(n) / static_cast<double>(intervalCount);

    // ������ ��-�������
    double chi_square = 0;
    for (int i = 0; i < intervalCount; i++) {
        double difference = interval[i] - expected_value;//���-�� ��������� � �������� � ��������� ����������
        chi_square += (difference * difference) / expected_value;
    }
    cout << " �� ������� = " << chi_square << endl << endl;
}


int main() {
    setlocale(LC_ALL, "Russian"); // ��������� ������ ��� ������ ������� ����� � �������
    const int Numbers = 3000; // ���������� ������������ �����

    // ��������� ����� � ������� ����������� �������
    double* myNumbers = Random(Numbers);

    // ��������� ����� � ������� ������������� ����������
    double* libraryNumbers = RandomBiblioteca(Numbers);

    cout << " ������������������ ����� ��������������� �������� ������������ ������� " << endl << endl;
    generateHistogram(myNumbers, Numbers);
    cout << " ������������������ ����� ��������������� �� ���������� " << endl << endl;
    generateHistogram(libraryNumbers, Numbers);
    cout << "�������� ������� �������� ����������� ����� ������������� ��-������� �������� �������, ����� ������������, ��� ������� ���������� ��� ����� ������ 0.05, � ����� �������� ������� 9, ����� ������� ���������� ������ ���� < ��� = 16,9 " << endl << endl;
    // ������������ ����������� ����� � ������
    delete[] myNumbers;
    delete[] libraryNumbers;

    return 0;
}