#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <fstream> 


double getWinProbabilityCircleUpdate(double leftStrength, int cntLeft, double rightStrength, int cntRight) {
    std::vector<double> winProbs(cntLeft);
    std::fill(winProbs.begin(), winProbs.end(), 1.0);
    auto sumStrength = leftStrength + rightStrength;

    for (int r = 0; r < cntRight; r++) {
        winProbs[0] = leftStrength * winProbs[0] / sumStrength;
        for (int l = 1; l < cntLeft; l++)
            winProbs[l] = (rightStrength * winProbs[l-1] + leftStrength * winProbs[l]) / sumStrength;
    }
    return winProbs[cntLeft-1];
}

double getWinProbabilityCircle(std::vector<double> teamLeft, std::vector<double> teamRight) {
    auto cntLeft = teamLeft.size();
    auto cntRight = teamRight.size();
    std::vector<double> winProbs((cntLeft+1) * (cntRight+1));
    for (int l = 0; l < cntLeft+1; l++)
        winProbs[l] = 1;
    for (int r = 0; r < cntRight+1; r++)
        winProbs[r * (cntLeft+1)] = 0;
    for (int l = 1; l < cntLeft+1; l++) {
        for (int r = 1; r < cntRight+1; r++) {
            winProbs[l + r * (cntLeft+1)] = (teamLeft[l-1] * winProbs[l + (r-1) * (cntLeft+1)] +
                                        teamRight[r-1] * winProbs[l-1 + r * (cntLeft+1)]) /
                                                (teamLeft[l-1] + teamRight[r-1]);
        }
    }

    return winProbs[(cntLeft+1) * (cntRight+1) - 1];
}

double getWinProbabilityRecursion(std::vector<double> teamLeft, std::vector<double> teamRight) {
    auto cntLeft = teamLeft.size();
    auto cntRight = teamRight.size();
    
    if (cntRight == 0)
        return 1;
    else if (cntLeft == 0)
        return 0;
    else {
        auto gladLeft = teamLeft[cntLeft-1];
        auto gladRight = teamRight[cntRight-1];
        teamLeft.pop_back();
        auto rightWin = getWinProbabilityRecursion(teamLeft, teamRight);
        teamLeft.push_back(gladLeft);
        teamRight.pop_back();
        auto leftWin = getWinProbabilityRecursion(teamLeft, teamRight);
        return (gladLeft * leftWin + gladRight * rightWin) / (gladLeft + gladRight);
    }
}

int findMinN(double totalStrangeRight, const int optimalK, const int maxMembers, const int optimalTimes) {
    int curTimes = 0;
    int i;
    for (i = 0; (i < maxMembers) && (curTimes < optimalTimes); i++) {
        auto leftStrength = 1.0 / optimalK;
        auto rightStrength = totalStrangeRight / (i+1);
        auto cntLeft = optimalK;
        auto cntRight = i + 1;

        auto curMax = getWinProbabilityCircleUpdate(leftStrength, cntLeft, rightStrength, cntRight);
        bool optIsMax = true;

        for (int j = 0; (j < optimalK-1) && optIsMax; j++) {
            auto leftStrength = 1.0 / (j+1);
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


        for (int j = optimalK; (j < maxMembers) && optIsMax; j++) {
            auto leftStrength = 1.0 / (j+1);
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
        }
        else
            curTimes = 0;
    }
    return i - curTimes;
}

int findMinNArray(double totalStrangeRight, const int optimalK, const int maxMembers, const int optimalTimes) {
    int curTimes = 0;
    int i;
    for (i = 0; (i < maxMembers) && (curTimes < optimalTimes); i++) {
        auto policyLeft = std::vector<double>(optimalK);
        auto policyRight = std::vector<double>(i+1);
        std::fill(policyLeft.begin(), policyLeft.end(), 1.0 / optimalK);
        std::fill(policyRight.begin(), policyRight.end(), totalStrangeRight / (i+1));
        
        auto curMax = getWinProbabilityCircle(policyLeft, policyRight);
        bool optIsMax = true;
        
        for (int j = 0; (j < optimalK-1) && optIsMax; j++) {
            auto policyLeft = std::vector<double>(j+1);
            auto policyRight = std::vector<double>(i+1);
            std::fill(policyLeft.begin(), policyLeft.end(), 1.0 / (j+1));
            std::fill(policyRight.begin(), policyRight.end(), totalStrangeRight / (i+1));
            
            auto winProb = getWinProbabilityCircle(policyLeft, policyRight);
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
            
        
        for (int j = optimalK; (j < maxMembers) && optIsMax; j++) {
            auto policyLeft = std::vector<double>(j+1);
            auto policyRight = std::vector<double>(i+1);
            std::fill(policyLeft.begin(), policyLeft.end(), 1.0 / (j+1));
            std::fill(policyRight.begin(), policyRight.end(), totalStrangeRight / (i+1));
            
            auto winProb = getWinProbabilityCircle(policyLeft, policyRight);
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
        }
        else
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
}
