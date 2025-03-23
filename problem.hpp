#pragma once

#include <iostream>
#include <vector>
#include <string>


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

    virtual double Result() const = 0;

    void Guess(const std::string& guess) {
        try {
            double guessedValue = std::stod(guess);
            if (std::abs(guessedValue - Result()) <= errorMargin) {
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
