#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

struct Question {
    string question;
    vector<string> options;
    int correctOptionIndex;
};

int main() {
    int score = 0; // Переменная для счета очков
    setlocale(LC_ALL, "Russian");

    // Приветствие
    cout << "Добро пожаловать на викторину по C++! Выполнила 1 команда.\n";

    // Файл с вопросами и ответами
    ifstream file("quiz.txt");
    if (!file) {
        cerr << "Не удалось открыть файл с вопросами.\n";
        return 1;
    }

    vector<Question> questions;
    string line;

    // Считывание вопросов и ответов из файла
    while (getline(file, line)) {
        size_t delimiterPos = line.find('|');
        if (delimiterPos != string::npos) {
            string questionText = line.substr(0, delimiterPos);
            string optionsAndAnswer = line.substr(delimiterPos + 1);

            Question q;
            q.question = questionText;

            // Разделение вариантов ответов и правильного ответа
            size_t optionsDelimiterPos = optionsAndAnswer.find('|');
            if (optionsDelimiterPos != string::npos) {
                string optionsStr = optionsAndAnswer.substr(0, optionsDelimiterPos);
                q.correctOptionIndex = stoi(optionsAndAnswer.substr(optionsDelimiterPos + 1));

                // Разделение вариантов ответов
                size_t start = 0;
                size_t end = optionsStr.find(';');
                while (end != string::npos) {
                    q.options.push_back(optionsStr.substr(start, end - start));
                    start = end + 1;
                    end = optionsStr.find(';', start);
                }
                q.options.push_back(optionsStr.substr(start)); // Последний вариант ответа

                questions.push_back(q);
            }
        }
    }

    file.close();

    if (questions.empty()) {
        cerr << "Файл с вопросами имеет некорректный формат.\n";
        return 1;
    }

    // Прохождение викторины
    for (size_t i = 0; i < questions.size(); ++i) {
        cout << questions[i].question << "\n";
        for (size_t j = 0; j < questions[i].options.size(); ++j) {
            cout << j + 1 << ". " << questions[i].options[j] << "\n";
        }

        int userChoice;
        cin >> userChoice;
        if (userChoice > 0 && userChoice <= static_cast<int>(questions[i].options.size())) {
            if (static_cast<size_t>(userChoice - 1) == questions[i].correctOptionIndex) {
                cout << "Правильно!\n";
                score++;
            }
            else {
                cout << "Неправильно, правильный ответ: "
                    << questions[i].options[questions[i].correctOptionIndex] << "\n";
            }
        }
        else {
            cout << "Некорректный ввод. Попробуйте снова.\n";
            --i; // Повторяем вопрос
        }
    }

    // Итог
    cout << "Вы набрали " << score << " из " << questions.size() << " очков. Держи твой приз чупа-чупс.))))\n";
    return 0;
}