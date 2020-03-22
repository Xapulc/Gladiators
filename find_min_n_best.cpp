#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <iostream>
#include <string.h>


double getWinProbabilityCircleUpdate(double leftStrength, int cntLeft, double rightStrength, int cntRight) {
    std::vector<double> winProbs(cntLeft);
    std::fill(winProbs.begin(), winProbs.end(), 1.0);
    auto sumStrength = leftStrength + rightStrength;

    for (int r = 0; r < cntRight; r++) {
        winProbs[0] = leftStrength * winProbs[0] / sumStrength;
        for (int l = 1; l < cntLeft; l++)
            winProbs[l] = (rightStrength * winProbs[l - 1] + leftStrength * winProbs[l]) / sumStrength;
    }
    return winProbs[cntLeft - 1];
}

int findMinN(double totalStrangeRight, int optimalK, int maxMembers, int optimalTimes) {
    std::cout << totalStrangeRight << std::endl;
    std::cout << optimalK << std::endl;
    std::cout << maxMembers << std::endl;
    std::cout << optimalTimes << std::endl;
    int curTimes = 0;
    int i;
    for (i = optimalK; (i < maxMembers) && (curTimes < optimalTimes); i++) {
        auto leftStrength = 1.0 / optimalK;
        auto rightStrength = totalStrangeRight / (i + 1);
        auto cntLeft = optimalK;
        auto cntRight = i + 1;

        auto curMax = getWinProbabilityCircleUpdate(leftStrength, cntLeft, rightStrength, cntRight);
        bool optIsMax = true;

        for (int j = 0; (j < optimalK - 1) && optIsMax; j++) {
            auto leftStrength = 1.0 / (j + 1);
            auto cntLeft = j + 1;

            auto winProb = getWinProbabilityCircleUpdate(leftStrength, cntLeft, rightStrength, cntRight);
            if (curMax < winProb) {
                std::cout << j+1 << " gladiators is better then " << optimalK << std::endl;
                std::cout << "when enemy has " << i+1 << " gladiators" << std::endl;
                std::cout << winProb << " > " << curMax << std::endl;
                optIsMax = false;
            }
        }
        if (!optIsMax) {
            curTimes = 0;
            continue;
        }


        for (int j = optimalK; (j < cntRight) && optIsMax; j++) {
            auto leftStrength = 1.0 / (j + 1);
            auto cntLeft = j + 1;

            auto winProb = getWinProbabilityCircleUpdate(leftStrength, cntLeft, rightStrength, cntRight);
            if (curMax < winProb) {
                std::cout << j+1 << " gladiators is better then " << optimalK << std::endl;
                std::cout << "when enemy has " << i+1 << " gladiators" << std::endl;
                std::cout << winProb << " > " << curMax << std::endl;
                optIsMax = false;
            }
        }
        if (optIsMax) {
            curTimes += 1;
            std::cout << optimalK << " gladiators is the best" << std::endl;
        } else
            curTimes = 0;
    }
    return i - curTimes;
}

int main(int argc, char *argv[]) {
    const double totalStrangeRight = atof(argv[1]);
    const int optimalK = atof(argv[2]);
    const int maxMembers = atof(argv[3]);
    const int optimalTimes = atof(argv[4]);

    auto min_n = findMinN(totalStrangeRight, optimalK, maxMembers, optimalTimes);
    std::cout << "min n: " << min_n << std::endl;
//     return min_n;
}
