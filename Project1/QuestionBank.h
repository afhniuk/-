/// @file QuestionBank.h
/// @brief 题库管理类声明

#ifndef QUESTIONBANK_H
#define QUESTIONBANK_H

#include <string>
#include <vector>

struct Question {
    int id;
    int aValue;
    std::vector<std::string> gridData;
    std::string description;

    std::string toString() const;
};

class QuestionBank {
private:
    std::string questionsFilePath;
    std::vector<Question> questions;

public:
    explicit QuestionBank(const std::string& dataDir);

    void loadFromFile();
    void saveToFile();

    const std::vector<Question>& getQuestions() const { return questions; }

    void addQuestion(const Question& q);

    const Question* getQuestionById(int id) const;

    void showQuestionList() const;

    void generateBuiltinQuestions();

    bool isEmpty() const { return questions.empty(); }

private:
    void ensureDataDir(const std::string& dataDir);
};

#endif // QUESTIONBANK_H
