#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

// хранение вопроса, вариантов ответов и правильного ответа
struct Question {
    string question;
    vector<string> options;
    int correctOptionIndex;
};

// Функция для вывода разделительной линии
void printLine(int length = 50) {
    cout << "\n";
    for (int i = 0; i < length; i++) cout << "=";
    cout << "\n";
}

// имитации загрузки с анимацией точек
void printLoading(const string& message) {
    cout << message;
    for (int i = 0; i < 3; i++) {
        this_thread::sleep_for(chrono::milliseconds(500)); // Задержка в 500 мс
        cout << ".";
    }
    cout << "\n";
}

int main() {
    int score = 0; // Подсчет очков
    setlocale(LC_ALL, "Russian"); 

    // Вывод приветственного сообщения
    printLine();
    cout << "\tДобро пожаловать на викторину по C++!\n";
    cout << "\t        Выполнила 1 команда.\n";
    printLine();

    // типо загрузка вопросов
    printLoading("Загрузка вопросов");

    ifstream file("quiz.txt"); // Открываем файл с вопросами
    if (!file) {
        cerr << "Не удалось открыть файл с вопросами.\n";
        return 1;
    }

    vector<Question> questions;
    string line;

    // Считываем вопросы и ответы из файла
    while (getline(file, line)) {
        size_t delimiterPos = line.find('|'); // Поиск разделителя вопроса
        if (delimiterPos != string::npos) {
            string questionText = line.substr(0, delimiterPos);
            string optionsAndAnswer = line.substr(delimiterPos + 1);

            Question q;
            q.question = questionText;

            size_t optionsDelimiterPos = optionsAndAnswer.find('|'); // Поиск разделителя вариантов ответа
            if (optionsDelimiterPos != string::npos) {
                string optionsStr = optionsAndAnswer.substr(0, optionsDelimiterPos);
                q.correctOptionIndex = stoi(optionsAndAnswer.substr(optionsDelimiterPos + 1)); // Получаем правильный ответ

                size_t start = 0;
                size_t end = optionsStr.find(';');
                while (end != string::npos) {
                    q.options.push_back(optionsStr.substr(start, end - start)); // Добавляем вариант ответа
                    start = end + 1;
                    end = optionsStr.find(';', start);
                }
                q.options.push_back(optionsStr.substr(start)); // Добавляем последний вариант ответа

                questions.push_back(q);
            }
        }
    }

    file.close(); // Закрываем файл

    if (questions.empty()) {
        cerr << "Файл с вопросами имеет некорректный формат.\n";
        return 1;
    }

    // Цикл прохождения викторины
    for (size_t i = 0; i < questions.size(); ++i) {
        printLine();
        cout << "Вопрос " << i + 1 << ": " << questions[i].question << "\n";
        printLine();

        // Вывод вариантов ответов
        for (size_t j = 0; j < questions[i].options.size(); ++j) {
            cout << "[" << j + 1 << "] " << questions[i].options[j] << "\n";
        }

        int userChoice;
        cout << "\nВведите номер ответа: ";
        cin >> userChoice;

        // Проверка правильности ответа
        if (userChoice > 0 && userChoice <= static_cast<int>(questions[i].options.size())) {
            if (static_cast<size_t>(userChoice - 1) == questions[i].correctOptionIndex) {
                cout << "\n:) Правильно!\n";
                score++;
            }
            else {
                cout << "\nX Неправильно! Правильный ответ: "
                    << questions[i].options[questions[i].correctOptionIndex] << "\n";
            }
        }
        else {
            cout << "\n Некорректный ввод! Попробуйте снова.\n";
            --i; // Повторяем вопрос
        }
    }

    // Вывод итогового результата
    printLine();
    cout << "Вы набрали " << score << " из " << questions.size() << " очков!\n";
    cout << "Поздравляем! Держи твой приз чупа-чупс! \n";
    printLine();
    return 0;
}
