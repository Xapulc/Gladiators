#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <iostream>
#include <string.h>


double getWinProbabilityRecursion(std::vector<double> teamLeft, std::vector<double> teamRight,
                                  const double decayRate) {
    auto cntLeft = teamLeft.size();
    auto cntRight = teamRight.size();

    if (cntRight == 0) {
        return 1;
    }

    else if (cntLeft == 0) {
        return 0;
    }

    else {
        auto gladLeft = teamLeft[cntLeft-1];
        auto gladRight = teamRight[cntRight-1];

        teamRight[cntRight-1] *= decayRate;
        teamLeft.pop_back();
        auto rightWin = getWinProbabilityRecursion(teamLeft, teamRight, decayRate);
        teamLeft.push_back(gladLeft * decayRate);
        teamRight.pop_back();
        auto leftWin = getWinProbabilityRecursion(teamLeft, teamRight, decayRate);
        return (gladLeft * leftWin + gladRight * rightWin) / (gladLeft + gladRight);
    }
}

int main(int argc, char *argv[]) {
    const double leftStrength = atof(argv[1]);
    const int cntLeft = atof(argv[2]);
    const double rightStrength = atof(argv[3]);
    const int cntRight = atof(argv[4]);
    const double decayRate = (argc > 5) ? atof(argv[5]) : 1;

    auto policyLeft = std::vector<double>(cntLeft);
    auto policyRight = std::vector<double>(cntRight);
    std::fill(policyLeft.begin(), policyLeft.end(), leftStrength);
    std::fill(policyRight.begin(), policyRight.end(), rightStrength);

    auto winProb = getWinProbabilityRecursion(policyLeft, policyRight, decayRate);
    std::cout << "Win left probability: " << winProb << std::endl;
//     return min_n;
}
