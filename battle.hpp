#pragma once

#include <vector>
#include <memory>

#include "problem.hpp"


class Battle {
private:
    std::vector<std::unique_ptr<Problem>> problems;

public:
    void AddProblem(std::unique_ptr<Problem> problem) {
        problems.push_back(std::move(problem));
    }

    void Update(double seconds) {
        for (auto& problem : problems) {
            problem->Update(seconds);
        }
    }
};
