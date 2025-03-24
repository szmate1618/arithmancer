#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>


class Problem {
protected:
    std::string name;
    double lifetime;
    double errorMargin;
    bool solved;

public:
    Problem(const std::string& name, double lifetime, double errorMargin)
        : name(name), lifetime(lifetime), errorMargin(errorMargin), solved(false) {
    }

    virtual ~Problem() = default;

    void Update(double seconds) {
        lifetime -= seconds;
    }

    virtual double GetResult() const = 0;

    virtual std::string GetProblemStatement() const = 0;

    void Guess(const std::string& guess) {
        try {
            double guessedValue = std::stod(guess);
            if (std::abs(guessedValue - GetResult()) <= errorMargin) {
                solved = true;
            }
        }
        catch (const std::invalid_argument&) {
            //std::cerr << "Invalid guess input. Not a number." << std::endl;
        }
    }

    bool HasTimedOut() const {
        return lifetime <= 0;
    }

    bool IsSolved() const {
        return solved;
    }

	std::string GetName() const {
		return name;
	}

	double GetErrorMargin() const {
		return errorMargin;
	}
};

class AdditionProblem : public Problem
{
public:
    AdditionProblem(double lifetime = 10, double errorMargin = 0)
        : Problem("Additive arachnid", lifetime, errorMargin) {
		numbers.push_back(rand() % 10);
		numbers.push_back(rand() % 10);
    }

    virtual ~AdditionProblem() = default;

    double GetResult() const override {
        int sum = 0;
        for (auto& n : numbers)
            sum += n;
        return sum;
    }

    std::string GetProblemStatement() const override {
        std::string text = std::to_string(numbers.at(0));
        for (size_t i = 1; i < numbers.size(); i++)
        {
			text += " + " + std::to_string(numbers.at(i));
        }
        return text;
    }
private:
	std::vector<int> numbers;
};
