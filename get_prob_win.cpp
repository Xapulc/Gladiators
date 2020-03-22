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

int main(int argc, char *argv[]) {
    const double leftStrength = atof(argv[1]);
    const int cntLeft = atof(argv[2]);
    const double rightStrength = atof(argv[3]);
    const int cntRight = atof(argv[4]);

    auto winLeftProb = getWinProbabilityCircleUpdate(leftStrength, cntLeft, rightStrength, cntRight);
    std::cout << "Win left probability: " << winLeftProb << std::endl;
//     return min_n;
}
