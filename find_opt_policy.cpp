#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <fstream>

int playGame(std::vector<double> teamLeft, std::vector<double> teamRight, std::mt19937& gen) {
    auto cntLeft = teamLeft.size();
    auto cntRight = teamRight.size();
    std::bernoulli_distribution bernoulli_distribution(teamLeft[cntLeft-1] / 
                                                      (teamLeft[cntLeft-1] + teamRight[cntRight-1]));
    
//     for (auto el: teamLeft)
//         std::cout << el << " ";
//     std::cout << "Left end" << std::endl;
    
//     for (auto el: teamRight)
//         std::cout << el << " ";
//     std::cout << "Right end" << std::endl;
    
    if (bernoulli_distribution(gen)) {
        if (cntRight == 1)
            return 1;
        else {
            teamRight.pop_back();
            return playGame(teamLeft, teamRight, gen);
        }  
    } else {
        if (cntLeft == 1)
            return 0;
        else {
            teamLeft.pop_back();
            return playGame(teamLeft, teamRight, gen);
        }  
    }
}


int softMaxChoice(std::vector<double>& expQ, std::mt19937& gen) {
    std::discrete_distribution<int> distribution(expQ.begin(), expQ.end());
    return distribution(gen);
}

std::vector<double> findPolicyWeights(const double totalStrangeLeft, const double totalStrangeRight, 
                                      const int maxMembers, const int countTrials=10) {
    std::random_device rd;
    std::mt19937 gen(rd());
    double tao = 5;
    
    auto QLeft = std::vector<double>(maxMembers);
    auto QRight = std::vector<double>(maxMembers);
    std::fill(QLeft.begin(), QLeft.end(), 0);
    std::fill(QRight.begin(), QRight.end(), 0);
    
    auto expQLeft = std::vector<double>(maxMembers);
    auto expQRight = std::vector<double>(maxMembers);
    std::fill(expQLeft.begin(), expQLeft.end(), 1);
    std::fill(expQRight.begin(), expQRight.end(), 1);
    
    auto trialLeft = std::vector<double>(maxMembers);
    auto trialRight = std::vector<double>(maxMembers);
    std::fill(trialLeft.begin(), trialLeft.end(), 0);
    std::fill(trialRight.begin(), trialRight.end(), 0);
    
    for (int i = 0; i < countTrials; i++) {
        auto cntPolicyLeft = softMaxChoice(expQLeft, gen);
        auto cntPolicyRight = softMaxChoice(expQRight, gen);
        
        auto policyLeft = std::vector<double>(cntPolicyLeft+1);
        auto policyRight = std::vector<double>(cntPolicyRight+1);
        std::fill(policyLeft.begin(), policyLeft.end(), totalStrangeLeft / (cntPolicyLeft+1));
        std::fill(policyRight.begin(), policyRight.end(), totalStrangeRight / (cntPolicyRight+1));
        
        auto winner = 2 * (playGame(policyLeft, policyRight, gen) - 0.5);
        
        QLeft[cntPolicyLeft] = (trialLeft[cntPolicyLeft] * QLeft[cntPolicyLeft] + winner) / 
                                     (trialLeft[cntPolicyLeft] + 1);
        QRight[cntPolicyRight] = (trialRight[cntPolicyRight] * QRight[cntPolicyRight] - winner) / 
                                       (trialRight[cntPolicyRight] + 1);
        trialLeft[cntPolicyLeft]++;
        trialRight[cntPolicyRight]++;
        expQLeft[cntPolicyLeft] = std::exp(QLeft[cntPolicyLeft] / tao);
        expQRight[cntPolicyRight] = std::exp(QRight[cntPolicyRight] / tao);
    }
    QLeft.insert(QLeft.end(), QRight.begin(), QRight.end());
    QLeft.insert(QLeft.end(), trialLeft.begin(), trialLeft.end());
    QLeft.insert(QLeft.end(), trialRight.begin(), trialRight.end());
    return QLeft;
}


int main(int argc, char *argv[]) {
    const double totalStrangeLeft = 1;
    const double totalStrangeRight = atof(argv[1]);
    const int maxMembers = atof(argv[2]);
    
    auto res = (argc == 4) ? findPolicyWeights(totalStrangeLeft, totalStrangeRight, maxMembers, atof(argv[3])) 
                           : findPolicyWeights(totalStrangeLeft, totalStrangeRight, maxMembers, 100);
    
    std::ofstream outfile;
    outfile.open("results.csv");
    outfile << "Side";
    for (int i = 0; i < res.size() / 4; i++)
        outfile << "," << i+1;
    outfile << std::endl;
        
    std::cout << "QLeft" << std::endl;
    outfile << "QLeft";
    for (int i = 0; i < res.size() / 4; i++) {
        std::cout << res[i] / 2 + 0.5 << ", ";
        outfile << "," << res[i] / 2 + 0.5;
    }
    std::cout << std::endl;
    outfile << std::endl;
    
    std::cout << "QRight" << std::endl;
    outfile << "QRight";
    for (int i = res.size() / 4; i < res.size() / 2; i++) {
        std::cout << res[i] / 2 + 0.5 << ", ";
        outfile << "," << res[i] / 2 + 0.5;
    }
    std::cout << std::endl;
    outfile << std::endl;
    
    std::cout << "trialLeft" << std::endl;
    for (int i = res.size() / 2; i < 3 * res.size() / 4; i++)
        std::cout << res[i] << ", ";
    std::cout << std::endl;
    
    std::cout << "trialRight" << std::endl;
    for (int i = 3 * res.size() / 4; i < res.size(); i++)
        std::cout << res[i] << ", ";
    std::cout << std::endl;
}